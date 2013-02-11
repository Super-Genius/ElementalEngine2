///==========================================================================
/// \file	CPhysicsObject.h
/// \brief	Basic rigid body physics objects
/// \date	2/22/2005
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

#include "CPhysicsObjectCreateCallback.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ------------- Register Object ---------------- //
REGISTER_COMPONENT(CPhysicsObject);

// ------------- Register Messages -------------- //
REGISTER_MESSAGE_HANDLER( ApplyForce, OnApplyForce, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( ApplyTorque, OnApplyTorque, CPhysicsObject );

REGISTER_MESSAGE_HANDLER( SetLinearVelocity, OnSetLinearVelocity, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( SetAngularVelocity, OnSetAngularVelocity, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( GetLinearVelocity, OnGetLinearVelocity, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( GetAngularVelocity, OnGetAngularVelocity, CPhysicsObject );

REGISTER_MESSAGE_HANDLER( SetLinearMomentum, OnSetLinearMomentum, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( SetAngularMomentum, OnSetAngularMomentum, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( GetLinearMomentum, OnGetLinearMomentum, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( GetAngularMomentum, OnGetAngularMomentum, CPhysicsObject );

REGISTER_MESSAGE_HANDLER( SetLinearDamping, OnSetLinearDamping, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( SetAngularDamping, OnSetAngularDamping, CPhysicsObject );

REGISTER_MESSAGE_HANDLER( EnableVisualization, OnEnableVisualization, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( DisableVisualization, OnDisableVisualization, CPhysicsObject );

REGISTER_MESSAGE_HANDLER( SetKinematic, OnSetKinematic, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( SetDynamic, OnSetDynamic, CPhysicsObject );

REGISTER_MESSAGE_HANDLER( ReinitPhysics, OnReinitPhysics, CPhysicsObject );

REGISTER_MESSAGE_HANDLER( LockTranslationAxis, OnLockTranslationAxis, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( LockRotationAxis, OnLockRotationAxis, CPhysicsObject );

REGISTER_MESSAGE_HANDLER( EnablePhysicsAnimations, OnEnablePhysicsAnimations, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( SetHierarchalData, OnSetHierarchalData, CPhysicsObject );

REGISTER_MESSAGE_HANDLER( ForceSetGlobalPosition, OnForceSetGlobalTransform, CPhysicsObject );

REGISTER_MESSAGE_HANDLER( GetActorPositionByName, OnGetActorPositionByName, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( SetActorVelocityByName, OnSetActorVelocityByName, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( GetActorVelocityByName, OnGetActorVelocityByName, CPhysicsObject );

REGISTER_MESSAGE_HANDLER( SetActorCollisionGroup, OnSetCollisionGroup, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( BreakAllJoints, OnBreakAllJoints, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( BreakJoint, OnBreakJoint, CPhysicsObject );

REGISTER_MESSAGE_HANDLER( GetClothPhysicsGeometry, OnGetClothPhysicsGeometry, CPhysicsObject );

REGISTER_MESSAGE_HANDLER( EnableGravity, OnEnableGravity, CPhysicsObject );
REGISTER_MESSAGE_HANDLER( DisableGravity, OnDisableGravity, CPhysicsObject );

REGISTER_MESSAGE_HANDLER( EnableTrigger, OnEnableTrigger, CPhysicsObject );

REGISTER_MESSAGE_HANDLER( SetCollisionSurrogate, OnSetCollisionSurrogate, CPhysicsObject );


CPhysicsObject::CPhysicsObject( IHashString *parentName, IHashString *name, bool addToHier ) : 
	IPhysicsObject( _T("CPhysicsManager"),  _T("CPhysicsObject"), parentName, name )
{
	CHashString hashString( _T("CPhysicsManager") );
	m_pManager = (CPhysicsManager*)m_ToolBox->GetComponent( &hashString );

	m_bAddToHierarchy = addToHier;
	if( m_bAddToHierarchy )
	{
		AddToHierarchy();
	}
	m_bPositionUpdateLock = false;

	m_PhysicsActor = NULL;
	m_PhysicsActors.clear();
	m_PhysicsJoints.clear();
	m_PhysicsCloth.clear();

	m_CurrentScale.Set( 1.0f, 1.0f, 1.0f );

	m_bTransformIsDirty = true;

	m_IsPhysicsControlled = false;

	m_ForceMove = false;

	m_CollisionSurrogate = NULL;

	m_EnableGravityEvent = NULL;
	m_DisableGravityEvent = NULL;
	m_SetKinematicEvent = NULL;
	m_SetDynamicEvent = NULL;
	m_SetCollisionGroupEvent = NULL;
	m_SetTriggerEvent = NULL;
	m_ApplyForceEvent = NULL;
	m_ApplyLocalForceEvent = NULL;
	m_ApplyTorqueEvent = NULL;
	m_ApplyLocalTorqueEvent = NULL;
	m_SetLinearVelocityEvent = NULL;
	m_SetAngularVelocityEvent = NULL;
	m_SetLinearMomentumEvent = NULL;
	m_SetAngularMomentumEvent = NULL;
	m_SetLinearDampingEvent = NULL;
	m_SetAngularDampingEvent = NULL;
	m_LockTranslationAxisEvent = NULL;
	m_LockRotationAxisEvent = NULL;
	m_EnablePhysicsAnimationsEvent = NULL;
	m_SetHierarchalDataEvent = NULL;
	m_SetActorVelocityByNameEvent = NULL;
	m_BreakAllJointsEvent = NULL;
	m_BreakJointEvent = NULL;
}

CPhysicsObject::~CPhysicsObject()
{
	DeInit();

	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

IComponent *CPhysicsObject::Create( int nArgs, va_list argptr )
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
			self = new CPhysicsObject( parentName, name, true );
		}
		else if( nArgs == 3 )
		{
			bAddToHier = va_arg( argptr, bool );
			self = new CPhysicsObject( parentName, name, bAddToHier );
		}
		else
		{
			EngineGetToolBox()->Log( LOGERROR, _T("Invalid # - %d passed for argument to Create Physics Object\n"), nArgs );
			return NULL;
		}
	}
	catch(...)
	{
		EngineGetToolBox()->Log( LOGERROR, _T("CPhysicsObject: Failed to create CPhysicsObject!\n") );
		return NULL;
	}

	return self;
}

void CPhysicsObject::Init()
{
	if( m_PhysicsActor != NULL )
	{
		ReleaseModel();
	}

	m_hsParentType.Init(_T("CEntity"));

	static CHashString actor( _T("Actor") );
	static CHashString hierarchal( _T("Hierarchal") );
	static CHashString bullet( _T("Bullet") );
	static CHashString plane( _T("Plane") );

	if( m_Type == actor || m_Type == hierarchal )
	{
		// Load File
		LoadActorFromFile( m_File, false );
	}
	if( m_Type == bullet )
	{
		// Load File
		LoadActorFromFile( m_File, true );
	}	
	if( m_Type == plane )
	{
		BuildPlaneActor();
	}

	/*
	// With physics actors not getting created until the next PhysManager update this won't get called...
	// do not scale planes
	if( m_Type != plane )
	{
		// set the scale
		Vec3 parentScale;
		static DWORD msgHash_GetGlobalScale = CHashString(_T("GetGlobalScale")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetGlobalScale, sizeof(Vec3), &parentScale, GetParentName(), &m_hsParentType );
		SetScale( parentScale );
	}
	*/
}

bool CPhysicsObject::Update()
{
	if( m_PhysicsActors.size() <= 0 || m_PhysicsActor == NULL )
	{
		return false;
	}

	if( m_bTransformIsDirty )
	{
		SetTransform();
		m_bTransformIsDirty = false;
	}

	// update actors
	static CHashString hierarchal( _T("Hierarchal") );
	if( m_Type == hierarchal )
	{
		if( m_IsPhysicsControlled )
			UpdateHierarchalActors();
		else
		{
			// skeleton info is a list of BONEINFO structs
			BONELIST bones;
			SKELETONINFO skeleton;
			skeleton.bones = bones;

			static DWORD msgHash_GetSkeletonPose = CHashString(_T("GetSkeletonPose")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetSkeletonPose, sizeof(SKELETONINFO), &skeleton, GetParentName() );

			BONELIST::iterator iter = skeleton.bones.begin();
			while( iter != skeleton.bones.end() )
			{
				BONEINFO currentBone = *iter;
				CPhysicsActor* matchingActor = (CPhysicsActor*) GetActorByName( currentBone.bonename );

				if( matchingActor != NULL )
				{
					matchingActor->SetFromBoneData( &currentBone );
				}

				iter++;
			}
		}
	}
	else
		if( m_PhysicsActor != NULL )
		{
			UpdateActor( m_PhysicsActor );
		}
		
		
	// Update any cloth objects
	UpdateCloth();

	return true;
}

void CPhysicsObject::DeInit()
{
	// Remove any physics visualizations
	RemovePhysicsVisualization();
	
	// Release actors
	ReleaseModel();
}

void CPhysicsObject::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		// Read in the type
		StdString type;
		ar.Read( type );
		m_Type = type;
		// Read in the collision group
		int group;
		ar.Read( group );
		m_CollisionGroup = (COLLISIONGROUP)group;
		// Read in the contact group
		ar.Read( m_ContactGroup );
		// Read in the file
		ar.Read( m_File );
	}
	else
	{
		ar.Write( m_Type.GetString(), _T("type") );
		ar.Write( m_CollisionGroup, _T("collisiongroup") );
		ar.Write( m_ContactGroup, _T("contactgroup") );
		ar.Write( m_File, _T("fileName") );
	}
}

IHashString *CPhysicsObject::GetComponentType()
{
	static CHashString cpo( _T("CPhysicsObject") );
	return &cpo;
}

bool CPhysicsObject::IsKindOf( IHashString *compType )
{
	return ( compType->GetUniqueID() == GetComponentType()->GetUniqueID() );
}

void CPhysicsObject::SetCollisionGroup( COLLISIONGROUP group )
{   
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		CPhysicsActor* currentActor = *actorIter;
		currentActor->SetCollisionGroup( group );
		actorIter++;
	}	
}

void CPhysicsObject::SetContactGroup( unsigned int group )
{
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		CPhysicsActor* currentActor = *actorIter;
		currentActor->SetContactGroup( group );
		actorIter++;
	}	
}

void CPhysicsObject::SetShapeFlag( int flag, bool enable )
{
	NxShapeFlag nxFlag = (NxShapeFlag) flag;
	if( m_PhysicsActor == NULL )
	{
  		m_ToolBox->Log( LOGERROR, _T("CPhysicsObject::SetShapeFlag() Actor is NULL.\n") );
		return;
	}

	// Loop through shapes in the actor
	unsigned int numShapes = m_PhysicsActor->m_Actor->getNbShapes();
	NxShape*const* shapes = m_PhysicsActor->m_Actor->getShapes();
   	NxShape* shape;

	while( numShapes-- )
	{
		shape = shapes[numShapes];
		shape->setFlag( nxFlag, enable );
	}
}

void CPhysicsObject::LoadActorFromFile( const TCHAR* fileName, bool hasCCD )
{
	if( m_pManager )
	{
		// Queue up the load event
		LOADPHYSICSEVENT event;
		event.fileName = fileName;
		event.hasCCD = hasCCD;
		event.physObject = this;
		m_pManager->AddLoadEvent( event );
	}
}

void CPhysicsObject::BuildPlaneActor()
{
	// Build Plane
	NxPlaneShapeDesc* planeDesc = new NxPlaneShapeDesc();
	NxActorDesc* actorDesc = new NxActorDesc();

	// get the parent's transform
	Vec3 parentPos;
	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(parentPos), &parentPos, GetParentName(), &m_hsParentType );

	planeDesc->normal.set( 0, 1, 0 );
	planeDesc->d = parentPos.y;
	actorDesc->shapes.pushBack( planeDesc );

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

	// plane shouldn't move, not even once
	m_bTransformIsDirty = false;
}

void CPhysicsObject::ReleaseModel()
{
	ReleaseActors();
	ReleaseJoints();
	ReleaseCloth();
}

void CPhysicsObject::ReleaseActors()
{
	CPhysicsActor* currentActor = NULL;
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		currentActor = *actorIter;

		// NULL out userdata
		currentActor->SetUserData( NULL, NULL );

		// Queue up the delete event
		DELETEPHYSICSEVENT event;
		event.actor = currentActor;
		m_pManager->AddDeleteEvent( event );

		actorIter++;
	}

	m_PhysicsActors.clear();
	m_PhysicsActor = NULL;
}

void CPhysicsObject::ReleaseJoints()
{
	CPhysicsJoint* currentJoint = NULL;
	JOINTLIST::iterator jointIter = m_PhysicsJoints.begin();
	while( jointIter != m_PhysicsJoints.end() )
	{
		currentJoint = *jointIter;

		// NULL out userdata
		//currentJoint->SetUserData( NULL );

		// Queue up the delete event
		DELETEPHYSICSEVENT event;
		event.joint = currentJoint;
		m_pManager->AddDeleteEvent( event );

		jointIter++;
	}

	m_PhysicsJoints.clear();
}

void CPhysicsObject::ReleaseCloth()
{
	CPhysicsCloth* currentCloth = NULL;
	CLOTHLIST::iterator clothIter = m_PhysicsCloth.begin();
	while( clothIter != m_PhysicsCloth.end() )
	{
		currentCloth = *clothIter;

		// NULL out userdata
		//clothIter->SetUserData( NULL );

		// Queue up the delete event
		DELETEPHYSICSEVENT event;
		event.cloth = currentCloth;
		m_pManager->AddDeleteEvent( event );

		clothIter++;
	}

	m_PhysicsCloth.clear();
}

void CPhysicsObject::AddActor( IPhysicsActor* actor )
{
	if( actor == NULL )
	{
		m_ToolBox->Log( LOGERROR, _T("CPhysicsObject::AddActor() - Cannot add a NULL actor!\n") ); 
		return;
	}

	CPhysicsActor* physActor = (CPhysicsActor*) actor;

	// Create a new CPhysicsActor
	CPhysicsActor* physicsActor = new CPhysicsActor( physActor->GetNxActor() );
	physicsActor->SetUserData( this, physicsActor );

	// Assume only a single actor for the moment
	m_PhysicsActors.push_back( physicsActor );

	// Find the root actor.
	m_PhysicsActor = (CPhysicsActor*) FindRootActor();

	SetCollisionGroup( m_CollisionGroup );
	SetContactGroup( m_ContactGroup );

	if( m_pManager->ForceKinematic() )
	{
		physicsActor->ForceKinematic();
	}
}

void CPhysicsObject::SetActor( IPhysicsActor* actor )
{
	if( actor == NULL )
	{
		m_ToolBox->Log( LOGERROR, _T("CPhysicsObject::AddActor() - Cannot set a NULL actor!\n") ); 
		return;
	}
	CPhysicsActor* physActor = (CPhysicsActor*) actor;

	// clear old actors
	ReleaseModel();

	CPhysicsActor* physicsActor = new CPhysicsActor( physActor->GetNxActor() );
	physicsActor->SetUserData( this, physicsActor );

	m_PhysicsActors.push_back( physicsActor );
	m_PhysicsActor = physicsActor;

	SetCollisionGroup( m_CollisionGroup );
	SetContactGroup( m_ContactGroup );

	if( m_pManager->ForceKinematic() )
	{
		m_PhysicsActor->ForceKinematic();
	}
}

void CPhysicsObject::RemoveActor( IHashString* actorName )
{
	ACTORLIST::iterator iter = m_PhysicsActors.begin();
	while( iter != m_PhysicsActors.end())
	{
		CPhysicsActor* currentActor = *iter;
		if( actorName == currentActor->GetName() )
		{
			// Remove Actor
			// what if its the root?
		}
	}
}

CPhysicsActor* CPhysicsObject::GetActor( IHashString* actorName )
{
	ACTORLIST::iterator iter = m_PhysicsActors.begin();
	while( iter != m_PhysicsActors.end())
	{
		CPhysicsActor* currentActor = *iter;
		if( actorName == currentActor->GetName() )
		{
			return currentActor;
		}
		iter++;
	}

	return NULL;
}

void CPhysicsObject::AddJoint( IPhysicsJoint* joint )
{
	if( joint == NULL )
	{
		m_ToolBox->Log( LOGERROR, _T("CPhysicsObject::AddJoint() - Cannot add a NULL joint!\n") ); 
		return;
	}
	CPhysicsJoint* physJoint = (CPhysicsJoint*) joint;

	// Create a new CPhysicsActor
	CPhysicsJoint* physicsJoint = new CPhysicsJoint( physJoint->GetNxJoint() );

	BuildJointLinks( physicsJoint );

	// Assume only a single actor for the moment
	m_PhysicsJoints.push_back( physicsJoint );
}

void CPhysicsObject::RemoveJoint( IHashString* jointName )
{
	JOINTLIST::iterator iter = m_PhysicsJoints.begin();
	while( iter != m_PhysicsJoints.end())
	{
		CPhysicsJoint* currentJoint = *iter;
		if( jointName == currentJoint->GetName() )
		{
			// Break the joint
			currentJoint->SetBreakable( 0.0f, 0.0f );
		}
	}
}

IPhysicsJoint* CPhysicsObject::GetJoint( IHashString* jointName )
{
	JOINTLIST::iterator iter = m_PhysicsJoints.begin();
	while( iter != m_PhysicsJoints.end())
	{
		CPhysicsJoint* currentJoint = *iter;
		if( jointName == currentJoint->GetName() )
		{
			return currentJoint;
		}
	}

	return NULL;
}

void CPhysicsObject::AddCloth( IPhysicsCloth* cloth )
{
	CPhysicsCloth* physCloth = (CPhysicsCloth*) cloth;
	NxClothDesc* clothDesc = physCloth->GetNxClothDesc();

	// How to get the cloth's name...
	CHashString name( clothDesc->name );

	// Get the position of the node
	Matrix4x4 pose;
	BONEINFO boneInfo;
	boneInfo.bonename = &name;
	boneInfo.m_Matrix = &pose;

	static DWORD msgHash_GetBoneInfo = CHashString(_T("GetGlobalBoneInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetBoneInfo, sizeof(BONEINFO), &boneInfo, GetParentName() );

	// Create a new CPhysicsActor
	CPhysicsCloth* physicsCloth = new CPhysicsCloth( GetParentName(), &name, pose, *clothDesc );

	// Assume only a single actor for the moment
	m_PhysicsCloth.push_back( physicsCloth );
}

void CPhysicsObject::RemoveCloth( IHashString* clothName )
{
	CLOTHLIST::iterator iter = m_PhysicsCloth.begin();
	while( iter != m_PhysicsCloth.end())
	{
		CPhysicsCloth* currentCloth = *iter;
		if( clothName == currentCloth->GetName() )
		{
			// Remove the cloth
		}
	}
}

IPhysicsCloth* CPhysicsObject::GetCloth( IHashString* clothName )
{
	CLOTHLIST::iterator iter = m_PhysicsCloth.begin();
	while( iter != m_PhysicsCloth.end())
	{
		CPhysicsCloth* currentCloth = *iter;
		if( clothName == currentCloth->GetName() )
		{
			return currentCloth;
		}
	}

	return NULL;
}

COLLISIONGROUP CPhysicsObject::GetCollisionGroup()
{
	return m_CollisionGroup;
}

unsigned int CPhysicsObject::GetContactGroup()
{
	return m_ContactGroup;
}

void CPhysicsObject::AddPhysicsVisualization()
{
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		CPhysicsActor* currentActor = *actorIter;
		currentActor->AddVisualization();
		actorIter++;
	}	
}

void CPhysicsObject::RemovePhysicsVisualization()
{
	REMOVEOBJECTORIENTEDBOXPARAMS oobbParams;
	oobbParams.name = GetParentName();
	static DWORD msgHash_RemoveObjectOrientedBox = CHashString(_T("RemoveObjectOrientedBox")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveObjectOrientedBox, sizeof(REMOVEOBJECTORIENTEDBOXPARAMS), &oobbParams );

	REMOVEAXISALIGNEDBOXPARAMS aabbParams;
	aabbParams.name = GetParentName();
	static DWORD msgHash_RemoveAxisAlignedBox = CHashString(_T("RemoveAxisAlignedBox")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveAxisAlignedBox, sizeof(REMOVEAXISALIGNEDBOXPARAMS), &oobbParams );

	REMOVESPHEREPARAMS sphereParams;
	sphereParams.name = GetParentName();
	static DWORD msgHash_RemoveSphere = CHashString(_T("RemoveSphere")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveSphere, sizeof(REMOVESPHEREPARAMS), &sphereParams );
}

void CPhysicsObject::GetParentTransform( Matrix4x4& transform )
{
	Matrix4x4 parentTrans;
	static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof(parentTrans), &parentTrans, GetParentName(), &m_hsParentType );
	
	// remove scale from matrix
	parentTrans.ResetScale();

	transform = parentTrans.GetMatrix();
}

void CPhysicsObject::SetTransform()
{
	// Check that the actors are valid
	if( m_PhysicsActor == NULL )
	{
		return;
	}

	Matrix4x4 newTransform;
	GetParentTransform( newTransform );

	// test if kinematic
	bool isKinematic = m_PhysicsActor->IsKinematic();
	bool isDynamic = m_PhysicsActor->IsDynamic();

	bool forceMove = m_ForceMove | m_pManager->m_ForceMoveActors;

	// if the force move flag is enabled, statics still use their
	// own move func since they are a special case.
	if( forceMove && isDynamic )
	{
		Vec3 zero( 0, 0, 0 );

		m_PhysicsActor->SetGlobalPose( newTransform );

		if( !isKinematic )
		{
			m_PhysicsActor->SetLinearVelocity( zero );
			m_PhysicsActor->SetAngularVelocity( zero );
		}
		return;
	}

	// if the actor is Kinematic
	if( isKinematic )
	{
		m_PhysicsActor->MoveGlobalPose( newTransform );
		return;
	}

	// if the actor is Dynamic
	if( isDynamic && !isKinematic )
	{
		m_PhysicsActor->SetGlobalPose( newTransform );
		return;
	}

	// if the actor is Static
	if( !isDynamic )
	{
		MoveStaticActor( newTransform );
		return;
	}
}

void CPhysicsObject::GetTransform( Matrix4x4& transform )
{
	Matrix4x4 parentTrans;
	static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalTransform, sizeof(parentTrans), &parentTrans, GetParentName(), &m_hsParentType );
	
	// remove scale from matrix
	parentTrans.ResetScale();

	transform = parentTrans.GetMatrix();
}

void CPhysicsObject::SetScale( Vec3& scale )
{
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		CPhysicsActor* currentActor = *actorIter;
		currentActor->SetScale( scale );
		actorIter++;
	}	

	// Set the current scale to the new scale so we can unscale the scale
	m_CurrentScale = scale;
}

void CPhysicsObject::RescaleVector( Vec3& vec, Vec3& newScale )
{
	// undo the current scale factor so that the scale is 1
	vec.x /= m_CurrentScale.x;
	vec.y /= m_CurrentScale.y;
	vec.z /= m_CurrentScale.z;

	// rescale with the new scale
	vec.x *= newScale.x;
	vec.y *= newScale.y;
	vec.z *= newScale.z;
}

void CPhysicsObject::MoveStaticActor( Matrix4x4& newTransform )
{
	/*
	 * It no make sense
	 * NxMat34 rootActorGlobalTransform = m_PhysicsActor->GetGlobalPose();
	 */

	ACTORLIST::iterator iter = m_PhysicsActors.begin();
	while( iter != m_PhysicsActors.end() )
	{
		CPhysicsActor* actor = dynamic_cast<CPhysicsActor*>(*iter);
		actor->SetGlobalPose( newTransform );

		iter++;
	}
}

void CPhysicsObject::BuildJointLinks( IPhysicsJoint* joint )
{
	// set parent and child actors
	// get the joint's parent
	// get the joint's child
	IHashString* parentName = joint->GetParent();
	IHashString* childName = joint->GetChild();

	CPhysicsActor* parent = NULL;
	CPhysicsActor* child = NULL;

	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		CPhysicsActor* currentActor = *actorIter;
		IHashString* currentActorName = currentActor->GetName();

		if( currentActorName == parentName )
			parent = currentActor;

		if( currentActorName == childName )
			child = currentActor;

		if( parent != NULL && child != NULL )
			break;

		actorIter++;
	}

	if( parent == NULL || child == NULL )
	{
		// If we can't find the parent and child objects for the joint then we have a problem
		m_ToolBox->Log( LOGERROR, _T("CPhysicsObject::AddJoint() : Invalid Joint\n") );
		return;
	}

	// Set the parent's values
	parent->AddChild( child );
	// Set the child's values
	child->SetParent( parent );
}

IPhysicsActor* CPhysicsObject::FindRootActor()
{
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		CPhysicsActor* currentActor = *actorIter;
		
		if( currentActor->m_Parent == NULL )
			return currentActor;

		actorIter++;
	}

	// If there is no root then we have a problem
	m_ToolBox->Log( LOGERROR, _T("CPhysicsObject::FindRootActor() - No Root Actor!\n") ); 
	return NULL;
}

IPhysicsActor* CPhysicsObject::GetActorByName( IHashString* name )
{
	// find the actor matching the name in the actorlist and return it
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		CPhysicsActor* currentActor = *actorIter;
		
		if( currentActor->GetName() == name )
			return currentActor;

		actorIter++;
	}

	return NULL;
}

void CPhysicsObject::UpdateActor( IPhysicsActor* actor )
{
	// test if kinematic
	bool isKinematic = actor->IsKinematic();
	bool isDynamic = actor->IsDynamic();
	bool isSleeping = actor->IsSleeping();
	
	// if the actor is Dynamic
	if( isDynamic && !isSleeping && !isKinematic )
	{
		CPhysicsActor* physActor = (CPhysicsActor*) actor;

		m_bPositionUpdateLock = true;

		Vec3 position = actor->GetGlobalPosition();

		EulerAngle eulers;
		float actorRot[9];
		physActor->GetNxActor()->getGlobalOrientation().getColumnMajor( actorRot );

		Matrix3x3 rotation( actorRot );
		MathUtil math;
		math.MatrixToEuler( rotation, eulers );

		static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), &position, GetParentName(), &m_hsParentType );
		static DWORD msgHash_SetGlobalEuler = CHashString(_T("SetGlobalEuler")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetGlobalEuler, sizeof(EulerAngle), &eulers, GetParentName(), &m_hsParentType );

		m_bPositionUpdateLock = false;

		// since we just updated the transform we know it is up to date in physics
		// even though the setTransform messages set the dirty flag to true.
		m_bTransformIsDirty = false;
	}
}

void CPhysicsObject::UpdateHierarchalActors()
{
	BONELIST boneList;

	// do the following for each phys actor
	CPhysicsActor* currentActor = NULL;
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		currentActor = *actorIter;
		
		// get transform
		Matrix4x4* actorTransform = new Matrix4x4;
		currentActor->GetTransform( *actorTransform );

		// get bone name
		CHashString* boneName = new CHashString( currentActor->GetName() );

		// send a bone message to CHRO
		BONEINFO boneInfo;

		boneInfo.bonename = boneName;
		boneInfo.m_Matrix = actorTransform;

		boneList.push_back( boneInfo );

		actorIter++;
	}

	SKELETONINFO skeletonInfo;
	skeletonInfo.bones = boneList;

	static DWORD msgHash_SetSkeletonPose = CHashString(_T("SetSkeletonPose")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetSkeletonPose, sizeof(SKELETONINFO), &skeletonInfo, GetParentName() );

	// clean up SKELETONINFO
	BONELIST::iterator boneIter = boneList.begin();
	while( boneIter != boneList.end() )
	{
		BONEINFO currentBone = *boneIter;

		delete currentBone.bonename;
		delete currentBone.m_Matrix;

		boneIter++;
	}
}

