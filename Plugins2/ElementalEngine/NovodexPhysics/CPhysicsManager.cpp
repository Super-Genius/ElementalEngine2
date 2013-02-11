///==========================================================================
/// \file	CPhysicsManager.cpp
/// \brief	Physics implementation of the IManager interface
/// \date	2-18-2005
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
#include "PerformanceProfiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Decleration for lua bindings
TOLUA_API int tolua_NovodexPhysicsScript_open( lua_State* tolua_S );

// Register the singleton
REGISTER_COMPONENT_SINGLETON(CPhysicsManager);

// ------------------ Messages ------------------ //
REGISTER_MESSAGE_HANDLER( Start, OnStart, CPhysicsManager );
REGISTER_MESSAGE_HANDLER( Stop, OnStop, CPhysicsManager );
REGISTER_MESSAGE_HANDLER( SetCollisionPair, OnSetCollisionPair, CPhysicsManager );
REGISTER_MESSAGE_HANDLER( SetContactPair, OnSetContactPair, CPhysicsManager );
REGISTER_MESSAGE_HANDLER( Raycast, OnRaycast, CPhysicsManager );
REGISTER_MESSAGE_HANDLER( RaycastArchive, OnRaycastArchive, CPhysicsManager );
REGISTER_MESSAGE_HANDLER( RaycastList, OnRaycastList, CPhysicsManager );
REGISTER_MESSAGE_HANDLER( IntersectAABB, OnIntersectAABB, CPhysicsManager );
REGISTER_MESSAGE_HANDLER( IntersectOOBB, OnIntersectOOBB, CPhysicsManager );
REGISTER_MESSAGE_HANDLER( AddNovodexImporter, OnCachePhysicsObject, CPhysicsManager );
REGISTER_MESSAGE_HANDLER( ToggleRemoteVisualizer, OnToggleRemoteVisualizer, CPhysicsManager );
REGISTER_MESSAGE_HANDLER( ShutDownPhysics, OnShutDownPhysics, CPhysicsManager );
REGISTER_MESSAGE_HANDLER( SetGravity, OnSetGravity, CPhysicsManager );
REGISTER_MESSAGE_HANDLER( SetPhysicsScale, OnSetPhysicsScale, CPhysicsManager );
REGISTER_MESSAGE_HANDLER( SetForceMoveActors, OnSetForceMoveActors, CPhysicsManager );

REGISTER_MESSAGE_HANDLER( ForceKinematic, OnForceKinematic, CPhysicsManager );

CPhysicsManager::CPhysicsManager() : 
	IPhysicsManager(_T("CPhysicsManager"))
{
	// Register LUA bindings
	m_LuaState = NULL;
	RegisterLuaDefinitions();

	// Set the default initial physics variables
	m_Scene					= NULL;
	m_PhysicsSDK			= NULL;

	m_Scale					= 100.0f; // This needs to be read from a config file
	m_Gravity.Set( 0.0f, -9.8f, 0.0f * m_Scale );

	m_DefaultRestitution = 0.0f;
	m_DefaultDynamicFriction = 1.0f;
	m_DefaultStaticFriction = 3.0f;

	m_ErrorStream = NULL;

	m_RemoteDebugger = NULL;
	m_RemoteDebuggerUserAllocator = NULL;

	m_IsRunning = true;
	m_FirstUpdate = true;

	m_ForceMoveActors = false;

	m_ForceKinematic = false;

	Init();
}

CPhysicsManager::~CPhysicsManager()
{
	// Clean up any cached physics objects
	CACHEDPHYSICSOBJECTSMAP::iterator iter = m_CachedPhysicsObjects.begin();
	for( ; iter != m_CachedPhysicsObjects.end(); iter++ )
	{
		NXU::releaseCollection( iter->second );
	}

	DeInit();
	NXU::releasePersistentMemory();
}

