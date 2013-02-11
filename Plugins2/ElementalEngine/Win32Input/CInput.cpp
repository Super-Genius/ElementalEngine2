///============================================================================
/// \file		CInput.cpp
/// \brief		Implementation file for Input Class
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
#include "KeyDefs.h"

//bool DICALLBACK EEDIEnumCallBack(LPDIDEVICEINSTANCE lpddi, LPVOID pvRef);

// EE Key Definition Table
// Used to map keycodes to strings
struct SEEKey keys[256] =
{
  {0, 0, 0, "Undefined"},
  {1, 0, 0, "Any"},
  {2, 0, 0, "Left Shift"},
  {3, 0, 0, "Right Shift"},
  {4, 0, 0, "Left Control"},
  {5, 0, 0, "Right Control"},
  {6, 0, 0, "Left Alt"},
  {7, 0, 0, "Alt Gr"},
  {8, 0, 0, "Backspace"},
  {9, 0, 0, "Tab"},

  {10, 0, 0, "Left WinKey"},
  {11, 0, 0, "Right WinKey"},
  {12, 0, 0, "Menu Key"},
  {13, 0, 0, "Enter"},
  {14, 0, 0, "Left Command"},
  {15, 0, 0, "Right Command"},
  {16, 0, 0, "Special"},
  {17, 0, 0, "Left Meta"},
  {18, 0, 0, "Right Meta"},
  {19, 0, 0, ""},

  {20, 0, 0, ""},
  {21, 0, 0, "Insert"},
  {22, 0, 0, "Delete"},
  {23, 0, 0, "Home"},
  {24, 0, 0, "End"},
  {25, 0, 0, "PgUp"},
  {26, 0, 0, "PgDn"},
  {27, 0, 0, "Esc"},
  {28, 0, 0, "Up"},
  {29, 0, 0, "Down"},

  {30, 0, 0, "Left"},
  {31, 0, 0, "Right"},
  {32, ' ', ' ', "Space"},
  {EEK_COMMA,  ',', '<', ","},  // Uppercase is US standard, patched at startup if different
  {EEK_PERIOD, '.', '>', "."},  // Uppercase is US standard, patched at startup if different
  {EEK_MINUS,  '-', '_', "-"},
  {EEK_GRAVE,  '`', '~', "`"},
  {37, 0, 0, ""},
  {38, 0, 0, ""},
  {39, 0, 0, ""},

  {40, 0, 0, ""},
  {42, 0, 0, ""},
  {41, 0, 0, ""},
  {43, '=', '+', "="},
  {44, ',', '<', ","}, 
  {45, '-', '_', "-"},
  {46, '.', '>', "."},
  {47, '/', '?', "/"},
  {48, '0', ')', "0"},  // Uppercase is US standard, patched at startup if different
  {49, '1', '!', "1"},  // Uppercase is US standard, patched at startup if different

  {50, '2', '@', "2"},  // Uppercase is US standard, patched at startup if different
  {51, '3', '#', "3"},  // Uppercase is US standard, patched at startup if different
  {52, '4', '$', "4"},  // Uppercase is US standard, patched at startup if different
  {53, '5', '%', "5"},  // Uppercase is US standard, patched at startup if different
  {54, '6', '^', "6"},  // Uppercase is US standard, patched at startup if different
  {55, '7', '&', "7"},  // Uppercase is US standard, patched at startup if different
  {56, '8', '*', "8"},  // Uppercase is US standard, patched at startup if different
  {57, '9', '(', "9"},  // Uppercase is US standard, patched at startup if different
  
  {58, 0, 0, "Caps Lock"},
  {59, 0, 0, "Scroll Lock"},

  {60, 0, 0, "Num Lock"},
  {61, 0, 0, "Print Screen"},
  {62, 0, 0, "Pause"},
  {63, 0, 0, ""},
  {64, 0, 0, ""},
  {65,'a','A', "A"},
  {66,'b','B', "B"},
  {67,'c','C', "C"},
  {68,'d','D', "D"},
  {69,'e','E', "E"},

