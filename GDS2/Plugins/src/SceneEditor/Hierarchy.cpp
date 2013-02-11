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

extern AFX_EXTENSION_MODULE SceneEditorDLL;

static UINT ResourceDropMessage = RegisterWindowMessage(RESOURCE_DROP_MSG_STR);

struct HIERARCHYTREESTATE
{
	HIERARCHYTREESTATE()
		: selectedName(0)
	{
	}

	/// \brief	hash value for selected item in the hierarchy 
	DWORD selectedName;
	///	\brief	sorted vector with expanded hierarchy items
	vector<DWORD> collapsedItems;
};

/////////////////////////////////////////////////////////////////////////////
// CHierarchy dialog

CHierarchy::CHierarchy(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CHierarchy::IDD)
	, m_pToolBox(EngineGetToolBox())
{
	//{{AFX_DATA_INIT(CHierarchy)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CHierarchy::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHierarchy)
	DDX_Control(pDX, IDC_HIERARCHY, m_HierarchyControl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHierarchy, CPropertyPage)
	//{{AFX_MSG_MAP(CHierarchy)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_KEEPLOCAL, OnKeeplocal)
	ON_COMMAND(ID_KEEPGLOBAL, OnKeepglobal)
	ON_NOTIFY(NM_CLICK, IDC_HIERARCHY, OnClickTree)
	ON_NOTIFY(NM_DBLCLK, IDC_HIERARCHY, OnDblClickTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_HIERARCHY, OnSelChangedTree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_HIERARCHY, OnBeginLabelEdit)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_HIERARCHY, OnEndLabelEdit)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(ResourceDropMessage, OnDropMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHierarchy message handlers

void CHierarchy::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);

	if (m_HierarchyControl.m_hWnd != NULL)
	{
		CRect rect;
		GetClientRect(rect);
		m_HierarchyControl.MoveWindow(rect);
	}
}

void CHierarchy::OnSelChangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	if ( !isUnderUpdate )
	{
		LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
		*pResult = 0;
		HTREEITEM item = pNMTreeView->itemNew.hItem;
		if (item != NULL)
		{
			HIERARCHYINFO *hi = GetItemData(item);
			SelectObject(hi);
			SELECTOBJECTPARAMS sop;
		}
	}
}

void CHierarchy::OnClickTree(NMHDR *pnhm, LRESULT* result)
{
	UINT flags;

	// itemNew is not valid at this point
	//	HTREEITEM item = data->itemNew.hItem; 
	DWORD dw = GetMessagePos();						// retrieve mouse cursor position when msg was sent
	CPoint mousePos(MAKEPOINTS(dw).x, MAKEPOINTS(dw).y); // ..and put into point structure
	m_HierarchyControl.ScreenToClient(&mousePos);	// make coords local to tree client area
	HTREEITEM item = m_HierarchyControl.HitTest(mousePos, &flags);
	if (item != NULL)
	{
		HIERARCHYINFO *hi = GetItemData(item);

		// we have to select the item
		m_HierarchyControl.SelectItem(item);
		if (flags & TVHT_ONITEMSTATEICON)
		{
			m_HierarchyControl.SetCheck(item, m_HierarchyControl.GetCheck(item) == 0);
		}

		SelectObject(hi);
	}
}

