///============================================================================
/// \file		EffectEditor.cpp
/// \brief		Implementation of Effect Editor
/// \date		01-30-2007
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
#include "EffectEditor.h"
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

#define EFFECTEDITOR_PRIORITY 21000

static AFX_EXTENSION_MODULE EffectEditorDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("EffectEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(EffectEditorDLL, hInstance))
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

		new CDynLinkLibrary(EffectEditorDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("EffectEditor.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(EffectEditorDLL);
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
	return SINGLETONINSTANCE(CEffectEditor);
}

CEffectEditor::CEffectEditor()
{
	SINGLETONINSTANCE(CEffectEditorComponent)->SetParent(this);
	m_ToolBox = EngineGetToolBox();
	m_hAccel = NULL;
	m_hMenu = NULL;
	m_pMainWnd = NULL;
	m_DocTemplate = NULL;
}

CEffectEditor::~CEffectEditor()
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

BOOL CEffectEditor::Initialize()
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
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(EffectEditorDLL.hModule);

	m_DocTemplate = new CMultiDocTemplate(IDR_EFFECTEDITORTYPE,
			RUNTIME_CLASS(CEffectEditorDoc),
			RUNTIME_CLASS(CMDIChildWnd),
			RUNTIME_CLASS(CEffectEditorView));

	pApp->AddDocTemplate(m_DocTemplate);

	AfxSetResourceHandle(appInst);

	return TRUE;
}

// our onCreate needs to catch the 
void CEffectEditor::OnCreate(CMDIFrameWnd *mainWnd)
{
	m_pMainWnd = mainWnd;

	// Menus/Accelerators
	MENUINFOMESSAGE mim;
	static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);
	// merge our menu/accelerator into the main menu
	m_hMenu = LoadMenu(EffectEditorDLL.hModule, MAKEINTRESOURCE(ID_MENU_EFFECTEDITOR));
	m_hAccel = LoadAccelerators(EffectEditorDLL.hModule, MAKEINTRESOURCE(ID_MENU_EFFECTEDITOR));
	VERIFY(MergeSubMenus(&m_hMenu, mim.m_hMenu));
	VERIFY(MergeAccelerators(&m_hAccel, mim.m_hAccel));
	mim.m_hMenu = m_hMenu;
	mim.m_hAccel = m_hAccel;
	mim.m_MenuRsrc = NULL;
	mim.m_MenuBitmap = NULL;
	static DWORD msgHash_SetMenuInfo = CHashString(_T("SetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	// Dock Win
	if (m_EffectEditorDockWin.Create(_T("Effect Properties"), WS_CHILD | WS_VISIBLE, mainWnd, IDD_EFFECTEDITOR_DCKDLG))
	{
		CRect winRect;
		mainWnd->GetWindowRect(&winRect);
		m_EffectEditorDockWin.EnableDocking(CBRS_ALIGN_ANY);
		m_EffectEditorDockWin.SetBarStyle(m_EffectEditorDockWin.GetBarStyle() |
			CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		mainWnd->DockControlBar(&m_EffectEditorDockWin, AFX_IDW_DOCKBAR_RIGHT, winRect);
		m_EffectEditorDockWin.LockPushPin(FALSE);
	}
}

BOOL CEffectEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (m_EffectEditorCmdTarget.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	if (m_EffectEditorDockWin.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	return FALSE;
}

CString* CEffectEditor::GetName( void )
{
	static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(EffectEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

DWORD CEffectEditor::GetPriority()
{
	return EFFECTEDITOR_PRIORITY;
}

void CEffectEditor::DisplayToolbar(bool bDisplay)
{
	if (bDisplay)
	{
		if (m_EffectEditorToolbar.m_hWnd == NULL)
		{
			if (!m_EffectEditorToolbar.Create(m_pMainWnd, CEffectEditorToolbar::IDD,
				CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY, CEffectEditorToolbar::IDD))
			{
				TRACE0("Failed to create dockbar\n");
				return;      // fail to create
			}

			TOOLBARSTRUCT atbs;
			atbs.hIcon = (HICON)LoadImage(EffectEditorDLL.hModule, 
				MAKEINTRESOURCE(IDR_EFFECTEDITORICON), IMAGE_ICON, 0, 0, 0);
			atbs.Text = _T("Effect Editor");
			atbs.cBar = &m_EffectEditorToolbar;
			static DWORD msgHash_AddToolBar = CHashString(_T("AddToolBar")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_AddToolBar, sizeof(TOOLBARSTRUCT), &atbs);
		}
	}
	else
	{
		// TODO: cleanup the toolbar somehow.  This isn't possible in guilib?
		m_EffectEditorToolbar.ShowWindow( SW_HIDE );
//		m_EffectEditorToolbar.DestroyWindow();
	}
}

void CEffectEditor::StartEditor()
{
	// create blank document, with frame
	CEffectEditorDoc *createDoc = DYNAMIC_DOWNCAST(CEffectEditorDoc, m_DocTemplate->OpenDocumentFile(NULL));

	DisplayToolbar(true);
}

void CEffectEditor::StopEditor()
{
	m_DocTemplate->CloseAllDocuments(FALSE);

	DisplayToolbar(false);
}

CEffectEditorView* CEffectEditor::GetActiveView()
{
	CMDIFrameWnd *mainFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	CMDIChildWnd *childFrame = mainFrame->MDIGetActive();
	return DYNAMIC_DOWNCAST(CEffectEditorView, childFrame->GetActiveView());
}
