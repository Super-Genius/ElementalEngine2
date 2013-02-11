///==========================================================================
/// \file	CEECharacterPhysicsObject.cpp
/// \brief	Uses the PhysX Character controller object to drive character's
///			physics
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

// move this to stdafx
#include "CEECharacterPhysicsObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ------------- Register Object ---------------- //
REGISTER_COMPONENT(CEECharacterPhysicsObject);

// ------------ Register Messages --------------- //
REGISTER_MESSAGE_HANDLER( MoveCharacter, OnMoveCharacter, CEECharacterPhysicsObject );
REGISTER_MESSAGE_HANDLER( SetCharacterPosition, OnSetCharacterPosition, CEECharacterPhysicsObject );

REGISTER_MESSAGE_HANDLER( SetCharacterHeight, OnSetCharacterHeight, CEECharacterPhysicsObject );
REGISTER_MESSAGE_HANDLER( SetCharacterWidth, OnSetCharacterWidth, CEECharacterPhysicsObject );

REGISTER_MESSAGE_HANDLER( SetMaxVelocity, OnSetMaxVelocity, CEECharacterPhysicsObject );

REGISTER_MESSAGE_HANDLER( SetActorCollisionGroup, OnSetCollisionGroup, CEECharacterPhysicsObject );

REGISTER_MESSAGE_HANDLER( SetLinearVelocity, OnSetLinearVelocity, CEECharacterPhysicsObject );
REGISTER_MESSAGE_HANDLER( SetAngularVelocity, OnSetAngularVelocity, CEECharacterPhysicsObject );
REGISTER_MESSAGE_HANDLER( GetLinearVelocity, OnGetLinearVelocity, CEECharacterPhysicsObject );
REGISTER_MESSAGE_HANDLER( GetAngularVelocity, OnGetAngularVelocity, CEECharacterPhysicsObject );

REGISTER_MESSAGE_HANDLER( SetLinearMomentum, OnSetLinearMomentum, CEECharacterPhysicsObject );
REGISTER_MESSAGE_HANDLER( SetAngularMomentum, OnSetAngularMomentum, CEECharacterPhysicsObject );
REGISTER_MESSAGE_HANDLER( GetLinearMomentum, OnGetLinearMomentum, CEECharacterPhysicsObject );
REGISTER_MESSAGE_HANDLER( GetAngularMomentum, OnGetAngularMomentum, CEECharacterPhysicsObject );

REGISTER_MESSAGE_HANDLER( SetKinematic, OnSetKinematic, CEECharacterPhysicsObject );
REGISTER_MESSAGE_HANDLER( SetDynamic, OnSetDynamic, CEECharacterPhysicsObject );

REGISTER_MESSAGE_HANDLER( EnableGravity, OnEnableGravity, CEECharacterPhysicsObject );
REGISTER_MESSAGE_HANDLER( DisableGravity, OnDisableGravity, CEECharacterPhysicsObject );

REGISTER_MESSAGE_HANDLER( LockHorizontalAxies, OnLockHorizontalAxies, CEECharacterPhysicsObject );
REGISTER_MESSAGE_HANDLER( UnLockHorizontalAxies, OnUnLockHorizontalAxies, CEECharacterPhysicsObject );

REGISTER_MESSAGE_HANDLER( GetLocalPosition, OnGetLocalPosition, CEECharacterPhysicsObject );
REGISTER_MESSAGE_HANDLER( SetLocalPosition, OnSetLocalPosition, CEECharacterPhysicsObject );

// ------------- Register Messages -------------- //

CEECharacterPhysicsObject::CEECharacterPhysicsObject( IHashString *parentName, IHashString *name, bool addToHier ) : 
	CPhysicsObject(parentName, name, addToHier)
{
	m_SweptCollisionDetected = false;
	m_GravityEnable = true;

	m_MoveCharacterEvent = NULL;
	m_SetCharacterPositionEvent = NULL;
	m_SetCharacterHeightEvent = NULL;
	m_SetCharacterWidthEvent = NULL;
	m_SetLocalPositionEvent = NULL;
	m_LockHorizontalAxiesEvent = NULL;
	m_UnLockHorizontalAxiesEvent = NULL;

	m_Mass = 1.0f;

	m_MaxVelocity = 1000000.0f;

	m_HeightAdjustFactor = 13.0f;

	m_CCDSkeleton = NULL;
}

