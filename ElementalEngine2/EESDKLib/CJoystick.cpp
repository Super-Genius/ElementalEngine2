///============================================================================
/// \file		CJoystick.cpp
/// \brief		Implementation file for Joystick class
/// \date		03-31-2008
/// \author		Richard Nguyen
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

#include "stdafx.h"
#include "JoystickKeyDefs.h"
#include "CJoystick.h"

CJoystick::CJoystick( IJoystickHandler *joystickHandler, int iJoystickId, bool bIsConnected )
{
	m_pToolbox = EngineGetToolBox();
	m_pJoystickHandler = joystickHandler;
	m_iJoystickId = iJoystickId;
	m_iAutoDetectedJoystickId = 0;
	m_bIsConnected = bIsConnected;
}

void CJoystick::UpdateInputStates( float fTime )
{
}

void CJoystick::UpdateMotorStates( float fTime )
{
}

void CJoystick::EnableFaults( int iInput, bool bEnable )
{
	if (iInput == -1)
	{
		for( BUTTONSTATEMAP::iterator itrButtonState=m_ButtonState.begin(); itrButtonState != m_ButtonState.end(); itrButtonState++ )
		{
			BUTTONSTATE *pButtonState = &itrButtonState->second;
			pButtonState->bCheckFault = bEnable;
		}
		
		for( AXISSTATEMAP::iterator itrAxis = m_AxisState.begin(); itrAxis != m_AxisState.end(); itrAxis++ )
		{
			CJoystickAxis *pAxis = &itrAxis->second;
			pAxis->m_bCheckFault = bEnable;
		}		
	}
	else
	{
		BUTTONSTATEMAP::iterator itrButtonState = m_ButtonState.find(iInput);
		if (itrButtonState != m_ButtonState.end())
		{
			BUTTONSTATE *pButtonState = &itrButtonState->second;
			pButtonState->bCheckFault = bEnable;
		}
		AXISSTATEMAP::iterator itrAxis = m_AxisState.find(iInput);
		if (itrAxis != m_AxisState.end())
		{
			CJoystickAxis *pAxis = &itrAxis->second;
			pAxis->m_bCheckFault = bEnable;
		}
	}

	if (bEnable)
	{
		ResetFaults();
	}
}
void CJoystick::ResetFaults()
{
	float fTime = m_pJoystickHandler->GetTime();
	for( BUTTONSTATEMAP::iterator itrButtonState=m_ButtonState.begin(); itrButtonState != m_ButtonState.end(); itrButtonState++ )
	{
		BUTTONSTATE *pButtonState = &itrButtonState->second;
		pButtonState->fTimeLastPressed = fTime;
		pButtonState->fTimeLastReleased = fTime;
		pButtonState->bPressedFault = false;
		pButtonState->bUnpressedFault = false;

		pButtonState->bFaultFlag = false;
	}
	for (AXISSTATEMAP::iterator itrAxis = m_AxisState.begin(); itrAxis != m_AxisState.end(); itrAxis++)
	{
		CJoystickAxis *pAxis = &itrAxis->second;
		pAxis->ResetFaults( fTime );
	}
}
void CJoystick::UpdateFaults()
{
	float fTime = m_pJoystickHandler->GetTime();
	for( BUTTONSTATEMAP::iterator itrButtonState = m_ButtonState.begin(); itrButtonState != m_ButtonState.end(); itrButtonState++ )
	{
		int iButton = itrButtonState->first;
		BUTTONSTATE *pButtonState = &itrButtonState->second;
		bool bUnpressedFault = (fTime - pButtonState->fTimeLastPressed) > m_pJoystickHandler->GetButtonUnpressedFaultTime(iButton);
		bool bPressedFault = (fTime - pButtonState->fTimeLastReleased) > m_pJoystickHandler->GetButtonPressedFaultTime(iButton);
		bool bFaultFlag = (bUnpressedFault || bPressedFault);

		pButtonState->bUnpressedFault = bUnpressedFault;
		pButtonState->bPressedFault = bPressedFault;

		if ((bFaultFlag != pButtonState->bFaultFlag) && ((pButtonState->bCheckFault) || (!bFaultFlag)))
		{
			pButtonState->bFaultFlag = bFaultFlag;

			JOYSTICKHARDWAREFAULTPARAMS jhrp;
			if (m_pJoystickHandler->IsAutoDetect())
			{
				jhrp.iJoystick = m_iAutoDetectedJoystickId;
			}
			else
			{
				jhrp.iJoystick = m_iJoystickId;
			}
			jhrp.iInput = iButton;
			jhrp.bFault = bFaultFlag;
			static DWORD msgJoystickHardwareFault = CHashString(_T("JoystickHardwareFault")).GetUniqueID();
			m_pToolbox->SendMessage(msgJoystickHardwareFault, sizeof(jhrp), &jhrp);			
		}
	}
	for (AXISSTATEMAP::iterator itrAxis = m_AxisState.begin(); itrAxis != m_AxisState.end(); itrAxis++)
	{
		int iAxis = itrAxis->first;
		CJoystickAxis *pAxis = &itrAxis->second;

		bool bNoChangeFault = (fTime - pAxis->GetTimeAxisLastChanged()) > m_pJoystickHandler->GetAxisNoChangeFaultTime(iAxis);
		bool bFaultFlag = (bNoChangeFault);

		pAxis->m_bNoChangeFault = bNoChangeFault;

		if ((bFaultFlag != pAxis->m_bFaultFlag) && ((pAxis->m_bCheckFault) || (!bFaultFlag)))
		{
			pAxis->m_bFaultFlag = bFaultFlag;

			JOYSTICKHARDWAREFAULTPARAMS jhrp;
			if (m_pJoystickHandler->IsAutoDetect())
			{
				jhrp.iJoystick = m_iAutoDetectedJoystickId;
			}
			else
			{
				jhrp.iJoystick = m_iJoystickId;
			}
			jhrp.iInput = iAxis;
			jhrp.bFault = bFaultFlag;
			static DWORD msgJoystickHardwareFault = CHashString(_T("JoystickHardwareFault")).GetUniqueID();
			m_pToolbox->SendMessage(msgJoystickHardwareFault, sizeof(jhrp), &jhrp);			
		}
	}
}
bool CJoystick::HasFault( int iInput )
{
	if (iInput == EEJ_NONE)
	{
		bool bFaultFlag = false;
		for( BUTTONSTATEMAP::iterator itrButtonState=m_ButtonState.begin(); itrButtonState != m_ButtonState.end(); itrButtonState++ )
		{
			int iButton = itrButtonState->first;
			BUTTONSTATE *pButtonState = &itrButtonState->second;

			if (pButtonState->bCheckFault)
			{				
				bFaultFlag |= pButtonState->bFaultFlag;
			}
		}
		for (AXISSTATEMAP::iterator itrAxis = m_AxisState.begin(); itrAxis != m_AxisState.end(); itrAxis++)
		{
			CJoystickAxis *pAxis = &itrAxis->second;
			if (pAxis->m_bCheckFault)
			{
				bFaultFlag |= pAxis->m_bFaultFlag;
			}
		}
		return bFaultFlag;
	}
	else
	{
		BUTTONSTATEMAP::iterator itrButtonState = m_ButtonState.find(iInput);
		if (itrButtonState != m_ButtonState.end())
		{
			BUTTONSTATE *pButtonState = &itrButtonState->second;
			return pButtonState->bFaultFlag;
		}
		AXISSTATEMAP::iterator itrAxis = m_AxisState.find(iInput);
		if (itrAxis != m_AxisState.end())
		{
			CJoystickAxis *pAxis = &itrAxis->second;
			return pAxis->m_bFaultFlag;
		}
		return true;
	}
}