void CHierarchy::OnDblClickTree(NMHDR *pnhm, LRESULT* result)
{
	UINT flags;

	// open object resource file in editor
	DWORD dw = GetMessagePos();								// retrieve mouse cursor position when msg was sent
	CPoint mousePos(MAKEPOINTS(dw).x, MAKEPOINTS(dw).y);	// ..and put into point structure
	m_HierarchyControl.ScreenToClient(&mousePos);			// make coords local to tree client area
	HTREEITEM item = m_HierarchyControl.HitTest(mousePos, &flags);
	if (item != NULL)
	{
		HIERARCHYINFO *hi = GetItemData(item);

		StdString filename = GetEEObjectResourceFilename(hi->objectName);
		TCHAR path[1024];
		_tcsncpy(path, filename.c_str(), _countof(path));
		LPTSTR szExt = const_cast<LPTSTR>(FileHelpers::GetExtension(path));
		if (szExt != NULL)
		{
			StdString messageName = _T("FileLoaded_");
			messageName += _tcslwr(szExt);
			// send the message
			DWORD msgHash_messageName = CHashString(messageName).GetUniqueID();
			DWORD res = m_pToolBox->SendMessage(msgHash_messageName, sizeof(LPCTSTR), (void*)filename.c_str());
			switch (res)
			{
				case MSG_ERROR:
					MessageBox(_T("Failed to open resource for editing"), _T("Error"), MB_ICONERROR);
					break;
				case MSG_WARNING:
					MessageBox(_T("Warnig message was sent while opening resource for editing"), _T("Warning"), MB_ICONASTERISK);
					break;
			}
		} 

		m_HierarchyControl.SelectItem(item);
	}
}

void CHierarchy::OnBeginLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	HTREEITEM hItem = pInfo->item.hItem;
	HIERARCHYINFO *hi = GetItemData(hItem);
	ASSERT(hi != NULL);
	if (HasEEObjectLabel(hi->objectName))
	{
		if (!IsEEObjectLabelEditable(hi->objectName))
		{
			// this object does not support editing. Cancel editing
			*pResult = TRUE;
			return;
		}
	}
	// store off the old name
	m_szOldName = pInfo->item.pszText;

	*pResult = 0;
}

void CHierarchy::OnEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	// sometimes the text is a bad pointer, esp if you don't do anything with the label
	if ((pInfo->item.pszText == NULL) || (pInfo->item.pszText == _T("")))
	{
		return;
	}

	StdString newName = pInfo->item.pszText;

	// if the old name and new name are the same, may as well return
	if (_tcscmp(m_szOldName, newName) == 0)
	{
		return;
	}
	m_pToolBox->Log(LOGINFORMATION, _T("Scene Editor: renaming %s to %s\n"), (const TCHAR*)m_szOldName, (const TCHAR*)newName);

	HTREEITEM hItem = pInfo->item.hItem;
	HIERARCHYINFO *hi = GetItemData(hItem);
	ASSERT(hi != NULL);
	if (HasEEObjectLabel(hi->objectName))
	{
		CUndoCommandGuard commandGuard;
		CObjectsStateUndo objUndo(hi->objectName);
		SetEEObjectLabel(hi->objectName, newName);
	}
	else
	{
		CHashString hszOldName(m_szOldName);
		CHashString hszNewName(newName);

		CHANGEOBJNAME con;
		con.oldName = &hszOldName;
		con.newName = &hszNewName;
		static DWORD msgHash_RenameObject = CHashString(_T("RenameObject")).GetUniqueID();
		DWORD retval = m_pToolBox->SendMessage(msgHash_RenameObject, sizeof(CHANGEOBJNAME), &con);

		// if the rename was a success...
		// NOTE: retval will be MSG_ERROR if the new name is taken
		if (retval != MSG_HANDLED)
		{
			m_pToolBox->Log(LOGINFORMATION, _T("Scene Editor: could not rename item.\n"));
		}
		else
		{
			CUndoCommandGuard commandGuard;
			UndoForRenameObject(&hszOldName, &hszNewName);
		}
	}
}

//---------------------------------------------------------------------
// Function:	RemoveHierarchyItem
// Description:	Removes a hierarchy item from the CTreeControl
// Parameters:	child = pointer to hierarchy info of object.
// Returns:		TRUE, if successful, otherwise FALSE
//---------------------------------------------------------------------
BOOL CHierarchy::RemoveHierarchyItem(HIERARCHYINFO *child)
{
	// create the item
	return m_HierarchyControl.DeleteItem(child->hItem);
}

//---------------------------------------------------------------------
// Function:	GetItemData
// Description:	Returns the Item data associated with an HTREEITEM
// Parameters:	item = HTREEITEM to retrieve the data from
// Returns:		DWORD, value retrieved
//---------------------------------------------------------------------
HIERARCHYINFO *CHierarchy::GetItemData(HTREEITEM item)
{
	// get the item data
	if( m_HierarchyControl.GetSafeHwnd() )
	{
		return reinterpret_cast<HIERARCHYINFO *>(m_HierarchyControl.GetItemData(item));
	}
	return NULL;
}

