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
#include "WorldEventEditor.h"
#include "WorldEventDlg.h"
#include ".\worldeventdlg.h"


// CWorldEventDlg dialog

IMPLEMENT_DYNAMIC(CWorldEventDlg, CDialog)
CWorldEventDlg::CWorldEventDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWorldEventDlg::IDD, pParent)
{
	m_ToolBox = EngineGetToolBox();
	m_pEditor = SINGLETONINSTANCE(CWorldEventEditor);
}

CWorldEventDlg::~CWorldEventDlg()
{
}

void CWorldEventDlg::UpdateSelectedObject( IHashString *pObjectName )
{
	m_ObjWorldEventTree.UpdateSelectedObject( pObjectName );
}

void CWorldEventDlg::SetSelectedObject( IHashString *pObjectName )
{
	if (m_pEditor->IsWaitingForObjectSelection())
	{
		CHashString hszEvent;
		hszEvent = m_ObjWorldEventTree.GetItemText( m_ObjWorldEventTree.GetContextPopupItem() );
		bool bEventAdded = m_pEditor->AddWorldEventToEntity( 
			m_pEditor->GetSelectedObject(), // object triggering the world event
			&hszEvent, // object's state event that triggers the world event
			pObjectName, // target object of the world event
			NULL // target state event name
			);
		if (bEventAdded)
		{
			UpdateSelectedObject( m_pEditor->GetSelectedObject() );
		}
		m_pEditor->WaitForObjectSelection( false );
	}
	else
	{
		UpdateSelectedObject(pObjectName);
	}
}

void CWorldEventDlg::ShowAddGameEventPopupMenu( CPoint& point )
{
	if (m_ObjWorldEventTree.GetSelectedItem() == 0)
		return;

	static DWORD msgHash_QueryEvents = CHashString(_T("QueryStateMachineEvents")).GetUniqueID();
	static DWORD msgHash_SetWorldEventEvent = CHashString(_T("SetWorldEventEvent")).GetUniqueID();
	static DWORD msgHash_GetWorldEventInfo = CHashString(_T("GetWorldEventInfo")).GetUniqueID();
	CHashString hszStateObjectType(_T("CQHStateObject"));
	DWORD result;

	CHashString hszGameStateObjectName(_T("GameStateObject"));

	// gather all state events from the entity
	std::set<DWORD> uniqueStateEventNameSet;
	std::map<int,DWORD> uniqueStateEventNameMap;
	QUERYEVENTSPARAMS qeparams;
	result = m_ToolBox->SendMessage(msgHash_QueryEvents, sizeof(QUERYEVENTSPARAMS), &qeparams, &hszGameStateObjectName, &hszStateObjectType );
	if (result == MSG_HANDLED)
	{
		QUERYEVENTSLIST::iterator itr;
		for (itr = qeparams.QueryEventsList.begin(); itr != qeparams.QueryEventsList.end(); itr++ )
		{
			DWORD eventID = itr->EventName->GetUniqueID();
			if (uniqueStateEventNameSet.find( eventID ) == uniqueStateEventNameSet.end())
			{
				uniqueStateEventNameMap[uniqueStateEventNameMap.size()+1] = eventID;
				uniqueStateEventNameSet.insert( eventID );
			}
		}
	}
	else
	{
		MessageBox( _T("Failed to read events from GameStateObject, the object may not exist.  Try loading a game state object file (ie. game.xms)."), _T("WARNING") );
		return;
	}

	if (uniqueStateEventNameMap.size() == 0)
		return;

	// create a popup menu with all the state events available
	CMenu popMenu;
	popMenu.CreatePopupMenu();
	std::map<int,DWORD>::iterator itrMap = uniqueStateEventNameMap.begin();
	while (itrMap != uniqueStateEventNameMap.end())
	{
		popMenu.AppendMenu( MF_STRING, itrMap->first, m_ToolBox->GetHashString( itrMap->second ) );
		itrMap++;
	}

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{
		pWndPopupOwner = pWndPopupOwner->GetParent();
	}

	int resultItem = (int)popMenu.TrackPopupMenu(TPM_LEFTBUTTON|TPM_RETURNCMD, point.x, point.y, this);

	if (resultItem > 0)
	{
		DWORD dstEventID = uniqueStateEventNameMap[resultItem];
		CHashString hszDstEvent = m_ToolBox->GetHashString( dstEventID );
		CHashString hszSrcEvent = m_ObjWorldEventTree.GetItemText( m_ObjWorldEventTree.GetSelectedItem() );
		bool bEventAdded = m_pEditor->AddWorldEventToEntity( 
			m_pEditor->GetSelectedObject(), // object triggering the world event
			&hszSrcEvent, // object's state event that triggers the world event
			&hszGameStateObjectName, // target object of the world event
			&hszDstEvent // target state event name
			);
		if (bEventAdded)
		{
			UpdateSelectedObject( m_pEditor->GetSelectedObject() );
		}
	}
}

void CWorldEventDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJ_WORLDEVENT_TREE, m_ObjWorldEventTree);
	DDX_Control(pDX, IDC_WORLDEVENTPARAM_EDIT, m_WorldEventParamEdit);
	DDX_Control(pDX, IDC_BUTTON_ADDGAMEEVENT, m_ButtonAddGameEvent);
}


BEGIN_MESSAGE_MAP(CWorldEventDlg, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_OBJ_WORLDEVENT_TREE, OnTvnSelchangedObjWorldeventTree)
	ON_NOTIFY(TVN_DELETEITEM, IDC_OBJ_WORLDEVENT_TREE, OnTvnDeleteitemObjWorldeventTree)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BUTTON_ADDGAMEEVENT, OnBnClickedButtonAddgameevent)
	ON_BN_CLICKED(IDC_BUTTON_LOADGAMESCRIPT, OnBnClickedButtonLoadgamescript)
END_MESSAGE_MAP()


// CWorldEventDlg message handlers

void CWorldEventDlg::OnTvnSelchangedObjWorldeventTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM htItem = pNMTreeView->itemNew.hItem;
	bool bSelectedItemIsRoot = (m_ObjWorldEventTree.GetParentItem(htItem) == NULL);
	// enable the world event parameter edit if the selected item is a world event
	m_WorldEventParamEdit.EnableWindow( !bSelectedItemIsRoot );
	m_WorldEventParamEdit.SetWindowText( m_ObjWorldEventTree.GetWorldEventParamByItem( htItem ) );

	*pResult = 0;
}

void CWorldEventDlg::OnTvnDeleteitemObjWorldeventTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	*pResult = 0;
}

void CWorldEventDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_ObjWorldEventTree.m_hWnd)
	{
		CRect dlgRect;
		GetClientRect(dlgRect);
		CRect treeRect;
		m_ObjWorldEventTree.GetWindowRect(treeRect);
		ScreenToClient(treeRect);
		m_ObjWorldEventTree.MoveWindow( dlgRect.left, treeRect.top, dlgRect.Width(), treeRect.Height() );
	}
}

void CWorldEventDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CWorldEventDlg::OnBnClickedButtonAddgameevent()
{
	CRect rect;
	m_ButtonAddGameEvent.GetWindowRect( &rect );
	ShowAddGameEventPopupMenu( rect.TopLeft() );
}

void CWorldEventDlg::OnBnClickedButtonLoadgamescript()
{
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	CHashString hszGameStateObjectName(_T("GameStateObject"));
	FINDOBJECTPARAMS fop;
	fop.hszName = &hszGameStateObjectName;

	// make sure the object isn't already made, otherwise put up an informative message box
	fop.bFound = false;
	m_ToolBox->SendMessage(msgHash_FindObject, sizeof(fop), &fop);
	if (fop.bFound)
	{
		MessageBox( _T("GameStateObject already exists.  Cannot load a different game script."), _T("WARNING") );
		return;
	}

	// open file dialog for a script file to load
	TCHAR filePathName[MAX_PATH] = {'\0'};
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetSafeHwnd();
	ofn.lpstrTitle = "Open Game State Machine";
	ofn.lpstrFilter = "State Machine Files (*.xms)\0*.xms\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = filePathName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "xms";
	if(!GetOpenFileName(&ofn))
	{
		return;
	}

	LoadGlobalStateObject( _T("GameStateObject"), _T(""), _T("Game_StateMachine"), filePathName, _T(""), false );

	// make sure the object we need is made, otherwise put up an informative message box
	fop.bFound = false;
	m_ToolBox->SendMessage(msgHash_FindObject, sizeof(fop), &fop);
	if (!fop.bFound)
	{
		MessageBox( _T("Failed to find GameStateObject after loading game state machine."), _T("WARNING") );
		return;
	}
}

void CWorldEventDlg::LoadGlobalStateObject( StdString stateObjectName, StdString stateObjectParentName,  StdString stateMachineName, StdString stateMachineFileName, StdString startStateName, bool isInHierarchy )
{
	LOADANDINITSTATEOBJECTPARAMS liso;
	CHashString StateObject( stateObjectName );
	CHashString StateObjectParent( stateObjectParentName );
	CHashString StateMachineName( stateMachineName );
	CHashString StateMachineFileName( stateMachineFileName );
	CHashString StateMachineStartState( startStateName );

	liso.StateObjectName = &StateObject;
	liso.StateObjectParentName = &StateObjectParent;
	liso.StateMachineName = &StateMachineName;
	liso.StateMachineFileName = &StateMachineFileName;
	liso.StateMachineStartState = &StateMachineStartState;
	liso.bIsInHierarchy = isInHierarchy;

	static DWORD msgHash_LoadAndInitStateObject = CHashString(_T("LoadAndInitStateObject")).GetUniqueID();
	DWORD returnValue = m_ToolBox->SendMessage(msgHash_LoadAndInitStateObject, sizeof(liso), &liso );
	if( returnValue != MSG_HANDLED )
	{
		m_ToolBox->Log( LOGFATALERROR, "Could Not Load State Machine File: %s", StateMachineFileName.GetString() );
	}
}
