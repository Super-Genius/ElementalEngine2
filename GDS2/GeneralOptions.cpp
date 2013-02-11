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
#include "GeneralOptions.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CGeneralOptions, CPropertyPage)
CGeneralOptions::CGeneralOptions()
	: CPropertyPage(CGeneralOptions::IDD)
{
}

CGeneralOptions::~CGeneralOptions()
{
}

void CGeneralOptions::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GENERALOPT_EDIT, m_EditDirCtrl);
	DDX_Control(pDX, IDC_TOOLBAR_DELAY, m_trackToolbarDelay);
}

BEGIN_MESSAGE_MAP(CGeneralOptions, CPropertyPage)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_GENERALOPT_BROWSE, OnBnClickedGeneraloptBrowse)
END_MESSAGE_MAP()


// CGeneralOptions message handlers

BOOL CGeneralOptions::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//m_EditDirCtrl.SetWindowText( StdString(theApp.GetResourcePath()) );
	StdString BaseDirectory;
	StdString ResourceDirectory;
	EngineGetToolBox()->GetDirectories( &BaseDirectory, &ResourceDirectory );
	m_EditDirCtrl.SetWindowText( ResourceDirectory ); 

	// set range from 0 to 10000 ms with 100 ms step
	m_trackToolbarDelay.SetRange( 0, 100 );
	m_trackToolbarDelay.SetTicFreq( 2 );
	m_trackToolbarDelay.SetPos( CGuiDockToolBarEx::GetScrollDelay() / 100 );

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CGeneralOptions::OnOK()
{

	// resource path is not set on the options page any more
	// set it in the project settings
/*	CString szNewPath;
	m_EditDirCtrl.GetWindowText( szNewPath );
	CString szPrevPath = theApp.GetResourcePath();

	if (szNewPath != szPrevPath)
	{
		MessageBox( _T("Application must be restarted for changes to take effect.\n") );
		theApp.SetResourcePath( szNewPath );
		// Why don't set the EE resource path?
		StdString sstdNewPath = szNewPath.GetString();
		EngineGetToolBox()->SetDirectories( NULL, &sstdNewPath );
	}
*/
	int nScrollDelay = m_trackToolbarDelay.GetPos() * 100;
	CGuiDockToolBarEx::SetScrollDelay( nScrollDelay );
	TCHAR chNum[32];
	_itot( nScrollDelay, chNum, 10 );
	WriteINIKey( INI_ToolbarScrollDelay, chNum, INI_SubSection );

	GetParent()->ShowWindow(SW_HIDE);
}

void CGeneralOptions::OnCancel()
{
	m_EditDirCtrl.SetWindowText( StdString(theApp.GetResourcePath()) );
}

BOOL CGeneralOptions::OnApply()
{
	// do validation here.
	//return CPropertyPage::OnApply();
	OnOK();

	// returning TRUE caused the handlers to be destroyed.
	return FALSE;
}

void CGeneralOptions::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

void CGeneralOptions::OnBnClickedGeneraloptBrowse()
{
	// TODO: Add your control notification handler code here
	OleInitialize( NULL );

	LPITEMIDLIST  pidl;
	TCHAR szFolderPath[MAX_PATH+1];
	m_EditDirCtrl.GetWindowText( szFolderPath, MAX_PATH );

	BROWSEINFO bi;
	bi.hwndOwner = GetSafeHwnd();
	bi.lpszTitle = _T("Browse for base directory");
	bi.pszDisplayName = szFolderPath;
	bi.pidlRoot = NULL;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	bi.iImage = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	if ((pidl = SHBrowseForFolder(&bi)) != NULL)
	{
		SHGetPathFromIDList(pidl, szFolderPath);
		m_EditDirCtrl.SetWindowText( szFolderPath );
	}

	OleUninitialize();
}


void CGeneralOptions::updateInformation()
{
	// we should update the window data.
		
	StdString BaseDirectory;
	StdString ResourceDirectory;
	EngineGetToolBox()->GetDirectories( &BaseDirectory, &ResourceDirectory );
	m_EditDirCtrl.SetWindowText( ResourceDirectory ); 

	return;
}