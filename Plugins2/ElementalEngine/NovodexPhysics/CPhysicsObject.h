///==========================================================================
/// \file	CPhysicsObject.h
/// \brief	Basic rigid body physics objects
/// \date	2/22/2005
/// \date	3/23/2009
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

#ifndef	CPHYSICSOBJECT_H
#define CPHYSICSOBJECT_H

// Forward delcerations
class CPhysicsManager;
struct DISABLEGRAVITYEVENT;
struct ENABLEGRAVITYEVENT;
struct SETDYNAMICEVENT;
struct SETKINEMATICEVENT;
struct SETCOLLISIONGROUPEVENT;
struct SETTRIGGEREVENT;
struct APPLYFORCEEVENT;
struct APPLYLOCALFORCEEVENT;
struct APPLYTORQUEEVENT;
struct APPLYLOCALTORQUEEVENT;
struct SETLINEARVELOCITYEVENT;
struct SETANGULARVELOCITYEVENT;
struct SETLINEARMOMENTUMEVENT;
struct SETANGULARMOMENTUMEVENT;
struct SETLINEARDAMPINGEVENT;
struct SETANGULARDAMPINGEVENT;
struct LOCKTRANSLATIONAXISEVENT;
struct LOCKROTATIONAXISEVENT;
struct ENABLEPHYSICSANIMATIONSEVENT;
struct SETHIERARCHALDATAEVENT;
struct SETACTORVELOCITYBYNAMEEVENT;
struct BREAKALLJOINTSEVENT;
struct BREAKJOINTEVENT;

namespace ElementalEngine
{

///==========================================================================
/// \class	CPhysicsObject
/// \brief	Physics Implementation of IObject
///==========================================================================
class CPhysicsObject : public IPhysicsObject
{
protected:
	/// Constructor
	/// \param	parentName = name of parent in hierarchy
	/// \param	name = IHashString * name of this object
	/// \param	addToHier = Add this object to the hierarchy?
	CPhysicsObject( IHashString *parentName, IHashString *name, bool addToHier );

public:
	///	Destructor
	virtual ~CPhysicsObject();

	/// Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create( int nArgs, va_list argptr );

	/// Initialize the object
	virtual void Init();

	/// The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();

	/// Deinitializes the CPhysicsObject and deallocates
	///	any allocated memory. Resets all internal data.
	virtual void DeInit();

	/// Serialize this component to or from an archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// Get the object's sort id.
	virtual OBJECTSORTTYPES GetSceneSortID() const { return RENDEROBJECT3D; }

	/// Return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is the same type, else false
	virtual bool IsKindOf( IHashString *compType );

	/// Adds an actor
	/// If the actor is the only actor then it is automaticly the root actor
	void AddActor( IPhysicsActor* actor );

	/// Adds an actor.
	/// Use this if there is only a single actor, eg terrain
	void SetActor( IPhysicsActor* actor );

	/// Remove an actor.
	void RemoveActor( IHashString* actorName );

	/// Get an actor by name
	/// \param actorName = The name of the actor to get.
	CPhysicsActor* GetActor( IHashString* actorName );

	/// Add a joint
	/// \param joint = Joint to add.
	void AddJoint( IPhysicsJoint* joint );

	/// Remove a joint
	/// \param jointName = Name of the joint to remove.
	void RemoveJoint( IHashString* jointName );

	/// Get a joint by name
	/// \param jointName = Name of the desired joint.
	/// \return A joint.
	IPhysicsJoint* GetJoint( IHashString* jointName );

	/// Add a cloth
	/// \param clothDesc = A cloth descriptor.
	void AddCloth( IPhysicsCloth* clothDesc );

	/// Remove a cloth
	/// \param clothName = Name of the cloth to remove.
	void RemoveCloth( IHashString* clothName );

	/// Get a cloth by name
	/// \param clothName = Name of the cloth to remove.
	/// \return The cloth.
	IPhysicsCloth* GetCloth( IHashString* clothName );

	/// Get the object's collision group.
	/// \return The CPhysicsObject's collision group.
	COLLISIONGROUP GetCollisionGroup();

	/// Get the object's contact group.
	/// \return The CPhysicsObject's contatc group
	unsigned int GetContactGroup();