CEECharacterPhysicsObject::~CEECharacterPhysicsObject()
{
	DeInit();

	if( m_bAddToHierarchy )
	{
		RemoveFromHierarchy();
	}
}

IComponent *CEECharacterPhysicsObject::Create( int nArgs, va_list argptr )
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
			self = new CEECharacterPhysicsObject( parentName, name, true );
		}
		else if( nArgs == 3 )
		{
			addToHier = va_arg( argptr, bool );
			self = new CEECharacterPhysicsObject( parentName, name, addToHier );
		}
		else
		{
			EngineGetToolBox()->Log( LOGERROR, _T("Invalid # - %d passed for argument to Create Physics Object\n"), nArgs );
			return NULL;
		}
	}
	catch(...)
	{
		EngineGetToolBox()->Log( LOGERROR, _T("CEECharacterPhysicsObject: Failed to create CEECharacterPhysicsObject!\n") );
		return NULL;
	}

	return self;
}

void CEECharacterPhysicsObject::Init()
{
	if( m_PhysicsActor != NULL )
	{
		ReleaseModel();
	}

	// Get the extents of the object
	GetParentExtents( m_Position, m_Extents, m_Rotation );

	// Test for invalid extents
	if( !NxMath::isFinite( m_Extents.x ) ||
		!NxMath::isFinite( m_Extents.y ) ||
		!NxMath::isFinite( m_Extents.z ) )
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid extents on %s!\n"), GetParentName()->GetString() );
		return;
	}

	m_hsParentType.Init(_T("CEntity"));

	static CHashString capsule( _T("Capsule") );
	static CHashString box( _T("Box") );

	if( m_Shape == capsule )
	{
		BuildCharacterCapsuleActor();
	}
	else if( m_Shape == box )
	{
		BuildCharacterBoxActor();
	}
	else
	{
		m_ToolBox->Log(LOGERROR, _T("CEECharacterPhysicsObject::Init() - Invalid shape.\n"));
	}

	// Set the inital oldTransform to the current transform
	NxMat34Adapter adapter( oldTransform );
	GetParentTransform( adapter.ToMatrix4x4() );
}

bool CEECharacterPhysicsObject::Update()
{
	if( m_PhysicsActor == NULL )
	{
		return false;
	}

	// If we have moved then try to move to the new location
	if( m_bTransformIsDirty )
	{
		SetTransform();
		m_bTransformIsDirty = false;
	}

	PostUpdateCharacterController();

	return true;
}

void CEECharacterPhysicsObject::DeInit()
{
	CPhysicsObject::DeInit();

	// Delete old CCD skeleton
	if( m_CCDSkeleton != NULL )
	{
		m_pManager->GetPhysicsSDK()->releaseCCDSkeleton( *m_CCDSkeleton );
	}
}

void CEECharacterPhysicsObject::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		// Read in the collision group
		int group;
		ar.Read( group );
		m_CollisionGroup = (COLLISIONGROUP)group;
		// Read in the contact group
		ar.Read( m_ContactGroup );
		// Read in the character's shape (capsule or box)
		StdString shape;
		ar.Read( shape );
		m_Shape = shape;
		// Read in the character's mass
		ar.Read( m_Mass );
	}
	else
	{
		ar.Write( m_CollisionGroup, _T("collisiongroup") );
		ar.Write( m_ContactGroup, _T("contactgroup") );
		ar.Write( m_Shape.GetString(), _T("shape") );
		ar.Write( m_Mass, _T("mass") );
	}
}

IHashString *CEECharacterPhysicsObject::GetComponentType()
{
	static CHashString componentType( _T("CEECharacterPhysicsObject") );
	return &componentType;
}

bool CEECharacterPhysicsObject::IsKindOf( IHashString *componentType )
{
  	static CHashString cpo( _T("CPhysicsObject") );
	if( componentType->GetUniqueID() == cpo.GetUniqueID() )
	{
		return true;
	}
	return ( componentType->GetUniqueID() == GetComponentType()->GetUniqueID() );
}

