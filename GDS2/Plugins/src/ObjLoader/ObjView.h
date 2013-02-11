#if !defined(AFX_TEXTUREVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_)
#define AFX_TEXTUREVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextureView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObjView view

class CObjView : public CView
{
protected:
	CObjView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CObjView)

// Attributes
public:
	static void RenderImage(CDC* pDC, CObjDoc *pDoc, CRect cRect);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CObjView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CObjView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTUREVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_)
