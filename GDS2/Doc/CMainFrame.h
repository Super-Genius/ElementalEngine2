//-----------------------------------------------------------------------------
//
// File:          CMainFrame.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CMainFrame'
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

#ifndef _CMAINFRAME_H
#define _CMAINFRAME_H

//@START_USER1
//@END_USER1



class CMainFrame
    : public CMDIFrameWnd
{

//@START_USER2
//@END_USER2

//
// Group: ClassBuilder methods
//

private:
    void ConstructorInclude();
    void DestructorInclude();

//
// Non-Grouped Members
//

protected:
    CStatusBar m_wndStatusBar;
    CHiColorToolBar m_wndToolBar;
    CBitmap m_bmToolbarHi;
    CBitmap m_bmToolbarHiD;
    CObjectBar m_ObjectBar;

public:
    COptionsDialog* m_OptDialog;
    MAPSTRTOCLASS m_OverrideMap;

//
// Non-Grouped Methods
//

protected:
    afx_msg LRESULT OnAddOptionPageMessage(WPARAM wParam, LPARAM lParam);
    afx_msg void OnClose();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnFileNew();
    afx_msg LRESULT OnGetStatusBar(WPARAM wParam, LPARAM lParam);
    afx_msg void OnToolsOptions();
    afx_msg void OnUpdateToolsOptions(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewObjects(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
    afx_msg void OnViewObjects();
    afx_msg void OnViewOutput();

public:
    CMainFrame();
    virtual ~CMainFrame();
    void AssertValid();
    BOOL BroadcastMessage(UINT nIDmsg, WPARAM wParam, LPARAM lParam, HWND tWnd);
    void Dump(CDumpContext& dc);
    PFNCLASSOVERRIDE GetOverrideClass(LPCTSTR className);
    BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
                  AFX_CMDHANDLERINFO* pHandlerInfo);
    BOOL PreCreateWindow(CREATESTRUCT& cs);
    void SetOverrideClass(OVERRIDECLASS* overrideInfo);
    BOOL VerifyBarState(LPCTSTR lpszProfileName);
};

#endif


#ifdef CB_INLINES
#ifndef _CMAINFRAME_H_INLINES
#define _CMAINFRAME_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
