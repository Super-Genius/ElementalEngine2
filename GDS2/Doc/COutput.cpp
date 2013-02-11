//-----------------------------------------------------------------------------
//
// File:          COutput.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'COutput'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'OnSize'
*     Added method 'OnSelectPage'
*     Added method 'OnPrintLine'
*     Added method 'OnCreate'
*     Added method 'OnClearPage'
*     Added method 'DoDataExchange'
*     Added method '~COutput'
*     Added method 'COutput'
*     Added inheritance 'CSizingControlBarG'
*     Added member 'm_FindOutput'
*     Added member 'm_DebugOutput'
*     Added member 'm_BuildOutput'
*     Added member 'm_PropSheet'
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




/*@NOTE_3871
Constructor method.
*/
COutput::COutput() //@INIT_3871
{//@CODE_3871

	//{{AFX_DATA_INIT(COutput)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// add the property sheets
	m_PropSheet.AddPage(&m_BuildOutput);
	m_PropSheet.AddPage(&m_DebugOutput);
	m_PropSheet.AddPage(&m_FindOutput);

}//@CODE_3871


/*@NOTE_3867
Destructor method.
*/
COutput::~COutput()
{//@CODE_3867

}//@CODE_3867


void COutput::DoDataExchange(CDataExchange* pDX)
{//@CODE_3872

	CSizingControlBarG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutput)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}//@CODE_3872


afx_msg LRESULT COutput::OnClearPage(WPARAM wParam, LPARAM lParam)
{//@CODE_3890

	CPropertyPage *pPage = m_PropSheet.GetPage((UINT)lParam);
	if (pPage == NULL)
	{
		return FALSE;
	}
	pPage->OnReset();
	return TRUE;
}//@CODE_3890


afx_msg int COutput::OnCreate(LPCREATESTRUCT lpCreateStruct)
{//@CODE_3881

	CTabCtrl* tab;
	TCITEM tItem;

	if (CSizingControlBarG::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create the property sheet
	m_PropSheet.Create(this, WS_CHILD | WS_VISIBLE, 0);

	tab = m_PropSheet.GetTabControl();

	DWORD temp = tab->GetStyle();
	// Put the tabs at the bottom
	tab->ModifyStyle(WS_BORDER | WS_THICKFRAME | WS_DLGFRAME | WS_CAPTION | TCS_MULTILINE,
		TCS_BOTTOM | TCS_FIXEDWIDTH | WS_CHILD);

	// now set the tab strings/icons
	tItem.pszText = _T("Build");
	tItem.mask = TCIF_TEXT;
	tab->SetItem(0, &tItem);

	// now set the tab strings/icons
	tItem.pszText = _T("Debug");
	tItem.mask = TCIF_TEXT;
	tab->SetItem(1, &tItem);

	// now set the tab strings/icons
	tItem.pszText = _T("Find in Files");
	tItem.mask = TCIF_TEXT;
	tab->SetItem(2, &tItem);

	SetBorders(4, 4, 6, 4);
	return 0;
}//@CODE_3881


afx_msg LRESULT COutput::OnPrintLine(WPARAM wParam, LPARAM lParam)
{//@CODE_3893

	PRINTSTRUCT *printInfo;
	printInfo = (PRINTSTRUCT *)lParam;
	CPropertyPage *pPage;

	pPage = (CPropertyPage *)m_PropSheet.GetPage(printInfo->pageNum);
	if (pPage == NULL)
		return FALSE;
	pPage->SendMessage(PrintLineMessage, 0, (LPARAM)printInfo->outString);
	return TRUE;
}//@CODE_3893


afx_msg LRESULT COutput::OnSelectPage(WPARAM wParam, LPARAM lParam)
{//@CODE_3887

	m_PropSheet.SetActivePage((UINT)lParam);
	return TRUE;
}//@CODE_3887


afx_msg void COutput::OnSize(UINT nType, int cx, int cy)
{//@CODE_3883

	CTabCtrl* tab;
	CRect rect;

	CSizingControlBarG::OnSize(nType, cx, cy);

	GetClientRect(rect);

	m_PropSheet.MoveWindow(rect);
	m_PropSheet.GetClientRect(rect);
	tab = m_PropSheet.GetTabControl();
	tab->MoveWindow(rect);

	// adjust PopertyPage inside of CTabCtrl
	tab->AdjustRect(FALSE, rect);

	CPropertyPage *pPage = m_PropSheet.GetActivePage();
	if (pPage)
	{
		pPage->MoveWindow(rect);
	}
	
}//@CODE_3883


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3868
Method which must be called first in a constructor.
*/
void COutput::ConstructorInclude()
{
}


/*@NOTE_3869
Method which must be called first in a destructor.
*/
void COutput::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
