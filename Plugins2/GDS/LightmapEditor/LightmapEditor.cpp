///============================================================================
/// \file		LightmapEditor.cpp
/// \brief		Implementation of Lightmap Editor
/// \date		09-07-2005
/// \author		Brian Bazyk
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
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
// major version in upper 16 bits
// low word =  minor version, low bit debug =0, release=1
#define INTERFACEDLLVERSION 0x00006100
#else
#define INTERFACEDLLVERSION 0x00006101
#endif

#define LIGHTMAPEDITOR_PRIORITY 19000

static AFX_EXTENSION_MODULE LightmapEditorDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("LightmapEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(LightmapEditorDLL, hInstance))
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

		new CDynLinkLibrary(LightmapEditorDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("LightmapEditor.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(LightmapEditorDLL);
	}
	return 1;   // ok
}

extern "C" DWORD GetDLLVersion()
{
	return INTERFACEDLLVERSION;
}

// Exported DLL initialization is run in context of running application
extern "C" IDLL *GetDLLInterface()
{
	// this has to be done after module is intialized.
	return SINGLETONINSTANCE(CLightmapEditor);
}

CLightmapEditor::CLightmapEditor()
{
	SINGLETONINSTANCE(CLightmapComponent)->SetParent(this);
	m_ToolBox = EngineGetToolBox();
	m_hAccel = NULL;
	m_hMenu = NULL;
}

CLightmapEditor::~CLightmapEditor()
{
	if (m_hAccel != NULL)
	{
		DestroyAcceleratorTable(m_hAccel);
		m_hAccel = NULL;
	}
	if (m_hMenu != NULL)
	{
		DestroyMenu(m_hMenu);
		m_hMenu = NULL;
	}
}

BOOL CLightmapEditor::Initialize()
{
	HINSTANCE appInst;

	// Register the doc templates we provide to the app
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);

    if (pApp == NULL)
    {
        return FALSE;
    }

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();

	// walk all templates in the application
	CMultiDocTemplate* pTemplate;
	pTemplate = (CMultiDocTemplate *)FindDocumentTemplate(IDR_RENDERVIEWERTYPE);
	assert(pTemplate != NULL);

	// now tell the application to look in dll for resources
	AfxSetResourceHandle(LightmapEditorDLL.hModule);

	// merge our menu/accelerator into the renderviewer menu
	m_hMenu = LoadMenu(LightmapEditorDLL.hModule, MAKEINTRESOURCE(IDR_RENDERLIGHTMAPVIEWER));
	// we have no accelarators for this
	m_hAccel = LoadAccelerators(LightmapEditorDLL.hModule, MAKEINTRESOURCE(IDR_RENDERLIGHTMAPVIEWER));

	if (m_hMenu)
	{
		VERIFY(MergeSubMenus(&pTemplate->m_hMenuShared, m_hMenu));
	}
	if (m_hAccel)
	{
		VERIFY(MergeAccelerators(&pTemplate->m_hAccelTable, m_hAccel));
	}

	AfxSetResourceHandle(appInst);


	return TRUE;
}

// our onCreate needs to catch the 
void CLightmapEditor::OnCreate(CMDIFrameWnd *mainWnd)
{
}

BOOL CLightmapEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return m_LightmapCmdTarget.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

CString* CLightmapEditor::GetName( void )
{
	static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(LightmapEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

DWORD CLightmapEditor::GetPriority()
{
	return LIGHTMAPEDITOR_PRIORITY;
}
