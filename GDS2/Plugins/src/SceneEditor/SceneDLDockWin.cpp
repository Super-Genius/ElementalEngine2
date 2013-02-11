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

#define ICON_FOLDER_CLOSE 0
#define ICON_WORLD 1

/////////////////////////////////////////////////////////////////////////////
// CSceneDLDockWin dialog

IMPLEMENT_DYNCREATE(CSceneDLDockWin, CGuiControlBar)

CSceneDLDockWin::CSceneDLDockWin() : 
	m_hszSceneRoot(_T("Root"))
{
	//{{AFX_DATA_INIT(CSceneDLDockWin)
	//}}AFX_DATA_INIT

	// add the property sheets
	m_PropSheet.AddPage(&m_Hierarchy);
	m_pDocument = NULL;
	m_ToolBox = EngineGetToolBox();

	SINGLETONINSTANCE(CSceneComponent)->SetParent(this);
}

CSceneDLDockWin::~CSceneDLDockWin()
{
	static DWORD msgHash_ClearHierarchy = CHashString(_T("ClearHierarchy")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_ClearHierarchy, 0, NULL, NULL, NULL);
}

// free up the hierarchy info structures
void CSceneDLDockWin::FreeHierarchyInfo()
{
	NAMETOTREEMAP::iterator nttmIter;
	HTREEITEM curItem;
	HIERARCHYINFO *hi;

	for (nttmIter = m_NameToTree.begin(); nttmIter != m_NameToTree.end(); ++nttmIter)
	{
		curItem = nttmIter->second;
		hi = (HIERARCHYINFO *)m_Hierarchy.GetItemData(curItem);
		// free hierarchy info
		delete hi;
	}
	m_NameToTree.clear();
}

void CSceneDLDockWin::DoDataExchange(CDataExchange* pDX)
{
	CGuiControlBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSceneDLDockWin)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSceneDLDockWin, CGuiControlBar)
	//{{AFX_MSG_MAP(CSceneDLDockWin)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_SAVE_SCENE, OnFileSaveScene)
	ON_COMMAND(ID_FILE_SAVE_AS_SCENE, OnFileSaveAsScene)
	ON_COMMAND(ID_VIEW_SCENE, OnViewScene)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCENE, OnUpdateViewScene)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_CLOSE_SCENE, OnFileCloseScene)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE_SCENE, OnUpdateFileCloseScene)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS_SCENE, OnUpdateFileSaveAsScene)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_SCENE, OnUpdateFileSaveScene)
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_SCENE_COLLAPSESCENE, OnUpdateSceneCollapseScene)
	ON_UPDATE_COMMAND_UI(ID_SCENE_EXPANDSCENE, OnUpdateSceneExpandScene)
	ON_COMMAND(ID_SCENE_COLLAPSESCENE, OnSceneCollapseScene)
	ON_COMMAND(ID_SCENE_EXPANDSCENE, OnSceneExpandScene)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSceneDLDockWin message handlers

int CSceneDLDockWin::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	isUnderUpdate = false;

	CTabCtrl* tab;
	TCITEM tItem;
	CRect rect;

	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText(_T("Hierarchy"));

	// create the property sheet
	m_PropSheet.Create(this, WS_CHILD | WS_VISIBLE, 0);

	tab = m_PropSheet.GetTabControl();

	// Put the tabs at the bottom
	tab->ModifyStyle(WS_BORDER | WS_THICKFRAME | WS_DLGFRAME | WS_CAPTION | TCS_MULTILINE, 
					TCS_BOTTOM | TCS_FIXEDWIDTH | WS_CHILD);

	// now set the tab strings/icons
	tItem.pszText = _T("Hierarchy");
	tItem.mask = TCIF_TEXT;
	tab->SetItem(0, &tItem);

	SetBorders(4, 4, 6, 4);

	StyleDispl(GUISTYLE_XP);

	UpdateHierarchy();

	return 0;
}

void CSceneDLDockWin::OnSize(UINT nType, int cx, int cy) 
{
	CTabCtrl* tab;
	CRect rect;

	CGuiControlBar::OnSize(nType, cx, cy);

	GetClientRect(rect);

	//m_PropSheet.MoveWindow(rect);
	m_PropSheet.GetClientRect(rect);
	tab = m_PropSheet.GetTabControl();
	tab->MoveWindow(rect);

	// adjust PopertyPage inside of CTabCtrl
	tab->AdjustRect(FALSE, rect);

	CPropertyPage *pPage = m_PropSheet.GetActivePage();
	if (pPage)
	{
		pPage->MoveWindow(rect);
	}
}

