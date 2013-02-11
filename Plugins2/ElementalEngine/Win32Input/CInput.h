///============================================================================
/// \file		CInput.h
/// \brief		Header file for Input class
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

#ifndef CINPUT_H
#define CINPUT_H

#include "DXVersion.h"
#include <dinput.h>
#include "IInputHandler.h"

// retrieval mode for direct input
enum RMODE { DI_BUFFERED, DI_IMMEDIATE };

#include "CKeyboard.h"
#include "CMouse.h"

/// \class CInput
/// \brief Input class
///
/// Wrapper for Direct Input.
/// Responsible for creating/updating/destroying keyboard and mouse devices
class CInput : public IInputHandler
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CInput );
	CInput();
public:
	/// Destructor
	virtual ~CInput();

	/// Create Function
	static IComponent* Create(int nArgs, va_list argptr);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// Returns a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	/// default init function
	void Init();

	/// Init mouse and keyboard
	/// \param inputParams - pointer to mouse/keyboard paraamters structure (INPUTENGPARAMS)
	/// \return HRESULT - result of the initialization
	HRESULT Init(INPUTENGPARAMS *inputParams);

	/// Update mouse and keyboard
	/// \return void
	void Update();

	/// De-Initialize the mouse and keyboard
	/// \return true on success, false on failure
	bool DeInit();

	// Keyboard

	/// Set focus for the keyboard
	/// \return void
	void KBSetFocus();

	/// Kill focus for the keyboard
	/// \return void
	void KBKillFocus();

	/// Is any key pressed?
	/// \return true if there is, false if not
	bool KBIsAnyKeyPressed();

	/// Is any key released?
	/// \return true if there is, false if not
	bool KBIsAnyKeyReleased();

	/// Is a specific key pressed?
	/// \param key - key to check
	/// \return true if it is, false if not
	bool KBIsKeyPressed(int key);

	/// Is a specific key released?
	/// \param key - key to check
	/// \return true if it is, false if not
	bool KBIsKeyReleased(int key);

	/// Set the key to be a single hit
	/// \param  key - key
	///	\param	bSingleHit - if true, hit only once
	///	\param	bSingleRelease - if true, release only once
	void KBSetSingleHit(int key, bool bSingleHit, bool bSingleRelease);

	/// Temp function to test keyboard buffered mode
	/// \return TCHAR* - pointer to keyboard buffer
	TCHAR* KBGetBuffer();

	/// Get the key table
	/// \return SEEKEY - Elemental Engine Key Table
	const SEEKey *GetKeyTable();

	// Mouse

	/// get window rect bounds for mouse movement
	/// \param width = reference to long to fill in width of window
	/// \param height = reference to long to fill in height of window
	void GetWindowSize(long &width, long &height);

	/// Set mouse focus to a window
	/// \param hWnd - window to focus on
	/// \return void
	void MouseSetFocus(HWND &hWnd);

	/// Kill mouse focus
	/// \return void
	void MouseKillFocus();

	/// Is Left Mouse Button Pressed?
	/// \return true if it is, false if not
	bool IsLeftButtonPressed() const;

	/// Is Middle Mouse Button Pressed?
	/// \return true if it is, false if not
	bool IsMiddleButtonPressed() const;

	/// Is Right Mouse Button Pressed?
	/// \return true if it is, false if not
	bool IsRightButtonPressed() const;

	/// Get Mouse's X Position
	/// \return long - x position of the mouse
	long GetX() const;

	/// Get Mouse's Y Position
	/// \return long - y position of the mouse
	long GetY() const;

	/// Get Mouse's Z (wheel) position
	/// \return long - z position of the mouse
	long GetZ() const;

	/// Get Delta X of Mouse
	/// \return float - delta x of the mouse
	float GetDeltaX() const;

	/// Get Delta Y of Mouse
	/// \return float - delta y of the mouse
	float GetDeltaY() const;

	/// Get Change in Mouse Wheel
	/// \return long - change
	long GetWheelDelta() const;


	/// Is direct input initialized?
	/// \return true if it is, false if not
	bool IsInitialized();

	/// Set Acquire based on boolean for devices
	/// \param acquire = true if acquire, false to unacquire
	void SetAcquire(bool acquire);

private:
	// keyboard
	CKeyboard m_Keyboard;
	// mouse
	CMouse m_Mouse;

	// Windows handle
	HWND m_hWnd;

	// direct input interface
	LPDIRECTINPUT8 m_pDI;
	// direct input device instance
	LPDIDEVICEINSTANCE m_pDeviceInstance;

	// key table
	SEEKey m_Keys[256];

	// current state of the mouse
	SEEMouseData m_CurrentMouseStatus;
	// previous state of the mouse
	SEEMouseData m_OldMouseStatus;

	// is input initialized?
	bool m_bInputInitialized;
};
#endif