void CEECharacterPhysicsObject::SetTransform()
{
	// Check that the actors are valid
	if( m_PhysicsActor == NULL )
	{
		return;
	}

	bool isKinematic = m_PhysicsActor->IsKinematic();

	// If the character is kinematic then allow the setting of its position.
	if( isKinematic )
	{
		Matrix4x4 newTransform;
		GetParentTransform( newTransform );
		m_PhysicsActor->SetGlobalPose( newTransform );
	}
}

void CEECharacterPhysicsObject::PostUpdateCharacterController()
{
	if( m_PhysicsActor != NULL )
	{
		Vec3 position;

		// Send the position after gravity, elevators and what not
		position = m_PhysicsActor->GetGlobalPosition();

		static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), &position, GetParentName(), &m_hsParentType );
	}
}


void CEECharacterPhysicsObject::ExecuteEventQueue()
{
	// Make sure the actor didn't get deleted while waiting for
	// the event queue to execute.
	if( m_PhysicsActor == NULL )
		return;

	CPhysicsObject::ExecuteEventQueue();

	if( m_MoveCharacterEvent != NULL )
		MoveCharacter();
	if( m_SetCharacterPositionEvent != NULL )
		SetCharacterPosition();
	if( m_SetCharacterHeightEvent != NULL )
		SetCharacterHeight();
	if( m_SetCharacterWidthEvent != NULL )
		SetCharacterWidth();
	if( m_SetLocalPositionEvent != NULL )
		SetLocalPosition();
	if( m_LockHorizontalAxiesEvent != NULL )
		LockHorizontalAxies();
	if( m_UnLockHorizontalAxiesEvent != NULL )
		UnLockHorizontalAxies();
}

void CEECharacterPhysicsObject::MoveCharacter()
{
	// If gravity is enabled then reset the downward velocity from the
	// last frame.
	if( m_GravityEnable )
	{
		// Get the old linear velocity
		Vec3 oldVelocity;
		m_PhysicsActor->GetLinearVelocity( oldVelocity );

		// clamp y velocity to negative numbers
		float yVelocity = oldVelocity.y;
		if( yVelocity >= 0.0f )
			yVelocity = 0.0f;

		m_MoveCharacterEvent->newVelocity.SetY( yVelocity );
	}

	m_PhysicsActor->SetLinearVelocity( m_MoveCharacterEvent->newVelocity );
	SAFEDELETE( m_MoveCharacterEvent );
}

void CEECharacterPhysicsObject::SetCharacterPosition()
{
	m_PhysicsActor->SetGlobalPosition( m_SetCharacterPositionEvent->newPosition );
	SAFEDELETE( m_SetCharacterPositionEvent );
}

void CEECharacterPhysicsObject::SetCharacterHeight()
{
	m_Extents.SetY( m_SetCharacterHeightEvent->newHeight );
	
	static CHashString capsule( _T("Capsule") );
	static CHashString box( _T("Box") );
	if( m_Shape == capsule )
	{
		m_Position.SetY( (m_SetCharacterHeightEvent->newHeight) );
		BuildCharacterCapsuleActor();
	}
	else if( m_Shape == box )
	{
		BuildCharacterBoxActor();
	}

	SAFEDELETE( m_SetCharacterHeightEvent );
}

void CEECharacterPhysicsObject::SetCharacterWidth()
{
	m_Extents.x = m_SetCharacterWidthEvent->newWidth;
	m_Extents.z = m_SetCharacterWidthEvent->newWidth;

	static CHashString capsule( _T("Capsule") );
	static CHashString box( _T("Box") );
	if( m_Shape == capsule )
	{
		BuildCharacterCapsuleActor();
	}
	else if( m_Shape == box )
	{
		BuildCharacterBoxActor();
	}

	SAFEDELETE( m_SetCharacterWidthEvent );
}

void CEECharacterPhysicsObject::SetLocalPosition()
{
	// Get the actor's capsule shape
	NxActor* actor = m_PhysicsActor->m_Actor;
	NxShape*const* shapes = actor->getShapes();
   	NxShape* currentShape = shapes[0];

	Vec3 newLocalPosition = m_SetLocalPositionEvent->newLocalPosition;
	NxVec3 pos( newLocalPosition.x, newLocalPosition.y, newLocalPosition.z );
	currentShape->setLocalPosition( pos );

	SAFEDELETE( m_SetLocalPositionEvent );
}

