// SceneDLDockWin.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

GUIDTOICON guidIconList[] =
{
	{ &GUID_WORLD, ICON_WORLD },
	{ &GUID_MATRIX3, ICON_FRAME },
	{ &GUID_MATRIX4, ICON_FRAME },
	{ &GUID_QUATERNION, ICON_FRAME },
	{ &GUID_TRANSFORM, ICON_FRAME },
	{ &GUID_PLANE, ICON_FRAME },
	{ &GUID_MESH, ICON_GEOMETRY },
	{ &GUID_CAMERA, ICON_CAMERA },
#if 0	// until we get these implemented
	{ &GUID_LIGHT, ICON_LIGHT },
	{ &GUID_MUSIC, ICON_MUSIC },
	{ &GUID_SCRIPT, ICON_SCRIPT },
	{ &GUID_SOUND, ICON_SOUND}
#endif
};

#define NUM_GUID_ICONS (sizeof(guidIconList)/sizeof(guidIconList[0]))

/////////////////////////////////////////////////////////////////////////////
// CSceneDLDockWin dialog

IMPLEMENT_DYNCREATE(CSceneDLDockWin, CGuiControlBar)

CSceneDLDockWin::CSceneDLDockWin()
{
	//{{AFX_DATA_INIT(CSceneDLDockWin)
	//}}AFX_DATA_INIT

	// add the property sheets
	m_PropSheet.AddPage(&m_Hierarchy);

	m_pDocument = NULL;
}

CSceneDLDockWin::~CSceneDLDockWin()
{
	if (m_pDocument != NULL)
	{
		delete m_pDocument;
		m_pDocument = NULL;
	}

	if (m_ObjToHier.size() != 0)
	{
		FreeList(&m_ObjToHier);
		m_ObjToHier.clear();
	}

}

