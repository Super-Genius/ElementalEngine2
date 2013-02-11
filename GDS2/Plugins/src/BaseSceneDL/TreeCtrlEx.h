#if !defined(AFX_TREECTRLEX_H__D8807356_6C3E_403E_8F9A_BF522BB1EF97__INCLUDED_)
#define AFX_TREECTRLEX_H__D8807356_6C3E_403E_8F9A_BF522BB1EF97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeCtrlEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlEx window

class CTreeCtrlEx : public CTreeCtrl
{
// Construction
public:
	CTreeCtrlEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreeCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeCtrlEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg LRESULT OnDropMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREECTRLEX_H__D8807356_6C3E_403E_8F9A_BF522BB1EF97__INCLUDED_)