	/// Tell the object that it's world space transform is dirty and needs to be updated.
	/// \param bFlag = Flag the CPhysicsObject's transform as in need of update.
	inline void SetDirtyTransformFlag( bool bFlag ) { m_bTransformIsDirty = bFlag; }

	/// Get the object's world space transform.
	/// \param transform = CPhysicsObject's world space transform matrix.
	void GetTransform( Matrix4x4& transform );

	/// Execute all cached events.
	virtual void ExecuteEventQueue();

	/// Get if the actor has a collision surrogate set.
	/// \return True if there is a collision surrogate, false if not.
	bool HasCollisionSurrogate();

	/// Get the name of hte actor's collision surrogate.
	/// \return The name of the collision surrogate.
	IHashString* GetCollisionSurrogate();

	// ------------------ Messages ------------------ //

	/// Apply a world space force to the CPhysicsObject.
	DWORD OnApplyForce( DWORD size, void* params );

	/// Apply a local space force to the CPhysicsObject.
	DWORD OnApplyLocalForce( DWORD size, void* params );

	/// Apply a world space torque to the CPhysicsObject.
	DWORD OnApplyTorque( DWORD size, void* params );

	/// Apply a local space torque to the CPhysicsObject.
	DWORD OnApplyLocalTorque( DWORD size, void* params );

	/// Set the CPhysicsActor's linear velocity.
	virtual DWORD OnSetLinearVelocity( DWORD size, void* params );

	/// Set the CPhysicsActor's angular velocity.
	DWORD OnSetAngularVelocity( DWORD size, void* params );

	/// Get the CPhysicsActor's linear velocity.
	DWORD OnGetLinearVelocity( DWORD size, void* params );

	/// Get the CPhysicsActor's angular velocity.
	DWORD OnGetAngularVelocity( DWORD size, void* params );

	/// Set the CPhysicsActor's linear momentum.
	DWORD OnSetLinearMomentum( DWORD size, void* params );

	/// Set the CPhysicsActor's angular momentum.
	DWORD OnSetAngularMomentum( DWORD size, void* params );

	/// Get the CPhysicsActor's linear momentum.
	DWORD OnGetLinearMomentum( DWORD size, void* params );

	/// Get the CPhysicsActor's angular momentum.
	DWORD OnGetAngularMomentum( DWORD size, void* params );

	/// Set the CPhysicsObject's linear damping.
	DWORD OnSetLinearDamping( DWORD size, void* params );

	/// Set the CPhysicsObject's angular damping.
	DWORD OnSetAngularDamping( DWORD size, void* params );

	/// Disable CPhysicsObject's visualization.
	DWORD OnEnableVisualization( DWORD size, void* params );

	/// Disable CPhysicsObject's visualization.
	DWORD OnDisableVisualization( DWORD size, void* params );

	/// Rebuild the CPhysicsObject.
	DWORD OnReinitPhysics( DWORD size, void* params );

	/// Set the CPhysicsObject static.
	DWORD OnSetKinematic( DWORD size, void* params );

	/// Set the CPhysicsObject dynamic.
	DWORD OnSetDynamic( DWORD size, void* params );

	/// Lock the CPhysicsObject's translation on the specified axies.
	DWORD OnLockTranslationAxis( DWORD size, void* params );

	/// Lock the CPhysicsObject's rotation on the specified axies.
	DWORD OnLockRotationAxis( DWORD size, void* params );

	/// Enable physics driven hierarchal animations. (ragdoll)
	DWORD OnEnablePhysicsAnimations( DWORD size, void* params );

	/// Set physics hierarchy to match animation hierarchy.
	DWORD OnSetHierarchalData( DWORD size, void* params );

	/// Force the CPhysicsObject's world transform.
	DWORD OnForceSetGlobalTransform( DWORD size, void* params );

	/// Get one of the CPhysicsObject's actor's position.
	DWORD OnGetActorPositionByName( DWORD size, void* params );

	/// Set one of the CPhysicsObject's actor's velocity.
	DWORD OnSetActorVelocityByName( DWORD size, void* params );

	/// Get one of the CPhysicsObject's actor's velocity.
	DWORD OnGetActorVelocityByName( DWORD size, void* params );

	/// Set the CPhysicsObject's collision group
	DWORD OnSetCollisionGroup( DWORD size, void* params );

