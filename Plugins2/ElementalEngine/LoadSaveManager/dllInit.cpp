///============================================================================
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
#include <string>
using namespace std;

#ifdef _DEBUG
#define INTERFACEDLLVERSION 0x00006100
#else
#define INTERFACEDLLVERSION 0x00006101
#endif

// we don't need loading before anybody, so set priority rather high
#define LOADERSAVERMANAGER_PRIORITY 15450

void SearchDLLS(const string *pathName);

list<HMODULE> gLoadDLLs;

#ifdef _LIB
#define DLL_API static
#else
#define DLL_API extern "C" __declspec(dllexport)

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_PROCESS_DETACH:
			list<HMODULE>::iterator lhi;
			for (lhi=gLoadDLLs.begin(); lhi != gLoadDLLs.end(); lhi++)
			{
				FreeLibrary(*lhi);
			}
			break;
	}

    return TRUE;
}

// ----- search through dlls and load them
void SearchDLLS(const string *pathName)
{
	string path;
	WIN32_FIND_DATA fData;
	HANDLE hFile;
	BOOL done;
	size_t wildIndex;
	string dllName;

	wildIndex = pathName->rfind('\\');
	path = pathName->substr(0, wildIndex+1);


	hFile = FindFirstFile(pathName->c_str(), &fData);
	done = (hFile == INVALID_HANDLE_VALUE);

	while (!done)
	{ 
		HMODULE lHandle;
		dllName = path;
		dllName += fData.cFileName;
		lHandle = LoadLibrary(dllName.c_str());
		if (lHandle == NULL)
		{
			EngineGetToolBox()->SetErrorValue(ERR_FILE_LOAD);
			EngineGetToolBox()->Log(LOGWARNING, _T("Unable to load dll %s error %d\n"), 
				fData.cFileName, GetLastError());
		}
		else
		{
			gLoadDLLs.push_back(lHandle);
		}
 
	    if (!FindNextFile(hFile, &fData))
		{
	
        done = TRUE;
	    }
	}

	if (hFile != INVALID_HANDLE_VALUE)
	{
		FindClose(hFile);
	}

} 
#endif

DLL_API void InitDLL()
{
	// on initialization, search for and load all
	// "*.dls" which are dynamic Loader/Saver modules in
	// dll format.
#ifndef _LIB
	SearchDLLS(&string(".\\Plugins\\*.dls"));
#endif
}

DLL_API DWORD GetDLLVersion()
{
	return INTERFACEDLLVERSION;
}

DLL_API DWORD GetPriority()
{
	return LOADERSAVERMANAGER_PRIORITY;
}

#ifdef _LIB
#include ".\StaticLibSymbols.h"
INITDLLINFO LOADSAVEMANAGER_INITDLLINFO = {
	InitDLL, 
	GetDLLVersion, 
	GetPriority, 
	NULL,
	LOADSAVEMANAGER_LIBEXTERNS
};
#endif
