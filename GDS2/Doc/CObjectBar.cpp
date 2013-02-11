//-----------------------------------------------------------------------------
//
// File:          CObjectBar.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CObjectBar'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'OnSize'
*     Added method 'OnRemoveObject'
*     Added method 'OnCreate'
*     Added method 'OnClearObjects'
*     Added method 'OnAddObject'
*     Added method 'InitObjectPages'
*     Added method '~CObjectBar'
*     Added method 'CObjectBar'
*     Added inheritance 'CSizingControlBarG'
*     Added member 'm_Icons'
*     Added member 'm_PropSheet'
*     Added member 'obOptions'
*     Added member 'm_ObjectPages'
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




/*@NOTE_3752
Constructor method.
*/
CObjectBar::CObjectBar() //@INIT_3752
{//@CODE_3752

	obOptions = NULL;
}//@CODE_3752


/*@NOTE_3746
Destructor method.
*/
CObjectBar::~CObjectBar()
{//@CODE_3746

	vector<CObjectPage *>::iterator itRPage;

	itRPage = m_ObjectPages.begin();
	while (itRPage != m_ObjectPages.end())
	{
		delete *itRPage;
		itRPage++;
	}

	if (obOptions != NULL)
	{
		delete obOptions;
		obOptions = NULL;
	}
}//@CODE_3746


/*@NOTE_3759
Initialize all the object tab pages on the object bar.
*/
void CObjectBar::InitObjectPages()
{//@CODE_3759

	CObjectPage *rPage;
	list<CDLLInterface *>::iterator itRI;
	list<CDLLInterface *>::iterator itRIend;
	CObjectDLLInterface *objDLL;
	int retVal;
	DWORD resourceDLLCount;

	resourceDLLCount = 0;
	for (itRI = theApp.m_DLLS.begin(); itRI != theApp.m_DLLS.end(); itRI++)
	{
        if ((*itRI)->m_Type == DLLTYPE_OBJECT)
        {
    		objDLL = (CObjectDLLInterface *)*itRI;
	    	retVal = IDRETRY;
		    while (retVal == IDRETRY)
		    {
			    rPage = new CObjectPage;
			    if (rPage == NULL)
			    {
				    retVal = MessageBox(_T("Unable to allocate resource page\n"),
					    _T("Memory Error"), MB_ABORTRETRYIGNORE);
				    if (retVal == IDABORT)
				    {
					    // immediatly exit?
					    AfxGetApp()->ExitInstance();
				    }
			    }
			    else
			    {
				    retVal = IDIGNORE;
			    }
		    }

#ifdef _DEBUG
		    // somebody forgot to fill this in
		    ASSERT(_CrtIsValidPointer(objDLL->m_ObjInfo.pageIndex, sizeof(UINT), TRUE));
#endif
		    if (objDLL->m_ObjInfo.pageIndex != NULL)
			    *(objDLL->m_ObjInfo.pageIndex) = m_ObjectPages.size();
		    // save off for deleting upon destruction
		    m_ObjectPages.push_back(rPage);
		    // add in resource dialog page for resource dll
		    m_PropSheet.AddPage(rPage);
		    // bump to next page
		    // bump count for image list creation
		    resourceDLLCount++;
        }
	}

	if (resourceDLLCount != 0)
	{
		m_Icons.Create(16, 16, ILC_COLORDDB | ILC_MASK, resourceDLLCount, 0);
	}
	else
	{
		MessageBox(_T("No Plugins found.\n"
				"Current incarnation of GDS assumes Plugins directory\n"
				"Directly beneath executable directory\n"
				"gds will not function correctly"), _T("Location Error!"),
				MB_OK);
	}
}//@CODE_3759


/*@NOTE_3766
Message handler for adding objects to the object bar.
*/
afx_msg LRESULT CObjectBar::OnAddObject(WPARAM wParam, LPARAM lParam)
{//@CODE_3766

	OBJECTSTRUCT *os;
	CObjectPage *destPage;

	os = (OBJECTSTRUCT *)lParam;
	ASSERT(*(os->pageIndex) < m_ObjectPages.size());
	destPage = m_ObjectPages[*(os->pageIndex)];
	m_PropSheet.SetActivePage(destPage);
	destPage->AddObject(os);

	return TRUE;
}//@CODE_3766


