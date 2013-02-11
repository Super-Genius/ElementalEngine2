//-----------------------------------------------------------------------------
//
// File:          CMainFrame.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CMainFrame'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'VerifyBarState'
*     Added method 'SetOverrideClass'
*     Added method 'PreCreateWindow'
*     Added method 'OnViewOutput'
*     Added method 'OnViewObjects'
*     Added method 'OnUpdateViewOutput'
*     Added method 'OnUpdateViewObjects'
*     Added method 'OnUpdateToolsOptions'
*     Added method 'OnToolsOptions'
*     Added method 'OnGetStatusBar'
*     Added method 'OnFileNew'
*     Added method 'OnCreate'
*     Added method 'OnCmdMsg'
*     Added method 'OnClose'
*     Added method 'OnAddOptionPageMessage'
*     Added method 'GetOverrideClass'
*     Added method 'Dump'
*     Added method 'BroadcastMessage'
*     Added method 'AssertValid'
*     Added method '~CMainFrame'
*     Added method 'CMainFrame'
*     Added inheritance 'CMDIFrameWnd'
*     Added member 'm_ObjectBar'
*     Added member 'm_bmToolbarHiD'
*     Added member 'm_bmToolbarHi'
*     Added member 'm_wndToolBar'
*     Added member 'm_wndStatusBar'
*     Added member 'm_OverrideMap'
*     Added member 'm_OptDialog'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

//@START_USER1
//@END_USER1


// Master include file
#include "StdAfx.h"


//@START_USER2
//@END_USER2




/*@NOTE_3683
Constructor method.
*/
CMainFrame::CMainFrame() //@INIT_3683
{//@CODE_3683

	m_OptDialog = NULL;
}//@CODE_3683


/*@NOTE_3677
Destructor method.
*/
CMainFrame::~CMainFrame()
{//@CODE_3677

	m_bmToolbarHi.DeleteObject();
	if (m_OptDialog != NULL)
	{
		m_OptDialog->DestroyWindow();
		delete m_OptDialog;
		m_OptDialog = NULL;
	}
}//@CODE_3677


void CMainFrame::AssertValid()
{//@CODE_3709

	CMDIFrameWnd::AssertValid();
}//@CODE_3709


/*@NOTE_3693
This is the generic Broadcast message that all windows receive.
*/
BOOL CMainFrame::BroadcastMessage(UINT nIDmsg, WPARAM wParam, LPARAM lParam,
                                  HWND tWnd)
{//@CODE_3693

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
}//@CODE_3693


void CMainFrame::Dump(CDumpContext& dc)
{//@CODE_3710

	CMDIFrameWnd::Dump(dc);
}//@CODE_3710


PFNCLASSOVERRIDE CMainFrame::GetOverrideClass(LPCTSTR className)
{//@CODE_3702

	PFNCLASSOVERRIDE pfnClassOverride;
	MAPSTRTOCLASS::iterator mscIT;

	pfnClassOverride = NULL;

	mscIT = m_OverrideMap.find(className);
	if (mscIT != m_OverrideMap.end())
	{
		pfnClassOverride = mscIT->second;
	}

	return pfnClassOverride;
}//@CODE_3702


/*@NOTE_3742
Function that handles message to add a page to the options page of the main applications
options dialog.  These are property pages on a property sheet.
*/
afx_msg LRESULT CMainFrame::OnAddOptionPageMessage(WPARAM wParam, LPARAM lParam)
{//@CODE_3742

	if (m_OptDialog != NULL)
	{
		CPropertyPage *tProp;

		tProp = (CPropertyPage *)lParam;
		VERIFY(tProp->IsKindOf(RUNTIME_CLASS(CPropertyPage)));

		m_OptDialog->AddPage(tProp);
	}

	return TRUE;
}//@CODE_3742


afx_msg void CMainFrame::OnClose()
{//@CODE_3730

	if (m_OptDialog->IsWindowVisible())
	{
		m_OptDialog->SendMessage(WM_COMMAND, IDOK);
		if (m_OptDialog->IsWindowVisible())
			return;
	}

	CString sProfile = _T("BarState");
// this needs to be fixed as some bar's in DLL's are saved and not
// present when gds reloads.
//	SaveBarState(sProfile);

	CMDIFrameWnd::OnClose();
}//@CODE_3730


