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

// CGUIUseGroupBox dialog

IMPLEMENT_DYNAMIC(CGUIUseGroupBox, CDialog)
CGUIUseGroupBox::CGUIUseGroupBox(CWnd* pParent /*=NULL*/)
	: CDialog(CGUIUseGroupBox::IDD, pParent)
	, m_szGroupName(_T(""))
{
	m_Doc = NULL;
}

CGUIUseGroupBox::~CGUIUseGroupBox()
{
}

void CGUIUseGroupBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USEGROUP, m_ctUseGroup);
	DDX_CBString(pDX, IDC_USEGROUP, m_szGroupName);
}


BEGIN_MESSAGE_MAP(CGUIUseGroupBox, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CGUIUseGroupBox message handlers

void CGUIUseGroupBox::OnBnClickedOk()
{
	UpdateData(true);
	if (m_Doc)
	{
		CHashString gName(m_szGroupName);
		if (!m_Doc->CheckIsGroupInPage(gName))
		{
			m_Doc->AddGroup(gName, true);
			OnOK();
		}
		else
		{
			MessageBox("Selected page already has that group!");
		}
	}
	else
	{
		OnOK();
	}
}

void CGUIUseGroupBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	m_ctUseGroup.ResetContent();
	for (UINT i=0; i<m_vGroupNames.size(); i++)
	{
		m_ctUseGroup.AddString(m_vGroupNames[i].GetString());
	}
}