void CPhysicsObject::UpdateCloth()
{
	CPhysicsCloth* currentCloth = NULL;
	CLOTHLIST::iterator clothIter = m_PhysicsCloth.begin();
	while( clothIter != m_PhysicsCloth.end() )
	{
		currentCloth = *clothIter;
		
		// get the mesh info and send it to the
		// corrosponding render object
		// Get the position of the node

		// cannot set the cloth's pose after its initial creation
		// we will have to rely on attach points for this...

		// either that or we delete and recreate teh cloth every time we move the object...

		clothIter++;
	}
}

void CPhysicsObject::ForceKinematic()
{
	// find the actor matching the name in the actorlist and return it
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		CPhysicsActor* currentActor = *actorIter;
		
		currentActor->ForceKinematic();

		actorIter++;
	}
}

void CPhysicsObject::RestoreDynamic()
{
	// find the actor matching the name in the actorlist and return it
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		CPhysicsActor* currentActor = *actorIter;
		
		currentActor->RestoreDynamic();

		actorIter++;
	}
}

void CPhysicsObject::ExecuteEventQueue()
{
	// Make sure the actor didn't get deleted while waiting for
	// the event queue to execute.
	if( m_PhysicsActor == NULL )
		return;

	if( m_EnableGravityEvent != NULL )
		EnableGravity();
	if( m_DisableGravityEvent != NULL )
		DisableGravity();
	if( m_SetKinematicEvent != NULL )
		SetKinematic();
	if( m_SetDynamicEvent != NULL )
		SetDynamic();
	if( m_SetCollisionGroupEvent != NULL )
		SetCollisionGroup();
	if( m_SetTriggerEvent != NULL )
		SetTrigger();
	if( m_ApplyForceEvent != NULL )
		ApplyForce();
	if( m_ApplyLocalForceEvent != NULL )
		ApplyLocalForce();
	if( m_ApplyTorqueEvent != NULL )
		ApplyTorque();
	if( m_ApplyLocalTorqueEvent != NULL )
		ApplyLocalTorque();
	if( m_SetLinearVelocityEvent != NULL )
		SetLinearVelocity();
	if( m_SetAngularVelocityEvent != NULL )
		SetAngularVelocity();
	if( m_SetLinearMomentumEvent != NULL )
		SetLinearMomentum();
	if( m_SetAngularMomentumEvent != NULL )
		SetAngularMomentum();
	if( m_SetLinearDampingEvent != NULL )
		SetLinearDamping();
	if( m_SetAngularDampingEvent != NULL )
		SetAngularDamping();
	if( m_LockTranslationAxisEvent != NULL )
		LockTranslationAxis();
	if( m_LockRotationAxisEvent != NULL )
		LockRotationAxis();
	if( m_EnablePhysicsAnimationsEvent != NULL )
		EnablePhysicsAnimations();
	if( m_SetHierarchalDataEvent != NULL )
		SetHierarchalData();
	if( m_SetActorVelocityByNameEvent != NULL )
		SetActorVelocity();
	if( m_BreakAllJointsEvent != NULL )
		BreakAllJoints();
	if( m_BreakJointEvent != NULL )
		BreakJoint();
}