/*@NOTE_3687
Commands sent to the active window.  i.e. Hotkeys, Menu messages etc.
*/
BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra,
                          AFX_CMDHANDLERINFO* pHandlerInfo)
{//@CODE_3687

	STDDLLLIST::iterator DLLit;
    CDLLInterface *di;

	CMDIChildWnd* pActiveChild = MDIGetActive();
	// pump through active child FIRST
	if (pActiveChild != NULL)
	{
		CPushRoutingFrame push(this);
		if (pActiveChild->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

	// try pumping through base DLLS
	for (DLLit = theApp.m_DLLS.begin(); DLLit != theApp.m_DLLS.end(); DLLit++)
	{
		di = (CDLLInterface *)*DLLit;
		if (di->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

	// now try the resource manager
	if (m_ObjectBar.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// try use then
	
	// then pump through normal frame
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}//@CODE_3687


afx_msg int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{//@CODE_3723

	int cButtons;
	CRect rect;
	STDDLLLIST::iterator DLLit;
    CDLLInterface *DLLInt;
	CDockState dockState;

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_OptDialog = new COptionsDialog();

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME_LARGE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CDC *tempDC;

	// get the main screen's handle
	tempDC = GetDesktopWindow()->GetDC();
	int nPlanes;
	int bpp;
	bpp = tempDC->GetDeviceCaps(BITSPIXEL);
	nPlanes = tempDC->GetDeviceCaps(PLANES);
	BOOL isHiColor;

	isHiColor = (bpp*nPlanes) > 8;
	if (isHiColor)
	{
		 // attach the hicolor bitmaps to the toolbar
		m_wndToolBar.AttachToolbarImages (IDB_TOOLBAR_LARGE,
							 IDB_TOOLBAR_LARGE_DISABLED,
							 IDB_TOOLBAR_LARGE);
	}

	CToolBarCtrl &tbCtrl = m_wndToolBar.GetToolBarCtrl();

	cButtons = sizeof(standardBarLabels) / sizeof(standardBarLabels[0]);
	for( int nButton = 0; nButton < cButtons; nButton++ )
	{
		int nButtonIndex = m_wndToolBar.CommandToIndex( standardBarLabels[nButton].id );
		if (!m_wndToolBar.SetButtonText( nButtonIndex, standardBarLabels[nButton].label ))
			TRACE("failed SetButtonText(%d, \"%s\").\n",
				   nButtonIndex, standardBarLabels[nButton].label);
	}

	// resize rectangle buttons to fit text
	m_wndToolBar.GetItemRect(0, rect);
	m_wndToolBar.SetSizes(rect.Size(), CSize(24, 24));

	m_wndToolBar.SetWindowText(_T("Menu Toolbar"));

	// Get the first button size after adding the labels.

	if (!m_wndStatusBar.Create(this) || 
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	if (!m_ObjectBar.Create("", this, IDD_OBJECTPANE, CBRS_BOTTOM | WS_VISIBLE))
	{
		TRACE0("Failed to create resource dialog bar\n");
		return -1;      
	} 
	
	m_ObjectBar.SetBarStyle(m_ObjectBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_ObjectBar.EnableDocking(CBRS_ALIGN_ANY);
	
	DockControlBar(&m_ObjectBar);

	SetMenu(CMenu::FromHandle(theApp.m_CurMenu));
    
	for (DLLit = theApp.m_DLLS.begin(); DLLit != theApp.m_DLLS.end(); DLLit++)
	{
		DLLInt = (CDLLInterface *)*DLLit;
		DLLInt->OnCreate(this);
	}

	// modeless options dialog, not visible
	m_OptDialog->Create(this, WS_POPUP | WS_CAPTION | WS_SYSMENU);

	CString sProfile = _T("BarState");
//	LoadBarState(sProfile);

	return 0;
}//@CODE_3723


afx_msg void CMainFrame::OnFileNew()
{//@CODE_3737

	// TODO: Add your command handler code here
	
}//@CODE_3737


/*@NOTE_3739
Handles windows registered message that retrieves the Status bar.  This is so that
plugins can update or add things to the status bar.
*/
afx_msg LRESULT CMainFrame::OnGetStatusBar(WPARAM wParam, LPARAM lParam)
{//@CODE_3739

	CStatusBar **statBar;

	statBar = (CStatusBar **)lParam;
	*statBar = &m_wndStatusBar;

	return TRUE;
}//@CODE_3739


/*@NOTE_3734
Intercepts the menu command to show the options dialog.
*/
afx_msg void CMainFrame::OnToolsOptions()
{//@CODE_3734

	if (!m_OptDialog->IsWindowVisible())
	{
		m_OptDialog->ShowWindow(SW_SHOW);
	}
}//@CODE_3734


/*@NOTE_3735
This is called when the main application wants to show the Tools Options command.  This
function can disable, check, etc, the menu item.
*/
afx_msg void CMainFrame::OnUpdateToolsOptions(CCmdUI* pCmdUI)
{//@CODE_3735

	pCmdUI->Enable(!m_OptDialog->IsWindowVisible());	
}//@CODE_3735


/*@NOTE_3732
When menu for View->Object is displayed, this function updates the attributes (i.e. checkmark).
*/
afx_msg void CMainFrame::OnUpdateViewObjects(CCmdUI* pCmdUI)
{//@CODE_3732

	pCmdUI->SetCheck(m_ObjectBar.IsVisible());	
}//@CODE_3732


/*@NOTE_3727
When the View->Output menu item is selected this function updates the attibutes of the
menu item.
*/
afx_msg void CMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI)
{//@CODE_3727
    afx_msg value;

    return value;
}//@CODE_3727


/*@NOTE_3731
Handles the View->Objects menu item user selection
*/
afx_msg void CMainFrame::OnViewObjects()
{//@CODE_3731

	if (m_ObjectBar.IsVisible())
	{
		ShowControlBar(&m_ObjectBar, FALSE, FALSE);
	}
	else
	{
		ShowControlBar(&m_ObjectBar, TRUE, FALSE);
	}	
}//@CODE_3731


/*@NOTE_3726
Handles the View->Output menu item user selection
*/
afx_msg void CMainFrame::OnViewOutput()
{//@CODE_3726
    afx_msg value;

    return value;
}//@CODE_3726


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{//@CODE_3707

	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}//@CODE_3707


void CMainFrame::SetOverrideClass(OVERRIDECLASS* overrideInfo)
{//@CODE_3705

	m_OverrideMap[overrideInfo->className] = overrideInfo->overrideClass;

}//@CODE_3705


BOOL CMainFrame::VerifyBarState(LPCTSTR lpszProfileName)
{//@CODE_3712
    BOOL value;

    return value;
}//@CODE_3712


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3678
Method which must be called first in a constructor.
*/
void CMainFrame::ConstructorInclude()
{
}


/*@NOTE_3679
Method which must be called first in a destructor.
*/
void CMainFrame::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
