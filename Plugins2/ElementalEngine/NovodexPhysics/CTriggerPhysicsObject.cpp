///==========================================================================
/// \file	CTriggerPhysicsObject.h
/// \brief	Trigger volume physics object
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
REGISTER_COMPONENT(CTriggerPhysicsObject);

CTriggerPhysicsObject::CTriggerPhysicsObject( IHashString *parentName, IHashString *name, bool addToHier ) : 
	CPhysicsObject(parentName, name, addToHier)
{
	m_TriggerOnEntry = false;
	m_TriggerOnStay = false;
	m_TriggerOnLeave = false;

	m_NumVerts = 0;
	m_NumFaces = 0;
	m_Verts = NULL;
	m_Faces = NULL;

	m_isMovable = false;
}

CTriggerPhysicsObject::~CTriggerPhysicsObject()
{
	DeInit();

	if( m_bAddToHierarchy )
	{
		RemoveFromHierarchy();
	}
}

IComponent* CTriggerPhysicsObject::Create( int nArgs, va_list argptr )
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
			self = new CTriggerPhysicsObject( parentName, name, true );
		}
		else if( nArgs == 3 )
		{
			bAddToHier = va_arg( argptr, bool );
			self = new CTriggerPhysicsObject( parentName, name, bAddToHier );
		}
		else
		{
			EngineGetToolBox()->Log( LOGERROR, _T("Invalid # - %d passed for argument to Create Physics Object\n"), nArgs );
			return NULL;
		}
	}
	catch(...)
	{
		EngineGetToolBox()->Log( LOGERROR, _T("CTriggerPhysicsObject: Failed to create CTriggerPhysicsObject!\n") );
		return NULL;
	}

	return self;
}

void CTriggerPhysicsObject::Init()
{
	if( m_PhysicsActor != NULL )
	{
		ReleaseModel();
	}

	m_hsParentType.Init( _T("CEntity") );

	PopulateVertexData();

	// Build trigger
	BuildTriggerActor();

	// Transform is set in BuildTriggerActor()
	m_bTransformIsDirty = false;
}

bool CTriggerPhysicsObject::Update()
{
	// Triggers are always static
	if( m_bTransformIsDirty )
	{
		SetTransform();
		m_bTransformIsDirty = false;
	}

	return true;
}

void CTriggerPhysicsObject::DeInit()
{
	CPhysicsObject::DeInit();

	m_NumVerts = 0;
	m_NumFaces = 0;
	SAFEARRAYDELETE( m_Verts );
	SAFEARRAYDELETE( m_Faces );
}

void CTriggerPhysicsObject::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		// Read in the collision group
		int group;
		ar.Read( group );
		m_CollisionGroup = (COLLISIONGROUP)group;
		ar.Read( m_ContactGroup );
		// Read in TriggerOn flags
		ar.Read( m_TriggerOnEntry );
		ar.Read( m_TriggerOnStay );
		ar.Read( m_TriggerOnLeave );
	}
	else
	{
		ar.Write( m_CollisionGroup, _T("collisiongroup") );
		ar.Write( m_ContactGroup, _T("contactgroup") );
		ar.Write( m_TriggerOnEntry, _T("triggerOnEntry") );
		ar.Write( m_TriggerOnStay, _T("triggerOnStay") );
		ar.Write( m_TriggerOnLeave, _T("triggerOnLeave") );
	}
}

IHashString* CTriggerPhysicsObject::GetComponentType()
{
	static CHashString cpo( _T("CTriggerPhysicsObject") );
	return &cpo;
}

bool CTriggerPhysicsObject::IsKindOf(IHashString *compType)
{
	static CHashString cpo( _T("CPhysicsObject") );
	if( compType->GetUniqueID() == cpo.GetUniqueID() ) 
	{
		return true;
	}
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}


void CTriggerPhysicsObject::SetTransform()
{
	// Can't set the transform on static meshes
}