IComponent *CPhysicsManager::Create( int nArgs, va_list argptr )
{
	// Return the singleton instance of this class
  	return SINGLETONINSTANCE( CPhysicsManager );
}

bool CPhysicsManager::Init()
{
	// Create the physics SDK
	m_ErrorStream = new CErrorStream();
    m_PhysicsSDK = NxCreatePhysicsSDK( NX_PHYSICS_SDK_VERSION, 0, m_ErrorStream );
	
	if( m_PhysicsSDK == NULL )
	{
		m_ToolBox->Log( LOGFATALERROR, _T("CPhysicsManager: Failed to initialize Physics Engine!\n") );
		return false;
	}

	OnToggleRemoteVisualizer( 0, NULL );

	float invScale = ( 1/m_Scale );
 	m_PhysicsSDK->setParameter( NX_SKIN_WIDTH, 0.05f*invScale );
	//m_PhysicsSDK->setParameter( NX_SKIN_WIDTH, 0.05f );
	m_PhysicsSDK->setParameter( NX_CONTINUOUS_CD, true );

	// Create the physics scene
	ResetScene();

	// Set the controll group not to collide with objects or terrain
	// by default.
	COLLISIONPAIR collsionPair;
	collsionPair.enable = false;

	collsionPair.groupA = CONTROL;
	collsionPair.groupB = TERRAIN;
	OnSetCollisionPair( sizeof(COLLISIONPAIR), &collsionPair);

	collsionPair.groupA = CONTROL;
	collsionPair.groupB = OBJECTS;
	OnSetCollisionPair( sizeof(COLLISIONPAIR), &collsionPair);

	return CManager::Init();
}

void CPhysicsManager::Update(DWORD tickCount)
{	
	// Get the map of managed physics objects
	static CHashString physicsObjectType( _T("CPhysicsObject") );
	static CHashString character( _T("Character") );

	IDTOOBJECTMAP::iterator iter;
	IDTOOBJECTMAP* physicsObjectMap = GetObjectMap( &physicsObjectType );

	if( m_IsRunning )
	{
		// Get the results from the last simulation tick
		// Don't run this the first time through since we have to have
		// called RunPhysics() at least once.
		if( !m_FirstUpdate )
		{
			GetPhysicsResults();
		}
		else
		{
			m_FirstUpdate = false;
		}
	}

	// We want creates and deletes to happen even when physics isn't updating.
	ExecuteDeleteEventQueue();
	ExecuteCreateEventQueue();
	ExecuteLoadEventQueue();

	if( m_IsRunning )
	{
		if( physicsObjectMap != NULL )
		{
			CPhysicsObject* currentPhysicsObject;
			for( iter = physicsObjectMap->begin(); iter != physicsObjectMap->end(); iter++ )
			{
				currentPhysicsObject = (CPhysicsObject*)( iter->second );
				currentPhysicsObject->Update();
				currentPhysicsObject->ExecuteEventQueue();
			}
		}

		m_TriggerReport.ExecuteTriggerEventQueue();
		m_ContactReport.ExecuteContactEventQueue();

		RunPhysics();
	}
}

bool CPhysicsManager::DeInit()
{
	// Complete any physics updates
	if (m_Scene)
	{
		if( m_Scene->checkResults( NX_RIGID_BODY_FINISHED ) )
		{
			GetPhysicsResults();
			m_Scene->simulate( 1 );
		}
	}
	GetPhysicsResults();

	// Flush out the delete event queue
	ExecuteDeleteEventQueue();

	// Release the Scene
	if( m_Scene != NULL )
	{
		m_PhysicsSDK->releaseScene( *m_Scene );
		m_Scene = NULL;
	}

	// Release the Physics Engine
	if( m_PhysicsSDK != NULL )
	{
		NxReleasePhysicsSDK( m_PhysicsSDK );
		m_PhysicsSDK = NULL;
	}

	SAFE_DELETE( m_ErrorStream );
	SAFE_DELETE( m_RemoteDebuggerUserAllocator );

	return CManager::DeInit();
}

