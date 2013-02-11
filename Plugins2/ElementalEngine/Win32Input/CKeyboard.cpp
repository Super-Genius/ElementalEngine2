///============================================================================
/// \file		CKeyboard.cpp
/// \brief		Implementation file for Keyboard Class
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
#include <tchar.h>
#include "DXVersion.h"
#include "CInput.h"
#include "KeyDefs.h"

#define KEYBOARD_BUFFER_SIZE 16


///
/// Constuctor
///
CKeyboard::CKeyboard()
{
	for (int i = 0; i < KEYBUFFER_SIZE; ++i)
	{
		m_cKeyBuffer[i] = 0;
		m_bKeyPressedBuffer[i] = false;
	}
	m_pKBDevice = 0;
	m_bIsKeyboardInitialized = false;
	m_bUsingDIKeyboard = false;
}

///
/// Destructor
///
CKeyboard::~CKeyboard()
{
	if (m_pKBDevice)
	{
		m_pKBDevice->Unacquire();
		m_pKBDevice->Release();
		m_pKBDevice = 0;
	}
}

///
/// Initialize the Keyboard Device
///
HRESULT CKeyboard::Init(LPDIRECTINPUT8 &pDI, 
						HWND &hWnd,
						DWORD keyboardFlags,
						bool kbImmediate,
						bool kbExclusive,
						bool kbFG)
{
	HRESULT hr;

	// clear tables
	for (int i = 0; i < KEYBUFFER_SIZE; ++i)
	{
		m_cKeyStatus[i] = 0;
		m_cKeyStatusSC[i] = 0;
	}
  
	// handle to window
	m_hWnd = hWnd;

	//Initialize keyboard lookup tables
	InitVirtualKeyTable();
	InitDIKeyTable();



	if (FAILED(hr = SetupKeyboard(pDI, hWnd, keyboardFlags, 
		            kbImmediate, kbExclusive, kbFG)))
	{
		return hr;
	}

	if (kbImmediate)
	{
		m_RetrievalMode = DI_IMMEDIATE;
	}
	else 
	{
		m_RetrievalMode = DI_BUFFERED;
	}

	return S_OK;
}

///
/// Update the Keyboard
///
void CKeyboard::Update()
{

	// Just relay this function to the DirectInput interface
	if (m_bUsingDIKeyboard && m_bIsKeyboardInitialized)
	{
		if (m_RetrievalMode == DI_IMMEDIATE)
		{
			GetKeyboardStateImmediate();
		}
		else 
		{
			GetKeyboardStateBuffered();
		}
	}

}

///
/// DeInitialize the Keyboard
///
HRESULT CKeyboard::DeInit()
{
	HRESULT hr;

	if (!m_pKBDevice)
	{
		return S_OK;
	}

	if (FAILED(hr = m_pKBDevice->Unacquire()))
	{
		return hr;
	}

	m_pKBDevice->Release();

	// In case this is called twice for some reason
	m_pKBDevice = NULL; 

	for (int i = 0; i < KEYBUFFER_SIZE; ++i)
	{
		m_cKeyStatus[i] = 0;
		m_cKeyStatusSC[i] = 0;
	}

	m_bIsKeyboardInitialized = false;
	m_bUsingDIKeyboard = false;

	return S_OK;
}

///
/// Acquire the Keyboard
///
void CKeyboard::Acquire()
{
	if (m_bIsKeyboardInitialized)
	{
		m_pKBDevice->Acquire();
	}
}

///
/// Unacquire (release) the Keyboard
///
void CKeyboard::Unacquire()
{
	if (m_bIsKeyboardInitialized)
	{
		m_pKBDevice->Unacquire();
	}
}

