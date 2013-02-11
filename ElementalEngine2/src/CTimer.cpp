///============================================================================
/// \file		CTimer.h
/// \brief		Default timer implementation
/// \date		12-19-2007
/// \author		Kyle Swaim
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
#include "CTimer.h"

// Register the singleton
REGISTER_COMPONENT_SINGLETON(CTimer);

// Register Messages
REGISTER_MESSAGE_HANDLER(GlobalUpdateTick, GlobalTickMessage, CTimer);

CTimer::CTimer()
{
	ResetTime();
	m_fMaxTick = 99999999.9f;
}

CTimer::~CTimer()
{
}


IComponent* CTimer::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CTimer);
}

void CTimer::Serialize(IArchive &ar)
{
	// Nothing to serialize
}

IHashString* CTimer::GetComponentType()
{
	static CHashString LFCT(_T("CTimer"));
	return &LFCT;
}

bool CTimer::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

float CTimer::GetTime()
{
	return _curGameTime;
}

float CTimer::GetRealTime()
{
	UINT64 ticks;
	float time;

	// This is the number of clock ticks since start
	if( !QueryPerformanceCounter((LARGE_INTEGER *)&ticks) )
		ticks = (UINT64)GetTickCount();

	// Divide by frequency to get the time in seconds
	time = (float)(INT64)ticks/(float)(INT64)_ticksPerSecond;
	return time;
}

float CTimer::GetFPS()
{
	return _fps;
}

float CTimer::GetTimeDelta(IHashString* hsTimerName)
{
	float rawDelta = CalcTimeDelta();

	// Check if its a named timer
	if (hsTimerName != NULL)
	{
		DWORD dwName = hsTimerName->GetUniqueID();
		if (m_mDeltaTimers[dwName].m_bPaused == true)
		{
			return 0.0f;
		}

		return rawDelta * m_mDeltaTimers[dwName].m_fTimeScale;
	}
	else
	{
		return rawDelta;
	}
}

int CTimer::GetCurrentFrame()
{
	return _numFrames;
}

void CTimer::ResetTime()
{
	InitGameTime();
	_lastUpdate        = 0;
	_fpsUpdateInterval = 0.5f;
	_numFrames         = 0;
	_fps               = 0;		
	_curGameTime		= 0;
	_lastTime			= 0;
	_curTimeDelta		= 0;
}

void CTimer::PauseTime(IHashString* hsTimerName)
{
	if (hsTimerName)
	{
		m_mDeltaTimers[hsTimerName->GetUniqueID()].m_bPaused = true;
	}
}

void CTimer::UnpauseTime(IHashString* hsTimerName)
{
	if (hsTimerName)
	{
		m_mDeltaTimers[hsTimerName->GetUniqueID()].m_bPaused = false;
	}
}

void CTimer::SetTimeScale(float scale, IHashString* hsTimerName)
{
	if (hsTimerName)
	{
		m_mDeltaTimers[hsTimerName->GetUniqueID()].m_fTimeScale = scale;
	}
}

void CTimer::SetMaxTick(float maxTick)
{
	m_fMaxTick = maxTick;
}

void CTimer::Update()
{
	GetGameTime();
	UpdateFPS();
}

DWORD CTimer::GlobalTickMessage(DWORD size, void *param)
{
	//VERIFY_MESSAGE_SIZE(size, sizeof(GLOBALTICKPARAMS));
	//GLOBALTICKPARAMS *gtp = (GLOBALTICKPARAMS*)param;

	Update();

	return MSG_HANDLED_PROCEED;
}
