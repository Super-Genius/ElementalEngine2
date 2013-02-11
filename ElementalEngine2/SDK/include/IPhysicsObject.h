///==========================================================================
/// \file	IPhysicsObject.h
/// \brief	Interface basic rigid body physics objects
/// \date	2/22/2005
/// \date	2/25/2009
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


#ifndef _IPHYSICSOBJECT_H
#define _IPHYSICSOBJECT_H

namespace ElementalEngine
{

class IPhysicsActor;

///==========================================================================
/// \class	IPhysicsObject
/// \brief	Physics object interface
///==========================================================================
class IPhysicsObject : public CSceneObject<ISceneObject>
{
protected:
	IPhysicsObject(const TCHAR *theManager, const TCHAR *className, IHashString *parentName, IHashString *name) 
		: CSceneObject(theManager, className, parentName, name) {} 

public:
	///	Destructor
	virtual ~IPhysicsObject() {};

	/// Initialize the object
	virtual void Init() = 0;

	/// The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update() = 0;

	/// Deinitializes the CPhysicsObject and deallocates
	///	any allocated memory. Resets all internal data.
	virtual void DeInit() = 0;

	/// Serialize this component to or from an archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar ) = 0;

	/// Get the object's sort id.
	virtual OBJECTSORTTYPES GetSceneSortID() const = 0;

	/// Return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType() = 0;

	/// Checks if this is similar to it's base classes component
	/// \return true if it is the same type, else false
	virtual bool IsKindOf( IHashString *compType ) = 0;

	/// Adds an actor
	/// If the actor is the only actor then it is automaticly the root actor
	virtual void AddActor( IPhysicsActor* actor ) = 0;

	/// Adds an actor.
	/// Use this if there is only a single actor, eg terrain
	virtual void SetActor( IPhysicsActor* actor ) = 0;

	/// Remove an actor.
	virtual void RemoveActor( IHashString* actorName ) = 0;

	/// Get an actor by name
	/// \param actorName = The name of the actor to get.
	virtual IPhysicsActor* GetActor( IHashString* actorName ) = 0;

	/// Add a joint
	/// \param joint = Joint to add.
	virtual void AddJoint( IPhysicsJoint* joint ) = 0;

	/// Remove a joint
	/// \param jointName = Name of the joint to remove.
	virtual void RemoveJoint( IHashString* jointName ) = 0;

	/// Get a joint by name
	/// \param jointName = Name of the desired joint.
	/// \return A joint.
	virtual IPhysicsJoint* GetJoint( IHashString* jointName ) = 0;

	/// Add a cloth
	/// \param clothDesc = A cloth descriptor.
	virtual void AddCloth( IPhysicsCloth* clothDesc ) = 0;

	/// Remove a cloth
	/// \param clothName = Name of the cloth to remove.
	virtual void RemoveCloth( IHashString* clothName ) = 0;

	/// Get a cloth by name
	/// \param clothName = Name of the cloth to remove.
	/// \return The cloth.
	virtual IPhysicsCloth* GetCloth( IHashString* clothName ) = 0;

	/// Get the object's collision group.
	/// \return The CPhysicsObject's collision group.
	virtual COLLISIONGROUP GetCollisionGroup() = 0;

	/// Get the object's contact group.
	/// \return The CPhysicsObject's contatc group
	virtual unsigned int GetContactGroup() = 0;

	/// Tell the object that it's world space transform is dirty and needs to be updated.
	/// \param bFlag = Flag the CPhysicsObject's transform as in need of update.
	virtual inline void SetDirtyTransformFlag( bool bFlag ) = 0;

	/// Get the object's world space transform.
	/// \param transform = CPhysicsObject's world space transform matrix.
	virtual void GetTransform( Matrix4x4& transform ) = 0;

	/// Execute all cached events.
	virtual void ExecuteEventQueue() = 0;

	/// Get if the actor has a collision surrogate set.
	/// \return True if there is a collision surrogate, false if not.
	virtual bool HasCollisionSurrogate() = 0;

	/// Get the name of hte actor's collision surrogate.
	/// \return The name of the collision surrogate.
	virtual IHashString* GetCollisionSurrogate() = 0;

	// ------------------ Messages ------------------ //

	/// Apply a world space force to the CPhysicsObject.
	virtual DWORD OnApplyForce( DWORD size, void* params ) = 0;

	/// Apply a local space force to the CPhysicsObject.
	virtual DWORD OnApplyLocalForce( DWORD size, void* params ) = 0;

	/// Apply a world space torque to the CPhysicsObject.
	virtual DWORD OnApplyTorque( DWORD size, void* params ) = 0;

	/// Apply a local space torque to the CPhysicsObject.
	virtual DWORD OnApplyLocalTorque( DWORD size, void* params ) = 0;

