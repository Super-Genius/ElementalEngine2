///============================================================================
/// \file	CPerfTimer.cpp
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
#include "CPerfTimer.h"


REGISTER_COMPONENT_SINGLETON( CPerfTimeManager );

REGISTER_MESSAGE_HANDLER(GetRealTime,	OnGetRealTime,	CPerfTimeManager );
REGISTER_MESSAGE_HANDLER(GetTime,		OnGetTime,		CPerfTimeManager );
REGISTER_MESSAGE_HANDLER(GetTimeDelta,	OnGetTimeDelta, CPerfTimeManager );
REGISTER_MESSAGE_HANDLER(UpdateTime,	OnUpdateTime,	CPerfTimeManager );
REGISTER_MESSAGE_HANDLER(ResetTime,		OnResetTime,	CPerfTimeManager );
REGISTER_MESSAGE_HANDLER(InitTime,		OnResetTime,	CPerfTimeManager ); //same as reset
REGISTER_MESSAGE_HANDLER(PauseTime,		OnPauseTime,	CPerfTimeManager );
REGISTER_MESSAGE_HANDLER(UnpauseTime,	OnUnpauseTime,	CPerfTimeManager );
REGISTER_MESSAGE_HANDLER(GetCurrentFPS,	OnGetCurrentFPS,CPerfTimeManager );
REGISTER_MESSAGE_HANDLER(SetTimeScale,	OnSetTimeScale,	CPerfTimeManager );
REGISTER_MESSAGE_HANDLER(SetMaxTick,	OnSetMaxTick,	CPerfTimeManager );
REGISTER_MESSAGE_HANDLER(GetCurrentFrame, OnGetCurrentFrame, CPerfTimeManager);

CPerfTimeManager::CPerfTimeManager() : 
	CManager(_T("CPerfTimeManager"), TIMEPRIORITY)
{
	m_Timer = m_ToolBox->GetTimer();
	m_Timer->ResetTime();
}

IComponent *CPerfTimeManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CPerfTimeManager );
}

CPerfTimeManager::~CPerfTimeManager( )
{
}

bool CPerfTimeManager::Init()
{
	return true;
}

void CPerfTimeManager::Update( DWORD tickCount )
{	
}

bool CPerfTimeManager::DeInit( void )
{	
	return true;
}

DWORD CPerfTimeManager::OnGetRealTime(DWORD size, void *params)
{
	float * tdata;
	VERIFY_MESSAGE_SIZE( size, sizeof( float ) );
	tdata = (float*) params;
	if( tdata )
	{
		*tdata = m_Timer->GetRealTime();
	}
	return MSG_HANDLED_STOP;
}

DWORD CPerfTimeManager::OnGetTime(DWORD size, void *params)
{
	float * tdata;
	VERIFY_MESSAGE_SIZE( size, sizeof( float ) );
	tdata = (float*) params;
	if( tdata )
	{
		*tdata = m_Timer->GetTime();
	}
	return MSG_HANDLED_STOP;
}

DWORD CPerfTimeManager::OnGetCurrentFPS(DWORD size, void *params)
{
	float * tdata;
	VERIFY_MESSAGE_SIZE( size, sizeof( float ) );
	tdata = (float*) params;
	if( tdata )
	{
		*tdata = m_Timer->GetFPS();
	}
	return MSG_HANDLED_STOP;
}
DWORD CPerfTimeManager::OnGetTimeDelta(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETTIMEDELTAPARAMS));
	GETTIMEDELTAPARAMS *gtdp = (GETTIMEDELTAPARAMS*)params;

	gtdp->fDeltaTime = m_Timer->GetTimeDelta(gtdp->hsTimerName);

	return MSG_HANDLED_STOP;
}

DWORD CPerfTimeManager::OnUpdateTime(DWORD size, void *params)
{
	float * tdata;
	VERIFY_MESSAGE_SIZE( size, sizeof( float ) );
	tdata = (float*) params;
	float v = m_Timer->GetTime();
	if( tdata )
	{
		*tdata = v; 
	}
	return MSG_HANDLED_STOP;
}

DWORD CPerfTimeManager::OnResetTime(DWORD size, void *params)
{
	m_Timer->ResetTime();
	return MSG_HANDLED_STOP;
}

DWORD CPerfTimeManager::OnPauseTime(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));
	IHashString *timerName = (IHashString*)params;

	m_Timer->PauseTime(timerName);

	return MSG_HANDLED_STOP;
}

DWORD CPerfTimeManager::OnUnpauseTime(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));
	IHashString *timerName = (IHashString*)params;

	m_Timer->UnpauseTime(timerName);

	return MSG_HANDLED_STOP;
}


DWORD CPerfTimeManager::OnGetCurrentFrame(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( unsigned int ) );
	unsigned int * msg = (unsigned int * )params;
	if( msg )
	{
		*msg = m_Timer->GetCurrentFrame();
	}
	return MSG_HANDLED_STOP;
}

IHashString* CPerfTimeManager::GetComponentType()
{
	static CHashString perfCT = _T("CPerfTimeManager");
	return &perfCT;
}

DWORD CPerfTimeManager::OnSetTimeScale(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(SETDELTATIMERSCALEPARAMS));
	SETDELTATIMERSCALEPARAMS sdtsp = *(SETDELTATIMERSCALEPARAMS*)param;

	m_Timer->SetTimeScale( sdtsp.fTimeScale, sdtsp.hsTimerName );

	return MSG_HANDLED_STOP;
}

DWORD CPerfTimeManager::OnSetMaxTick(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float maxTick = *(float*) param;

	m_Timer->SetMaxTick( maxTick );

	return MSG_HANDLED_STOP;
}