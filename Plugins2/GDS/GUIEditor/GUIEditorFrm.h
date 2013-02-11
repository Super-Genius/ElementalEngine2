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

#if !defined(AFX_GUIEDITORFRM_H__D0F99C6B_8296_469D_9CCD_028B2E15383F__INCLUDED_)
#define AFX_GUIEDITORFRM_H__D0F99C6B_8296_469D_9CCD_028B2E15383F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUIEditorFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUIEditorFrm frame

class CGUITabBar;

class CGUIEditorFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CGUIEditorFrm)
protected:
	
	CGUIEditorFrm();           // protected constructor used by dynamic creation

// Attributes
public:
	CGUITools m_Tools;
	CGUIData m_GUIDataDlg;
	CGUIAnimationView m_AnimView;
	CLanguageView m_LangView;
	CGenericDockBar *m_ToolsBar;
	CGenericDockBar *m_AnimViewBar;
	CGenericDockBar *m_GUIDataBar;
    COptionTreeDock *m_OptionTree;
	HINSTANCE m_AppInst;

// Operations
public:

// Overrides
	
// Implementation
protected:
	
	virtual ~CGUIEditorFrm();

	// Generated message map functions
	//{{AFX_MSG(CGUIEditorFrm)
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	
public:
		
	afx_msg void OnGuieditorLanguages();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIEditorFRM_H__D0F99C6B_8296_469D_9CCD_028B2E15383F__INCLUDED_)

