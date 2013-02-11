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
#include "WorldEventTree.h"
#include ".\worldeventtree.h"


// CWorldEventTree

IMPLEMENT_DYNAMIC(CWorldEventTree, CTreeCtrl)
CWorldEventTree::CWorldEventTree()
{
	m_ToolBox = EngineGetToolBox();
	m_pEditor = SINGLETONINSTANCE(CWorldEventEditor);
	m_ContextPopupItem = NULL;
}

CWorldEventTree::~CWorldEventTree()
{
}


BEGIN_MESSAGE_MAP(CWorldEventTree, CTreeCtrl)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_STATEEVENTMENU_ADDEVENTTOENTITY, OnAddEventEntity)
	ON_COMMAND(ID_STATEEVENTMENU_ADDEVENTTOALL, OnAddEventAll)
	ON_COMMAND(ID_STATEEVENTMENU_ADDEVENTTOGROUP, OnAddEventGroup)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CWorldEventTree message handlers


void CWorldEventTree::OnContextMenu(CWnd* pWnd, CPoint point)
{
	UINT uFlags;
	CPoint ptTree = point;
	ScreenToClient(&ptTree);
	HTREEITEM htItem = HitTest(ptTree, &uFlags);
	if ((htItem != NULL) && (uFlags & TVHT_ONITEM))
	{
		m_ContextPopupItem = htItem;
		m_pEditor->WaitForObjectSelection( false );
		if (GetParentItem(htItem) == NULL)
			ShowPopupMenu( IDR_MENU_STATEEVENT, point );
		else
			ShowWorldEventPopupMenu( htItem, point );

//		SetItemState(htItem, 0, TVIS_DROPHILITED);
	}

	CTreeCtrl::OnContextMenu(pWnd, point);
}

BOOL CWorldEventTree::DeleteAllItems()
{
	m_EventNameToTreeItemMap.clear();
	m_TreeItemToWorldEventNameMap.clear();
	return CTreeCtrl::DeleteAllItems();
}

void CWorldEventTree::ShowWorldEventPopupMenu( HTREEITEM htItem, CPoint& point )
{
	static DWORD msgHash_QueryEvents = CHashString(_T("QueryStateMachineEvents")).GetUniqueID();
	static DWORD msgHash_SetWorldEventEvent = CHashString(_T("SetWorldEventEvent")).GetUniqueID();
	static DWORD msgHash_GetWorldEventInfo = CHashString(_T("GetWorldEventInfo")).GetUniqueID();
	CHashString hszStateObjectType(_T("CQHStateObject"));
	DWORD result;

	// get the name of the world event associated with the htItem
	map<HTREEITEM,DWORD>::iterator itrTreeItemToWorldEventMap;
	itrTreeItemToWorldEventMap = m_TreeItemToWorldEventNameMap.find(htItem);
	if (itrTreeItemToWorldEventMap == m_TreeItemToWorldEventNameMap.end())
		return;

	CHashString hszWorldEventName( m_ToolBox->GetHashString( itrTreeItemToWorldEventMap->second ) );

	// get the name of the entity associated with that world event
	WORLDEVENTINFOPARAMS worldEventInfo;
	result = m_ToolBox->SendMessage(msgHash_GetWorldEventInfo, sizeof(worldEventInfo), &worldEventInfo, &hszWorldEventName );
	if (result != MSG_HANDLED)
		return;
	if (!worldEventInfo.bUseEntityName)
		return;

	// gather all state events from the entity
	std::set<DWORD> uniqueStateEventNameSet;
	std::map<int,DWORD> uniqueStateEventNameMap;
	QUERYEVENTSPARAMS qeparams;
	result = m_ToolBox->SendMessage(msgHash_QueryEvents, sizeof(QUERYEVENTSPARAMS), &qeparams, &worldEventInfo.hszEntityName, &hszStateObjectType );
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
		// set the world event's state event to the newly selected event
		DWORD eventID = uniqueStateEventNameMap[resultItem];
		result = m_ToolBox->SendMessage( msgHash_SetWorldEventEvent, sizeof(DWORD), &eventID, &hszWorldEventName );

		StdString szEventName;
		szEventName = GetWorldEventItemText( &hszWorldEventName );
		SetItemText( htItem, szEventName );
	}
}

