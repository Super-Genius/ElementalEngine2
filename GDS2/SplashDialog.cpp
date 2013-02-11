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
#include ".\splashdialog.h"


IMPLEMENT_DYNAMIC(CSplashDialog, CDialog)
CSplashDialog::CSplashDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashDialog::IDD, pParent)
{
	m_OKEnabled = false;
	m_TextFont.CreateFont(14,			// nHeight
			0,							// nWidth
			0,							// nEscapement
			0,							// nOrientation
			FW_BOLD,					// nWeight
			FALSE,						// bItalic
			FALSE,						// bUnderline
			0,							// cStrikeOut
			ANSI_CHARSET,				// nCharSet
			OUT_DEFAULT_PRECIS,			// nOutPrecision
			CLIP_DEFAULT_PRECIS,		// nClipPrecision
			DEFAULT_QUALITY,			// nQuality
			DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
			"Arial");					// lpszFacename

		m_bOKBitmapLoaded = false;
}

CSplashDialog::~CSplashDialog()
{
}

void CSplashDialog::EnableOK(bool enable)
{
	m_OKEnabled = enable;
}

// set the loading text
void  CSplashDialog::SetLoadingText(LPCTSTR text)
{
	m_LoadingText.SetWindowText(text);
}

void CSplashDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplashDialog)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDC_WRITTENBY, m_WrittenBy);
	DDX_Control(pDX, IDC_LOADING, m_LoadingText);
	DDX_Control(pDX, IDC_LICENSE, m_LicensedTo);
}


BEGIN_MESSAGE_MAP(CSplashDialog, CDialog)
END_MESSAGE_MAP()


// CSplashDialog message handlers

BOOL CSplashDialog::OnInitDialog()
{
	if (!m_bOKBitmapLoaded)
	{
		m_OKBitmap.LoadBitmap(IDB_STARTSCREENOK);
		m_bOKBitmapLoaded = true;
	}

	CDialog::OnInitDialog();

	SetFont(&m_TextFont, false);
	m_LicensedTo.SetFont(&m_TextFont, false);
	m_WrittenBy.SetFont(&m_TextFont, false);
	m_LoadingText.SetFont(&m_TextFont, false);
	m_LicensedTo.SetWindowText(_T("Licensed To: Nobody\n")
								_T("Serial No: 0000-0000-0000-0000"));

	if (m_OKEnabled)
	{
		m_OKButton.SetBitmap(m_OKBitmap);
		m_OKButton.ModifyStyle(0, WS_VISIBLE);
		m_LoadingText.ModifyStyle(WS_VISIBLE, 0);
	}
	else
	{
		m_OKButton.ModifyStyle(WS_VISIBLE, 0);
		m_LoadingText.ModifyStyle(0, WS_VISIBLE);
	}

	m_OKButton.RedrawWindow();
	m_WrittenBy.RedrawWindow();
	m_LoadingText.RedrawWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSplashDialog::PreCreateWindow(CREATESTRUCT& cs)
{
	return CDialog::PreCreateWindow(cs);
}