//---------------------------------------------------------------------
// Function:	AddHierarchyItem
// Description:	Fills in the CTreeControl with the hierarchy data
// Parameters:	parent = pointer to hierarchy information of parent
//						or NULL, for root
//				child = pointer to hierarchy info of object.
// Returns:		TRUE, if successful, otherwise FALSE
//---------------------------------------------------------------------
BOOL CHierarchy::AddHierarchyItem(HTREEITEM parent, HIERARCHYINFO *child)
{
	TVINSERTSTRUCT tvInsert;

	tvInsert.hParent = parent;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE;
	tvInsert.item.iImage = 0;
	tvInsert.item.iSelectedImage = 0;
	tvInsert.item.pszText = (LPSTR)child->name.c_str();
	tvInsert.item.lParam = (LPARAM)child;
	tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
	tvInsert.item.state = INDEXTOSTATEIMAGEMASK( 1 );

	// create the item
	child->hItem = m_HierarchyControl.InsertItem(&tvInsert);

	return TRUE;
}

void CHierarchy::ClearHierarchy()
{
	m_HierarchyControl.DeleteAllItems(); 
}

HTREEITEM CHierarchy::GetParentItem()
{
	// return root of the tree
	return m_HierarchyControl.GetRootItem();
}

bool CHierarchy::IsItemChecked(HTREEITEM item)
{
	return m_HierarchyControl.GetCheck(item) != FALSE;
}

//---------------------------------------------------------------------
// Function:	OnInitDialog
// Description:	Come here when Initializing the dialog so we can
//				load in the Image lists for the hierarchy
// Parameters:	.
// Returns:		TRUE, unless you change focus
//---------------------------------------------------------------------
BOOL CHierarchy::OnInitDialog() 
{
	HINSTANCE appInst;

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(SceneEditorDLL.hModule);

	CPropertyPage::OnInitDialog();

//	MakeImageList(IDB_TREE_ICONS, m_Icons, GetDC()->GetBkColor());
	// we need to load images here.
//	m_HierarchyControl.SetImageList(&m_Icons, TVSIL_NORMAL);

	m_HierarchyControl.SetParent(this);
	AfxSetResourceHandle(appInst);

	isUnderUpdate = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// select them item in the tree
void CHierarchy::SelectItem(HTREEITEM item)
{
	// Expand the parent, if possible.
	HTREEITEM hParent = m_HierarchyControl.GetParentItem(item);

	// if parent is root don't expand all

	if (hParent != NULL)
	{
		m_HierarchyControl.Expand(hParent, TVE_EXPAND);
	}

	// Ensure the item is visible.
	m_HierarchyControl.EnsureVisible(item);

	m_HierarchyControl.Select(item, TVGN_CARET);
}

BOOL CHierarchy::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		MENUINFOMESSAGE mim;

		static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
		if (MSG_HANDLED == m_pToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(mim), &mim) &&
			NULL != mim.m_hAccel)
		{
			if (TranslateAccelerator(*AfxGetMainWnd(), mim.m_hAccel, pMsg))
			{
				return TRUE;
			}
		}
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

// deselect the item in the tree
void CHierarchy::DeselectItem(HTREEITEM item)
{
	m_HierarchyControl.SelectItem(NULL);
}

