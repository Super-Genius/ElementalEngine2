///============================================================================
/// \file		Win32EnvironmentDLL.cpp
/// \brief		Implementation of Win32 Environment
/// \date		06-27-2008
/// \author		Zsolt Matyas
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

//This section defining INTERFACEDLLVERSION is a PURE HACK until Ken checks-in his IDLL interface changes.
#ifdef _DEBUG
// major version in upper 16 bits
// low word =  minor version, low bit debug =0, release=1
#define INTERFACEDLLVERSION 0x00006100
#else
#define INTERFACEDLLVERSION 0x00006101
#endif

// we don't need loading before anybody, so set priority rather high
#define WIN32ENVIRONMENTPRIORITY_PRIORITY 3400

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
			break;
	}

    return TRUE;
}
#endif

DLL_API void InitDLL()
{
	IToolBox *toolBox = EngineGetToolBox();
	static CHashString hszEnvironmentMgr(_T("CWin32EnvironmentManager"));
	toolBox->CreateComponent(&hszEnvironmentMgr, 0);
}

DLL_API void DeInitDLL()
{
	IToolBox *toolBox = EngineGetToolBox();
}

DLL_API DWORD GetDLLVersion()
{
	return INTERFACEDLLVERSION;
}

DLL_API DWORD GetPriority()
{
	return WIN32ENVIRONMENTPRIORITY_PRIORITY;
}

#ifdef _LIB
INITDLLINFO WIN32ENVIRONMENT_INITDLLINFO = {
	InitDLL, 
	GetDLLVersion, 
	GetPriority, 
	NULL
};
#endif