void CEECharacterPhysicsObject::LockHorizontalAxies()
{
	m_PhysicsActor->SetBodyFlag( NX_BF_FROZEN_POS_X, true );
	m_PhysicsActor->SetBodyFlag( NX_BF_FROZEN_POS_Z, true );

	SAFEDELETE( m_LockHorizontalAxiesEvent );
}

void CEECharacterPhysicsObject::UnLockHorizontalAxies()
{
	m_PhysicsActor->SetBodyFlag( NX_BF_FROZEN_POS_X, false );
	m_PhysicsActor->SetBodyFlag( NX_BF_FROZEN_POS_Z, false );

	SAFEDELETE( m_UnLockHorizontalAxiesEvent );
}

void CEECharacterPhysicsObject::EnableGravity()
{
	m_PhysicsActor->SetBodyFlag( NX_BF_DISABLE_GRAVITY, false );
	m_GravityEnable = true;

	SAFEDELETE( m_EnableGravityEvent );
}

void CEECharacterPhysicsObject::DisableGravity()
{
	m_PhysicsActor->SetBodyFlag( NX_BF_DISABLE_GRAVITY, true );
	m_GravityEnable = false;

	SAFEDELETE( m_DisableGravityEvent );
}

void CEECharacterPhysicsObject::CreateCCDSkeleton( Vec3 extents, NxSimpleTriangleMesh &triMesh )
{
	// need to shrink the ccd skeleton to prevent sliding sticking...
	// see PhysX sdk documentation for more details.
	float scale = 0.5f;
	NxVec3 dimensions(extents.x * scale, extents.y * scale, extents.z * scale );
	NxBox obb = NxBox( NxVec3(0.0f,0.0f,0.0f), dimensions, NxMat33(NX_IDENTITY_MATRIX) );
	
	triMesh.points = new NxVec3[8];
	triMesh.numVertices = 8;
	triMesh.pointStrideBytes = sizeof(NxVec3);
	triMesh.numTriangles = 2*6;
	triMesh.triangles = new NxU32[2*6*3];
	triMesh.triangleStrideBytes = sizeof(NxU32)*3;
	triMesh.flags = 0;

	NxComputeBoxPoints( obb,(NxVec3 *)triMesh.points );

	memcpy( (NxU32 *)triMesh.triangles,NxGetBoxTriangles(),sizeof(NxU32)*2*6*3 );
}