  {70,'f','F', "F"},
  {71,'g','G', "G"},
  {72,'h','H', "H"},
  {73,'i','I', "I"},
  {74,'j','J', "J"},
  {75,'k','K', "K"},
  {76,'l','L', "L"},
  {77,'m','M', "M"},
  {78,'n','N', "N"},
  {79,'o','O', "O"},

  {80,'p','P', "P"},
  {81,'q','Q', "Q"},
  {82,'r','R', "R"},
  {83,'s','S', "S"},
  {84,'t','T', "T"},
  {85,'u','U', "U"},
  {86,'v','V', "V"},
  {87,'w','W', "W"},
  {88,'x','X', "X"},
  {89,'y','Y', "Y"},

  {90,'z','Z', "Z"},
  {91,'/','/', "Keypad /"},
  {92,'*','*', "Keypad *"},
  {93,'-','-', "Keypad -"},
  {94,'+','+', "Keypad +"},
  {95, 0, 0, "Keypad Enter"},
  {96, 0, 0, "Keypad Del"},
  {97, 0, 0, "Keypad Ins"},
  {98, 0, 0, "Keypad End"},
  {99, 0, 0, "Keypad Down"},

  {100, 0, 0, "Keypad PgDn"},
  {101, 0, 0, "Keypad Left"},
  {102, 0, 0, "Keypad 5"},
  {103, 0, 0, "Keypad Right"},
  {104, 0, 0, "Keypad Home"},
  {105, 0, 0, "Keypad Up"},
  {106, 0, 0, "Keypad PgUp"},
  {107, 0, 0, ""},
  {108, 0, 0, ""},
  {109, 0, 0, ""},

  {110, 0, 0, "F1"},
  {111, 0, 0, "F2"},
  {112, 0, 0, "F3"},
  {113, 0, 0, "F4"},
  {114, 0, 0, "F5"},
  {115, 0, 0, "F6"},
  {116, 0, 0, "F7"},
  {117, 0, 0, "F8"},
  {118, 0, 0, "F9"},
  {119, 0, 0, "F10"},

  {120, 0, 0, "F11"},
  {121, 0, 0, "F12"},
  {122, 0, 0, ""},
  {123, 0, 0, ""},
  {124, 0, 0, ""},
  {125, 0, 0, ""},
  {126, 0, 0, ""},
  {127, 0, 0, ""},
  {128, 0, 0, ""},
  {129, 0, 0, ""},

  //US/English keyboard
  {EEK_EQUAL,  '=','+', "="}, // 130
  {EEK_LSQBRK, '[','{', "["},
  {EEK_RSQBRK, ']','}', "]"},
  {EEK_SEMICL, ';',':', ";"},
  {EEK_APOSTR, '\'','\"', "'"},
  {EEK_BACKSL, '\\','|', "\\"},
  {EEK_SLASH,  '/','?', "/"},
  {137, 0, 0, ""},
  {138, 0, 0, ""},
  {139, 0, 0, ""},

  //German keyboard
  {EEK_DE_SS,    'ß','?',  "ß"},  //140
  {EEK_DE_ACCENT,'´','`',  "´"},
  {EEK_DE_UE,    'ü','Ü',  "Ü"},
  {EEK_DE_PLUS,  '+','*',  "+"},
  {EEK_DE_OE,    'ö','Ö',  "Ö"},
  {EEK_DE_AE,    'ä','Ä',  "Ä"},
  {EEK_DE_HASH,  '#','\'', "#"},
  {EEK_DE_LT,    '<','>',  "<"},
  {EEK_DE_CIRC,  '^','°',  "^"},
  {149, 0, 0, ""},

  {150, 0, 0, ""},
  {151, 0, 0, ""},
  {152, 0, 0, ""},
  {153, 0, 0, ""},
  {154, 0, 0, ""},
  {155, 0, 0, ""},
  {156, 0, 0, ""},
  {157, 0, 0, ""},
  {158, 0, 0, ""},
  {159, 0, 0, ""},

  {160, 0, 0, ""},
  {161, 0, 0, ""},
  {162, 0, 0, ""},
  {163, 0, 0, ""},
  {164, 0, 0, ""},
  {165, 0, 0, ""},
  {166, 0, 0, ""},
  {167, 0, 0, ""},
  {168, 0, 0, ""},
  {169, 0, 0, ""},