void CPhysicsManager::SetGravity( Vec3& gravity )
{
	// Set the gravity variable
	m_Gravity.Set( gravity );

	// Set the gravity in the scene
	m_Scene->setGravity( NxVec3( m_Gravity.x, m_Gravity.y, m_Gravity.z ) );
}

Vec3* CPhysicsManager::GetGravity()
{
	return &m_Gravity;
}

void CPhysicsManager::SetScale( float scale )
{
	m_Scale = scale;

	// Reset physics parameters for the new scale
	SetGravity( m_Gravity * m_Scale );

	// Reset the scene
	DeInit();
	Init();
}

float CPhysicsManager::GetScale()
{
	return m_Scale;
}

void CPhysicsManager::ResetScene()
{
	GetPhysicsResults();
	RunPhysics();
	GetPhysicsResults();

	if( m_Scene == NULL )
	{
		NxSceneDesc sceneDesc;
		NxVec3 gravityVec( m_Gravity.x, m_Gravity.y, m_Gravity.z );
		sceneDesc.gravity = gravityVec * m_Scale;
		sceneDesc.simType = NX_SIMULATION_SW;
		sceneDesc.upAxis				= NX_Y;
		sceneDesc.userContactReport		= &m_ContactReport;
		sceneDesc.userTriggerReport		= &m_TriggerReport;
		sceneDesc.userNotify			= &m_PhysicsReport;
		//sceneDesc.flags					|= NX_SF_ENABLE_MULTITHREAD;

		m_Scene = m_PhysicsSDK->createScene( sceneDesc );
		
		// Set timestep
		//NxReal maxTimeStep = 1.0f/60.0f;
		//m_Scene->setTiming( maxTimeStep, 8, NX_TIMESTEP_VARIABLE );
		//m_Scene->setTiming( maxTimeStep, 8, NX_TIMESTEP_FIXED );

		if( m_Scene != NULL )
		{			
			NxMaterial* defaultMaterial = m_Scene->getMaterialFromIndex(0);
			defaultMaterial->setRestitution( m_DefaultRestitution );
			defaultMaterial->setStaticFriction( m_DefaultStaticFriction );
			defaultMaterial->setDynamicFriction( m_DefaultDynamicFriction );
		}
	}
}

void CPhysicsManager::GetPhysicsResults()
{
	// Get the results of the last PhysX simulation update
	if( m_Scene != NULL )
	{
		m_Scene->fetchResults( NX_RIGID_BODY_FINISHED, true );
	}
}

void CPhysicsManager::RunPhysics()
{
	if( m_Scene == NULL )
	{
		return;
	}

	if( !m_IsRunning )
	{
		m_Scene->simulate( 0 );
		m_Scene->flushStream();
		return;
	}

	// Update the time step
	static CHashString hsGameTime(GAMEDELTATIMER);
	float deltaT = m_Timer->GetTimeDelta( &hsGameTime );

	// Start collision and dynamics for delta time since the last frame
    m_Scene->simulate( deltaT );
    m_Scene->flushStream();
}

NxActor* CPhysicsManager::CreateActor( NxActorDesc& actorDesc )
{
	NxActor* actor = m_Scene->createActor( actorDesc );
	return actor;
}

void CPhysicsManager::ReleaseActor( CPhysicsActor* actor )
{
	m_Scene->releaseActor( *actor->m_Actor );
}

NxJoint* CPhysicsManager::CreateJoint( NxJointDesc& jointDesc )
{
	NxJoint* joint = m_Scene->createJoint( jointDesc );
	return joint;
}

void CPhysicsManager::ReleaseJoint( CPhysicsJoint* joint )
{
	m_Scene->releaseJoint( *joint->m_Joint );
}

