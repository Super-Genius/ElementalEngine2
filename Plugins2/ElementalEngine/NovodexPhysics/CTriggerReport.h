///============================================================================
/// \file	CTriggerReport.h
/// \brief	Callback report for trigger volumes
/// \date	12-07-2006
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

#ifndef CTRIGGERREPORT_H
#define CTRIGGERREPORT_H

#include <queue>

///============================================================================
/// \class	CTriggerReport
/// \brief	Callback report for trigger volumes
///============================================================================
class CTriggerReport : public NxUserTriggerReport
{
public:
	/// Constructor
	CTriggerReport();

	/// Destructor
	virtual ~CTriggerReport();

	/// Execute queued trigger events.
	/// This has to be called during the CPhysicsManager's update when the
	/// physics thread is not running.
	void ExecuteTriggerEventQueue();

	/// Callback when a CTriggerPhysicsObject is triggered.
	/// \param triggerShape = The trigger.
	/// \param otherShape = The shape that triggered the trigger.
	/// \param status = The type of triggering (enter, stay, leave)
	virtual void onTrigger( NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status );

private:
	IToolBox* m_ToolBox;

	/// Pointer to Physics Manager
	CPhysicsManager *m_pManager;

	struct TRIGGEREVENT
	{
		IArchive *pArchive;
		CHashString szTriggerObject;
		CHashString szTriggerEventName;
	};
	typedef std::queue<TRIGGEREVENT> TRIGGEREVENTQUEUE;
	TRIGGEREVENTQUEUE m_TriggerEventQueue;
};

#endif	// CTRIGGERREPORT_H