void CPhysicsObject::EnableGravity()
{
	CPhysicsActor* currentActor = NULL;
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		currentActor = *actorIter;
		currentActor->SetBodyFlag( NX_BF_DISABLE_GRAVITY, false );
		actorIter++;
	}

	SAFEDELETE( m_EnableGravityEvent );
}

void CPhysicsObject::DisableGravity()
{
	CPhysicsActor* currentActor = NULL;
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		currentActor = *actorIter;
		currentActor->SetBodyFlag( NX_BF_DISABLE_GRAVITY, true );
		actorIter++;
	}

	SAFEDELETE( m_DisableGravityEvent );
}

void CPhysicsObject::SetKinematic()
{
	m_PhysicsActor->SetKinematic();

	SAFEDELETE( m_SetKinematicEvent );
}

void CPhysicsObject::SetDynamic()
{
	m_PhysicsActor->SetDynamic();

	SAFEDELETE( m_SetDynamicEvent );
}

void CPhysicsObject::SetCollisionGroup()
{
	CPhysicsActor* currentActor = NULL;
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		currentActor = *actorIter;
		currentActor->SetCollisionGroup( (COLLISIONGROUP)m_SetCollisionGroupEvent->group );
		actorIter++;
	}

	SAFEDELETE( m_SetCollisionGroupEvent );
}

