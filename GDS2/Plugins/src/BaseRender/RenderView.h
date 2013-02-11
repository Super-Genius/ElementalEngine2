#if !defined(AFX_RENDERVIEW_H__255895D9_C497_4536_9F0D_2397CF4F614F__INCLUDED_)
#define AFX_RENDERVIEW_H__255895D9_C497_4536_9F0D_2397CF4F614F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenderView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRenderView view

class CRenderView : public CView
{
protected:
	CRenderView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CRenderView)

    CRender *m_Renderer;
	CMatrix4 *m_ViewMat;
	CCamera *m_ViewCamera;
	CMatrix4 *m_BoxMatrix;
	CMesh *m_BoxMesh;
    
// Attributes
public:

// Operations
public:
    virtual void SetRenderer(CRender *renderer, CString *name);
    BOOL OnIdle(LONG lCount);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenderView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL



// Implementation
protected:
	virtual ~CRenderView();
	void CreateBoxObject();
	void RemoveBoxObject();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CRenderView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRenderWireframe();
	afx_msg void OnRenderTexture();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENDERVIEW_H__255895D9_C497_4536_9F0D_2397CF4F614F__INCLUDED_)
