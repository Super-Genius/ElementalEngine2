///==========================================================================
/// \file	CHeightfieldPhysicsObject.cpp
/// \brief	Physics object for heightfields
/// \date	3/29/2007
/// \author	Kyle Swaim
///         Copyright (c)  2005-2008 Signature Devices, Inc.
///
///         This code is redistributable under the terms of the EE License.
///
///         This code is distributed without warranty or implied warranty of
///			merchantability or fitness for a particular purpose. See the 
///			EE License for more details.
///
///         You should have received a copy of the EE License along with this
///			code; If not, write to Signature Devices, Inc.,
///			3200 Bridge Parkway Suite 102, Redwood City, CA 94086 USA.
///==========================================================================

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ------------- Register Object ---------------- //
REGISTER_COMPONENT(CHeightfieldPhysicsObject);

CHeightfieldPhysicsObject::CHeightfieldPhysicsObject( IHashString *parentName, IHashString *name, bool addToHier ) : 
	CPhysicsObject(parentName, name, addToHier)
{
	// Everything needed is in the CPhysicsObject constructor
}

CHeightfieldPhysicsObject::~CHeightfieldPhysicsObject()
{
	DeInit();

	if( m_bAddToHierarchy )
	{
		RemoveFromHierarchy();
	}
}

IComponent* CHeightfieldPhysicsObject::Create( int nArgs, va_list argptr )
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg( argptr, IHashString* );
	parentName = va_arg( argptr, IHashString* );
	try
	{
		if( nArgs == 2 )
		{
			self = new CHeightfieldPhysicsObject( parentName, name, true );
		}
		else if( nArgs == 3 )
		{
			bAddToHier = va_arg( argptr, bool );
			self = new CHeightfieldPhysicsObject( parentName, name, bAddToHier );
		}
		else
		{
			EngineGetToolBox()->Log( LOGERROR, _T("Invalid # - %d passed for argument to Create Heightfield Physics Object\n"), nArgs );
			return NULL;
		}
	}
	catch(...)
	{
		EngineGetToolBox()->Log( LOGERROR, _T("CHeightfieldPhysicsObject: Failed to create CHeightfieldPhysicsObject!\n") );
		return NULL;
	}

	return self;
}

void CHeightfieldPhysicsObject::Init()
{
	if( m_PhysicsActor != NULL )
	{
		ReleaseModel();
	}

	m_hsParentType.Init( _T("CTerrainSector") );

	// Build Terrain
	BuildHeightfield();

	// Transform is set in BuildTriggerActor()
	m_bTransformIsDirty = false;
}

bool CHeightfieldPhysicsObject::Update()
{
	if( m_bTransformIsDirty )
	{
		SetTransform();
		m_bTransformIsDirty = false;
	}

	return true;
}

void CHeightfieldPhysicsObject::DeInit()
{
	CPhysicsObject::DeInit();
}

void CHeightfieldPhysicsObject::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		// Read in the collision group
		int group;
		ar.Read( group );
		m_CollisionGroup = (COLLISIONGROUP)group;
		// Read in the contact group
		ar.Read( m_ContactGroup );
	}
	else
	{
		ar.Write( m_CollisionGroup, _T("collisiongroup") );
		ar.Write( m_ContactGroup, _T("contactgroup") );
	}
}

IHashString* CHeightfieldPhysicsObject::GetComponentType()
{
	static CHashString cpo( _T("CHeightfieldPhysicsObject") );
	return &cpo;
}

