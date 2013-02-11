///============================================================================
/// \file		CMouse.cpp
/// \brief		Implementation file for Mouse class
/// \date		04-23-2005
/// \author		Josh Lulewicz
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
#include "DXVersion.h"
#include "CInput.h"

#include <tchar.h>

#define DINPUT_BUFFERSIZE 512

///
/// Constructor
///
CMouse::CMouse()
{
	// initialize mouse data
	m_lMouseX = m_lMouseY = m_lMouseZ = 0;
	m_lMouseL = m_lMouseM = m_lMouseR = 0;
	m_lMouseWheelUp = m_lMouseWheelDown = 0;
	m_lMouseX0 = m_lMouseX1 = 0;
	m_lMouseDX = m_lMouseDY = 0;
	m_lMouseAX = m_lMouseAY = 0;

	// mouse device
	m_pMouseDev = 0;

	// not initialized
	m_bIsMouseInitialized = false;
	// not using direct input mouse
	m_bUsingDIMouse = false;
	// not capturing
	m_bMouseCapture = false;
}

///
/// Destructor
///
CMouse::~CMouse()
{
	// if we have a device
	if (m_pMouseDev)
	{
		// release it
		m_pMouseDev->Unacquire();
		m_pMouseDev->Release();
		m_pMouseDev = 0;
	}
}

///
/// Initialize Mouse
///
HRESULT CMouse::Init(LPDIRECTINPUT8 &pDI, 
					 HWND &hWnd,
					 DWORD mouseFlags,
					 bool mouseImmediate, 
					 bool mouseExclusive, 
					 bool mouseFG)
{
	HRESULT hr;

	// store the window handle
	m_hWnd = hWnd;

	// set mode
	if (mouseImmediate)
	{
		m_RetrievalMode = DI_IMMEDIATE;
	}
	else
	{
		m_RetrievalMode = DI_BUFFERED;
	}

	// setup the mouse
	if (FAILED(hr = SetupDIMouse(pDI, hWnd, mouseFlags, mouseImmediate, 
					 mouseExclusive, mouseFG)))
	{
		return hr;
	}

	if (m_bIsMouseInitialized)
	{
		// hide cursor
		ShowCursor(0);
		m_bUsingDIMouse = true;
		m_bMouseCapture = true;

		return S_OK;
	}

	// is this necessary???
	/*
	if (hWnd && (!m_bIsMouseInitialized))
	{
		m_bIsMouseInitialized = true;
		m_bUsingDIMouse = false;
		ReleaseCapture();
		ShowCursor(1);
		SetForegroundWindow(hWnd);
		SetCapture(hWnd);
		m_bMouseCapture = true;
		return ERR_NOERROR;
	}
	*/

	return S_OK;
}

///
/// Update Mouse
///
void CMouse::Update()
{
	HRESULT r;

	if (m_bIsMouseInitialized && m_bUsingDIMouse)
	{
		if (m_RetrievalMode == DI_IMMEDIATE)
		{
			r = GetMouseStateImmediate();
		}
		else 
		{
			r = GetMouseStateBuffered();
		}

		// restrict mouse to within the window only
		/*
		if (!FAILED(r))
		{
			m_lMouseX += m_lMouseDX;
			if (m_lMouseX > m_lWindowWidth - 1)
			{
				m_lMouseX = m_lWindowWidth - 1;
			}
			if (m_lMouseX < 0)
			{
//				m_lMouseX = 0;
			}
			m_lMouseY += m_lMouseDY;
			if (m_lMouseY > m_lWindowHeight - 1)
			{
				m_lMouseY = m_lWindowHeight - 1;
			}
			if (m_lMouseY < 0)
			{
//				m_lMouseY = 0;    
			}
		}
		*/
			
	}
}

