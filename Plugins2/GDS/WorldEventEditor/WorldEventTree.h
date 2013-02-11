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

#pragma once


// CWorldEventTree

class CWorldEventTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CWorldEventTree)

public:
	CWorldEventTree();
	virtual ~CWorldEventTree();
	HTREEITEM GetContextPopupItem() { return m_ContextPopupItem; }

	virtual BOOL DeleteAllItems();

	void UpdateSelectedObject( IHashString *pObjectName );
	StdString GetWorldEventItemText( IHashString *pWorldEventName );
	StdString GetWorldEventParamByItem( HTREEITEM htItem );
	void DeleteWorldEventByItem( HTREEITEM htItem );

	map<DWORD,HTREEITEM> m_EventNameToTreeItemMap;
	map<HTREEITEM,DWORD> m_TreeItemToWorldEventNameMap;

protected:
	void ShowPopupMenu( UINT idr, CPoint& point );
	void ShowWorldEventPopupMenu( HTREEITEM htItem, CPoint& point );

	IToolBox *m_ToolBox;
	CWorldEventEditor* m_pEditor;
	HTREEITEM m_ContextPopupItem;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnAddEventEntity();
	afx_msg void OnAddEventAll();
	afx_msg void OnAddEventGroup();
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