BOOL CSceneDLDockWin::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// then pump through document
	if (m_pDocument != NULL)
	{
		if (m_pDocument->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}

	return FALSE;
}

// add name to the hierarchy tree control
HTREEITEM CSceneDLDockWin::AddNameToHierarchy(HTREEITEM parent, IHashString *name, IHashString *compType)
{
	NAMETOTREEMAP::iterator ntmIter;

	// see if we have a parent item named this
	ntmIter = m_NameToTree.find(name->GetUniqueID());
	if (ntmIter != m_NameToTree.end())
	{
		// found already so return HTREEITEM
		return ntmIter->second;
	}
	else		// we need to create the HTREEITEM
	{
		HIERARCHYINFO *hi;
		hi = new HIERARCHYINFO;
		IHashString *objName = new CHashString(name->GetString());
		hi->objectName = objName;
		hi->name = GetEEObjectLabel(objName);
		// check if component type was passed
		LPCTSTR szCompTypeName = NULL;
		if (compType == NULL)
		{
			szCompTypeName = GetComponentType(name).GetString();
		}
		else
		{
			szCompTypeName = compType->GetString();
		}
		hi->compType = new CHashString(szCompTypeName);

		m_Hierarchy.AddHierarchyItem(parent, hi);
		m_NameToTree.insert( make_pair(name->GetUniqueID(), hi->hItem));

		return hi->hItem;
	}
}
void CSceneDLDockWin::CloseScene()
{	
	// set under update flags, so the windows won't be refreshed during populating the tree
	setHierarchyUnderUpdateFlag ( TRUE );
	m_Hierarchy.setUnderUpdateFlag( TRUE );

	if (m_pDocument != NULL)
	{	

		m_pDocument->OnCloseDocument();
		m_pDocument = NULL;		
	}

	m_Hierarchy.setUnderUpdateFlag ( FALSE );
	setHierarchyUnderUpdateFlag ( FALSE );
	UpdateHierarchy();
}

// open a scene file.
DWORD CSceneDLDockWin::OpenScene(LPCTSTR fname) 
{
	if (IsSceneOpen())
	{
		// close modified scene or abort, if user cancels
		if (!CloseModifiedScene())
		{
			// user wants to abort, this is OK.
			return S_OK;
		}
	}
	
	// create the underlying document architecture
	CRuntimeClass *pSceneDocClass = RUNTIME_CLASS(CSceneDLDoc);
	m_pDocument = DYNAMIC_DOWNCAST(CSceneDLDoc, pSceneDocClass->CreateObject());

	BOOL success = FALSE;
	if (fname != NULL)
	{
		success = m_pDocument->OnOpenDocument(fname);
	}
	else
	{
		success = m_pDocument->OnNewDocument();
	}

	if (!success)
	{
		m_pDocument->OnCloseDocument();
		m_pDocument = NULL;
		return GetLastError();
	}

	// Reset the camera's position and view
	// get active camera name
	CHashString pCameraName;
	CONTEXTCAMPARAMS ccp;
	ccp.szCameraName = &pCameraName;	
	ccp.pRC = NULL;
	static DWORD msg_GetActiveCamera = CHashString(_T("GetActiveCamera")).GetUniqueID();
	if (m_ToolBox->SendMessage(msg_GetActiveCamera, sizeof(CONTEXTCAMPARAMS), &ccp) != MSG_HANDLED)
	{
		m_ToolBox->Log( LOGWARNING, _T("Failed to get active camera on map loading.\n") );
	}

	// set camera position
	Vec3 camPos(0, 0, 0);
	static DWORD msg_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
	if (m_ToolBox->SendMessage(msg_SetGlobalPosition, sizeof(Vec3), &camPos, &pCameraName, &CHashString(_T("CCamera"))) != MSG_HANDLED)
	{
		m_ToolBox->Log( LOGWARNING, _T("Failed to set camera position on map loading.\n") );
	}

	// Set the camera to look at the focus object
	EulerAngle camRot(0, 0, 1);
	static DWORD msgHash_SetGlobalEuler = CHashString(_T("SetGlobalEuler")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_SetGlobalEuler, sizeof(EulerAngle), &camRot, &pCameraName, &CHashString(_T("CCamera"))) != MSG_HANDLED)
	{
		m_ToolBox->Log( LOGWARNING, _T("Failed to set camera rotation on map loading.\n") );
	}

	// ZSOLT: I comment this out, because it seems that the next message will call this again anyways.
	//UpdateHierarchy();

	m_Hierarchy.ExpandScene();

	// send out a message that the scene is no longer modified
	static DWORD msgHash_WorldModified = CHashString(_T("WorldModified")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_WorldModified, sizeof(BOOL), (void *)false);
	return S_OK;
}