///
/// Kill keyboard focus
///
void CKeyboard::KillFocus()
{
	if (m_bIsKeyboardInitialized)
	{
		for (int i = 0; i < KEYBUFFER_SIZE; ++i)
		{
			// Key is now released and lock status removed
			m_cKeyStatus[i] &= ~(KEY_STATE_PRESSED & KEY_STATE_LOCKED & KEY_STATE_RELEASE_LOCKED);   
			m_cKeyStatusSC[i] &= ~(KEY_STATE_PRESSED & KEY_STATE_LOCKED & KEY_STATE_RELEASE_LOCKED);
		}
	}
}

///
/// find out if any key is pressed
///
bool CKeyboard::IsAnyKeyPressed()
{
	return CheckTableAnyKeyPressed(m_cKeyStatus);
}

///
/// find out if any key is released
///
bool CKeyboard::IsAnyKeyReleased()
{
	return CheckTableAnyKeyReleased(m_cKeyStatus);
}

/// 
/// find out if a specific key is pressed
///
bool CKeyboard::IsKeyPressed(int key)
{
	return CheckTableSpecificKeyPressed(key, m_cKeyStatus);
}

/// 
/// find out if a specific key is released
///
bool CKeyboard::IsKeyReleased(int key)
{
	return CheckTableSpecificKeyReleased(key, m_cKeyStatus);
}

///
/// Get the Keyboard State (Immediate Mode)
///
HRESULT CKeyboard::GetKeyboardStateImmediate()
{
	HRESULT hr;
	int changed;

	if (m_pKBDevice == NULL) 
	{
        return S_OK;
	}
    
    // Get the input's device state, and put the state in dims
    ZeroMemory( m_cKeyBuffer, sizeof(m_cKeyBuffer) );
    hr = m_pKBDevice->GetDeviceState( sizeof(m_cKeyBuffer), m_cKeyBuffer );
/*
	if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) // Maybe we had focus but lost it?
	{      
		m_pKBDevice->Acquire();
		hr = m_pKBDevice->GetDeviceState(sizeof(m_cKeyBuffer), m_cKeyBuffer);
		if (hr == DIERR_INPUTLOST) // Okay, so we don't have focus at all
		{
			return S_OK;
		}
	}
*/
    if (FAILED(hr)) 
    {
        // DirectInput may be telling us that the input stream has been
        // interrupted.  We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done.
        // We just re-acquire and try again.
        
        // If input is lost then acquire and keep trying 
        hr = m_pKBDevice->Acquire();
        while (hr == DIERR_INPUTLOST)
		{
            hr = m_pKBDevice->Acquire();
		}

        // Update the dialog text 
        if (hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED) 
		{
            //SetDlgItemText( hDlg, IDC_DATA, TEXT("Unacquired") );
		}

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
        return S_OK; 
    }
    
    for (int i = 0; i < KEYBUFFER_SIZE; ++i) 
    {
		m_cKeyBuffer[i] &= 0x80;
		// Has status of key been changed
		if (m_cKeyBuffer[i] != m_cKeyBufferOld[i]) 
		{
			changed = 1;

			#ifdef DEBUG
				if (m_DirectInputKeys[i] == 0)
				{
					char str[100];
					sprintf(str,"%i",i);
					MessageBox(0,str,"Missing key",MB_OK);
				}
			#endif
			//assert(m_DirectInputKeys[i]);

			if (m_cKeyBuffer[i])
			{
				// update scancode
				KeyDown(i,m_cKeyStatusSC); 
				if (m_DirectInputKeys[i] > 0)
				{
					// Okay, this is pressed, do the stuff
					KeyDown(m_DirectInputKeys[i],m_cKeyStatus); 
				}
			}
			else
			{
				// update scancode
				KeyUp(i,m_cKeyStatusSC); 
				if (m_DirectInputKeys[i] > 0)
				{
					// Okay, this is unpressed, do the stuff
					KeyUp(m_DirectInputKeys[i],m_cKeyStatus); 
				}
			}
	    
			m_cKeyBufferOld[i] = m_cKeyBuffer[i];
		}
	}
    
	return S_OK;
}

