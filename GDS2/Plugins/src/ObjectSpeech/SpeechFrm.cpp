// SpeechFrm.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NUMICONS 4
#define NICONSWIDE 4

/////////////////////////////////////////////////////////////////////////////
// CSpeechFrm

IMPLEMENT_DYNCREATE(CSpeechFrm, CMDIChildWnd)

CSpeechFrm::CSpeechFrm()
{
}

CSpeechFrm::~CSpeechFrm()
{
}


BEGIN_MESSAGE_MAP(CSpeechFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CSpeechFrm)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpeechFrm message handlers

int CSpeechFrm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CMDIFrameWnd *mainWnd;
		
	// Register the doc templates we provide to the app
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);

	// get the apps instance handle for loading resources
	m_AppInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(ObjectSpeechDLL.hModule);
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_SpeechToolPal.CreateEx(mainWnd, TBSTYLE_FLAT | TBSTYLE_WRAPABLE , WS_CHILD | WS_VISIBLE | CBRS_RIGHT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED, CRect(0, 2, 2, 4)) ||
		!m_SpeechToolPal.LoadToolBar(IDR_Speech_TOOL_PALETTE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_SpeechToolPal.SetBorders(2, 2, 2, 2);

	m_SpeechToolPal.EnableDocking(CBRS_ORIENT_VERT);
	// make control bar notify us.

	mainWnd->DockControlBar(&m_SpeechToolPal);

	CRect tcRect;
	m_SpeechToolPal.GetToolBarCtrl().SetRows(1, FALSE, tcRect);
	int iconWidth;
	int iconHeight;
	int nRows;

	iconWidth = tcRect.Width()/NUMICONS;
	iconHeight = tcRect.Height();
	nRows = (NUMICONS / NICONSWIDE) + (((NUMICONS % NICONSWIDE) == 0) ? 0 : 1);

	tcRect.SetRect(0, 0, iconWidth * NICONSWIDE, (iconHeight * nRows));

	// Set the window size
	m_SpeechToolPal.SetWindowPos(NULL, 0, 0, tcRect.Width(), tcRect.Height(),
		SWP_NOACTIVATE  | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREDRAW);

	
	mainWnd->ShowControlBar(&m_SpeechToolPal, FALSE, FALSE);
	
	AfxSetResourceHandle(m_AppInst);
	
	return 0;
}
