///============================================================================
/// \file		CMouse.h
/// \brief		Header file for Mouse class
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

#ifndef CMOUSE_H
#define CMOUSE_H

#include <dinput.h>

struct SEEMouseData
{
  long posX;           // X position of the cursor
  long posY;           // Y position of the cursor
  long posZ;		   // Z position of the cursor (wheel data)
  long deltaX;         // Difference in X position since last call
  long deltaY;         // Difference in Y position since last call
  bool buttonL;        // TRUE if left mouse button is pressed
  bool buttonM;        // TRUE if middle mouse button is pressed
  bool buttonR;        // TRUE if right mouse button is pressed
  long wheelUp;        // Mouse wheel up movement      (notice that both up and down
  long wheelDown;      // Mouse wheel down movement     movement can happen at once)
  long buttonX0;       // TRUE if first extra button is pressed
  long buttonX1;       // TRUE if second extra button is pressed
  long reserved;       // Reserved, might be used in the future
};

/// \class CMouse
/// \brief Class to represent a Direct Input Mouse
class CMouse
{
public:
	/// Constructor
	CMouse();

	/// Destructor
	~CMouse();

	/// Initialize the Mouse
	/// \param pDI - pointer to Direct Input Interface
	/// \param hWnd - handle of window
	/// \return HRESULT - result of initialization
	HRESULT Init(LPDIRECTINPUT8 &pDI, 
				 HWND &hWnd, 
				 DWORD mouseFlags,
				 bool mouseImmediate, 
				 bool mouseExclusive, 
				 bool mouseFG);

	/// Update the Mouse
	/// \return void
	void Update();

	/// De-Initialize the Mouse Device
	/// \return HRESULT - result of de-initialization
	HRESULT DeInit();

	/// Get Mouse Status
	/// \param mouseState - structure to hold mouse data
	/// \return void
	void GetMouseStatus(SEEMouseData *mouseState);

	/// Enable the mouse
	/// \return void
	void EnableMouse();

	/// Disable the Mouse
	/// \return void
	void DisableMouse();

	/// Is one of the mouse buttons pressed?
	/// \return true or false
	bool IsMouseButtonPressed();

	/// Set Mouse Focus
	/// \return void
	void SetMouseFocus(LPDIRECTINPUT8 &pDI, HWND &hWnd);

	/// Kill Mouse Focus
	/// \return void
	void KillMouseFocus();

	/// Setup the Mouse
	/// \param pDI -
	/// \param hWnd -
	/// \return result of the setup
	HRESULT SetupDIMouse(LPDIRECTINPUT8 &pDI, 
						 HWND &hWnd,
						 DWORD mouseFlags,
						 bool mouseImmediate, 
						 bool mouseExclusive, 
						 bool mouseFG);

	/// Acquire the mouse input
	/// \return void
	void Acquire();

	/// Unacquire the mouse input
	/// \return void
	void Unacquire();

	/// Find out if Mouse is initialized
	/// \return true if it is, false if not
	bool IsInitialized() { return m_bIsMouseInitialized; }

private:
	/// Get the mouse state from direct input
	/// \return true if no errors, false if error occured
	HRESULT GetMouseStateImmediate();

	/// Get the mouse state from direct input
	/// \return true if no errors, false if error occured
	HRESULT GetMouseStateBuffered();

	/// Release the mouse device
	/// \return HRESULT - result of the release
	HRESULT ReleaseMouse();

private:

	// handle to window
	HWND m_hWnd;

	//Direct Input Constructs
	LPDIRECTINPUTDEVICE8 m_pMouseDev;	

	// mouse data
	bool m_lMouseL, m_lMouseM, m_lMouseR;
	long m_lMouseWheelUp, m_lMouseWheelDown;
	long m_lMouseX0, m_lMouseX1;
	long m_lMouseDX, m_lMouseDY;
	long m_lMouseX, m_lMouseY, m_lMouseZ;
	long m_lMouseAX, m_lMouseAY;
	long m_lWindowWidth, m_lWindowHeight;
	long m_lMouseCapture;
	long m_lFullControl;

	// flag for knowing whether EE is using DirectInput for the mouse
	bool m_bUsingDIMouse; 
	// flag for knowing whether the mouse is initialized
	bool m_bIsMouseInitialized;
	bool m_bMouseCapture;

	// buffered or immediate?
	RMODE m_RetrievalMode;
};

#endif