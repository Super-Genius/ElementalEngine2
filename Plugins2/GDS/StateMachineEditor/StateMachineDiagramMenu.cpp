///============================================================================
/// \file	StateMachineDiagramMenu.cpp
/// \brief	Implementation of context menu for state machine diagram editor
/// \date	12/18/2007
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
///============================================================================
#include "stdafx.h"

CStateMachineDiagramMenu::CStateMachineDiagramMenu()
{
	CMenu contextMenu;
	contextMenu.LoadMenu( IDR_EDITOR_CONTEXT_MENU );
	m_menu.Attach( contextMenu.GetSubMenu( 0 )->Detach() );
	contextMenu.RemoveMenu( 0, MF_BYPOSITION );

	ASSERT( m_menu.m_hMenu != NULL );
}

CMenu* CStateMachineDiagramMenu::GetPopupMenu( CDiagramEditor* editor )
{
	CStateMachineDiagramEditor* stateMachineEditor = (CStateMachineDiagramEditor *)editor;

	UINT undo = MF_GRAYED;
	UINT redo = MF_GRAYED;
	UINT cut = MF_GRAYED;
	UINT copy = MF_GRAYED;
	UINT paste = MF_GRAYED;
	UINT group = MF_GRAYED;
	if( editor->IsAnyObjectSelected() && !stateMachineEditor->IsFlatMode() )
	{
		cut = 0;
		copy = 0;
	}

	if( editor->GetSelectCount() > 1 && !stateMachineEditor->IsFlatMode() )
	{
		group = 0;
	}

	if( editor->GetDiagramEntityContainer() && stateMachineEditor->CanPaste() )
	{
		paste = 0;
	}

	m_menu.EnableMenuItem( ID_EDITOR_BACKONELEVEL , editor->IsAtRootLayer() );
	m_menu.EnableMenuItem( ID_EDIT_CUT, MF_BYCOMMAND | cut );
	m_menu.EnableMenuItem( ID_EDIT_COPY, MF_BYCOMMAND | copy );
	m_menu.EnableMenuItem( ID_EDIT_PASTE, MF_BYCOMMAND | paste );
	m_menu.EnableMenuItem( ID_EDITOR_GROUP, MF_BYCOMMAND | group );
	m_menu.EnableMenuItem( ID_EDITOR_UNGROUP, MF_BYCOMMAND | group );

	CCmdUI cmdUI;
	cmdUI.m_pMenu = &m_menu;
	cmdUI.m_nIndexMax = m_menu.GetMenuItemCount();
	cmdUI.m_nIndex = 2;
	ASSERT( ID_EDIT_UNDO == m_menu.GetMenuItemID( cmdUI.m_nIndex ) );
	editor->OnCmdMsg( ID_EDIT_UNDO, CN_UPDATE_COMMAND_UI, &cmdUI, NULL );
	cmdUI.m_nIndex = 3;
	ASSERT( ID_EDIT_REDO == m_menu.GetMenuItemID( cmdUI.m_nIndex ) );
	editor->OnCmdMsg( ID_EDIT_REDO, CN_UPDATE_COMMAND_UI, &cmdUI, NULL );

	return &m_menu;
}