void CWorldEventTree::ShowPopupMenu( UINT idr, CPoint& point )
{
	CMenu popMenu;
	popMenu.LoadMenu(idr);
	ASSERT(popMenu.m_hMenu);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{
		pWndPopupOwner = pWndPopupOwner->GetParent();
	}

	// TODO: change this to a return cmd flag version like in the other popup menu here
	BOOL result = popMenu.GetSubMenu(0)->TrackPopupMenu(0, point.x, point.y, this);
}

StdString CWorldEventTree::GetWorldEventItemText( IHashString *pWorldEventName )
{
	static DWORD msgHash_GetWorldEventInfo = CHashString(_T("GetWorldEventInfo")).GetUniqueID();

	StdString szEventName;
	WORLDEVENTINFOPARAMS worldEventInfo;
	DWORD result = m_ToolBox->SendMessage(msgHash_GetWorldEventInfo, sizeof(worldEventInfo), &worldEventInfo, pWorldEventName );
	if (result == MSG_HANDLED)
	{
		if (worldEventInfo.bUseEntityName)
		{
			szEventName = worldEventInfo.hszEntityName.GetString();
		}
		else if (worldEventInfo.bUseEntityType)
		{
			szEventName += "*";
			szEventName = worldEventInfo.hszEntityType.GetString();
		}
		else
		{
			szEventName = "[ALL]";
		}
		szEventName += ":";
		szEventName += worldEventInfo.hszEventName.GetString();
	}
	return szEventName;
}

StdString CWorldEventTree::GetWorldEventParamByItem( HTREEITEM htItem )
{
	static DWORD msgHash_GetWorldEventInfo = CHashString(_T("GetWorldEventInfo")).GetUniqueID();

	StdString szWorldEventParam;
	map<HTREEITEM,DWORD>::iterator itrMap = m_TreeItemToWorldEventNameMap.find(htItem);
	if (itrMap != m_TreeItemToWorldEventNameMap.end())
	{
		DWORD worldEventNameID = itrMap->second;
		CHashString hszWorldEventName(m_ToolBox->GetHashString(worldEventNameID));
		WORLDEVENTINFOPARAMS worldEventTriggerInfo;
		DWORD result = m_ToolBox->SendMessage(msgHash_GetWorldEventInfo, sizeof(worldEventTriggerInfo), &worldEventTriggerInfo, &hszWorldEventName );
		if (result == MSG_HANDLED)
		{
			szWorldEventParam = worldEventTriggerInfo.hszEventParam.GetString();
		}
	}

	return szWorldEventParam;
}

void CWorldEventTree::DeleteWorldEventByItem( HTREEITEM htItem )
{
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();

	map<HTREEITEM,DWORD>::iterator itrMap = m_TreeItemToWorldEventNameMap.find(htItem);
	if (itrMap != m_TreeItemToWorldEventNameMap.end())
	{
		DWORD worldEventNameID = itrMap->second;
		CHashString hszWorldEventName(m_ToolBox->GetHashString(worldEventNameID));

		DELETEOBJECTPARAMS dop;
		dop.name = &hszWorldEventName;
		DWORD result = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop);

		DeleteItem( htItem );
		m_TreeItemToWorldEventNameMap.erase( htItem );
	}
}

