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

// CGUIGroupNameBox dialog

IMPLEMENT_DYNAMIC(CGUIGroupNameBox, CDialog)
CGUIGroupNameBox::CGUIGroupNameBox(CWnd* pParent /*=NULL*/)
	: CDialog(CGUIGroupNameBox::IDD, pParent)
	, m_szGroupName(_T(""))
{
	m_Doc = NULL;
}

CGUIGroupNameBox::~CGUIGroupNameBox()
{
}

void CGUIGroupNameBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GROUPNAME, m_szGroupName);
}


BEGIN_MESSAGE_MAP(CGUIGroupNameBox, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CGUIGroupNameBox message handlers

void CGUIGroupNameBox::OnBnClickedOk()
{
	UpdateData(true);
	if (m_Doc)
	{
        CHashString groupName(m_szGroupName);
		if (!m_Doc->CheckNameIsRegistered(groupName))
		{
			m_Doc->AddGroup(groupName, false);
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