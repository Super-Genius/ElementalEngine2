///============================================================================
/// \file	PreformanceProfilerHandler.cpp
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

#include "StdAfx.h"
#include "PerformanceProfilerHandler.h"
#ifdef WIN32
#include <psapi.h>
#endif

REGISTER_COMPONENT_SINGLETON( CPerformanceProfilerHandler );
REGISTER_MESSAGE_HANDLER(UpdatePerformanceProfiler, OnUpdatePerformanceProfiler, CPerformanceProfilerHandler);
REGISTER_MESSAGE_HANDLER(GetPerformanceProfilers, OnGetPerformanceProfilers, CPerformanceProfilerHandler);
REGISTER_MESSAGE_HANDLER(GetProcessMemoryUsage, OnGetProcessMemoryUsage, CPerformanceProfilerHandler);
REGISTER_MESSAGE_HANDLER(ClearProfilerSubMap, OnClearProfilerSubMap, CPerformanceProfilerHandler);

CPerformanceProfilerHandler::CPerformanceProfilerHandler()
{
	m_dwDefault = CHashString(_T("Default")).GetUniqueID();
}

CPerformanceProfilerHandler::~CPerformanceProfilerHandler()
{
	EngineGetToolBox()->UnRegisterComponent( GetComponentType() );
}

IComponent *CPerformanceProfilerHandler::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CPerformanceProfilerHandler );
}

DWORD CPerformanceProfilerHandler::OnUpdatePerformanceProfiler(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(PERFORMANCEPROFILERUPDATE) );
	PERFORMANCEPROFILERUPDATE *message = (PERFORMANCEPROFILERUPDATE*)params;

	DWORD dwType = 0;

	if (message->type == NULL)
	{
		dwType = m_dwDefault;
	}
	else
	{
		dwType = message->type->GetUniqueID();
	}

	m_PerformanceProfilers[dwType].map[message->name->GetUniqueID()] = message->time;	
	m_PerformanceProfilers[dwType].displayType = message->displayType;
	
	return MSG_HANDLED_PROCEED;
}

DWORD CPerformanceProfilerHandler::OnGetPerformanceProfilers(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof(GETPERFORMANCEPROFILERS) );
	GETPERFORMANCEPROFILERS *message = (GETPERFORMANCEPROFILERS*)params;

	if (message->pPerformanceProfilerMap == NULL)
		return MSG_ERROR;

	// should probably move this to a better place... give this manager an update tick?
	// do we want to do this every frame?
	PROCESSMEMINFO pmi;
	OnGetProcessMemoryUsage(sizeof(PROCESSMEMINFO), &pmi);
	UpdatePerfMapMemory(pmi);

	// get map address (faster than deep copy)
	message->pPerformanceProfilerMap = &m_PerformanceProfilers;

	return MSG_HANDLED_PROCEED;
}

DWORD CPerformanceProfilerHandler::OnGetProcessMemoryUsage(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(PROCESSMEMINFO));
	PROCESSMEMINFO *pmi = (PROCESSMEMINFO*)param;	

#ifdef WIN32
	// get process handle
	HANDLE tempProcess;
	DWORD currID = GetCurrentProcessId();

	// open with new access modifiers
	tempProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, currID);

	if (tempProcess == NULL)
	{
		return MSG_ERROR;
	}

	PROCESS_MEMORY_COUNTERS pmc;

	// get info
	BOOL ret = GetProcessMemoryInfo(tempProcess, &pmc, sizeof(pmc));
	if (ret == FALSE)
	{
		return MSG_ERROR;
	}

	// move info into out struct
	pmi->pageFaultCount = pmc.PageFaultCount;
	// PROCESSMEMINFO size_t instead of UINT
	pmi->pageFileUsage = (UINT)pmc.PagefileUsage;
	pmi->peakPageFileUsage = (UINT)pmc.PeakPagefileUsage;
	pmi->peakWorkingSetSize = (UINT)pmc.PeakWorkingSetSize;
	pmi->quotaNonPagedPoolUsage = (UINT)pmc.QuotaNonPagedPoolUsage;
	pmi->quotaPagedPoolUsage = (UINT)pmc.QuotaPagedPoolUsage;
	pmi->quotaPeakNonPagedPoolUsage = (UINT)pmc.QuotaPeakNonPagedPoolUsage;
	pmi->quotaPeakPagedPoolUsage = (UINT)pmc.QuotaPeakPagedPoolUsage;
	pmi->workingSetSize = (UINT)pmc.WorkingSetSize;

	// close temp process
	CloseHandle(tempProcess);

	return MSG_HANDLED_STOP;
