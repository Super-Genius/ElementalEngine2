///============================================================================
/// \file		IInputHandler.h
/// \brief		Header file for Input interfae
/// \date		01-24-2009
/// \author		Kenneth Hurley
/// \note   Elemental Engine
///         Copyright (c)  2005-2009 Signature Devices, Inc.
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

#ifndef IINPUTHANDLER_H
#define IINPUTHANDLER_H

namespace ElementalEngine
{


// Elemental Engine Key struct
struct SEEKey
{
  int EEKey;				// int representation of the key
  char  m_chLower;			// lower case character
  char  m_chUpper;			// upper case character
  const char* m_pszName;	// character representation of the key
};

/// \class CInput
/// \brief Input class
/// Responsible for creating/updating/destroying keyboard and mouse devices
class IInputHandler : public IComponent
{
public:
	/// Destructor
	virtual ~IInputHandler() {};

	/// Init mouse and keyboard
	/// \param inputParams - Mouse/Keyboard settings
	/// \return HRESULT - result of the initialization
	virtual HRESULT Init(INPUTENGPARAMS *inputParams) = 0;

	/// Update mouse and keyboard
	/// \return void
	virtual void Update() = 0;

	/// De-Initialize the mouse and keyboard
	/// \return true on success, false on failure
	virtual bool DeInit() = 0;

	// Keyboard

	/// Set focus for the keyboard
	/// \return void
	virtual void KBSetFocus() = 0;

	/// Kill focus for the keyboard
	/// \return void
	virtual void KBKillFocus() = 0;

	/// Is any key pressed?
	/// \return true if there is, false if not
	virtual bool KBIsAnyKeyPressed() = 0;

	/// Is any key released?
	/// \return true if there is, false if not
	virtual bool KBIsAnyKeyReleased() = 0;

	/// Is a specific key pressed?
	/// \param key - key to check
	/// \return true if it is, false if not
	virtual bool KBIsKeyPressed(int key) = 0;

	/// Is a specific key released?
	/// \param key - key to check
	/// \return true if it is, false if not
	virtual bool KBIsKeyReleased(int key) = 0;

	/// Set the key to be a single hit
	/// \param  key - key
	///	\param	bSingleHit - if true, hit only once
	///	\param	bSingleRelease - if true, release only once
	virtual void KBSetSingleHit(int key, bool bSingleHit, bool bSingleRelease) = 0;

	/// Temp function to test keyboard buffered mode
	/// \return TCHAR* - pointer to keyboard buffer
	virtual TCHAR* KBGetBuffer() = 0;

	/// Get the key table
	/// \return SEEKEY - Elemental Engine Key Table
	virtual const SEEKey *GetKeyTable() = 0;

	// Mouse

	/// get window rect bounds for mouse movement
	/// \param width = reference to long to fill in width of window
	/// \param height = reference to long to fill in height of window
	virtual void GetWindowSize(long &width, long &height) = 0;

	/// Set mouse focus to a window
	/// \param hWnd - window to focus on
	/// \return void
	virtual void MouseSetFocus(HWND &hWnd) = 0;

	/// Kill mouse focus
	/// \return void
	virtual void MouseKillFocus() = 0;

	/// Is Left Mouse Button Pressed?
	/// \return true if it is, false if not
	virtual bool IsLeftButtonPressed() const = 0;

	/// Is Middle Mouse Button Pressed?
	/// \return true if it is, false if not
	virtual bool IsMiddleButtonPressed() const = 0;

	/// Is Right Mouse Button Pressed?
	/// \return true if it is, false if not
	virtual bool IsRightButtonPressed() const = 0;

	/// Get Mouse's X Position
	/// \return long - x position of the mouse
	virtual long GetX() const = 0;

	/// Get Mouse's Y Position
	/// \return long - y position of the mouse
	virtual long GetY() const = 0;

	/// Get Mouse's Z (wheel) position
	/// \return long - z position of the mouse
	virtual long GetZ() const = 0;

	/// Get Delta X of Mouse
	/// \return float - delta x of the mouse
	virtual float GetDeltaX() const = 0;

	/// Get Delta Y of Mouse
	/// \return float - delta y of the mouse
	virtual float GetDeltaY() const = 0;

	/// Get Change in Mouse Wheel
	/// \return long - change
	virtual long GetWheelDelta() const = 0;


	/// Is direct input initialized?
	/// \return true if it is, false if not
	virtual bool IsInitialized() = 0;

	/// Set Acquire based on boolean for devices
	/// \param acquire = true if acquire, false to unacquire
	virtual void SetAcquire(bool acquire) = 0;
};

}; // namespace ElementalEngine

#endif