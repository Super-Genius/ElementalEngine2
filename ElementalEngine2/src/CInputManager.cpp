///============================================================================
/// \file		CInputManager.cpp
/// \brief		Implementation for CInputManager
/// \date		03-21-2005
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

#include "Stdafx.h"
#include "CInputManager.h"

REGISTER_COMPONENT_SINGLETON(CInputManager);

REGISTER_MESSAGE_HANDLER(SetKeyBinding, OnSetKeyBinding, CInputManager);
REGISTER_MESSAGE_HANDLER(GetKeyBinding, OnGetKeyBinding, CInputManager);
REGISTER_MESSAGE_HANDLER(SetInputHandler, OnSetInputHandler, CInputManager);
REGISTER_MESSAGE_HANDLER(UnsetInputHandler, OnUnsetInputHandler, CInputManager);
REGISTER_MESSAGE_HANDLER(Begin, OnBegin, CInputManager);
REGISTER_MESSAGE_HANDLER(End, OnEnd, CInputManager);
REGISTER_MESSAGE_HANDLER(Start, OnStart, CInputManager);
REGISTER_MESSAGE_HANDLER(Stop, OnStop, CInputManager);
REGISTER_MESSAGE_HANDLER(LoadKeyBinding, OnLoadKeyBinding, CInputManager);
REGISTER_MESSAGE_HANDLER(SetActiveKeyBinding, OnSetActiveKeyBinding, CInputManager);
REGISTER_MESSAGE_HANDLER(GetActiveKeyBinding, OnGetActiveKeyBinding, CInputManager);
REGISTER_MESSAGE_HANDLER(GetCurrentInputMode, OnGetCurrentInputMode, CInputManager);


// TESTING ONLY
REGISTER_MESSAGE_HANDLER(GetMousePosition, OnGetMousePosition, CInputManager);

/// Constructor / Destructor
CInputManager::CInputManager() : 
	CManager(_T("CInputManager"), INPUTPRIORITY)
{
	Init();
	m_pInputModule = NULL;
}

CInputManager::~CInputManager()
{
}

DWORD CInputManager::OnSetKeyBinding(DWORD msgSize, void *keyStruct)
{
	KEYBIND *msgKey;
	VERIFY_MESSAGE_SIZE(sizeof(KEYBIND), msgSize);

	msgKey = (KEYBIND*)keyStruct;
	
	DWORD kbpage = msgKey->m_kbNameID;
	m_KeyMap[kbpage][msgKey->m_KeyID].msg = msgKey->m_MessageID;
	m_KeyMap[kbpage][msgKey->m_KeyID].hitOnce = msgKey->m_bHitOnce;
	m_KeyMap[kbpage][msgKey->m_KeyID].releaseOnce = msgKey->m_bReleaseOnce;
	m_KeyMap[kbpage][msgKey->m_KeyID].bindType = msgKey->m_cBindType;

	return MSG_HANDLED_STOP;
}

DWORD CInputManager::OnGetKeyBinding(DWORD msgSize, void *keyStruct)
{
	KEYBIND *msgKey;
	VERIFY_MESSAGE_SIZE(sizeof(KEYBIND), msgSize);

	msgKey = (KEYBIND*)keyStruct;
	// get message based on key id
	msgKey->m_kbNameID = m_ActiveKeyBindPage;
	msgKey->m_MessageID = m_KeyMap[m_ActiveKeyBindPage][msgKey->m_KeyID].msg;
	msgKey->m_bHitOnce = m_KeyMap[m_ActiveKeyBindPage][msgKey->m_KeyID].hitOnce;
	msgKey->m_cBindType = m_KeyMap[m_ActiveKeyBindPage][msgKey->m_KeyID].bindType;

	return MSG_HANDLED_STOP;
}

DWORD CInputManager::OnGetCurrentInputMode(DWORD msgSize, void *inputStruct)
{
	VERIFY_MESSAGE_SIZE(sizeof(INPUTENGPARAMS), msgSize);

	INPUTENGPARAMS *inputParams;

	inputParams = (INPUTENGPARAMS*)inputStruct;
	*inputParams = m_InputParams;

	return MSG_HANDLED_STOP;
}