void CTriggerPhysicsObject::BuildTriggerActor()
{
	// Make sure that we have a valid mesh
	if( m_Verts == NULL || m_Faces == NULL || m_NumVerts == 0 || m_NumFaces == 0 )
	{
		EngineGetToolBox()->Log( LOGERROR, _T("Invalid Mesh data to build CTriggerPhysicsObject!\n") );
	}

	// Change this from a box to a concave mesh
	NxTriangleMeshDesc meshDesc;
	meshDesc.numVertices = m_NumVerts;
	meshDesc.numTriangles = m_NumFaces;
	meshDesc.pointStrideBytes = sizeof(Vec3);
	meshDesc.triangleStrideBytes = 3*sizeof(int);
	meshDesc.points = m_Verts;
	meshDesc.triangles = m_Faces;

	// Cook the mesh data
	bool cookingInitStatus = NxInitCooking();
	MemoryWriteStream buf;
	bool status = NxCookTriangleMesh(meshDesc, buf);
	// make sure cooking worked
	if( status == false )
	{
		EngineGetToolBox()->Log( LOGERROR, _T("Failed to cook mesh data in CTriggerPhysicsObject::BuildTriggerActor()!\n") );
		NxCloseCooking();
		return;
	}
	NxCloseCooking();

	NxPhysicsSDK* sdk = m_pManager->GetPhysicsSDK();
	
	// Build mesh shape descriptor
	NxTriangleMeshShapeDesc* meshShapeDesc = new NxTriangleMeshShapeDesc();

	if( m_TriggerOnEntry )
		meshShapeDesc->shapeFlags |= NX_TRIGGER_ON_ENTER;
	if( m_TriggerOnStay )
		meshShapeDesc->shapeFlags |= NX_TRIGGER_ON_STAY;
	if( m_TriggerOnLeave )
		meshShapeDesc->shapeFlags |= NX_TRIGGER_ON_LEAVE;

    meshShapeDesc->meshData = sdk->createTriangleMesh( MemoryReadStream(buf.data) );

	// Build Actor descriptor
	NxActorDesc* actorDesc = new NxActorDesc();
	actorDesc->shapes.pushBack( meshShapeDesc );

	// Build body descriptor if flagged as movable
	if( m_isMovable == true )
	{
		NxBodyDesc* bodyDesc = new NxBodyDesc();
		bodyDesc->mass = 1;
		bodyDesc->flags = NX_BF_KINEMATIC;

		actorDesc->body = bodyDesc;
	}

	// Set the pose
	Matrix4x4 tmp;
	GetParentTransform( tmp );
	NxMat34Adapter adapter(tmp);
	NxMat34 transform = adapter;
	actorDesc->globalPose = transform;

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

void CTriggerPhysicsObject::PopulateVertexData()
{
	CREATEARCHIVE ca;
	CHashString memType(_T("Memory"));

	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &memType;

	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage( msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca );

	GETVERTICES gv;
	gv.archive = ca.archive;
	if( retVal == MSG_HANDLED && gv.archive != NULL )
	{
		static DWORD msgHash_GetVertices = CHashString(_T("GetVertices")).GetUniqueID();
		retVal = m_ToolBox->SendMessage( msgHash_GetVertices, sizeof(GETVERTICES), &gv, GetParentName() );

		if( retVal == MSG_HANDLED )
		{
			gv.archive->SetIsWriting( false );
			gv.archive->Read( m_NumVerts );
			m_Verts = new Vec3[m_NumVerts];
			for(int i=0; i<m_NumVerts; i++)
			{
				gv.archive->Read( m_Verts[i] );
			}

			gv.archive->Read( m_NumFaces );
			m_Faces = new int[3*m_NumFaces];
			for(int i=0; i<(m_NumFaces*3); i+=3)
			{
				gv.archive->Read( m_Faces[i] );
				gv.archive->Read( m_Faces[i+1] );
				gv.archive->Read( m_Faces[i+2] );
			}
		}
		else
		{
			m_ToolBox->Log( LOGERROR, _T( "CTriggerPhysicsObject sent unhandled message 'GetVertices'\n") );
		}

		gv.archive->Close();
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T( "CTriggerPhysicsObject could not create archive for in PopulateVertexData()\n") );
	}
}