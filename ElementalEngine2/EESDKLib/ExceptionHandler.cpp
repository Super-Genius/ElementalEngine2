///============================================================================
/// \file	ExceptionHandler.cpp
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
#ifdef WIN32
#include "dbghelp.h"

// hidden function - does a mini dump
void DumpMiniDump(HANDLE hFile, PEXCEPTION_POINTERS excpInfo)
{
	if (excpInfo == NULL) 
	{
		// Generate exception to get proper context in dump
		__try 
		{
			RaiseException(EXCEPTION_BREAKPOINT, 0, 0, NULL);
		} 
		__except(DumpMiniDump(hFile, GetExceptionInformation()),
				 EXCEPTION_CONTINUE_EXECUTION) 
		{
		}
	} 
	else
	{
		MINIDUMP_EXCEPTION_INFORMATION eInfo;
		eInfo.ThreadId = GetCurrentThreadId();
		eInfo.ExceptionPointers = excpInfo;
		eInfo.ClientPointers = FALSE;

		// note:  MiniDumpWithIndirectlyReferencedMemory does not work on Win98
		MiniDumpWriteDump(
			GetCurrentProcess(),
			GetCurrentProcessId(),
			hFile,
			MiniDumpNormal,
			excpInfo ? &eInfo : NULL,
			NULL,
			NULL);
	}
}

int __cdecl HandleException(PEXCEPTION_POINTERS pExceptPtrs)
{
	static bool bFirstTime = true;
	if (!bFirstTime)	// Going recursive! That must mean this routine crashed!
		return EXCEPTION_CONTINUE_SEARCH;
	bFirstTime = false;

	TCHAR *dumpFile = _T("EECrash.dmp");

	// Create the file
	HANDLE hMiniDumpFile = CreateFile(
		dumpFile,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
		NULL);

	// Write the minidump to the file
	if (hMiniDumpFile != INVALID_HANDLE_VALUE)
	{
		DumpMiniDump(hMiniDumpFile, pExceptPtrs);

		// Close file
		CloseHandle(hMiniDumpFile);
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

#endif //WIN32
