///============================================================================
/// \file		CJoystickManager.cpp
/// \brief		Implementation file for Joystick class
/// \date		12-15-2006
/// \author		Brian Bazyk
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
#include "JoystickManager.h"

REGISTER_COMPONENT_SINGLETON( CJoystickManager );
REGISTER_MESSAGE_HANDLER(SetJoystickHandler, OnSetJoystickHandler, CJoystickManager);
REGISTER_MESSAGE_HANDLER(UnsetJoystickHandler, OnUnsetJoystickHandler, CJoystickManager);
REGISTER_MESSAGE_HANDLER(SetJoystickKeyBinding, OnSetJoystickKeyBinding, CJoystickManager);
//REGISTER_MESSAGE_HANDLER(GetKeyBinding, OnGetKeyBinding, CJoystickManager);
REGISTER_MESSAGE_HANDLER(Start, OnStart, CJoystickManager);
REGISTER_MESSAGE_HANDLER(Stop, OnStop, CJoystickManager);
REGISTER_MESSAGE_HANDLER(SetActiveJoystickKeyBinding, OnSetActiveJoystickKeyBinding, CJoystickManager);
//REGISTER_MESSAGE_HANDLER(GetActiveKeyBinding, OnGetActiveKeyBinding, CJoystickManager);
REGISTER_MESSAGE_HANDLER(SwapJoystickDeviceMapping, OnSwapJoystickDeviceMapping, CJoystickManager);
REGISTER_MESSAGE_HANDLER(EnableJoystickFaultDetection, OnEnableJoystickFaultDetection, CJoystickManager);
REGISTER_MESSAGE_HANDLER(QueryJoystickFault, OnQueryJoystickFault, CJoystickManager);
REGISTER_MESSAGE_HANDLER(GetJoystickDeviceMapping, OnGetJoystickDeviceMapping, CJoystickManager);
REGISTER_MESSAGE_HANDLER(SetJoystickDeviceMapping, OnSetJoystickDeviceMapping, CJoystickManager);
REGISTER_MESSAGE_HANDLER(GetJoystickCount, OnGetJoystickCount, CJoystickManager);
REGISTER_MESSAGE_HANDLER(RefreshJoysticks, OnRefreshJoysticks, CJoystickManager);
REGISTER_MESSAGE_HANDLER(SetAutoDetectJoystickPlayer, OnSetAutoDetectJoystickPlayer, CJoystickManager);
REGISTER_MESSAGE_HANDLER(SetMotorSpeed, OnSetMotorSpeed, CJoystickManager);

/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CJoystickManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CJoystickManager );
}

/// \brief Default Constructor
CJoystickManager::CJoystickManager() : 
	CManager(_T("CJoystickManager"), INPUTPRIORITY)
{
	m_bIsRunning = true;
	m_ToolBox = EngineGetToolBox();
	m_Timer = m_ToolBox->GetTimer();
	m_pJoystickHandler = NULL;
}

CJoystickManager::~CJoystickManager()
{
	if( m_pJoystickHandler )
	{
		m_pJoystickHandler->DeInit();
	}
}

bool CJoystickManager::Init()
{
	if( m_pJoystickHandler )
	{
		m_pJoystickHandler->Init();
	}
	return CManager::Init();
}

bool CJoystickManager::DeInit()
{
	if( m_pJoystickHandler )
	{
		m_pJoystickHandler->DeInit();
	}
	return CManager::DeInit();
}

void CJoystickManager::Update( DWORD tickCount )
{
	if( m_bIsRunning )
	{
		if( m_pJoystickHandler )
		{
			m_pJoystickHandler->Update( tickCount );
		}
	}

}
DWORD CJoystickManager::OnStart(DWORD size, void *param)
{
	m_bIsRunning = true;
	return MSG_HANDLED_PROCEED;
}

DWORD CJoystickManager::OnStop(DWORD size, void *param)
{
	m_bIsRunning = false;
	return MSG_HANDLED_PROCEED;
}

DWORD CJoystickManager::OnSetJoystickHandler(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(IJoystickHandler*) );
	m_pJoystickHandler = (IJoystickHandler*)param;
	return MSG_HANDLED_STOP;
}

DWORD CJoystickManager::OnUnsetJoystickHandler(DWORD size, void *param)
{
	m_pJoystickHandler = NULL;
	return MSG_HANDLED_STOP;
}