//---------------------------------------------------------------------
// Function:	OnDropMessage
// Description:	Come here when some CBaseObject is dropped on us
// Parameters:	wParam = 0
//				lParam = DROPOBJECT pointer.
// Returns:		TRUE
//---------------------------------------------------------------------
LRESULT CHierarchy::OnDropMessage(WPARAM wParam, LPARAM lParam)
{
	DROPOBJECTINFO *pInfo = reinterpret_cast<DROPOBJECTINFO*>(lParam);
	ASSERT(pInfo != NULL);

	LPCTSTR szPath = pInfo->pResourcePageInfo->filepath;
	LPCTSTR szExtension = FileHelpers::GetExtension(szPath);
	if (szExtension == NULL)
	{
		return 0;
	}

	static CHashString hash_CWorld = _T("CWorld");
	CHashString hszActiveScene;
	IHashString *selection = NULL;
	IHashString *selectionType = &hash_CWorld;

	CPoint pt = pInfo->xy;
	m_HierarchyControl.ScreenToClient(&pt);
	HTREEITEM hParentItem = m_HierarchyControl.HitTest(pt);
	if (hParentItem != NULL)
	{
		HIERARCHYINFO *hi = GetItemData(hParentItem);
		if (hi != NULL)
		{
			selection = hi->objectName;
			selectionType = hi->compType;
		}
	}

	if (selection == NULL)
	{
		hszActiveScene = GetActiveScene();
		if (hszActiveScene.IsEmpty())
		{
			return 0;
		}
		// get parent as current scene world
		static CHashString hash_World = _T("World");
		selection = &hszActiveScene;
		if (hash_World == *selection)
		{
			// create a blank scene if necessary
			static DWORD msgHash_CreateBlankScene = CHashString(_T("CreateBlankScene")).GetUniqueID();
			m_pToolBox->SendMessage(msgHash_CreateBlankScene, 0, NULL);
		}
	}
	
	IXMLArchive *pSpawner = NULL;
	if (0 == _tcsicmp(szExtension, "sxl"))
	{
		pSpawner = OpenXMLArchive(szPath);
	}
	else
	{
		pSpawner = GetFileSpawner(szPath);
	}

	if (pSpawner == NULL)
	{
		return 0;
	}

	if (IsLinkAllowed(selectionType, pSpawner))
	{
		BeginWaitCursor();
		pSpawner->SetIsWriting(false);
		pSpawner->SeekTo(0);
		static DWORD msgHash_SetSpawnParentName = CHashString(_T("SetSpawnParentName")).GetUniqueID();
		m_pToolBox->SendMessage(msgHash_SetSpawnParentName, sizeof(selection), selection);

		static DWORD msgHash_SpawnFromXMLArchive = CHashString(_T("SpawnFromXMLArchive")).GetUniqueID();
		if (MSG_HANDLED == m_pToolBox->SendMessage(msgHash_SpawnFromXMLArchive, sizeof(pSpawner), pSpawner))
		{
			// name of spawned object
			CUndoCommandGuard undoCommand;
			UndoForObjectSpawns();
		}
		EndWaitCursor();
	}
	pSpawner->Close();

	return 0;
}

void CHierarchy::OnMouseMove(UINT nFlags, CPoint point)
{
	HWND hwndTV = m_HierarchyControl.m_hWnd;
	UINT xCur = point.x;
	UINT yCur = point.y;
    HTREEITEM htiTarget;  // handle to target item 
    TVHITTESTINFO tvht;  // hit test information 

    if (m_HierarchyControl.m_fDragging) 
    { 
        // Drag the item to the current position of the mouse pointer. 
        ImageList_DragMove(xCur, yCur); 

        // Find out if the pointer is on the item. If it is, 
        // highlight the item as a drop target. 
        tvht.pt.x = xCur; 
        tvht.pt.y = yCur; 
        if ((htiTarget = TreeView_HitTest(hwndTV, &tvht)) != NULL) 
        { 
            TreeView_SelectDropTarget(hwndTV, htiTarget); 
        } 
    } 
}