void CWorldEventTree::UpdateSelectedObject( IHashString *pObjectName )
{
	static DWORD msgHash_QueryEvents = CHashString(_T("QueryStateMachineEvents")).GetUniqueID();
	static DWORD msgHash_GetWorldEventInfo = CHashString(_T("GetWorldEventInfo")).GetUniqueID();
	static DWORD msgHash_QueryWorldEventTriggers = CHashString(_T("QueryWorldEventTriggers")).GetUniqueID();
	CHashString hszStateObjectType(_T("CQHStateObject"));

	DeleteAllItems();

	// if no selected object, return early after clearing tree
	if (!pObjectName)
		return;

	std::set<DWORD> uniqueEventNameSet;

	// display all available state events as root tree nodes
	QUERYEVENTSPARAMS qeparams;
	DWORD result = m_ToolBox->SendMessage(msgHash_QueryEvents, sizeof(QUERYEVENTSPARAMS), &qeparams, pObjectName, &hszStateObjectType );
	if (result == MSG_HANDLED)
	{
		QUERYEVENTSLIST::iterator itr;
		itr = qeparams.QueryEventsList.begin();
		while( itr != qeparams.QueryEventsList.end() )
		{
			DWORD eventID = itr->EventName->GetUniqueID();
			// check to make sure we haven't already added a state event of the same name
			// (the same event in another substate will show up multiple times)
			if (uniqueEventNameSet.find( eventID ) == uniqueEventNameSet.end())
			{
				//itr->StateName->GetString();
				LPCTSTR pEventName = itr->EventName->GetString();

				m_EventNameToTreeItemMap[eventID] = InsertItem( pEventName );

				uniqueEventNameSet.insert( eventID );
			}
			itr++;
		}
	}

	// find all the world trigger events attached to this object and display them under 
	// the corresponding state event
	WORLDEVENTTRIGGERMAP WorldEventTriggerMap;
	WORLDEVENTTRIGGERMAP::iterator itrMap;

	QUERYWORLDEVENTTRIGGERSPARAMS qwetp;
	qwetp.pEntityName = pObjectName;
	qwetp.pWorldEventTriggerMap = &WorldEventTriggerMap;
	m_ToolBox->SendMessage(msgHash_QueryWorldEventTriggers, sizeof(qwetp), &qwetp);

    itrMap = WorldEventTriggerMap.begin();
	while( itrMap != WorldEventTriggerMap.end() )
	{
		// get the name of the world event trigger
		const TCHAR *pWorldEventTriggerName = m_ToolBox->GetHashString( itrMap->first );

		// get the name of the event that this world event trigger fires
		WORLDEVENTINFOPARAMS worldEventTriggerInfo;
		m_ToolBox->SendMessage(msgHash_GetWorldEventInfo, sizeof(worldEventTriggerInfo), &worldEventTriggerInfo, &CHashString(pWorldEventTriggerName) );

		HTREEITEM hEventTreeItem = 0;
		map<DWORD,HTREEITEM>::iterator itrEventToTree = m_EventNameToTreeItemMap.find( worldEventTriggerInfo.hszEventName.GetUniqueID() );
		if (itrEventToTree != m_EventNameToTreeItemMap.end())
		{
			hEventTreeItem = itrEventToTree->second;

			WORLDEVENTLIST::iterator itrList = itrMap->second.begin();
			while (itrList != itrMap->second.end())
			{
				const TCHAR *pWorldEventName = m_ToolBox->GetHashString( *itrList );
				CHashString hszWorldEventName(pWorldEventName);

				// get the name of the event that this world event trigger fires
				WORLDEVENTINFOPARAMS worldEventInfo;
				DWORD result = m_ToolBox->SendMessage(msgHash_GetWorldEventInfo, sizeof(worldEventInfo), &worldEventInfo, &hszWorldEventName );
				if (result == MSG_HANDLED)
				{
					StdString szEventName;
					szEventName = GetWorldEventItemText( &hszWorldEventName );
					HTREEITEM htNewItem = InsertItem( szEventName, hEventTreeItem );
					m_TreeItemToWorldEventNameMap[htNewItem] = hszWorldEventName.GetUniqueID();
				}
				itrList++;
			}
		}
		itrMap++;
	}
}

void CWorldEventTree::OnAddEventEntity()
{
	// put editor into a state where it is waiting for an object selection
	m_pEditor->WaitForObjectSelection(true);
}

void CWorldEventTree::OnAddEventAll()
{
	// TODO: Add your command handler code here
//	m_pEditor->AddWorldEventToAll( m_pEditor->GetSelectedObject(), pObjectName, NULL );
}

void CWorldEventTree::OnAddEventGroup()
{
	// TODO: Add your command handler code here
}

void CWorldEventTree::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	SendMessage(WM_CONTEXTMENU, (WPARAM)m_hWnd, GetMessagePos());
	*pResult = 0;
}

void CWorldEventTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DELETE)
		DeleteWorldEventByItem( GetSelectedItem() );

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}
