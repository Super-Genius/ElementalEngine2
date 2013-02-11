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
#include "MainFrm.h"
#include "GeneralOptions.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct tbblabel
{
  UINT id;
  char *label;
};

static tbblabel standardBarLabels[] = 
{
  { ID_FILE_NEW, "File New" },
  { ID_FILE_OPEN, "File Open" },
  { ID_FILE_SAVE, "File Save" },
  { ID_EDIT_CUT, "Cut" },
  { ID_EDIT_COPY, "Copy" },
  { ID_EDIT_PASTE, "Paste" },
  { ID_FILE_PRINT, "Print" },
  { ID_APP_ABOUT, "About" },
  { ID_CONTEXT_HELP, "What Is?" },
  { ID_HELP_FINDER, "Help" }
};

const UINT GetStatusBarMessage = RegisterWindowMessage(GET_STATUS_BAR_MSG);
UINT AddOptionPageMessage = RegisterWindowMessage(ADD_OPTION_PAGE_MSG);

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CGuiMDIFrame)

BEGIN_MESSAGE_MAP(CMainFrame, CGuiMDIFrame)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_OBJECTS, OnViewObjects)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OBJECTS, OnUpdateViewObjects)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_OPTIONS, OnUpdateToolsOptions)
	ON_COMMAND(ID_FILE_NEW, OnFileNewFile)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
	ON_COMMAND(ID_HELP, CGuiMDIFrame::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CGuiMDIFrame::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CGuiMDIFrame::OnHelpFinder)
	ON_REGISTERED_MESSAGE(GetStatusBarMessage, OnGetStatusBar)
	ON_REGISTERED_MESSAGE(AddOptionPageMessage, OnAddOptionPageMessage)
	ON_COMMAND(ID_TOOLS_PAUSESCENE, OnToolsPausescene)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PAUSESCENE, OnUpdateToolsPausescene)
	ON_COMMAND(ID_TOOLS_SHOWPROFILER, OnToolsShowprofiler)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SHOWPROFILER, OnUpdateToolsShowprofiler)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_0,
	ID_INDICATOR_1,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_OptDialog = NULL;
	m_GeneralOptPage = NULL;
	m_bProfilerVisible = false;
}

