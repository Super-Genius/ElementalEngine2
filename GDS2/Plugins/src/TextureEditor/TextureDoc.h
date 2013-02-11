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

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CTextureDoc document

enum TEXTURETYPES
{
TEXTURE_2D = 0,
TEXTURE_3D,
TEXTURE_CUBEMAP,
TEXTURE_ANIMATED
};

class CTextureDoc : public CDocument
{
protected:
	CTextureDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTextureDoc)

// Attributes
public:
	CString m_DisplayName;	// without path
	CBitmap m_ThumbNail;
	IBaseTextureObject *m_pTextureObject;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextureDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	/// Open document for create thumbnail or other actions.
	virtual BOOL TargetOpenDocument(LPCTSTR lpszPathName, bool isOnlyForThumbnail = false);
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
	//void DeleteImageInfo(BITIMAGELIST &tImage);

	//{{AFX_MSG(CTextureDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	IToolBox *m_ToolBox;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTUREDOC_H__80A18743_A0D3_4593_AADE_3EB5BA220CC5__INCLUDED_)
