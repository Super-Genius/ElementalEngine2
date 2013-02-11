///==========================================================================
/// \file	CPhysicsObjectCreateCallback.h
/// \brief	Callback that gets triggered on the instantiation of a
///			NxuPhysicsModel into the physics simulation
/// \date	10/25/2006
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

#ifndef	CPHYSICSOBJECTCREATECALLBACK_H
#define CPHYSICSOBJECTCREATECALLBACK_H

//#include "NxuPhysicsInstantiator.h"
#include "NXU_Helper.h"

namespace ElementalEngine
{

///==========================================================================
/// \class	CPhysicsObjectCreateCallback
/// \brief	Callback that gets triggered on the instantiation of a
///			NxuPhysicsModel into the physics simulation
///==========================================================================
class CPhysicsObjectCreateCallback : public NXU_userNotify 
{
public:
	CPhysicsObjectCreateCallback( CPhysicsObject* po );
	CPhysicsObject* physicsObject;
	IToolBox* m_ToolBox;

private:
	// Pre-Notify application of cloth creation.
	virtual bool NXU_preNotifyCloth(NxClothDesc& clothDesc, const char *userProperties);

	// Notify application of a actor creation.
	virtual void NXU_notifyActor(NxActor *actor, const char *userProperties);

	// Notify application of a joint creation.
	virtual void NXU_notifyJoint(NxJoint *joint, const char *userProperties);

	// Notify application of a cloth creation.
	virtual void NXU_notifyCloth(NxCloth *cloth, const char *userProperties);

	// Notify application of a failure to create an actor.
	virtual void NXU_notifyActorFailed(NxActorDesc& actorDesc, const char *userProperties);

	// Notify application of a failure to create a joint.
	virtual void NXU_notifyJointFailed(NxJointDesc& jointDesc, const char *userProperties);

	// Notify application of a failure to create a joint.
	virtual void NXU_notifyClothFailed(NxClothDesc& clothDesc, const char *userProperties);
};

} // namespace ElementalEngine

#endif // CPHYSICSOBJECTCREATECALLBACK_H