  {170, 0, 0, ""},
  {171, 0, 0, ""},
  {172, 0, 0, ""},
  {173, 0, 0, ""},
  {174, 0, 0, ""},
  {175, 0, 0, ""},
  {176, 0, 0, ""},
  {177, 0, 0, ""},
  {178, 0, 0, ""},
  {179, 0, 0, ""},

  {180, 0, 0, ""},
  {181, 0, 0, ""},
  {182, 0, 0, ""},
  {183, 0, 0, ""},
  {184, 0, 0, ""},
  {185, 0, 0, ""},
  {186, 0, 0, ""},
  {187, 0, 0, ""},
  {188, 0, 0, ""},
  {189, 0, 0, ""},

  {190, 0, 0, ""},
  {191, 0, 0, ""},
  {192, 0, 0, ""},
  {193, 0, 0, ""},
  {194, 0, 0, ""},
  {195, 0, 0, ""},
  {196, 0, 0, ""},
  {197, 0, 0, ""},
  {198, 0, 0, ""},
  {199, 0, 0, ""},

  {200, 0, 0, ""},
  {201, 0, 0, ""},
  {202, 0, 0, ""},
  {203, 0, 0, ""},
  {204, 0, 0, ""},
  {205, 0, 0, ""},
  {206, 0, 0, ""},
  {207, 0, 0, ""},
  {208, 0, 0, ""},
  {209, 0, 0, ""},

  {210, 0, 0, ""},
  {211, 0, 0, ""},
  {212, 0, 0, ""},
  {213, 0, 0, ""},
  {214, 0, 0, ""},
  {215, 0, 0, ""},
  {216, 0, 0, ""},
  {217, 0, 0, ""},
  {218, 0, 0, ""},
  {219, 0, 0, ""},

  {220, 0, 0, ""},
  {221, 0, 0, ""},
  {222, 0, 0, ""},
  {223, 0, 0, ""},
  {224, 0, 0, ""},
  {225, 0, 0, ""},
  {226, 0, 0, ""},
  {227, 0, 0, ""},
  {228, 0, 0, ""},
  {229, 0, 0, ""},

  {230, 0, 0, ""},
  {231, 0, 0, ""},
  {232, 0, 0, ""},
  {233, 0, 0, ""},
  {234, 0, 0, ""},
  {235, 0, 0, ""},
  {236, 0, 0, ""},
  {237, 0, 0, ""},
  {238, 0, 0, ""},
  {239, 0, 0, ""},

  {240, 0, 0, ""},
  {241, 0, 0, ""},
  {242, 0, 0, ""},
  {243, 0, 0, ""},
  {244, 0, 0, ""},
  {245, 0, 0, ""},
  {246, 0, 0, ""},
  {247, 0, 0, ""},
  {248, 0, 0, ""},
  {249, 0, 0, ""},

  {250, 0, 0, ""},
  {251, 0, 0, ""},
  {252, 0, 0, ""},
  {253, 0, 0, ""},
  {254, 0, 0, ""},
  {255, 0, 0, ""},
};

REGISTER_COMPONENT_SINGLETON(CInput);
//
// Constructor
//
CInput::CInput()
{
	m_pDI = NULL;
	m_pDeviceInstance = NULL;
	m_bInputInitialized = false;
	m_hWnd;

	IToolBox *toolBox = EngineGetToolBox();

	static DWORD msgHash_SetInputHandler = CHashString(_T("SetInputHandler")).GetUniqueID();
	toolBox->SendMessage( msgHash_SetInputHandler, sizeof(IInputHandler *), this);
}

//
// Destructor
//
CInput::~CInput()
{
	IToolBox *toolBox = EngineGetToolBox();
	static DWORD msgHash_UnsetInputHandler = CHashString(_T("UnsetInputHandler")).GetUniqueID();
	toolBox->SendMessage( msgHash_UnsetInputHandler, 0, NULL);
}

IComponent *CInput::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CInput);
}

void CInput::Serialize(IArchive &ar)
{
}

IHashString *CInput::GetComponentType()
{
	static CHashString hszComponentType = _T("CInput");
	return &hszComponentType; 
}

