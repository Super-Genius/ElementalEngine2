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
#include "GUIEditor.h"
#include "NewAnimationBox.h"
#include ".\newanimationbox.h"


// CNewAnimationBox dialog

IMPLEMENT_DYNAMIC(CNewAnimationBox, CDialog)
CNewAnimationBox::CNewAnimationBox(CWnd* pParent /*=NULL*/)
	: CDialog(CNewAnimationBox::IDD, pParent)
	, m_szAnimName(_T(""))
{
	m_Doc = NULL;
}

CNewAnimationBox::~CNewAnimationBox()
{
}

void CNewAnimationBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ANIMNAME, m_szAnimName);
}


BEGIN_MESSAGE_MAP(CNewAnimationBox, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CNewAnimationBox message handlers

void CNewAnimationBox::OnBnClickedOk()
{
	UpdateData(true);
	CHashString animName = m_szAnimName;
	if (m_Doc)
	{
		if (!m_Doc->CheckNameIsRegistered(animName))
		{
			m_Doc->AddAnimation(animName);
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

void CNewAnimationBox::SetAnimName(StdString name)
{
	m_szAnimName = (const TCHAR*)name;
	UpdateData(false);
}