DWORD CInputManager::OnBegin(DWORD msgSize, void *inputStruct)
{
	VERIFY_MESSAGE_SIZE(sizeof(INPUTENGPARAMS), msgSize);

	m_InputParams = *((INPUTENGPARAMS*)inputStruct);

	// if the input engine isn't already initialized
	if (m_pInputModule && !m_pInputModule->IsInitialized())
	{
		m_pInputModule->Init(&m_InputParams);

		// reset all values
		m_lCurrentMouseX = 0;
		m_lCurrentMouseY = 0;
		m_lLastMouseX = -99999;
		m_lLastMouseY = -99999;
		m_bMiddleMousePressed = false;
		m_bLeftMousePressed = false;
		m_bRightMousePressed = false;

	}

	return MSG_HANDLED_STOP;
}

DWORD CInputManager::OnEnd(DWORD msgSize, void *nothing)
{
	if (m_pInputModule && m_pInputModule->IsInitialized())
	{
		// deinitialize the input module
		m_pInputModule->DeInit();
	}

	return MSG_HANDLED_STOP;
}

DWORD CInputManager::OnStart(DWORD msgSize, void *nothing)
{
	m_bIsRunning = true;

	return MSG_HANDLED_PROCEED;
}

DWORD CInputManager::OnStop(DWORD msgSize, void *nothing)
{

	m_bIsRunning = false;

	return MSG_HANDLED_PROCEED;

}

DWORD CInputManager::OnGetMousePosition(DWORD msgSize, void *mousePos)
{
	VERIFY_MESSAGE_SIZE(sizeof(MOUSECOORDS), msgSize);

	// set current mouse coords
	((MOUSECOORDS*)mousePos)->m_lXPos = m_lLastMouseX;
	((MOUSECOORDS*)mousePos)->m_lYPos = m_lLastMouseY;

	return MSG_HANDLED_STOP;
}

// We only have a singleton, so return that
IComponent *CInputManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CInputManager);
}

bool CInputManager::Init()
{
	// initialize mouse flags
	m_lCurrentMouseX = 0;
	m_lCurrentMouseY = 0;
	m_lLastMouseX = -99999;
	m_lLastMouseY = -99999;
	m_bMiddleMousePressed = false;
	m_bLeftMousePressed = false;
	m_bRightMousePressed = false;
	m_ActiveKeyBindPage=0;

	m_bIsRunning = false;
	m_fTimeDifference = 0;
	m_fOldCount = GetTickCount();
	m_fTime = 0;

	return CManager::Init();
}

void CInputManager::Update( DWORD tickCount )
{
	if (m_pInputModule && m_pInputModule->IsInitialized())
	{
		m_pInputModule->Update();
		HandleKeyBindings();
		HandleKeyboard();
		HandleMouseEvents();
	}
}

void CInputManager::HandleKeyBindings()
{
	short modifiers = 0;
	short modifierTest = 0;

	DWORD dwOldKB = m_ActiveKeyBindPage;

	// no handler, so don't do anything
	if (!m_pInputModule)
	{
		return;
	}

	// cycle through key bindings
	for (KEYBINDINGMAP::iterator keyItor = m_KeyMap[m_ActiveKeyBindPage].begin(); 
		keyItor != m_KeyMap[m_ActiveKeyBindPage].end();
		++keyItor)
	{
		char bindType = ((*keyItor).second).bindType;

		// get modifiers (if any)
		modifiers = GetModifiers((*keyItor).first);

		// see if key is pressed
		if ( m_pInputModule->KBIsKeyPressed(LOBYTE(LOWORD((*keyItor).first))))
		{
			if( bindType & 1 )
			{
				// check them
				if (m_pInputModule->KBIsKeyPressed(EEK_LSHIFT))
				{
					modifierTest = modifierTest | 0x0001;	
				}
				if (m_pInputModule->KBIsKeyPressed(EEK_RSHIFT))
				{
					modifierTest = modifierTest | 0x0002;	
				}
				if (m_pInputModule->KBIsKeyPressed(EEK_RCTRL))
				{
					modifierTest = modifierTest | 0x0004;	
				}
				if (m_pInputModule->KBIsKeyPressed(EEK_LCTRL))
				{
					modifierTest = modifierTest | 0x0008;		
				}
				if (m_pInputModule->KBIsKeyPressed(EEK_RALT))
				{
					modifierTest = modifierTest | 0x0010;	
				}
				if (m_pInputModule->KBIsKeyPressed(EEK_LALT))
				{
					modifierTest = modifierTest | 0x0020;
				}
		
				// if the modifiers pressed match expected
				if (modifierTest == modifiers)
				{
					KEYEVENTPARAMS keyParams;
					keyParams.m_cBindType = KEYBIND_PRESSED;
					// send message
	//				Vision::Message.Print(1, 0, 60, "%s PRESSED!", (*keyItor).second.c_str());
					DWORD tMsgID = ((*keyItor).second).msg;
					DWORD retVal = m_ToolBox->SendMessage(tMsgID, sizeof(KEYEVENTPARAMS), &keyParams);
					
					// check to see if keybindings were swapped
					if(dwOldKB != m_ActiveKeyBindPage)
					{
						return;
					}
				}
				// reset modifierTest
				modifierTest = 0;
			}
		}
		else if( m_pInputModule->KBIsKeyReleased(LOBYTE(LOWORD((*keyItor).first))) )
		{
			if( bindType & 2 )
			{
				KEYEVENTPARAMS keyParams;
				keyParams.m_cBindType = KEYBIND_RELEASED;
				DWORD tMsgID = ((*keyItor).second).msg;
				DWORD retVal = m_ToolBox->SendMessage(tMsgID, sizeof(KEYEVENTPARAMS), &keyParams);
			}
		}
	}
}

