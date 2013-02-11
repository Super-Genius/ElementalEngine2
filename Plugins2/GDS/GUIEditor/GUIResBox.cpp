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

// CGUIResBox dialog

IMPLEMENT_DYNAMIC(CGUIResBox, CDialog)
CGUIResBox::CGUIResBox(CWnd* pParent /*=NULL*/)
	: CDialog(CGUIResBox::IDD, pParent)
	, m_iXRes(0)
	, m_iYRes(0)
{
	m_pEditorParent = NULL;	
}

CGUIResBox::~CGUIResBox()
{
}

void CGUIResBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_XRES, m_iXRes);
	DDX_Text(pDX, IDC_YRES, m_iYRes);
}


BEGIN_MESSAGE_MAP(CGUIResBox, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
//	ON_WM_CREATE()
//ON_WM_SETFOCUS()
ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CGUIResBox message handlers

void CGUIResBox::OnBnClickedOk()
{
	UpdateData(true);
	if (m_iXRes == 0)
	{
		MessageBox(_T("Must enter a value for x res!"));
	}
	else if (m_iYRes == 0)
	{
		MessageBox(_T("Must enter a value for y res!"));
	}
	else
	{
		m_pEditorParent->CreateDocView(m_iXRes, m_iYRes);
		OnOK();
	}
}
void CGUIResBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	m_iXRes = 1024;
	m_iYRes = 768;
	UpdateData(false);
}
