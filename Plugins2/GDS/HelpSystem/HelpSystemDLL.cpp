///============================================================================
/// \file		stdafx.h
/// \brief		Implementation of DLL entrypoint.
/// \date		11-11-2007
/// \author		Dmitriy S. Sergeyev
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#define INTERFACEDLLVERSION 0x00006100
#else
#define INTERFACEDLLVERSION 0x00006101
#endif

// we don't need loading before anybody, so set priority rather high
#define HELPSYSTEM_PRIORITY 40200

AFX_EXTENSION_MODULE HelpSystemDLL = { false, NULL, NULL };
CString DLLModuleName;

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("HelpSystem.dlo Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(HelpSystemDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(HelpSystemDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("HelpSystem.dlo Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(HelpSystemDLL);
	}
    return TRUE;
}

extern "C" __declspec(dllexport) void InitDLL()
{
	const int BUFFSIZE = 0x10000;
	TCHAR lpBuff[BUFFSIZE];
	if (GetModuleFileName(HelpSystemDLL.hModule, lpBuff, BUFFSIZE) != 0)
	{
		DLLModuleName = lpBuff;
	}
}

extern "C" __declspec(dllexport) DWORD GetDLLVersion()
{
	return INTERFACEDLLVERSION;
}

extern "C" __declspec(dllexport) DWORD GetPriority()
{
	return HELPSYSTEM_PRIORITY;
}
