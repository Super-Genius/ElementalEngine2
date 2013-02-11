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

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ADD_RESOURCE_MSG			"AddResourceToPage"
#define REMOVE_RESOURCE_MSG			"RemoveResourceFromPage"
#define CLEAR_ALL_RESOURCE_MSG		"ClearResourcePage"

/////////////////////////////////////////////////////////////////////////////
// CTextureDoc

IMPLEMENT_DYNCREATE(CTextureDoc, CDocument)

CTextureDoc::CTextureDoc()
{
	m_ToolBox = EngineGetToolBox();
	m_pTextureObject = NULL;
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
	return TargetOpenDocument(lpszPathName);
}

BOOL CTextureDoc::TargetOpenDocument(LPCTSTR lpszPathName, bool isOnlyForThumbnail) 
{
	CString message;
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	bool loadedTexture = false;
	CHashString hashName(lpszPathName);
	static DWORD msgHash_SetLoadParameters = CHashString(_T("SetLoadParameters")).GetUniqueID();
	LOADPARAMETERS lp1;

	if (!isOnlyForThumbnail)
	{
		m_pTextureObject = CRenderObject<>::LoadTexture(lpszPathName);
	}
	else
	{
		TEXTUREOBJECTPARAMS top;
		top.Name = &hashName;
		static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(TEXTUREOBJECTPARAMS), &top);

		if (top.TextureObjectInterface == NULL)
		{
			static DWORD msgHash_GetLoadParameters = CHashString(_T("GetLoadParameters")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetLoadParameters, sizeof(LOADPARAMETERS), &lp1);

			LOADPARAMETERS lp2(TEX_MEM_SYSMEM);
			m_ToolBox->SendMessage(msgHash_SetLoadParameters, sizeof(LOADPARAMETERS), &lp2);
			m_pTextureObject = CRenderObject<>::LoadTexture(lpszPathName);
			loadedTexture = true;
		}
		else
		{
			m_pTextureObject = top.TextureObjectInterface;
		}
	}

	if (loadedTexture)
		m_ToolBox->SendMessage(msgHash_SetLoadParameters, sizeof(LOADPARAMETERS), &lp1);

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

	tDC.FillSolidRect(tRect, RGB(0xff, 0x00, 0x00));
	// create a thumbnail
	CTextureView::RenderImage(&tDC, this, tRect);

	tDC.SelectObject(oldBitmap);
	if (m_pTextureObject != NULL)
		GetFileTitle(m_pTextureObject->GetTextureName()->GetString(), m_DisplayName.GetBuffer(_MAX_FNAME), _MAX_FNAME);
	
	if (loadedTexture)
	{
		static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
		TEXTUREOBJECTPARAMS top;
		top.Name = &hashName;
		top.TextureObjectInterface = m_pTextureObject;
		m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(TEXTUREOBJECTPARAMS), &top);
	}

	m_DisplayName.ReleaseBuffer();

	// made it this far so add to program list of objects
	//SINGLETONINSTANCE(CTextureEditor)->GetTextureDataList()->push_back(m_TextureObject);

	//OBJECTPAGEINFO objPageInfo;
	//objPageInfo.name = &m_DisplayName;
	//objPageInfo.thumbNail = &m_ThumbNail;
	//objPageInfo.pageIndex = SINGLETONINSTANCE(CTextureEditor)->m_PageIndex;
	//objPageInfo.rsrcIndex = m_ObjectIndex;
	//objPageInfo.object = m_TextureObject;
	//static DWORD msgHash_AddObjectMessage = CHashString(_T("AddObjectMessage")).GetUniqueID();
	//m_ToolBox->SendMessage(msgHash_AddObjectMessage, sizeof(OBJECTPAGEINFO), &objPageInfo);
	return TRUE;
}

void CTextureDoc::OnCloseDocument() 
{
	CDocument::OnCloseDocument();
}
