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
#include "NodeSystemEditor.h"
#include "NodeToolPal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CNodeToolPal dialog

IMPLEMENT_DYNAMIC(CNodeToolPal, CGuiDialogBar)
CNodeToolPal::CNodeToolPal()
{
	m_Parent = NULL;
}

CNodeToolPal::~CNodeToolPal()
{
}

void CNodeToolPal::SetParent( CNodeSystemEditor *parent )
{
	m_Parent = parent;
}

void CNodeToolPal::ObjectSelected( IHashString *hszObjectName, IHashString *hszComponentType )
{
	if( hszComponentType != NULL &&
		( _tcscmp( hszComponentType->GetString(), _T("CNodeSystem") ) == 0 ||
		_tcscmp( hszComponentType->GetString(), _T("CNode") ) == 0 ||
		_tcscmp( hszComponentType->GetString(), _T("CNodeConnection") ) == 0 ))
	{
		CButton *button = (CButton*)GetDlgItem(IDC_BUTTON1);
		if( button != NULL )
		{
			button->EnableWindow();
		}
	}
	else if( m_hWnd != NULL )
	{
		CButton *button = (CButton*)GetDlgItem(IDC_BUTTON1);
		if( button != NULL )
		{
			button->EnableWindow( FALSE );
		}
	}
}

void CNodeToolPal::DoDataExchange(CDataExchange* pDX)
{
	CGuiDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNodeToolPal, CGuiDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1Clicked)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2Clicked)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON1, OnUpdateButton1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1Clicked)
END_MESSAGE_MAP()

LRESULT CNodeToolPal::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init.\n");
	}

	OnCheck1Clicked();
	OnCheck2Clicked();

	GetDlgItem(IDC_BUTTON1)->EnableWindow( FALSE );
	GetDlgItem(IDC_BUTTON2)->EnableWindow( FALSE );
	GetDlgItem(IDC_BUTTON3)->EnableWindow( FALSE );
	GetDlgItem(IDC_BUTTON4)->EnableWindow( FALSE );

	return bRet;
}

void CNodeToolPal::OnCheck1Clicked()
{
	CButton *button = (CButton *)GetDlgItem(IDC_CHECK1);
	ASSERT( button );
	ASSERT( m_Parent );
	if( button->GetCheck() )
	{
		m_Parent->ActivateNodePlot();
	}
	else
	{
		m_Parent->DeactivateNodePlot();
	}
}

void CNodeToolPal::OnCheck2Clicked()
{
	CButton *button = (CButton *)GetDlgItem(IDC_CHECK2);
	ASSERT( button );
	ASSERT( m_Parent );
	if( button->GetCheck() )
	{
		m_Parent->ActivateConnectionPlot();
	}
	else
	{
		m_Parent->DeactivateConnectionPlot();
	}
}

void CNodeToolPal::OnUpdateButton1(CCmdUI* pCmdUI)
{
}

void CNodeToolPal::OnButton1Clicked()
{
	m_Parent->PlotNode();
}
// CNodeToolPal message handlers