short CInputManager::GetModifiers(DWORD key)
{
	// get modifier portion of the DWORD
	return HIBYTE(LOWORD(key));
}


void CInputManager::ClearAllKeyStates()
{
	int i;
	// go through pressed and time arrays and reset them
	for (i = 0; i < MAX_NUM_KEYS; ++i)
	{
		m_fPressedTime[i] = - KEYREPEAT_DELAYTIME;
		m_bPressed[i] = false;
	}
}

char CInputManager::HandleKeyboard(float dtime)
{
	DWORD oldTime = 0;
	DWORD tickDelta;
	// update counts and time
    oldTime = m_fTime;

	DWORD newCount = GetTickCount();

	tickDelta = newCount - m_fOldCount;

	// if none of the keys are pressed
	if (!m_pInputModule->KBIsAnyKeyPressed())
	{
		// clear the key states and return
		ClearAllKeyStates();
		m_fOldCount = (newCount-1);
		return 0;
	}

	int i;
	char cLastASCII = 0;

	// figure out if shift is pressed
	bool bShift = (m_pInputModule->KBIsKeyPressed(EEK_RSHIFT)!=0)||
				  (m_pInputModule->KBIsKeyPressed(EEK_LSHIFT)!=0);

	// load key table
	const SEEKey *pTable = m_pInputModule->GetKeyTable();

	// go through all the keys
	for (i = 0; i < MAX_NUM_KEYS;i++)
	{
		// check if key is pressed
		bool state = (m_pInputModule->KBIsKeyPressed(i)!=0);

		// check modifiers and escape
		if (i==EEK_RSHIFT || i==EEK_LSHIFT || i==EEK_LCTRL || 
			i==EEK_RCTRL  || i==EEK_LALT || i==EEK_ALTGR || 
			i==EEK_ESC) 
		{
			m_bPressed[i]=state;
			continue;
		}

		// if a key is pressed
		if (state)
		{
			// make sure the repeat delay time as passed
			// if it has see the key pressed to be true
			m_bPressed[i] = (m_fPressedTime[i] == -KEYREPEAT_DELAYTIME);
			// add time change to time array
			m_fPressedTime[i] += tickDelta;

			if (m_fPressedTime[i] >= KEYREPEAT_FREQUENCY)
			{
				m_fPressedTime[i] = fmodf(m_fPressedTime[i],
										   KEYREPEAT_FREQUENCY);
				m_bPressed[i] = true;
				// also display in output
				//OutputDebugString(_T("PASSED TEST!\n"));
			}
		} 
		else
		{
			// otherwise make sure the key isn't pressed
			// in the array
			m_fPressedTime[i] = -KEYREPEAT_DELAYTIME;
			m_bPressed[i] = false;
		}
    
		// map virtual key to ASCII
		if (!cLastASCII && m_bPressed[i])
		{
			const SEEKey *pKey = &pTable[i];
			cLastASCII = (bShift) ? 
						  pKey->m_chUpper:pKey->m_chLower;
		}
	}	
	
	// send message that a key has been pressed
	static DWORD msgHash_KeyPress = CHashString(_T("KeyPress")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_KeyPress, (DWORD)(sizeof(char)), &cLastASCII);

	m_fOldCount = (newCount-1);
	return cLastASCII;
}

