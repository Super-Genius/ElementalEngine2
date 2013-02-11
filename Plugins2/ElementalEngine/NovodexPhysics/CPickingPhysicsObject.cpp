///==========================================================================
/// \file	CPickingPhysicsObject.cpp
/// \brief	An OOBB used for picking objects
/// \date	2/21/2007
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
REGISTER_COMPONENT(CPickingPhysicsObject);

CPickingPhysicsObject::CPickingPhysicsObject( IHashString *parentName, IHashString *name, bool addToHier ) : 
	CPhysicsObject(parentName, name, addToHier)
{
	// TESTING: Lest see if this allows us to move an object while the sim is paused.
	m_ForceMove = true;

	m_CollisionGroup = CONTROL;
	m_ContactGroup = 0;
}

CPickingPhysicsObject::~CPickingPhysicsObject()
{
	DeInit();

	if( m_bAddToHierarchy )
	{
		RemoveFromHierarchy();
	}
}

IComponent *CPickingPhysicsObject::Create( int nArgs, va_list argptr )
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool addToHier;

	name = va_arg( argptr, IHashString* );
	parentName = va_arg( argptr, IHashString* );
	try
	{
		if( nArgs == 2 )
		{
			self = new CPickingPhysicsObject( parentName, name, true );
		}
		else if( nArgs == 3 )
		{
			addToHier = va_arg( argptr, bool );
			self = new CPickingPhysicsObject( parentName, name, addToHier );
		}
		else
		{
			EngineGetToolBox()->Log( LOGERROR, _T("Invalid # - %d passed for argument to Create Picking Physics Object\n"), nArgs );
			return NULL;
		}
	}
	catch(...)
	{
		EngineGetToolBox()->Log( LOGERROR, _T("CPickingPhysicsObject: Failed to create CPickingPhysicsObject!\n") );
		return NULL;
	}

	return self;
}

void CPickingPhysicsObject::Init()
{
	if( m_PhysicsActor != NULL )
	{
		ReleaseModel();
	}

	m_hsParentType.Init( _T("CEntity") );

	// Build Terrain
	BuildBox();
}

bool CPickingPhysicsObject::Update()
{
	if( m_bTransformIsDirty )
	{
		SetTransform();
		m_bTransformIsDirty = false;
	}

	return true;
}

void CPickingPhysicsObject::DeInit()
{
	CPhysicsObject::DeInit();
}

void CPickingPhysicsObject::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		ar.Read( m_Position );
		ar.Read( m_Extents );
	}
	else
	{
		ar.Write( m_Extents );
	}
}

IHashString *CPickingPhysicsObject::GetComponentType()
{
	static CHashString componentType( _T("CPickingPhysicsObject") );
	return &componentType;
}

bool CPickingPhysicsObject::IsKindOf( IHashString *componentType )
{
	static CHashString cpo( _T("CPhysicsObject") );
	if( componentType->GetUniqueID() == cpo.GetUniqueID() ) 
	{
		return true;
	}
	return ( componentType->GetUniqueID() == GetComponentType()->GetUniqueID() );
}

void CPickingPhysicsObject::BuildBox()
{
	// Factor in scale?
	Vec3 scale;
	static DWORD msgHash_GetGlobalScale = CHashString(_T("GetGlobalScale")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_GetGlobalScale, sizeof(Vec3), &scale, GetParentName() );
	m_Extents *= scale;

	// Fix any 0 scale extents
	if( m_Extents.x <= 0.0f)
	{
		m_Extents.x = 0.1f;
	}
	if( m_Extents.y <= 0.0f)
	{
		m_Extents.y = 0.1f;
	}
	if( m_Extents.z <= 0.0f)
	{
		m_Extents.z =0.1f;
	}

	// Build the shape descriptor
	NxBoxShapeDesc* boxDesc = new NxBoxShapeDesc();
	boxDesc->localPose.t = NxVec3( m_Position.x, m_Position.y, m_Position.z );
	boxDesc->dimensions = NxVec3( m_Extents.x, m_Extents.y, m_Extents.z );

	// get the pose
	Matrix4x4 tmp;
	GetParentTransform( tmp );
	NxMat34 pose;

	// Define the body descriptor
	NxBodyDesc* bodyDesc = new NxBodyDesc();
	bodyDesc->flags |= NX_BF_KINEMATIC;
	
	// Build the actor descriptor
	NxActorDesc* actorDesc = new NxActorDesc();
	actorDesc->shapes.pushBack( boxDesc );
	actorDesc->body = bodyDesc;
	actorDesc->density = 1;
	actorDesc->globalPose = pose;

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