DWORD CJoystickManager::OnSetJoystickKeyBinding(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(JOYSTICKKEYBINDPARAMS));
	if( m_pJoystickHandler )
	{
		JOYSTICKKEYBINDPARAMS *pKeyBind = (JOYSTICKKEYBINDPARAMS*)param;

		JOYSTICKKEYBIND keybind;
		keybind.MessageID = pKeyBind->m_MessageID;
		keybind.iEventFilter = pKeyBind->m_EventFilter;
		keybind.bRepeat = pKeyBind->m_bRepeat;
		m_pJoystickHandler->SetKeybinding( pKeyBind->m_kbNameID, pKeyBind->m_DeviceID, pKeyBind->m_KeyID, &keybind );
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CJoystickManager::OnSetActiveJoystickKeyBinding(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DWORD));
	if( m_pJoystickHandler )
	{
		DWORD nameID = *(DWORD*)param;

		m_pJoystickHandler->SetActiveKeyBinding( nameID );
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CJoystickManager::OnSwapJoystickDeviceMapping(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(JOYSTICKDEVICEMAPPARAMS));
	if( m_pJoystickHandler )
	{
		JOYSTICKDEVICEMAPPARAMS *pDeviceMap = (JOYSTICKDEVICEMAPPARAMS*)param;
		m_pJoystickHandler->SwapDeviceMapping( pDeviceMap->iOldDeviceID, pDeviceMap->iNewDeviceID );
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CJoystickManager::OnEnableJoystickFaultDetection(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(JOYSTICKFAULTDETECTIONPARAMS));
	if( m_pJoystickHandler )
	{
		JOYSTICKFAULTDETECTIONPARAMS *pParams = (JOYSTICKFAULTDETECTIONPARAMS*)param;
		m_pJoystickHandler->EnableFaultDetection( pParams->iJoystick, pParams->iInput, pParams->bEnable );
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CJoystickManager::OnQueryJoystickFault(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(JOYSTICKFAULTQUERYPARAMS));
	if( m_pJoystickHandler )
	{
		JOYSTICKFAULTQUERYPARAMS *pParams = (JOYSTICKFAULTQUERYPARAMS*)param;
		pParams->out_bFault = m_pJoystickHandler->HasFault( pParams->iJoystick, pParams->iInput );
	}

	return MSG_HANDLED_STOP;
}

DWORD CJoystickManager::OnSetJoystickDeviceMapping(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(JOYSTICKDEVICEMAPPARAMS));
	DWORD retVal = MSG_ERROR;
	if( m_pJoystickHandler )
	{
		JOYSTICKDEVICEMAPPARAMS *pDeviceMap = (JOYSTICKDEVICEMAPPARAMS*)param;
		bool bResult = m_pJoystickHandler->SetDeviceMapping( pDeviceMap->iOldDeviceID, pDeviceMap->iNewDeviceID );
		if( bResult )
		{
			retVal = MSG_HANDLED_PROCEED;
		}
	}

	return retVal;
}
DWORD CJoystickManager::OnGetJoystickDeviceMapping(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(JOYSTICKDEVICEMAPPARAMS));
	if( m_pJoystickHandler )
	{
		JOYSTICKDEVICEMAPPARAMS *pDeviceMap = (JOYSTICKDEVICEMAPPARAMS*)param;
		pDeviceMap->iNewDeviceID = m_pJoystickHandler->GetDeviceMapping( pDeviceMap->iOldDeviceID );
	}

	return MSG_HANDLED_STOP;
}

DWORD CJoystickManager::OnGetJoystickCount(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DWORD));
	if( m_pJoystickHandler )
	{
		DWORD *outCount = (DWORD*)param;
		*outCount = m_pJoystickHandler->GetCount();
	}

	return MSG_HANDLED_STOP;
}

DWORD CJoystickManager::OnRefreshJoysticks(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, 0);
	if( m_pJoystickHandler )
	{
		m_pJoystickHandler->Refresh();
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CJoystickManager::OnSetAutoDetectJoystickPlayer(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	if( m_pJoystickHandler )
	{
		m_pJoystickHandler->SetAutoDetectPlayer( *(bool*)param );
	}

	return MSG_HANDLED_STOP;
}

DWORD CJoystickManager::OnSetMotorSpeed(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( JOYSTICKMOTORSPEEDPARAMS ) );
	if( m_pJoystickHandler )
	{
		JOYSTICKMOTORSPEEDPARAMS *motorParams = (JOYSTICKMOTORSPEEDPARAMS*)param;
		m_pJoystickHandler->SetMotorSpeed( motorParams->iJoystick, motorParams->cMotor, motorParams->fSpeed );
	}
	return MSG_HANDLED_STOP;
}