void CPhysicsObject::SetTrigger()
{
	CPhysicsActor* currentActor = NULL;
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		currentActor = *actorIter;
		currentActor->SetTrigger( *m_SetTriggerEvent->triggerParams );
		actorIter++;
	}

	SAFEDELETE( m_SetTriggerEvent );
}

void CPhysicsObject::ApplyForce()
{
	m_PhysicsActor->AddForce( m_ApplyForceEvent->force );

	SAFEDELETE( m_ApplyForceEvent );
}

void CPhysicsObject::ApplyLocalForce()
{
	m_PhysicsActor->AddLocalForce( m_ApplyLocalForceEvent->force );

	SAFEDELETE( m_ApplyLocalForceEvent );
}

void CPhysicsObject::ApplyTorque()
{
	m_PhysicsActor->AddTorque( m_ApplyTorqueEvent->torque );

	SAFEDELETE( m_ApplyTorqueEvent );
}

void CPhysicsObject::ApplyLocalTorque()
{
	m_PhysicsActor->AddLocalTorque( m_ApplyLocalTorqueEvent->torque );

	SAFEDELETE( m_ApplyLocalTorqueEvent );
}

void CPhysicsObject::SetLinearVelocity()
{
	m_PhysicsActor->SetLinearVelocity( m_SetLinearVelocityEvent->velocity );

	SAFEDELETE( m_SetLinearVelocityEvent );
}