	/// Break all joints on this CPhysicsObject.
	DWORD OnBreakAllJoints( DWORD size, void* params );

	// Break a named joint.
	DWORD OnBreakJoint( DWORD size, void* params );

	/// Get the geometry to generate a physics cloth from.
	DWORD OnGetClothPhysicsGeometry( DWORD size, void* params );

	/// Enable gravity on this CPhysicsObject.
	DWORD OnEnableGravity( DWORD size, void* params );

	/// Disable gravity on this CPhysicsObject.
	DWORD OnDisableGravity( DWORD size, void* params );

	/// Set the CPhysicsObject to act as a trigger volume.
	DWORD OnEnableTrigger( DWORD size, void* params );

	/// Set a collision surrogate.
	DWORD OnSetCollisionSurrogate( DWORD size, void* params );

	/// \brief	Sets the collision group that the object belongs to
	void SetCollisionGroup( COLLISIONGROUP group );

	/// \brief	Sets the contact group to trigger contact callbacks
	void SetContactGroup( unsigned int group );

	/// Set a shape flag
	///	Since there isn't a good way to access individual shapes
	///	this currently sets the flag on ALL shapes in the actor
	void SetShapeFlag( int flag, bool enable );

	/// Load actors/joints from a .ssa, ssb or dae file
	void LoadActorFromFile( const TCHAR* fileName, bool hasCCD );

	// need to move this to a seperate class at some point
	void BuildPlaneActor();

	virtual void AddPhysicsVisualization();

	void RemovePhysicsVisualization();

	/// Release all actors, joints, cloth and fluid objects owned by the CPhysicsObject
	void ReleaseModel();

	/// Release all CPhysicsActors.
	void ReleaseActors();

	/// Release all CPhysicsJoints.
	void ReleaseJoints();

	/// Release all CPhysicsCloths.
	void ReleaseCloth();

	/// Get the parent's transform
	/// \param transform = the parent's transform.
	virtual void GetParentTransform( Matrix4x4& transform );

	/// Set the actor's transform
	virtual void SetTransform();

	/// Set the actor's scale.
	/// \param scale = The actor's new scale.
	void SetScale( Vec3& scale );

	/// Rescale a vector by another vector.
	/// \param vec = Vector to scale.
	/// \paran newScale = Vector's new scale.
	void RescaleVector( Vec3& vec, Vec3& newScale );

	// Move a static actor by destroying it and recreating it
	// in the new location
	void MoveStaticActor( Matrix4x4& newTransform );

	/// Build joint linkages in a hierarchal actor.
	/// \param joint = The joint to link.
	void BuildJointLinks( IPhysicsJoint* joint );

	/// Find the root actor of a hierarchal actor.
	IPhysicsActor* FindRootActor();

	/// Get a CPhysicsActor by its name.
	/// \param name = Name of the desired CPhysicsActor.
	IPhysicsActor* GetActorByName( IHashString* name );

	/// Update a CPhysicsActor
	/// \param actor = The actor to update.
	void UpdateActor( IPhysicsActor* actor );

	/// Update all actors.
	void UpdateHierarchalActors();

	/// Update all CPhysicsCloth objects.
	void UpdateCloth();

	/// Force the CPhysicsActor to be kinematic.
	void ForceKinematic();

	/// Force the CPhysicsActor to be dynamic.
	void RestoreDynamic();

	/// Handles EnableGravity event.
	virtual void EnableGravity();

	/// Handles DisableGravity event.
	virtual void DisableGravity();

	/// Handles SetKinematic event.
	virtual void SetKinematic();

	/// Handles SetDynamic event.
	virtual void SetDynamic();

	/// Handles SetCollisionGroup event.
	virtual void SetCollisionGroup();

	/// Handles SetTrigger event.
	virtual void SetTrigger();

	/// Handles ApplyForce event.
	virtual void ApplyForce();

	/// Handles ApplyLocalForce event.
	virtual void ApplyLocalForce();

	/// Handles ApplyTorque event.
	virtual void ApplyTorque();

	/// Handles ApplyLocalTorque event.
	virtual void ApplyLocalTorque();

	/// Handles SetLinearVelocity event.
	virtual void SetLinearVelocity();

	/// Handles SetAngularVelocity event.
	virtual void SetAngularVelocity();

