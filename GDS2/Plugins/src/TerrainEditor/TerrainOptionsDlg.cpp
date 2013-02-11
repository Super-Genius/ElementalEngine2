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

// CTerrainOptionsDlg dialog

IMPLEMENT_DYNAMIC(CTerrainOptionsDlg, CPropertyPage)
CTerrainOptionsDlg::CTerrainOptionsDlg()
	: CPropertyPage(CTerrainOptionsDlg::IDD)
{
	CWinApp *app = AfxGetApp();
	if (app)
		m_iMaxLayers = app->GetProfileInt( _T("TerrainOptions"), _T("Max Layers"), 6 );
	else
		m_iMaxLayers = 6;

	m_iTempMaxLayers = m_iMaxLayers;
}

CTerrainOptionsDlg::~CTerrainOptionsDlg()
{
	CWinApp *app = AfxGetApp();
	if (app)
		app->WriteProfileInt( _T("TerrainOptions"), _T("Max Layers"), m_iMaxLayers );
}

void CTerrainOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TERRAIN_OPT_MAXLAYER_EDIT, m_iTempMaxLayers);
}


BEGIN_MESSAGE_MAP(CTerrainOptionsDlg, CPropertyPage)
	ON_EN_CHANGE(IDC_TERRAIN_OPT_MAXLAYER_EDIT, OnEnChangeTerrainOptMaxlayerEdit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TERRAIN_OPT_MAXLAYER_SPIN, OnDeltaposTerrainOptMaxlayerSpin)
END_MESSAGE_MAP()


// CTerrainOptionsDlg message handlers

void CTerrainOptionsDlg::OnOK()
{
	m_iMaxLayers = m_iTempMaxLayers;
	// calling OnOK() would close the property sheet, we only want to hide it
//	CPropertyPage::OnOK();
}

void CTerrainOptionsDlg::OnCancel()
{
	//m_iTempMaxLayers = m_iMaxLayers;
	//UpdateData( FALSE );
	// calling OnCancel() would close the property sheet, we only want to hide it
//	CPropertyPage::OnCancel();
}

void CTerrainOptionsDlg::OnEnChangeTerrainOptMaxlayerEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CEdit *pEdit = (CEdit*)GetDlgItem( IDC_TERRAIN_OPT_MAXLAYER_EDIT );
	TCHAR szBuffer[256];
	szBuffer[0] = '\0';
	pEdit->GetWindowText( szBuffer, 256 );
	m_iTempMaxLayers = atoi( StdString(szBuffer) );

	if (m_iTempMaxLayers < 1)
	{
		m_iTempMaxLayers = 1;
		pEdit->SetWindowText( _T("1") );
	}
}

void CTerrainOptionsDlg::OnDeltaposTerrainOptMaxlayerSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	m_iTempMaxLayers += pNMUpDown->iDelta;

	if (m_iTempMaxLayers < 1)
		m_iTempMaxLayers = 1;

	*pResult = 0;
	UpdateData(FALSE);
}