NxCloth* CPhysicsManager::CreateCloth( NxClothDesc& clothDesc )
{
    NxCloth* cloth = m_Scene->createCloth( clothDesc );
    return cloth;
}

void CPhysicsManager::ReleaseCloth( CPhysicsCloth* cloth )
{
}

NxHeightField* CPhysicsManager::CreateHeightField( NxHeightFieldDesc& heightFieldDesc )
{
	return m_PhysicsSDK->createHeightField( heightFieldDesc );
}

void CPhysicsManager::RegisterLuaDefinitions()
{
	static DWORD msgHash_GetMasterScriptState = CHashString(_T("GetMasterScriptState")).GetUniqueID();
	DWORD result = m_ToolBox->SendMessage(msgHash_GetMasterScriptState, sizeof(lua_State*), &m_LuaState );
	if( result != MSG_HANDLED )
	{
		EngineGetToolBox()->Log( LOGWARNING, _T("Error missing Master Script State Data\n") );
    }
	else
	{
		// register our additional structures/handlers with LUA master
		tolua_NovodexPhysicsScript_open( m_LuaState );
	}
}


unsigned int CPhysicsManager::CollisionGroupToMask( COLLISIONGROUP group )
{
	NxU32 defaultCollisionMask = 0x00000000;

	if( group == ALL )
	{
		return -1;
	}

	// Add the desired group to the collision mask
	defaultCollisionMask |= (1 << group);

	// Remove the NOCOLLISION group from the collision mask
	defaultCollisionMask &= ~(1 << NOCOLLISION);

	return defaultCollisionMask;
}

NxScene* CPhysicsManager::GetScene()
{
	return m_Scene;
}

NxPhysicsSDK* CPhysicsManager::GetPhysicsSDK()
{
	return m_PhysicsSDK;
}

NXU::NxuPhysicsCollection* CPhysicsManager::GetCachedPhysicsObject( const TCHAR* filename )
{
	StdString lowerName = filename;
	lowerName.MakeSafeFileName();
	CHashString hashName( lowerName );
	
	// Check to see if the object is in the map
	// if its not then add it to the map
	if( m_CachedPhysicsObjects[hashName.GetUniqueID()] == NULL )
	{
		LOADFILEEXTPARAMS lfep;
		lfep.fileName = (TCHAR*)lowerName.c_str();
		lfep.bInternalLoad = true;
		static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep );	
		if( retval != MSG_HANDLED )
		{
			m_ToolBox->Log( LOGERROR, _T("CPhysicsManager: Failed to cache physics file.\n") );
			return NULL;
		}
	}

	// Grab the physics file from map
	return m_CachedPhysicsObjects[ hashName.GetUniqueID() ];
}

bool CPhysicsManager::IsContactPair( unsigned int groupA, unsigned int groupB )
{
	unsigned int contactFlag = m_Scene->getActorGroupPairFlags ( groupA, groupB );

	if( contactFlag == 2 )
	{
		return true;
	}

	return false;
}

void CPhysicsManager::AddDeleteEvent( DELETEPHYSICSEVENT event )
{
	m_DeleteEventQueue.push( event );
}

void CPhysicsManager::AddCreateEvent( CREATEPHYSICSEVENT event )
{
	m_CreateEventQueue.push( event );
}

void CPhysicsManager::AddLoadEvent( LOADPHYSICSEVENT event )
{
	m_LoadEventQueue.push( event );
}

void CPhysicsManager::ExecuteDeleteEventQueue()
{
	while ( !m_DeleteEventQueue.empty() )
	{
		DELETEPHYSICSEVENT* pDeleteEvent = &m_DeleteEventQueue.front();
	
		if( pDeleteEvent->actor != NULL )
		{
			CPhysicsActor* actor = pDeleteEvent->actor;
			ReleaseActor( actor );
			SAFE_DELETE( actor );
		}

		if( pDeleteEvent->joint != NULL )
		{
			CPhysicsJoint* joint = pDeleteEvent->joint;
			ReleaseJoint( joint );
			SAFE_DELETE( joint );
		}

		if( pDeleteEvent->cloth != NULL )
		{
			CPhysicsCloth* cloth = pDeleteEvent->cloth;
			ReleaseCloth( cloth );
			SAFE_DELETE( cloth );
		}
	
		m_DeleteEventQueue.pop();
	}
}