void CPhysicsObject::SetAngularVelocity()
{
	m_PhysicsActor->SetAngularVelocity( m_SetAngularVelocityEvent->velocity );

	SAFEDELETE( m_SetAngularVelocityEvent );
}

void CPhysicsObject::SetLinearMomentum()
{
	m_PhysicsActor->SetLinearMomentum( m_SetLinearMomentumEvent->momentum );

	SAFEDELETE( m_SetLinearMomentumEvent );
}

void CPhysicsObject::SetAngularMomentum()
{
	m_PhysicsActor->SetLinearMomentum( m_SetAngularMomentumEvent->momentum );

	SAFEDELETE( m_SetAngularMomentumEvent );
}

void CPhysicsObject::SetLinearDamping()
{
	CPhysicsActor* currentActor = NULL;
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		currentActor = *actorIter;
		currentActor->SetLinearDamping( m_SetLinearDampingEvent->damping );
		actorIter++;
	}

	SAFEDELETE( m_SetLinearDampingEvent );
}

void CPhysicsObject::SetAngularDamping()
{
	CPhysicsActor* currentActor = NULL;
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		currentActor = *actorIter;
		currentActor->SetAngularDamping( m_SetAngularDampingEvent->damping );
		actorIter++;
	}

	SAFEDELETE( m_SetAngularDampingEvent );
}

