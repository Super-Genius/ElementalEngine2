///============================================================================
/// \file		ResourceView.cpp
/// \brief		Implementation file for ResourceView
/// \date		11-17-2005
/// \author		Halbert Nakagawa
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
#include "gds.h"
#include "ResourceView.h"
#include ".\resourceview.h"


// CResourceView

IMPLEMENT_DYNCREATE(CResourceView, CView)

CResourceView::CResourceView()
{
	m_pPropSheet = NULL;
	m_pIcons = NULL;
}

CResourceView::~CResourceView()
{
}

BEGIN_MESSAGE_MAP(CResourceView, CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CResourceView drawing

void CResourceView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


void CResourceView::OnInitialUpdate()
{	
	CTabCtrl* tab;
	TCITEM tItem;
	CBitmap tempBM;
	RESOURCECOLLECTIONLIST::iterator itObjCollList;
	RESOURCECOLLECTION *curObjCollection;
	RESOURCECOLLECTIONLIST ResourceCollections;
	DWORD index = 0;

	CView::OnInitialUpdate();

	CResourceDoc *pDoc = DYNAMIC_DOWNCAST(CResourceDoc, GetDocument());
	
	pDoc->InitResourcePages();
	ResourceCollections = pDoc->GetResourceCollection();
	
	m_pIcons = pDoc->GetImageList();
	m_pPropSheet = pDoc->GetPropertySheet();

	// create the property sheet
	if (m_pPropSheet->Create(this, WS_CHILD | WS_VISIBLE, 0) )
	{
		tab = m_pPropSheet->GetTabControl();

		if ( tab )
		{
			DWORD temp = tab->GetStyle();
			// Put the tabs at the bottom
			tab->ModifyStyle(WS_BORDER | WS_THICKFRAME | WS_DLGFRAME | WS_CAPTION,
				TCS_BOTTOM | WS_CHILD);

			// go through the Resource collections their icon handles
			index = 0;
			for (itObjCollList = ResourceCollections.begin(); 
					itObjCollList != ResourceCollections.end(); itObjCollList++)
			{
    			curObjCollection = (RESOURCECOLLECTION *)*itObjCollList;
    
				m_pIcons->Add(curObjCollection->hIcon);
				// now set the tab strings/icons
				tItem.pszText = (LPTSTR)(LPCTSTR)curObjCollection->name;
				tItem.iImage = index;
				tItem.mask = TCIF_TEXT | TCIF_IMAGE;
				tab->SetItem(index, &tItem);
				// bump index
				index++;
            }

			// we need to load images here.
			tab->SetImageList(m_pIcons);
		}
	}
}

void CResourceView::OnSize(UINT nType, int cx, int cy)
{
	CTabCtrl* tab;
	CRect rect, tabRect;

	CView::OnSize(nType, cx, cy);

	GetClientRect(rect);

	// wierd sometimes we get an size message before create?
	if (m_pPropSheet == NULL)
	{
		return;
	}

	if (m_pPropSheet->m_hWnd != NULL)
	{
		m_pPropSheet->MoveWindow(rect);
		m_pPropSheet->GetClientRect(tabRect);
		tab = m_pPropSheet->GetTabControl();
		if ( tab )
		{
			tab->MoveWindow(rect);

			// adjust PopertyPage inside of CTabCtrl
			tab->AdjustRect(FALSE, rect);
		}

		CPropertyPage *pPage = m_pPropSheet->GetActivePage();
		if (pPage)
		{
			pPage->MoveWindow(rect);
		}
	}
}

// CResourceView diagnostics

#ifdef _DEBUG
void CResourceView::AssertValid() const
{
	CView::AssertValid();
}

void CResourceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG
