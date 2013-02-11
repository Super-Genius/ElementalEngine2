///==========================================================================
/// \file	CPhysicsManager.h
/// \brief	Physics implementation of the CManager interface
/// \date	2-18-2005
/// \date	3-20-2009
/// \author	Kyle Swaim
/// \author	Marivaldo Cabral
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

#ifndef CPHYSICSMANAGER_H
#define CPHYSICSMANAGER_H

// Includes from EE
#include "CManager.h"

#include "CContactReport.h"
#include "UserAllocator.h"
#include "CErrorStream.h"
#include "CTriggerReport.h"
#include "CPhysicsReport.h"

#include "CPhysicsObjectCreateCallback.h"

// Forward Declerations
class NxPhysicsSDK;
class NxScene;

typedef map<DWORD, NXU::NxuPhysicsCollection*> CACHEDPHYSICSOBJECTSMAP;
typedef map<DWORD, IObject*> PHYSICSOBJECTMAP;

///==========================================================================
/// \class	CPhysicsManager
/// \brief	Defines a Manager for IPhysObjects using the
///			3rd party Ageia PhysX libraries.
///==========================================================================
class CPhysicsManager : public IPhysicsManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CPhysicsManager );

protected:
	/// Default Constructor
	///	Since CPhysicsManager is implemented as a singleton the
	///	constructor is private.
	CPhysicsManager();

public:
	/// Default Destructor
	virtual ~CPhysicsManager();

	/// Function to get this component.
	/// \return	The singleton instance in this case
	static IComponent *Create( int nArgs, va_list argptr );

	/// Initializes the Physics Manager and Physics system
	/// \return	True on sucess, False on failure
	virtual bool Init();

	/// Updates the Physics Manager and Physics system
	virtual void Update(DWORD tickCount);
	
	/// DeInitializes the Physics Manager and Physics system
	/// \return	True on sucess, False on failure
	virtual bool DeInit();

	/// Set the gravity
	/// \brief	The new gravity vector
	void SetGravity( Vec3& gravity );

	/// Get the gravity
	/// \return The gravity vector
	Vec3* GetGravity();

	/// Set the physical world's scale
	/// \param	The scale in meters per PhysX units
	void SetScale( float scale );

	/// Get the physical world's scale
	/// \return The scale in meters per PhysX units
	float GetScale();

	/// Returns loaded importer from map, or creates and adds to map if not there
	/// \param filename = name of file
	/// \return importer
	NXU::NxuPhysicsCollection* GetCachedPhysicsObject( const TCHAR* filename ); // needs to be hidden

	/// Test if two contact groups are set as a contact pair
	bool IsContactPair( unsigned int groupA, unsigned int groupB );

	// ------------------ Messages ------------------ //

	/// Start the physics manager.
	DWORD OnStart( DWORD size, void* params );

	/// Stop the physics manager.
	DWORD OnStop( DWORD size, void* params );

	/// Set a contact pair to trigger contact reports.
	DWORD OnSetContactPair( DWORD size, void* params );

	/// Set a collision pair to trigger collision events.
	DWORD OnSetCollisionPair( DWORD size, void* params );

	/// Gets the first object that intersects the specified ray.
	DWORD OnRaycast( DWORD size, void* params );

	/// Gets all objects that intersect the specified ray.
	DWORD OnRaycastArchive( DWORD size, void* params );

	/// Gets all objects that intersect the specified ray.
	DWORD OnRaycastList( DWORD size, void* params );

	/// Gets all objects that intersect the specified AABB.
	DWORD OnIntersectAABB( DWORD size, void* params );

	/// Gets all objects that intersect the specified OOBB.
	DWORD OnIntersectOOBB( DWORD size, void* params );

	/// Cache a physics object for fast loading
	DWORD OnCachePhysicsObject( DWORD size, void* param );

	/// Toggle physics visualization.
	DWORD OnToggleRemoteVisualizer( DWORD size, void* param );

	/// Shut down the physics system.
	DWORD OnShutDownPhysics( DWORD size, void* params );

	/// Set gravity
	DWORD OnSetGravity( DWORD size, void* param );

	/// Set the unit scale
	DWORD OnSetPhysicsScale( DWORD size, void* param );

	/// Forces actors to move to their specified location.
	DWORD OnSetForceMoveActors( DWORD size, void* param );

	/// Force all actors kinematic.
	DWORD OnForceKinematic( DWORD size, void* param );