void CSceneDLDockWin::CreateScene()
{
	if (!IsSceneOpen())
	{
		OpenScene(NULL);
	}
}

void CSceneDLDockWin::OnViewScene() 
{
	if ( IsVisible() || IsMinimized() )
	{
		Close();
	}
	else
	{
		// GuiControlBar::Show redocks the controlbar if it
		// was docked or minimized before hiding.
		Show();
	}
}

void CSceneDLDockWin::OnUpdateViewScene( CCmdUI* pCmdUI ) 
{
	// checked if visible
	pCmdUI->SetCheck( IsVisible() || IsMinimized() );

	// always enabled
	pCmdUI->Enable();
}

// function when object has been added to the hierarchy
void CSceneDLDockWin::AddHierarchy(IHashString *name, IHashString *type, IHashString *parent)
{
	NAMETOTREEMAP::iterator ntmIter;

	// call hierarchy to add to list
	// make sure parent is valid
	if (parent != NULL && !parent->IsEmpty())
	{
		// see if we have a parent item named this
		ntmIter = m_NameToTree.find(parent->GetUniqueID());
	
		if (ntmIter != m_NameToTree.end())
		{
			// set htreeitem
			AddNameToHierarchy(ntmIter->second, name, type);
		}
		else 
		{
			// report item not found!
		}
	}
	else
	{
		// since root is null use the top of the tree
		AddNameToHierarchy(m_Hierarchy.GetParentItem(), name, type);
	}
}

void CSceneDLDockWin::RemoveHierarchy(IHashString *name)
{
	NAMETOTREEMAP::iterator ntmIter;

	if (name)
	{
		// call document to remove from list

		// call hierarchy to remove from list
		// see if we have an item by the name coming in
		ntmIter = m_NameToTree.find(name->GetUniqueID());

		if (ntmIter != m_NameToTree.end())
		{
			// create temporary hierarchy info
			HIERARCHYINFO *hi;
			// remove from hierarchy
			hi = (HIERARCHYINFO*)m_Hierarchy.GetItemData(ntmIter->second);

			if ( ! isHierarchyUnderUpdate() )
			{
				// select the next item if we are removing the currently selected object
				if (m_hszSelectedObject == *name)
				{
					SELECTOBJECTPARAMS sop;
					HIERARCHYINFO *hinext = m_Hierarchy.GetNextItem( hi, TVGN_NEXT );
					if (hinext != NULL)
					{
						m_Hierarchy.SelectObject(hinext);
					}
				}
			}

			// recurse thru all children, delete
			HIERARCHYINFO *childHi = m_Hierarchy.GetNextItem(hi, TVGN_CHILD);
			if (childHi)
			{
				RecurseDeleteChildren(childHi);
			}

			// free allocated hash strings
			m_Hierarchy.RemoveHierarchyItem(hi);
			delete hi;
			m_NameToTree.erase(ntmIter);
		
			// clean up state if scene has been deleted
			StateMap::iterator it = m_SceneStates.find(name->GetUniqueID());
			if (it != m_SceneStates.end())
			{
				m_Hierarchy.DeleteState(it->second);
				m_SceneStates.erase(it);
			}
		}
	}
}

void CSceneDLDockWin::RecurseDeleteChildren(HIERARCHYINFO *hi)
{
	HIERARCHYINFO *childHi = m_Hierarchy.GetNextItem(hi, TVGN_CHILD);
	while (childHi)
	{
		RecurseDeleteChildren(childHi);
		childHi = m_Hierarchy.GetNextItem(hi, TVGN_CHILD);
	}
	
	HIERARCHYINFO *siblingHi = m_Hierarchy.GetNextItem(hi, TVGN_NEXT);
	while (siblingHi)
	{
		RecurseDeleteChildren(siblingHi);
		siblingHi = m_Hierarchy.GetNextItem(hi, TVGN_NEXT);
	}

	NAMETOTREEMAP::iterator mapIter = m_NameToTree.find(hi->objectName->GetUniqueID());
	if (mapIter != m_NameToTree.end())
	{
		m_NameToTree.erase(mapIter);
	}

	m_Hierarchy.RemoveHierarchyItem(hi);

	delete hi;
}