bool CInput::IsKindOf(IHashString *compType)
{
	static CHashString hszComponentType = _T("CInput");
	return (hszComponentType.GetUniqueID() == compType->GetUniqueID());
}

void CInput::Init()
{
}

//
// Initialize the direct input, the keyboard and the mouse
//
HRESULT CInput::Init(INPUTENGPARAMS *inputParams)
{
	HRESULT hr;
	DWORD mouseFlags;
	DWORD keyboardFlags;

	// MOUSE
    if( inputParams->mouseExclusive )
	{
        mouseFlags = DISCL_EXCLUSIVE;
	}
    else
	{
        mouseFlags = DISCL_NONEXCLUSIVE;
	}

    if( inputParams->mouseFG )
	{
        mouseFlags |= DISCL_FOREGROUND;
	}
    else
	{
        mouseFlags |= DISCL_BACKGROUND;
	}

	// KEYBOARD
	if( inputParams->kbExclusive )
	{
        keyboardFlags = DISCL_EXCLUSIVE;
	}
    else
	{
        keyboardFlags = DISCL_NONEXCLUSIVE;
	}

    if( inputParams->kbFG )
	{
        keyboardFlags |= DISCL_FOREGROUND;
	}
    else
	{
        keyboardFlags |= DISCL_BACKGROUND;
	}

	//make new DI object
	if (FAILED(hr = DirectInput8Create(inputParams->hMod, 
									   DIRECTINPUT_VERSION, 
									   IID_IDirectInput8, 
									   (void**)&m_pDI, 
									   NULL)))
	{
		return hr;
	}

	/*
	// Enumerate the mice; will be useful for future code
	m_pDI->EnumDevices(DI8DEVTYPE_MOUSE, (LPDIENUMDEVICESCALLBACK)EEDIEnumCallBack, 0, DIEDFL_ATTACHEDONLY);
	*/

	// initialize the keyboard
	if (FAILED(hr = m_Keyboard.Init(m_pDI,
								   inputParams->hWnd, 
								   keyboardFlags, 
								   inputParams->kbImmediate,
								   inputParams->kbExclusive, 
								   inputParams->kbFG)))
	{
		return hr;
	}

	// initialize the mouse
	if (FAILED(hr = m_Mouse.Init(m_pDI, 
								 inputParams->hWnd, 
								 mouseFlags, 
								 inputParams->mouseImmediate, 
								 inputParams->mouseExclusive, 
								 inputParams->mouseFG)))
	{
		return hr;
	}

	if (m_Mouse.IsInitialized() && m_Keyboard.IsInitialized())
	{
		// input is initialized!
		m_bInputInitialized = true;
	}
	else
	{
		// input is not initialized!
		m_bInputInitialized = false;

		// since the input devices were not 
		// initialized, make sure they are both
		// deinit'd
		m_Keyboard.DeInit();
		m_Mouse.DeInit();
	}

	m_hWnd = inputParams->hWnd;


	return S_OK;
}

//
// Update Keyboard and Mouse
//
void CInput::Update()
{
	// if input has been initialized
	if (m_bInputInitialized)
	{
		// update keyboard
		m_Keyboard.Update();
		// update mouse
		m_Mouse.Update();
		// save old mouse state
		m_OldMouseStatus = m_CurrentMouseStatus;
		// get current mouse state
		m_Mouse.GetMouseStatus(&m_CurrentMouseStatus);
	}
}

//
// Acquire mouse and keyboard
//
void CInput::SetAcquire(bool acquire)
{
	if (acquire)
	{
		m_Keyboard.Acquire();
		m_Mouse.Acquire();
	}
	else
	{
		m_Keyboard.Acquire();
		m_Mouse.Unacquire();
	}
}

//
// DeInitialize the keyboard and mouse
//
bool CInput::DeInit()
{
	// if input is initialized
	if (m_bInputInitialized)
	{
		// deinit keyboard
		m_Keyboard.DeInit();
		// deinit mouse
		m_Mouse.DeInit();
		// input no longer initialized
		m_bInputInitialized = false;
	}

	return true;
}

