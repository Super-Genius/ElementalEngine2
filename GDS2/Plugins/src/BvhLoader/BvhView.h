#if !defined(AFX_TEXTUREVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_)
#define AFX_TEXTUREVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextureView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBvhView view

class CBvhView : public CScrollView
{
	enum bvhViewHeights { HIER_HEIGHT = 20, HIER_WIDTH = 40};
protected:
	CBvhView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBvhView)

// Attributes
public:
	static void RenderImage(CDC* pDC, CBvhDoc *pDoc, CRect cRect);
	void DrawHeirarchy(  CBvhDoc * pDoc );
	void BvhRecurseForwardPrint(BvhNode * bNode, CRect * pRect, HTREEITEM hParentItem);
	void OnInitialUpdate();
	CTreeCtrl BvhTreeControl;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBvhView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CBvhView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CBvhView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTUREVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_)