void CSceneDLDockWin::SelectFromHierarchy(IHashString *name)
{
	NAMETOTREEMAP::iterator ntmIter;

	if (name)
	{
		m_hszSelectedObject = name->GetString();

		// see if we have an item by the name coming in
		ntmIter = m_NameToTree.find(name->GetUniqueID());

		if (ntmIter != m_NameToTree.end())
		{
			// we do so select from hierarchy
			m_Hierarchy.SelectItem(ntmIter->second);
		}
	}
	else
	{
		m_hszSelectedObject = _T("");
	}
}

void CSceneDLDockWin::CheckSaveScene()
{
	int retval;
	if( IsSceneOpen() )
	{
		retval = MessageBox( _T("Save current scene?"), _T("Save Scene"), MB_YESNO );
		if( retval == IDYES )
		{
			OnFileSaveScene();
		}
	}
}

// For saving a scene file
void CSceneDLDockWin::OnFileSaveScene()
{
	DWORD retVal;
	CString errMess;
	LPCTSTR szScenePathName = m_pDocument->GetPathName();
	
	static DWORD msgHash_SaveFileByExtension = CHashString(_T("SaveFileByExtension")).GetUniqueID();
	retVal =  m_ToolBox->SendMessage(msgHash_SaveFileByExtension, sizeof(LPTSTR), (void*)szScenePathName );
	if (retVal != MSG_HANDLED)
	{
		//Can't save the file using current file path, save as!
		OnFileSaveAsScene();
		//errMess.Format(_T("Unable to save file %s\nError %d\n"), filePathName, GetLastError());
		//MessageBox( errMess, _T("Error Saving File"), MB_OK);
		return;
	}

	// send out a message to all editors to changed modified scene references
	static DWORD msgHash_WorldSave = CHashString(_T("WorldSave")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_WorldSave, sizeof(LPTSTR), (void*)szScenePathName );

	// send out a message that the scene is no longer modified
	static DWORD msgHash_WorldModified = CHashString(_T("WorldModified")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_WorldModified, sizeof(BOOL), (void *)false);
}

// for saving a scene file as...
void CSceneDLDockWin::OnFileSaveAsScene()
{
	DWORD retVal;
	CString errMess;
	StdString oldPathName = m_pDocument->GetPathName();
	TCHAR filePathName[_MAX_PATH];
	
	_tcscpy(filePathName, oldPathName.c_str());

	OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = this->GetSafeHwnd();
    ofn.lpstrFilter = "World Hierarchy XML (*.wxl)\0*.wxl\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filePathName;
    ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOREADONLYRETURN | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = "wxl";

    if(!GetSaveFileName(&ofn))
    {
        return;
    }

	// alert the media, the world file name is about to change!
	WORLDCHANGEFILENAMEPARAMS wchfp;
	wchfp.newName = filePathName;
	wchfp.oldName = (TCHAR *)oldPathName.c_str();

	// if the name is the same 
	if (_tcscmp(wchfp.newName, wchfp.oldName) == 0)
	{
		// just do regular save
		OnFileSaveScene();
	}
	else
	{
		// otherwise do a "save as"
		static DWORD msgHash_WorldChangeFileName = CHashString(_T("WorldChangeFileName")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_WorldChangeFileName, sizeof(WORLDCHANGEFILENAMEPARAMS), &wchfp );

		// now actually save the file by extension
		static DWORD msgHash_SaveFileByExtension = CHashString(_T("SaveFileByExtension")).GetUniqueID();
		retVal =  m_ToolBox->SendMessage(msgHash_SaveFileByExtension, sizeof(LPTSTR), filePathName );
		if (retVal != MSG_HANDLED)
		{
			errMess.Format(_T("Unable to save file %s\nError %d\n"), filePathName, GetLastError());
			MessageBox( errMess, _T("Error Saving File"), MB_OK);
			return;
		}
		else
		{
			// adding information for undo location changes
			IUndoRedoAction *pAction = NULL;

			CUndoCommandGuard undoCommand;
			pAction = new CWorldChangeNameAction(wchfp.oldName, wchfp.newName);
			AddRedoAction(pAction);

			pAction = new CWorldChangeNameAction(wchfp.newName, wchfp.oldName);
			AddUndoAction(pAction);
		}

		// Set the PathName to the new File Name
		m_pDocument->SetPathName(filePathName);

		// send out a message to all editors to changed modified scene references
		static DWORD msgHash_WorldSave = CHashString(_T("WorldSave")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_WorldSave, sizeof(LPTSTR), filePathName );

		// send out a message that the scene is no longer modified
		static DWORD msgHash_WorldModified = CHashString(_T("WorldModified")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_WorldModified, sizeof(BOOL), (void *)false);
	}
}