//
// Set Keyboard focus
//
void CInput::KBSetFocus()
{
	// TODO:
	// implement this if needed
}

//
// Kill Keyboard focus
//
void CInput::KBKillFocus()
{
	// kill focus
	m_Keyboard.KillFocus();
}

//
// Check the keyboard to see if any key is pressed
//
bool CInput::KBIsAnyKeyPressed()
{
	return m_Keyboard.IsAnyKeyPressed();
}

//
// Check the keyboard to see if any key is released
//
bool CInput::KBIsAnyKeyReleased()
{
	return m_Keyboard.IsAnyKeyReleased();
}

//
// Check the keyboard to see if a specific key is pressed
//
bool CInput::KBIsKeyPressed(int key)
{
	return m_Keyboard.IsKeyPressed(key);
}

//
// Check the keyboard to see if a specific key is released
//
bool CInput::KBIsKeyReleased(int key)
{
	return m_Keyboard.IsKeyReleased(key);
}

void CInput::KBSetSingleHit(int key, bool bSingleHit, bool bSingleRelease)
{
	m_Keyboard.SetSingleHit(key, bSingleHit, bSingleRelease);
}

//
// Return the Key Table
//
const SEEKey* CInput::GetKeyTable() 
{
	return keys;
}

//
// Return the buffered keys hit
//
TCHAR* CInput::KBGetBuffer()
{
	return m_Keyboard.GetBuffer();
}

//
// Set Mouse Focus
//
void CInput::MouseSetFocus(HWND &hWnd)
{
	m_Mouse.SetMouseFocus(m_pDI, hWnd);
}

//
// Kill Mouse Focus
//
void CInput::MouseKillFocus()
{
	m_Mouse.KillMouseFocus();
}

//
// Check mouse to see if left button is pressed
//
bool CInput::IsLeftButtonPressed() const
{
  return m_CurrentMouseStatus.buttonL;
}

//
// Check mouse to see if middle button is pressed
//
bool CInput::IsMiddleButtonPressed() const
{
  return m_CurrentMouseStatus.buttonM;
}

//
// Check mouse to see if right button is pressed
//
bool CInput::IsRightButtonPressed() const
{
  return m_CurrentMouseStatus.buttonR;
}


//
// Check mouse and get its X position
//
long CInput::GetX() const
{
  return m_CurrentMouseStatus.posX;
}

//
// Check mouse and get its Y position
//
long CInput::GetY() const
{
  return m_CurrentMouseStatus.posY;
}

//
// Check mouse and get its Z position
long CInput::GetZ() const
{
	return m_CurrentMouseStatus.posZ;
}

//
// Check mouse and get its delta X 
//
float CInput::GetDeltaX() const
{
  return(float)( m_CurrentMouseStatus.deltaX);
}

//
// Check mouse and get its delta Y
//
float CInput::GetDeltaY() const
{
  return(float)( m_CurrentMouseStatus.deltaY);
}

//
// Check mouse and get its wheel data
//
long CInput::GetWheelDelta() const
{
  return m_CurrentMouseStatus.wheelUp - m_CurrentMouseStatus.wheelDown;
}

//
// Has Input been initialized?
//
bool CInput::IsInitialized()
{
	return m_bInputInitialized;
}

void CInput::GetWindowSize(long &width, long &height)
{
	RECT winRect;
	if (GetClientRect( m_hWnd, &winRect))
	{
		width  = (winRect.right - winRect.left);
		height = (winRect.bottom - winRect.top);
	}
	else
	{
		width = 0;
		height = 0;
	}
}

/*
bool DICALLBACK EEDIEnumCallBack(LPDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	LPDIDEVICEINSTANCE *diInst = GetDeviceInstance();

	if (m_pDeviceInstance)
	{
		delete m_pDeviceInstance;
	}

	//  assert(lpddi);
	if (lpddi == NULL)  // should never happen
	{
		m_pDeviceInstance = NULL;
		return DIENUM_CONTINUE;
	}

	m_pDeviceInstance = new DIDEVICEINSTANCE;
	memcpy(m_pDeviceInstance, lpddi, lpddi->dwSize);

	return DIENUM_STOP; // Only catch the first device
}
*/