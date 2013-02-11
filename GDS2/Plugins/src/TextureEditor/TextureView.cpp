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
	CRect destRect;
	CDC srcDC;
	CBitmap *oldBitmap;
	auto_ptr<CBitmap> Image(new CBitmap());
	
	float scaleX, scaleY;
	void* buffer;
	
	// Scale image into window for now
	srcDC.CreateCompatibleDC(pDC);
	
	if (pDoc->m_pTextureObject != NULL )
	{
		UINT width = pDoc->m_pTextureObject->GetWidth();
		UINT height = pDoc->m_pTextureObject->GetHeight();
		if (pDoc->m_pTextureObject->GetTextureType() == TEXTURE_CUBEMAP)
		{
			ICubeTextureObject * textureObjectCube = dynamic_cast<ICubeTextureObject *>(pDoc->m_pTextureObject);
			scaleX = (float)cRect.Width() / 1024.0f;
			scaleY = (float)cRect.Height() / 768.0f;
			for (int i = static_cast<int>(CUBEMAP_FACE_X); i <= static_cast<int>(CUBEMAP_FACE_NEGATIVE_Z); i++)
			{

				buffer = textureObjectCube->Read(static_cast<EE_CUBEMAPFACE>(i));
				if (buffer != NULL) 
				{
					Image->CreateBitmap(width, height, 1, textureObjectCube->GetColorDepth(), buffer);
					oldBitmap = srcDC.SelectObject(Image.get());

					destRect.left = (DWORD)((float)(cRect.left + cubeFaceDestRect[i].left) * scaleX);
					destRect.right = (DWORD)((float)(cRect.left + cubeFaceDestRect[i].right) * scaleX);
					destRect.top = (DWORD)((float)(cRect.top + cubeFaceDestRect[i].top) * scaleY);
					destRect.bottom = (DWORD)((float)(cRect.top + cubeFaceDestRect[i].bottom) * scaleY);

					pDC->SetStretchBltMode(COLORONCOLOR);
					pDC->StretchBlt(destRect.left, destRect.top, destRect.Width(), destRect.Height(), 
						&srcDC, 0, 0, width, height, SRCCOPY);

					srcDC.SelectObject(oldBitmap);
				}
			}
		}
		/*else if (pDoc->m_pTextureObject->GetTextureType() == EE_TEXTUREANIMATED2D)
		{
			IAnimatedTextureObject * textureObject = dynamic_cast<IAnimatedTextureObject *>(pDoc->m_pTextureObject);
			buffer = textureObject->Read(0);
			if (buffer != NULL)
			{
				Image->CreateBitmap(width, height, 1, textureObject->GetColorDepth(), buffer);

				//pDoc->m_TextureData->GetBitmaps()->front();

				oldBitmap = srcDC.SelectObject(Image.get());

				pDC->SetStretchBltMode(COLORONCOLOR);
				pDC->StretchBlt(cRect.left, cRect.top, cRect.Width(), cRect.Height(), &srcDC,
					0, 0, width, height, SRCCOPY);
				srcDC.SelectObject(oldBitmap);
		}*/
		else
		{
			ITextureObject * textureObject = dynamic_cast<ITextureObject *>(pDoc->m_pTextureObject);
			if (NULL == textureObject)
				buffer = NULL;
			else
				buffer = textureObject->Read();
			if (buffer != NULL)
			{
				/////////////////////////////////////////////
				//Dealing with 16bit devices as well 
				// lets create a device independent bitmap from our data
				BITMAPINFO bmpi = {0};
				BYTE *pBits = NULL;

				bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				bmpi.bmiHeader.biWidth = textureObject->GetWidth();
				bmpi.bmiHeader.biHeight = textureObject->GetHeight();
				bmpi.bmiHeader.biPlanes = 1;
				bmpi.bmiHeader.biBitCount = textureObject->GetColorDepth();	
				bmpi.bmiHeader.biCompression = BI_RGB;

				// handle to our device independent image
				HBITMAP hBmp = CreateDIBSection(srcDC, &bmpi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
				
				//CBitmap* currentBitmapRGBA = CBitmap::FromHandle( hBmp );
				CBitmap* currentBitmapRGBA = new CBitmap();
				currentBitmapRGBA->Attach ( hBmp );
				
				if ( currentBitmapRGBA != NULL )
				{
					UINT bufSize = textureObject->GetColorDepth() / 8 * textureObject->GetHeight() * textureObject->GetWidth();
					UINT bytesWritten =	currentBitmapRGBA->SetBitmapBits( bufSize , buffer );

					if ( bytesWritten == 0 )
					{
						EngineGetToolBox()->Log( LOGWARNING, _T("%s(%d): Error creating device independent bitmap for thumbnail.\n"), __FILE__, __LINE__ );
					} 
					else 
					{
						// everything seems to be OK.
						//Image = currentBitmapRGBA;

						oldBitmap = srcDC.SelectObject( currentBitmapRGBA );
						if ( oldBitmap != NULL )
						{
							pDC->SetStretchBltMode(COLORONCOLOR);
							pDC->StretchBlt(cRect.left, cRect.top, cRect.Width(), cRect.Height(), &srcDC,
								0, 0, width, height, SRCCOPY);
							srcDC.SelectObject(oldBitmap);
						}
						else
						{
							EngineGetToolBox()->Log( LOGWARNING, _T("%s(%d): Error selecting device independent bitmap into device context during generating thumbnail.\n"), __FILE__, __LINE__ );
						}
					}
				} 

				// remove our object;
				delete currentBitmapRGBA;


				/////////////////////////////////////////////
				// ORIGINAL ONLY 32bit VERION

				//Image->CreateBitmap(width, height, 1, textureObject->GetColorDepth(), buffer);

				//oldBitmap = srcDC.SelectObject(Image.get());

				//pDC->SetStretchBltMode(COLORONCOLOR);
				//pDC->StretchBlt(cRect.left, cRect.top, cRect.Width(), cRect.Height(), &srcDC,
				//	0, 0, width, height, SRCCOPY);
				//srcDC.SelectObject(oldBitmap);
			}
		}
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

void CTextureView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	if (bActivate)
	{
		SetActiveScene();
	}
}
