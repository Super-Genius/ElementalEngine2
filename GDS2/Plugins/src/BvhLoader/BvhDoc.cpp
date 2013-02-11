// TextureDoc.cpp : implementation file
//

#include "stdafx.h"
#include "BvhUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT AddObjectMessage = RegisterWindowMessage(ADD_OBJECT_MSG);
const UINT RemoveObjectMessage = RegisterWindowMessage(REMOVE_OBJECT_MSG);
const UINT ClearObjectMessage = RegisterWindowMessage(CLEAR_ALL_OBJECT_MSG);

/////////////////////////////////////////////////////////////////////////////
// CBvhDoc

IMPLEMENT_DYNCREATE(CBvhDoc, CDocument)

CBvhDoc::CBvhDoc()
{
//	m_ImageLoaded = FALSE;
}

BOOL CBvhDoc::OnNewDocument()
{
	BOOL retVal;

	retVal = CDocument::OnNewDocument();

	return retVal;
}

CBvhDoc::~CBvhDoc()
{
}


BEGIN_MESSAGE_MAP(CBvhDoc, CDocument)
	//{{AFX_MSG_MAP(CBvhDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBvhDoc diagnostics

#ifdef _DEBUG
void CBvhDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBvhDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBvhDoc serialization

void CBvhDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBvhDoc commands

BOOL CBvhDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	HANDLE hFile = OpenFile((TCHAR*)lpszPathName);
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	else
	{
		CloseHandle(hFile);
		try{
			m_BvhObject = new CBvhObject(lpszPathName);
		}catch(fileNotFound){
			if(m_BvhObject)
			{
				delete m_BvhObject;
				m_BvhObject = NULL;
			}
			return FALSE;
		}
	}
	CryCAFExporter CryExportCAF;
	CryExportCAF.SaveBoneAnimFile(m_BvhObject,"c:\\MyTestBvhCaf.caf");

	CRect tRect(0, 0, THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT);
	CDC tDC;
	CBitmap *oldBitmap;
	CRgn ClipRgn;
	CDC scrDC;
	
	scrDC.Attach(GetDC(GetDesktopWindow()));

	ClipRgn.CreateRectRgn(tRect.left, tRect.top, tRect.right, tRect.bottom);

	tDC.CreateCompatibleDC(&scrDC);
	tDC.SetMapMode(scrDC.GetMapMode());

	tDC.DPtoLP(&tRect);
	tDC.SetWindowExt(tRect.Size());
	tDC.SetWindowOrg(tRect.left, tRect.top);

	tDC.LPtoDP(&tRect);
	tDC.SetViewportExt(tRect.Size());
	tDC.SetViewportOrg(tRect.left, tRect.top);

	m_ThumbNail.CreateCompatibleBitmap(&scrDC, tRect.Width(), tRect.Height());
	oldBitmap = tDC.SelectObject(&m_ThumbNail);

	tDC.FillSolidRect(tRect, RGB(0xff, 0xff, 0xff));
	// create a thumbnail
	//CBvhView::DrawHeirarchy(&tDC, &tRect, this);

	tDC.SelectObject(oldBitmap);

	m_FileName = lpszPathName;
	GetFileTitle(m_FileName, m_DisplayName.GetBuffer(_MAX_FNAME),
		_MAX_FNAME);
	m_DisplayName.ReleaseBuffer();

	// made it this far so add to program list of objects
	gBvhLoader->m_BvhObjects.push_back(m_BvhObject);

	OBJECTSTRUCT os;

	os.name = &m_DisplayName;
	os.fileName = &m_FileName;
	os.thumbNail = &m_ThumbNail;
	os.pageIndex = &gBvhLoader->m_PageIndex;
	os.rsrcIndex = &m_ObjectIndex;
	os.object = m_BvhObject;

	gBvhLoader->m_ProgramInfo->BroadcastMessage(
		AddObjectMessage, &os);
	return TRUE;
}

void CBvhDoc::OnCloseDocument() 
{
	if(m_BvhObject)
	{
		delete m_BvhObject;
		m_BvhObject = NULL;
	}	
	CDocument::OnCloseDocument();
}