// close a modified scene, prompting user if they want to save
BOOL CSceneDLDockWin::CloseModifiedScene()
{
	int hr = IDYES;
	if (m_pDocument->IsModified())
	{
		hr = MessageBox(_T("Unsaved changes to scene!\n"
			"Do you want to save your changes?\n"), _T("Unsaved Scene Warning!"),
			MB_YESNOCANCEL);

		if (hr == IDCANCEL)
			return FALSE;
		if (hr == IDYES)
			OnFileSaveScene();
	}

	CloseScene();	

	return TRUE;
}

void CSceneDLDockWin::OnFileCloseScene()
{
	CloseModifiedScene();
}

void CSceneDLDockWin::OnUpdateFileCloseScene(CCmdUI *pCmdUI)
{
	if (!IsSceneOpen())
	{
		pCmdUI->Enable(false);
	}
}

void CSceneDLDockWin::OnUpdateFileSaveAsScene(CCmdUI *pCmdUI)
{
	if (!IsSceneOpen())
	{
		pCmdUI->Enable(false);
	}
}

void CSceneDLDockWin::OnUpdateFileSaveScene(CCmdUI *pCmdUI)
{
	if (!IsSceneOpen() || m_pDocument->GetPathName().IsEmpty())
	{
		pCmdUI->Enable(false);
	}
}

void CSceneDLDockWin::OnDestroy()
{
	// ORDER: Close scene should be called before any resources be released
	CloseScene();
	// free allocated memory
	FreeHierarchyInfo();
	m_Hierarchy.ClearHierarchy();		

	// clean up saved states
	for (StateMap::iterator it = m_SceneStates.begin(); it != m_SceneStates.end(); ++it)
	{
		m_Hierarchy.DeleteState(it->second);
	}
	m_SceneStates.clear();

	CGuiControlBar::OnDestroy();
}

void CSceneDLDockWin::OnWorldModified(BOOL isModified)
{
	if (IsSceneOpen())
	{
		m_pDocument->SetModifiedFlag(isModified);
		UpdateHierarchy();
	}
}

void CSceneDLDockWin::LeftClickObject(IHashString *objectName)
{
	NAMETOTREEMAP::iterator ntmIter;

	ntmIter = m_NameToTree.find(objectName->GetUniqueID());
	if (ntmIter != m_NameToTree.end())
	{
		HTREEITEM selItem;
		selItem = ntmIter->second;
		m_Hierarchy.SelectItem(selItem);
	}
}

void CSceneDLDockWin::GetSceneSavePath( IHashString *pPath )
{
	if (IsSceneOpen())
	{
		*pPath = m_pDocument->GetPathName();
	}
	else
	{
		*pPath = _T("");
	}
}

bool CSceneDLDockWin::IsObjectChecked(IHashString *objectName)
{
	// see if we have an item by the name coming in
	NAMETOTREEMAP::iterator ntmIter = m_NameToTree.find(objectName->GetUniqueID());
	if (ntmIter != m_NameToTree.end())
	{
		HTREEITEM curItem = ntmIter->second;
		return m_Hierarchy.IsItemChecked(curItem);
	}
	return false;
}

void CSceneDLDockWin::RenameObject( IHashString *pOldName, IHashString *pNewName )
{
	NAMETOTREEMAP::iterator oldItemItr = m_NameToTree.find(pOldName->GetUniqueID());
	// if item not in the tree then ignore this request to rename it
	if (oldItemItr == m_NameToTree.end())
		return;

	// change the name within the data in the tree control
	HTREEITEM oldItem = oldItemItr->second;
	m_Hierarchy.RenameItem( oldItem, pNewName );	

	// change the name within our name to tree map
	m_NameToTree.erase( oldItemItr );
	m_NameToTree[pNewName->GetUniqueID()] = oldItem;
}

void CSceneDLDockWin::OnUpdateSceneCollapseScene(CCmdUI *pCmdUI)
{
	if (!IsSceneOpen())
	{
		pCmdUI->Enable(false);
	}
}

void CSceneDLDockWin::OnUpdateSceneExpandScene(CCmdUI *pCmdUI)
{
	if (!IsSceneOpen())
	{
		pCmdUI->Enable(false);
	}
}