// find out if a specific key is pressed
bool CInputManager::IsKeyPressed(int key)
{
	if (key < MAX_NUM_KEYS)
	{
		return m_bPressed[key];
	}
	else
	{
		return FALSE;
	}
}

void CInputManager::HandleMouseEvents()
{
	// mouse coord struct
	MOUSECOORDS mouse;

	if (!m_pInputModule)
	{
		return;
	}

	// get current mouse position
	m_lCurrentMouseX += m_pInputModule->GetX();
	m_lCurrentMouseY += m_pInputModule->GetY();


	// if the mouse has changed
	if (m_lCurrentMouseX != m_lLastMouseX || m_lCurrentMouseY != m_lLastMouseY)
	{
		// get size of window in order to clamp cursor
		long lWindowWidth=0, lWindowHeight=0;
		m_pInputModule->GetWindowSize(lWindowWidth, lWindowHeight);

		// calc mouse delta
		long deltaX = m_lCurrentMouseX - m_lLastMouseX;
		long deltaY = m_lCurrentMouseY - m_lLastMouseY;
		// clamp cursor coords
		m_lCurrentMouseX = Clamp<long>( m_lCurrentMouseX, 0, lWindowWidth );
		m_lCurrentMouseY = Clamp<long>( m_lCurrentMouseY, 0, lWindowHeight );

		// send message that mouse moved
		mouse.m_lXDelta = deltaX;
		mouse.m_lYDelta = deltaY;
		mouse.m_lXPos = m_lCurrentMouseX;
		mouse.m_lYPos = m_lCurrentMouseY;
		static DWORD msgHash_MouseMove = CHashString(_T("MouseMove")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_MouseMove, (DWORD)(sizeof(MOUSECOORDS)), &mouse, NULL, NULL);

		m_lLastMouseX = m_lCurrentMouseX;
		m_lLastMouseY = m_lCurrentMouseY;
	}
	else
	{
		// fill in the mouse coord struct
		mouse.m_lXDelta = 0;
		mouse.m_lYDelta = 0;
		mouse.m_lXPos = m_lCurrentMouseX;
		mouse.m_lYPos = m_lCurrentMouseY;
	}

	// if the middle mouse button is pressed
	if (m_pInputModule->IsMiddleButtonPressed())
	{
		if (!m_bMiddleMousePressed)
		{
			// send message middle mouse pressed
			static DWORD msgHash_MiddleMousePress = CHashString(_T("MiddleMousePress")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_MiddleMousePress, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
			m_bMiddleMousePressed = true;
		}
	}
	else
	{
		if (m_bMiddleMousePressed)
		{
			// send message middle mouse released
			static DWORD msgHash_MiddleMouseRelease = CHashString(_T("MiddleMouseRelease")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_MiddleMouseRelease, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
			m_bMiddleMousePressed = false;
		}
	}

	// if the right mouse button is pressed
	if (m_pInputModule->IsRightButtonPressed())
	{
		if (!m_bRightMousePressed)
		{
			// send message right mouse pressed
			static DWORD msgHash_RightMousePress = CHashString(_T("RightMousePress")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_RightMousePress, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
			m_bRightMousePressed = true;
		}
	}
	else
	{
		if (m_bRightMousePressed)
		{
			// send message right mouse released
			static DWORD msgHash_RightMouseRelease = CHashString(_T("RightMouseRelease")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_RightMouseRelease, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
			m_bRightMousePressed = false;
		}
	}

	// if the left mouse button is pressed
	if (m_pInputModule->IsLeftButtonPressed())
	{
		if (!m_bLeftMousePressed)
		{
			// send message left mouse pressed
			static DWORD msgHash_LeftMousePress = CHashString(_T("LeftMousePress")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_LeftMousePress, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
			m_bLeftMousePressed = true;
		}
	}
	else
	{
		if (m_bLeftMousePressed)
		{
			// send message left mouse released
			static DWORD msgHash_LeftMouseRelease = CHashString(_T("LeftMouseRelease")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_LeftMouseRelease, sizeof(MOUSECOORDS), &mouse, NULL, NULL);
			m_bLeftMousePressed = false;
		}
	}

	// check the scroll wheel
	float scroll = -(float)(m_pInputModule->GetWheelDelta());

	if (scroll != 0.0f)
	{
		// send scroll message
		static DWORD msgHash_Scroll = CHashString(_T("Scroll")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_Scroll, (DWORD)(sizeof(float *)), &scroll, NULL, NULL);
	}

}

DWORD CInputManager::OnLoadKeyBinding(DWORD msgSize, void *param)
{
	IHashString *szhFileName;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(IHashString*));
	szhFileName = (IHashString*)param;
	StdString szNodeName;
	
	IXMLArchive *Archive;
	CHashString streamType(_T("File"));
	CREATEARCHIVE ca;
	DWORD retVal = MSG_NOT_HANDLED;
	
	ca.streamData = (void*)szhFileName->GetString();
	ca.mode = STREAM_MODE_READ;
	ca.streamType = &streamType;

	// call the Archive factory to create an XML archive
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (retVal = m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		//log error
		m_ToolBox->SetErrorValue(WARN_INVALID_OPERATION);
		m_ToolBox->Log(LOGWARNING, _T("CInputManager: Could not create archive from: %s\n"), szhFileName->GetString());
		return MSG_NOT_HANDLED;
	}

	Archive = dynamic_cast<IXMLArchive *>(ca.archive);
	int num;
	CHashString curKeyBind(_T(""));
	StdString keybindName;
	StdString parentType;
	StdString childType;
	StdString msg;
	int		  key;
	bool	  hitOnce;

	Archive->GetNode(szNodeName);
	Archive->Read(keybindName, _T("name"));
	Archive->Read(num, _T("num"));
	
	curKeyBind = keybindName.c_str();
	
	while (num != 0)
	{
		Archive->Read( msg, _T("msg") );
		Archive->Read( key, _T("key") );
		Archive->Read( hitOnce, _T("hitOnce") );
        //Add Entry to page			
		CHashString szMessage(msg);
		KEYBINDINGMAP &curPage = m_KeyMap[ curKeyBind.GetUniqueID() ];
		curPage[ key ].msg = szMessage.GetUniqueID();
		curPage[ key ].hitOnce = hitOnce;
		num--;
	}

	Archive->Close();
	return MSG_HANDLED_STOP;
}

DWORD CInputManager::OnSetActiveKeyBinding(DWORD msgSize, void *param)
{
	DWORD id;

	VERIFY_MESSAGE_SIZE(msgSize, sizeof(DWORD*));

	id = *((DWORD*)param);
	
	if (m_pInputModule && m_pInputModule->IsInitialized())
	{
		// first check that the keybinding id is valid
		KEYBINDINGPAGELIST::iterator kbplIter = m_KeyMap.find(id);
		if(kbplIter != m_KeyMap.end())
		{
			m_ActiveKeyBindPage = id;

			SetupKeyHitOnce();

			return MSG_HANDLED_STOP;
		}
		else
		{
			return MSG_NOT_HANDLED;
		}
	}
	else
	{
		m_ToolBox->Log(LOGERROR, _T("Input manager: must initialize input module before specifying keybindings\n\tCall message 'begin'\n"));
		return MSG_NOT_HANDLED;
	}
}

DWORD CInputManager::OnGetActiveKeyBinding(DWORD msgSize, void *param)
{
	DWORD *id;

	VERIFY_MESSAGE_SIZE(msgSize, sizeof(DWORD*));

	id = (DWORD*)param;

	*id = m_ActiveKeyBindPage;

	return MSG_HANDLED_STOP;
}

void CInputManager::SetupKeyHitOnce()
{
	if (!m_pInputModule)
	{
		return;
	}

	for (KEYBINDINGMAP::iterator keyItor = m_KeyMap[m_ActiveKeyBindPage].begin(); 
		keyItor != m_KeyMap[m_ActiveKeyBindPage].end();
		++keyItor)
	{
		// set the key to single hit or not
		int key = (int)keyItor->first;
		KEYBIND_DATA keydata = keyItor->second;
		m_pInputModule->KBSetSingleHit(key, keydata.hitOnce, keydata.releaseOnce);
	}
}

DWORD CInputManager::OnSetInputHandler(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(IInputHandler*) );
	m_pInputModule = (IInputHandler*)param;
	return MSG_HANDLED_STOP;
}

DWORD CInputManager::OnUnsetInputHandler(DWORD size, void *param)
{
	m_pInputModule = NULL;
	return MSG_HANDLED_STOP;
}
