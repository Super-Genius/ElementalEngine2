///==========================================================================
/// \file	NovodexPhysicsDLL.cpp
/// \brief	Defines the entry point for the DLL application.
/// \date	9/22/2006
/// \author	Kyle Swaim
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
///==========================================================================

#include "stdafx.h"

#ifdef _DEBUG
#define INTERFACEDLLVERSION 0x00006100
#else
#define INTERFACEDLLVERSION 0x00006101
#endif

// Are lower numbers higher priority or are higher numbers higher priority?
#define PHYSICS_PRIORITY 5500

#ifdef _LIB
#define DLL_API static
#else
#define DLL_API extern "C" __declspec(dllexport)

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
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
	IToolBox *toolBox;
	toolBox = EngineGetToolBox();

	toolBox->CreateComponent( &CHashString("CPhysicsManager"), 0 );
}

DLL_API DWORD GetDLLVersion()
{
	return INTERFACEDLLVERSION;
}

DLL_API DWORD GetPriority()
{
	return PHYSICS_PRIORITY;
}

DLL_API void DeInitDLL()
{
	IToolBox *toolBox;
	toolBox = EngineGetToolBox();
	static DWORD msgHash_ShutDownPhysics = CHashString(_T("ShutDownPhysics")).GetUniqueID();
	toolBox->SendMessage(msgHash_ShutDownPhysics, 0, NULL);
}

#ifdef _LIB
#include ".\StaticLibSymbols.h"
INITDLLINFO NOVODEXPHYSICS_INITDLLINFO = {
	InitDLL, 
	GetDLLVersion, 
	GetPriority, 
	DeInitDLL,
	NOVODEXPHYSICS_LIBEXTERNS
};
#endif
