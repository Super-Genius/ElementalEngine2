///============================================================================
/// \file		CJoystick.h
/// \brief		Definition file for Joystick class
/// \date		03-31-2008
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

#ifndef CJOYSTICK_H__
#define CJOYSTICK_H__

#include "IJoystick.h"
#include "IJoystickHandler.h"
#include "CJoystickAxis.h"

/// \class CJoystick
/// \brief Class to represent a Direct Input Joystick
class CJoystick : public IJoystick
{
public:
	CJoystick( IJoystickHandler *joystickHandler, int iJoystickId, bool bIsConnected );
	virtual void UpdateInputStates( float fTime );
	virtual void UpdateMotorStates( float fTime );
	virtual void EnableFaults( int iInput, bool bEnable );
	virtual void UpdateFaults();
	virtual bool HasFault( int iInput );
	virtual bool HasAxisChanged( char axisIdentifier );
	virtual float GetAxisValue( char axisIdentifier );
	virtual void ResetAxisCalibration( char axisIdentifier, int min, int max );
	virtual int GetJoystickId();
	virtual int GetAutoDetectedJoystickId();
	virtual void SetJoystickId( int id );
	virtual BUTTONSTATEMAP *GetButtonStateMap();
	virtual bool IsConnected();
	virtual float GetMotorSpeed( char motorIdentifier );
	virtual void SetMotorSpeed( char motorIdentifier, float speed );
	virtual void StopMotors();

	typedef std::map<char, CJoystickAxis> AXISSTATEMAP;
	typedef std::map<char, float> MOTORSTATEMAP;

protected:
	virtual void ResetFaults();
	virtual void NotifyConnected();
	virtual void NotifyDisconnected();

	AXISSTATEMAP m_AxisState;
	BUTTONSTATEMAP m_ButtonState;
	MOTORSTATEMAP m_MotorState;

	/// The id of the player as automatically detected by the hardware (if supported)
	int m_iAutoDetectedJoystickId;
	int m_iJoystickId;

private:
	IToolBox *m_pToolbox;
	IJoystickHandler *m_pJoystickHandler;
	bool m_bIsConnected;
};

#endif //#ifndef CJOYSTICK_H__