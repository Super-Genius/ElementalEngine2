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

#if !defined(AFX_EXAMPLEVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_)
#define AFX_EXAMPLEVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExampleView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExampleView view


class CExampleView : public CView
{
protected:
	
	CExampleView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CExampleView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExampleView)
	protected:
	
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	
	virtual void OnInitialUpdate();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	virtual ~CExampleView();
#ifdef _DEBUG
	
	virtual void AssertValid() const;
	
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CExampleView)
	
	afx_msg void OnUpdateExampleTest1(CCmdUI* pCmdUI);
	
	afx_msg void OnUpdateExampleTest2(CCmdUI* pCmdUI);
	
	afx_msg void OnUpdateExampleTest3(CCmdUI* pCmdUI);
	
	afx_msg void OnExampleTest1();
	
	afx_msg void OnExampleTest2();
	
	afx_msg void OnExampleTest3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool	m_bIsActive;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXAMPLEVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_)
