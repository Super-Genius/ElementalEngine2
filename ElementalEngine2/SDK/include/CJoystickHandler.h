///============================================================================
/// \file		CJoystickHandler.h
/// \brief		Definition file for Generic Joystick class
/// \date		04-01-2008
/// \author		Richard Nguyen
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

#ifndef CJOYSTICKHANDLER_H__
#define CJOYSTICKHANDLER_H__

#include "CJoystick.h"

#define KEYREPEAT_FREQUENCY     250 //0.05f

typedef std::map<int,JOYSTICKKEYBIND> KEYBINDMAP;
typedef std::map<int,KEYBINDMAP> DEVICEKEYBINDMAP;
typedef std::map<DWORD,DEVICEKEYBINDMAP> KEYBINDSETMAP;

class CJoystickHandler : public IJoystickHandler
{
public:
	CJoystickHandler();
	~CJoystickHandler();

	bool Init();
	void Update(DWORD tickcount);
	bool DeInit();

	inline float GetButtonUnpressedFaultTime( int iButton ) const { return 240.0f; }
	inline float GetButtonPressedFaultTime( int iButton ) const { return 240.0f; }
	inline float GetAxisNoChangeFaultTime( int iAxis ) const { return 60.0f; }
	inline float GetTime() const { return m_fTime; }
	inline bool IsAutoDetect() { return m_bAutoDetectJoystickMapping; }

	virtual void SetKeybinding( DWORD kbNameID, DWORD deviceID, DWORD keyID, JOYSTICKKEYBIND *keybind );
	virtual void SetActiveKeyBinding( DWORD nameID );
	virtual void SwapDeviceMapping( int iOldDeviceID, int iNewDeviceID );
	virtual void EnableFaultDetection( int iJoystick, int iInput, bool bEnable );
	virtual bool HasFault( int iJoystick, int iInput );
	virtual int GetDeviceMapping( int iDeviceID );
	virtual bool SetDeviceMapping( int iOldDeviceID, int iNewDeviceID );
	virtual DWORD GetCount();
	virtual void Refresh();
	virtual void SetAutoDetectPlayer( bool bAutoDetect );
	virtual void NotifyJoystickConnected( int iJoystick );
	virtual void NotifyJoystickDisconnected( int iJoystick );
	virtual void SetMotorSpeed( int iJoystick, char cMotor, float fSpeed );
	
protected:
	/// Runs a key binding if one has been set for the current key binds set
	void RunKeyBinding( int iDeviceID, int iKeyID, float fValue, int iEvent );
	/// Runs repeat key bindings if one has been set for the current key binds
	void RunRepeatKeyBinding( int iDeviceID, int iKeyID, float fValue );

	float m_fTime;
	bool m_bAutoDetectJoystickMapping;
	int m_iAxisResolution;
	std::vector<IJoystick*> m_Joysticks;

private:
	IJoystick* GetJoystick( int iJoystick );
	/// Reset axis calibration data for each joystick
	virtual void ResetAxisCalibration();

	IToolBox * m_ToolBox;
	ITimer * m_Timer;

	typedef std::map<int,JOYSTICKKEYBIND> KEYBINDMAP;
	typedef std::map<int,KEYBINDMAP> DEVICEKEYBINDMAP;
	typedef std::map<DWORD,DEVICEKEYBINDMAP> KEYBINDSETMAP;

	KEYBINDSETMAP m_KeyBindSets;
	CHashString m_ActiveKeyBindSet;
	int m_iJoystickCount;
};

#endif