///============================================================================
/// \file		LogViewerDLL.cpp
/// \brief		Defines the entry point for the DLL application.
/// \date		02/15/2008
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

#include "stdafx.h"

#ifdef _DEBUG
#define INTERFACEDLLVERSION 0x00006100
#else
#define INTERFACEDLLVERSION 0x00006101
#endif

// we don't need loading before anybody, so set priority rather high
#define LOGVIEWER_PRIORITY 52001

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}


extern "C" __declspec(dllexport) void InitDLL()
{
	IToolBox *toolBox;
	toolBox = EngineGetToolBox();

	toolBox->CreateComponent(&CHashString("CLogViewerComponent"), 0);
}

extern "C" __declspec(dllexport) DWORD GetDLLVersion()
{
	return INTERFACEDLLVERSION;
}

extern "C" __declspec(dllexport) DWORD GetPriority()
{
	return LOGVIEWER_PRIORITY;
}