	/// Handles SetLinearMomentum event.
	virtual void SetLinearMomentum();

	/// Handles SetAngularMomentum event.
	virtual void SetAngularMomentum();

	/// Handles SetLinearDamping event.
	virtual void SetLinearDamping();

	/// Handles SetAngularDamping event.
	virtual void SetAngularDamping();

	/// Handles LockTranslationAxis event.
	virtual void LockTranslationAxis();

	/// Handles LockRotationAxis event.
	virtual void LockRotationAxis();

	/// Handles EnablePhysicsAnimations event.
	virtual void EnablePhysicsAnimations();

	/// Handles SetHierarchalData event.
	virtual void SetHierarchalData();

	/// Handles SetActorVelocity event.
	virtual void SetActorVelocity();

	/// Handles BreakAllJoints event.
	virtual void BreakAllJoints();

	/// Handles BreakJoint event.
	virtual void BreakJoint();

protected:

	/// Pointer to Physics Manager
	CPhysicsManager *m_pManager;

	/// the file containing the physics data
	StdString m_File;
	/// the type (actor, control, terrain)
	CHashString m_Type;
	/// the collision group
	///	used to set what object collide with other objets
	COLLISIONGROUP m_CollisionGroup;
	/// the contact group
	///	used to set collision pairs for callbacks
	unsigned int m_ContactGroup;

	bool m_bAddToHierarchy;

	/// When PhysX is updating the position/rotation internally, make sure we don't reupdate the position
	bool m_bPositionUpdateLock;

	Vec3 m_CurrentScale;

	CHashString m_hsParentType;

	/// Root actor
	CPhysicsActor* m_PhysicsActor;
	// List of all actors
	ACTORLIST m_PhysicsActors; 
	// List of all joints
	JOINTLIST m_PhysicsJoints; 
	// List of all cloth
	CLOTHLIST m_PhysicsCloth; 

	bool m_IsPhysicsControlled;

	/// Force move flag. If this flag is set then the CPhysicsObject will
	/// 'teleport' to the next specified position.
	bool m_ForceMove;

	/// If the surrogate is not null then collision reports are redirected
	/// to the surrogate instead of the object.
	IHashString* m_CollisionSurrogate;

	// Event Queues
	ENABLEGRAVITYEVENT* m_EnableGravityEvent;
	DISABLEGRAVITYEVENT* m_DisableGravityEvent;
	SETKINEMATICEVENT* m_SetKinematicEvent;
	SETDYNAMICEVENT* m_SetDynamicEvent;
	SETCOLLISIONGROUPEVENT* m_SetCollisionGroupEvent;
	SETTRIGGEREVENT* m_SetTriggerEvent;
	APPLYFORCEEVENT* m_ApplyForceEvent;
	APPLYLOCALFORCEEVENT* m_ApplyLocalForceEvent;
	APPLYTORQUEEVENT* m_ApplyTorqueEvent;
	APPLYLOCALTORQUEEVENT* m_ApplyLocalTorqueEvent;
	SETLINEARVELOCITYEVENT* m_SetLinearVelocityEvent;
	SETANGULARVELOCITYEVENT* m_SetAngularVelocityEvent;
	SETLINEARMOMENTUMEVENT* m_SetLinearMomentumEvent;
	SETANGULARMOMENTUMEVENT* m_SetAngularMomentumEvent;
	SETLINEARDAMPINGEVENT* m_SetLinearDampingEvent;
	SETANGULARDAMPINGEVENT* m_SetAngularDampingEvent;
	LOCKTRANSLATIONAXISEVENT* m_LockTranslationAxisEvent;
	LOCKROTATIONAXISEVENT* m_LockRotationAxisEvent;
	ENABLEPHYSICSANIMATIONSEVENT* m_EnablePhysicsAnimationsEvent;
	SETHIERARCHALDATAEVENT* m_SetHierarchalDataEvent;
	SETACTORVELOCITYBYNAMEEVENT* m_SetActorVelocityByNameEvent;
	BREAKALLJOINTSEVENT* m_BreakAllJointsEvent;
	BREAKJOINTEVENT* m_BreakJointEvent;
};

}	//namespace ElementalEngine

#endif	// CPHYSICSOBJECT_H
