//---------------------------------------------------------------------
// File:		ResourceListCtrlEx.cpp
// Description:	MFC List Control Extension that adds resource image dragging functionality
// Created:		07/07/05
// Author:		Brian Bazyk
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
//---------------------------------------------------------------------

#include "stdafx.h"
#include "ResourceListCtrlEx.h"
#include "Messages.h"
#include "..\include\resourcelistctrlex.h"
#include "memdc.h"

#define IMAGESIZEX	64
#define IMAGESIZEY	64

static UINT ResourceDropMessage = RegisterWindowMessage(RESOURCE_DROP_MSG_STR);
static UINT ResourceDragMessage = RegisterWindowMessage(RESOURCE_DRAG_MSG_STR);

BEGIN_MESSAGE_MAP(CResourceListCtrlEx, CListCtrl)
	ON_REGISTERED_MESSAGE(ResourceDropMessage, OnDropMessage)
	ON_REGISTERED_MESSAGE(ResourceDragMessage, OnDragMessage)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_KEYUP()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

int CALLBACK CompareResourceItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	return (lParam1 <= lParam2) ? -1 : 1;
}

CResourceListCtrlEx::CResourceListCtrlEx()
{
	m_ToolBox = EngineGetToolBox();

	m_ThumbNails.Create(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT,
		ILC_COLOR32 | ILC_MASK, 0, -1);

	m_nDropIndex = -1;
}

CResourceListCtrlEx::~CResourceListCtrlEx()
{
}

LRESULT CResourceListCtrlEx::OnDropMessage(WPARAM wParam, LPARAM lParam)
{
	SetItemState( m_nDropIndex, 0, LVIS_DROPHILITED );

	return 0;
}

LRESULT CResourceListCtrlEx::OnDragMessage(WPARAM wParam, LPARAM lParam)
{
	DRAGOBJECTINFO *pInfo = (DRAGOBJECTINFO*)lParam;

	// turn off hilight for previous drop target
	SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);
	// redraw previous item
	RedrawItems (m_nDropIndex, m_nDropIndex);
	// get the item that is below cursor
	UINT uFlags;
	m_nDropIndex = HitTest( pInfo->xy, &uFlags );
	if (m_nDropIndex > -1)
	{
		// highlight it
		SetItemState( m_nDropIndex, LVIS_DROPHILITED, LVIS_DROPHILITED );
		// redraw item
		RedrawItems( m_nDropIndex, m_nDropIndex );
	}

	// set insert mark (-1 for no mark)
//	LVINSERTMARK mark;
//	mark.cbSize = sizeof(LVINSERTMARK);
//	mark.dwFlags = 0; // LVIM_AFTER; // or 0 for before
//	mark.iItem = m_nDropIndex;
//	mark.dwReserved = 0;
//	SetInsertMarkColor( RGB(0,0,0) );
//	SetInsertMark( &mark );

	UpdateWindow();

	return 0;
}

bool CResourceListCtrlEx::InsertResource( int iPositionIndex, const TCHAR *szFilePath )
{
	// check for duplicate resource, if dup then return
	for (int i=0; i < GetItemCount(); i++)
	{
		if (CString(szFilePath) == GetItemText( i, 0 ))
			return false;
	}

	std::auto_ptr<CBitmap> pBitmap;
	CBitmap * pThumbnail = CreateThumbnail(szFilePath);
	if (pThumbnail == NULL)
	{
		pBitmap = std::auto_ptr<CBitmap>(new CBitmap());
		pBitmap->CreateBitmap(0, 0, 1, 3, NULL);
	}else
	{
		pBitmap = std::auto_ptr<CBitmap>(pThumbnail);
	}

	int iImage = m_ThumbNails.Add( pBitmap.get(), RGB(0, 0, 0) );

	SetImageList( &m_ThumbNails, LVSIL_SMALL );
	SetImageList( &m_ThumbNails, LVSIL_NORMAL );

	// insert into list control
	if (iPositionIndex == -1)
		iPositionIndex = GetItemCount();
	
	if (InsertItem( LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, iPositionIndex, szFilePath, 0, 0, iImage, iPositionIndex ) == -1)
		return false;
	SortItems( CompareResourceItems, (LPARAM)this );
	ResynchSortValues();
	
	return true;
}

CBitmap * CResourceListCtrlEx::CreateThumbnail(const TCHAR *szFilePath)
{
	return NULL;
}

void CResourceListCtrlEx::RemoveResource( int iIndex )
{
	// get image index from list item
	int nImage = GetImageIndex( iIndex );

	// delete list item
	DeleteItem( iIndex );

	// remap image indices from nItem to Count
	for (int i=0; i < GetItemCount(); i++)
	{
		int temp = GetImageIndex( i );
		if (temp > nImage)
			SetImageIndex( i, temp - 1 );
	}

	// remove image
	m_ThumbNails.Remove( nImage );

	// update image list
	SetImageList( &m_ThumbNails, LVSIL_SMALL );
	SetImageList( &m_ThumbNails, LVSIL_NORMAL );

	// no sort needed since they remain sorted, just a resynch of sort values
	ResynchSortValues();
}