/// 
/// Deinitialize Mouse
///
HRESULT CMouse::DeInit()
{
	HRESULT r;

	// if we are using a direct input mouse
	if (m_bUsingDIMouse)
	{
		// release it
		m_bIsMouseInitialized = false;
		m_bUsingDIMouse = false;
		if (FAILED(r = ReleaseMouse()))
		{
			return r;
		}

		// redisplay cursor
		ShowCursor(1);
	}

	// is this necessary???
	/*
	if (m_hWnd && m_bIsMouseInitialized)
	{
		m_bIsMouseInitialized = false;
		ShowCursor(1);
		ReleaseCapture();
		m_bMouseCapture = false;

		return S_OK;
	}
	*/

	return S_OK;
}

///
/// Get the current status of the mouse
///
void CMouse::GetMouseStatus(SEEMouseData *mouseState)
{
	mouseState->buttonL = m_lMouseL;
	mouseState->buttonM = m_lMouseM;
	mouseState->buttonR = m_lMouseR;
	mouseState->buttonX0 = m_lMouseX0;
	mouseState->buttonX1 = m_lMouseX1;
	mouseState->deltaX = m_lMouseDX;
	mouseState->deltaY = m_lMouseDY;
	mouseState->posX = m_lMouseX;
	mouseState->posY = m_lMouseY;
	mouseState->posZ = m_lMouseZ;
	mouseState->wheelUp = m_lMouseWheelUp;
	mouseState->wheelDown = m_lMouseWheelDown;
	m_lMouseWheelUp = m_lMouseWheelDown = m_lMouseDX = m_lMouseDY = 0; // reset the values
}

///
/// Enable Mouse
///
void CMouse::EnableMouse()
{
	if((m_hWnd != NULL) && m_bIsMouseInitialized)
	{
		if (!m_bMouseCapture)
		{
			m_bMouseCapture = 1;
			ShowCursor(0);
		}
	}
}

/// 
/// Disable Mouse
///
void CMouse::DisableMouse()
{
	if((m_hWnd != NULL) && m_bIsMouseInitialized)
	{
		if (m_bMouseCapture)
		{
			m_bMouseCapture = false;
			ShowCursor(1);
		}
	}
}

///
/// Check to see if a mouse button has been pressed
///
bool CMouse::IsMouseButtonPressed()
{
	if (m_lMouseL || m_lMouseM || m_lMouseR)
	{
		return true;
	}

	return false;
}

/// 
/// Set the mouse focus
///
void CMouse::SetMouseFocus(LPDIRECTINPUT8 &pDI, HWND &hWnd)
{
	if (!m_bUsingDIMouse)
	{
//		Init(pDI, hWnd);
	}

	m_bMouseCapture = true;
}

///
/// Kill Mouse Focus
///
void CMouse::KillMouseFocus()
{
	m_bMouseCapture = false;

	if (!m_bIsMouseInitialized)
	{
		return;
	}

	if (!m_bUsingDIMouse)
	{
		DeInit();
	}
}

/// 
/// Release the Mouse
///
HRESULT CMouse::ReleaseMouse()
{
	HRESULT r;
	DIDEVICEOBJECTDATA od;
	DWORD dwElements;

	if (!m_pMouseDev)
	{
		return ERR_DINPUTERROR;
	}

	dwElements = 1;
	memset(&od, 0, sizeof(DIDEVICEOBJECTDATA));
	r = m_pMouseDev->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &od, &dwElements, 0);
  

	r = m_pMouseDev->Unacquire();

	if (r != DI_OK)
	{
		return ERR_DINPUTERROR;
	}

	m_pMouseDev->Release();

	m_pMouseDev = NULL; // In case this is called twice for some reason

	return ERR_NOERROR;
}