#else
	return MSG_NOT_HANDLED;
#endif
}

DWORD CPerformanceProfilerHandler::OnClearProfilerSubMap(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));

	IHashString *clearMap = (IHashString*)param;
	
	if (clearMap)
	{
		m_PerformanceProfilers[clearMap->GetUniqueID()].map.clear();		
	}

	return MSG_HANDLED_STOP;
}

void CPerformanceProfilerHandler::UpdatePerfMapMemory(PROCESSMEMINFO pmi)
{
	PERFORMANCEPROFILERUPDATE ppu;

	static CHashString hsMemUsage(_T("Memory Usage"));

	static CHashString hsPageFaultCount(_T("PageFaultCount"));
	static CHashString hsPageFileUsage(_T("PageFileUsage"));
	static CHashString hsPeakPageFileUsage(_T("PeakPageFileUsage"));
	static CHashString hsPeakWorkingSetSize(_T("PeakWorkingSetSize"));	
	static CHashString hsQuotaNonPagedPoolUsage(_T("QuotaNonPagedPoolUsage"));
	static CHashString hsQuotaPagedPoolUsage(_T("QuotaPagedPoolUsage"));
	static CHashString hsQuotaPeakNonPagedPoolUsage(_T("QuotaPeakNonPagedPoolUsage"));
	static CHashString hsQuotaPeakPagedPoolUsage(_T("QuotaPeakPagedPoolUsage"));
	static CHashString hsWorkingSetSize(_T("WorkingSetSize"));

	ppu.displayType = NAME_NUMBER;
	ppu.type = &hsMemUsage;

	// one for each
	ppu.name = &hsPageFaultCount;
	ppu.time = (float)pmi.pageFaultCount;
	OnUpdatePerformanceProfiler(sizeof(PERFORMANCEPROFILERUPDATE), &ppu);
	
	ppu.name = &hsPageFileUsage;
	ppu.time = (float)pmi.pageFileUsage;
	OnUpdatePerformanceProfiler(sizeof(PERFORMANCEPROFILERUPDATE), &ppu);

	ppu.name = &hsPeakPageFileUsage;
	ppu.time = (float)pmi.peakPageFileUsage;
	OnUpdatePerformanceProfiler(sizeof(PERFORMANCEPROFILERUPDATE), &ppu);

	ppu.name = &hsPeakWorkingSetSize;
	ppu.time = (float)pmi.peakWorkingSetSize;
	OnUpdatePerformanceProfiler(sizeof(PERFORMANCEPROFILERUPDATE), &ppu);
	
	ppu.name = &hsQuotaNonPagedPoolUsage;
	ppu.time = (float)pmi.quotaNonPagedPoolUsage;
	OnUpdatePerformanceProfiler(sizeof(PERFORMANCEPROFILERUPDATE), &ppu);

	ppu.name = &hsQuotaPagedPoolUsage;
	ppu.time = (float)pmi.quotaPagedPoolUsage;
	OnUpdatePerformanceProfiler(sizeof(PERFORMANCEPROFILERUPDATE), &ppu);

	ppu.name = &hsQuotaPeakNonPagedPoolUsage;
	ppu.time = (float)pmi.quotaPeakNonPagedPoolUsage;
	OnUpdatePerformanceProfiler(sizeof(PERFORMANCEPROFILERUPDATE), &ppu);
	
	ppu.name = &hsQuotaPeakPagedPoolUsage;
	ppu.time = (float)pmi.quotaPeakPagedPoolUsage;
	OnUpdatePerformanceProfiler(sizeof(PERFORMANCEPROFILERUPDATE), &ppu);
	
	ppu.name = &hsWorkingSetSize;
	ppu.time = (float)pmi.workingSetSize;
	OnUpdatePerformanceProfiler(sizeof(PERFORMANCEPROFILERUPDATE), &ppu);		
}