void CHierarchy::OnLButtonUp(UINT nFlags, CPoint point)
{
	HWND hwndTV = m_HierarchyControl.m_hWnd;

	if (m_HierarchyControl.m_fDragging)
	{
		ImageList_EndDrag();
		ReleaseCapture();
//		ShowCursor(TRUE);
		TreeView_SelectDropTarget( hwndTV, NULL );
		m_HierarchyControl.m_fDragging = FALSE;

		UINT xCur = point.x;
		UINT yCur = point.y;
		HTREEITEM htiTarget;  // handle to target item 
		TVHITTESTINFO tvht;  // hit test information 

		// Find out if the pointer is on an item.
		tvht.pt.x = xCur; 
		tvht.pt.y = yCur; 
		if ((htiTarget = TreeView_HitTest(hwndTV, &tvht)) != NULL) 
		{
			HIERARCHYINFO *dragItem = GetItemData(m_HierarchyControl.m_DragItem);
			HIERARCHYINFO *dropItem = GetItemData(htiTarget);
			if (dragItem && dropItem)
			{
				// cache names because these get deleted and recreated during the change
				CHashString hszObjectName(dragItem->name);
				CHashString hszObjectComp(dragItem->compType);
				CHashString hszParentName(dropItem->name);
				// can't be parented to itself
				if (hszObjectName != hszParentName)
				{
					CUndoCommandGuard undoCommand;
					UndoForSelectObject(&hszObjectName, true);
					ChangeObjectParentName(&hszObjectName, &hszObjectComp, &hszParentName);
					UndoForSelectObject(&hszObjectName, false);
				}
			}
		}
	}
}

void CHierarchy::RenameItem(HTREEITEM renamedItem, IHashString *pNewName)
{
	SetItemText(renamedItem, pNewName->GetString());

	HIERARCHYINFO *hi = GetItemData(renamedItem);
	if (hi)
	{
		delete hi->objectName;
		hi->objectName = new CHashString(pNewName);
	}
}

HIERARCHYINFO* CHierarchy::GetNextItem(HIERARCHYINFO *hi, UINT nCode)
{
	HTREEITEM htItem = m_HierarchyControl.GetNextItem( hi->hItem, nCode );
	if (htItem)
		return GetItemData(htItem);
	else
		return NULL;
}

void CHierarchy::ExpandItem( HTREEITEM Item, UINT nCode )
{
	if( m_HierarchyControl.ItemHasChildren( Item ) )
	{
		for( HTREEITEM ChildItem = m_HierarchyControl.GetChildItem( Item );
				ChildItem != NULL;
				ChildItem = m_HierarchyControl.GetNextItem( ChildItem, TVGN_NEXT ) )
		{
			ExpandItem( ChildItem, nCode );
		}
		m_HierarchyControl.Expand( Item, nCode );
	}
}

void CHierarchy::CollapseScene()
{
	//Grabs the World
	HTREEITEM WorldItem = m_HierarchyControl.GetRootItem();
	
	if( WorldItem != NULL )
	{
		ExpandItem( WorldItem, TVE_COLLAPSE );
	}
}

void CHierarchy::ExpandScene()
{
	HTREEITEM WorldItem = m_HierarchyControl.GetRootItem();
	
	if( WorldItem != NULL )
	{
		ExpandItem( WorldItem, TVE_EXPAND );
	}
}

void CHierarchy::OnRButtonUp(UINT nFlags, CPoint point)
{
	HWND hwndTV = m_HierarchyControl.m_hWnd;

	if (m_HierarchyControl.m_fDragging)
	{
		ImageList_EndDrag();
		ReleaseCapture();
//		ShowCursor(TRUE);
		TreeView_SelectDropTarget( hwndTV, NULL );
		m_HierarchyControl.m_fDragging = FALSE;

		UINT xCur = point.x;
		UINT yCur = point.y;
		HTREEITEM htiTarget;  // handle to target item 
		TVHITTESTINFO tvht;  // hit test information 

		// Find out if the pointer is on an item.
		tvht.pt.x = xCur; 
		tvht.pt.y = yCur; 
		if ((htiTarget = TreeView_HitTest(hwndTV, &tvht)) != NULL) 
		{
			HIERARCHYINFO *dragItem = GetItemData(m_HierarchyControl.m_DragItem);
			HIERARCHYINFO *dropItem = GetItemData(htiTarget);
			if (dragItem && dropItem)
			{
				// cache names because these get deleted and recreated during the change
				m_hsDragObjectName.Init(dragItem->objectName->GetString());
				m_hsDragObjectComp.Init(dragItem->compType->GetString());
				m_hsDragObjectNewParent.Init(dropItem->objectName->GetString());
				// can't be parented to itself
				if (m_hsDragObjectName != m_hsDragObjectNewParent)
				{
					CMenu menu;
					menu.LoadMenu(IDR_RHIERARCHYDRAG);
					CMenu *pMenu = menu.GetSubMenu(0);
					if (pMenu != NULL)
					{
						ClientToScreen(&point);
						pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
					}				
				}
			}
		}
	}	
}

