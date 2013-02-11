///============================================================================
/// \file		CInputManager.h
/// \brief		Header file for the Input Manager
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

#ifndef CINPUTMANAGER_H
#define CINPUTMANAGER_H

#include "CManager.h"
#include "IInputHandler.h"

#define MAX_NUM_KEYS  256
// key repeat and deleay times (in seconds)
#define KEYREPEAT_DELAYTIME     500 //0.25f
#define KEYREPEAT_FREQUENCY     250 //0.05f

struct KEYBIND_DATA
{
	DWORD	msg;
	bool	hitOnce;
	bool	releaseOnce;
	char	bindType;
};
typedef map<DWORD, KEYBIND_DATA> KEYBINDINGMAP;
typedef map<DWORD, KEYBINDINGMAP > KEYBINDINGPAGELIST;

/// \class CInputManager
/// \brief Manager input
///
/// Handles input from the keyboard and mouse
class CInputManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CInputManager);

	/// Constructor
	/// \return void
	CInputManager();

public:
	/// Destructor for an Input Manager
	~CInputManager();

	/// Function to get this component.  This is a singleton
	/// in this case
	static IComponent *Create(int nArgs, va_list argptr);

	/// \fn virtual void Init()
	/// \brief Function call back when system is being started up
	virtual bool Init(void);

	/// \fn virtual void Update()
	/// \brief Function call back when system is being updated
	virtual void Update(DWORD tickCount);
	
	/// Sets a message to a specific key
	/// \param msgSize = size of keyStruct
	/// \param keyStruct = pointer to a KEYBIND structure
	/// \return DWORD = message return code, MSG_HANDLED_STOP,  
	///					MSG_HANDLED_PROCEED, error
	DWORD OnSetKeyBinding(DWORD msgSize, void *keyStruct);

	/// Gets a message for a specific key
	/// \param msgSize = size of keyStruct
	/// \param keyStruct = pointer to a KEYBIND structure
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnGetKeyBinding(DWORD msgSize, void *keyStruct);

	/// \brief	Load a keybinding file
	/// \param	msgSize = size of KEYBINDFILE strcut
	///	\param	*param = pointer to the KEYBINDFILE struct
	///	\return	DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	DWORD OnLoadKeyBinding(DWORD msgSize, void *param);

	///	\brief	Activate a keybinding
	///	\param	msgSize = size of DWORD 
	///	\param	*param	= pointer to DWORD (unique id of keybinding name)
	///	\return	DWORD = message return code, MSG_HANDLED_STOP,
	///					MSG_HANDLED_PROCEED, error
	DWORD OnSetActiveKeyBinding(DWORD msgSize, void *param);

	/// \brief Gets the currently loaded keybinding
	///	\param	msgSize = size of DWORD
	///	\param	*param	= pointer to DWORD (unique id of keybinding name)
	///	\return	DWORD = message return code, MSG_HANDLED_STOP,
	DWORD OnGetActiveKeyBinding(DWORD msgSize, void *param);
  
	/// Gets the current input modes
	/// \param msgSize = size of input engine data structure
	/// \param inputStruct = pointer to an INPUTENGPARAMS structure
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnGetCurrentInputMode(DWORD msgSize, void *inputStruct);

	/// Gets a message to BEGIN the input polling
	/// \param msgSize = size of input engine data structure
	/// \param inputStruct = pointer to an INPUTENGPARAMS structure
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnBegin(DWORD msgSize, void *inputStruct);

	/// Gets a message to END the input polling
	/// \param msgSize = size of message (in this
	///                  case there is not a message)
	/// \param nothing = pointer to nothing 
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnEnd(DWORD msgSize, void *nothing);

	/// Gets a message to START the input polling
	/// \param msgSize = size of input engine data structure
	/// \param inputStruct = pointer to an INPUTENGPARAMS structure
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnStart(DWORD msgSize, void *nothing);

	/// Gets a message to STOP the input polling
	/// \param msgSize = size of message (in this
	///                  case there is not a message)
	/// \param nothing = pointer to nothing 
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnStop(DWORD msgSize, void *nothing);

	/// Gets the current mouse position
	/// \param msgSize = size of message (in this
	///                  case there is not a message)
	/// \param mousePos = pointer to MOUSECOORD struct 
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnGetMousePosition(DWORD msgSize, void *mousePos);

	/// Sets the mouse/keyboard handler class pointer
	/// \param size = sizeof (IInputHandler *)
	/// \param param = IInputHandler *
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnSetInputHandler(DWORD size, void *param);

	/// unSets the mouse/keyboard handler class pointer
	/// \param size = 0
	/// \param param = NULL
	/// \return DWORD = message return code, MSG_HANDLED_STOP, 
	///					MSG_HANDLED_PROCEED, error
	DWORD OnUnsetInputHandler(DWORD size, void *param);

private:
	/// Loads key bindings from xml CArchive
	/// \return True for success, false for failure
	bool LoadKeyBindings();

	/// Get modifiers from the DWORD
	/// \param key - key that was pressed
	/// \return short - modifier code
	short GetModifiers(DWORD key);

	/// Handle the defined key bindings
	/// \return void
	void HandleKeyBindings();

	/// Handle Mouse Events
	/// \return void
	void HandleMouseEvents();

	/// Take care of all keys that are pressed
	/// \param dtime - time 
	/// \return char - returns the last key pressed
	char HandleKeyboard(float dtime = 0.25f);

	/// Tells wether a specific key is pressed
	/// \param key - key in question
	/// \return bool - true if yes, false if no
	bool IsKeyPressed(int key);

	/// Clear key arrays
	/// \return void
	void ClearAllKeyStates();

	/// Setup which keys can be hit only once
	void SetupKeyHitOnce();

	// keyboard

	//current active page
	DWORD	m_ActiveKeyBindPage;
	// map of current keybindings 
	KEYBINDINGPAGELIST  m_KeyMap;				
	// key pressed time array
	float  m_fPressedTime[MAX_NUM_KEYS];	
	// key pressed array
	bool  m_bPressed[MAX_NUM_KEYS];			

	// mouse 
	// current x position of mouse
	long m_lCurrentMouseX;
	// current y position of mouse
	long m_lCurrentMouseY;
	// last x position of mouse
	long m_lLastMouseX;					
	// last y position of mouse
	long m_lLastMouseY;	
	// is middle mouse pressed
	bool m_bMiddleMousePressed;
	// is left mouse pressed
	bool m_bLeftMousePressed;				
	// is right mouse pressed
	bool m_bRightMousePressed;				

	// Input Interface
	IInputHandler *m_pInputModule;

	// is the manager currently updating
	bool m_bIsRunning;

	// Current input modes
	INPUTENGPARAMS m_InputParams;

	DWORD m_fTimeDifference;
	DWORD m_fTime;
	DWORD m_fOldCount;
};

#endif	// CINPUTMANAGER_H