void CSceneDLDockWin::FreeList(MAPOBJTOHIER *lhi)
{
	MAPOBJTOHIER::iterator lhiIt;
	HIERARCHYINFO *curHI;

	// run through iterator freeing memory
	for (lhiIt = lhi->begin(); lhiIt != lhi->end(); lhiIt++)
	{
		curHI = lhiIt->second;
		delete curHI;
	}
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
	ON_REGISTERED_MESSAGE(AddHierarchyObject, OnAddHierarchy)
	ON_REGISTERED_MESSAGE(RemoveHierarchyObject, OnRemoveHierarchy)
	ON_REGISTERED_MESSAGE(ClearHierarchyObjects, OnClearHierarchy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSceneDLDockWin message handlers

int CSceneDLDockWin::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CTabCtrl* tab;
	TCITEM tItem;
	CRect rect;
	CRuntimeClass *m_pSceneDocument = RUNTIME_CLASS(CSceneDLDoc);

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

	OnClearHierarchy(0, 0);

	return 0;
}

void CSceneDLDockWin::OnSize(UINT nType, int cx, int cy) 
{
	CTabCtrl* tab;
	CRect rect;

	CGuiControlBar::OnSize(nType, cx, cy);

	GetClientRect(rect);

	m_PropSheet.MoveWindow(rect);
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

//---------------------------------------------------------------------
// Function:	AddObjectInfo
// Description:	Adds object icon/text to hierarchy structure
// Parameters:	object = CBaseObject * pointer to object to add
//				enabled = whether to enable the item or not
// Returns:		TRUE, if success
//---------------------------------------------------------------------
BOOL CSceneDLDockWin::AddObjectInfo(CBaseObject *object, BOOL enabled)
{
	HIERARCHYINFO *parentHInfo;
	MAPOBJTOHIER::iterator itMOTH;
	HIERARCHYINFO *tHInfo;
	DWORD i;
	DWORD iconID;
	CBaseObject *parent;
	CBaseObject *world;

	world = gBaseSceneDL->m_ProgramInfo->ObjectManager->GetHeadObject();

	parent = object->GetParent();

	if ((parent == NULL) && (object != world))
	{
		parent = world;
	}

	parentHInfo = NULL;
	// only attempt this if parent is not NULL
	if (parent != NULL)
	{
		// quick lookup of object->hierarchy structure
		itMOTH = m_ObjToHier.find(parent);
		if (itMOTH != m_ObjToHier.end())
		{
			parentHInfo = itMOTH->second;
		}
	}

	iconID = ICON_FOLDER_CLOSE;

	// now look up the icon based on the GUID
	for (i=0; i<NUM_GUID_ICONS; i++)
	{
		if (*(guidIconList[i].type) == *(object->GetType()))
		{
			iconID = guidIconList[i].iconID;
			break;
		}
	}

	tHInfo = new HIERARCHYINFO;
	if (tHInfo == NULL)
	{
		gLogger->Print(_T("Unable to add object to hierarchy."));
		return FALSE;
	}

	tHInfo->image = iconID;
	tHInfo->label = object->GetName();
	tHInfo->enabled = enabled;
	tHInfo->children.clear();

	// save mapping of CBaseObject to tHInfo
	m_ObjToHier[object] = tHInfo;

	m_Hierarchy.AddHierarchyItem(parentHInfo, tHInfo);

	return TRUE;
}

DWORD CSceneDLDockWin::CloseScene(LPCTSTR fname)
{
	if (m_ObjToHier.size() != 0)
	{
		FreeList(&m_ObjToHier);
		m_ObjToHier.clear();
	}

	m_Hierarchy.ClearHierarchy();

	m_pDocument = NULL;
	return S_OK;
}

DWORD CSceneDLDockWin::OpenScene(LPCTSTR fname) 
{
	if (m_pDocument)
	{
		CSceneDLDoc* pSceneDoc = DYNAMIC_DOWNCAST(CSceneDLDoc, m_pDocument);
		pSceneDoc->OnCloseDocument();
	}

	if (m_pDocument)
		return E_FAIL;
	
	// create the underlying document architecture
	CRuntimeClass *m_pSceneDocument = RUNTIME_CLASS(CSceneDLDoc);
	m_pDocument = (CDocument *)m_pSceneDocument->CreateObject();

	if (!m_pDocument->OnOpenDocument(fname))
	{
		m_pDocument->OnCloseDocument();
		return GetLastError();
	}

	OnClearHierarchy(0, 0);

	return S_OK;
}

void CSceneDLDockWin::OnFileSaveScene() 
{
}

void CSceneDLDockWin::OnFileSaveAsScene() 
{	
}

void CSceneDLDockWin::OnViewScene() 
{
	CFrameWnd *mainWnd;

	mainWnd = DYNAMIC_DOWNCAST(CFrameWnd, AfxGetApp()->GetMainWnd());

	if (gBaseSceneDL->m_SceneDLDockWin.IsVisible())
	{
		mainWnd->ShowControlBar(&gBaseSceneDL->m_SceneDLDockWin, FALSE, FALSE);
	}
	else
	{
		mainWnd->ShowControlBar(&gBaseSceneDL->m_SceneDLDockWin, TRUE, FALSE);
	}
	
}

void CSceneDLDockWin::OnUpdateViewScene(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(gBaseSceneDL->m_SceneDLDockWin.IsVisible());
}

//---------------------------------------------------------------------
// Function:	OnAddHierarchy
// Description:	Come here when some CBaseObject added
// Parameters:	wParam = 0
//				lParam = HIEROBJMESSINFO structure pointer
// Returns:		FALSE, to let processing continue
//---------------------------------------------------------------------
LRESULT CSceneDLDockWin::OnAddHierarchy(WPARAM wParam, LPARAM lParam)
{
	HIEROBJMESSINFO *homi;

	homi = (HIEROBJMESSINFO *)lParam;
	if (!AddObjectInfo(homi->object, homi->enable))
	{
		gLogger->Print(_T("Unable to add object to hierarchy."));
	}

	// we handled it, but let others handle this message also
	return FALSE;
}

//---------------------------------------------------------------------
// Function:	OnRemoveHierarchy
// Description:	Come here when CBaseObject is deleted from hierarchy
// Parameters:	wParam = 0
//				lParam = HIEROBJMESSINFO structure pointer
// Returns:		FALSE, to let processing continue
//---------------------------------------------------------------------
LRESULT CSceneDLDockWin::OnRemoveHierarchy(WPARAM wParam, LPARAM lParam)
{
	HIEROBJMESSINFO *homi;
	MAPOBJTOHIER::iterator itMOTH;

	homi = (HIEROBJMESSINFO *)lParam;
	itMOTH = m_ObjToHier.find(homi->object);
	if (itMOTH != m_ObjToHier.end())
	{
		// we found a mapping, meaning it's in our hierarchy
		m_Hierarchy.RemoveHierarchyItem(itMOTH->second);		
	}
	
	// we handled it, but let others handle this message also
	return FALSE;
}

//---------------------------------------------------------------------
// Function:	OnClearHierarchy
// Description:	Come here when all CBaseObjects are deleted from 
//					hierarchy
// Parameters:	wParam = 0
//				lParam = .
// Returns:		FALSE, to let processing continue
//---------------------------------------------------------------------
LRESULT CSceneDLDockWin::OnClearHierarchy(WPARAM wParam, LPARAM lParam)
{
	m_ObjToHier.clear();

	if (!AddObjectInfo(gBaseSceneDL->m_ProgramInfo->ObjectManager->GetHeadObject(), TRUE))
	{
		gLogger->Print(_T("Unable to add world object, aborting."));
		return FALSE;
	}

	// Expand the parent, if possible.
	HTREEITEM hParent = m_Hierarchy.m_HierarchyControl.GetRootItem();
	if (hParent != NULL)
	{
		m_Hierarchy.m_HierarchyControl.Expand(hParent, TVE_EXPAND);
	}

	// we handled it, but let others handle this message also
	return FALSE;
}
