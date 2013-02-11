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

// CGUISetBGColorBox dialog

IMPLEMENT_DYNAMIC(CGUISetBGColorBox, CDialog)
CGUISetBGColorBox::CGUISetBGColorBox(CWnd* pParent /*=NULL*/)
	: CDialog(CGUISetBGColorBox::IDD, pParent)
	, m_szColorChoice(_T(""))
	, m_iRVal(0)
	, m_iGVal(0)
	, m_iBVal(0)
{
	m_Doc = NULL;
}

CGUISetBGColorBox::~CGUISetBGColorBox()
{
}

void CGUISetBGColorBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLORS, m_ctColorCombo);
	DDX_CBString(pDX, IDC_COLORS, m_szColorChoice);
	DDX_Text(pDX, IDC_RVAL, m_iRVal);
	DDX_Text(pDX, IDC_GVAL, m_iGVal);
	DDX_Text(pDX, IDC_BVAL, m_iBVal);
}


BEGIN_MESSAGE_MAP(CGUISetBGColorBox, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COLORS, OnCbnSelchangeColors)
//	ON_CBN_EDITCHANGE(IDC_COLORS, OnCbnEditchangeColors)
//	ON_CBN_EDITUPDATE(IDC_COLORS, OnCbnEditupdateColors)
END_MESSAGE_MAP()


// CGUISetBGColorBox message handlers

void CGUISetBGColorBox::OnBnClickedOk()
{
	UpdateData(true);
	if (m_Doc)
	{
		m_Doc->ChangeBGColor(m_iRVal, m_iGVal, m_iBVal);
		OnOK();
	}
	else 
	{
		OnOK();
	}
}

void CGUISetBGColorBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	m_ctColorCombo.ResetContent();
	m_ctColorCombo.AddString(_T("Black"));
	m_ctColorCombo.AddString(_T("White"));
	m_ctColorCombo.AddString(_T("Blue"));
	m_ctColorCombo.SetCurSel(0);
}

void CGUISetBGColorBox::OnCbnSelchangeColors()
{
	m_ctColorCombo.GetLBText(m_ctColorCombo.GetCurSel(), m_szColorChoice);
	if (_tcscmp(m_szColorChoice, _T("Black")) == 0)
	{
		m_iRVal = 0;
		m_iGVal = 0;
		m_iBVal = 0;
		UpdateData(false);
	}
	else if (_tcscmp(m_szColorChoice, _T("White")) == 0)
	{
		m_iRVal = 255;
		m_iGVal = 255;
		m_iBVal = 255;
		UpdateData(false);
	}
	else if (_tcscmp(m_szColorChoice, _T("Blue")) == 0)
	{
		m_iRVal = 37;
		m_iGVal = 129;
		m_iBVal = 215;
		UpdateData(false);
	}
}