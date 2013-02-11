///============================================================================
/// \file	CPhysicsReport.h
/// \brief	Callback report general physics events
/// \date	6-18-2007
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
///============================================================================

#ifndef CPHYSICSREPORT_H
#define CPHYSICSREPORT_H

///============================================================================
/// \class	CPhysicsReport
/// \brief	Callback report general physics events
///============================================================================
class CPhysicsReport : public NxUserNotify
{
public:
	/// Constructor.
	CPhysicsReport();

	/// Destructor.
	virtual ~CPhysicsReport();

public:
	/// Callback when a joint is broken
	/// \param breakingImpulse = The impulse that broke the joint.
	/// \param brokenJoint = The broken joint
	virtual bool onJointBreak( NxReal breakingImpulse, NxJoint &brokenJoint );

	/// Callback when an actor is woken up.
	virtual void onWake( NxActor **actors, NxU32 count ) {};

	/// Callback when an actor goes to sleep.
	virtual void onSleep( NxActor **actors, NxU32 count ) {};

private:
	IToolBox* m_ToolBox;
};

#endif	// CPHYSICSREPORT_H