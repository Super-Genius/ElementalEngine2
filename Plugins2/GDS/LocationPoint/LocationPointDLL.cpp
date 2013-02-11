///============================================================================
/// \file		LocationPointDLL.cpp
/// \brief		Defines the entry point for the DLL application.
/// \date		12/13/2006
/// \author		Nick Tourte
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

#define LOCATIONPOINT_PRIORITY 31000

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


extern "C" __declspec(dllexport) void InitDLL()
{

}

extern "C" __declspec(dllexport) DWORD GetDLLVersion()
{
	return INTERFACEDLLVERSION;
}

extern "C" __declspec(dllexport) DWORD GetPriority()
{
	return LOCATIONPOINT_PRIORITY;
}
