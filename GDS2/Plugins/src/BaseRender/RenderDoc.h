#if !defined(AFX_RENDERDOC_H__7C8AA33D_B3B9_4FA6_B79C_B8748CFD6A0C__INCLUDED_)
#define AFX_RENDERDOC_H__7C8AA33D_B3B9_4FA6_B79C_B8748CFD6A0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenderDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRenderDoc document

class CRenderDoc : public CDocument
{
protected:
	CRenderDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CRenderDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenderDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRenderDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CRenderDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENDERDOC_H__7C8AA33D_B3B9_4FA6_B79C_B8748CFD6A0C__INCLUDED_)
