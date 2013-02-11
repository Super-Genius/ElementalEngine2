#if !defined(AFX_SpeechVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_)
#define AFX_SpeechVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpeechView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpeechView view


class CSpeechView : public CView
{
protected:
	
	CSpeechView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSpeechView)

// Attributes
public:

// Operations
public:
	void DrawWave( CDC *pDC, CRect *pRect, CSpeechDoc * pDoc );
	void DrawEvents( CDC *pDC, CRect *pRect, CSpeechDoc * pDoc );
	float CSpeechView::GetRatio( CSpeechDoc * pDoc, int i );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpeechView)
	protected:
	
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	virtual ~CSpeechView();
#ifdef _DEBUG
	
	virtual void AssertValid() const;
	
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CSpeechView)
	
//	afx_msg void OnUpdateSpeechTest1(CCmdUI* pCmdUI);
	
//	afx_msg void OnUpdateSpeechTest2(CCmdUI* pCmdUI);
	
//	afx_msg void OnUpdateSpeechTest3(CCmdUI* pCmdUI);
	
//	afx_msg void OnSpeechTest1();
	
//	afx_msg void OnSpeechTest2();
	
//	afx_msg void OnSpeechTest3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnPaint();
//	afx_msg void OnUpdateUIState(UINT /*nAction*/, UINT /*nUIElement*/);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SpeechVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_)