///
/// Setup the direct input mouse
///
HRESULT CMouse::SetupDIMouse(LPDIRECTINPUT8 &pDI, 
							 HWND &hWnd, 
							 DWORD mouseFlags,
							 bool mouseImmediate, 
							 bool mouseExclusive, 
							 bool mouseFG)
{
	HRESULT hr;

	// Obtain an interface to the system mouse device.
	hr = pDI->CreateDevice(GUID_SysMouse, &m_pMouseDev, NULL);
	if (hr != DI_OK)
	{
        return ERR_NODINPUTDEVICE;
	}

	// Set the data format to "mouse format" - a predefined data format 
    //
    // A data format specifies which controls on a device we
    // are interested in, and how they should be reported.
    //
    // This tells DirectInput that we will be passing a
    // DIMOUSESTATE2 structure to IDirectInputDevice::GetDeviceState.
	hr = m_pMouseDev->SetDataFormat(&c_dfDIMouse2);
	if (hr != DI_OK)
	{
		return ERR_DINPUTERROR;
	}

	// Set the cooperativity level to let DirectInput know how
    // this device should interact with the system and with other
    // DirectInput applications.
	hr = m_pMouseDev->SetCooperativeLevel(hWnd, mouseFlags);
	if( hr == DIERR_UNSUPPORTED && !mouseFG && mouseExclusive )
    {
        DeInit();
		MessageBox( m_hWnd, _T("SetCooperativeLevel() returned DIERR_UNSUPPORTED.\n")
                          _T("For security reasons, background exclusive mouse\n")
                          _T("access is not allowed."), _T("Mouse"), MB_OK );
        
        return S_OK;
    }

    if( FAILED(hr) )
	{
        return hr;
	}

	if( !mouseImmediate )
    {
        // IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
        //
        // DirectInput uses unbuffered I/O (buffer size = 0) by default.
        // If you want to read buffered data, you need to set a nonzero
        // buffer size.
        //
        // Set the buffer size to SAMPLE_BUFFER_SIZE (defined above) elements.
        //
        // The buffer size is a DWORD property associated with the device.
        DIPROPDWORD dipdw;
        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = DINPUT_BUFFERSIZE; // Arbitary buffer size

        if( FAILED( hr = m_pMouseDev->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
            return hr;
    }

	// Acquire the newly created device
    m_pMouseDev->Acquire();

	m_lMouseX = m_lMouseY = m_lMouseDX = m_lMouseDY = 0;

	// mouse has been initialized
	m_bIsMouseInitialized = true;

	return S_OK;
}

///
/// Aquire the Mouse
///
void CMouse::Acquire()
{
	if (m_bIsMouseInitialized)
	{
		m_pMouseDev->Acquire();
	}
}

///
/// Unacquire the Mouse
///
void CMouse::Unacquire()
{
	if (m_bIsMouseInitialized)
	{
		m_pMouseDev->Unacquire();
	}
}

///
/// Get the state of the mouse from DI (immediate mode)
///
HRESULT CMouse::GetMouseStateImmediate()
{
	HRESULT hr;
	DIMOUSESTATE2 dims2;      // DirectInput mouse state structure

	// zero mouse delta in case we fail to get relative mouse position from device
	m_lMouseX = 0;
	m_lMouseY = 0;

	if( m_pMouseDev == NULL ) 
	{
        return S_OK;
	}

	// Get the input's device state, and put the state in dims
    ZeroMemory( &dims2, sizeof(dims2) );
    hr = m_pMouseDev->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 );
    if( FAILED(hr) ) 
    {
        // DirectInput may be telling us that the input stream has been
        // interrupted.  We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done.
        // We just re-acquire and try again.
        
        // If input is lost then acquire and keep trying 
        hr = m_pMouseDev->Acquire();
        while (hr == DIERR_INPUTLOST) 
		{
            hr = m_pMouseDev->Acquire();
		}

        // Update the dialog text 
        if (hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED)
		{
            //SetDlgItemText( m_hWnd, 1012, TEXT("Unacquired") );
		}

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
        return S_OK; 
    }

	m_lMouseX = dims2.lX;
	m_lMouseY = dims2.lY;
	m_lMouseZ = dims2.lZ;

	if ((signed)m_lMouseZ < 0)
	{
		m_lMouseWheelDown -= (signed)m_lMouseZ / 120;
	}
	if ((signed)m_lMouseZ > 0)
	{
		m_lMouseWheelUp += (signed)m_lMouseZ / 120;
	}

	m_lMouseL = (dims2.rgbButtons[0] & 0x80) ? true : false;
	m_lMouseR = (dims2.rgbButtons[1] & 0x80) ? true : false;
    m_lMouseM = (dims2.rgbButtons[2] & 0x80) ? true : false;
	// TODO:
	// add more buttons if necessary, supposedly there can be eight!

	/*
	HRESULT r;
	DIDEVICEOBJECTDATA od;
	DWORD dwElements;

	m_lMouseAX = m_lMouseAY = 0;

	memset(&od, 0, sizeof(DIDEVICEOBJECTDATA));
	dwElements = 1;

	if (!m_pMouseDev)
	{
		return ERR_NODINPUTDEVICE;
	}

	do
	{
		r = m_pMouseDev->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &od, &dwElements, 0);

		if (r == DI_BUFFEROVERFLOW)
		{
			// Place overflow code here
			r = DI_OK;
		}
		if ((r == DIERR_INPUTLOST) || (r == DIERR_NOTACQUIRED)) // Maybe we had focus but lost it?
		{
			m_lMouseX = m_lMouseY = m_lMouseDX = m_lMouseDY = 0;
			// [FLO] since it is annoying when mouse buttons remain pressed
			m_lMouseL = m_lMouseR = m_lMouseM = false;
			m_lMouseX0 = m_lMouseX1 = 0; 

			m_pMouseDev->Acquire();
			r = m_pMouseDev->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &od, &dwElements, 0);      
			if (r == DIERR_INPUTLOST) // Okay, so we don't have focus at all
			{
				return ERR_NOERROR;
			}
		}

		if (FAILED(r))
		{
			return ERR_DINPUTERROR;
		}
		

		if (dwElements < 1)
		{
			return ERR_NOERROR;
		}

		switch (od.dwOfs)
		{
			case DIMOFS_X:
				m_lMouseDX += od.dwData;
				m_lMouseX += od.dwData;
				if (m_lMouseX > m_lWindowWidth - 1)
				{
					m_lMouseX = m_lWindowWidth - 1;
				}
				if (m_lMouseX < 0)
				{
					m_lMouseX = 0;
				}
				break;

			case DIMOFS_Y:
				m_lMouseDY += od.dwData;
				m_lMouseY += od.dwData;
				if (m_lMouseY > m_lWindowHeight - 1)
				{
					m_lMouseY = m_lWindowHeight - 1;
				}
				if (m_lMouseY < 0)
				{
					m_lMouseY = 0;
				}
				break;

			case DIMOFS_Z:      
				if ((signed)od.dwData < 0)
				{
					m_lMouseWheelDown -= (signed)od.dwData / 120;
				}
				if ((signed)od.dwData > 0)
				{
					m_lMouseWheelUp += (signed)od.dwData / 120;
				}
				break;

			case DIMOFS_BUTTON0:
				m_lMouseL = (od.dwData & 0x80)?1:0;
				break;

			case DIMOFS_BUTTON1:
				m_lMouseR = (od.dwData & 0x80)?1:0;
				break;

			case DIMOFS_BUTTON2:
				m_lMouseM = (od.dwData & 0x80)?1:0;
				break;

			case DIMOFS_BUTTON3:
				m_lMouseX0 = (od.dwData & 0x80)?1:0;
				break;

			case DIMOFS_BUTTON4:
				m_lMouseX1 = (od.dwData & 0x80)?1:0;
				break;

			default:
				break;
		}

	} while (dwElements); // Loop if there's still data in the queue
	*/

	return S_OK;
}

///
/// Get the state of the mouse from DI (Buffered Mode)
///
HRESULT CMouse::GetMouseStateBuffered()
{
	// Receives buffered data 
    DIDEVICEOBJECTDATA didod[DINPUT_BUFFERSIZE];  
    DWORD              dwElements;
    DWORD              i;
    HRESULT            hr;

    if( m_pMouseDev == NULL ) 
	{
        return S_OK;
	}
    
    dwElements = DINPUT_BUFFERSIZE;
    hr = m_pMouseDev->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
                                     didod, &dwElements, 0 );
    if( hr != DI_OK ) 
    {
        // We got an error or we got DI_BUFFEROVERFLOW.
        //
        // Either way, it means that continuous contact with the
        // device has been lost, either due to an external
        // interruption, or because the buffer overflowed
        // and some events were lost.
        //
        // Consequently, if a button was pressed at the time
        // the buffer overflowed or the connection was broken,
        // the corresponding "up" message might have been lost.
        //
        // But since our simple sample doesn't actually have
        // any state associated with button up or down events,
        // there is no state to reset.  (In a real game, ignoring
        // the buffer overflow would result in the game thinking
        // a key was held down when in fact it isn't; it's just
        // that the "up" event got lost because the buffer
        // overflowed.)
        //
        // If we want to be cleverer, we could do a
        // GetDeviceState() and compare the current state
        // against the state we think the device is in,
        // and process all the states that are currently
        // different from our private state.
        hr = m_pMouseDev->Acquire();

        while (hr == DIERR_INPUTLOST)
		{
            hr = m_pMouseDev->Acquire();
		}

        // Update the dialog text 
        if (hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED)
		{
          //  SetDlgItemText( hDlg, IDC_DATA, TEXT("Unacquired") );
		}

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
        return S_OK; 
    }

    if (FAILED(hr))  
	{
        return hr;
	}

	// TODO:
	// figure out exactly how to buffer this data


    // Study each of the buffer elements and process them.
    //
    // Since we really don't do anything, our "processing"
    // consists merely of squirting the name into our
    // local buffer.
    for (i = 0; i < dwElements; ++i) 
    {
        switch (didod[i].dwOfs)
        {
            case DIMOFS_BUTTON0:
                m_lMouseL = (didod[i].dwData & 0x80) ? 1 : 0;
                break;

            case DIMOFS_BUTTON1:
                m_lMouseR = (didod[i].dwData & 0x80) ? 1 : 0;
                break;

            case DIMOFS_BUTTON2:
                m_lMouseM = (didod[i].dwData & 0x80) ? 1 : 0;
                break;

            case DIMOFS_BUTTON3:

                break;

            case DIMOFS_BUTTON4:

                break;

            case DIMOFS_BUTTON5:

                break;

            case DIMOFS_BUTTON6:

                break;

            case DIMOFS_BUTTON7:

                break;

            case DIMOFS_X:
				m_lMouseX = didod[i].dwData;
                break;

            case DIMOFS_Y:
                m_lMouseY = didod[i].dwData;
                break;

            case DIMOFS_Z:
                m_lMouseZ = didod[i].dwData;
                break;

            default:
                break;
        }

        switch (didod[i].dwOfs)
        {
            case DIMOFS_BUTTON0:
            case DIMOFS_BUTTON1:
            case DIMOFS_BUTTON2:
            case DIMOFS_BUTTON3:
            case DIMOFS_BUTTON4:
            case DIMOFS_BUTTON5:
            case DIMOFS_BUTTON6:
            case DIMOFS_BUTTON7:
                if (didod[i].dwData & 0x80)
				{
//                    _tcscat( strNewText, TEXT("U ") );
				}
                else
				{
//                    _tcscat( strNewText, TEXT("D ") );
				}
                break;

            case DIMOFS_X:
            case DIMOFS_Y:
            case DIMOFS_Z:
            {
//                TCHAR strCoordValue[20];
//                wsprintf( strCoordValue, TEXT("%d "), didod[ i ].dwData );
//                _tcscat( strNewText, strCoordValue );
                break;
            }
        }
    }

	return S_OK;
}
