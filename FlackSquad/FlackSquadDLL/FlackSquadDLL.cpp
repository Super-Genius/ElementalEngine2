// GUISystem.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#ifdef _DEBUG
#define INTERFACEDLLVERSION 0x00006100
#else
#define INTERFACEDLLVERSION 0x00006101
#endif

#define TOD_PRIORITY 49000 // I wonder, what's a good priority for this?

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
	SINGLETONINSTANCE(FlackSquadComponent); // poke component
}

DLL_API DWORD GetDLLVersion()
{
	return INTERFACEDLLVERSION;
}

DLL_API DWORD GetPriority()
{
	return TOD_PRIORITY;
}

#ifdef _LIB
#include "./StaticLibSymbols.h"
INITDLLINFO FLACKSQUAD_INITDLLINFO = {
	InitDLL, 
	GetDLLVersion, 
	GetPriority, 
	NULL,
	FLACKSQUAD_LIBEXTERNS
};
#endif
