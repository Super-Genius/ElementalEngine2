#if !defined(AFX_TEXTUREDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
#define AFX_TEXTUREDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextureDoc.h : header file
//

#define DDS_CUBEMAP_POSITIVEX	0x00000400L
#define DDS_CUBEMAP_NEGATIVEX	0x00000800L
#define DDS_CUBEMAP_POSITIVEY	0x00001000L
#define DDS_CUBEMAP_NEGATIVEY	0x00002000L
#define DDS_CUBEMAP_POSITIVEZ	0x00004000L
#define DDS_CUBEMAP_NEGATIVEZ	0x00008000L
#define DDS_CUBEMAP_MASK		(DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX | \
					DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY | \
					DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ)

#include "BvhPart.h"

/////////////////////////////////////////////////////////////////////////////
// CBvhDoc document

class CBvhDoc : public CDocument
{
protected:
	CBvhDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBvhDoc)

// Attributes
public:
	
	UINT m_ObjectIndex;
	CBitmap m_ThumbNail;
	CString m_FileName;
	CString m_DisplayName;	// without path
	CBvhObject * m_BvhObject;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBvhDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBvhDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void DeleteImageInfo(std::list<BITMAPIMAGES *> &tImage);

	//{{AFX_MSG(CBvhDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTUREDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