bool CResourceListCtrlEx::ResourceExists( const TCHAR *szFilePath )
{
	for (int i=0; i < GetItemCount(); i++)
	{
		if (CString(szFilePath) == GetItemText( i, 0 ))
			return true;
	}
	return false;
}

void CResourceListCtrlEx::ResynchSortValues()
{
	for (int i=0; i < GetItemCount(); i++)
	{
		LVITEM item;
		ZeroMemory( &item, sizeof(item) );
		item.mask = LVIF_PARAM;
		item.iItem = i;
		item.lParam = i;
		SetItem( &item );
	}
}

void CResourceListCtrlEx::CreateThumbnail(HBITMAP hSrc, CBitmap &tBitmap)
{
	CDC *pDC = GetDC();

	// create a memory DC to blit into
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	tBitmap.CreateDiscardableBitmap(pDC, IMAGESIZEX, IMAGESIZEY);

	if (hSrc)
	{
		// get diminension of source bitmap
		BITMAP bm;
		GetObject(hSrc, sizeof(BITMAP), &bm);

		CBitmap *oldBitmap = memDC.SelectObject(&tBitmap);
		memDC.FillSolidRect(0, 0, IMAGESIZEX, IMAGESIZEY, pDC->GetBkColor());

		int offX = (IMAGESIZEX-bm.bmWidth)/2;
		if (offX < 0)
			offX = 0;
		int offY = (IMAGESIZEY-bm.bmHeight)/2;
		if (offY < 0)
			offY = 0;

		CDC srcMemDC;
		srcMemDC.CreateCompatibleDC(pDC);
		srcMemDC.SelectObject(hSrc);
		memDC.BitBlt(offX, offY, bm.bmWidth, bm.bmHeight, 
			&srcMemDC, 0, 0, SRCCOPY); 

		memDC.SelectObject(oldBitmap);
	}
	else
		memDC.FillSolidRect(0, 0, IMAGESIZEX, IMAGESIZEY, RGB(255,255,255));

	ReleaseDC(pDC);
}

int CResourceListCtrlEx::GetImageIndex( int iItem )
{
	// get image index from list item
	LVITEM item;
	ZeroMemory( &item, sizeof(item) );
	item.mask = LVIF_IMAGE;
	item.iItem = iItem;
	GetItem( &item );
	return item.iImage;
}

void CResourceListCtrlEx::SetImageIndex( int iItem, int iImage )
{
	// get image index from list item
	LVITEM item;
	ZeroMemory( &item, sizeof(item) );
	item.mask = LVIF_IMAGE;
	item.iItem = iItem;
	item.iImage = iImage;
	SetItem( &item );
}


void CResourceListCtrlEx::CalculateVisibleItems()
{
	RECT rect;
	POINT pt;
	POINT origin_pt;
	// get the size of the client view 
	GetClientRect(&rect);

	int iCount = GetItemCount();
	int success = GetOrigin(&origin_pt);
	
	ASSERT(success);

	rect.top += origin_pt.y;
	rect.bottom += origin_pt.y;
	rect.left += origin_pt.x;
	rect.right += origin_pt.x;

	// clear the visible items list
	m_VisibleItems.clear();


	// loop through all items in the CListCtrl
	for (int i = 0; i<iCount; i++)
	{
		CRect crect = CRect(&rect);
		GetItemPosition(i, &pt);
		// check if the item is visible
		if ( crect.PtInRect(pt) )
		{
			m_VisibleItems.push_back( i );
		}
	}
}

INTEGERLIST CResourceListCtrlEx::GetVisibleItems()
{
	CalculateVisibleItems();
	return m_VisibleItems;
}

BOOL CResourceListCtrlEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	UNUSED_ALWAYS(pDC);
	//return CListCtrl::OnEraseBkgnd(pDC);
	return TRUE;
}

void CResourceListCtrlEx::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);

    GetClientRect(m_rectClient);

    CHeaderCtrl* pHC;
    pHC = GetHeaderCtrl();
    if (pHC != NULL)
    {
        CRect rectHeader;
        pHC->GetItemRect( 0, &rectHeader );
        m_rectClient.top += rectHeader.bottom;
    }
}

void CResourceListCtrlEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CListCtrl::OnPaint() for painting messages
	CMemDC memDC(&dc, &m_rectClient);

    // Let the window do its default painting...
    CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}

void CResourceListCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);

	// notify parent
	GetParent()->SendMessage( WM_VSCROLL, MAKEWPARAM(nSBCode, nPos) );
}

void CResourceListCtrlEx::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CListCtrl::OnKeyUp(nChar, nRepCnt, nFlags);

	// notify parent
	GetParent()->SendMessage( WM_KEYUP, (WPARAM)nChar, NULL );
}

BOOL CResourceListCtrlEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	BOOL bVal = CListCtrl::OnMouseWheel(nFlags, zDelta, pt);

	// notify parent
	GetParent()->SendMessage( WM_MOUSEWHEEL, MAKEWPARAM(zDelta, nFlags), MAKELPARAM(pt.x, pt.y) );

	return bVal;
}
