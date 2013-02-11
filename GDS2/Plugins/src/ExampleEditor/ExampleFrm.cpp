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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NUMICONS 4
#define NICONSWIDE 4

/////////////////////////////////////////////////////////////////////////////
// CExampleFrm

IMPLEMENT_DYNCREATE(CExampleFrm, CMDIChildWnd)

CExampleFrm::CExampleFrm()
{
}

CExampleFrm::~CExampleFrm()
{
}


BEGIN_MESSAGE_MAP(CExampleFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CExampleFrm)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExampleFrm message handlers

int CExampleFrm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
	AfxSetResourceHandle(ExampleEditorDLL.hModule);

	mainWnd->GetClientRect(&rectMain);

	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_ExampleToolPal.CreateEx(mainWnd, TBSTYLE_FLAT | TBSTYLE_WRAPABLE , WS_CHILD | WS_VISIBLE | CBRS_RIGHT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED, CRect(0, 2, 2, 4)) ||
		!m_ExampleToolPal.LoadToolBar(IDR_EXAMPLE_TOOL_PALETTE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_ExampleToolPal.SetBorders(2, 2, 2, 2);

	m_ExampleToolPal.EnableDocking(CBRS_ORIENT_ANY);
	// make control bar notify us.

	mainWnd->DockControlBar(&m_ExampleToolPal);

	CRect tcRect;
	m_ExampleToolPal.GetToolBarCtrl().SetRows(1, FALSE, tcRect);
	int iconWidth;
	int iconHeight;
	int nRows;

	iconWidth = tcRect.Width()/NUMICONS;
	iconHeight = tcRect.Height();
	nRows = (NUMICONS / NICONSWIDE) + (((NUMICONS % NICONSWIDE) == 0) ? 0 : 1);

	tcRect.SetRect(0, 0, iconWidth * NICONSWIDE, (iconHeight * nRows));

	// Set the window size
	m_ExampleToolPal.SetWindowPos(NULL, 0, 0, tcRect.Width(), tcRect.Height(),
		SWP_NOACTIVATE  | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREDRAW);

	
	mainWnd->ShowControlBar(&m_ExampleToolPal, FALSE, FALSE);
	
	AfxSetResourceHandle(m_AppInst);
	
	return 0;
}
