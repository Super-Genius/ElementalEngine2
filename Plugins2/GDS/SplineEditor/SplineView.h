///============================================================================
/// \file		SplineComponent.cpp
/// \brief		Header file for Spline View
/// \date		11-14-2006
/// \author		Richard Nguyen
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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSplineView view


class CSplineView : public CView
{
protected:
	
	CSplineView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSplineView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplineView)
	protected:
	
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	virtual ~CSplineView();
#ifdef _DEBUG
	
	virtual void AssertValid() const;
	
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CSplineView)
	
	afx_msg void OnUpdateSplineTest1(CCmdUI* pCmdUI);
	
	afx_msg void OnUpdateSplineTest2(CCmdUI* pCmdUI);
	
	afx_msg void OnUpdateSplineTest3(CCmdUI* pCmdUI);
	
	afx_msg void OnSplineTest1();
	
	afx_msg void OnSplineTest2();
	
	afx_msg void OnSplineTest3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
