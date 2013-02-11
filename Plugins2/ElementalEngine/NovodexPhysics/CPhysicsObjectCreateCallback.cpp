///==========================================================================
/// \file	CPhysicsObjectCreateCallback.cpp
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

#include "StdAfx.h"

#include "CPhysicsObjectCreateCallback.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPhysicsObjectCreateCallback::CPhysicsObjectCreateCallback( CPhysicsObject* po )
{
	m_ToolBox = EngineGetToolBox();
	physicsObject = po;
}

bool CPhysicsObjectCreateCallback::NXU_preNotifyCloth(NxClothDesc& clothDesc, const char *userProperties)
{
	CPhysicsCloth* physCloth = &CPhysicsCloth(clothDesc);
	physicsObject->AddCloth( physCloth );

	// now, how to stop the cloth from loading any further...
	//return true;
	return false;
}

void CPhysicsObjectCreateCallback::NXU_notifyActor( NxActor *actor, const char *userProperties )
{
	CPhysicsActor* physActor = &CPhysicsActor(actor);
	physicsObject->AddActor( physActor );
}

void CPhysicsObjectCreateCallback::NXU_notifyJoint( NxJoint *joint, const char *userProperties )
{
	CPhysicsJoint* physJoint = &CPhysicsJoint(joint);
	physicsObject->AddJoint( physJoint );
}

void CPhysicsObjectCreateCallback::NXU_notifyCloth(NxCloth *cloth, const char *userProperties)
{
	m_ToolBox->Log( LOGWARNING, _T("CPhysicsObjectCreateCallback: Adding Cloth!\n") );
	//physicsObject->AddCloth( cloth );
}

void CPhysicsObjectCreateCallback::NXU_notifyActorFailed(NxActorDesc& actorDesc, const char *userProperties)
{
	m_ToolBox->Log( LOGERROR, _T("CPhysicsObjectCreateCallback: Failed to create actor.\n") );
}

void CPhysicsObjectCreateCallback::NXU_notifyJointFailed(NxJointDesc& jointDesc, const char *userProperties)
{
	m_ToolBox->Log( LOGERROR, _T("CPhysicsObjectCreateCallback: Failed to create joint.\n") );
}

void CPhysicsObjectCreateCallback::NXU_notifyClothFailed(NxClothDesc& jointDesc, const char *userProperties)
{
	m_ToolBox->Log( LOGERROR, _T("CPhysicsObjectCreateCallback: Failed to create cloth.\n") );
}