void CEECharacterPhysicsObject::BuildCharacterCapsuleActor()
{
	if( m_PhysicsActor != NULL )
	{
		ReleaseModel();
		if( m_CCDSkeleton != NULL )
			m_pManager->GetPhysicsSDK()->releaseCCDSkeleton( *m_CCDSkeleton );
	}
	
	float min = m_Extents.x;
	if (min > m_Extents.z)
	{
		min = m_Extents.z;
	}
	if (min > m_Extents.y)
	{
		min = m_Extents.y;
	}

	// Since character's never stand around in their davinci poses we need to adjust some values
	float widthAdjustFactor = 0.75f;
	float radius = ((m_Extents.z + m_Extents.x) * 0.5f) * widthAdjustFactor;

	// Specify shape descriptor
	NxCapsuleShapeDesc* capsuleDesc = new NxCapsuleShapeDesc();
	capsuleDesc->height = (m_Extents.y*2) - (radius*2) + m_HeightAdjustFactor;
	capsuleDesc->radius = radius;
	capsuleDesc->localPose.t.set( m_Position.x, m_Position.y, m_Position.z );
	capsuleDesc->materialIndex = 0;

	// Create CCD mesh
	NxSimpleTriangleMesh triMesh;
	Vec3 m_Extentsents( min, capsuleDesc->height, min );
	CreateCCDSkeleton( m_Extentsents, triMesh );
	NxCCDSkeleton* newSkeleton = m_pManager->GetPhysicsSDK()->createCCDSkeleton( triMesh );
	capsuleDesc->ccdSkeleton = newSkeleton;

	SAFEDELETE( triMesh.points );
	SAFEDELETE( triMesh.triangles );

	// Specify body descriptor
	NxBodyDesc* bodyDesc = new NxBodyDesc();
	bodyDesc->flags = NX_BF_FROZEN_ROT_X | NX_BF_FROZEN_ROT_Y | NX_BF_FROZEN_ROT_Z;

	// Specify actor descriptor
	NxActorDesc* actorDesc = new NxActorDesc();
	actorDesc->density = m_Mass;
	actorDesc->shapes.pushBack( capsuleDesc );
	actorDesc->body = bodyDesc;

	Matrix4x4 tmp;
	GetParentTransform( tmp );

	NxMat34Adapter adapter(tmp);
	actorDesc->globalPose = adapter;


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

void CEECharacterPhysicsObject::BuildCharacterBoxActor()
{
	if( m_PhysicsActor != NULL )
	{
		ReleaseModel();
		if( m_CCDSkeleton != NULL )
			m_pManager->GetPhysicsSDK()->releaseCCDSkeleton( *m_CCDSkeleton );
	}

	float min = m_Extents.x;
	if (min > m_Extents.z)
	{
		min = m_Extents.z;
	}
	if (min > m_Extents.y)
	{
		min = m_Extents.y;
	}

	// gun robots give a wierd z skew, in any case z axis should be largely symettrical
	m_Position.SetZ( 0.0f );

	// Specify box descriptor
	NxBoxShapeDesc* boxDesc = new NxBoxShapeDesc();
	boxDesc->dimensions = NxVec3( m_Extents.x, m_Extents.y, m_Extents.z );
	boxDesc->localPose.t.set( m_Position.x, m_Position.y, m_Position.z );

	// Create CCD mesh
	NxSimpleTriangleMesh triMesh;
	Vec3 m_Extentsents( m_Extents.x, m_Extents.y, m_Extents.z );
	CreateCCDSkeleton( m_Extents, triMesh );
	NxCCDSkeleton* newSkeleton = m_pManager->GetPhysicsSDK()->createCCDSkeleton( triMesh );
	boxDesc->ccdSkeleton = newSkeleton;

	SAFEDELETE( triMesh.points );
	SAFEDELETE( triMesh.triangles );

	// Specify body descriptor
	NxBodyDesc* bodyDesc = new NxBodyDesc();
	bodyDesc->flags = NX_BF_FROZEN_ROT_X | NX_BF_FROZEN_ROT_Y | NX_BF_FROZEN_ROT_Z;

	// Specify actor descriptor
	NxActorDesc* actorDesc = new NxActorDesc();
	actorDesc->density = m_Mass;
	actorDesc->shapes.pushBack( boxDesc );
	actorDesc->body = bodyDesc;

	Matrix4x4 tmp;
	GetParentTransform( tmp );
	NxMat34Adapter transform( tmp );
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

void CEECharacterPhysicsObject::GetParentExtents( Vec3& pos, Vec3& ext, EulerAngle& rot )
{
	// Get the extents of the object
	GETBOUNDINGBOXPARAMS bBoxParms;
	bBoxParms.position = &pos;
	bBoxParms.extents = &ext;
	bBoxParms.rotation = &rot;
	
	static DWORD msgHash_GetBoundingBox = CHashString(_T("GetBoundingBox")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetBoundingBox, sizeof(bBoxParms), &bBoxParms, GetParentName() ) != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGERROR, _T("Unable to get Bounding box from Parent for character controller %s\n"), GetName()->GetString());
	}
}

DWORD CEECharacterPhysicsObject::OnMoveCharacter( DWORD size, void* params )
{
	if( m_PhysicsActor == NULL )
	{
		return MSG_HANDLED_STOP;
	}

	Vec3 velocity;
	VERIFY_MESSAGE_SIZE( sizeof(Vec3), size );
	velocity = *(Vec3*)params;

	// Make sure that we are getting a valid vec3
	NxVec3 tmp( velocity.x, velocity.y, velocity.z );
	if( !tmp.isFinite() )
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
		return MSG_ERROR;
	}

	m_MoveCharacterEvent = new MOVECHARACTEREVENT();
	m_MoveCharacterEvent->newVelocity = velocity;

	return MSG_HANDLED_STOP;
}