void CKeyboard::SetSingleHit(int key, bool bSingleHit, bool bSingleRelease)
{
	SetKeyOnce(key, bSingleHit, bSingleRelease, m_cKeyStatus);
}


///
/// Get the Keyboard State (Buffered Mode)
///
HRESULT CKeyboard::GetKeyboardStateBuffered()
{
	// Receives buffered data 
    DIDEVICEOBJECTDATA didod[KEYBOARD_BUFFER_SIZE];  
    DWORD              dwElements;
    DWORD              i;
    HRESULT            hr;

	if (m_pKBDevice == NULL) 
	{
        return S_OK;
	}
    
    dwElements = KEYBOARD_BUFFER_SIZE;
    hr = m_pKBDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA),
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
        hr = m_pKBDevice->Acquire();

        while (hr == DIERR_INPUTLOST) 
		{
            hr = m_pKBDevice->Acquire();
		}

        // Update the dialog text 
        if (hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED ) 
		{
            //SetDlgItemText( hDlg, IDC_DATA, TEXT("Unacquired") );
		}

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
        return S_OK; 
    }

    // Study each of the buffer elements and process them.
    //
    // Since we really don't do anything, our "processing"
    // consists merely of squirting the name into our
    // local buffer.
    for( i = 0; i < dwElements; i++ ) 
    {
        // this will display then scan code of the key
        // plus a 'D' - meaning the key was pressed 
        //   or a 'U' - meaning the key was released
        wsprintf( strLetter, TEXT("0x%02x%s "), didod[ i ].dwOfs,
                                         (didod[ i ].dwData & 0x80) ? TEXT("D") : TEXT("U"));
        _tcscat( strNewText, strLetter );
    } 

    return S_OK;
}

//
// Setup the direct input keyboard
//
HRESULT CKeyboard::SetupKeyboard(LPDIRECTINPUT8 &m_pDI, 
								 HWND &hWnd,
								 DWORD keyboardFlags,
								 bool kbImmediate,
								 bool kbExclusive,
								 bool kbFG)
{
	HRESULT hr;

	// Obtain an interface to the system keyboard device.
	if( FAILED(hr = m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKBDevice, NULL)))
	{
		return hr;
	}

	// Set the data format to "keyboard format" - a predefined data format 
    //
    // A data format specifies which controls on a device we
    // are interested in, and how they should be reported.
    //
    // This tells DirectInput that we will be passing an array
    // of 256 bytes to IDirectInputDevice::GetDeviceState.
	if( FAILED(hr = m_pKBDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}

	// Set the cooperativity level to let DirectInput know how
    // this device should interact with the system and with other
    // DirectInput applications.
	hr = m_pKBDevice->SetCooperativeLevel(hWnd, keyboardFlags);
	if( hr == DIERR_UNSUPPORTED && !kbFG && kbExclusive )
    {
        DeInit();
		
        MessageBox( m_hWnd, _T("SetCooperativeLevel() returned DIERR_UNSUPPORTED.\n")
                          _T("For security reasons, background exclusive keyboard\n")
                          _T("access is not allowed."), _T("Keyboard"), MB_OK );
	    
        return S_OK;
    }

	if (FAILED(hr))
	{
        return hr;
	}

	if( !kbImmediate )
    {
        // IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
        //
        // DirectInput uses unbuffered I/O (buffer size = 0) by default.
        // If you want to read buffered data, you need to set a nonzero
        // buffer size.
        //
        // Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
        //
        // The buffer size is a DWORD property associated with the device.
        DIPROPDWORD dipdw;

        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size

        if (FAILED(hr = m_pKBDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph )))
		{
            return hr;
		}
    }

	// Acquire the newly created device
	m_pKBDevice->Acquire();
	

	m_bIsKeyboardInitialized = true;
	m_bUsingDIKeyboard = true;

	return S_OK;
}