	/// Set the CPhysicsActor's linear velocity.
	virtual DWORD OnSetLinearVelocity( DWORD size, void* params ) = 0;

	/// Set the CPhysicsActor's angular velocity.
	virtual DWORD OnSetAngularVelocity( DWORD size, void* params ) = 0;

	/// Get the CPhysicsActor's linear velocity.
	virtual DWORD OnGetLinearVelocity( DWORD size, void* params ) = 0;

	/// Get the CPhysicsActor's angular velocity.
	virtual DWORD OnGetAngularVelocity( DWORD size, void* params ) = 0;

	/// Set the CPhysicsActor's linear momentum.
	virtual DWORD OnSetLinearMomentum( DWORD size, void* params ) = 0;

	/// Set the CPhysicsActor's angular momentum.
	virtual DWORD OnSetAngularMomentum( DWORD size, void* params ) = 0;

	/// Get the CPhysicsActor's linear momentum.
	virtual DWORD OnGetLinearMomentum( DWORD size, void* params ) = 0;

	/// Get the CPhysicsActor's angular momentum.
	virtual DWORD OnGetAngularMomentum( DWORD size, void* params ) = 0;

	/// Set the CPhysicsObject's linear damping.
	virtual DWORD OnSetLinearDamping( DWORD size, void* params ) = 0;

	/// Set the CPhysicsObject's angular damping.
	virtual DWORD OnSetAngularDamping( DWORD size, void* params ) = 0;

	/// Disable CPhysicsObject's visualization.
	virtual DWORD OnEnableVisualization( DWORD size, void* params ) = 0;

	/// Disable CPhysicsObject's visualization.
	virtual DWORD OnDisableVisualization( DWORD size, void* params ) = 0;

	/// Rebuild the CPhysicsObject.
	virtual DWORD OnReinitPhysics( DWORD size, void* params ) = 0;

	/// Set the CPhysicsObject static.
	virtual DWORD OnSetKinematic( DWORD size, void* params ) = 0;

	/// Set the CPhysicsObject dynamic.
	virtual DWORD OnSetDynamic( DWORD size, void* params ) = 0;

	/// Lock the CPhysicsObject's translation on the specified axies.
	virtual DWORD OnLockTranslationAxis( DWORD size, void* params ) = 0;

	/// Lock the CPhysicsObject's rotation on the specified axies.
	virtual DWORD OnLockRotationAxis( DWORD size, void* params ) = 0;

	/// Enable physics driven hierarchal animations. (ragdoll)
	virtual DWORD OnEnablePhysicsAnimations( DWORD size, void* params ) = 0;

	/// Set physics hierarchy to match animation hierarchy.
	virtual DWORD OnSetHierarchalData( DWORD size, void* params ) = 0;

	/// Force the CPhysicsObject's world transform.
	virtual DWORD OnForceSetGlobalTransform( DWORD size, void* params ) = 0;

	/// Get one of the CPhysicsObject's actor's position.
	virtual DWORD OnGetActorPositionByName( DWORD size, void* params ) = 0;

	/// Set one of the CPhysicsObject's actor's velocity.
	virtual DWORD OnSetActorVelocityByName( DWORD size, void* params ) = 0;

	/// Get one of the CPhysicsObject's actor's velocity.
	virtual DWORD OnGetActorVelocityByName( DWORD size, void* params ) = 0;

	/// Set the CPhysicsObject's collision group
	virtual DWORD OnSetCollisionGroup( DWORD size, void* params ) = 0;

	/// Break all joints on this CPhysicsObject.
	virtual DWORD OnBreakAllJoints( DWORD size, void* params ) = 0;

	// Break a named joint.
	virtual DWORD OnBreakJoint( DWORD size, void* params ) = 0;

	/// Get the geometry to generate a physics cloth from.
	virtual DWORD OnGetClothPhysicsGeometry( DWORD size, void* params ) = 0;

	/// Enable gravity on this CPhysicsObject.
	virtual DWORD OnEnableGravity( DWORD size, void* params ) = 0;

	/// Disable gravity on this CPhysicsObject.
	virtual DWORD OnDisableGravity( DWORD size, void* params ) = 0;

	/// Set the CPhysicsObject to act as a trigger volume.
	virtual DWORD OnEnableTrigger( DWORD size, void* params ) = 0;

	/// Set a collision surrogate.
	virtual DWORD OnSetCollisionSurrogate( DWORD size, void* params ) = 0;

	/// \brief	Sets the collision group that the object belongs to
	virtual void SetCollisionGroup( COLLISIONGROUP group ) = 0;

	/// \brief	Sets the contact group to trigger contact callbacks
	virtual void SetContactGroup( unsigned int group ) = 0;

