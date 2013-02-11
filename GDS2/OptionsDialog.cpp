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

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog dialog


BEGIN_MESSAGE_MAP(COptionsDialog, CPropertySheet)
	//{{AFX_MSG_MAP(COptionsDialog)
	ON_WM_CLOSE()
	// This one was missing, and that's why the window handlers were killed
	// automatically.
	ON_COMMAND( IDCANCEL , OnCancelButtonClick )
	ON_COMMAND( IDOK , OnOkButtonClick )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COptionsDialog::COptionsDialog() : 
	CPropertySheet()
{
	//{{AFX_DATA_INIT(COptionsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
}


void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CPropertySheet::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog message handlers

// message handler of the property sheet. If it is not overwritten, the 
// property sheet gets removed on close ButtonClick.
void COptionsDialog::OnCancelButtonClick() 
{
	//Call the cancel function for all the pages.
	for ( int i = 0; i < GetPageCount(); i++)
	{
		GetPage(i)->OnCancel();
	}

	// hide the window
	ShowWindow(SW_HIDE);	
}

void COptionsDialog::OnOkButtonClick() 
{
	//Call the ok function for all the pages.
	for ( int i = 0; i < GetPageCount(); i++)
	{
		GetPage(i)->OnOK();
	}

	// hide the window
	ShowWindow(SW_HIDE);	
}

// This part comes from the CPropertySheet reference. This is the way
// to creat modeless property sheet with ok, cancel and help buttons.
BOOL COptionsDialog::OnInitDialog() 
{
	m_bModeless = FALSE;   
	m_nFlags |= WF_CONTINUEMODAL;

	BOOL bResult = CPropertySheet::OnInitDialog();

	m_bModeless = TRUE;
	m_nFlags &= ~WF_CONTINUEMODAL;

	// not sure why, but if the caption is only set in the visual editor, this window
	// won't have the title.
	SetWindowText( "Options" );
	return bResult;  
}