///
/// Mark a key as down
///
void CKeyboard::KeyDown(int key, char *table)
{
	// If no error has occurred
	if (key >= 0) 
	{
		// Make sure that key is not locked
		if (!(table[key] & KEY_STATE_LOCKED)) 
		{
			// Key is now pressed
			table[key] |= KEY_STATE_PRESSED; 
		}
		// Remove key and release status
		table[key] &= ~(KEY_STATE_RELEASE_LOCKED | KEY_STATE_RELEASED);
	}
}

/// 
/// Mark a key as up
///
void CKeyboard::KeyUp(int key, char *table)
{
	// If no error has occurred
	if (key >= 0) 
	{
		// Key is now released and lock status removed
		table[key] &= ~(KEY_STATE_PRESSED | KEY_STATE_LOCKED);

		// Make sure that key release is not locked
		if (!(table[key] & KEY_STATE_RELEASE_LOCKED)) 
		{
			// Add set key release
			table[key] |= KEY_STATE_RELEASED;
		}
	}
}

///
/// Check key table for any keys that may be down
///
bool CKeyboard::CheckTableAnyKeyPressed(char *table)
{
	for (int i = 0; i < KEYBUFFER_SIZE; ++i)
	{
		if (table[i] & KEY_STATE_PRESSED)
		{
			return true;
		}
	}

	return false;
}

///
/// Check key table for any keys that may be released
///
bool CKeyboard::CheckTableAnyKeyReleased(char *table)
{
	for (int i = 0; i < KEYBUFFER_SIZE; ++i)
	{
		if (table[i] & KEY_STATE_RELEASED)
		{
			return true;
		}
	}

	return false;
}

///
/// Check key table for a specific key
///
bool CKeyboard::CheckTableSpecificKeyPressed(int key, char *table)
{
	// Compensate for malicious users who want to crash things :)
	if ((key < 0) || (key > 255))
		return false;

	// Is the key pressed?
	if (table[key] & KEY_STATE_PRESSED)
	{
		if (table[key] & KEY_STATE_HIT_ONCE) // If this key is registered as press-once
		{
			table[key] |= KEY_STATE_LOCKED;	// Once Only lock bit is set
			table[key] &= ~(KEY_STATE_PRESSED); // Key is not (officially) pressed anymore :)
		}
		return true;
	}

	return false;
}

///
/// Check key table for a specific key
///
bool CKeyboard::CheckTableSpecificKeyReleased(int key, char *table)
{
	// Compensate for malicious users who want to crash things :)
	if ((key < 0) || (key > 255))
		return false;

	// Is the key pressed?
	if (table[key] & KEY_STATE_RELEASED)
	{
		if (table[key] & KEY_STATE_RELEASE_ONCE) // If this key is registered as press-once
		{
			table[key] |= KEY_STATE_RELEASE_LOCKED;	// Once Only lock bit is set
			table[key] &= ~(KEY_STATE_RELEASED); // indicate key as "not released"
		}
		return true;
	}

	return false;
}

/// set as key can only be hit once
void CKeyboard::SetKeyOnce(int key, bool bHitOnce, bool bReleaseOnce, char *table)
{
	if(key >= 0 && key <= 255)
	{
		if(bHitOnce)
		{
			table[key] |= KEY_STATE_HIT_ONCE;		// turn on only once bit
		}
		else
		{
			table[key] &= ~(KEY_STATE_HIT_ONCE);	// turn off only once bit
		}

		if(bReleaseOnce)
		{
			table[key] |= KEY_STATE_RELEASE_ONCE;	// turn on only-once release bit
		}
		else
		{
			table[key] &= ~(KEY_STATE_RELEASE_ONCE);	// turn off only-once release bit
		}
//		table[key] |= 16;	// Lock key release
	}
}


