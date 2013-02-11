///==========================================================================
/// \file	CPhysicsJoint.h
/// \brief	Wrapper for an NxJoint
/// \date	5/08/2007
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPhysicsJoint::CPhysicsJoint()
{
	m_Joint = NULL;
	m_Name = NULL;
}

CPhysicsJoint::CPhysicsJoint( NxJoint* joint )
{
	m_Joint = joint;
	m_Name = new CHashString( joint->getName() );
}

CPhysicsJoint::~CPhysicsJoint()
{
	if( m_Name != NULL )
	{
		delete m_Name;
		m_Name = NULL;
	}
}

IHashString* CPhysicsJoint::GetParent()
{
	NxActor* parentActor;
	NxActor* childActor;
	m_Joint->getActors( &parentActor, &childActor );

	PHYSICSUSERDATA* userData = (PHYSICSUSERDATA*)parentActor->userData;
	return userData->physActor->GetName();
}

IHashString* CPhysicsJoint::GetChild()
{
	NxActor* parentActor;
	NxActor* childActor;
	m_Joint->getActors( &parentActor, &childActor );

	PHYSICSUSERDATA* userData = (PHYSICSUSERDATA*)childActor->userData;
	return userData->physActor->GetName();
}

IHashString* CPhysicsJoint::GetName()
{
	return m_Name;
}

void CPhysicsJoint::SetBreakable( float maxForce, float maxTorque )
{
	m_Joint->setBreakable( maxForce, maxTorque );
}

NxJoint* CPhysicsJoint::GetNxJoint()
{
	return this->m_Joint;
}