void CPhysicsObject::LockTranslationAxis()
{
	CPhysicsActor* currentActor = NULL;
	ACTORLIST::iterator iter = m_PhysicsActors.begin();
	while( iter != m_PhysicsActors.end() )
	{
		currentActor = *iter;
		currentActor->LockTranslationAxies( m_LockTranslationAxisEvent->lockAxies );
		iter++;
	}

	SAFEDELETE( m_LockTranslationAxisEvent );
}

void CPhysicsObject::LockRotationAxis()
{
	CPhysicsActor* currentActor = NULL;
	ACTORLIST::iterator iter = m_PhysicsActors.begin();
	while( iter != m_PhysicsActors.end() )
	{
		currentActor = *iter;
		currentActor->LockRotationAxies( m_LockRotationAxisEvent->lockAxies );
		iter++;
	}

	SAFEDELETE( m_LockRotationAxisEvent );
}

void CPhysicsObject::EnablePhysicsAnimations()
{
	if( m_IsPhysicsControlled )
	{
		// Set all CPhysicsActors to dynamic
		CPhysicsActor* currentActor = NULL;
		ACTORLIST::iterator iter = m_PhysicsActors.begin();
		while( iter != m_PhysicsActors.end() )
		{
			currentActor = *iter;
			currentActor->SetDynamic();
			iter++;
		}
	}
	else
	{
		// Set all CPhysicsActors to kinematic
		CPhysicsActor* currentActor = NULL;
		ACTORLIST::iterator iter = m_PhysicsActors.begin();
		while( iter != m_PhysicsActors.end() )
		{
			currentActor = *iter;
			currentActor->SetKinematic();
			iter++;
		}
	}
/*
	// TESTING
	// Break all joints
	CPhysicsJoint* currentJoint = NULL;
	JOINTLIST::iterator iter = m_PhysicsJoints.begin();
	while( iter != m_PhysicsJoints.end() )
	{
		currentJoint = *iter;
		currentJoint->SetBreakable( 0, 0 );
		iter++;
	}
	*/
/*
	CPhysicsActor* currentActor = NULL;
	ACTORLIST::iterator actorIter = m_PhysicsActors.begin();
	while( actorIter != m_PhysicsActors.end() )
	{
		Vec3 zero(0,0,0);
		currentActor = *actorIter;
		currentActor->SetLinearVelocity(zero);
		actorIter++;
	}
*/

	SAFEDELETE( m_EnablePhysicsAnimationsEvent );
}

void CPhysicsObject::SetHierarchalData()
{
	// skeleton info is a list of BONEINFO structs
	BONELIST bones = m_SetHierarchalDataEvent->skeletonInfo->bones;

	BONELIST::iterator iter = bones.begin();
	while( iter != bones.end() )
	{
		BONEINFO currentBone = *iter;
		CPhysicsActor* matchingActor = (CPhysicsActor*) GetActorByName( currentBone.bonename );
		
		if( matchingActor != NULL )
		{
			matchingActor->SetFromBoneData( &currentBone );
		}
		iter++;
	}

	SAFEDELETE( m_SetHierarchalDataEvent );
}

void CPhysicsObject::SetActorVelocity()
{
	CPhysicsActor* actor = GetActor( m_SetActorVelocityByNameEvent->actorForce->name );
	
	if( actor != NULL )
	{
		actor->SetLinearVelocity( *m_SetActorVelocityByNameEvent->actorForce->force );
	}

	SAFEDELETE( m_SetActorVelocityByNameEvent );
}

