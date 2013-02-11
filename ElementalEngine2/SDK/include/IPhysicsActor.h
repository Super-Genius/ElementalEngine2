///==========================================================================
/// \file	IPhysicsActor.h
/// \brief	Interface for an actor
/// \date	02/27/2009
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

#ifndef	_IPHYSICSACTOR_H
#define _IPHYSICSACTOR_H

namespace ElementalEngine
{

class IPhysicsObject;
class IPhysicsActor;


///==========================================================================
/// \class	IPhysicsActor
/// \brief	This class is used to represent an actor in the physics world 
///==========================================================================
class IPhysicsActor
{
public:

	/// Destructor
	virtual ~IPhysicsActor() {};

	/// Get the actor's name/
	/// \return The actor's name.
	virtual IHashString* GetName() = 0;

	/// Set the actor's parent
	/// \param actor = Parent actor.
	virtual void SetParent( IPhysicsActor* actor ) = 0;

	/// Add a child actor.
	/// \param actor = Child actor.
	virtual void AddChild( IPhysicsActor* actor ) = 0;

	/// Remove a child actor.
	/// \param actor = Child actor.
	virtual void RemoveChild( IPhysicsActor* actor ) = 0;

	/// Set the actor's scale.
	/// \param scale = Scale.
	virtual void SetScale( Vec3& scale ) = 0;

	/// Enable visualization of the actor.
	virtual void AddVisualization() = 0;

	/// Disable visualization of the actor.
	virtual void RemoveVisualization() = 0;

	/// Get the actor's transform matrix.
	/// \param trans = Matrix to copy transform to.
	virtual void GetTransform(Matrix4x4& trans) = 0;

	/// Set the actor's transform from a skeletal animation.
	/// \param boneInfo = The corrosponsing bone's information.
	virtual void SetFromBoneData( BONEINFO* boneInfo ) = 0;

	/// Sets the NxActor's userdata
	/// \param physObject = The CPhysicsObject the actor belongs to.
	/// \param physActor = The CPhysicsActor the NxActor corrosponds to.
	virtual void SetUserData( IPhysicsObject* physObj, IPhysicsActor* physActor ) = 0;

	/// Get the NxActor's userdata.
	virtual void* GetUserData() = 0;

	/// Get if the actor is dynamic.
	/// \return True if the actor is dynamic.
	virtual bool IsDynamic() = 0;

	/// Get if the actor is kinematic.
	/// \return True if the actor is kinematic.
	virtual bool IsKinematic() = 0;

	/// Get if the actor is sleeping
	/// \return True if the actor is asleep.
	virtual bool IsSleeping() = 0;

	/// Set the actor's collision group
	/// \param group = collision group.
	virtual void SetCollisionGroup( COLLISIONGROUP group ) = 0;

	/// Set the actor's contact group
	/// \param group = contact group.
	virtual void SetContactGroup( unsigned int group ) = 0;

	/// Get the actor's collision group
	/// \return collision group.
	virtual int GetCollisionGroup() = 0;

	/// Set the actor kinematic.
	virtual void SetKinematic() = 0;

	/// Set the actor dynamic.
	virtual void SetDynamic() = 0;

	/// Force an actor to the kinematic state.
	/// Used when setting the manager's kinematic/dynamic states
	virtual void ForceKinematic() = 0;

	/// Sets an actor back to dynamic if it was originally dynamic.
	virtual void RestoreDynamic() = 0;

	/// Lock translation on the specified axies.
	/// \param axies = The axies to lock.
	virtual void LockTranslationAxies( LOCKAXIES* axies ) = 0;

	/// Lock rotation on the specified axies.
	/// \param axies = The axies to lock.
	virtual void LockRotationAxies( LOCKAXIES* axies ) = 0;

	/// Get the global transform of the actor.
	/// \return The NxActor's global pose.
	virtual Matrix4x4 GetGlobalPose() = 0;

	/// Set the NxActor's global pose.
	/// Has the effect of 'teleporting' the actor.
	/// \param transform = new world space transform.
	virtual void SetGlobalPose( Matrix4x4& transform ) = 0;

