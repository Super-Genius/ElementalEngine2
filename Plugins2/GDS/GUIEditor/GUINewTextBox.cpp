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

// CGUINewTextBox dialog

IMPLEMENT_DYNAMIC(CGUINewTextBox, CDialog)
CGUINewTextBox::CGUINewTextBox(CWnd* pParent /*=NULL*/)
	: CDialog(CGUINewTextBox::IDD, pParent)
	, m_szTextName(_T(""))
{
	m_szTextType = _T("");
	m_Doc = NULL;
}

CGUINewTextBox::~CGUINewTextBox()
{
}

void CGUINewTextBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEXTNAME, m_szTextName);
}


BEGIN_MESSAGE_MAP(CGUINewTextBox, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_STATICTEXTBT, OnBnClickedStatictextbt)
	ON_BN_CLICKED(IDC_EDITTEXTBT, OnBnClickedEdittextbt)
	ON_BN_CLICKED(IDC_LISTBOXBT, OnBnClickedListboxbt)
	ON_BN_CLICKED(IDC_COMBOBOXBT, OnBnClickedComboboxbt)
	ON_BN_CLICKED(IDC_SPINNERBT, OnBnClickedSpinnerbt)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CGUINewTextBox message handlers

void CGUINewTextBox::OnBnClickedOk()
{
	UpdateData(true);
	CHashString tname(m_szTextName);
	if (m_Doc)
	{
		if (!m_Doc->CheckNameIsRegistered(tname))
		{
			m_Doc->AddElement(tname, m_szTextType, false);
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

void CGUINewTextBox::OnBnClickedStatictextbt()
{
	m_szTextType = _T("CGUIStaticText");
}

void CGUINewTextBox::OnBnClickedEdittextbt()
{
	m_szTextType = _T("CGUIEditText");
}

void CGUINewTextBox::OnBnClickedListboxbt()
{
	m_szTextType = _T("CGUIListBox");
}

void CGUINewTextBox::OnBnClickedComboboxbt()
{
	m_szTextType = _T("CGUIComboBox");
}

void CGUINewTextBox::OnBnClickedSpinnerbt()
{
	m_szTextType = _T("CGUISpinnerItem");
}

void CGUINewTextBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	CheckRadioButton(IDC_STATICTEXTBT, IDC_SPINNERBT, IDC_STATICTEXTBT);
	m_szTextType = _T("CGUIStaticText");
}

void CGUINewTextBox::SetTextName(StdString name)
{
	m_szTextName = (const TCHAR*)name;
	UpdateData(false);
}