void CPhysicsObject::BreakAllJoints()
{
	// Break all joints
	CPhysicsJoint* currentJoint = NULL;
	JOINTLIST::iterator iter = m_PhysicsJoints.begin();
	while( iter != m_PhysicsJoints.end() )
	{
		currentJoint = *iter;
		currentJoint->SetBreakable( 0, 0 );
		iter++;
	}

	SAFEDELETE( m_BreakAllJointsEvent );
}

void CPhysicsObject::BreakJoint()
{
	CPhysicsJoint* currentJoint = NULL;
	JOINTLIST::iterator iter = m_PhysicsJoints.begin();
	while( iter != m_PhysicsJoints.end() )
	{
		currentJoint = *iter;
		if( currentJoint->GetName()->GetUniqueID() == m_BreakJointEvent->jointName->GetUniqueID())
		{
			currentJoint->SetBreakable( 0, 0 );
			break;
		}

		iter++;
	}

	SAFEDELETE( m_BreakJointEvent );
}

bool CPhysicsObject::HasCollisionSurrogate()
{
	return ( m_CollisionSurrogate != NULL );
}

IHashString* CPhysicsObject::GetCollisionSurrogate()
{
	return m_CollisionSurrogate;
}

// ------------------ Messages ------------------ //
DWORD CPhysicsObject::OnApplyForce( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( sizeof(Vec3), size );
	Vec3* force = (Vec3*)params;

	// Make sure that we are getting a valid vec3
	NxVec3 tmp( force->x, force->y, force->z );
	if( !tmp.isFinite() )
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
		return MSG_ERROR;
	}

	m_ApplyForceEvent = new APPLYFORCEEVENT();
	m_ApplyForceEvent->force = *force;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnApplyLocalForce( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(Vec3), size );
	Vec3* force = (Vec3*)params;

	// Make sure that we are getting a valid vec3
	NxVec3 tmp( force->x, force->y, force->z );
	if( !tmp.isFinite() )
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
		return MSG_ERROR;
	}

	m_ApplyLocalForceEvent = new APPLYLOCALFORCEEVENT();
	m_ApplyLocalForceEvent->force = *force;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnApplyTorque( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(Vec3), size );
	Vec3* torque = (Vec3*)params;

	// Make sure that we are getting a valid vec3
	NxVec3 tmp( torque->x, torque->y, torque->z );
	if( !tmp.isFinite() )
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
		return MSG_ERROR;
	}

	m_ApplyTorqueEvent = new APPLYTORQUEEVENT();
	m_ApplyTorqueEvent->torque = *torque;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnApplyLocalTorque( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(Vec3), size );
	Vec3* torque = (Vec3*)params;

	// Make sure that we are getting a valid vec3
	NxVec3 tmp( torque->x, torque->y, torque->z );
	if( !tmp.isFinite() )
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
		return MSG_ERROR;
	}

	m_ApplyLocalTorqueEvent = new APPLYLOCALTORQUEEVENT();
	m_ApplyLocalTorqueEvent->torque = *torque;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnSetLinearVelocity( DWORD size, void *params )
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

	m_SetLinearVelocityEvent = new SETLINEARVELOCITYEVENT();
	m_SetLinearVelocityEvent->velocity = *velocity;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnSetAngularVelocity( DWORD size, void* params )
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

	m_SetAngularVelocityEvent = new SETANGULARVELOCITYEVENT();
	m_SetAngularVelocityEvent->velocity = *velocity;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnGetLinearVelocity( DWORD size, void* params )
{
	Vec3* velocity;
	VERIFY_MESSAGE_SIZE( sizeof(Vec3), size );
	velocity = (Vec3*)params;

	if( m_PhysicsActor == NULL )
	{
		velocity->Set( 0, 0, 0 );
	}
	else
	{
		NxVec3 actorVelocity;
		NxActor* actor = NULL;

		actorVelocity = m_PhysicsActor->m_Actor->getLinearVelocity();

		velocity->Set( actorVelocity.x, actorVelocity.y, actorVelocity.z );
	}

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnGetAngularVelocity( DWORD size, void* params )
{
	Vec3* velocity;
	VERIFY_MESSAGE_SIZE( sizeof(Vec3), size );
	velocity = (Vec3*)params;

	if( m_PhysicsActor == NULL )
	{
		velocity->Set( 0, 0, 0 );
	}
	else
	{
		NxVec3 actorVelocity;
		NxActor* actor = NULL;

		actorVelocity = m_PhysicsActor->m_Actor->getAngularVelocity();
	    
		velocity->Set( actorVelocity.x, actorVelocity.y, actorVelocity.z );
	}
	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnSetLinearMomentum( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(Vec3), size );
	Vec3* momentum = (Vec3*)params;

	// Make sure that we are getting a valid vec3
	NxVec3 tmp( momentum->x, momentum->y, momentum->z );
	if( !tmp.isFinite() )
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
		return MSG_ERROR;
	}

	m_SetLinearMomentumEvent = new SETLINEARMOMENTUMEVENT();
	m_SetLinearMomentumEvent->momentum = *momentum;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnSetAngularMomentum( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(Vec3), size );
	Vec3* momentum = (Vec3*)params;

	// Make sure that we are getting a valid vec3
	NxVec3 tmp( momentum->x, momentum->y, momentum->z );
	if( !tmp.isFinite() )
	{
		m_ToolBox->Log( LOGERROR, _T("Invalid vector given to Physics!\n"));
		return MSG_ERROR;
	}

	m_SetAngularMomentumEvent = new SETANGULARMOMENTUMEVENT();
	m_SetAngularMomentumEvent->momentum = *momentum;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnGetLinearMomentum( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(Vec3), size );
	Vec3* momentum = (Vec3*)params;

	if( m_PhysicsActor == NULL )
	{
		momentum->Set( 0, 0, 0 );
	}
	else
	{
		NxVec3 actorMomentum = m_PhysicsActor->m_Actor->getLinearMomentum();
		momentum->Set( actorMomentum.x, actorMomentum.y, actorMomentum.z );
	}
	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnGetAngularMomentum( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(Vec3), size );
	Vec3* momentum = (Vec3*)params;

	if( m_PhysicsActor == NULL )
	{
		momentum->Set( 0, 0, 0 );
	}
	else
	{
		NxVec3 actorMomentum = m_PhysicsActor->m_Actor->getAngularMomentum();
		momentum->Set( actorMomentum.x, actorMomentum.y, actorMomentum.z );
	}
	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnSetLinearDamping( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(float), size );
	float damping = *(float*)params;

	m_SetLinearDampingEvent = new SETLINEARDAMPINGEVENT();
	m_SetLinearDampingEvent->damping = damping;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnSetAngularDamping( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(float), size );
	float damping = *(float*)params;

	m_SetAngularDampingEvent = new SETANGULARDAMPINGEVENT();
	m_SetAngularDampingEvent->damping = damping;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnEnableVisualization( DWORD size, void* params )
{
	AddPhysicsVisualization();

	return MSG_HANDLED_PROCEED;
}

DWORD CPhysicsObject::OnDisableVisualization( DWORD size, void* params )
{
	RemovePhysicsVisualization();

	return MSG_HANDLED_PROCEED;
}

DWORD CPhysicsObject::OnReinitPhysics( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, 0 );

	Init();

	return MSG_HANDLED_PROCEED;
}

