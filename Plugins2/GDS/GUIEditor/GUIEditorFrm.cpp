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
#include "GUIEditorFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NUMICONS 4
#define NICONSWIDE 4

/////////////////////////////////////////////////////////////////////////////
// CGUIEditorFrm

IMPLEMENT_DYNCREATE(CGUIEditorFrm, CMDIChildWnd)

CGUIEditorFrm::CGUIEditorFrm()
{
	m_OptionTree = new COptionTreeDock(_T("GUIObjects.xgf"), _T("\\guiproperties\\"));
	m_ToolsBar = NULL;
	m_AnimViewBar = NULL;
	m_GUIDataBar = NULL;
}

CGUIEditorFrm::~CGUIEditorFrm()
{
	delete m_OptionTree;
	if (m_ToolsBar)
	{
		delete m_ToolsBar;
	}
	if (m_AnimViewBar)
	{
		delete m_AnimViewBar;
	}
	if (m_GUIDataBar)
	{
		delete m_GUIDataBar;
	}
}


BEGIN_MESSAGE_MAP(CGUIEditorFrm, CMDIChildWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_GUIEDITOR_LANGUAGES, &CGUIEditorFrm::OnGuieditorLanguages)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUIEditorFrm message handlers

int CGUIEditorFrm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CMDIFrameWnd *mainWnd;
	RECT rectMain;
		
	// Register the doc templates we provide to the app
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);	

	// get the apps instance handle for loading resources
	m_AppInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(GUIEditorDLL.hModule);

	mainWnd->GetClientRect(&rectMain);

	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_ToolsBar = new CGenericDockBar(&m_Tools, IDD_GUITOOLS, CHashString(_T("GUI Objects")));
	if (!m_ToolsBar->Create(_T("Tools"), WS_CHILD , mainWnd, 
		IDD_TOOLS_DOCK))
	{
		TRACE0("Failed to create toolbar\n");
        //m_pLogger->Print(_T("Failed to create scene dockable window bar"));
        return -1;
    }

    CRect winRect;
	mainWnd->GetWindowRect(&winRect);
	m_ToolsBar->EnableDocking(CBRS_ALIGN_ANY);
	m_ToolsBar->SetBarStyle(m_ToolsBar->GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_FLOAT_MULTI);

	mainWnd->DockControlBar(m_ToolsBar, AFX_IDW_DOCKBAR_LEFT, winRect);	

	m_AnimViewBar = new CGenericDockBar(&m_AnimView, IDD_ANIMVIEW, CHashString(_T("Animation View")));
	if (!m_AnimViewBar->Create(_T("Animations"), WS_CHILD , mainWnd, 
		IDD_ANIM_DOCK))
	{
		TRACE0("Failed to create toolbar\n");
        //m_pLogger->Print(_T("Failed to create scene dockable window bar"));
        return -1;
    }

    m_AnimViewBar->EnableDocking(CBRS_ALIGN_ANY);
	m_AnimViewBar->SetBarStyle(m_AnimViewBar->GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_FLOAT_MULTI);

	mainWnd->DockControlBar(m_AnimViewBar, AFX_IDW_DOCKBAR_BOTTOM);	

	m_AnimViewBar->LockPushPin(FALSE);	

	if(!m_OptionTree->Create(_T("Item Properties"), WS_CHILD | WS_VISIBLE, mainWnd, IDD_GUIPROPERTIES))
	{
		//m_Logger->Print(_T("Failed to create option tree view dockable window bar"));
        return -1;
	}	

	mainWnd->GetWindowRect(&winRect);
	m_OptionTree->EnableDocking(CBRS_ALIGN_ANY);
	m_OptionTree->SetBarStyle(m_OptionTree->GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	mainWnd->DockControlBar(m_OptionTree, AFX_IDW_DOCKBAR_RIGHT, winRect);

	m_GUIDataBar = new CGenericDockBar(&m_GUIDataDlg, IDD_GUIEDDATA, CHashString(_T("GUI Data")));
	if (!m_GUIDataBar->Create(_T("Tools"), WS_CHILD , mainWnd, 
		IDD_GUIEDDATADOCK))
	{
		TRACE0("Failed to create toolbar\n");
        //m_pLogger->Print(_T("Failed to create scene dockable window bar"));
        return -1;
    }

    mainWnd->GetWindowRect(&winRect);
	m_GUIDataBar->EnableDocking(CBRS_ALIGN_ANY);
	m_GUIDataBar->SetBarStyle(m_ToolsBar->GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_FLOAT_MULTI);

	mainWnd->DockControlBar(m_GUIDataBar, AFX_IDW_DOCKBAR_RIGHT, winRect);	

	m_GUIDataBar->LockPushPin(FALSE);

	AfxSetResourceHandle(m_AppInst);

	m_LangView.Create(IDD_LANGUAGEVIEW, mainWnd);
	
	return 0;
}

LRESULT CGUIEditorFrm::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// otherwise forward message to child
	return CMDIChildWnd::WindowProc(message, wParam, lParam);
}
void CGUIEditorFrm::OnGuieditorLanguages()
{
	m_LangView.ShowWindow(SW_SHOWNORMAL);
	m_LangView.SetActiveWindow();
	m_LangView.BringWindowToTop();
}
