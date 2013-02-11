#if !defined(AFX_SCENEDLDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_SCENEDLDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SceneDLDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSceneDLDoc document

class CSceneDLDoc : public CDocument
{
protected:
	CSceneDLDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSceneDLDoc)
	CString m_LocalName;
	CString m_MungeName;
	CString m_Root;
	
// Attributes
public:
	LPTSTR	m_SFXFile;
	DWORD m_SFXLen;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSceneDLDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSceneDLDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CSceneDLDoc)
	afx_msg void OnFileCloseScene();
	afx_msg void OnUpdateFileCloseScene(CCmdUI* pCmdUI);
	afx_msg void OnFileOpenScene();
	afx_msg void OnUpdateFileOpenScene(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAsScene();
	afx_msg void OnUpdateFileSaveAsScene(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveScene();
	afx_msg void OnUpdateFileSaveScene(CCmdUI* pCmdUI);
	afx_msg void OnSceneAddrenderwindow();
	afx_msg void OnUpdateSceneAddrenderwindow(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENEDLDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