	/// Set a shape flag
	///	Since there isn't a good way to access individual shapes
	///	this currently sets the flag on ALL shapes in the actor
	virtual void SetShapeFlag( int flag, bool enable ) = 0;

	/// Load actors/joints from a .ssa, ssb or dae file
	virtual void LoadActorFromFile( const TCHAR* fileName, bool hasCCD ) = 0;

	// need to move this to a seperate class at some point
	virtual void BuildPlaneActor() = 0;

	virtual void AddPhysicsVisualization() = 0;

	virtual void RemovePhysicsVisualization() = 0;

	/// Release all actors, joints, cloth and fluid objects owned by the CPhysicsObject
	virtual void ReleaseModel() = 0;

	/// Release all CPhysicsActors.
	virtual void ReleaseActors() = 0;

	/// Release all CPhysicsJoints.
	virtual void ReleaseJoints() = 0;

	/// Release all CPhysicsCloths.
	virtual void ReleaseCloth() = 0;

	/// Get the parent's transform
	/// \param transform = the parent's transform.
	virtual void GetParentTransform( Matrix4x4& transform ) = 0;

	/// Set the actor's transform
	virtual void SetTransform() = 0;

	/// Set the actor's scale.
	/// \param scale = The actor's new scale.
	virtual void SetScale( Vec3& scale ) = 0;

	/// Rescale a vector by another vector.
	/// \param vec = Vector to scale.
	/// \paran newScale = Vector's new scale.
	virtual void RescaleVector( Vec3& vec, Vec3& newScale ) = 0;

	// Move a static actor by destroying it and recreating it
	// in the new location
	virtual void MoveStaticActor( Matrix4x4& newTransform ) = 0;

	/// Build joint linkages in a hierarchal actor.
	/// \param joint = The joint to link.
	virtual void BuildJointLinks( IPhysicsJoint* joint ) = 0;

	/// Find the root actor of a hierarchal actor.
	virtual IPhysicsActor* FindRootActor() = 0;

	/// Get a CPhysicsActor by its name.
	/// \param name = Name of the desired CPhysicsActor.
	virtual IPhysicsActor* GetActorByName( IHashString* name ) = 0;

	/// Update a CPhysicsActor
	/// \param actor = The actor to update.
	virtual void UpdateActor( IPhysicsActor* actor ) = 0;

	/// Update all actors.
	virtual void UpdateHierarchalActors() = 0;

	/// Update all CPhysicsCloth objects.
	virtual void UpdateCloth() = 0;

	/// Force the CPhysicsActor to be kinematic.
	virtual void ForceKinematic() = 0;

	/// Force the CPhysicsActor to be dynamic.
	virtual void RestoreDynamic() = 0;

	/// Handles EnableGravity event.
	virtual void EnableGravity() = 0;

	/// Handles DisableGravity event.
	virtual void DisableGravity() = 0;

	/// Handles SetKinematic event.
	virtual void SetKinematic() = 0;

	/// Handles SetDynamic event.
	virtual void SetDynamic() = 0;

	/// Handles SetCollisionGroup event.
	virtual void SetCollisionGroup() = 0;

	/// Handles SetTrigger event.
	virtual void SetTrigger() = 0;

	/// Handles ApplyForce event.
	virtual void ApplyForce() = 0;

	/// Handles ApplyLocalForce event.
	virtual void ApplyLocalForce() = 0;

	/// Handles ApplyTorque event.
	virtual void ApplyTorque() = 0;

	/// Handles ApplyLocalTorque event.
	virtual void ApplyLocalTorque() = 0;

	/// Handles SetLinearVelocity event.
	virtual void SetLinearVelocity() = 0;

	/// Handles SetAngularVelocity event.
	virtual void SetAngularVelocity() = 0;

	/// Handles SetLinearMomentum event.
	virtual void SetLinearMomentum() = 0;

	/// Handles SetAngularMomentum event.
	virtual void SetAngularMomentum() = 0;

	/// Handles SetLinearDamping event.
	virtual void SetLinearDamping() = 0;

	/// Handles SetAngularDamping event.
	virtual void SetAngularDamping() = 0;

	/// Handles LockTranslationAxis event.
	virtual void LockTranslationAxis() = 0;

	/// Handles LockRotationAxis event.
	virtual void LockRotationAxis() = 0;

	/// Handles EnablePhysicsAnimations event.
	virtual void EnablePhysicsAnimations() = 0;

	/// Handles SetHierarchalData event.
	virtual void SetHierarchalData() = 0;

	/// Handles SetActorVelocity event.
	virtual void SetActorVelocity() = 0;

	/// Handles BreakAllJoints event.
	virtual void BreakAllJoints() = 0;

	/// Handles BreakJoint event.
	virtual void BreakJoint() = 0;
};

}	//namespace ElementalEngine

#endif	// _IPHYSICSOBJECT_H
