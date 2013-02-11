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

// CGUINewButtonBox dialog

IMPLEMENT_DYNAMIC(CGUINewButtonBox, CDialog)
CGUINewButtonBox::CGUINewButtonBox(CWnd* pParent /*=NULL*/)
	: CDialog(CGUINewButtonBox::IDD, pParent)
	, m_szButtonName(_T(""))
{
	m_Doc = NULL;
	m_szButtonType = _T("");	
}

CGUINewButtonBox::~CGUINewButtonBox()
{
}

void CGUINewButtonBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BUTTONNAME, m_szButtonName);
}


BEGIN_MESSAGE_MAP(CGUINewButtonBox, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_STANDARDBT, OnBnClickedStandardbt)
	ON_BN_CLICKED(IDC_RADIOBT, OnBnClickedRadiobt)
	ON_BN_CLICKED(IDC_CHECKBOXBT, OnBnClickedCheckboxbt)
	ON_BN_CLICKED(IDC_SLIDERBT, OnBnClickedSliderbt)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CGUINewButtonBox message handlers

void CGUINewButtonBox::OnBnClickedOk()
{
	if (m_Doc)
	{
		CHashString btName(m_szButtonName);
		if (!m_Doc->CheckNameIsRegistered(btName))
		{
			m_Doc->AddElement(btName, m_szButtonType, false);
			OnOK();
		}
		else
		{
			MessageBox("Name is already taken!");
		}
	}
	else
	{
		OnOK();
	}
}

void CGUINewButtonBox::OnBnClickedStandardbt()
{
	m_szButtonType = _T("CGUIItem");
}

void CGUINewButtonBox::OnBnClickedRadiobt()
{
	m_szButtonType = _T("CGUIRadioButton");
}

void CGUINewButtonBox::OnBnClickedCheckboxbt()
{
	m_szButtonType = _T("CGUICheckBox");
}

void CGUINewButtonBox::OnBnClickedSliderbt()
{
	m_szButtonType = _T("CGUISlider");
}

void CGUINewButtonBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	CheckRadioButton(IDC_CHECKBOXBT, IDC_RADIOBT, IDC_STANDARDBT);
	m_szButtonType = _T("CGUIItem");
}
