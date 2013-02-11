//=====================================================================
// File:		RenderViewer.cpp
// Description: Defines the initialization routines for the DLL.
// Started:		05/12/2005
// Original:	Brian Bazyk
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
//=====================================================================

#include "stdafx.h"
#include <afxdllx.h>
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BYTES_IN_MB 1048576
AFX_EXTENSION_MODULE RenderViewerDLL = { false, NULL, NULL };
const UINT GetStatusBarMessage = RegisterWindowMessage(GET_STATUS_BAR_MSG);

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("RenderViewer.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(RenderViewerDLL, hInstance))
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

		new CDynLinkLibrary(RenderViewerDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("RenderViewer.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(RenderViewerDLL);
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
	return SINGLETONINSTANCE(CRenderViewer);
}

//---------------------------------------------------------------------
// Function:	GetPriority
// Description:	DLL's priority
// Parameters:	.
// Returns:		DWORD = priority of DLL loaded
//---------------------------------------------------------------------
DWORD CRenderViewer::GetPriority()
{
    return IDR_RENDERVIEWERTYPE;
}

//---------------------------------------------------------------------
// Function:	CRenderViewer
// Description:	Standard Constructor
// Parameters:	pi = programs information structure
// Returns:		.
//---------------------------------------------------------------------
CRenderViewer::CRenderViewer()
{
	SINGLETONINSTANCE(CRenderViewerComponent)->SetParent(this);
	m_ToolBox = EngineGetToolBox();
	m_TickCount = 0;

	// full screen
	m_bFullScreen = false;
	m_uOldHeight = 0;
	m_uOldWidth = 0;
	m_pActiveView  = NULL;
	m_fCameraMoveSensitivity = 4.0f;
}

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CRenderViewer::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(RenderViewerDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	DLL's Initialization function, like InitInstance
// Parameters:	.
// Returns:		TRUE, if successful.
//---------------------------------------------------------------------
BOOL CRenderViewer::Initialize()
{
	CString tName;
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
	AfxSetResourceHandle(RenderViewerDLL.hModule);

	// use my string / icon resources
	// Note IDR_RENDERVIEWERTYPE has to be a unique id even between DLLS
	// to get the ICONS right on the Window
	m_DocTemplate = new CMultiDocTemplate(IDR_RENDERVIEWERTYPE,
			RUNTIME_CLASS(CRenderDoc),
			RUNTIME_CLASS(CRenderFrm),
			RUNTIME_CLASS(CRenderView));

	// put this after setting the menu and accelerators so it won't allocate it's own
	pApp->AddDocTemplate(m_DocTemplate);

	AfxSetResourceHandle(appInst);

    return TRUE;
}

//---------------------------------------------------------------------
// Function:	~CRenderViewer
// Description:	Standard Destructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CRenderViewer::~CRenderViewer()
{
}

//---------------------------------------------------------------------
// Function:	OnCreate
// Description:	Called from the main windows create function
// Parameters:	CMDIFrameWnd *mainWnd = main frames CMDIFrameWnd pointer
// Returns:		.
//---------------------------------------------------------------------
void CRenderViewer::OnCreate(CMDIFrameWnd *mainWnd)
{
	MENUINFOMESSAGE mim;
//	CBitmap menuBitmap;
	//HMENU tMenu;
	//HACCEL tAccel;
	HINSTANCE appInst;

	static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(RenderViewerDLL.hModule);

	//tMenu = LoadMenu(RenderViewerDLL.hModule, MAKEINTRESOURCE(IDR_RENDERVIEWERTYPE));
	//tAccel = LoadAccelerators(RenderViewerDLL.hModule, MAKEINTRESOURCE(IDR_RENDERVIEWERTYPE));
	//ASSERT(tMenu != NULL);
	//ASSERT(tAccel != NULL);
	
	VERIFY(MergeMenus(&m_DocTemplate->m_hMenuShared, mim.m_hMenu, _T("&Window")) );
	VERIFY(MergeAccelerators(&m_DocTemplate->m_hAccelTable, mim.m_hAccel));

	// we've been activated, so set our shared menu as active
	mim.m_hMenu = m_DocTemplate->m_hMenuShared;
	mim.m_hAccel = m_DocTemplate->m_hAccelTable;
	mim.m_MenuRsrc = NULL;
	mim.m_MenuBitmap = NULL;

	static DWORD msgHash_SetMenuInfo = CHashString(_T("SetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	// we need to mergeMenus
	//if (m_DocTemplate->m_hMenuShared)
	//	VERIFY(MergeMenus(&m_DocTemplate->m_hMenuShared, mim.m_hMenu, _T("&Window")));
	//else
	//	m_DocTemplate->m_hMenuShared = mim.m_hMenu;
	// and accelerators
	//VERIFY(MergeAccelerators(&m_DocTemplate->m_hAccelTable, mim.m_hAccel));

	AfxSetResourceHandle(appInst);
}

//---------------------------------------------------------------------
// Function:	OnCmdMsg
// Description:	Called from the main windows OnCmdMsg function.
//				Routes commands through doc/view architecture
// Returns:		TRUE if handled
//---------------------------------------------------------------------
BOOL CRenderViewer::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return FALSE;
}

//---------------------------------------------------------------------
// Function:	ViewDocument
// Description:	Edit an example Document retrieving initial data 
//					from a archive
// Parameters:	ar = archive to load into document
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CRenderViewer::ViewDocument()
{
	CRenderDoc *createDoc;
	// create blank document, with frame
	createDoc = DYNAMIC_DOWNCAST(CRenderDoc, m_DocTemplate->OpenDocumentFile(NULL));

	// load document?

	return NO_ERROR;
}


void CRenderViewer::OnUpdateTick(DWORD deltaTicks)
{
	// only update every 1/3 second
	m_TickCount += deltaTicks;
	if (m_TickCount > (1000/3))
	{
		m_TickCount = 0;
		DisplayRendererProfiling();
	}
}

void CRenderViewer::DisplayRendererProfiling()
{
	CGuiStatusBar * sBar;
	TCHAR buf[256];
	if( SendMessage( AfxGetApp()->GetMainWnd()->GetSafeHwnd(), GetStatusBarMessage, 0, (LPARAM)&sBar ) )
	{
		if(sBar)
		{
			float fps;
			static DWORD msgHash_GetCurrentFPS = CHashString(_T("GetCurrentFPS")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetCurrentFPS, sizeof(float), &fps);

			//Status bar
			sprintf( buf, "FPS: %3.1f", fps );
			sBar->SetPaneText( 2, buf );
		}
	}	
}

void CRenderViewer::ViewFullScreen(UINT nXRes, UINT nYRes)
{
	CRect rcDesktop;
	WINDOWPLACEMENT wpNew;

	CMDIFrameWnd *mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	
	if (mainWnd == NULL)
	{
		// error
		return;
	}

	CMDIChildWnd *childWnd = mainWnd->MDIGetActive();
	
	if (childWnd == NULL)
	{
		// error
		return;
	}

	if (!m_bFullScreen)
	{
		int style = GetWindowLong(childWnd->GetSafeHwnd(), GWL_STYLE);

		// save old window width and height
		RECT winRect;
		//GetClientRect(&winRect);
		mainWnd->GetClientRect(&winRect);
		m_uOldWidth =  (winRect.right - winRect.left);
		m_uOldHeight = (winRect.bottom - winRect.top);

		// send message to change the screen resolution
		VIEWPORTPARAMS svp;
		svp.height = nYRes;
		svp.width = nXRes;
		static DWORD msgHash_SetViewPort = CHashString(_T("SetViewPort")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_SetViewPort, sizeof(VIEWPORTPARAMS), &svp);
	
		// send message to switch Vision to full screen
		static DWORD msgHash_SetFullScreen = CHashString(_T("SetFullScreen")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_SetFullScreen, 0, NULL);
		
		CView *view = childWnd->GetActiveView();
		m_CurChild = childWnd;
		m_bFullScreen = true;
		if (m_CurChild != NULL)
		{
			m_CurChild->GetWindowPlacement(&m_wpPrev);
		
			::GetWindowRect(::GetDesktopWindow(), &rcDesktop);
		
			wpNew = m_wpPrev;
			wpNew.showCmd = SW_SHOWNORMAL;
			wpNew.rcNormalPosition = rcDesktop;
		
			m_ptMaxSize.x = rcDesktop.Width();
			m_ptMaxSize.y = rcDesktop.Height();
		
			m_oldStyles = m_CurChild->GetStyle();
			m_oldExStyles = m_CurChild->GetExStyle();
			m_OldParent = ::GetParent(m_CurChild->GetSafeHwnd());
			m_CurChild->ShowWindow(SW_HIDE);

			m_TempChild = mainWnd->CreateNewChild(m_CurChild->GetRuntimeClass(), IDR_DUMMY);
			m_TempChild->ShowWindow(SW_HIDE);
			mainWnd->MDIActivate(m_TempChild);

			HWND oldHWnd = m_CurChild->Detach();
			// create a new CWnd Object for full screen
			m_NewFullScreenWnd.Attach(oldHWnd);
			m_NewFullScreenWnd.ModifyStyle(WS_OVERLAPPEDWINDOW | WS_BORDER | WS_CHILD, 0);
			m_NewFullScreenWnd.ModifyStyleEx(WS_EX_WINDOWEDGE | WS_EX_MDICHILD, 0);
			::SetParent(m_NewFullScreenWnd.GetSafeHwnd(), GetDesktopWindow());
		
			// initialize input manager and setup keybindings
			m_NewFullScreenWnd.SetupInput();
		//}
		//if (m_CurChild != NULL)
		//{
			m_NewFullScreenWnd.SetWindowPlacement(&wpNew);
			m_NewFullScreenWnd.ActivateFrame(SW_SHOW);
			m_NewFullScreenWnd.SetActiveView( view );

			// save active view
			m_pActiveView = m_NewFullScreenWnd.GetActiveView();	
		}
	}
	else
	{		
		m_bFullScreen = false;

		// set the desired viewport
		VIEWPORTPARAMS svp;
		svp.height = m_uOldHeight; //nYRes;
		svp.width = m_uOldWidth; //nXRes;
		static DWORD msgHash_SetViewPort = CHashString(_T("SetViewPort")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_SetViewPort, sizeof(VIEWPORTPARAMS), &svp);
	
		// send message to switch Vision to full screen
		static DWORD msgHash_SetFullScreen = CHashString(_T("SetFullScreen")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_SetFullScreen, 0, NULL);

		if (m_CurChild != NULL)
		{
			m_NewFullScreenWnd.ShowWindow(SW_HIDE);
			::SetParent(m_NewFullScreenWnd.GetSafeHwnd(), m_OldParent);
			HWND oldHWnd = m_NewFullScreenWnd.Detach();
			m_CurChild->Attach(oldHWnd);

			m_CurChild->ModifyStyle(0, m_oldStyles);
			m_CurChild->ModifyStyleEx(0, m_oldExStyles);

			m_CurChild->SetWindowPlacement(&m_wpPrev);
			mainWnd->MDIActivate(m_CurChild);
			m_TempChild->DestroyWindow();

			//m_pActiveView = DYNAMIC_DOWNCAST(CRenderView, m_CurChild->GetActiveView());
			m_pActiveView = m_CurChild->GetActiveView();
		}
	}
}

void CRenderViewer::SetCameraMoveSensitivity( float f )
{
	m_fCameraMoveSensitivity = f;
	CRenderView *pView = GetActiveRenderView();
	if (pView)
		pView->m_ViewCamera.SetMoveSensitivity( f );
}


CView* CRenderViewer::GetActiveView()
{
	return m_pActiveView;
}

CRenderView* CRenderViewer::GetActiveRenderView()
{
	CMDIFrameWnd *mainFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	CMDIChildWnd *childFrame = mainFrame->MDIGetActive();
	return DYNAMIC_DOWNCAST(CRenderView, childFrame->GetActiveView());
}

///	\brief	Returns the first created RenderView
CRenderView *CRenderViewer::GetFirstView()
{
	POSITION posDoc = m_DocTemplate->GetFirstDocPosition();
	while (posDoc != NULL)
	{
		CDocument *pDoc = m_DocTemplate->GetNextDoc(posDoc);
		POSITION posView = pDoc->GetFirstViewPosition();
		if (posView != NULL)
		{
			return static_cast<CRenderView *>(pDoc->GetNextView(posView));
		}
	}
		 
	return NULL;	
}