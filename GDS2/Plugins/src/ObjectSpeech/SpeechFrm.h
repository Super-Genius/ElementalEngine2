#if !defined(AFX_SpeechFRM_H__D0F99C6B_8296_469D_9CCD_028B2E15383F__INCLUDED_)
#define AFX_SpeechFRM_H__D0F99C6B_8296_469D_9CCD_028B2E15383F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpeechFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpeechFrm frame


class CSpeechFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSpeechFrm)
protected:
	
	CSpeechFrm();           // protected constructor used by dynamic creation

// Attributes
public:
	
	CSpeechToolPal m_SpeechToolPal;
	
	HINSTANCE m_AppInst;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpeechFrm)
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	virtual ~CSpeechFrm();

	// Generated message map functions
	//{{AFX_MSG(CSpeechFrm)
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SpeechFRM_H__D0F99C6B_8296_469D_9CCD_028B2E15383F__INCLUDED_)
