#if !defined(AFX_RENDERFRM_H__9FAC4226_6DC6_457E_9DC3_DE547447C00D__INCLUDED_)
#define AFX_RENDERFRM_H__9FAC4226_6DC6_457E_9DC3_DE547447C00D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenderFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRenderFrm frame

class CRenderFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CRenderFrm)
protected:
	CRenderFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenderFrm)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRenderFrm();

	// Generated message map functions
	//{{AFX_MSG(CRenderFrm)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENDERFRM_H__9FAC4226_6DC6_457E_9DC3_DE547447C00D__INCLUDED_)