CMainFrame::~CMainFrame()
{
	m_bmToolbarHi.DeleteObject();
	if (m_GeneralOptPage !=	NULL)
	{
		m_GeneralOptPage->DestroyWindow();
		delete m_GeneralOptPage;
		m_GeneralOptPage = NULL;
	}
	if (m_OptDialog != NULL)
	{
		m_OptDialog->DestroyWindow();
		delete m_OptDialog;
		m_OptDialog = NULL;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int cButtons;
	CRect rect;
	CDockState dockState;
	CImageList imageList;
	CBitmap bitmap;
	CDC *tempDC;

	m_iTimer = 0;

	// initialize with scene paused
	m_bScenePaused = true;
	PauseScene( true );

	if (CGuiMDIFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndProgStatusBar.Create(this) || 
		!m_wndProgStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create progress status bar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.CreateEx(this, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_SIZE_DYNAMIC );
	m_wndMenuBar.LoadMenuBar(IDR_MAINFRAME);


	m_NewMenu.SetMenuDrawMode(CNewMenu::STYLE_XP);
	m_NewMenu.LoadMenu(IDR_MAINFRAME);

	m_OptDialog = new COptionsDialog();

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME_LARGE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_TabbedToolBar.Create(this))
	{
		TRACE0("Failed to create tabbed toolbar\n");
		return -1;      // fail to create
	}

	m_TabbedToolBar.EnableDocking(CBRS_ALIGN_ANY);
	
	EnableDocking(CBRS_ALIGN_ANY);

	CToolBarCtrl& tbCtrl = m_wndToolBar.GetToolBarCtrl();

	// get the main screen's handle
	tempDC = GetDesktopWindow()->GetDC();
	int nPlanes;
	int bpp;
	bpp = tempDC->GetDeviceCaps(BITSPIXEL);
	nPlanes = tempDC->GetDeviceCaps(PLANES);
	GetDesktopWindow()->ReleaseDC(tempDC);
	BOOL isHiColor;

	isHiColor = (bpp*nPlanes) > 8;
	int nCount = tbCtrl.GetImageList()->GetImageCount();
	if (isHiColor)
	{

		IMAGEINFO ii;
		tbCtrl.GetImageList()->GetImageInfo(0, &ii);
		int cx = ii.rcImage.right - ii.rcImage.left;
		int cy = ii.rcImage.bottom - ii.rcImage.top;

		bitmap.LoadBitmap(IDB_TOOLBAR_LARGE);
		imageList.Create(cx, cy, ILC_COLORDDB|ILC_MASK, nCount, 0);
		imageList.Add(&bitmap, RGB(192,192,192));
		 // attach the image lists to the toolbar control
		tbCtrl.SetImageList (&imageList);
		imageList.Detach();
		bitmap.Detach();

		
		// now the disabled one
		bitmap.LoadBitmap(IDB_TOOLBAR_LARGE_DISABLED);
		imageList.Create(cx, cy, ILC_COLORDDB|ILC_MASK, nCount, 0);
		imageList.Add(&bitmap, RGB(192,192,192));
		 // attach the image lists to the toolbar control
		tbCtrl.SetDisabledImageList(&imageList);
		imageList.Detach();
		bitmap.Detach();

	}

	cButtons = sizeof(standardBarLabels) / sizeof(standardBarLabels[0]);
	for( int nButton = 0; nButton < cButtons; nButton++ )
	{
		int nButtonIndex = m_wndToolBar.CommandToIndex( standardBarLabels[nButton].id );
		if (!m_wndToolBar.SetButtonText( nButtonIndex, standardBarLabels[nButton].label ))
			TRACE("failed SetButtonText(%d, \"%s\").\n",
				   nButtonIndex, standardBarLabels[nButton].label);
	}

	m_wndToolBar.SetWindowText(_T("Menu Toolbar"));

	// Get the first button size after adding the labels.
	// resize rectangle buttons to fit text
	m_wndToolBar.GetItemRect(0, rect);
	m_wndToolBar.SetSizes(rect.Size(), CSize(24, 24));

	m_TabbedToolBar.AddBar(&m_wndToolBar, _T("Main"));
	DockControlBar(&m_TabbedToolBar);

	bitmap.LoadBitmap(IDB_TOOLBAR);
	imageList.Create(16, 15, ILC_COLORDDB|ILC_MASK, nCount, 0);
	imageList.Add(&bitmap, RGB(192,192,192));

	m_wndCool.Install(this);
	// we want a hicolor bitmap for our menus
	CBitmap frameBMP;
	frameBMP.LoadBitmap(IDB_TOOLBAR);
	m_wndCool.LoadToolbar(IDR_MAINFRAME, &frameBMP);
//	m_NewMenu.LoadToolBar(tbCtrl, &imageList);

	// reset menu as plugins could have changed it.
//	SetMenu(m_hMenuDefault);

	m_GeneralOptPage = new CGeneralOptions;

	m_OptDialog->AddPage( m_GeneralOptPage );

	// modeless options dialog, not visible
	if (!m_OptDialog->Create(this, WS_POPUP | WS_CAPTION | WS_SYSMENU))
	{
		// no pages causes creation to fail
		delete m_OptDialog;
		m_OptDialog = NULL;
	}

	// preload menu and accelerators
	m_hMainMenu = LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	m_hMainAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

	// this is bogus and should be at the top, but then tabs are above docked
	// elements.
	InitMDITabbed();
	m_MdiTabbed.SetTabsMenu(IDR_MENUTABS);
	m_MdiTabbed.SetTabMenu(0,0);

	// send out message that main window has been created.
	static DWORD msgHash_WindowsMainCreate = CHashString(_T("WindowsMainCreate")).GetUniqueID();
	theApp.GetToolBox()->SendMessage(msgHash_WindowsMainCreate, sizeof(CMDIFrameWnd *), this);

	m_wndMenuBar.StyleDispl(GUISTYLE_XP);
	m_NewMenu.StyleDispl(GUISTYLE_XP);
	m_wndToolBar.StyleDispl(GUISTYLE_XP);
	StyleDispl(GUISTYLE_XP);

	//// resource bar
	if (!m_ResourceBar.Create(_T("Resource"), WS_CHILD | WS_VISIBLE, this, 
		IDD_OBJECTPANE))
	{
		TRACE0("Failed to create resource dialog bar\n");
		return -1;      
	} 
	
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.top = 0;
	clientRect.bottom = 256;
	m_ResourceBar.MoveWindow(&clientRect);

	m_ResourceBar.SetBarStyle(m_ResourceBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_ResourceBar.EnableDocking(CBRS_ALIGN_ANY);
	
	DockControlBar(&m_ResourceBar, AFX_IDW_DOCKBAR_BOTTOM);
	//// end resource bar

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CGuiMDIFrame::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CGuiMDIFrame::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CGuiMDIFrame::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	STDDLLLIST::iterator DLLit;
//    IDLL *di;

	CMDIChildWnd* pActiveChild = MDIGetActive();
	// pump through active child FIRST
	if (pActiveChild != NULL)
	{
		CPushRoutingFrame push(this);
		if (pActiveChild->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

	CMDMESSAGE cmdMsg;
	cmdMsg.m_nID = nID;
	cmdMsg.m_nCode = nCode;
	cmdMsg.m_pExtra = pExtra;
	cmdMsg.m_pHandlerInfo = pHandlerInfo;
	cmdMsg.retVal = false;

	// try pumping through DLLS
	static DWORD msgHash_WindowsCmdMsg = CHashString(_T("WindowsCmdMsg")).GetUniqueID();
	theApp.GetToolBox()->SendMessage(msgHash_WindowsCmdMsg, sizeof(CMDMESSAGE), &cmdMsg);

	if (cmdMsg.retVal)
		return TRUE;

	// now try the resource manager
	if (m_ResourceBar.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// then pump through normal frame
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CMainFrame::BroadcastMessage(UINT msg, WPARAM wParam, LPARAM lParam, HWND tWnd)
{
	BOOL topLevel;

	topLevel = FALSE;
	if (tWnd == NULL)
	{
		tWnd = m_hWnd;
		topLevel = TRUE;
	}

	// walk through HWNDs to avoid creating temporary CWnd objects
	// unless we need to call this function recursively
	for (HWND hWndChild = ::GetTopWindow(tWnd); hWndChild != NULL;
		hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
	{
		CWnd* pWnd = CWnd::FromHandlePermanent(hWndChild);
		if (pWnd != NULL)
		{
			// call window proc directly since it is a C++ window
			if (AfxCallWndProc(pWnd, pWnd->m_hWnd, msg, wParam, lParam)) 
			{
				return TRUE;
			}
		}
		else
		{
			// send message with Windows SendMessage API
			if (::SendMessage(hWndChild, msg, wParam, lParam))
				return TRUE;
		}
		if (::GetTopWindow(hWndChild) != NULL)
		{
			// send to child windows after parent
			if (BroadcastMessage(msg, wParam, lParam, hWndChild))
				return TRUE;
		}

	}

	if (topLevel)
		return SendMessage(msg, wParam, lParam);
	else
		return FALSE;
}

void CMainFrame::OnClose() 
{
	// send Application close event message to get the necessary
	// confirmations for closing the objects. 
	// ProjectSettings asks for confirmation and returns bool
	CHashString hszProjectEditorComponent(_T("CProjectEditorComponent"));
	static DWORD msgHash_GdsClose = CHashString(_T("GdsClose")).GetUniqueID();
	bool initBoolVariable = false;
	bool* doClose = &initBoolVariable; 
	bool successfulMessageHandling = MSG_HANDLED == theApp.GetToolBox()->SendMessage(msgHash_GdsClose, sizeof( bool* ), doClose, NULL, &hszProjectEditorComponent );


	// check for message errors
	if ( !successfulMessageHandling )
	{
		// TODO: Log the error some way:
		theApp.GetLogger()->Print(_T("Getting Project Close confrimation failed. Project will be closed."));
		*doClose = TRUE;
	}

	// handle the result
	if ( *doClose )
	{
		if (m_OptDialog && m_OptDialog->IsWindowVisible())
		{
			m_OptDialog->SendMessage(WM_COMMAND, IDOK);
			if (m_OptDialog->IsWindowVisible())
				return;
		}

		CString sProfile = _T("BarState");
	// this needs to be fixed as some bar's in DLL's are saved and not
	// present when gds reloads.
	//	SaveBarState(sProfile);

		CGuiMDIFrame::OnClose();
	}
}

void CMainFrame::OnViewObjects() 
{
	if ( m_ResourceBar.IsVisible() || m_ResourceBar.IsMinimized() )
	{
		m_ResourceBar.Close();
	}
	else
	{
		// GuiControlBar::Show redocks the controlbar if it
		// was docked or minimized before hiding.
		m_ResourceBar.Show();
	}

	//The original version does not turn off the menuitem, it is in minimized state
	/*if (m_ResourceBar.IsVisible() )
	{
		ShowControlBar(&m_ResourceBar, FALSE, FALSE);
	}
	else
	{
		ShowControlBar(&m_ResourceBar, TRUE, FALSE);
	}*/
}

void CMainFrame::OnUpdateViewObjects(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_ResourceBar.IsVisible() || m_ResourceBar.IsMinimized());	
}

LRESULT CMainFrame::OnGetStatusBar(WPARAM wParam, LPARAM lParam)
{
	CGuiStatusBar **statBar;

	statBar = (CGuiStatusBar **)lParam;
	*statBar = &m_wndProgStatusBar;

	return TRUE;
}

void CMainFrame::OnToolsOptions() 
{

	// only if the window is invisible
	if (m_OptDialog && !m_OptDialog->IsWindowVisible())
	{
		// Here we assume, that the first page is the generaloptions page
		CGeneralOptions* optionsPage = STATIC_DOWNCAST( CGeneralOptions, m_OptDialog->GetPage( 0 ) ); 
		
		if( optionsPage != NULL )
		{
			// Call the update function, to refresh the values on the page
			optionsPage->updateInformation();
		}
		else
		{
			EngineGetToolBox()->Log(LOGERROR, "Updating generalOptions page of the propetySheet failed.");
		}

		// let's make the propertysheet visible
		m_OptDialog->ShowWindow(SW_SHOW);
	}
}

void CMainFrame::OnUpdateToolsOptions(CCmdUI* pCmdUI) 
{
	// m_OptDialog->m_hWnd needs to be not null as well.
	if (m_OptDialog && m_OptDialog->m_hWnd)
	{
		pCmdUI->Enable(!m_OptDialog->IsWindowVisible());	
	}
	else
	{
		// disable the button on any error which makes the if 
		// constraint false
		pCmdUI->Enable(FALSE);
	}
}

LRESULT CMainFrame::OnAddOptionPageMessage(WPARAM wParam, LPARAM lParam)
{
	if (m_OptDialog != NULL)
	{
		CPropertyPage *tProp;

		tProp = (CPropertyPage *)lParam;
		VERIFY(tProp->IsKindOf(RUNTIME_CLASS(CPropertyPage)));

		m_OptDialog->AddPage(tProp);
	}

	return TRUE;
}

void CMainFrame::OnFileNewFile() 
{
	// TODO: Add your command handler code here
	//Open a dialog showing available new files

	if( !m_NewObjectDialog.GetSafeHwnd() )
	{
		m_NewObjectDialog.Create(IDD_NEWFILE_DIALOG, this);
	}
	else
	{
		m_NewObjectDialog.ShowWindow( SW_SHOW );
		m_NewObjectDialog.BringWindowToTop();
	}

	m_NewObjectDialog.Initialize( m_FileTypeCreationList );
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
	if (!CGuiMDIFrame::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
		return FALSE;

	// put in our new menus/accelerators
	FreeResource(m_hAccelTable);
	m_hAccelTable = m_hMainAccel;
	m_hMenuDefault = m_hMainMenu;
	m_wndMenuBar.LoadMenu(m_hMainMenu, NULL);

	return TRUE;

}

void CMainFrame::LoadMenuToolBar(UINT rsrc, CBitmap *bm)
{
	m_wndCool.LoadToolbar(rsrc, bm);
}

bool CMainFrame::OnRegisterFileTypeCreationInfo( FILETYPECREATIONINFO * pInfo )
{
	//check valid
	bool bFound;
	if( pInfo 
		&& pInfo->m_CreationCallback != NULL )
	{
		//find in list
		bFound = false;
		list< FILETYPECREATIONINFO >::iterator iter = m_FileTypeCreationList.begin();
		for( ; iter != m_FileTypeCreationList.end(); iter++ )
		{
			FILETYPECREATIONINFO &info = (*iter);
			if( info.m_Description == pInfo->m_Description )
			{
				//match change properties
				bFound = true;
				info = *pInfo;
				break;
			}
		}
		//if not found create a new one
		if( !bFound )
		{	
			m_FileTypeCreationList.push_back( (*pInfo) );
		}
        return true;
	}
	return false;

}
void CMainFrame::AddToolBar(TOOLBARSTRUCT *atbs)
{
	m_TabbedToolBar.AddBar(atbs->cBar, atbs->Text, atbs->hIcon);
}

void CMainFrame::IsBarActive( TOOLBARACTIVESTRUCT *atbs )
{
	atbs->bActive = m_TabbedToolBar.IsBarActive( atbs->cBar );
}

void CMainFrame::SetActiveBar(TOOLBARACTIVESTRUCT *atbs)
{
	m_TabbedToolBar.SetActiveBar(atbs->cBar);
}

void CMainFrame::HideBar(CControlBar *cBar)
{
	m_TabbedToolBar.HideBar(cBar);
}


///////////////////
// Handle progress message—any object can call send one.
//
void CMainFrame::UpdateProgress(DWORD percentage)
{
   m_wndProgStatusBar.OnProgress(percentage); // pass to prog/status bar

#if 0
   // timer
   // don't update on every node
   if (m_iTimer > GetTickCount())
   {
	   return;
   }

	m_iTimer = GetTickCount() + 250; // update every N ms
#endif

	//RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN);
//	CWnd *mainWnd = AfxGetMainWnd();

	MSG msg;
	while (PeekMessage( &msg, GetSafeHwnd(), 0, 0, PM_REMOVE ))
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

//	mainWnd->SendMessage(WM_IDLE);
}

void CMainFrame::OnToolsPausescene()
{
	PauseScene( !m_bScenePaused );
}

void CMainFrame::OnUpdateToolsPausescene(CCmdUI *pCmdUI)
{
	if (m_bScenePaused)
	{
		pCmdUI->SetText( _T("Start Scene") );
	}
	else
	{
		pCmdUI->SetText( _T("Stop Scene") );
	}
}

void CMainFrame::PauseScene(bool bScenePaused)
{
	m_bScenePaused = bScenePaused;

	CHashString hszStateMachineManager(_T("CQHStateMachineManager"));
	CHashString hszOpenALSoundManager(_T("COpenALSoundManager"));
	CHashString hszPhysicsManager(_T("CPhysicsManager"));
	if (m_bScenePaused)
	{		
		static DWORD msgHash_Stop = CHashString(_T("Stop")).GetUniqueID();
		theApp.GetToolBox()->SendMessage(msgHash_Stop, 0, NULL, NULL, &hszStateMachineManager );
		theApp.GetToolBox()->SendMessage(msgHash_Stop, 0, NULL, NULL, &hszOpenALSoundManager );
		//theApp.GetToolBox()->SendMessage(msgHash_Stop, 0, NULL, NULL, &hszPhysicsManager );
		static DWORD msgHash_ForceKinematic = CHashString(_T("ForceKinematic")).GetUniqueID();
		theApp.GetToolBox()->SendMessage(msgHash_ForceKinematic, sizeof(bool), &bScenePaused, NULL, &hszPhysicsManager );
	}
	else
	{
		static DWORD msgHash_Start = CHashString(_T("Start")).GetUniqueID();
		theApp.GetToolBox()->SendMessage(msgHash_Start, 0, NULL, NULL, &hszStateMachineManager );
		theApp.GetToolBox()->SendMessage(msgHash_Start, 0, NULL, NULL, &hszOpenALSoundManager );
		//theApp.GetToolBox()->SendMessage(msgHash_Start, 0, NULL, NULL, &hszPhysicsManager );
		static DWORD msgHash_ForceKinematic = CHashString(_T("ForceKinematic")).GetUniqueID();
		theApp.GetToolBox()->SendMessage(msgHash_ForceKinematic, sizeof(bool), &bScenePaused, NULL, &hszPhysicsManager );
	}
}
void CMainFrame::OnToolsShowprofiler()
{
	m_bProfilerVisible = !m_bProfilerVisible;

	static DWORD msgShowHideProfilerWindow = CHashString(_T("ShowHideProfilerWindow")).GetUniqueID();
	theApp.GetToolBox()->SendMessage(msgShowHideProfilerWindow, sizeof(bool), &m_bProfilerVisible);	
}

void CMainFrame::OnUpdateToolsShowprofiler(CCmdUI *pCmdUI)
{
	if (m_bProfilerVisible)
	{
		pCmdUI->SetText( _T("Hide Profiler") );
	}
	else
	{
		pCmdUI->SetText( _T("Show Profiler") );
	}
}

void CMainFrame::OnHelpFinder()
{
	DISPLAYHELPTOPICPARAMS dht;
	dht.chmName = _T("gds");
	dht.topicPath = _T("/topics/default.html");
	static DWORD msgHash_TestHelpSystemDialog = CHashString(_T("DisplayHelpTopic")).GetUniqueID();
	theApp.GetToolBox()->SendMessage(msgHash_TestHelpSystemDialog, sizeof(DISPLAYHELPTOPICPARAMS), &dht);
}

/// \brief	Get currently selected view
/// \return	pointer to a currently selected CView
const CView *CMainFrame::GetActiveView() const
{
	CFrameWnd *pFrame = DYNAMIC_DOWNCAST(CFrameWnd, m_MdiTabbed.GetActiveWnd());
	return pFrame->GetActiveView();
}

/// \brief	Select passed view as active
/// \param	pView - pointer to a CView to select
void CMainFrame::SetActiveView(CView *pView)
{
	ASSERT(pView != NULL);
	CWnd *pFrame = pView->GetParentFrame();
	for (int i = 0; i < m_MdiTabbed.GetCount(); ++i)
	{
		if (pFrame == m_MdiTabbed.GetNumWnd(i))
		{
			m_MdiTabbed.SetCurtab(i);
			return;
		}
	}
	ASSERT(!"Passed view does not exist");
}
