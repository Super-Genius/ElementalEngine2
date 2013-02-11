///============================================================================
/// \file		CWin32JoystickHandler.h
/// \brief		Definition file for Joystick class
/// \date		03-26-2008
/// \author		Brendan Geiger
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
#ifndef CWin32JoystickHandler_H__
#define CWin32JoystickHandler_H__

#ifdef WIN32

#include "CJoystickHandler.h"
#include "CWin32Joystick.h"
#include <dinput.h>

class CWin32JoystickHandler;
struct ENUMJOYSTICKOBJECTSCALLBACKPARAMS
{
	LPDIRECTINPUTDEVICE8 pDevice;
	int iAxisResolution;
};
struct ENUMJOYSTICKSCALLBACKPARAMS
{
	CWin32JoystickHandler *pHandler;
	LPDIRECTINPUT8 pDI;
	std::vector<IJoystick*> *pJoysticks;
	int iAxisResolution;
};

BOOL CALLBACK    EnumJoystickObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
BOOL CALLBACK    EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );

class CWin32JoystickHandler : public CJoystickHandler
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CWin32JoystickHandler );
	CWin32JoystickHandler();
public:
	// I think this needs to be virtual to call the base class
	virtual ~CWin32JoystickHandler();

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

	bool Init();
	bool DeInit();
	void ResetAxisCalibration();

private:
	void InitDirectInput();
	/// Enumerate all the joysticks attached to the system and 
	void EnumJoysticks();

	LPDIRECTINPUT8 m_pDI;
};

#endif//WIN32

#endif