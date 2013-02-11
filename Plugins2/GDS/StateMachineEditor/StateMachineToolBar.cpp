///=====================================================================
/// \file	StateMachineToolBar.cpp
/// \brief	Implementation of CStateMachineToolBar
/// \date	12/04/2007
/// \author	Andrey Ivanov
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
///=====================================================================
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CStateMachineToolBar, CGuiDialogBar)

BEGIN_MESSAGE_MAP(CStateMachineToolBar, CGuiDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
END_MESSAGE_MAP()

CStateMachineToolBar::CStateMachineToolBar()
{
}

CStateMachineToolBar::~CStateMachineToolBar()
{
}

void CStateMachineToolBar::DoDataExchange( CDataExchange* pDX )
{
	CGuiDialogBar::DoDataExchange( pDX );
}

LRESULT CStateMachineToolBar::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = HandleInitDialog( wParam, lParam );

	// setting up button icons
	SetButtonIcon( ID_STATE_BUTTON, IDI_STATE_BUTTON );
	SetButtonIcon( ID_EVENT_BUTTON, IDI_EVENT_BUTTON );
	SetButtonIcon( ID_ENTRY_EVENT_BUTTON, IDI_ENTRY_EVENT_BUTTON );
	SetButtonIcon( ID_UPDATE_EVENT_BUTTON, IDI_UPDATE_EVENT_BUTTON );
	SetButtonIcon( ID_EXIT_EVENT_BUTTON, IDI_EXIT_EVENT_BUTTON );
	SetButtonIcon( ID_TRANSITION_MODE, IDI_TRANSITION_MODE );
	SetButtonIcon( ID_VALIDATE, IDI_VALIDATE );
	SetButtonIcon( ID_FLAT_MODE, IDI_FLATVIEW );

	return bRet;
}

bool CStateMachineToolBar::IsTransitionMode() const
{
	return BST_CHECKED == IsDlgButtonChecked( ID_TRANSITION_MODE );
}

void CStateMachineToolBar::SetTransitionMode( bool mode )
{
	CheckDlgButton( ID_TRANSITION_MODE, mode ? BST_CHECKED : BST_UNCHECKED );
}

bool CStateMachineToolBar::IsFlatMode() const
{
	return BST_CHECKED == IsDlgButtonChecked( ID_FLAT_MODE );
}

void CStateMachineToolBar::SetFlatMode( bool mode )
{
	CheckDlgButton( ID_FLAT_MODE, mode ? BST_CHECKED : BST_UNCHECKED );
}

void CStateMachineToolBar::SetButtonIcon( int buttonId, int iconId )
{
	HINSTANCE hRes = StateMachineEditorDLL.hResource;
	CButton *pBtn = (CButton *)( GetDlgItem( buttonId ) );
	HICON hIcon = LoadIcon( hRes, MAKEINTRESOURCE( iconId ) );
	pBtn->SetIcon( hIcon );
}
