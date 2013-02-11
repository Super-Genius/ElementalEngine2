///=====================================================================
/// \file	IJoystickHandler.h
/// \brief	Definition of IJoystickHandler Interface
///         This class defines a IJoystickHandler which is the interface
///			that all platform joysticks handlers inherit
/// \date	04/03/2008
/// \author	Richard Nguyen
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
///=====================================================================

#ifndef _IJOYSTICKHANDLER_H__
#define _IJOYSTICKHANDLER_H__

#include "BaseMessages.h"

namespace ElementalEngine
{
struct JOYSTICKKEYBIND
{
	DWORD MessageID;		// msg to send (IHashString's uniqueID)
	int iEventFilter;
	bool bRepeat;
	DWORD tickCount; 

	JOYSTICKKEYBIND( )
	{
		MessageID = 0;
		iEventFilter = KEYBIND_PRESSED;
		bRepeat = false;
		tickCount = 0;
	}
};

class IJoystickHandler : public IComponent
{
public:
	/// Initialization process
	/// \return true if succesful, false otherwise
	virtual bool Init(void) = 0;
	
	/// Update cycle
	/// \param tickCount = System tick
	virtual void Update(DWORD tickCount) = 0;
	
	/// Deinitialization process
	/// \return true if succesful, false otherwise
	virtual bool DeInit(void) = 0;
	
	/// Gets the fault time of the specified button unpressed
	/// \param iButton = Button identifier
	/// \return Fault time
	virtual float GetButtonUnpressedFaultTime( int iButton ) const = 0;
	
	/// Gets the fault time of the specified button pressed
	/// \param iButton = Button identifier
	/// \return Fault time
	virtual float GetButtonPressedFaultTime( int iButton ) const = 0;
	
	/// Gets the fault time of the specified axis unchanged
	/// \param iAxis = Axis identifier
	/// \return Fault time
	virtual float GetAxisNoChangeFaultTime( int iAxis ) const = 0;
	
	/// Gets the current handler time
	/// \return Current handler time
	virtual float GetTime() const = 0;
	
	/// Gets whether or not the auto-detection is enabled
	/// \return true if succesful, false otherwise
	virtual bool IsAutoDetect() = 0;
	
	/// Assigns a binding to a button
	/// \param kbNameID = Key binding ID
	/// \param deviceID = Joystick ID
	/// \param keyID = Button ID
	/// \param keybind = Binding struct
	virtual void SetKeybinding( DWORD kbNameID, DWORD deviceID, DWORD keyID, JOYSTICKKEYBIND *keybind ) = 0;
	
	/// Sets the active key binding
	/// \param nameID = Name hash ID
	virtual void SetActiveKeyBinding( DWORD nameID ) = 0;
	
	/// Swaps Joystick device mappings
	/// \param iOldDeviceID = Old device ID
	/// \param iNewDeviceID = New device ID
	virtual void SwapDeviceMapping( int iOldDeviceID, int iNewDeviceID ) = 0;
	
	/// Enables fault detection for specified joystick
	/// \param iJoystick = Joystick ID
	/// \param iInput = Button or axis identifier
	/// \param bEnable = Enable or disable
	virtual void EnableFaultDetection( int iJoystick, int iInput, bool bEnable ) = 0;
	
	/// Checks whether or not a fault exists with the specified Joystick
	/// \param iJoystick = Joystick ID
	/// \param iInput = Button or axis identifier
	/// \return true if succesful, false otherwise
	virtual bool HasFault( int iJoystick, int iInput ) = 0;
	
	/// Gets the joystick ID mapped to the specifed device
	/// \param iDeviceID = Device ID
	/// \return Corresponding joystick ID
	virtual int GetDeviceMapping( int iDeviceID ) = 0;
	
	/// Sets the joystick of the specifed device ID with a new device ID
	/// \param iOldDeviceID = Old device ID
	/// \param iNewDeviceID = New device ID
	/// \return true if succesful, false otherwise
	virtual bool SetDeviceMapping( int iOldDeviceID, int iNewDeviceID ) = 0;
	
	/// Gets the count of joysticks connected
	/// \return Count of joysticks
	virtual DWORD GetCount() = 0;
	
	/// Refreshes joystick settings
	virtual void Refresh() = 0;
	
	/// Sets whether or not to auto-detect joysticks
	/// \param bAutoDetect = Auto detect enabled or disabled
	virtual void SetAutoDetectPlayer( bool bAutoDetect ) = 0;

	/// Forwards a message to indicate the specified joystick has been connected to the system
	/// \param iJoystick = Connected joystick
	virtual void NotifyJoystickConnected( int iJoystick ) = 0;

	/// Forwards a message to indicate the specified joystick has been disconnected from the system
	/// \param iJoystick = Disconnected joystick
	virtual void NotifyJoystickDisconnected( int iJoystick ) = 0;

	/// Sets the speed value of the specified joystick and its motor
	/// \param iJoystick = Specified joystick
	/// \param cMotor = Specified motor
	/// \param fSpeed = Motor speed
	virtual void SetMotorSpeed( int iJoystick, char cMotor, float fSpeed ) = 0;
};
}
#endif