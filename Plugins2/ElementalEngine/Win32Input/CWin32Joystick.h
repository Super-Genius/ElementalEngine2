///============================================================================
/// \file		CWin32Joystick.h
/// \brief		Definition file for Win32Joystick class
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

#ifndef CWIN32JOYSTICK_H__
#define CWIN32JOYSTICK_H__

#include "CJoystick.h"
#include <dinput.h>

/// \class CWin32Joystick
/// \brief Class to represent a Direct Input Joystick
class CWin32Joystick : public CJoystick
{
public:
	CWin32Joystick( IJoystickHandler *joystickHandler, int iJoystickId );
	void UpdateInputStates( float fTime );

	LPDIRECTINPUTDEVICE8 m_pDevice;
};

#endif //#ifndef CWIN32JOYSTICK_H__