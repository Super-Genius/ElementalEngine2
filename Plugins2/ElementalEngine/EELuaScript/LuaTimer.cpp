///============================================================================
/// \file		LuaTimer.cpp
/// \brief		Lua wrapper for CPerfTimer
/// \date		12-19-2007
/// \author		Kyle Swaim
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
//#include "..\..\..\ElementalEngine2\src\CPerfTimer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLuaTimer::CLuaTimer()
{
	m_Timer = EngineGetToolBox()->GetTimer();
}

CLuaTimer::~CLuaTimer()
{
}

float CLuaTimer::GetTime()
{
	return m_Timer->GetTime();
}

float CLuaTimer::GetRealTime()
{
	return m_Timer->GetRealTime();
}

float CLuaTimer::GetFPS()
{
	return m_Timer->GetFPS();
}

float CLuaTimer::GetTimeDelta(IHashString* hsTimerName)
{
	return m_Timer->GetTimeDelta(hsTimerName);
}

int CLuaTimer::GetCurrentFrame()
{
	return m_Timer->GetCurrentFrame();
}

void CLuaTimer::ResetTime()
{
	m_Timer->ResetTime();
}

void CLuaTimer::PauseTime(IHashString* hsTimerName)
{
	m_Timer->PauseTime(hsTimerName);
}

void CLuaTimer::UnpauseTime(IHashString* hsTimerName)
{
	m_Timer->UnpauseTime(hsTimerName);
}

void CLuaTimer::SetTimeScale(float scale, IHashString* hsTimerName)
{
	m_Timer->SetTimeScale(scale, hsTimerName);
}

void CLuaTimer::SetMaxTick(float maxTick)
{
	m_Timer->SetMaxTick(maxTick);
}