DWORD CPhysicsObject::OnSetKinematic( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( 0, size );

	m_SetKinematicEvent = new SETKINEMATICEVENT();

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnSetDynamic( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( 0, size );

	m_SetDynamicEvent = new SETDYNAMICEVENT();

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnLockTranslationAxis( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(LOCKAXIES), size );
	LOCKAXIES* axis = (LOCKAXIES*)params;

	m_LockTranslationAxisEvent = new LOCKTRANSLATIONAXISEVENT();
	m_LockTranslationAxisEvent->lockAxies = new LOCKAXIES();
	m_LockTranslationAxisEvent->lockAxies->xAxis = axis->xAxis;
	m_LockTranslationAxisEvent->lockAxies->yAxis = axis->yAxis;
	m_LockTranslationAxisEvent->lockAxies->zAxis = axis->zAxis;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnLockRotationAxis( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(LOCKAXIES), size );
	LOCKAXIES* axis = (LOCKAXIES*)params;

	m_LockRotationAxisEvent = new LOCKROTATIONAXISEVENT();
	m_LockRotationAxisEvent->lockAxies = new LOCKAXIES();
	m_LockRotationAxisEvent->lockAxies->xAxis = axis->xAxis;
	m_LockRotationAxisEvent->lockAxies->yAxis = axis->yAxis;
	m_LockRotationAxisEvent->lockAxies->zAxis = axis->zAxis;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnEnablePhysicsAnimations( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(bool), size );
	m_IsPhysicsControlled = *(bool*)params;

	m_EnablePhysicsAnimationsEvent = new ENABLEPHYSICSANIMATIONSEVENT();

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnSetHierarchalData( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(SKELETONINFO), size );
	SKELETONINFO* skeletonInfo = (SKELETONINFO*)params;

	m_SetHierarchalDataEvent = new SETHIERARCHALDATAEVENT();
	m_SetHierarchalDataEvent->skeletonInfo = skeletonInfo;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnForceSetGlobalTransform( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( 0, size );

	m_ForceMove = true;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnGetActorPositionByName( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(Vec3), size );
	ACTORINFO* actorInfo = (ACTORINFO*)params;
	
	CPhysicsActor* actor = GetActor(actorInfo->name);

	if( actor != NULL )
	{
		Vec3 actorPos = actor->GetGlobalPosition();
		actorInfo->pos->Set( actorPos.x, actorPos.y, actorPos.z );
	}

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnSetActorVelocityByName( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE(sizeof(ACTORFORCE), size);
	ACTORFORCE* actorForce = (ACTORFORCE*)params;
	
	m_SetActorVelocityByNameEvent = new SETACTORVELOCITYBYNAMEEVENT();
	m_SetActorVelocityByNameEvent->actorForce = actorForce;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnGetActorVelocityByName( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE(sizeof(ACTORFORCE), size);
	ACTORFORCE* actorForce = (ACTORFORCE*)params;

	CPhysicsActor* actor = GetActor( actorForce->name );

	if( actor != NULL )
	{
		actor->GetLinearVelocity( *actorForce->force );
	}

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnSetCollisionGroup( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( sizeof(int), size );
	int group = *(int*)params;

	if( group < 0 || group >= 32 )
	{
		m_ToolBox->Log( LOGERROR, _T("CPhysicsObject::OnSetCollisionGroup() Invalid collision group! Group must be between 1-31!\n") );
		return MSG_HANDLED_STOP;
	}

	m_SetCollisionGroupEvent = new SETCOLLISIONGROUPEVENT();
	m_SetCollisionGroupEvent->group = group;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnBreakAllJoints( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( 0, size );

	m_BreakAllJointsEvent = new BREAKALLJOINTSEVENT();

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnBreakJoint( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE(sizeof(IHashString), size);
	IHashString* jointName = (IHashString*)params;

	m_BreakJointEvent = new BREAKJOINTEVENT();
	m_BreakJointEvent->jointName = jointName;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnGetClothPhysicsGeometry( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE(sizeof(GETCLOTHPHYSICSGEOMETRYPARAMS), size);
	GETCLOTHPHYSICSGEOMETRYPARAMS* clothgeom = (GETCLOTHPHYSICSGEOMETRYPARAMS*)params;

	// assume only a single cloth for now
	CLOTHLIST::iterator clothIter = m_PhysicsCloth.begin();
	CPhysicsCloth* cloth = *clothIter;

	if( m_PhysicsCloth.size() == 0 )
	{
		clothgeom->pGeometry = NULL;
		return MSG_ERROR;
	}

	cloth->SetGeometry( clothgeom->pGeometry );

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnEnableGravity( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE(0, size);

	m_EnableGravityEvent = new ENABLEGRAVITYEVENT();
	
	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnDisableGravity( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE(0, size);

	m_DisableGravityEvent = new DISABLEGRAVITYEVENT();
	
	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnEnableTrigger( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE(sizeof(TRIGGERPARAMS), size);
	TRIGGERPARAMS* triggerParams = (TRIGGERPARAMS*)params;
	
	m_SetTriggerEvent = new SETTRIGGEREVENT();
	m_SetTriggerEvent->triggerParams = new TRIGGERPARAMS();
	m_SetTriggerEvent->triggerParams->triggerOnEnter = triggerParams->triggerOnEnter;
	m_SetTriggerEvent->triggerParams->triggerOnExit = triggerParams->triggerOnExit;
	m_SetTriggerEvent->triggerParams->triggerOnStay = triggerParams->triggerOnStay;
	
	return MSG_HANDLED_STOP;
}

DWORD CPhysicsObject::OnSetCollisionSurrogate( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE(sizeof(IHashString*), size);
	IHashString* surrogate = (IHashString*)params;

	m_CollisionSurrogate = surrogate;

	return MSG_HANDLED_STOP;
}