void CPhysicsManager::ExecuteCreateEventQueue()
{
	while ( !m_CreateEventQueue.empty() )
	{
		CREATEPHYSICSEVENT* pCreateEvent = &m_CreateEventQueue.front();

		if( pCreateEvent->physObject != NULL )
		{
			if( pCreateEvent->actorDesc != NULL )
			{
				if( !pCreateEvent->actorDesc->isValid() )
				{
					m_ToolBox->Log( LOGERROR, _T("CPhysicsManager::ExecuteCreateEventQueue() Invalid actor on %s.\n"), pCreateEvent->physObject->GetName()->GetString() );
					return;
				}

				NxActor* actor = m_Scene->createActor( *pCreateEvent->actorDesc );

				if( actor == NULL )
				{
					m_ToolBox->Log( LOGERROR, _T("CPhysicsManager::ExecuteCreateEventQueue() Failed to create physics actor.\n") );
					return;
				}

				pCreateEvent->physObject->SetActor( &CPhysicsActor( actor ) );

				// Need to deallocate actor descriptor shapes
				for( unsigned int i = 0; i < pCreateEvent->actorDesc->shapes.size(); i++ )
				{
					SAFEDELETE( pCreateEvent->actorDesc->shapes[i] );
				}

				// Need to deallocate body descriptor
				if( pCreateEvent->actorDesc->body != NULL )
				{
					SAFEDELETE( pCreateEvent->actorDesc->body );
				}
			}
			if( pCreateEvent->jointDesc != NULL )
			{
				NxJoint* joint = m_Scene->createJoint( *pCreateEvent->jointDesc );
				pCreateEvent->physObject->AddJoint( &CPhysicsJoint( joint ) );
			}
			if( pCreateEvent->clothDesc != NULL )
			{
				//NxCloth* cloth = m_Scene->createCloth( *pCreateEvent->clothDesc );
			}
		}

		SAFEDELETE( pCreateEvent->actorDesc );
		SAFEDELETE( pCreateEvent->jointDesc );
		SAFEDELETE( pCreateEvent->clothDesc );

		m_CreateEventQueue.pop();
	}
}

void CPhysicsManager::ExecuteLoadEventQueue()
{
	while ( !m_LoadEventQueue.empty() )
	{
		LOADPHYSICSEVENT* pLoadEvent = &m_LoadEventQueue.front();

		if( pLoadEvent->fileName != NULL )
		{
			// Try to grab the collection from the manager's cache
			NXU::NxuPhysicsCollection* collection = GetCachedPhysicsObject( pLoadEvent->fileName );

			if( collection != NULL )
			{
				Matrix4x4 tmp;
				pLoadEvent->physObject->GetTransform( tmp );

				NxMat34 pose = NxMat34Adapter( tmp );

				if( !pose.isFinite() )
				{
					m_ToolBox->Log( LOGERROR, _T("Invalid pose on %s!\n"), pLoadEvent->fileName );
					return;
				}

				CPhysicsObjectCreateCallback callback( pLoadEvent->physObject );
			
				NXU::instantiateCollection( collection, *m_PhysicsSDK, m_Scene, &pose, &callback );

				// Create a CCD skeleton
				if( pLoadEvent->hasCCD )
				{
					NXU::createCCDSkeletons( *collection );
				}
			}
			else
			{
				m_ToolBox->Log( LOGERROR, _T("CPhysicsObject::LoadActorFromFile() \"%s\" Collection is NULL.\n"), pLoadEvent->fileName );
			}
		}
	
		m_LoadEventQueue.pop();

		// reset material 0 and gravity.
		NxMaterial* defaultMaterial = m_Scene->getMaterialFromIndex(0);
		defaultMaterial->setRestitution( m_DefaultRestitution );
		defaultMaterial->setStaticFriction( m_DefaultStaticFriction );
		defaultMaterial->setDynamicFriction( m_DefaultDynamicFriction );

		SetGravity( m_Gravity );
	}
}

