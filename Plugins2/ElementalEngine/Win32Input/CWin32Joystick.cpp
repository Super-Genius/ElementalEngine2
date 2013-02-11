///============================================================================
/// \file		CWin32Joystick.cpp
/// \brief		Implementation file for Win32Joystick class
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

#include "stdafx.h"
#include "CWin32Joystick.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWin32Joystick::CWin32Joystick( IJoystickHandler *joystickHandler, int iJoystickId ) : CJoystick( joystickHandler, iJoystickId, true )
{
	m_pDevice = NULL;
}

void CWin32Joystick::UpdateInputStates( float fTime )
{
	if( m_pDevice != NULL )
	{
		// set previous button states
		for( BUTTONSTATEMAP::iterator itrButtonState = m_ButtonState.begin(); itrButtonState != m_ButtonState.end(); itrButtonState++ )
		{
			int iButton = itrButtonState->first;
			BUTTONSTATE *pButtonState = &itrButtonState->second;
			pButtonState->bPrevPressed = pButtonState->bPressed;
		}

		// DInput Joystick state 
		DIJOYSTATE2 js;

		memset( &js, 0, sizeof(js) );

		// Poll the device to read the current state
		HRESULT hr = m_pDevice->Poll();
		if( FAILED(hr) )  
		{
			// DInput is telling us that the input stream has been
			// interrupted. We aren't tracking any state between polls, so
			// we don't have any special reset that needs to be done. We
			// just re-acquire and try again.
			hr = m_pDevice->Acquire();
			while( hr == DIERR_INPUTLOST ) 
			{
				hr = m_pDevice->Acquire();
			}

			// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
			// may occur when the app is minimized or in the process of 
			// switching, so just try again later 
		}
		else
		{
			// Get the input's device state
			if( !FAILED( hr = m_pDevice->GetDeviceState( sizeof(DIJOYSTATE2), &js ) ) )
			{
				// The device should have been acquired during the Poll()

				m_AxisState[EEJ_AXISX].ProcessRawInput( js.lX, fTime, false );
				m_AxisState[EEJ_AXISY].ProcessRawInput( js.lY, fTime, false );
				m_AxisState[EEJ_AXISZ].ProcessRawInput( js.lZ, fTime, false );
				m_AxisState[EEJ_AXISRX].ProcessRawInput( js.lRx, fTime, false );
				m_AxisState[EEJ_AXISRY].ProcessRawInput( js.lRy, fTime, false );
				m_AxisState[EEJ_AXISRZ].ProcessRawInput( js.lRz, fTime, false );

				m_ButtonState[EEJ_BUTTON0].bPressed = (js.rgbButtons[0] & 0x80) != 0;
				m_ButtonState[EEJ_BUTTON1].bPressed = (js.rgbButtons[1] & 0x80) != 0;
				m_ButtonState[EEJ_BUTTON2].bPressed = (js.rgbButtons[2] & 0x80) != 0;
				m_ButtonState[EEJ_BUTTON3].bPressed = (js.rgbButtons[3] & 0x80) != 0;
				m_ButtonState[EEJ_BUTTON4].bPressed = (js.rgbButtons[4] & 0x80) != 0;
				m_ButtonState[EEJ_BUTTON5].bPressed = (js.rgbButtons[5] & 0x80) != 0;
				m_ButtonState[EEJ_BUTTON6].bPressed = (js.rgbButtons[6] & 0x80) != 0;
				m_ButtonState[EEJ_BUTTON7].bPressed = (js.rgbButtons[7] & 0x80) != 0;
				m_ButtonState[EEJ_BUTTON8].bPressed = (js.rgbButtons[8] & 0x80) != 0;
				m_ButtonState[EEJ_BUTTON9].bPressed = (js.rgbButtons[9] & 0x80) != 0;
				m_ButtonState[EEJ_BUTTON10].bPressed = (js.rgbButtons[10] & 0x80) != 0;
				m_ButtonState[EEJ_BUTTON11].bPressed = (js.rgbButtons[11] & 0x80) != 0;
				m_ButtonState[EEJ_BUTTON12].bPressed = (js.rgbButtons[12] & 0x80) != 0;
				m_ButtonState[EEJ_BUTTON13].bPressed = (js.rgbButtons[13] & 0x80) != 0;
				m_ButtonState[EEJ_BUTTON14].bPressed = (js.rgbButtons[14] & 0x80) != 0;
				m_ButtonState[EEJ_BUTTON15].bPressed = (js.rgbButtons[15] & 0x80) != 0;

				// auto detect player id from buttons held by hardware
				// NOTE: heroes button box specific... should move somewhere else?
				m_iAutoDetectedJoystickId = 0;
				if ((js.rgbButtons[21] & 0x80) != 0)
				{
					m_iAutoDetectedJoystickId += 1;
				}
				if ((js.rgbButtons[19] & 0x80) != 0)
				{
					m_iAutoDetectedJoystickId += 2;
				}

				for( BUTTONSTATEMAP::iterator itrButtonState = m_ButtonState.begin(); itrButtonState != m_ButtonState.end(); itrButtonState++ )
				{
					int iButton = itrButtonState->first;
					BUTTONSTATE *pButtonState = &itrButtonState->second;

					if (pButtonState->bPressed && !pButtonState->bPrevPressed)
					{
						pButtonState->fTimeLastPressed = fTime;
					}
					else if (!pButtonState->bPressed && pButtonState->bPrevPressed)
					{
						pButtonState->fTimeLastReleased = fTime;
					}
				}
			}
		}
	}
}