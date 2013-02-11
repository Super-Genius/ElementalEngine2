#if !defined(AFX_DRAGLISTBOXEX_H__0BED02A5_FBDC_4972_97E2_87BDD715A040__INCLUDED_)
#define AFX_DRAGLISTBOXEX_H__0BED02A5_FBDC_4972_97E2_87BDD715A040__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DragListBoxEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDragListBoxEx window

class CDragListBoxEx : public CDragListBox
{
// Construction
public:
	CDragListBoxEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDragListBoxEx)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDragListBoxEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDragListBoxEx)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAGLISTBOXEX_H__0BED02A5_FBDC_4972_97E2_87BDD715A040__INCLUDED_)