void CHierarchy::OnKeeplocal()
{
	CUndoCommandGuard undoCommand;
	UndoForSelectObject(&m_hsDragObjectName, true);
	// just need to change the name; defaults to keeping local offset
	ChangeObjectParentName(&m_hsDragObjectName, &m_hsDragObjectComp, &m_hsDragObjectNewParent);
	UndoForSelectObject(&m_hsDragObjectName, false);
}

void CHierarchy::OnKeepglobal()
{
	CUndoCommandGuard undoCommand;
	// need to grab global, if we can
	static DWORD msgGetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
	Matrix4x4 globalXForm;
	DWORD getRet = m_pToolBox->SendMessage(msgGetGlobalTransform, sizeof(Matrix4x4), &globalXForm, &m_hsDragObjectName, &m_hsDragObjectComp);
	
	if (getRet == MSG_HANDLED)
	{
		UndoForSelectObject(&m_hsDragObjectName, true);
		static DWORD msgHash_UndoSaveObject = CHashString(_T("UndoSaveObject")).GetUniqueID();
		m_pToolBox->SendMessage(msgHash_UndoSaveObject, sizeof(IHashString), &m_hsDragObjectName, &m_hsDragObjectComp);
	}

	if (ChangeObjectParentName(&m_hsDragObjectName, &m_hsDragObjectComp, &m_hsDragObjectNewParent))
	{
		if (getRet == MSG_HANDLED)
		{
			// go ahead and reset global now
			static DWORD msgSetGlobalTransform = CHashString(_T("SetGlobalTransform")).GetUniqueID();
			m_pToolBox->SendMessage(msgSetGlobalTransform, sizeof(Matrix4x4), &globalXForm, &m_hsDragObjectName, &m_hsDragObjectComp);

			static DWORD msgHash_RedoSaveObject = CHashString(_T("RedoSaveObject")).GetUniqueID();
			m_pToolBox->SendMessage(msgHash_RedoSaveObject, sizeof(IHashString), &m_hsDragObjectName, &m_hsDragObjectComp);

			UndoForSelectObject(&m_hsDragObjectName, false);
		}
	}
	else
	{
		undoCommand.Cancel();
	}
}

bool CHierarchy::ChangeObjectParentName(IHashString *name, IHashString *comp, IHashString *newParent)
{
	CHashString hszOldParent = GetComponentParent(name);
	CHANGEOBJECTPARENT cop;
	cop.objectName = name;
	cop.objectCompType = comp;
	cop.newParentName = newParent;

	static DWORD msgHash_ChangeObjectParent = CHashString(_T("ChangeObjectParent")).GetUniqueID();
	if (MSG_HANDLED == m_pToolBox->SendMessage(msgHash_ChangeObjectParent, sizeof(cop), &cop))
	{
		UndoForChangeParent(name, &hszOldParent, newParent);
		return true;
	}
	return false;
}

UINT CHierarchy::GetItemState(HTREEITEM item, UINT stateMask)
{
	return m_HierarchyControl.GetItemState(item, stateMask);
}

list<DWORD> CHierarchy::GetLinkableObjects(IHashString *componentType) const
{
	list<DWORD> result;
	LINKABLEOBJECTS lob;
	lob.linkableObjects = &result;
	lob.key = componentType;

	// now grab the possible object that can be created when this one is selected.
	static DWORD msgHash_GetLinkableObjects = CHashString(_T("GetLinkableObjects")).GetUniqueID();
	m_pToolBox->SendMessage(msgHash_GetLinkableObjects, sizeof(LINKABLEOBJECTS), &lob);
	return result;
}

