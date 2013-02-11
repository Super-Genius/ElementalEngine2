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
#include ".\guinewstaticbox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CGUINewStaticBox dialog

IMPLEMENT_DYNAMIC(CGUINewStaticBox, CDialog)
CGUINewStaticBox::CGUINewStaticBox(CWnd* pParent /*=NULL*/)
	: CDialog(CGUINewStaticBox::IDD, pParent)
	, m_szStaticName(_T(""))
{
	m_szStaticType = _T("");
	m_Doc = NULL;
}

CGUINewStaticBox::~CGUINewStaticBox()
{
}

void CGUINewStaticBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATICNAME, m_szStaticName);
}


BEGIN_MESSAGE_MAP(CGUINewStaticBox, CDialog)
	ON_BN_CLICKED(IDC_STATICARTBT, OnBnClickedStaticartbt)
	ON_BN_CLICKED(IDC_STATUSBARBT, OnBnClickedStatusbarbt)
	ON_BN_CLICKED(IDC_CURSORBT, OnBnClickedCursorbt)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_MOVIEBT, OnBnClickedMoviebt)
END_MESSAGE_MAP()


// CGUINewStaticBox message handlers

void CGUINewStaticBox::OnBnClickedStaticartbt()
{
	m_szStaticType = _T("CGUIStaticArt");
}

void CGUINewStaticBox::OnBnClickedStatusbarbt()
{
	m_szStaticType = _T("CGUIStatusBar");
}

void CGUINewStaticBox::OnBnClickedCursorbt()
{
	m_szStaticType = _T("CGUICursor");	
}

void CGUINewStaticBox::OnBnClickedOk()
{
	if (m_Doc)
	{
		if (_tcscmp(m_szStaticType.GetString(), _T("CGUICursor"))== 0)
		{
			m_Doc->SerializeCursor();
			OnOK();
		}
		else
		{
			CHashString name(m_szStaticName);
			if (!m_Doc->CheckNameIsRegistered(name))
			{
				m_Doc->AddElement(name, m_szStaticType, false);
				OnOK();
			}
			else
			{
				MessageBox("Name is already taken!");
			}
		}
	}
	else
	{
		OnOK();
	}
}

void CGUINewStaticBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	CheckRadioButton(IDC_STATICARTBT, IDC_CURSORBT, IDC_STATICARTBT);
	m_szStaticType = _T("CGUIStaticArt");
}

void CGUINewStaticBox::OnBnClickedMoviebt()
{
	m_szStaticType = _T("CGUIMovie");	
}
