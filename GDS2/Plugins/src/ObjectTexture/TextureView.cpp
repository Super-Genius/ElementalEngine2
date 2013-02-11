// TextureView.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

CRect cubeFaceDestRect[] =
{
	CRect(512, 256, 768, 512),		// Positive X
	CRect(0, 256, 256, 512),		// Negative X
	CRect(256, 0, 512, 256),		// Positive Y
	CRect(256, 512, 512, 768),		// Negative Y
	CRect(256, 256, 512, 512),		// Positive Z
	CRect(768, 256, 1024, 512)		// Negative Z
};

UINT CubeMapDirections[] =
{
	DDS_CUBEMAP_POSITIVEX,
	DDS_CUBEMAP_NEGATIVEX,
	DDS_CUBEMAP_POSITIVEY,
	DDS_CUBEMAP_NEGATIVEY,
	DDS_CUBEMAP_POSITIVEZ,
	DDS_CUBEMAP_NEGATIVEZ
};

#define CUBEMAP_TOTAL_DIR (sizeof(CubeMapDirections) / sizeof(CubeMapDirections[0]))

/////////////////////////////////////////////////////////////////////////////
// CTextureView

IMPLEMENT_DYNCREATE(CTextureView, CView)

CTextureView::CTextureView()
{
}

CTextureView::~CTextureView()
{
}


BEGIN_MESSAGE_MAP(CTextureView, CView)
	//{{AFX_MSG_MAP(CTextureView)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextureView drawing

void CTextureView::OnDraw(CDC* pDC)
{
	CRect cRect;
	CTextureDoc *pDoc;
	pDoc = DYNAMIC_DOWNCAST(CTextureDoc, GetDocument());
	ASSERT_VALID(pDoc);
	CDC srcDC;

	GetClientRect(cRect);
	CMemDC memDC(pDC);
	RenderImage(memDC, pDoc, cRect);

}

void CTextureView::RenderImage(CDC* pDC, CTextureDoc *pDoc, CRect cRect) 
{
	BITMAP bitmapInfo;
	CRect destRect;
	CDC srcDC;
	CBitmap *oldBitmap;
	BITMAPIMAGES *Image;
	float scaleX, scaleY;
	list<BITMAPIMAGES *>::iterator itBI;
	UINT i;
	
	// Scale image into window for now
	srcDC.CreateCompatibleDC(pDC);

	if (pDoc->m_TextureType == TEXTURE_CUBEMAP)
	{
		itBI = pDoc->m_TextureObject->m_TheBitmaps.begin();

		scaleX = (float)cRect.Width() / 1024.0f;
		scaleY = (float)cRect.Height() / 768.0f;
		for (i=0; i < CUBEMAP_TOTAL_DIR; i++)
		{

			if (pDoc->m_CubeFlags & CubeMapDirections[i])
			{
				Image = (BITMAPIMAGES *)(*itBI);
				itBI++;
				oldBitmap = srcDC.SelectObject(Image->bitmap);
				Image->bitmap->GetBitmap(&bitmapInfo);

				destRect.left = (DWORD)((float)(cRect.left + cubeFaceDestRect[i].left) * scaleX);
				destRect.right = (DWORD)((float)(cRect.left + cubeFaceDestRect[i].right) * scaleX);
				destRect.top = (DWORD)((float)(cRect.top + cubeFaceDestRect[i].top) * scaleY);
				destRect.bottom = (DWORD)((float)(cRect.top + cubeFaceDestRect[i].bottom) * scaleY);

				pDC->SetStretchBltMode(COLORONCOLOR);
				pDC->StretchBlt(destRect.left, destRect.top, destRect.Width(), destRect.Height(), 
					&srcDC, 0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, SRCCOPY);

				srcDC.SelectObject(oldBitmap);
			}
		}
	}
	else
	{
		Image = pDoc->m_TextureObject->m_TheBitmaps.front();

		oldBitmap = srcDC.SelectObject(Image->bitmap);

		Image->bitmap->GetBitmap(&bitmapInfo);

		pDC->SetStretchBltMode(COLORONCOLOR);
		pDC->StretchBlt(cRect.left, cRect.top, cRect.Width(), cRect.Height(), &srcDC,
			0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, SRCCOPY);
		srcDC.SelectObject(oldBitmap);
	}

}


/////////////////////////////////////////////////////////////////////////////
// CTextureView diagnostics

#ifdef _DEBUG
void CTextureView::AssertValid() const
{
	CView::AssertValid();
}

void CTextureView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextureView message handlers

BOOL CTextureView::OnEraseBkgnd(CDC* pDC) 
{
	// don't erase
	return TRUE;
}
