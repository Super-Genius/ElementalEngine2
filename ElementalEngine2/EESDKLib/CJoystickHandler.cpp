///============================================================================
/// \file		CJoystickHandler.cpp
/// \brief		Implementation file for Generic Joystick class
/// \date		04-01-2008
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
#include "CJoystickHandler.h"

CJoystickHandler::CJoystickHandler()
{
	m_ToolBox = EngineGetToolBox();
	m_Timer = m_ToolBox->GetTimer();
	m_bAutoDetectJoystickMapping = false;
	m_iAxisResolution = 2000;
	m_iJoystickCount = 0;
}

CJoystickHandler::~CJoystickHandler()
{
	for( std::vector<IJoystick*>::iterator itr = m_Joysticks.begin(); itr != m_Joysticks.end(); itr++ )
	{
		delete *itr;
	}
}

bool CJoystickHandler::Init()
{
	ResetAxisCalibration();
	return true;
}

void CJoystickHandler::Update( DWORD tickcount )
{
	m_fTime = m_Timer->GetRealTime();

	for( std::vector<IJoystick*>::iterator itr = m_Joysticks.begin(); itr != m_Joysticks.end(); itr++ )
	{
		(*itr)->UpdateInputStates( m_fTime );
		(*itr)->UpdateMotorStates( m_fTime );
	}

	// send out messages based on axis and button state changes between this and the last frame
	for( unsigned int iJoystick = 0; iJoystick < m_Joysticks.size(); iJoystick++ )
	{
		IJoystick *joystick = (IJoystick*)m_Joysticks[iJoystick];

		int iMappedJoystick;
		if (m_bAutoDetectJoystickMapping)
		{
			iMappedJoystick = joystick->GetAutoDetectedJoystickId();
		}
		else
		{
			iMappedJoystick = joystick->GetJoystickId();
		}

		BUTTONSTATEMAP *buttonStateMap = joystick->GetButtonStateMap();
		BUTTONSTATEMAP::iterator itrButton = buttonStateMap->begin();
		while (itrButton != buttonStateMap->end())
		{
			int iButton = itrButton->first;
			BUTTONSTATE *pButtonState = &itrButton->second;

			if (pButtonState->bPrevPressed != pButtonState->bPressed)
			{
				if (pButtonState->bPressed)
				{
					// digital button pressed
					RunKeyBinding( iMappedJoystick, iButton, 1.0f, KEYBIND_PRESSED );
				}
				else
				{
					// digital button released
					RunKeyBinding( iMappedJoystick, iButton, 0.0f, KEYBIND_RELEASED );
				}
			}
			else if (pButtonState->bPressed)
			{
				RunRepeatKeyBinding( iMappedJoystick, iButton, 1.0f );
			}
			itrButton++;
		}

		if( joystick->HasAxisChanged( EEJ_AXISX ) )
		{
			RunKeyBinding( iMappedJoystick, EEJ_AXISX, joystick->GetAxisValue(EEJ_AXISX), KEYBIND_ANALOG_CHANGED );
		}
		if (joystick->HasAxisChanged( EEJ_AXISY ) )
		{
			RunKeyBinding( iMappedJoystick, EEJ_AXISY, joystick->GetAxisValue(EEJ_AXISY), KEYBIND_ANALOG_CHANGED );
		}
		if (joystick->HasAxisChanged( EEJ_AXISZ ) )
		{
			RunKeyBinding( iMappedJoystick, EEJ_AXISZ, joystick->GetAxisValue(EEJ_AXISZ), KEYBIND_ANALOG_CHANGED );
		}
		if (joystick->HasAxisChanged( EEJ_AXISRX ) )
		{
			RunKeyBinding( iMappedJoystick, EEJ_AXISRX, joystick->GetAxisValue(EEJ_AXISRX), KEYBIND_ANALOG_CHANGED );
		}
		if (joystick->HasAxisChanged( EEJ_AXISRY ) )
		{
			RunKeyBinding( iMappedJoystick, EEJ_AXISRY, joystick->GetAxisValue(EEJ_AXISRY), KEYBIND_ANALOG_CHANGED );
		}
		if (joystick->HasAxisChanged( EEJ_AXISRZ ) )
		{
			RunKeyBinding( iMappedJoystick, EEJ_AXISRZ, joystick->GetAxisValue(EEJ_AXISRZ), KEYBIND_ANALOG_CHANGED );
		}

		// update hardware faults
		joystick->UpdateFaults();
	}
}

