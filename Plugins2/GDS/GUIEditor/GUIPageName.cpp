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

// CGUIPageName dialog

IMPLEMENT_DYNAMIC(CGUIPageName, CDialog)
CGUIPageName::CGUIPageName(CWnd* pParent /*=NULL*/)
	: CDialog(CGUIPageName::IDD, pParent)
	, m_szPageName(_T(""))
{
	m_Doc = NULL;
}

CGUIPageName::~CGUIPageName()
{
}

void CGUIPageName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PAGENAME, m_szPageName);
}


BEGIN_MESSAGE_MAP(CGUIPageName, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CGUIPageName message handlers

void CGUIPageName::OnBnClickedOk()
{
	UpdateData(true);
	CHashString pageName = m_szPageName;
	if (m_Doc)
	{
		if (!m_Doc->CheckNameIsRegistered(pageName))
		{
			m_Doc->AddPage(pageName);
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

void CGUIPageName::SetPageName(StdString name)
{
	m_szPageName = (const TCHAR*)name;
	UpdateData(false);
}
