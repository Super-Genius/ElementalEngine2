// TextureDoc.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT AddObjectMessage = RegisterWindowMessage(ADD_OBJECT_MSG);
const UINT RemoveObjectMessage = RegisterWindowMessage(REMOVE_OBJECT_MSG);
const UINT ClearObjectMessage = RegisterWindowMessage(CLEAR_ALL_OBJECT_MSG);

/////////////////////////////////////////////////////////////////////////////
// CTextureDoc

IMPLEMENT_DYNCREATE(CTextureDoc, CDocument)

CTextureDoc::CTextureDoc()
{
	m_ImageLoaded = FALSE;
}

BOOL CTextureDoc::OnNewDocument()
{
	BOOL retVal;

	retVal = CDocument::OnNewDocument();

	return retVal;
}

CTextureDoc::~CTextureDoc()
{
}


BEGIN_MESSAGE_MAP(CTextureDoc, CDocument)
	//{{AFX_MSG_MAP(CTextureDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextureDoc diagnostics

#ifdef _DEBUG
void CTextureDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTextureDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextureDoc serialization

void CTextureDoc::Serialize(CArchive& ar)
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
// CTextureDoc commands

BOOL CTextureDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	UINT Depth;
	UINT nMips;
	UINT nImages;
	UINT i, j;
	CBitmap *tBitmap;
	BITMAPIMAGES *tBI;

	CString message;
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_UPPER_LEFT);
	ilEnable(IL_ORIGIN_SET);

	ilGenImages(1, &m_TheImage);
	ilBindImage(m_TheImage);
	if (!ilLoadImage((char *const)lpszPathName))
	{
		message.Format(_T("Unable to open %s.\nError %d\n"), lpszPathName, GetLastError());
		MessageBox(NULL, message, _T("Load Error"), MB_OK);
		return FALSE;
	}


	m_TextureType = TEXTURE_2D;
	if ((Depth = ilGetInteger(IL_IMAGE_DEPTH)) > 1)
	{
		m_TextureType = TEXTURE_3D;
	}
	
	if ((nImages = ilGetInteger(IL_NUM_IMAGES)) > 0)
	{
		m_TextureType = TEXTURE_ANIMATED;
		if ((m_CubeFlags = ilGetInteger(IL_IMAGE_CUBEFLAGS)) & DDS_CUBEMAP_MASK)
		{
			m_TextureType = TEXTURE_CUBEMAP;
		}
	}

	// create a new texture object outside of document
	m_TextureObject = new CTextureObject((LPSTR)(LPCSTR)m_DisplayName);
	m_TextureObject->m_FullPath = m_FileName;

	// + base image
	nImages++;

	for (i=0; i <nImages; i++)
	{
		ilBindImage(m_TheImage);  // Set to parent image first.
		ilActiveImage(i);

		// we'll always have the base texture
		// make sure in 24 bit mode for display
		ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		tBI = new BITMAPIMAGES;

		tBitmap = new CBitmap();
		tBI->bitmap = tBitmap;
		m_TextureObject->m_TheBitmaps.push_back(tBI);

		tBitmap->m_hObject = ilutConvertToHBitmap(GetDC(NULL));
		if (tBitmap->m_hObject == NULL)
		{	
			message.Format(_T("Unable to convert to CBitmap Object\n for %s.\n"), lpszPathName);
			MessageBox(NULL, message, _T("Load Error"), MB_OK);
			delete m_TextureObject;
			ilDeleteImages(1, &m_TheImage);
			return FALSE;
		}

		if (m_TextureType == TEXTURE_CUBEMAP)
		{
			m_CubeFlags |= ilGetInteger(IL_IMAGE_CUBEFLAGS);
		}
		
		nMips = ilGetInteger(IL_NUM_MIPMAPS);
		for (j=0; j<nMips; j++)
		{
			CBitmap *tMip;
			BITMAPIMAGES *tMipImages;

			tMipImages = new BITMAPIMAGES;
			tMip = new CBitmap;
			tMipImages->bitmap = tMip;
			tBI->mipmaps.push_back(tMipImages);

			ilActiveMipmap(j+1);

			// we'll always have the base texture
			// make sure in 24 bit mode for display
			ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

			tMip->m_hObject = ilutConvertToHBitmap(GetDC(NULL));
			if (tMip->m_hObject == NULL)
			{

				message.Format(_T("Unable to convert to CBitmap Object\n for %s.\n"
						"Mip Level %d\n"), lpszPathName, i);
				MessageBox(NULL, message, _T("Load Error"), MB_OK);
				delete m_TextureObject;
				ilDeleteImages(1, &m_TheImage);
				return FALSE;
			}

			ilBindImage(m_TheImage);  // Set to parent image first.
			ilActiveImage(i);
		}
	}

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
	CTextureView::RenderImage(&tDC, this, tRect);

	tDC.SelectObject(oldBitmap);

	m_FileName = lpszPathName;
	GetFileTitle(m_FileName, m_DisplayName.GetBuffer(_MAX_FNAME),
		_MAX_FNAME);
	m_DisplayName.ReleaseBuffer();

	// made it this far so add to program list of objects
	gObjectTexture->m_TextureObjects.push_back(m_TextureObject);

	OBJECTSTRUCT os;

	os.name = &m_DisplayName;
	os.fileName = &m_FileName;
	os.thumbNail = &m_ThumbNail;
	os.pageIndex = &gObjectTexture->m_PageIndex;
	os.rsrcIndex = &m_ObjectIndex;
	os.object = m_TextureObject;

	gObjectTexture->m_ProgramInfo->BroadcastMessage(
		AddObjectMessage, &os);
	
	m_ImageLoaded = TRUE;

	return TRUE;
}

void CTextureDoc::OnCloseDocument() 
{
	if (m_ImageLoaded)
	{
		ilDeleteImages(1, &m_TheImage);
	}
	
	CDocument::OnCloseDocument();
}

