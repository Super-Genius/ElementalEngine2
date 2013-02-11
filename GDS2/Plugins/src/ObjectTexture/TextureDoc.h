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


/////////////////////////////////////////////////////////////////////////////
// CTextureDoc document

class CTextureDoc : public CDocument
{
protected:
	CTextureDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTextureDoc)

// Attributes
public:
	ILuint	m_TheImage;
	BOOL m_ImageLoaded;
	TEXTURETYPES m_TextureType;
	UINT m_CubeFlags;
	CString m_DisplayName;	// without path
	CString m_FileName;
	UINT m_ObjectIndex;
	CBitmap m_ThumbNail;
	CTextureObject *m_TextureObject;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextureDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTextureDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void DeleteImageInfo(std::list<BITMAPIMAGES *> &tImage);

	//{{AFX_MSG(CTextureDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTUREDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