bool CJoystick::HasAxisChanged( char axisIdentifier )
{
	return m_AxisState[axisIdentifier].HasChanged();
}

float CJoystick::GetAxisValue( char axisIdentifier )
{
	return m_AxisState[axisIdentifier].GetValue();
}

void CJoystick::ResetAxisCalibration( char axisIdentifier, int min, int max )
{
	m_AxisState[axisIdentifier].ResetAxisCalibration( min, max );
}

int CJoystick::GetJoystickId()
{
	return m_iJoystickId;
}

int CJoystick::GetAutoDetectedJoystickId()
{
	return m_iAutoDetectedJoystickId;
}

void CJoystick::SetJoystickId( int id )
{
	m_iJoystickId = id;
}

BUTTONSTATEMAP *CJoystick::GetButtonStateMap()
{
	return &m_ButtonState;
}

bool CJoystick::IsConnected()
{
	return m_bIsConnected;
}

void CJoystick::NotifyConnected()
{
	m_bIsConnected = true;
	m_pJoystickHandler->NotifyJoystickConnected( m_iJoystickId );
}

void CJoystick::NotifyDisconnected()
{
	m_bIsConnected = false;
	m_pJoystickHandler->NotifyJoystickDisconnected( m_iJoystickId );
}

float CJoystick::GetMotorSpeed( char motorIdentifier )
{
	return m_MotorState[ motorIdentifier ];
}

void CJoystick::SetMotorSpeed( char motorIdentifier, float speed )
{
	if( speed > 1.0f )
	{
		speed = 1.0f;
	}
	else if( speed < 0.0f )
	{
		speed = 0.0f;
	}
	
	m_MotorState[ motorIdentifier ] = speed;
}

void CJoystick::StopMotors()
{
	MOTORSTATEMAP::iterator motorIterator = m_MotorState.begin();
	for( ; motorIterator != m_MotorState.end(); motorIterator++ )
	{
		SetMotorSpeed( motorIterator->first, 0 );
	}
}