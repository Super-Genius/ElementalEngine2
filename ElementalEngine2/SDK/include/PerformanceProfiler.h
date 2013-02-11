///============================================================================
/// \file		PerformanceProfiler.h
/// \brief		Header file for performance profiler
/// \date		11-16-2006
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

#ifndef PERFORMANCEPROFILER_H
#define PERFORMANCEPROFILER_H

#include "ITimer.h"

typedef std::map<DWORD, float> PERFORMANCEPROFILERMAP;

enum PROFILERDISPLAYTYPE {NAME_TIME=0, NAME_NUMBER, NAME_COUNT};

typedef struct PERFORMANCEPROFILERUPDATE
{
	IHashString *name;
	IHashString *type;
	float time;
	PROFILERDISPLAYTYPE displayType;
	PERFORMANCEPROFILERUPDATE()
	{
		name = NULL;
		type = NULL;
		time = 0.0f;
		displayType = NAME_TIME;
	}
} PERFORMANCEPROFILERUPDATE;

struct PERFORMANCEPROFILERMAPDISPLAY
{
	PROFILERDISPLAYTYPE displayType;
	PERFORMANCEPROFILERMAP map;
	PERFORMANCEPROFILERMAPDISPLAY()
	{
		displayType = NAME_TIME;
	}
};

typedef map<DWORD, PERFORMANCEPROFILERMAPDISPLAY> PERFORMANCEPROFILERTYPEMAP;

struct GETPERFORMANCEPROFILERS
{
	PERFORMANCEPROFILERTYPEMAP *pPerformanceProfilerMap; // pointer to a map to copy performance profiler info into
};

struct PROCESSMEMINFO
{
	DWORD pageFaultCount;
	UINT peakWorkingSetSize;
	UINT workingSetSize;
	UINT quotaPeakPagedPoolUsage;
	UINT quotaPagedPoolUsage;
	UINT quotaPeakNonPagedPoolUsage;
	UINT quotaNonPagedPoolUsage;
	UINT pageFileUsage;
	UINT peakPageFileUsage;
};

class CPerformanceProfiler
{
public:
	CPerformanceProfiler( const TCHAR *szName, const TCHAR *szType, bool bAuto, PROFILERDISPLAYTYPE displayType = NAME_TIME ) 
		: m_hszType(szType), m_hszName(szName), m_bAuto( bAuto ), m_DisplayType( displayType )
	{
		if (bAuto)
			Start();		
	}
	~CPerformanceProfiler()
	{
		if (m_bAuto)
			Stop();
	}
	void Start()
	{
		ITimer* timer = EngineGetToolBox()->GetTimer();
		m_fStartTime = timer->GetRealTime();
	}
	void Stop()
	{
		ITimer* timer = EngineGetToolBox()->GetTimer();
		float fEndTime = timer->GetRealTime();

		float fDeltaTime = fEndTime - m_fStartTime;

		PERFORMANCEPROFILERUPDATE msg;
		msg.name = &m_hszName;
		msg.time = fDeltaTime;

		if (m_hszType.IsEmpty())
		{
			msg.type = &m_hszType;
		}

		msg.displayType = m_DisplayType;

		static DWORD msgHash_UpdatePerformanceProfiler = CHashString(_T("UpdatePerformanceProfiler")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_UpdatePerformanceProfiler, sizeof(msg), &msg );
	}

	bool m_bAuto;
	CHashString m_hszName;
	CHashString m_hszType;
	PROFILERDISPLAYTYPE m_DisplayType;
	float m_fStartTime;
};

#if defined _PROFILE || defined _DEBUG

#define PERFORMANCE_PROFILER_AUTO( profile_name ) CPerformanceProfiler performace_profiler_##profile_name( _T(#profile_name), _T(""), true )

// with macro strings...
#define PERFORMANCE_PROFILER_START( profile_name ) CPerformanceProfiler performace_profiler_##profile_name( _T(#profile_name), _T(""), false ); performace_profiler_##profile_name.Start();
#define PERFORMANCE_PROFILER_STOP( profile_name ) { performace_profiler_##profile_name.Stop(); }

// with TCHAR*...
#define PERFORMANCE_PROFILER_TCHAR_START( profile_name ) CPerformanceProfiler performace_profiler( profile_name, _T(""), false ); performace_profiler.Start();
#define PERFORMANCE_PROFILER_TCHAR_STOP( profile_name ) { performace_profiler.Stop(); }

// with types (TCHAR strings)
#define PERFORMANCE_PROFILER_TYPE_START( szProfileName, szProfileType ) CPerformanceProfiler performace_profiler( szProfileName, szProfileType, false ); performace_profiler.Start();
#define PERFORMANCE_PROFILER_TYPE_STOP( szProfileName, szProfileType ){ performace_profiler.Stop(); }

// with display type
#define PERFORMANCE_PROFILER_DISPLAY_TYPE_START( szProfileName, szProfileType, displayType ) CPerformanceProfiler performace_profiler( szProfileName, szProfileType, false, displayType ); performace_profiler.Start();
#define PERFORMANCE_PROFILER_DISPLAY_TYPE_STOP( szProfileName, szProfileType, displayType ){ performace_profiler.Stop(); }

#else // _PROFILE

#define PERFORMANCE_PROFILER_AUTO( profile_name )
#define PERFORMANCE_PROFILER_START( profile_name )
#define PERFORMANCE_PROFILER_STOP( profile_name )
#define PERFORMANCE_PROFILER_TCHAR_START( profile_name )
#define PERFORMANCE_PROFILER_TCHAR_STOP( profile_name )
#define PERFORMANCE_PROFILER_TYPE_START( szProfileName, szProfileType )
#define PERFORMANCE_PROFILER_TYPE_STOP( szProfileName, szProfileType )
#define PERFORMANCE_PROFILER_DISPLAY_TYPE_START( szProfileName, szProfileType, displayType )
#define PERFORMANCE_PROFILER_DISPLAY_TYPE_STOP( szProfileName, szProfileType, displayType )

#endif // _PROFILE

#endif // PERFORMANCEPROFILER_H