///
/// Initialized the Direct Input Key Table
///
bool CKeyboard::InitDIKeyTable()
{
	memset(&m_DirectInputKeys,0,sizeof(m_DirectInputKeys));

	m_DirectInputKeys[DIK_ESCAPE] = EEK_ESC;
	m_DirectInputKeys[DIK_1] = EEK_1;
	m_DirectInputKeys[DIK_2] = EEK_2;
	m_DirectInputKeys[DIK_3] = EEK_3;
	m_DirectInputKeys[DIK_4] = EEK_4;
	m_DirectInputKeys[DIK_5] = EEK_5;
	m_DirectInputKeys[DIK_6] = EEK_6;
	m_DirectInputKeys[DIK_7] = EEK_7;
	m_DirectInputKeys[DIK_8] = EEK_8;
	m_DirectInputKeys[DIK_9] = EEK_9;
	m_DirectInputKeys[DIK_0] = EEK_0;
	m_DirectInputKeys[DIK_BACK] = EEK_BACKSP; 
	m_DirectInputKeys[DIK_TAB] = EEK_TAB;
	m_DirectInputKeys[DIK_Q] = EEK_Q;
	m_DirectInputKeys[DIK_W] = EEK_W;
	m_DirectInputKeys[DIK_E] = EEK_E;
	m_DirectInputKeys[DIK_R] = EEK_R;
	m_DirectInputKeys[DIK_T] = EEK_T;
	m_DirectInputKeys[DIK_U] = EEK_U;
	m_DirectInputKeys[DIK_I] = EEK_I;
	m_DirectInputKeys[DIK_O] = EEK_O;
	m_DirectInputKeys[DIK_P] = EEK_P;
	m_DirectInputKeys[DIK_RETURN] = EEK_ENTER;
	m_DirectInputKeys[DIK_LCONTROL] = EEK_LCTRL;
	m_DirectInputKeys[DIK_A] = EEK_A;
	m_DirectInputKeys[DIK_S] = EEK_S;
	m_DirectInputKeys[DIK_D] = EEK_D;
	m_DirectInputKeys[DIK_F] = EEK_F;
	m_DirectInputKeys[DIK_G] = EEK_G;
	m_DirectInputKeys[DIK_H] = EEK_H;
	m_DirectInputKeys[DIK_J] = EEK_J;
	m_DirectInputKeys[DIK_K] = EEK_K;
	m_DirectInputKeys[DIK_L] = EEK_L;
	m_DirectInputKeys[DIK_LSHIFT] = EEK_LSHIFT;
	m_DirectInputKeys[DIK_X] = EEK_X;
	m_DirectInputKeys[DIK_C] = EEK_C;
	m_DirectInputKeys[DIK_V] = EEK_V;
	m_DirectInputKeys[DIK_B] = EEK_B;
	m_DirectInputKeys[DIK_N] = EEK_N;
	m_DirectInputKeys[DIK_M] = EEK_M;
	m_DirectInputKeys[DIK_COMMA] = EEK_COMMA;
	m_DirectInputKeys[DIK_PERIOD] = EEK_PERIOD;
	m_DirectInputKeys[DIK_RSHIFT] = EEK_RSHIFT;
	m_DirectInputKeys[DIK_MULTIPLY] = EEK_KP_MUL;
	m_DirectInputKeys[DIK_LMENU] = EEK_LALT;
	m_DirectInputKeys[DIK_SPACE] = EEK_SPACE;
	m_DirectInputKeys[DIK_CAPITAL] = EEK_CAPS;
	m_DirectInputKeys[DIK_F1] = EEK_F1;
	m_DirectInputKeys[DIK_F2] = EEK_F2;
	m_DirectInputKeys[DIK_F3] = EEK_F3;
	m_DirectInputKeys[DIK_F4] = EEK_F4;
	m_DirectInputKeys[DIK_F5] = EEK_F5;
	m_DirectInputKeys[DIK_F6] = EEK_F6;
	m_DirectInputKeys[DIK_F7] = EEK_F7;
	m_DirectInputKeys[DIK_F8] = EEK_F8;
	m_DirectInputKeys[DIK_F9] = EEK_F9;
	m_DirectInputKeys[DIK_F10] = EEK_F10;
	m_DirectInputKeys[DIK_NUMLOCK] = EEK_NUM;
	m_DirectInputKeys[DIK_SCROLL] = EEK_SCROLL;
	m_DirectInputKeys[DIK_NUMPAD7] = EEK_KP_7;
	m_DirectInputKeys[DIK_NUMPAD8] = EEK_KP_8;
	m_DirectInputKeys[DIK_NUMPAD9] = EEK_KP_9;
	m_DirectInputKeys[DIK_SUBTRACT] = EEK_KP_MINUS;
	m_DirectInputKeys[DIK_NUMPAD4] = EEK_KP_4;
	m_DirectInputKeys[DIK_NUMPAD5] = EEK_KP_5;
	m_DirectInputKeys[DIK_NUMPAD6] = EEK_KP_6;
	m_DirectInputKeys[DIK_ADD] = EEK_KP_PLUS;
	m_DirectInputKeys[DIK_NUMPAD1] = EEK_KP_1;
	m_DirectInputKeys[DIK_NUMPAD2] = EEK_KP_2;
	m_DirectInputKeys[DIK_NUMPAD3] = EEK_KP_3;
	m_DirectInputKeys[DIK_NUMPAD0] = EEK_KP_0;
	m_DirectInputKeys[DIK_DECIMAL] = EEK_KP_DEL;
	m_DirectInputKeys[DIK_F11] = EEK_F11;
	m_DirectInputKeys[DIK_F12] = EEK_F12;	
	m_DirectInputKeys[DIK_NUMPADENTER] = EEK_KP_ENTER;
	m_DirectInputKeys[DIK_RCONTROL] = EEK_RCTRL;
	m_DirectInputKeys[DIK_DIVIDE] = EEK_KP_SLASH;
	m_DirectInputKeys[DIK_SYSRQ] = EEK_PRSCR;
	m_DirectInputKeys[DIK_RMENU] = EEK_RALT;
	m_DirectInputKeys[DIK_PAUSE] = EEK_PAUSE;
	m_DirectInputKeys[DIK_HOME] = EEK_HOME;
	m_DirectInputKeys[DIK_UP] = EEK_UP;
	m_DirectInputKeys[DIK_PRIOR] = EEK_PGUP;
	m_DirectInputKeys[DIK_LEFT] = EEK_LEFT;
	m_DirectInputKeys[DIK_RIGHT] = EEK_RIGHT;
	m_DirectInputKeys[DIK_END] = EEK_END;
	m_DirectInputKeys[DIK_DOWN] = EEK_DOWN;
	m_DirectInputKeys[DIK_NEXT] = EEK_PGDN;
	m_DirectInputKeys[DIK_INSERT] = EEK_INS;
	m_DirectInputKeys[DIK_DELETE] = EEK_DEL;
	m_DirectInputKeys[DIK_LWIN] = EEK_LWIN;
	m_DirectInputKeys[DIK_RWIN] = EEK_RWIN;
	m_DirectInputKeys[DIK_APPS] = EEK_APPS;
	m_DirectInputKeys[DIK_Z] = EEK_Z;
	m_DirectInputKeys[DIK_Y] = EEK_Y;
	m_DirectInputKeys[DIK_GRAVE] = EEK_GRAVE;
	m_DirectInputKeys[DIK_MINUS] = EEK_MINUS;
	m_DirectInputKeys[DIK_EQUALS] = EEK_EQUAL;
	m_DirectInputKeys[DIK_LBRACKET] = EEK_LSQBRK;
	m_DirectInputKeys[DIK_RBRACKET] = EEK_RSQBRK;
	m_DirectInputKeys[DIK_BACKSLASH] = EEK_BACKSL;
	m_DirectInputKeys[DIK_SEMICOLON] = EEK_SEMICL;
	m_DirectInputKeys[DIK_APOSTROPHE] = EEK_APOSTR;
	m_DirectInputKeys[DIK_SLASH] = EEK_SLASH;

	return true;
}

