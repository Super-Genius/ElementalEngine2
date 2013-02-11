///==========================================================================
/// \file	CPhysicsActor.h
/// \brief	Wrapper for an NxActor
/// \date	5/07/2007
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

#ifndef	CPHYSICSACTOR_H
#define CPHYSICSACTOR_H

namespace ElementalEngine
{

class CPhysicsActor;
class CPhysicsObject;

typedef list<CPhysicsActor*> ACTORLIST;
typedef map<StdString*, CPhysicsActor*> ACTORMAP;

struct PHYSICSUSERDATA
{
	PHYSICSUSERDATA()
	{
		physObj = NULL;
		physActor = NULL;
	}

	CPhysicsObject* physObj;
	CPhysicsActor* physActor;
};

///==========================================================================
/// \class	CPhysicsActor
/// \brief	EE wrapper for NxActor. This class is internal to the
///			NovodexPhysics plugin and should not be accessed externally to
///			the plugin.
///==========================================================================
class CPhysicsActor : public IPhysicsActor
{
public:
	NxActor* m_Actor;
	CPhysicsActor* m_Parent;
	ACTORLIST m_Children;

	IToolBox* m_ToolBox;
	Vec3 m_CurrentScale;

private:
	/// Default Constructor
	CPhysicsActor();
public:
	/// Constructor
	/// \param actor = NxActor to wrap.
	CPhysicsActor( NxActor* actor );

	/// Destructor
	~CPhysicsActor();

	/// Get actor PhysX implementation
	NxActor* GetNxActor();

	/// Get the actor's name/
	/// \return The actor's name.
	IHashString* GetName();

	/// Set the actor's parent
	/// \param actor = Parent actor.
	void SetParent( IPhysicsActor* actor );

	/// Add a child actor.
	/// \param actor = Child actor.
	void AddChild( IPhysicsActor* actor );

	/// Remove a child actor.
	/// \param actor = Child actor.
	void RemoveChild( IPhysicsActor* actor );

	/// Set the actor's scale.
	/// \param scale = Scale.
	void SetScale( Vec3& scale );

	/// Enable visualization of the actor.
	void AddVisualization();

	/// Disable visualization of the actor.
	void RemoveVisualization();

	/// Get the actor's transform matrix.
	/// \param trans = Matrix to copy transform to.
	void GetTransform(Matrix4x4& trans);

	/// Set the actor's transform from a skeletal animation.
	/// \param boneInfo = The corrosponsing bone's information.
	void SetFromBoneData( BONEINFO* boneInfo );

	/// Sets the NxActor's userdata
	/// \param physObject = The CPhysicsObject the actor belongs to.
	/// \param physActor = The CPhysicsActor the NxActor corrosponds to.
	void SetUserData( IPhysicsObject* physObj, IPhysicsActor* physActor );

	/// Get the NxActor's userdata.
	void* GetUserData();

	/// Get if the actor is dynamic.
	/// \return True if the actor is dynamic.
	bool IsDynamic();

	/// Get if the actor is kinematic.
	/// \return True if the actor is kinematic.
	bool IsKinematic();

	/// Get if the actor is sleeping
	/// \return True if the actor is asleep.
	bool IsSleeping();

	/// Set the actor's collision group
	/// \param group = collision group.
	void SetCollisionGroup( COLLISIONGROUP group );

	/// Set the actor's contact group
	/// \param group = contact group.
	void SetContactGroup( unsigned int group );

	/// Get the actor's collision group
	/// \return collision group.
	int GetCollisionGroup();

	/// Set the actor kinematic.
	void SetKinematic();

	/// Set the actor dynamic.
	void SetDynamic();

	/// Force an actor to the kinematic state.
	/// Used when setting the manager's kinematic/dynamic states
	void ForceKinematic();

	/// Sets an actor back to dynamic if it was originally dynamic.
	void RestoreDynamic();

	/// Lock translation on the specified axies.
	/// \param axies = The axies to lock.
	void LockTranslationAxies( LOCKAXIES* axies );