/*@NOTE_3772
Clear all objects from the object bar.
*/
afx_msg LRESULT CObjectBar::OnClearObjects(WPARAM wParam, LPARAM lParam)
{//@CODE_3772

	OBJECTSTRUCT *os;
	CObjectPage *destPage;

	os = (OBJECTSTRUCT *)lParam;
	ASSERT(*(os->pageIndex) < m_ObjectPages.size());
	destPage = m_ObjectPages[*(os->pageIndex)];
	destPage->ClearObjects();

	return TRUE;
}//@CODE_3772


afx_msg int CObjectBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{//@CODE_3760

	CTabCtrl* tab;
	TCITEM tItem;
	CBitmap tempBM;
	list<CDLLInterface *>::iterator it;
	DWORD index = 0;

	if (CSizingControlBarG::OnCreate(lpCreateStruct) == -1)
		return -1;

	obOptions = new CObjectOptions;

	// this has to be here as the DLLS may need access to
	// the main window
	InitObjectPages();

	// create the property sheet
	if ( m_PropSheet.Create(this, WS_CHILD | WS_VISIBLE, 0) )
	{
		tab = m_PropSheet.GetTabControl();

		if ( tab )
		{
			DWORD temp = tab->GetStyle();
			// Put the tabs at the bottom
			tab->ModifyStyle(WS_BORDER | WS_THICKFRAME | WS_DLGFRAME | WS_CAPTION,
				TCS_BOTTOM | WS_CHILD);

			// go through the DLLS and get their icon resources
			index = 0;
			for (it = theApp.m_DLLS.begin(); it != theApp.m_DLLS.end(); it++)
			{
				CObjectDLLInterface *curRI;
                if ((*it)->m_Type == DLLTYPE_OBJECT)
                {
    				curRI = (CObjectDLLInterface *)*it;
    
				    m_Icons.Add(curRI->m_ObjInfo.icon);
				    // now set the tab strings/icons
				    tItem.pszText = (LPTSTR)(LPCTSTR)curRI->m_ObjInfo.name;
				    tItem.iImage = index;
				    tItem.mask = TCIF_TEXT | TCIF_IMAGE;
				    tab->SetItem(index, &tItem);
				    // bump index
				    index++;
			    }
            }

			// we need to load images here.
			tab->SetImageList(&m_Icons);
		}
	}

	SetBorders(4, 4, 6, 4);
	return 0;
}//@CODE_3760


/*@NOTE_3769
Message handler to remove and object from the message bar.
*/
afx_msg LRESULT CObjectBar::OnRemoveObject(WPARAM wParam, LPARAM lParam)
{//@CODE_3769

	OBJECTSTRUCT *os;
	CObjectPage *destPage;

	os = (OBJECTSTRUCT *)lParam;
	ASSERT(*(os->pageIndex) < m_ObjectPages.size());
	destPage = m_ObjectPages[*(os->pageIndex)];
	destPage->RemoveObject(*(os->rsrcIndex));

	return TRUE;
}//@CODE_3769


/*@NOTE_3762
Message handler for resizing of the dockable bars.
*/
afx_msg void CObjectBar::OnSize(UINT nType, int cx, int cy)
{//@CODE_3762

	CTabCtrl* tab;
	CRect rect, tabRect;

	CSizingControlBarG::OnSize(nType, cx, cy);

	GetClientRect(rect);

	// wierd sometimes we get an size message before create?
	if (m_PropSheet.m_hWnd != NULL)
	{
		m_PropSheet.MoveWindow(rect);
		m_PropSheet.GetClientRect(tabRect);
		tab = m_PropSheet.GetTabControl();
		if ( tab )
		{
			tab->MoveWindow(rect);

			// adjust PopertyPage inside of CTabCtrl
			tab->AdjustRect(FALSE, rect);
		}

		CPropertyPage *pPage = m_PropSheet.GetActivePage();
		if (pPage)
		{
			pPage->MoveWindow(rect);
		}
	}
	
}//@CODE_3762


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3747
Method which must be called first in a constructor.
*/
void CObjectBar::ConstructorInclude()
{
}


/*@NOTE_3748
Method which must be called first in a destructor.
*/
void CObjectBar::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
