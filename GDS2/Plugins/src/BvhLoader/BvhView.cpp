// TextureView.cpp : implementation file
//

#include "stdafx.h"
#include "bvhview.h"

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
// CBvhView

IMPLEMENT_DYNCREATE(CBvhView, CView)

CBvhView::CBvhView()
{
}

CBvhView::~CBvhView()
{
}


BEGIN_MESSAGE_MAP(CBvhView, CView)
	//{{AFX_MSG_MAP(CBvhView)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
ON_WM_VSCROLL()
ON_WM_HSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBvhView drawing

void CBvhView::OnDraw(CDC* pDC)
{
	CBvhDoc *pDoc;
	pDoc = DYNAMIC_DOWNCAST(CBvhDoc, GetDocument());
	ASSERT_VALID(pDoc);
	// Save DC
	pDC->SaveDC();
	// Fill Screen with Blue
	CBrush brush( RGB( 25, 25, 100 ) );
	CRect rect;
	pDC->GetClipBox(&rect);
	FillRect( *pDC, &rect, brush );
	// Draw Event List
	//DrawHeirarchy( pDC, pDoc );
	pDC->RestoreDC(-1);
}

void CBvhView::RenderImage(CDC* pDC, CBvhDoc *pDoc, CRect cRect) 
{
	

}

void CBvhView::OnInitialUpdate()
{
	CBvhDoc *pDoc;
	pDoc = DYNAMIC_DOWNCAST(CBvhDoc, GetDocument());
	ASSERT_VALID(pDoc);

	CBvhObject * tbvh = pDoc->m_BvhObject;
	//SIZE sview;
	//sview.cx = 0;
	//sview.cy =  400;// HIER_HEIGHT*tbvh->GetLinear()->size();
	SetScrollSizes(MM_TEXT,CSize(100,400));
	
	BvhTreeControl.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | TVS_EDITLABELS |  TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT,
		CRect(10,10,350,510),this,7003);
	BvhTreeControl.SetFocus();
	DrawHeirarchy( pDoc );

}

/////////////////////////////////////////////////////////////////////////////
// CBvhView diagnostics

#ifdef _DEBUG
void CBvhView::AssertValid() const
{
	CView::AssertValid();
}

void CBvhView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBvhView message handlers

BOOL CBvhView::OnEraseBkgnd(CDC* pDC) 
{
	// don't erase
	return TRUE;
}

void CBvhView::DrawHeirarchy(  CBvhDoc * pDoc )
{
	USES_CONVERSION;
	char buf[1024];
	CRect pRect(0,0,0,0);
	sprintf(buf, "%s.bvh", pDoc->m_DisplayName);
	//pDC->SetBkMode(TRANSPARENT);
	//pDC->TextOut( pRect.left, pRect.top, (CString)(buf) );
	HTREEITEM hParentItem = BvhTreeControl.InsertItem(LPCSTR(buf));
	//recurse into bvh:
	CBvhObject * tbvh = pDoc->m_BvhObject;
	BvhRecurseForwardPrint(tbvh->root, &pRect, hParentItem);
	//pDC->SetBkMode(OPAQUE);
}

void CBvhView::BvhRecurseForwardPrint(BvhNode * bNode, CRect * pRect, HTREEITEM hParentItem)
{
	USES_CONVERSION;
	char buf[255];
	sprintf(buf, "%s", bNode->name.c_str());
	if(strlen(buf) <= 0)
		return;
	pRect->left += HIER_WIDTH;
	pRect->top += HIER_HEIGHT;
	//pDC->TextOut( pRect->left, pRect->top, (CString)(buf) );
	HTREEITEM IamParent = BvhTreeControl.InsertItem(LPCSTR(buf),hParentItem);
	//for all of it's children, recurse:
	for(int i = 0; i < bNode->child.size(); i++)
	{
		BvhRecurseForwardPrint(bNode->child[i],pRect,IamParent);
	}
	pRect->left -= HIER_WIDTH;
}

void CBvhView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CBvhView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}