	/// Lock rotation on the specified axies.
	/// \param axies = The axies to lock.
	void LockRotationAxies( LOCKAXIES* axies );

	/// Get the global transform of the actor.
	/// \return The NxActor's global pose.
	Matrix4x4 GetGlobalPose();

	/// Set the NxActor's global pose.
	/// Has the effect of 'teleporting' the actor.
	/// \param transform = new world space transform.
	void SetGlobalPose( Matrix4x4& transform );

	/// Move the actor's global pose.
	/// The actor will collide with anything in it's path.
	/// \param transform = new world space transform.
	void MoveGlobalPose( Matrix4x4& transform );

	/// Get the actor's world space position.
	/// \return Actor's world position.
	Vec3 GetGlobalPosition();
	
	/// Set the actor's world space position.
	/// \param pos = new world position.
	void SetGlobalPosition( Vec3& pos );

	/// Get the actor's world space orientation.
	/// \return Actor's world rotation.
	Matrix3x3 GetGlobalOrientation();

	/// Set the actor's world space orientation.
	/// \param rot = new world rotation.
	void SetGlobalOrientation( Matrix3x3& rot );

	/// Apply a world space force to the actor.
	/// \param force = force vector.
	void AddForce( Vec3& force );

	/// Apply an object space force to the actor.
	/// \param force = force vector.
	void AddLocalForce( Vec3& force );

	/// Apply a world space torque to the actor.
	/// \param torque = torque vector.
	void AddTorque( Vec3& torque );

	/// Apply an object space torque to the actor.
	/// \param torque = torque vector.
	void AddLocalTorque( Vec3& torque );

	/// Set the actor's linear velocity.
	/// \param velocity = velocity vector.
	void SetLinearVelocity( Vec3& velocity );

	/// Set the actor's angular velocity.
	/// \param velocity = velocity vector.
	void SetAngularVelocity( Vec3& velocity );

	/// Set the actor's linear momentum.
	/// \param momentum = momentum vector.
	void SetLinearMomentum( Vec3& momentum );

	/// Set the actor's angular momentum.
	/// \param momentum = momentum vector.
	void SetAngularMomentum( Vec3& momentum );

	/// Set the actor's linear damping
	/// \param damping = damping factor.
	void SetLinearDamping( float damping );

	/// Set the actor's angular damping
	/// \param damping = damping factor.
	void SetAngularDamping( float damping );

	/// Get the actor's linear velocity.
	/// \param velocity = actor's velocity vector.
	void GetLinearVelocity( Vec3& velocity );

	/// Get the actor's angular velocity
	/// \param velocity = actor's velocity vector.
	void GetAngularVelocity( Vec3& velocity );

	/// Get the actor's linear momentum
	/// \param momentum = actor's momentum vector.
	void GetLinearMomentum( Vec3& momentum );

	/// Get the actor's angular momentum.
	/// \param momentum = actor's momentum vector.
	void GetAngularMomentum( Vec3& momentum );

	/// Preform a linear sweep of the actor along a vector.
	/// \param displacement = Path of the sweep.
	/// \param hitResult = Result of the sweep.
	void LinearSweep( Vec3& displacement, void* hitResult );

	/// Set a shape flag on the NxActor
	/// \param flag = flag.
	/// \param enable = enable/disable
	void SetShapeFlag( int flag, bool enable );

	/// Set a body flag on the NxActor
	/// \param flag = flag.
	/// \param enable = enable/disable
	void SetBodyFlag( int flag, bool enable );

	/// Set the actor to be a trigger.
	/// \param triggerParams = trigger parameters.
	void SetTrigger( TRIGGERPARAMS& triggerParams );

	/// Scale a vector.
	/// \param vec = vector to scale.
	/// \param newScale = scale.
	void RescaleVector( Vec3& vec, Vec3& newScale );

	// This flag stores if the actor was forced kinematic by the manager
	// allowing it to be restored to its former dynamic status.
	bool m_WasDynamic;

	IHashString* m_Name;
	PHYSICSUSERDATA* m_UserData;
};

} // namespace ElementalEngine

#endif // CPHYSICSACTOR_H