void CSceneDLDockWin::OnSceneCollapseScene()
{
	m_Hierarchy.CollapseScene();
}

void CSceneDLDockWin::OnSceneExpandScene()
{
	m_Hierarchy.ExpandScene();
}

void CSceneDLDockWin::UpdateHierarchy(bool force)
{
	// ignore hierarchy updates until window has been created
	if (m_hWnd == NULL)
		return;

	CHashString hszSceneName = GetActiveScene();

	// process if active scene was changed
	if (!force && m_hszActiveScene == hszSceneName)
	{
		return;
	}

	HIERARCHYTREESTATE *pState = m_Hierarchy.SaveState();

	const DWORD hashOldScene = m_hszActiveScene.GetUniqueID();
	StateMap::iterator it = m_SceneStates.find(hashOldScene);
	if (it != m_SceneStates.end())
	{
		m_Hierarchy.DeleteState(it->second);
		m_SceneStates[hashOldScene] = pState;
	}
	else
	{
		m_SceneStates.insert(make_pair(hashOldScene, pState));
	}
	
	m_hszActiveScene = hszSceneName;

	//update the hierarchy view
	FreeHierarchyInfo();

	m_Hierarchy.SetRedraw(FALSE);
	m_Hierarchy.ClearHierarchy();

	if (DoesEEObjectExist(&hszSceneName))
	{
		CHashString hszType = GetComponentType(&hszSceneName);
		AddHierarchy(&hszSceneName, &hszType, NULL);
		UpdateHierarchy(&hszSceneName, &hszType);
	}

	it = m_SceneStates.find(hszSceneName.GetUniqueID());
	if (it != m_SceneStates.end())
	{
		m_Hierarchy.RestoreState(it->second);
	}

	m_Hierarchy.SetRedraw(TRUE);
	m_Hierarchy.RedrawWindow();
}

///	\brief	update hierarchy started for passed object
///	\param	name - name of root object in updated hierarchy
///	\param	type - type of the root object in updated hierarchy
void CSceneDLDockWin::UpdateHierarchy(IHashString *name, IHashString *type)
{
	if (!DoesEEObjectExist(name))
	{
		return;
	}
	TREEINFOLIST til;
	// just let the filename be set, then traverse the hierarchy
	CListAllVisitor visitAll(&til);

	VISITHIERARCHYDATA vhd;
	vhd.name = NULL;
	vhd.visitor = &visitAll;
	vhd.rootNodeName = name;
	static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
	DWORD result;
	result = m_ToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(vhd), &vhd, name, type);
	if (MSG_HANDLED != result)
	{
		m_ToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(vhd), &vhd);
	}

	for (TREEINFOLIST::iterator it = til.begin(); it != til.end(); ++it)
	{
		// grab names of things in tree
		AddHierarchy(it->name, it->compType, it->parentName);
	}
}

void CSceneDLDockWin::ChangeParentName(IHashString *object, IHashString *compType, IHashString *parent)
{
	if (object && compType)
	{
		m_Hierarchy.SetRedraw(FALSE);

		UINT mask = TVIS_EXPANDED;
		UINT state = TVIS_EXPANDED;
		
		HTREEITEM hItem = GetItem(object);
		if (hItem != NULL)
		{
			state = m_Hierarchy.GetItemState(hItem, mask);
		}

		RemoveHierarchy(object);
		UpdateHierarchy(object, compType);

		// reget item
		if (0 == (state & TVIS_EXPANDED))
		{
			HTREEITEM hItem = GetItem(object);
			if (hItem != NULL)
			{
				m_Hierarchy.CollapseItem(hItem);
			}
		}

		m_Hierarchy.SetRedraw(TRUE);
		m_Hierarchy.RedrawWindow();
	}
}

///	\brief	Set file name for opened scene document
///	\param	pFilename - string with new filename
void CSceneDLDockWin::SetWorldFileName(LPCTSTR pFilename)
{
	ASSERT(m_pDocument != NULL);
	m_pDocument->SetPathName(pFilename, FALSE);
}

///	\brief	updated object label if it has been changed
///	\param	name - name of initialized object
void CSceneDLDockWin::OnInitObject(IHashString *name)
{
	if (m_hszActiveScene == *name)
	{
		UpdateHierarchy(true);
	}
	else
	{
		HTREEITEM item = GetItem(name);
		if (item != NULL && HasEEObjectLabel(name))
		{
			StdString label = GetEEObjectLabel(name);
			m_Hierarchy.SetItemText(item, label);
		}
	}
}