///
/// Initialize the Virtual Key Table
///
bool CKeyboard::InitVirtualKeyTable()
{
	memset(&m_iVKTable,0,sizeof(m_iVKTable));
	memset(&m_iVKTableExt,0,sizeof(m_iVKTableExt));

	//Init all known values of real keys
	m_iVKTable[VK_BACK] = EEK_BACKSP;
	m_iVKTable[VK_TAB] = EEK_TAB;
	m_iVKTable[VK_CLEAR] = EEK_KP_5; // CLEAR seems to map to the middle direction button on the numpad
	m_iVKTable[VK_RETURN] = EEK_ENTER;
	m_iVKTable[VK_SHIFT] = EEK_LSHIFT;
	m_iVKTable[VK_CONTROL] = EEK_LCTRL;
	m_iVKTable[VK_MENU] = EEK_LALT;
	m_iVKTable[VK_PAUSE] = EEK_PAUSE;
	m_iVKTable[VK_CAPITAL] = EEK_CAPS;
	m_iVKTable[VK_ESCAPE] = EEK_ESC;
	m_iVKTable[VK_SPACE] = EEK_SPACE;
	m_iVKTable[VK_PRIOR] = EEK_KP_PGUP;
	m_iVKTable[VK_NEXT]  = EEK_KP_PGDN;
	m_iVKTable[VK_END]  = EEK_KP_END;
	m_iVKTable[VK_HOME] = EEK_KP_HOME;
	m_iVKTable[VK_LEFT]  = EEK_KP_LEFT;
	m_iVKTable[VK_UP]    = EEK_KP_UP;
	m_iVKTable[VK_RIGHT]  = EEK_KP_RIGHT;
	m_iVKTable[VK_DOWN]    = EEK_KP_DOWN;
	m_iVKTable[VK_SNAPSHOT] = EEK_PRSCR;
	m_iVKTable[VK_INSERT] = EEK_KP_INS;
	m_iVKTable[VK_DELETE] = EEK_KP_DEL;
	m_iVKTable[VK_LWIN] = EEK_LWIN;
	m_iVKTable[VK_RWIN] = EEK_RWIN;
	m_iVKTable[VK_APPS] = EEK_APPS;
	m_iVKTable[VK_NUMPAD0] = EEK_KP_0;
	m_iVKTable[VK_NUMPAD1] = EEK_KP_1;
	m_iVKTable[VK_NUMPAD2] = EEK_KP_2;
	m_iVKTable[VK_NUMPAD3] = EEK_KP_3;
	m_iVKTable[VK_NUMPAD4] = EEK_KP_4;
	m_iVKTable[VK_NUMPAD5] = EEK_KP_5;
	m_iVKTable[VK_NUMPAD6] = EEK_KP_6;
	m_iVKTable[VK_NUMPAD7] = EEK_KP_7;
	m_iVKTable[VK_NUMPAD8] = EEK_KP_8;
	m_iVKTable[VK_NUMPAD9] = EEK_KP_9;
	m_iVKTable[VK_MULTIPLY] = EEK_KP_MUL;
	m_iVKTable[VK_ADD] = EEK_KP_PLUS;
	m_iVKTable[VK_SUBTRACT] = EEK_KP_MINUS;
	m_iVKTable[VK_DIVIDE] = EEK_KP_SLASH;
	m_iVKTable[VK_F1] = EEK_F1;
	m_iVKTable[VK_F2] = EEK_F2;
	m_iVKTable[VK_F3] = EEK_F3;
	m_iVKTable[VK_F4] = EEK_F4;
	m_iVKTable[VK_F5] = EEK_F5;
	m_iVKTable[VK_F6] = EEK_F6;
	m_iVKTable[VK_F7] = EEK_F7;
	m_iVKTable[VK_F8] = EEK_F8;
	m_iVKTable[VK_F9] = EEK_F9;
	m_iVKTable[VK_F10] = EEK_F10;
	m_iVKTable[VK_F11] = EEK_F11;
	m_iVKTable[VK_F12] = EEK_F12;

	//Do letters
//	assert(EEK_A == 'A'); // make sure they start at the right place
	for (int i='A'; i <= 'Z'; i++)
		m_iVKTable[i] = i;

	//Do numbers
//	assert(EEK_0 == '0'); // make sure they start at the right place
	for (int i='0'; i <= '9'; i++)
	{
		m_iVKTable[i] = i;
	}

	m_iVKTable[VK_NUMLOCK] = EEK_NUM;
	m_iVKTable[VK_SCROLL] = EEK_SCROLL;

	//These are just here for completeness.
	//Only GetAsyncKeyState() and GetKeyState() support them
	m_iVKTable[VK_LSHIFT] = EEK_LSHIFT;
	m_iVKTable[VK_RSHIFT] = EEK_RSHIFT;
	m_iVKTable[VK_LCONTROL] = EEK_LCTRL;
	m_iVKTable[VK_RCONTROL] = EEK_RCTRL;
	m_iVKTable[VK_LMENU] = EEK_LMETA;
	m_iVKTable[VK_RMENU] = EEK_RMETA;

	
	m_iVKTable[0xC0]= EEK_US_TILDE; //   `  

	m_iVKTable[0xBC]= EEK_US_COMMA; //   ,  
	m_iVKTable[0xBE]= EEK_US_PERIOD; //   .  
	m_iVKTable[0xBF]= EEK_US_SLASH; //   /  

	m_iVKTable[0xBD]= EEK_US_MINUS; //   -  
	m_iVKTable[0xBB]= EEK_US_EQUAL; //   =  

	m_iVKTable[0xDB]= EEK_US_LSQBRK; //   [  
	m_iVKTable[0xDD]= EEK_US_RSQBRK; //   ]  

	m_iVKTable[0xBA]= EEK_US_SEMICL; //   ;  
	m_iVKTable[0xDE]= EEK_US_APOSTR; //   '  
	m_iVKTable[0xDC]= EEK_US_BACKSL; //   \  

	//From MSDN:
	/*
	For enhanced 101- and 102-key keyboards, extended keys are the right ALT
	and CTRL keys on the main section of the keyboard; the INS, DEL, HOME, END,
	PAGE UP, PAGE DOWN, and arrow keys in the clusters to the left of the
	numeric keypad; and the divide (/) and ENTER keys in the numeric keypad.
	Other keyboards may support the extended-key bit 
	*/

	m_iVKTableExt[VK_MENU] = EEK_ALTGR;
	m_iVKTableExt[VK_CONTROL] = EEK_RCTRL;

	m_iVKTableExt[VK_INSERT] = EEK_INS;
	m_iVKTableExt[VK_DELETE] = EEK_DEL;
	m_iVKTableExt[VK_PRIOR] = EEK_PGUP;
	m_iVKTableExt[VK_NEXT]  = EEK_PGDN;
	m_iVKTableExt[VK_END]  = EEK_END;
	m_iVKTableExt[VK_HOME] = EEK_HOME;

	m_iVKTableExt[VK_LEFT]  = EEK_LEFT;
	m_iVKTableExt[VK_UP]    = EEK_UP;
	m_iVKTableExt[VK_RIGHT]  = EEK_RIGHT;
	m_iVKTableExt[VK_DOWN]    = EEK_DOWN;

	m_iVKTableExt[VK_DIVIDE]  = EEK_KP_SLASH;
	m_iVKTableExt[VK_RETURN]  = EEK_KP_ENTER;

	// These also seem to be extended keys...
	m_iVKTableExt[VK_NUMLOCK] = EEK_NUM; 
	m_iVKTableExt[VK_SNAPSHOT] = EEK_PRSCR; 
	m_iVKTableExt[VK_LWIN] = EEK_LWIN; 
	m_iVKTableExt[VK_RWIN] = EEK_RWIN; 
	m_iVKTableExt[VK_APPS] = EEK_APPS; 

	return true;
}
