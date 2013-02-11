///============================================================================
/// \file		ProjectEditorPropertySheet.cpp
/// \brief		Declaration of the new project settings property sheet.
/// \date		01-03-2009
/// \author		Zsolt Matyas
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

#include "StdAfx.h"
#include "ProjectEditorPropertySheet.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CProjectEditorPropertySheet, CPropertySheet)

BEGIN_MESSAGE_MAP(CProjectEditorPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CProjectEditorPropertySheet)
	ON_WM_CLOSE()
	// This one was missing, and that's why the window handlers were killed
	// automatically.
	ON_COMMAND( IDCANCEL , OnCancelButtonClick )
	ON_COMMAND( IDOK , OnOkButtonClick )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CProjectEditorPropertySheet::CProjectEditorPropertySheet( LPCTSTR pszCaption,
   CWnd* pParentWnd = NULL )
	: CPropertySheet( pszCaption,pParentWnd )
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
}

CProjectEditorPropertySheet::~CProjectEditorPropertySheet(void)
{
}



void CProjectEditorPropertySheet::DoDataExchange(CDataExchange* pDX)
{
	CPropertySheet::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void CProjectEditorPropertySheet::OnCancelButtonClick() 
{
	//Call the function for all the pages.
	for ( int i = 0; i < GetPageCount(); i++)
	{
		GetPage(i)->OnCancel();
	}

	// restore modeless state 
	SetModelessProperty( TRUE );

	// hide the window
	ShowWindow(SW_HIDE);	
}

 void CProjectEditorPropertySheet::RefreshPageValues()
{
	//Call the refresh function for all the pages.
	for ( int i = 0; i < GetPageCount(); i++)
	{
		CPropertyPage* currentPropPage = GetPage(i);
		IProjectSettingsPropertyPage *currentPage = DYNAMIC_DOWNCAST( IProjectSettingsPropertyPage, currentPropPage );

		currentPage->RefreshPageValues();
	}
		
}


void CProjectEditorPropertySheet::OnOkButtonClick() 
{
	//Call the validate function for all the pages.
	for ( int i = 0; i < GetPageCount(); i++)
	{
		CPropertyPage* currentPropPage = GetPage(i);
		CProjectEditorDefaultPropertyPage *currentPage = DYNAMIC_DOWNCAST( CProjectEditorDefaultPropertyPage, currentPropPage );

		// we call validate for the current page. it returns false if any value 
		// is invalid
		if( !currentPage->ValidateValues() )
		{
			// validateValues shows the error messages
			// we just exit this function, so no changes is made...
			BringWindowToTop();
			return;
		}
	}

	//Call the ok function for all the pages.
	for ( int i = 0; i < GetPageCount(); i++)
	{
		GetPage(i)->OnOK();
	}

	// enable the main window - we dont act as modal property sheet any more
	SetModelessProperty( TRUE );

	// hide the window
	ShowWindow(SW_HIDE);	
	
}

// This part comes from the CPropertySheet reference. This is the way
// to creat modeless property sheet with ok, cancel and help buttons.
BOOL CProjectEditorPropertySheet::OnInitDialog() 
{
	m_bModeless = FALSE;   
	m_nFlags |= WF_CONTINUEMODAL;

	BOOL bResult = CPropertySheet::OnInitDialog();

	m_bModeless = TRUE;
	m_nFlags &= ~WF_CONTINUEMODAL;

	// set the title 
	SetWindowText( "Project Specific Options" );

	return bResult;  
}

BOOL CProjectEditorPropertySheet::SetModelessProperty(BOOL newState)
{
	// we disable the main window, and make this property sheet the only available.
	// this way it is working as a modal dialog

	if ( newState )
	{
		AfxGetApp()->DoEnableModeless(TRUE);
		// this sets the dialog modeless 
		AfxGetMainWnd()->EnableWindow( TRUE );
		
		// this flags would be set automatically in a modal dialog, but they are not
		// as we do not call doModal(). But we want to use the loop of the modal dialogs
		// to disable main window when this property sheet is visible. So we set them
		// manually:
		m_nFlags &= ~(WF_MODALLOOP|WF_CONTINUEMODAL);
	}
	else
	{
		AfxGetApp()->DoEnableModeless(FALSE);
		AfxGetMainWnd()->EnableWindow( FALSE );
	}
	return TRUE;
}



