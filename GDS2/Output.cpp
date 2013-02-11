// Output.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutput dialog
const UINT SelectPageMessage = RegisterWindowMessage(SELECT_OUTPUT_PAGE_MSG);
const UINT ClearPageMessage = RegisterWindowMessage(CLEAR_OUTPUT_PAGE_MSG);
const UINT PrintLineMessage = RegisterWindowMessage(PRINT_LINE_MSG);

COutput::COutput()
{
	//{{AFX_DATA_INIT(COutput)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// add the property sheets
	m_PropSheet.AddPage(&m_BuildOutput);
	m_PropSheet.AddPage(&m_DebugOutput);
	m_PropSheet.AddPage(&m_FindOutput);

}

COutput::~COutput()
{
}

void COutput::DoDataExchange(CDataExchange* pDX)
{
	CSizingControlBarG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutput)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COutput, CSizingControlBarG)
	//{{AFX_MSG_MAP(COutput)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(SelectPageMessage, OnSelectPage)
	ON_REGISTERED_MESSAGE(ClearPageMessage, OnClearPage)
	ON_REGISTERED_MESSAGE(PrintLineMessage, OnPrintLine)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutput message handlers

int COutput::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
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
}

void COutput::OnSize(UINT nType, int cx, int cy) 
{
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
	
}


//=====================================================================
// Function:	SelectPage
// Description:	Selects the current output page
// Parameters:	lParam = page number of output panes
// Returns:		TRUE
//=====================================================================
LRESULT COutput::OnSelectPage(WPARAM wParam, LPARAM lParam)
{
	m_PropSheet.SetActivePage((UINT)lParam);
	return TRUE;
}

//=====================================================================
// Function:	ClearPage
// Description:	Clears the ouput pane
// Parameters:	lParam = page number of output panes
// Returns:		TRUE if handled
//=====================================================================
LRESULT COutput::OnClearPage(WPARAM wParam, LPARAM lParam)
{
	CPropertyPage *pPage = m_PropSheet.GetPage((UINT)lParam);
	if (pPage == NULL)
	{
		return FALSE;
	}
	pPage->OnReset();
	return TRUE;
}

//=====================================================================
// Function:	PrintLine
// Description:	Sends message to output pane to print a line
// Parameters:	lParam = pointer to PRINTSTRUCT
// Returns:		.
//=====================================================================
LRESULT COutput::OnPrintLine(WPARAM wParam, LPARAM lParam)
{
	PRINTSTRUCT *printInfo;
	printInfo = (PRINTSTRUCT *)lParam;
	CPropertyPage *pPage;

	pPage = (CPropertyPage *)m_PropSheet.GetPage(printInfo->pageNum);
	if (pPage == NULL)
		return FALSE;
	pPage->SendMessage(PrintLineMessage, 0, (LPARAM)printInfo->outString);
	return TRUE;
}
