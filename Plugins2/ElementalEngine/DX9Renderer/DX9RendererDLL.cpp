///============================================================================
/// \file	CDX9RendererDLL.cpp
/// \brief	Defines the entry point for the DLL application.
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
#define DX9RENDERER_PRIORITY 1000

void DeInitFontImplementation();
void InitFontImplementation();

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

	//toolBox->CreateComponent(&CHashString("CRenderManager"));
	FONTIMPLEMENTATIONMSG fimsg;
	fimsg.impl = CDX9FontImplementation::Instance();
	InitFontImplementation();
	static DWORD msgHash_SetFontImplementation = CHashString(_T("SetFontImplementation")).GetUniqueID();
	toolBox->SendMessage(msgHash_SetFontImplementation, sizeof(FONTIMPLEMENTATIONMSG), &fimsg );

}

DLL_API DWORD GetDLLVersion()
{
	return INTERFACEDLLVERSION;
}

DLL_API DWORD GetPriority()
{
	return DX9RENDERER_PRIORITY;
}

DLL_API void DeInitDLL()
{
	IToolBox *toolBox;
	toolBox = EngineGetToolBox();

	SINGLETONINSTANCE( CDX9Renderer )->OnDestroyRenderer();

	static DWORD msgHash_DestroyRenderer = CHashString(_T("DestroyRenderer")).GetUniqueID();
	toolBox->SendMessage(msgHash_DestroyRenderer, 0, NULL);

	DeInitFontImplementation();
	CDX9FontImplementation::Destroy();

	SINGLETONINSTANCE( CDX9Renderer )->DeInitialize();
}

#ifdef _LIB
#include ".\StaticLibSymbols.h"
INITDLLINFO DX9RENDERER_INITDLLINFO = {
	InitDLL, 
	GetDLLVersion, 
	GetPriority, 
	DeInitDLL,
	DX9RENDERER_LIBEXTERNS
};
#endif
