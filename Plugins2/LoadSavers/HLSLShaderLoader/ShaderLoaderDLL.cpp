// ParticleLoader.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	IToolBox *toolBox;
	toolBox = EngineGetToolBox();

	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			// on initialization, search for and load all
			// "*.dls" which are dynamic Loader/Saver modules in
			// dll format.
			// force creation of CExampleLoader
			toolBox->CreateComponent(&CHashString("CHierarchicalModelLoader"));
			toolBox->CreateComponent(&CHashString("CHierarchicalAnimationLoader"));
			return TRUE;
			break;
	}
    return TRUE;
}

