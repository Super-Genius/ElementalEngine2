//-----------------------------------------------------------------------------
//
// File:          CObjectPage.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CObjectPage'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'RemoveObject'
*     Added method 'OnSize'
*     Added method 'OnMouseMove'
*     Added method 'OnLButtonUp'
*     Added method 'OnInitDialog'
*     Added method 'OnBegindragObjectsList'
*     Added method 'DoDataExchange'
*     Added method 'ClearObjects'
*     Added method 'AddObject'
*     Added method '~CObjectPage'
*     Added method 'CObjectPage'
*     Added inheritance 'CPropertyPage'
*     Added member 'm_pDropWnd'
*     Added member 'm_nDropIndex'
*     Added member 'm_bDragging'
*     Added member 'm_pDragImage'
*     Added member 'm_ThumbNails'
*     Added member 'm_ObjectsList'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

//@START_USER1
//@END_USER1


// Master include file
#include "StdAfx.h"


//@START_USER2
//@END_USER2




/*@NOTE_3820
Constructor method.
*/
CObjectPage::CObjectPage() //@INIT_3820
: CPropertyPage(CObjectPage::IDD)
{//@CODE_3820

	//{{AFX_DATA_INIT(CObjectPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDragImage = NULL;
}//@CODE_3820


/*@NOTE_3816
Destructor method.
*/
CObjectPage::~CObjectPage()
{//@CODE_3816

	if (m_pDragImage)
	{
		delete m_pDragImage;
		m_pDragImage = NULL;
	}
}//@CODE_3816


void CObjectPage::AddObject(OBJECTSTRUCT* rs)
{//@CODE_3821

	UINT nItems;
	int insertedItem;

	if (os->thumbNail != NULL)
	{
		m_ObjectsList.ModifyStyle(0, LVS_ICON);
		m_ThumbNails.Add(os->thumbNail, RGB(0, 0, 0));
		m_ObjectsList.SetImageList(&m_ThumbNails, LVSIL_NORMAL);
		nItems = m_ObjectsList.GetItemCount();
		if (os->name == NULL)
			insertedItem = m_ObjectsList.InsertItem(nItems, "", nItems);
		else
			insertedItem = m_ObjectsList.InsertItem(nItems, *os->name, nItems);

		m_ObjectsList.SetItemData(insertedItem, (DWORD)os->object);
	}
	else // assume for now that if there is no thumbnail, we are in list mode
	{
		m_ObjectsList.ModifyStyle(0, LVS_LIST);
		insertedItem = m_ObjectsList.InsertItem(m_ObjectsList.GetItemCount(), *os->name);
		m_ObjectsList.SetItemData(insertedItem, (DWORD)os->object);
	}
}//@CODE_3821


void CObjectPage::ClearObjects()
{//@CODE_3825

}//@CODE_3825


void CObjectPage::DoDataExchange(CDataExchange* pDX)
{//@CODE_3828

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectPage)
	DDX_Control(pDX, IDC_OBJECTS_LIST, m_ObjectsList);
	//}}AFX_DATA_MAP
}//@CODE_3828


afx_msg void CObjectPage::OnBegindragObjectsList(NMHDR* pNMHDR,
                                                 LRESULT* pResult)
{//@CODE_3842

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// save the index of the item being dragged in m_nDragIndex
	m_nDragIndex = pNMListView->iItem;
	POINT pt;
	pt.x = 8;
	pt.y = 8;
	// create a drag image

	if (m_pDragImage)
		delete m_pDragImage;
	
	m_pDragImage = m_ObjectsList.CreateDragImage(m_nDragIndex, &pt);
	ASSERT (m_pDragImage);
	// changes the cursor to the drag image (DragMove() is still required in 
	// OnMouseMove())
	VERIFY(m_pDragImage->BeginDrag(0, CPoint (8, 8)));
	VERIFY(m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction));
	// set dragging flag
	m_bDragging = TRUE;
	m_nDropIndex = -1;
	m_pDropWnd = NULL;
	// capture all mouse messages
	SetCapture();

	*pResult = 0;
}//@CODE_3842


BOOL CObjectPage::OnInitDialog()
{//@CODE_3841

	CPropertyPage::OnInitDialog();
	
	m_ThumbNails.Create(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT,
		ILC_COLOR24, 0, 65535);
	m_ObjectsList.SetImageList(&m_ThumbNails, LVSIL_NORMAL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}//@CODE_3841


afx_msg void CObjectPage::OnLButtonUp(UINT nFlags, CPoint point)
{//@CODE_3847

	CWnd* pDropWnd;
	if (m_bDragging)
	{
		// release mouse capture
		VERIFY (::ReleaseCapture());
		m_bDragging = FALSE;
		// end dragging
		VERIFY (m_pDragImage->DragLeave(GetDesktopWindow()));
		m_pDragImage->EndDrag();

		CPoint pt(point);
		ClientToScreen(&pt);
		// get the CWnd pointer of the window that is under the mouse cursor
		pDropWnd = WindowFromPoint(pt);
		if (pDropWnd == NULL)
			return;
		DROPOBJECTINFO doInfo;
		doInfo.object = (CBaseObject *)m_ObjectsList.GetItemData(m_nDragIndex);
		doInfo.xy = pt;
		pDropWnd->SendMessage(ObjectDropMessage, 0, (LPARAM)&doInfo);
	}
	else
	{
		CPropertyPage::OnLButtonUp(nFlags, point);
	}
}//@CODE_3847


afx_msg void CObjectPage::OnMouseMove(UINT nFlags, CPoint point)
{//@CODE_3851

	if (m_bDragging)
	{
		CPoint pt(point);
		ClientToScreen(&pt);
		// move the drag image
		VERIFY(m_pDragImage->DragMove(pt));
		// unlock window updates
		VERIFY(m_pDragImage->DragShowNolock(FALSE));

		// save current window pointer
		m_pDropWnd = WindowFromPoint(pt);

		// convert from screen coordinates to drop target client coordinates
		m_pDropWnd->ScreenToClient(&pt);

		// lock window updates
		VERIFY(m_pDragImage->DragShowNolock(TRUE));
	}
		
	CPropertyPage::OnMouseMove(nFlags, point);
}//@CODE_3851


afx_msg void CObjectPage::OnSize(UINT nType, int cx, int cy)
{//@CODE_3837

	CRect rect;

	CPropertyPage::OnSize(nType, cx, cy);

	GetClientRect(rect);
	if (m_ObjectsList.m_hWnd != NULL)
	{
		m_ObjectsList.MoveWindow(rect);
	}
		
}//@CODE_3837


void CObjectPage::RemoveObject(UINT index)
{//@CODE_3823

}//@CODE_3823


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3817
Method which must be called first in a constructor.
*/
void CObjectPage::ConstructorInclude()
{
}


/*@NOTE_3818
Method which must be called first in a destructor.
*/
void CObjectPage::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
