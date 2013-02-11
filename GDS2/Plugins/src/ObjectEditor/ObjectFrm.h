#if !defined(AFX_OBJECTFRM_H__D0F99C6B_8296_469D_9CCD_028B2E15383F__INCLUDED_)
#define AFX_OBJECTFRM_H__D0F99C6B_8296_469D_9CCD_028B2E15383F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObjectFrm frame


class CObjectFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CObjectFrm)
protected:
	
	CObjectFrm();           // protected constructor used by dynamic creation

// Attributes
public:
		
	HINSTANCE m_AppInst;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectFrm)
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	virtual ~CObjectFrm();

	// Generated message map functions
	//{{AFX_MSG(CObjectFrm)
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CObjectToolPal m_ObjectToolPal;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTFRM_H__D0F99C6B_8296_469D_9CCD_028B2E15383F__INCLUDED_)
