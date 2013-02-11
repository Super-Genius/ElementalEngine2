///============================================================================
/// \file		CJoystick.h
/// \brief		Header file for Joystick class
/// \date		12-15-2006
/// \author		Brian Bazyk
/// \note   Elemental Engine
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

#ifndef CJOYSTICK_H
#define CJOYSTICK_H

// key repeat and deleay times (in seconds)
#define KEYREPEAT_DELAYTIME     500 //0.25f

#include "IJoystickHandler.h"

class CJoystickManager : public CManager
{
public:
	/// Create function
	static IComponent* Create( int nArgs, va_list argptr );

	/// Constructor
	CJoystickManager();
	/// Destructor
	~CJoystickManager();

	virtual bool Init(void);
	virtual void Update(DWORD tickCount);
	virtual bool DeInit(void);

	inline int GetJoystickCount() const { return 0; } // FIX


	// *** Messages ***
	DWORD OnStart(DWORD size, void *param);
	DWORD OnStop(DWORD size, void *param);
	DWORD OnSetJoystickHandler(DWORD size, void *param);
	DWORD OnUnsetJoystickHandler(DWORD size, void *param);
	DWORD OnSetJoystickKeyBinding(DWORD size, void *param);
	DWORD OnSetActiveJoystickKeyBinding(DWORD size, void *param);
	DWORD OnSwapJoystickDeviceMapping(DWORD size, void *param);
	DWORD OnEnableJoystickFaultDetection(DWORD size, void *param);
	DWORD OnQueryJoystickFault(DWORD size, void *param);
	DWORD OnSetJoystickDeviceMapping(DWORD size, void *param);
	DWORD OnGetJoystickDeviceMapping(DWORD size, void *param);
	DWORD OnGetJoystickCount(DWORD size, void *param);
	DWORD OnRefreshJoysticks(DWORD size, void *param);
	DWORD OnSetAutoDetectJoystickPlayer(DWORD size, void *param);
	DWORD OnSetMotorSpeed(DWORD size, void *param);

private:
	bool m_bIsRunning;

	IToolBox * m_ToolBox;
	ITimer * m_Timer;

	// Platform dependant
	IJoystickHandler * m_pJoystickHandler;
};

#endif