bool CPhysicsManager::ForceKinematic()
{
	return m_ForceKinematic;
}

// ------------------ Messages ------------------ //

DWORD CPhysicsManager::OnStart( DWORD size, void *params )
{
	m_IsRunning = true;

	return MSG_HANDLED_PROCEED;
}

DWORD CPhysicsManager::OnStop( DWORD size, void *params )
{
	m_IsRunning = false;

	return MSG_HANDLED_PROCEED;
}

DWORD CPhysicsManager::OnSetCollisionPair( DWORD size, void *params )
{
	// verify and cast the message
	VERIFY_MESSAGE_SIZE( size, sizeof(COLLISIONPAIR) );
	COLLISIONPAIR *message = (COLLISIONPAIR *)params;

	unsigned int groupA = message->groupA;
	unsigned int groupB = message->groupB;
	bool enable			= message->enable;

	m_Scene->setGroupCollisionFlag( groupA, groupB, enable );

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsManager::OnSetContactPair( DWORD size, void* params )
{
	// verify and cast the message
	VERIFY_MESSAGE_SIZE( size, sizeof(CONTACTPAIR) );
	CONTACTPAIR *message = (CONTACTPAIR *)params;

	unsigned int groupA = message->groupA;
	unsigned int groupB = message->groupB;
	unsigned int flags = message->flags;

	m_Scene->setActorGroupPairFlags( groupA, groupB, flags );

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsManager::OnRaycast( DWORD size, void *params )
{
	if( m_Scene == NULL )
	{
		return MSG_HANDLED_STOP;
	}
  
	// verify and cast the message
	VERIFY_MESSAGE_SIZE( size, sizeof(RAYCASTPARAMS) );
	RAYCASTPARAMS *message = (RAYCASTPARAMS *)params;

	Vec3 rayPos = *message->rayPos;
	Vec3 rayDir = *message->rayDir;

	if( rayDir.Length() == 0.0f )
	{
		return MSG_HANDLED_STOP;
	}

	NxVec3Adapter origin( rayPos );
	NxVec3Adapter direction( rayDir );
	direction.normalize();

	// Create the ray
	NxRay ray(origin, direction);
	// Create the raycast hit report
	NxRaycastHit hit;
	// Set the collsion group
	NxU32 groupFlag = CollisionGroupToMask( message->group );

	NxShape* closestShape = m_Scene->raycastClosestShape( ray, NX_ALL_SHAPES, hit, groupFlag );

	// if we hit something
	if(closestShape != NULL)
	{
   		// Get the CPhysicsObject from the user data
   		NxActor *actor = &closestShape->getActor();

		// The object hit by the raycast has been deleted but not yet removed from the simulation
		if( actor->userData == NULL )
		{
			return MSG_HANDLED_STOP;
		}

		PHYSICSUSERDATA* userData = (PHYSICSUSERDATA*)actor->userData;
		CPhysicsObject* physObj = userData->physObj;
		CPhysicsActor* physActor = userData->physActor;

   		RAYCOLLISION *collisionReport;
   		collisionReport = message->listRayCollisions;
   		collisionReport->distance = hit.distance;
   		collisionReport->hitPoint[0] = hit.worldImpact.x;
   		collisionReport->hitPoint[1] = hit.worldImpact.y;
   		collisionReport->hitPoint[2] = hit.worldImpact.z;
   		collisionReport->normal[0] = hit.worldNormal.x;
   		collisionReport->normal[1] = hit.worldNormal.y;
   		collisionReport->normal[2] = hit.worldNormal.z;
   		collisionReport->object = physObj->GetName();
		collisionReport->actor = physActor->GetName();
		collisionReport->objectType = physObj->GetComponentType();
   		collisionReport->parentobject = physObj->GetParentName();
	}

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsManager::OnRaycastArchive( DWORD size, void* params )
{
	if( m_Scene == NULL )
	{
		return MSG_HANDLED_STOP;
	}
  
	// verify and cast the message
	VERIFY_MESSAGE_SIZE( size, sizeof(IArchive*) );
	IArchive* archive = (IArchive*)params;

	Vec3 rayPos;
	Vec3 rayDir;

	archive->SetIsWriting(false);
	archive->Read( rayPos );
	archive->Read( rayDir );

	if( rayDir.Length() == 0.0f )
	{
		return MSG_HANDLED_STOP;
	}

	NxVec3Adapter origin( rayPos );
	NxVec3Adapter direction( rayDir );
	direction.normalize();

	int numGroups;
	int currentGroup;
	NxU32 groupFlag = 0x00000000;

	archive->Read( numGroups );
	for( int i = 0; i < numGroups; i++ )
	{
		archive->Read( currentGroup );
		groupFlag |= (1 << currentGroup);
	}

	groupFlag &= ~(1 << NOCOLLISION);

	// Create the ray
	NxRay ray(origin, direction);
	// Create the raycast hit report
	NxRaycastHit hit;

	NxShape* closestShape = m_Scene->raycastClosestShape( ray, NX_ALL_SHAPES, hit, groupFlag );

	archive->SetIsWriting( true );

	// if we hit something
	if(closestShape != NULL)
	{
		// we hit something so write true
		archive->Write( true );

   		// Get the CPhysicsObject from the user data
   		NxActor *actor = &closestShape->getActor();

		// The object hit by the raycast has been deleted but not yet removed from the simulation
		if( actor->userData == NULL )
		{
			return MSG_HANDLED_STOP;
		}

		PHYSICSUSERDATA* userData = (PHYSICSUSERDATA*)actor->userData;
		CPhysicsObject *physObj = userData->physObj;

		archive->Write( hit.distance );

		archive->Write( hit.worldImpact.x );
		archive->Write( hit.worldImpact.y );
		archive->Write( hit.worldImpact.z );

		archive->Write( hit.worldNormal.x );
		archive->Write( hit.worldNormal.y );
		archive->Write( hit.worldNormal.z );

		archive->Write( physObj->GetName()->GetString() );
		archive->Write( physObj->GetParentName()->GetString() );
		if( actor->getName() != NULL )
		{
			archive->Write( actor->getName() );
		}
	}
	else
	{
		// Nothing was hit so write false
		archive->Write( false );
	}

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsManager::OnRaycastList( DWORD size, void* params )
{
	if( m_Scene == NULL )
	{
		return MSG_HANDLED_STOP;
	}
  
	// verify and cast the message
	VERIFY_MESSAGE_SIZE( size, sizeof(RAYCASTLISTPARAMS) );
	RAYCASTLISTPARAMS* message = (RAYCASTLISTPARAMS*)params;

	Vec3 rayPos = *message->rayPos;
	Vec3 rayDir = *message->rayDir;

	if( rayDir.Length() == 0.0f )
	{
		return MSG_HANDLED_STOP;
	}

	NxVec3Adapter origin( rayPos );
	NxVec3Adapter direction( rayDir);
	direction.normalize();

	// Create the ray
	NxRay ray(origin, direction);
	// Create the raycast hit report
	CRaycastReport raycastReport( message );
	// Set the collsion group
	NxU32 groupFlag = CollisionGroupToMask( message->group );

	NxU32 numShapes = m_Scene->raycastAllShapes( ray, raycastReport, NX_ALL_SHAPES, groupFlag );

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsManager::OnIntersectAABB(DWORD size, void *params)
{
	// verify and cast the message
	VERIFY_MESSAGE_SIZE( size, sizeof(AABBINTERSECTPARAMS) );
	AABBINTERSECTPARAMS* message = (AABBINTERSECTPARAMS*)params;

	NxVec3Adapter min(*message->min);
	NxVec3Adapter max(*message->max);

	// Create the AABB collision volume
	NxBounds3 collisionAABB;
	collisionAABB.set(min, max);

	// Create the AABB intersection report
	CEntityReport collisionReport(message);

	// Find the number of shapes in the scene
	NxU32 nbShapes = m_Scene->getNbDynamicShapes() + m_Scene->getNbStaticShapes();

	// Preform the intersection test
	NxU32 groupFlag = CollisionGroupToMask( (COLLISIONGROUP)message->collisionGroup );

	m_Scene->overlapAABBShapes(collisionAABB, NX_ALL_SHAPES, nbShapes, 
    					NULL, &collisionReport, groupFlag, NULL);

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsManager::OnIntersectOOBB( DWORD size, void* params )
{
	return MSG_HANDLED_STOP;
}

DWORD CPhysicsManager::OnCachePhysicsObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ADDNOVODEXIMPORTER));
	ADDNOVODEXIMPORTER ani = *(ADDNOVODEXIMPORTER*) param;
	
	StdString lowerName(ani.szFileName.GetString());
	lowerName.MakeSafeFileName();
	CHashString hashName( lowerName );

	m_CachedPhysicsObjects[hashName.GetUniqueID()] = ani.nxCollection;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsManager::OnToggleRemoteVisualizer( DWORD size, void* param )
{
	if( m_RemoteDebuggerUserAllocator == NULL )
	{
		m_RemoteDebuggerUserAllocator = new UserAllocator();
		m_RemoteDebugger = m_PhysicsSDK->getFoundationSDK().getRemoteDebugger();
		m_RemoteDebugger->connect( "localhost" );
	}
	else
	{
		SAFE_DELETE( m_RemoteDebuggerUserAllocator );
	}

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsManager::OnShutDownPhysics( DWORD size, void* params )
{
	DeInit();
	return MSG_HANDLED_STOP;
}

DWORD CPhysicsManager::OnSetGravity( DWORD size, void* param )
{
	// Gravity should always be in meters/second
	VERIFY_MESSAGE_SIZE( size, sizeof(Vec3) );
	Vec3 gravity = *(Vec3*)param;

	SetGravity( gravity );

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsManager::OnSetPhysicsScale( DWORD size, void* param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(float) );
	float scale = *(float*)param;

	SetScale( scale );

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsManager::OnSetForceMoveActors( DWORD size, void* param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(bool) );
	m_ForceMoveActors = *(bool*)param;

	return MSG_HANDLED_STOP;
}

DWORD CPhysicsManager::OnForceKinematic( DWORD size, void* param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(bool) );
	m_ForceKinematic = *(bool*)param;

	static CHashString physicsObjectType( _T("CPhysicsObject") );
	IDTOOBJECTMAP::iterator iter;
	IDTOOBJECTMAP* physicsObjectMap = GetObjectMap( &physicsObjectType );

	if( physicsObjectMap != NULL )
	{
		CPhysicsObject* currentPhysicsObject;
		for( iter = physicsObjectMap->begin(); iter != physicsObjectMap->end(); iter++ )
		{
			currentPhysicsObject = (CPhysicsObject*)( iter->second );
			if( m_ForceKinematic )
			{
				currentPhysicsObject->ForceKinematic();
			}
			if( !m_ForceKinematic )
			{
				currentPhysicsObject->RestoreDynamic();
			}
		}
	}

	return MSG_HANDLED_STOP;
}