private:

	// declare friends with CPhysicsObjects this breaks encapsulation
	// between the physics manager and physics objects but preserves
	// the Ageia PhysX encapsulation with the rest of the engine.
	friend class CPhysicsObject;
	friend class CHeightfieldPhysicsObject;
	friend class CTriggerPhysicsObject;
	friend class CPickingPhysicsObject;
	friend class CCharacterPhysicsObject;
	friend class CEECharacterPhysicsObject;
	friend class CPhysicsCloth;
	friend class CBoundaryPhysicsObject;

	/// Clears all actors and resets the physics scene
	void ResetScene();

	/// Get the results of the last update from PhysX
	void GetPhysicsResults();
	/// Run a single timestep update of physics
	void RunPhysics();

	/// Creates and returns a point to a NxActor using PhysX
	/// \param	The NxActorDesc defining the actor's properties
	/// \return	The NxActor created from the actorDesc
	NxActor* CreateActor( NxActorDesc& actorDesc );
	/// Releases the actor
	void ReleaseActor( CPhysicsActor* actor );

	/// Creates and returns a point to a NxJoint using PhysX
	/// \param	The NxJointDesc defining the joint's properties
	/// \return	The NxJoint created from the jointDesc
	NxJoint* CreateJoint( NxJointDesc& jointDesc );
	/// Releases the joint
	void ReleaseJoint( CPhysicsJoint* joint );

	/// Creates and returns a point to a NxCloth using PhysX
	/// \param	The NxClothDesc defining the cloth's properties
	/// \return	The NxCloth created from the clothDesc
	NxCloth* CreateCloth( NxClothDesc& clothDesc );
	/// Releases the cloth
	void ReleaseCloth( CPhysicsCloth* cloth );

	/// Creates a heightfield from a NxHeightFieldDesc
	NxHeightField* CreateHeightField( NxHeightFieldDesc& heightFieldDesc );

	/// Helper function to register the lua bindings so that 
	///	lua can call Physics related messages
	void RegisterLuaDefinitions();

	/// Converts a collision group into a collision mask.
	///	used for filtering raycasts and other collisions.
	/// \param	group - The collision group to convert
	/// \return	The group converted into a collision mask
	unsigned int CollisionGroupToMask( COLLISIONGROUP group );

	/// Get a pointer to the NxScene.
	NxScene* GetScene();

	/// Ge ta pointer to the NxPhysicsSDK.
	NxPhysicsSDK* GetPhysicsSDK();

	/// Lua state for adding sctipt bindings
	lua_State*			m_LuaState;

	/// Novodex Physics SDK
	NxPhysicsSDK*		m_PhysicsSDK;
	/// The Scene representing the physics simulation
	NxScene*			m_Scene;
	/// Novodex error stream
	CErrorStream*		m_ErrorStream;

	/// Scale is meters per PhysX units
	float				m_Scale;
	/// Vector representing the force of gravity
	Vec3				m_Gravity;

	/// Contact callback
	CContactReport		m_ContactReport;
	/// Trigger volume callback
	CTriggerReport		m_TriggerReport;
	/// general callback
	CPhysicsReport		m_PhysicsReport;

	/// map of importers, mapped to filename 
	CACHEDPHYSICSOBJECTSMAP m_CachedPhysicsObjects;

	// remote debugger
	UserAllocator* m_RemoteDebuggerUserAllocator;
	NxRemoteDebugger* m_RemoteDebugger;

	bool m_IsRunning;
	bool m_FirstUpdate;

	// This is used to force a move or 'teleport' actors around.
	// It also forces actors to move even if the simulation is paused.
	bool m_ForceMoveActors;

	bool m_ForceKinematic;

	float m_DefaultRestitution;
	float m_DefaultDynamicFriction;
	float m_DefaultStaticFriction;

public:

	// Queues to buffer events that change the physics simulation state
	// when physics is running.
	typedef std::queue<DELETEPHYSICSEVENT> DELETEEVENTQUEUE;
	typedef std::queue<CREATEPHYSICSEVENT> CREATEEVENTQUEUE;
	typedef std::queue<LOADPHYSICSEVENT> LOADEVENTQUEUE;

	DELETEEVENTQUEUE m_DeleteEventQueue;
	CREATEEVENTQUEUE m_CreateEventQueue;
	LOADEVENTQUEUE m_LoadEventQueue;

	/// Queues up a delete event.
	void AddDeleteEvent( DELETEPHYSICSEVENT event );

	/// Queues up a create event.
	void AddCreateEvent( CREATEPHYSICSEVENT event );

	/// Queues up a load event.
	void AddLoadEvent( LOADPHYSICSEVENT event );

	/// Execute delete events.
	void ExecuteDeleteEventQueue();

	/// Execute create events.
	void ExecuteCreateEventQueue();

	/// Execute load events.
	void ExecuteLoadEventQueue();


	bool ForceKinematic();
};

#endif	// CPHYSICSMANAGER_H
