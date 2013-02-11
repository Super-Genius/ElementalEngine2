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

	static CHashString Cal3dLoaderType(_T("CCal3dLoader"));
	static CHashString Cal3dSkeletonLoaderType(_T("CCal3dSkeletonLoader"));
	static CHashString Cal3dMeshLoaderType(_T("CCal3dMeshLoader"));
	static CHashString Cal3dMeshTextLoaderType(_T("CCal3dMeshTextLoader"));
	static CHashString Cal3dAnimationLoaderType(_T("CCal3dAnimationLoader"));
	toolBox->CreateComponent(&Cal3dLoaderType, 0);
	toolBox->CreateComponent(&Cal3dSkeletonLoaderType, 0);
	toolBox->CreateComponent(&Cal3dMeshLoaderType, 0);
	toolBox->CreateComponent(&Cal3dMeshTextLoaderType, 0);
	toolBox->CreateComponent(&Cal3dAnimationLoaderType, 0);
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

INITDLLINFO CAL3DLOADER_INITDLLINFO = {
	InitDLL, 
	GetDLLVersion, 
	GetPriority, 
	NULL,
	CAL3DLOADER_LIBEXTERNS
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
