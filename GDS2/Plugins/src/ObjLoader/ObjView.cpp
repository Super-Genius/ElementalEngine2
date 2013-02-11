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
// CObjView

IMPLEMENT_DYNCREATE(CObjView, CView)

CObjView::CObjView()
{
}

CObjView::~CObjView()
{
}


BEGIN_MESSAGE_MAP(CObjView, CView)
	//{{AFX_MSG_MAP(CObjView)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjView drawing

void CObjView::OnDraw(CDC* pDC)
{
	CRect cRect;
	CObjDoc *pDoc;
	pDoc = DYNAMIC_DOWNCAST(CObjDoc, GetDocument());
	ASSERT_VALID(pDoc);
	CDC srcDC;

	GetClientRect(cRect);
	CMemDC memDC(pDC);
	RenderImage(memDC, pDoc, cRect);

}

void CObjView::RenderImage(CDC* pDC, CObjDoc *pDoc, CRect cRect) 
{
	

}


/////////////////////////////////////////////////////////////////////////////
// CObjView diagnostics

#ifdef _DEBUG
void CObjView::AssertValid() const
{
	CView::AssertValid();
}

void CObjView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CObjView message handlers

BOOL CObjView::OnEraseBkgnd(CDC* pDC) 
{
	// don't erase
	return TRUE;
}
