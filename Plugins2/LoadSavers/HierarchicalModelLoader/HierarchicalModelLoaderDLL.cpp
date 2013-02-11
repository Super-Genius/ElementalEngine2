// ParticleLoader.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "EEDefines.h"

#ifdef _DEBUG
#define INTERFACEDLLVERSION 0x00006100
#else
#define INTERFACEDLLVERSION 0x00006101
#endif

#define LOADERSAVER_PRIORITY 15451

#define DLL_API static

DLL_API void InitDLL()
{
	IToolBox *toolBox = EngineGetToolBox();

	static CHashString CHierarchicalModelLoaderType(_T("CHierarchicalModelLoader"));
	static CHashString CHierarchicalAnimationLoaderType(_T("CHierarchicalAnimationLoader"));
	toolBox->CreateComponent(&CHierarchicalModelLoaderType, 0);
	toolBox->CreateComponent(&CHierarchicalAnimationLoaderType, 0);
}

DLL_API DWORD GetDLLVersion()
{
	return INTERFACEDLLVERSION;
}

DLL_API DWORD GetPriority()
{
	return LOADERSAVER_PRIORITY;
}

#ifdef _LIB

#include ".\StaticLibSymbols.h"

INITDLLINFO HIERARCHICALMODELLOADER_INITDLLINFO = {
	InitDLL, 
	GetDLLVersion, 
	GetPriority, 
	NULL,
	HIERARCHICALMODELLOADER_LIBEXTERNS
};

#else

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			InitDLL();
			break;
	}
    return TRUE;
}

#endif