bool CJoystickHandler::DeInit()
{
	return true;
}

IJoystick* CJoystickHandler::GetJoystick( int iJoystick )
{
	IJoystick *retJoystick = NULL;

	for (std::vector<IJoystick*>::iterator itr = m_Joysticks.begin(); itr != m_Joysticks.end() && retJoystick == NULL; itr++)
	{
		IJoystick *joystick = *itr;
		if( m_bAutoDetectJoystickMapping )
		{
			if( joystick->GetAutoDetectedJoystickId() == iJoystick )
			{
				retJoystick = joystick;
			}
		}
		else
		{
			if( joystick->GetJoystickId() == iJoystick )
			{
				retJoystick = joystick;
			}
		}
	}
	return retJoystick;
}

void CJoystickHandler::ResetAxisCalibration()
{
	for( std::vector<IJoystick*>::iterator itr = m_Joysticks.begin(); itr != m_Joysticks.end(); itr++ )
	{
		IJoystick *joystick = *itr;
		joystick->ResetAxisCalibration( EEJ_AXISX, 0, m_iAxisResolution );
		joystick->ResetAxisCalibration( EEJ_AXISY, 0, m_iAxisResolution );
		joystick->ResetAxisCalibration( EEJ_AXISZ, 0, m_iAxisResolution );
		joystick->ResetAxisCalibration( EEJ_AXISRX, 0, m_iAxisResolution );
		joystick->ResetAxisCalibration( EEJ_AXISRY, 0, m_iAxisResolution );
		joystick->ResetAxisCalibration( EEJ_AXISRZ, 0, m_iAxisResolution );
	}
}

void CJoystickHandler::RunKeyBinding( int iDeviceID, int iKeyID, float fValue, int iEvent )
{
	KEYBINDSETMAP::iterator itrKeyBindSet = m_KeyBindSets.find(m_ActiveKeyBindSet.GetUniqueID());
	if( itrKeyBindSet != m_KeyBindSets.end() )
	{
		DEVICEKEYBINDMAP::iterator itrDeviceKeyBinds = itrKeyBindSet->second.find(iDeviceID);
		if( itrDeviceKeyBinds != itrKeyBindSet->second.end() )
		{
			KEYBINDMAP::iterator itrKeyBind = itrDeviceKeyBinds->second.find(iKeyID);
			if( itrKeyBind != itrDeviceKeyBinds->second.end() )
			{
				JOYSTICKKEYBIND &keybind = itrKeyBind->second;
				if ((keybind.iEventFilter & iEvent) != 0)
				{
					KEYEVENTPARAMS params;
					params.m_fValue = fValue;
					params.m_cBindType = iEvent;
					params.m_iDeviceID = iDeviceID;
					params.m_iKeyID = iKeyID;
					m_ToolBox->SendMessage( keybind.MessageID, sizeof(KEYEVENTPARAMS), &params );

					if( keybind.bRepeat == true )
					{
						if( iEvent == KEYBIND_PRESSED )
						{
							keybind.tickCount = GetTickCount();
						}
						else if( iEvent == KEYBIND_RELEASED )
						{
							keybind.tickCount = 0;
						}
					}
				}
			}
		}
	}
}

void CJoystickHandler::RunRepeatKeyBinding( int iDeviceID, int iKeyID, float fValue )
{
	KEYBINDSETMAP::iterator itrKeyBindSet = m_KeyBindSets.find(m_ActiveKeyBindSet.GetUniqueID());
	if( itrKeyBindSet != m_KeyBindSets.end() )
	{
		DEVICEKEYBINDMAP::iterator itrDeviceKeyBinds = itrKeyBindSet->second.find(iDeviceID);
		if( itrDeviceKeyBinds != itrKeyBindSet->second.end() )
		{
			KEYBINDMAP::iterator itrKeyBind = itrDeviceKeyBinds->second.find(iKeyID);
			if( itrKeyBind != itrDeviceKeyBinds->second.end() )
			{
				JOYSTICKKEYBIND &keybind = itrKeyBind->second;
				DWORD tickCount = GetTickCount();

				if( (keybind.bRepeat == true) && ( KEYREPEAT_FREQUENCY < (tickCount - keybind.tickCount) ) )
				{
					KEYEVENTPARAMS params;
					params.m_fValue = fValue;
					params.m_cBindType = KEYBIND_PRESSED;
					m_ToolBox->SendMessage( keybind.MessageID, sizeof(KEYEVENTPARAMS), &params );
					keybind.tickCount = tickCount;
				}
			}
		}
	}
}

