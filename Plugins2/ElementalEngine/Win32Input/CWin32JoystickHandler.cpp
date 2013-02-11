///============================================================================
/// \file		CWin32JoystickHandler.cpp
/// \brief		Implementation file for Joystick class
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

#include "stdafx.h"
#include "CWin32JoystickHandler.h"
#include "CWin32Joystick.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CWin32JoystickHandler);

BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext )
{
    HRESULT hr;

	ENUMJOYSTICKSCALLBACKPARAMS *pParams = (ENUMJOYSTICKSCALLBACKPARAMS*)pContext;

	CWin32Joystick *desc = new CWin32Joystick( pParams->pHandler, (int)pParams->pJoysticks->size() );

    // Obtain an interface to the enumerated Joystick.
	hr = pParams->pDI->CreateDevice( pdidInstance->guidInstance, &desc->m_pDevice, NULL );
    // If it failed, then we can't use this Joystick. (Maybe the user unplugged
    // it while we were in the middle of enumerating it.)
    if( FAILED(hr) ) 
        return DIENUM_CONTINUE;

	// Set the data format to "simple Joystick" - a predefined data format 
	//
	// A data format specifies which controls on a device we are interested in,
	// and how they should be reported. This tells DInput that we will be
	// passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
	if( FAILED( hr = desc->m_pDevice->SetDataFormat( &c_dfDIJoystick2 ) ) )
        return DIENUM_CONTINUE;

	// Enumerate the Joystick objects. The callback function enabled user
	// interface elements for objects that are found, and sets the min/max
	// values property for discovered axes.
	ENUMJOYSTICKOBJECTSCALLBACKPARAMS enumObjectsParams;
	enumObjectsParams.pDevice = desc->m_pDevice;
	enumObjectsParams.iAxisResolution = pParams->iAxisResolution;

	if( FAILED( hr = desc->m_pDevice->EnumObjects( EnumJoystickObjectsCallback, &enumObjectsParams, DIDFT_ALL ) ) )
        return DIENUM_CONTINUE;

	// push new joystick descriptor onto list
	pParams->pJoysticks->push_back( (IJoystick*)desc );

    return DIENUM_CONTINUE;
}

BOOL CALLBACK EnumJoystickObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext )
{
	ENUMJOYSTICKOBJECTSCALLBACKPARAMS *pParams = (ENUMJOYSTICKOBJECTSCALLBACKPARAMS*)pContext;
    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( pdidoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg; 
        diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        diprg.diph.dwHow        = DIPH_BYID; 
        diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin              = 0;
		diprg.lMax              = pParams->iAxisResolution;

        // Set the range for the axis
		if( FAILED(pParams->pDevice->SetProperty( DIPROP_RANGE, &diprg.diph )) )
            return DIENUM_STOP;
    }

    return DIENUM_CONTINUE;
}


CWin32JoystickHandler::CWin32JoystickHandler() : CJoystickHandler()
{
	m_pDI = NULL;

	Init();

	IToolBox *toolBox = EngineGetToolBox();

	static DWORD msgHash_SetJoystickHandler = CHashString(_T("SetJoystickHandler")).GetUniqueID();
	toolBox->SendMessage( msgHash_SetJoystickHandler, sizeof(IJoystickHandler *), this);
}

CWin32JoystickHandler::~CWin32JoystickHandler()
{
	IToolBox *toolBox = EngineGetToolBox();
	static DWORD msgHash_UnsetJoystickHandler = CHashString(_T("UnsetJoystickHandler")).GetUniqueID();
	toolBox->SendMessage( msgHash_UnsetJoystickHandler, 0, NULL);
}

IComponent *CWin32JoystickHandler::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CWin32JoystickHandler);
}

void CWin32JoystickHandler::Serialize(IArchive &ar)
{
}

IHashString *CWin32JoystickHandler::GetComponentType()
{
	static CHashString hszComponentType = _T("CWin32JoystickHandler");
	return &hszComponentType; 
}

bool CWin32JoystickHandler::IsKindOf(IHashString *compType)
{
	static CHashString hszComponentType = _T("CWin32JoystickHandler");
	return (hszComponentType.GetUniqueID() == compType->GetUniqueID());
}

bool CWin32JoystickHandler::Init()
{
	InitDirectInput();
	EnumJoysticks();

	CJoystickHandler::Init();

	return true;
}

bool CWin32JoystickHandler::DeInit()
{
	CJoystickHandler::DeInit();

	std::vector<IJoystick*>::iterator itrJoystick = m_Joysticks.begin();
	while (itrJoystick != m_Joysticks.end())
	{
		CWin32Joystick *joystick = (CWin32Joystick*)(*itrJoystick);
		LPDIRECTINPUTDEVICE8 &joystickDevice = joystick->m_pDevice;

		if( joystickDevice )
		{
			joystickDevice->Unacquire();
		}
		SAFE_RELEASE( joystickDevice );
		SAFE_DELETE( joystick );

		itrJoystick++;
	}
	m_Joysticks.clear();
	SAFE_RELEASE( m_pDI );

	return true;
}

void CWin32JoystickHandler::ResetAxisCalibration()
{
	for( std::vector<IJoystick*>::iterator itr = m_Joysticks.begin(); itr != m_Joysticks.end(); itr++ )
	{
		IJoystick *joystick = *itr;
		joystick->ResetAxisCalibration( EEJ_AXISX, 0, m_iAxisResolution );
		joystick->ResetAxisCalibration( EEJ_AXISY, m_iAxisResolution, 0 );
		joystick->ResetAxisCalibration( EEJ_AXISZ, 0, m_iAxisResolution );
		joystick->ResetAxisCalibration( EEJ_AXISRX, 0, m_iAxisResolution );
		joystick->ResetAxisCalibration( EEJ_AXISRY, m_iAxisResolution, 0 );
		joystick->ResetAxisCalibration( EEJ_AXISRZ, 0, m_iAxisResolution );
	}
}

void CWin32JoystickHandler::InitDirectInput()
{
	DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL);
}

void CWin32JoystickHandler::EnumJoysticks()
{
	if( m_pDI )
	{
		ENUMJOYSTICKSCALLBACKPARAMS params;
		params.pHandler = this;
		params.pDI = m_pDI;
		params.pJoysticks = &m_Joysticks;
		params.iAxisResolution = m_iAxisResolution;

		m_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, &params, DIEDFL_ATTACHEDONLY );
	}
}