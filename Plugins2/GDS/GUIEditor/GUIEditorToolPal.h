#if !defined(AFX_GUIEDITORTOOLPAL_H__CDA99BDB_2D5F_479F_BC0B_FCC8383C662F__INCLUDED_)
#define AFX_GUIEDITORTOOLPAL_H__CDA99BDB_2D5F_479F_BC0B_FCC8383C662F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUIEditorToolPal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUIEditorToolPal window


class CGUIEditorToolPal : public CGuiToolBarWnd
{
// Construction
public:
	
	CGUIEditorToolPal();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUIEditorToolPal)
	//}}AFX_VIRTUAL

// Implementation
public:
	
	virtual ~CGUIEditorToolPal();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGUIEditorToolPal)
	afx_msg void On3dButton();
	afx_msg void OnUpdate3dButton(CCmdUI* pCmdUI);
	afx_msg void OnTestPalButton2();
	afx_msg void OnUpdateTestPalButton2(CCmdUI* pCmdUI);
	afx_msg void OnTestPalButton3();
	afx_msg void OnUpdateTestPalButton3(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIEditorTOOLPAL_H__CDA99BDB_2D5F_479F_BC0B_FCC8383C662F__INCLUDED_)