bool CHeightfieldPhysicsObject::IsKindOf(IHashString *compType)
{
	static CHashString cpo( _T("CPhysicsObject") );
	if( compType->GetUniqueID() == cpo.GetUniqueID() ) 
	{
		return true;
	}
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

void CHeightfieldPhysicsObject::BuildHeightfield()
{
	const float fFixedPointScaler = 8.0f;

	Matrix4x4 tmp;
	GetParentTransform( tmp );
	NxMat34Adapter adapter(tmp);

	// get the terrain descriptor
	TERRAINDESCRIPTOR terrainDescriptor;
	static DWORD msgHash_GetTerrainDescriptor = CHashString(_T("GetTerrainDescriptor")).GetUniqueID();
	if( m_ToolBox->SendMessage(msgHash_GetTerrainDescriptor, sizeof(TERRAINDESCRIPTOR), &terrainDescriptor ) != MSG_HANDLED )
	{
		m_ToolBox->Log( LOGERROR, _T("CHeightfieldPhysicsObject: Failed to get Terrain Descriptor.\n") );
	}

	// Get the mesh object from the parent
	TERRAINSECTORGETPOLYDATA terrainSectorGetPolyData;
	static DWORD msgHash_TerrainSectorGetPolyData = CHashString(_T("TerrainSectorGetPolyData")).GetUniqueID();
	if( m_ToolBox->SendMessage(msgHash_TerrainSectorGetPolyData, sizeof(TERRAINSECTORGETPOLYDATA), &terrainSectorGetPolyData, GetParentName() ) != MSG_HANDLED )
	{
		m_ToolBox->Log( LOGERROR, _T("CHeightfieldPhysicsObject: Failed to get Terrain Polygon data.\n") );
	}

	if( terrainSectorGetPolyData.pVertexData == NULL || terrainSectorGetPolyData.pIndexData == NULL ||
		terrainSectorGetPolyData.iVertexCount == 0 || terrainSectorGetPolyData.iIndexCount == 0 ||
		terrainDescriptor.SectorResolution == 0 || terrainDescriptor.SectorSize == 0 )
	{
		m_ToolBox->Log( LOGERROR, _T("CHeightfieldPhysicsObject: Heightfield data is invalid.\n") );
	
		// we have incorrect data, that can't be used to initialize the  
		// terrain. let's return...
		return;
	}

	int nbColumns = terrainDescriptor.SectorResolution +1;
	int nbRows = terrainDescriptor.SectorResolution +1;

	// Start building the heightfield descriptor
	NxHeightFieldDesc heightFieldDesc;
	heightFieldDesc.nbColumns			= nbColumns;
	heightFieldDesc.nbRows				= nbRows;
	heightFieldDesc.verticalExtent		= -1000;
	heightFieldDesc.convexEdgeThreshold = 0;
	heightFieldDesc.flags = NX_HF_NO_BOUNDARY_EDGES;

	// Allocate storage for heightfield samples
	heightFieldDesc.samples			= new NxU32[nbColumns*nbRows];
	heightFieldDesc.sampleStride	= sizeof(NxU32);

	char* currentByte = (char*)heightFieldDesc.samples;
	for( int row = 0; row < nbRows; row++ )
	{
		for( int column = nbColumns-1; column >= 0; column-- )
		{
			NxHeightFieldSample* currentSample = (NxHeightFieldSample*)currentByte;

			// Get the terrain vertex
			Vec3 currentTerrainVertex = terrainSectorGetPolyData.pVertexData[ (row*nbColumns)+column ];

			currentSample->height = unsigned int( currentTerrainVertex.y*fFixedPointScaler );
			currentSample->materialIndex0 = 0;
			currentSample->materialIndex1 = 0;
			currentSample->tessFlag = 0;

			currentByte += heightFieldDesc.sampleStride;
		}
	}

	NxHeightField* heightField = m_pManager->CreateHeightField( heightFieldDesc );

	// Data has been copied, we can free our buffer
	delete heightFieldDesc.samples;

	float heightfieldSpacing = terrainDescriptor.SectorSize / terrainDescriptor.SectorResolution;

	// The actor has one shape, a heightfield
	NxHeightFieldShapeDesc* heightFieldShapeDesc = new NxHeightFieldShapeDesc();
	heightFieldShapeDesc->heightField	= heightField;
	heightFieldShapeDesc->heightScale	= 1.0f/fFixedPointScaler; // vert scale
	heightFieldShapeDesc->rowScale		= NxReal(heightfieldSpacing); // horizontal scale
	heightFieldShapeDesc->columnScale	= NxReal(heightfieldSpacing); // horizontal scale
	heightFieldShapeDesc->materialIndexHighBits = 0;
	heightFieldShapeDesc->holeMaterial = 2;

	NxActorDesc* actorDesc = new NxActorDesc();

	actorDesc->shapes.pushBack( heightFieldShapeDesc );
	MathUtil math;
	EulerAngle euler( 0, 0, -90 );
	Matrix3x3 tmpMatrix; 
	math.EulerToMatrix( euler, tmpMatrix ); 
	actorDesc->globalPose = adapter;
	actorDesc->globalPose.M.setColumnMajor( tmpMatrix.m ); 
	actorDesc->globalPose.t.set(	actorDesc->globalPose.t.x + terrainDescriptor.SectorSize,
									actorDesc->globalPose.t.y,
									actorDesc->globalPose.t.z ); 

	if( m_pManager && actorDesc->isValid() )
	{
		// Queue up the create event
		CREATEPHYSICSEVENT event;
		event.actorDesc = actorDesc;
		event.physObject = this;
		m_pManager->AddCreateEvent( event );
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid actor descriptor!\n") );
		return;
	}
}

void CHeightfieldPhysicsObject::SetTransform()
{
	// Check that the actors are valid
	if( m_PhysicsActor == NULL )
	{
		return;
	}

	// Get the new transform from the parent object
	Matrix4x4 tmp;
	GetParentTransform( tmp );
	NxMat34Adapter newTransform(tmp);


	// get the actor desc
	NxActorDesc* desc = new NxActorDesc();
	m_PhysicsActor->m_Actor->saveToDesc( *desc );

	// save off the old shape into a new shape descriptor
	NxShape*const* shapes = m_PhysicsActor->m_Actor->getShapes();
	NxHeightFieldShape* currentShape = (NxHeightFieldShape*)shapes[0];
	NxHeightFieldShapeDesc* shapeDesc = new NxHeightFieldShapeDesc;
	currentShape->saveToDesc( *shapeDesc );

	// re-add the shapes to the saved off actor descriptor
	desc->shapes.pushBack( shapeDesc );

	// set the description's new pose
	desc->globalPose = newTransform;

	// delete the actor
	ReleaseModel();

	// We need to reorient the terrain to our internal axies
	// get the terrain descriptor 
    TERRAINDESCRIPTOR terrainDescriptor; 
	static DWORD msgHash_GetTerrainDescriptor = CHashString(_T("GetTerrainDescriptor")).GetUniqueID();
    m_ToolBox->SendMessage(msgHash_GetTerrainDescriptor, sizeof(TERRAINDESCRIPTOR), &terrainDescriptor ); 

	MathUtil math;
	EulerAngle euler( 0, 0, -90 );
	Matrix3x3 tmpMatrix; 
	math.EulerToMatrix( euler, tmpMatrix ); 
	desc->globalPose.M.setColumnMajor( tmpMatrix.m ); 
	desc->globalPose.t.set( desc->globalPose.t.x+terrainDescriptor.SectorSize, desc->globalPose.t.y, desc->globalPose.t.z ); 

	// recreate the actor in the new location
	if( m_pManager && desc->isValid() )
	{
		// Queue up the create event
		CREATEPHYSICSEVENT event;
		event.actorDesc = desc;
		event.physObject = this;
		m_pManager->AddCreateEvent( event );
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid actor descriptor!\n") );
		return;
	}

	if( m_PhysicsActor == NULL )
	{
		m_ToolBox->Log( LOGERROR, _T("CHeightfieldPhysicsObject::SetTransform(): Failed to re-build static actor after move!\n") );
		return;
	}

	// Delete the saved shape
	delete shapeDesc;
}
