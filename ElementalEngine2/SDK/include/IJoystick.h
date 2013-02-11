///=====================================================================
/// \file	IJoystick.h
/// \brief	Definition of IJoystick Interface
///         This class defines a IJoystick which is the interface
///			that all platform joysticks inherit
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

#ifndef	_IJOYSTICK__H__
#define _IJOYSTICK__H__

namespace ElementalEngine
{

	struct BUTTONSTATE
{
	BUTTONSTATE()
	{
		bPressed = false;
		bPrevPressed = false;
		fTimeLastPressed = 0;
		fTimeLastReleased = 0;
		bPressedFault = false;
		bUnpressedFault = false;
		bFaultFlag = false;
		bCheckFault = false;
	}
	bool bPressed;
	bool bPrevPressed;
	float fTimeLastPressed;
	float fTimeLastReleased;
	bool bPressedFault;
	bool bUnpressedFault;
	bool bFaultFlag;
	bool bCheckFault;
};

typedef std::map<int, BUTTONSTATE> BUTTONSTATEMAP;

class IJoystick
{
public:
	/// Updates axis positions and buttons being pressed
	/// \param fTime = Delta time
	virtual void UpdateInputStates( float fTime ) = 0;

	/// Updates joystick motors based on the specified speed
	/// \param fTime = Delta time
	virtual void UpdateMotorStates( float fTime ) = 0;

	/// Enables fault detection
	/// \param iInput = Button or axis enumeration
	/// \param bEnable = Value which faults will be set
	virtual void EnableFaults( int iInput, bool bEnable ) = 0;
	
	/// Increments fault timers and checks the faults
	virtual void UpdateFaults() = 0;
	
	/// Checks whether or not a fault exists with the specified input
	/// \param iInput = Button or axis enumeration
	/// \return true if succesful, false otherwise
	virtual bool HasFault( int iInput ) = 0;
	
	/// Checks whether or not a fault exists with the specified axis
	/// \param axisIdentifier = Axis identifier
	/// \return true if succesful, false otherwise
	virtual bool HasAxisChanged( char axisIdentifier ) = 0;
	
	/// Gets position value of the specified axis
	/// \param axisIdentifier = Axis identifier
	/// \return axis value
	virtual float GetAxisValue( char axisIdentifier ) = 0;
	
	/// Resets the min/max axis extremities
	/// \param axisIdentifier = Axis identifier
	/// \param min = Min extremity
	/// \param max = Max extremity
	virtual void ResetAxisCalibration( char axisIdentifier, int min, int max ) = 0;
	
	/// Gets the ID of the joystick
	/// \return Joystick ID
	virtual int GetJoystickId() = 0;
	
	/// Gets the ID of the joystick assigned by auto-detection
	/// \return Auto-detected joystick ID
	virtual int GetAutoDetectedJoystickId() = 0;
	
	/// Sets the joystick ID
	/// \param id = ID which to assign to the joystick
	virtual void SetJoystickId( int id ) = 0;
	
	/// Gets the state map struct of the joystick
	/// \return state map struct
	virtual BUTTONSTATEMAP *GetButtonStateMap() = 0;

	/// Returns whether or not the joystick is connected to the system
	/// \return true if successful, false otherwise
	virtual bool IsConnected() = 0;

	/// Returns the speed of the specified motor
	/// \return motor speed
	virtual float GetMotorSpeed( char motorIdentifier ) = 0;

	/// Sets the speed of the specified motor
	virtual void SetMotorSpeed( char motorIdentifier, float speed ) = 0;

	/// Sets the speed of all motors to zero
	virtual void StopMotors() = 0;
};

}	// namespace ElementalEngine

#endif