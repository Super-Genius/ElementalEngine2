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

// CGUIUseItemBox dialog

IMPLEMENT_DYNAMIC(CGUIUseItemBox, CDialog)
CGUIUseItemBox::CGUIUseItemBox(CWnd* pParent /*=NULL*/)
	: CDialog(CGUIUseItemBox::IDD, pParent)
	, m_szItemName(_T(""))
{
	m_Doc = NULL;
}

CGUIUseItemBox::~CGUIUseItemBox()
{
}

void CGUIUseItemBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ITEMSELECT, m_ctUseItem);
	DDX_CBString(pDX, IDC_ITEMSELECT, m_szItemName);
}


BEGIN_MESSAGE_MAP(CGUIUseItemBox, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CGUIUseItemBox message handlers

void CGUIUseItemBox::OnBnClickedOk()
{
	UpdateData(true);
	UINT objectPos = 0;
	for (UINT i=0; i<m_vNames.size(); i++)
	{
		if (_tcscmp(m_vNames[i].GetString(), m_szItemName) == 0)
		{
			objectPos = i;
			break;
		}
	}
	if (m_Doc)
	{
//		if (!m_Doc->CheckIsItemInGroupOrPage(m_vNames[objectPos]))
//		{
			m_Doc->AddElement(m_vNames[objectPos], m_vTypes[objectPos], true);
			OnOK();
//		}
//		else
//		{
//			MessageBox("Selected page or group already has that item!");
//		}
	}
	else
	{
		OnOK();
	}
}

void CGUIUseItemBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	m_ctUseItem.ResetContent();
	for (UINT i=0; i<m_vNames.size(); i++)
	{
		m_ctUseItem.AddString((TCHAR*)m_vNames[i].GetString());
	}
}

void CGUIUseItemBox::RemoveItemFromList(CHashString name)
{
	for (UINT i=0; i<m_vNames.size(); i++)
	{
		if (_tcscmp(m_vNames[i].GetString(), name.GetString()) == 0)
		{
			m_vNames.erase(m_vNames.begin() + i);
			m_vTypes.erase(m_vTypes.begin() + i);
		}
	}
}