void CJoystickHandler::SetKeybinding( DWORD kbNameID, DWORD deviceID, DWORD keyID, JOYSTICKKEYBIND *keybind )
{
	m_KeyBindSets[kbNameID][deviceID][keyID] = *keybind;
}

void CJoystickHandler::SetActiveKeyBinding( DWORD nameID )
{
	m_ActiveKeyBindSet.Init( m_ToolBox->GetHashString( nameID ) );
}

void CJoystickHandler::SwapDeviceMapping( int iOldDeviceID, int iNewDeviceID )
{
	for (size_t i=0; i < m_Joysticks.size(); i++)
	{
		IJoystick *joystick = m_Joysticks[i];
		if ( joystick->GetJoystickId() == iOldDeviceID )
		{
			joystick->SetJoystickId( iNewDeviceID );
		}
		// swap
		else if( joystick->GetJoystickId() == iNewDeviceID )
		{
			joystick->SetJoystickId( iOldDeviceID );
		}
	}
}

void CJoystickHandler::EnableFaultDetection( int iJoystick, int iInput, bool bEnable )
{
	IJoystick *pJoystick = GetJoystick( iJoystick );
	if( pJoystick )
	{
		pJoystick->EnableFaults( iInput, bEnable );
	}
}

bool CJoystickHandler::HasFault( int iJoystick, int iInput )
{
	bool bReturn = false;
	if( iJoystick == -1 )
	{
		bReturn = false;
		for (std::vector<IJoystick*>::iterator itr = m_Joysticks.begin(); itr != m_Joysticks.end(); itr++)
		{
			IJoystick *pJoystick = *itr;
			bReturn |= pJoystick->HasFault( EEJ_NONE );
		}
	}
	else
	{
		IJoystick *pJoystick = GetJoystick(iJoystick);
		if (pJoystick)
		{
			bReturn = pJoystick->HasFault( iInput );
		}
		else
		{
			bReturn = true;
		}
	}

	return bReturn;
}

int CJoystickHandler::GetDeviceMapping( int iDeviceID )
{
	int iReturn = -1;
	if( (UINT)iDeviceID < m_Joysticks.size() && iDeviceID >= 0 )
	{
		iReturn = m_Joysticks[iDeviceID]->GetJoystickId();
	}

	return iReturn;
}

bool CJoystickHandler::SetDeviceMapping( int iOldDeviceID, int iNewDeviceID )
{
	bool bReturn = false;
	if( (UINT)iOldDeviceID < m_Joysticks.size() &&
		(UINT)iNewDeviceID < m_Joysticks.size() &&
		iOldDeviceID >= 0 &&
		iNewDeviceID >= 0 )
	{
		bReturn = true;
		m_Joysticks[iOldDeviceID]->SetJoystickId( iNewDeviceID );
	}

	return bReturn;
}
	
DWORD CJoystickHandler::GetCount()
{
	m_iJoystickCount = 0;
	std::vector<IJoystick*>::iterator joystickIterator = m_Joysticks.begin();
	for( ; joystickIterator != m_Joysticks.end(); joystickIterator++ )
	{
		if( (*joystickIterator)->IsConnected() )
		{
			m_iJoystickCount++;
		}
	}
	DWORD dReturn = (DWORD)m_iJoystickCount;
	return dReturn;
}

void CJoystickHandler::Refresh()
{
	Init();
}

void CJoystickHandler::SetAutoDetectPlayer( bool bAutoDetect )
{
	m_bAutoDetectJoystickMapping = bAutoDetect;
}

void CJoystickHandler::NotifyJoystickConnected( int iJoystick )
{
	static DWORD msgHash_JoystickConnected = CHashString(_T("JoystickConnected")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_JoystickConnected, sizeof(iJoystick), &iJoystick );
}

void CJoystickHandler::NotifyJoystickDisconnected( int iJoystick )
{
	static DWORD msgHash_JoystickDisconnected = CHashString(_T("JoystickDisconnected")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_JoystickDisconnected, sizeof(iJoystick), &iJoystick );
}

void CJoystickHandler::SetMotorSpeed( int iJoystick, char cMotor, float fSpeed )
{
	IJoystick *joystick = GetJoystick( iJoystick );
	if( joystick != NULL )
	{
		joystick->SetMotorSpeed( cMotor, fSpeed );
	}
}