DWORD CEECharacterPhysicsObject::OnSetCharacterPosition( DWORD size, void* params )
{
	Vec3 position;
	VERIFY_MESSAGE_SIZE( sizeof(Vec3), size );
	position = *(Vec3*)params;

	// Make sure that we are getting a valid vec3
	NxVec3 tmp( position.x, position.y, position.z );
	if( !tmp.isFinite() )
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
		return MSG_ERROR;
	}

	m_SetCharacterPositionEvent = new SETCHARACTERPOSITIONEVENT();
	m_SetCharacterPositionEvent->newPosition = position;

	return MSG_HANDLED_STOP;
}

DWORD CEECharacterPhysicsObject::OnSetCharacterHeight( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(float), size );
	float newHeight = *(float*)params;

	if( newHeight <= 0.0f )
	{
		// invalid height
		return MSG_HANDLED_STOP;
	}

	m_SetCharacterHeightEvent = new SETCHARACTERHEIGHTEVENT();
	m_SetCharacterHeightEvent->newHeight = newHeight;

	return MSG_HANDLED_STOP;
}

DWORD CEECharacterPhysicsObject::OnSetCharacterWidth( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(float), size );
	float newWidth = *(float*)params;

	if( newWidth <= 0.0f )
	{
		// invalid width
		return MSG_HANDLED_STOP;
	}

	m_SetCharacterWidthEvent = new SETCHARACTERWIDTHEVENT();
	m_SetCharacterWidthEvent->newWidth = newWidth;

	return MSG_HANDLED_STOP;
}

DWORD CEECharacterPhysicsObject::OnSetMaxVelocity( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(float), size );
	float newMaxVelocity = *(float*)params;

	m_MaxVelocity = newMaxVelocity;

	return MSG_HANDLED_STOP;
}

DWORD CEECharacterPhysicsObject::OnSetLinearVelocity( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( sizeof(Vec3), size );
	Vec3* velocity = (Vec3*)params;

	// Make sure that we are getting a valid vec3
	NxVec3 tmp( velocity->x, velocity->y, velocity->z );
	if( !tmp.isFinite() )
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
		return MSG_ERROR;
	}

	if( velocity->Length() > m_MaxVelocity )
	{
		velocity->SetLength( m_MaxVelocity );
	}

	m_SetLinearVelocityEvent = new SETLINEARVELOCITYEVENT();
	m_SetLinearVelocityEvent->velocity = *velocity;

	return MSG_HANDLED_STOP;
}

DWORD CEECharacterPhysicsObject::OnLockHorizontalAxies( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( 0, size );

	m_LockHorizontalAxiesEvent = new LOCKHORIZONTALAXIESEVENT();

	return MSG_HANDLED_STOP;
}

DWORD CEECharacterPhysicsObject::OnUnLockHorizontalAxies( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( 0, size );

	m_UnLockHorizontalAxiesEvent = new UNLOCKHORIZONTALAXIESEVENT();

	return MSG_HANDLED_STOP;
}

DWORD CEECharacterPhysicsObject::OnGetLocalPosition( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(Vec3), size );
	Vec3* localPosition = (Vec3*)params;

	if( m_PhysicsActor != NULL )
	{
		NxActor* actor = m_PhysicsActor->m_Actor;
		NxShape*const* shapes = actor->getShapes();
   		NxShape* currentShape = shapes[0];

		NxVec3 pos = currentShape->getLocalPosition();

		localPosition->Set( pos.x, pos.y, pos.z );
	}
	else
	{
		localPosition->Set( 0.0f, 0.0f, 0.0f );
	}

	return MSG_HANDLED_STOP;
}

DWORD CEECharacterPhysicsObject::OnSetLocalPosition( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(Vec3), size );
	Vec3* localPosition = (Vec3*)params;

	m_SetLocalPositionEvent = new SETLOCALPOSITIONEVENT();
	m_SetLocalPositionEvent->newLocalPosition = *localPosition;

	return MSG_HANDLED_STOP;
}