bool CHierarchy::IsLinkAllowed(IHashString *componentType, IXMLArchive *pSpawnerArchive) const
{
	// collecting allowed child node types for passed component type
	list<DWORD> linkableObjects = GetLinkableObjects(componentType);
	// then check nodes in the spawner file
	pSpawnerArchive->SetIsWriting(false);
	pSpawnerArchive->SeekTo(0);
	// spawner file may have root element only, so check for this condition also
	if (pSpawnerArchive->GetNumberOfNodes() <= 1)
	{
		return false;
	}
	StdString nodeName;
	// skip root node
	pSpawnerArchive->GetNode(nodeName);
	while (pSpawnerArchive->GetNode(nodeName))
	{
		// only nodes with depth 1 will be added to the component
		if (1 == pSpawnerArchive->GetDepth())
		{
			CHashString hszName = nodeName;
			// so, check these types against allowed node list
			if (0 == count(linkableObjects.begin(), linkableObjects.end(), hszName.GetUniqueID()))
			{
				return false;
			}
		}
	}
	// all existing nodes may be child for the componentType
	return true;
}

StdString CHierarchy::GetFileSpawnerPath(LPCTSTR szResourcePath) const
{
	LPCTSTR szExtension = FileHelpers::GetExtension(szResourcePath);
	StdString spawnerPath;
	m_pToolBox->GetDirectories(&spawnerPath, NULL);
	spawnerPath += _T("\\objectproperties\\FileSpawner.");
	spawnerPath += szExtension;
	spawnerPath += _T(".sxl");
	return spawnerPath;
}

IXMLArchive *CHierarchy::OpenXMLArchive(LPCTSTR szFilePath) const
{
	CHashString streamType(_T("File"));
	CREATEARCHIVE ca;
	ca.streamData = (void*)szFilePath;
	ca.mode = STREAM_MODE_READ;
	ca.streamType = &streamType;
	// call the Archive factory to create an XML archive
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (m_pToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return NULL;
	}
	return static_cast<IXMLArchive *>(ca.archive);
}

IXMLArchive *CHierarchy::GetFileSpawner(LPCTSTR szResourcePath) const
{
	StdString spawerFilePath = CHierarchy::GetFileSpawnerPath(szResourcePath);
	IXMLArchive *pSpawnerArchive = OpenXMLArchive(spawerFilePath);
	if (pSpawnerArchive == NULL)
	{
		return NULL;
	}

	MacrosList macroses;
	FillMacroses(szResourcePath, macroses);
	IXMLArchive *pResult = CreateXMLArchiveStream();
	ASSERT(pResult != NULL);

	StdString nodeName;
	int depth = 0;
	while (pSpawnerArchive->GetNode(nodeName))
	{
		int currentDepth = pSpawnerArchive->GetDepth();
		for (; depth > currentDepth; --depth)
		{
			pResult->EndClass();
		}
		int attributesCount = pSpawnerArchive->GetNumAttributes();
		pResult->StartClass(nodeName);
		++depth;
		while (attributesCount--)
		{
			StdString value;
			StdString tag;
			pSpawnerArchive->Read(value, tag);
			value = ApplyMacroses(value, macroses);
			pResult->Write(value, pSpawnerArchive->GetCurrentNodeName());
		}
	}
	while (depth--)
	{
		pResult->EndClass();
	}
	pSpawnerArchive->Close();
	return pResult;
}

void CHierarchy::FillMacroses(LPCTSTR szResourcePath, MacrosList &macroses)
{
	macroses.clear();
	macroses.push_back(make_pair<StdString, StdString>(_T("%Path%"), szResourcePath));
	StdString fileName = FileHelpers::GetNameWithoutExtension(szResourcePath);
	macroses.push_back(make_pair<StdString, StdString>(_T("%FileName%"), fileName));
}

StdString CHierarchy::ApplyMacroses(const StdString &value, const MacrosList &macroses)
{
	CString res = value.c_str();
	MacrosList::const_iterator it = macroses.begin();
	for (; it != macroses.end(); ++it)
	{
		res.Replace(it->first.c_str(), it->second.c_str());
	}
	return (LPCTSTR)res;
}

///	\brief	return CHashString with text of the item
///	\param	item - HTREEITEM from the tree
///	\return	CHashString with text of the item
CHashString CHierarchy::GetItemName(HTREEITEM item)
{
	return (LPCTSTR)m_HierarchyControl.GetItemText(item);
}

