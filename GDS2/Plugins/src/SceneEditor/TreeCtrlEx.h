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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreeCtrlEx();

	BOOL SetCheck(HTREEITEM hItem, BOOL fCheck = 1);

	// Generated message map functions
protected:

	//{{AFX_MSG(CTreeCtrlEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnDropMessage(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_fDragging;
	HTREEITEM m_DragItem;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREECTRLEX_H__D8807356_6C3E_403E_8F9A_BF522BB1EF97__INCLUDED_)
