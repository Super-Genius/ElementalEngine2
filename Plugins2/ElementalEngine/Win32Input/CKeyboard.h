///============================================================================
/// \file		CKeyboard.h
/// \brief		Header file for Keyboard Class
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

#ifndef CKEYBOARD_H
#define CKEYBOARD_H

#define MAX_INPUT_LENGTH 2048
#define BLINKING_INTERVAL 500
#define KEYBUFFER_SIZE 256
#include <dinput.h>

typedef signed long int SLONG;          //32 bit signed.

#define KEY_STATE_PRESSED			(1<<0)
#define KEY_STATE_LOCKED			(1<<1)
#define KEY_STATE_HIT_ONCE			(1<<2)
#define KEY_STATE_RELEASE_ONCE		(1<<3)
#define KEY_STATE_RELEASE_LOCKED	(1<<4)
#define KEY_STATE_RELEASED			(1<<5)


/// \class CKeyboard
/// \brief Class to represent a Direct Input Keyboard
class CKeyboard
{
public:
	/// Constructor
	CKeyboard();

	/// Destructor
	~CKeyboard();

	/// Initialize the Keyboard
	/// \param pDI - pointer to Direct Input Interface
	/// \param hWnd - handle of window
	/// \param keyboardFlags - flags for direct input create
	/// \param kbImmediate - Immediate data is a snapshot of the current
    ///                      state of a device.
	/// \param kbExclusive - Exclusive mode prevents other applications from 
    ///                      also acquiring the device exclusively.
	/// \param kbFG - Foreground cooperative level means that the
    ///               application has access to data only when in the
    ///               foreground or, in other words, has the input focus.
	/// \return HRESULT - result of initialization
	HRESULT Init(LPDIRECTINPUT8 &pDI, 
				 HWND &hWnd,
				 DWORD keyboardFlags,
				 bool kbImmediate,
				 bool kbExclusive,
				 bool kbFG);

	/// Update the Keyboard
	/// \return void
	void Update();

	/// DeInit
	/// \return true for success, false on failure
	HRESULT DeInit();


	/// Is any key pressed
	/// \return true if a key is pressed false if not
	bool IsAnyKeyPressed();

	/// Is any key released
	/// \return true if a key is released false if not
	bool IsAnyKeyReleased();

	/// Is a specific key pressed?
	/// \param key - key to check
	/// \return true if specified key is down false if not
	bool IsKeyPressed( int key );

	/// Is a specific key released?
	/// \param key - key to check
	/// \return true if specified key is up false if not
	bool IsKeyReleased( int key );


	/// Initialize Direct Input Key Table
	/// \return true on success, false on failure
	bool InitDIKeyTable();

	/// Initialize Virtual Key Table
	/// \return true on success, false on failure
	bool InitVirtualKeyTable();

	/// Testing buffered input!
	/// \return buffer of input
	TCHAR* GetBuffer() { return strNewText; }

	/// Acquire the keyboard input
	/// \return void
	void Acquire();

	/// Unacquire the keyboard input
	/// \return void
	void Unacquire();

	/// Kill Keyboard Focus
	/// \return void
	void KillFocus();

	/// Find out if the Keyboard is Initialized
	/// \return true if it is, false if not
	bool IsInitialized() { return m_bIsKeyboardInitialized; }

	///	Set a key as single hit
	/// \param dxkey - key value
	///	\param bSingleHit - true if single hit
	///	\param bSingleRelease - true if single release
	void SetSingleHit(int key, bool bSingleHit, bool bSingleRelease);

private:
	/// Setup the Keyboard
	/// \param pDI - pointer to Direct Input Interface
	/// \param hWnd - handle of window
	/// \param keyboardFlags - flags for direct input create
	/// \param kbImmediate - Immediate data is a snapshot of the current
    ///                      state of a device.
	/// \param kbExclusive - Exclusive mode prevents other applications from 
    ///                      also acquiring the device exclusively.
	/// \param kbFG - Foreground cooperative level means that the
    ///               application has access to data only when in the
    ///               foreground or, in other words, has the input focus.
	/// \return result of the operation
	HRESULT SetupKeyboard(LPDIRECTINPUT8 &pDI, 
						  HWND &hWnd,
						  DWORD keyboardFlags,
						  bool kbImmediate,
					      bool kbExclusive,
					      bool kbFG);

	/// Get the keyboard state immediate
	/// \return true if no errors, false if error occured
	HRESULT GetKeyboardStateImmediate();

	/// Get the keyboard state buffered
	/// \return true if no errors, false if error occured
	HRESULT GetKeyboardStateBuffered();

	/// Check specified table for any key pressed
	/// \param table = pointer to table to check
	/// \return true if a key is pressed false if not
	bool CheckTableAnyKeyPressed(char *table);

	/// Check specified table for a specific key press
	/// \param key - key to check
	/// \param table - pointer to table to check
	/// \return true if a the key is pressed false if not
	bool CheckTableSpecificKeyPressed(int key, char *table);

	/// Check specified table for any key pressed
	/// \param table = pointer to table to check
	/// \return true if a key is pressed false if not
	bool CheckTableAnyKeyReleased(char *table);

	/// Check specified table for a specific key press
	/// \param key - key to check
	/// \param table - pointer to table to check
	/// \return true if a the key is pressed false if not
	bool CheckTableSpecificKeyReleased(int key, char *table);

	/// Mark a key as down
	/// \param key - key that is down
	/// \param table - pointer to key table
	/// \return void
	void KeyDown(int key, char *table);

	/// Mark a key as up
	/// \param key - key that is up
	/// \param table - point to the key table
	/// \return void
	void KeyUp(int key, char *table);

	/// Set the key to be hit only once
	///	\param key - the key
	///	\param bHitOnce - if true, hit only once
	///	\param *table - key table
	void SetKeyOnce(int key, bool bHitOnce, bool bReleaseOnce, char* table);

private:
	// handle to window
	HWND m_hWnd;

	//Direct Input Constructs
	LPDIRECTINPUTDEVICE8 m_pKBDevice;		

	// key buffers
	char m_cKeyBuffer[KEYBUFFER_SIZE];
	char m_cKeyBufferOld[KEYBUFFER_SIZE];

	// status for all keys
	char  m_cKeyStatus[KEYBUFFER_SIZE]; 
	// scancode version status for all VGL keys
	char  m_cKeyStatusSC[KEYBUFFER_SIZE]; 

	// buffer to store if key is pressed
	bool m_bKeyPressedBuffer[KEYBUFFER_SIZE];

	// the input code is the index of each table, and the corresponding
	// Elemental Engine Key Code is the value for each index
	SLONG m_DirectInputKeys[KEYBUFFER_SIZE]; 

	// table to map windows virtual keys to EE virtual keys
	int m_iVKTable[KEYBUFFER_SIZE];		
	// table to map extended windows virtual keys to EE virtual keys
	int m_iVKTableExt[KEYBUFFER_SIZE];		

	// flag for knowing whether EE is using DirectInput for keyboard
	bool m_bUsingDIKeyboard; 
	// fFlag for knowing whether the keyboard is initialized
	bool m_bIsKeyboardInitialized;

	// buffered or immediate?
	RMODE m_RetrievalMode;

	// temp variables to test buffering!
	TCHAR              strNewText[256*5 + 1];
    TCHAR              strLetter[10];  
};
#endif