///	\brief	select passed object in the EE
///	\param	hi - pointer to hierarchy item info
void CHierarchy::SelectObject(HIERARCHYINFO *hi)
{
	SELECTOBJECTPARAMS sop;
	sop.object = hi->objectName;
	sop.compType = hi->compType;
	static DWORD msgHash_SelectObject = CHashString(_T("SelectObject")).GetUniqueID();
	m_pToolBox->SendMessage(msgHash_SelectObject, sizeof(sop), &sop);
}

///	\brief	save current hierarchy tree state (expand/collapse and selection)
///	\return pointer to structure with hierarchy tree state
HIERARCHYTREESTATE *CHierarchy::SaveState()
{
	HIERARCHYTREESTATE *pState = new HIERARCHYTREESTATE();
	HTREEITEM item = m_HierarchyControl.GetSelectedItem();
	if (item != NULL)
	{
		pState->selectedName = GetItemName(item).GetUniqueID();
	}
	HTREEITEM rootItem = m_HierarchyControl.GetRootItem();
	if (rootItem != NULL)
	{
		SaveState(rootItem, pState);
	}
	sort(pState->collapsedItems.begin(), pState->collapsedItems.end());
	return pState;
}

///	\brief	helper function for recursive tree state processing
///	\param	item - currently processed tree item
///	\param	pState - pointer to structure with hierarchy state
void CHierarchy::SaveState(HTREEITEM item, HIERARCHYTREESTATE *pState)
{
	HTREEITEM childItem = m_HierarchyControl.GetChildItem(item);
	// items without children are not treeated as collapsed
	if (childItem != NULL)
	{
		const UINT state = m_HierarchyControl.GetItemState(item, TVIS_EXPANDED);
		if (0 == (TVIS_EXPANDED & state))
		{
			// add collapsed item to the list
			pState->collapsedItems.push_back(GetItemName(item).GetUniqueID());
		}

		// we have valid child item for 1 iteration at least
		do
		{
			SaveState(childItem, pState);
			childItem = m_HierarchyControl.GetNextSiblingItem(childItem);
		}
		while (childItem != NULL);
	}
}

///	\brief	restore hierarchy tree state (expand/collapse and selection)
///	\param	pState - pointer to structure with previously saved state
void CHierarchy::RestoreState(const HIERARCHYTREESTATE *pState)
{
	assert(pState != NULL);
	HTREEITEM rootItem = m_HierarchyControl.GetRootItem();
	if (rootItem != NULL)
	{
		RestoreState(rootItem, pState);
		if (0 == pState->selectedName)
		{
			HTREEITEM worldItem = m_HierarchyControl.GetChildItem(rootItem);
			if (worldItem != NULL)
			{
				m_HierarchyControl.SelectItem(worldItem);
			}
			else
			{
				m_HierarchyControl.SelectItem(rootItem);
			}
		}
	}
}

///	\brief	helper function for recursive tree state restore
///	\param	item - currently processed tree item
///	\param	pState - pointer to structure with hierarchy state
void CHierarchy::RestoreState(HTREEITEM item, const HIERARCHYTREESTATE *pState)
{
	const DWORD nameHash = GetItemName(item).GetUniqueID();
	if (nameHash == pState->selectedName)
	{
		m_HierarchyControl.SelectItem(item);
	}
	if (binary_search(pState->collapsedItems.begin(), pState->collapsedItems.end(), nameHash))
	{
		m_HierarchyControl.Expand(item, TVE_COLLAPSE);
	}
	else
	{
		m_HierarchyControl.Expand(item, TVE_EXPAND);
	}

	for (item = m_HierarchyControl.GetChildItem(item);
		item != NULL;
		item = m_HierarchyControl.GetNextSiblingItem(item))
	{
		RestoreState(item, pState);
	}
}

///	\brief	release data for structure with previously saved state
///	\param	pState - pointer to structure
void CHierarchy::DeleteState(HIERARCHYTREESTATE *pState)
{
	assert(pState != NULL);
	delete pState;
}