	/// Move the actor's global pose.
	/// The actor will collide with anything in it's path.
	/// \param transform = new world space transform.
	virtual void MoveGlobalPose( Matrix4x4& transform ) = 0;

	/// Get the actor's world space position.
	/// \return Actor's world position.
	virtual Vec3 GetGlobalPosition() = 0;
	
	/// Set the actor's world space position.
	/// \param pos = new world position.
	virtual void SetGlobalPosition( Vec3& pos ) = 0;

	/// Get the actor's world space orientation.
	/// \return Actor's world rotation.
	virtual Matrix3x3 GetGlobalOrientation() = 0;

	/// Set the actor's world space orientation.
	/// \param rot = new world rotation.
	virtual void SetGlobalOrientation( Matrix3x3& rot ) = 0;

	/// Apply a world space force to the actor.
	/// \param force = force vector.
	virtual void AddForce( Vec3& force ) = 0;

	/// Apply an object space force to the actor.
	/// \param force = force vector.
	virtual void AddLocalForce( Vec3& force ) = 0;

	/// Apply a world space torque to the actor.
	/// \param torque = torque vector.
	virtual void AddTorque( Vec3& torque ) = 0;

	/// Apply an object space torque to the actor.
	/// \param torque = torque vector.
	virtual void AddLocalTorque( Vec3& torque ) = 0;

	/// Set the actor's linear velocity.
	/// \param velocity = velocity vector.
	virtual void SetLinearVelocity( Vec3& velocity ) = 0;

	/// Set the actor's angular velocity.
	/// \param velocity = velocity vector.
	virtual void SetAngularVelocity( Vec3& velocity ) = 0;

	/// Set the actor's linear momentum.
	/// \param momentum = momentum vector.
	virtual void SetLinearMomentum( Vec3& momentum ) = 0;

	/// Set the actor's angular momentum.
	/// \param momentum = momentum vector.
	virtual void SetAngularMomentum( Vec3& momentum ) = 0;

	/// Set the actor's linear damping
	/// \param damping = damping factor.
	virtual void SetLinearDamping( float damping ) = 0;

	/// Set the actor's angular damping
	/// \param damping = damping factor.
	virtual void SetAngularDamping( float damping ) = 0;

	/// Get the actor's linear velocity.
	/// \param velocity = actor's velocity vector.
	virtual void GetLinearVelocity( Vec3& velocity ) = 0;

	/// Get the actor's angular velocity
	/// \param velocity = actor's velocity vector.
	virtual void GetAngularVelocity( Vec3& velocity ) = 0;

	/// Get the actor's linear momentum
	/// \param momentum = actor's momentum vector.
	virtual void GetLinearMomentum( Vec3& momentum ) = 0;

	/// Get the actor's angular momentum.
	/// \param momentum = actor's momentum vector.
	virtual void GetAngularMomentum( Vec3& momentum ) = 0;

	/// Preform a linear sweep of the actor along a vector.
	/// \param displacement = Path of the sweep.
	/// \param hitResult = Result of the sweep.
	virtual void LinearSweep( Vec3& displacement, void* hitResult ) = 0;

	/// Set a shape flag on the NxActor
	/// \param flag = flag.
	/// \param enable = enable/disable
	virtual void SetShapeFlag( int flag, bool enable ) = 0;

	/// Set a body flag on the NxActor
	/// \param flag = flag.
	/// \param enable = enable/disable
	virtual void SetBodyFlag( int flag, bool enable ) = 0;

	/// Set the actor to be a trigger.
	/// \param triggerParams = trigger parameters.
	virtual void SetTrigger( TRIGGERPARAMS& triggerParams ) = 0;

	/// Scale a vector.
	/// \param vec = vector to scale.
	/// \param newScale = scale.
	virtual void RescaleVector( Vec3& vec, Vec3& newScale ) = 0;
};

} // namespace ElementalEngine

#endif // _IPHYSICSACTOR_H