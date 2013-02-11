//=====================================================================
// File:		ObjectEditor.cpp
// Description: Defines the initialization routines for the DLL.
// Started:		6/16/2005
// Original:	Kyle Swaim
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
//=====================================================================

#include "stdafx.h"
#include "EntityQuery\EntityQueryDefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define OBJECT_EXTENSIONS "v3o"

#define BITMAPSIZE 256

AFX_EXTENSION_MODULE ObjectEditorDLL = { false, NULL, NULL };

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("ObjectEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(ObjectEditorDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(ObjectEditorDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("ObjectEditor.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(ObjectEditorDLL);
	}
	return 1;   // ok
}

extern "C" DWORD GetDLLVersion()
{
	return INTERFACEDLLVERSION;
}

// Exported DLL initialization is run in context of running application
extern "C" IDLL *GetDLLInterface()
{
	// this has to be done after module is intialized.
	return SINGLETONINSTANCE(CObjectEditor);
}

///---------------------------------------------------------------------
/// Function:	GetPriority
/// Description:	DLL's priority
/// Parameters:	.
/// Returns:		DWORD = priority of DLL loaded
///---------------------------------------------------------------------
DWORD CObjectEditor::GetPriority()
{
    return IDR_OBJECTTYPE;
}

//---------------------------------------------------------------------
/// Function:	CObjectEditor
/// Description:	Standard Constructor
/// Parameters:	pi = programs information structure
/// Returns:		.
//---------------------------------------------------------------------
CObjectEditor::CObjectEditor()
{
	SINGLETONINSTANCE(CObjectComponent)->SetParent(this);
	m_ToolBox = EngineGetToolBox();
	m_CurrentBrush = _T("");
	m_BrushName = _T("");
	m_ObjectFootprintBitmap = NULL;

	m_ObjectFootprintBitmap = new CBitmap;
	//m_ObjectFootprintBitmap->CreateBitmap(256, 256, 1, 8, NULL);
	m_ObjectFootprintBitmap->CreateCompatibleBitmap(bitmapDlg.m_BitMapPreview.GetDC(), BITMAPSIZE,BITMAPSIZE);

	m_vecMoveAccum.Set( 0, 0, 0 );
}

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CObjectEditor::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(ObjectEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	DLL's Initialization function, like InitInstance
// Parameters:	.
// Returns:		TRUE, if successful.
//---------------------------------------------------------------------
BOOL CObjectEditor::Initialize()
{
	CString tName;
	HINSTANCE appInst;
		
	// Register the doc templates we provide to the app
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
    if (pApp == NULL)
    {
        return FALSE;
    }

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(ObjectEditorDLL.hModule);

	// put this after setting the menu and accelerators so it won't allocate it's own
	pApp->AddDocTemplate(m_DocTemplate);

	// construct resource info
	m_ObjCollection.name.LoadString(IDS_RESOURCE_NAME);
	m_ObjCollection.pageIndex = &m_PageIndex;

	m_ObjCollection.hIcon = (HICON)LoadImage(ObjectEditorDLL.hModule, MAKEINTRESOURCE(IDR_OBJECTTYPE), 
		IMAGE_ICON, 0, 0, LR_CREATEDIBSECTION);
	VERIFY(m_ObjCollection.hIcon != NULL);

	// value should be 1 - 1000
	m_ObjCollection.priority = IDR_OBJECTTYPE;
	m_ObjCollection.extensions = OBJECT_EXTENSIONS;
	//m_ObjCollection.objectType = _T("CObject");

	AfxSetResourceHandle(appInst);

    return TRUE;
}

//---------------------------------------------------------------------
// Function:	~CObjectEditor
// Description:	Standard Destructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CObjectEditor::~CObjectEditor()
{
	if (m_ObjCollection.hIcon != NULL)
	{
		DestroyIcon(m_ObjCollection.hIcon);
		m_ObjCollection.hIcon = NULL;
	}

	// delete bitmap
	if (m_ObjectFootprintBitmap != NULL)
	{
		delete m_ObjectFootprintBitmap;
		m_ObjectFootprintBitmap = NULL;
	}

	// release memory
	ClearSelectedObjects();
}

///---------------------------------------------------------------------
/// Function:	OnCreate
/// Description:	Called from the main windows create function
/// Parameters:	CMDIFrameWnd *mainWnd = main frames CMDIFrameWnd pointer
/// Returns:		.
//---------------------------------------------------------------------
void CObjectEditor::OnCreate(CMDIFrameWnd *mainWnd)
{
	HINSTANCE appInst;
	// object menu
	HMENU tMenu;
	//	HACCEL tAccel;
	// one new menuitem in the view menu
	HMENU tMenuView;
//	HACCEL tAccelView;

	MENUINFOMESSAGE mim;
	CBitmap menuBitmap;

	m_iRaycastTimer = 0;
	m_bGlobalUpdateTicked = false;

	static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(ObjectEditorDLL.hModule);

	m_ToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	tMenu = LoadMenu(ObjectEditorDLL.hModule, MAKEINTRESOURCE(IDR_OBJECTTYPE));
//	tAccel = LoadAccelerators(ObjectEditorDLL.hModule, MAKEINTRESOURCE(IDR_OBJECTTYPE));
	ASSERT(tMenu != NULL);
//	ASSERT(tAccel != NULL);

	VERIFY(MergeMenus(&tMenu, mim.m_hMenu, _T("&Window")));
//	VERIFY(MergeAccelerators(&tAccel, mim.m_hAccel));

	// load our new resources
	tMenuView = LoadMenu( ObjectEditorDLL.hModule, MAKEINTRESOURCE( IDR_VIEW_OBJECT_PROP_MENU ) );
//	tAccelView = LoadAccelerators( ObjectEditorDLL.hModule, MAKEINTRESOURCE( IDR_VIEW_OBJECT_PROP_MENU ) );
	
	// error check
	ASSERT(tMenuView != NULL);
//	ASSERT(tAccelView != NULL);

	// merge the new stuff
	VERIFY(MergeSubMenus(&tMenuView, tMenu));
	//VERIFY(MergeAccelerators(&tAccelView, mim.m_hAccel));

	//	VERIFY(MergeMenus(&tMenu, m_hMenu, _T("&Window")));
	//	VERIFY(MergeAccelerators(&tAccel, m_hAccel));

	mim.m_hMenu = tMenuView;
//	mim.m_hAccel = tAccel;
	// load up hi-res toolbar icon
	menuBitmap.LoadBitmap(IDB_TOOLBAR);
	// we don't really have to have to toolbar visible do we?
	mim.m_MenuRsrc = IDR_OBJECTTYPE;
	mim.m_MenuBitmap = &menuBitmap;

	static DWORD msgHash_SetMenuInfo = CHashString(_T("SetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	// v3o files are deprecated, so remove this tab from resource pane in GDS
	//static DWORD msgHash_RegisterResourceCollection = CHashString(_T("RegisterResourceCollection")).GetUniqueID();
	//m_ToolBox->SendMessage(msgHash_RegisterResourceCollection, sizeof(RESOURCECOLLECTION), &m_ObjCollection);

	AfxSetResourceHandle(appInst);

	// create the objectview dock window
	if(!m_ObjectDockWin.Create(_T("Option Tree View"), WS_CHILD | WS_VISIBLE, mainWnd, IDD_OBJECT_DLG))
	{
		m_ToolBox->Log( LOGWARNING, _T( "Failed to create option tree view dockable window bar" ) );
        return;
	}

	CRect winRect;
	mainWnd->GetWindowRect(&winRect);
	m_ObjectDockWin.EnableDocking(CBRS_ALIGN_ANY);
	m_ObjectDockWin.SetBarStyle(m_ObjectDockWin.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	mainWnd->DockControlBar(&m_ObjectDockWin, AFX_IDW_DOCKBAR_RIGHT, winRect);

	// setup the dockwindow so it minimizes itself
	m_ObjectDockWin.LockPushPin(FALSE);
}

//---------------------------------------------------------------------
// Function:	OnCmdMsg
// Description:	Called from the main windows OnCmdMsg function.
//				Routes commands through doc/view architecture
// Returns:		TRUE if handled
//---------------------------------------------------------------------
BOOL CObjectEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return m_ObjectDockWin.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
//	return FALSE;
}


void CObjectEditor::UpdateTree(IHashString *name, IHashString *compType)
{
	if (IsWindow(m_ObjectDockWin))
	{
		if (name == NULL || compType == NULL)
		{
			m_ObjectDockWin.ClearOptionTree();
		}
		else
		{
			//m_ObjectDockWin.CreateNewOptionTree(name, compType);
			m_ObjectDockWin.RestoreOptionTree(name, compType);
		}
	}
}

void CObjectEditor::SetSelected( IHashString *compType, IHashString *objName, bool bAdd)
{
	SELECTEDOBJECT selectedObj;

	// for now we only want one object selected, but keep this in place
	// as when we go to multiple objects you still want all the objects.
	ClearSelectedObjects();

	// only add this object if not null
	if (objName != NULL)
	{
		selectedObj.name = new CHashString(objName);
		selectedObj.type = new CHashString(compType);

		m_SelectedObjects.push_back(selectedObj);
	}
}

void CObjectEditor::ClearSelectedObjects()
{
	SELECTEDOBJECTLIST::iterator solIter;
	SELECTEDOBJECT selectedObject;

	for (solIter = m_SelectedObjects.begin(); solIter != m_SelectedObjects.end(); ++solIter)
	{
		selectedObject = *solIter;
		if (selectedObject.name)
		{
			delete selectedObject.name;
		}
		if (selectedObject.type)
		{
			delete selectedObject.type;
		}
	}

	m_SelectedObjects.clear();
}

void CObjectEditor::ClearSelectedObject()
{
	SELECTEDOBJECT selectedObject;

	selectedObject = m_SelectedObjects.front();
	if (selectedObject.name)
	{
		delete selectedObject.name;
	}
	if (selectedObject.type)
	{
		delete selectedObject.type;
	}

	m_SelectedObjects.pop_front(); 
}

void CObjectEditor::SetEditableSelected( IHashString * object, bool bAdd)
{
	// clear list of selected objects
	if( !bAdd )
	{
		OBJECTNAMELIST::iterator iter = m_SelectedEditableObjects.begin();
		for(; iter != m_SelectedEditableObjects.end(); iter++ )
		{
			//unselect objects 				
			REMOVEOBJECTORIENTEDBOXPARAMS roobp;
			roobp.name = &(*iter);
			static DWORD msgHash_RemoveObjectOrientedBox = CHashString(_T("RemoveObjectOrientedBox")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_RemoveObjectOrientedBox, sizeof(REMOVEOBJECTORIENTEDBOXPARAMS), &roobp);
		}	
		m_SelectedEditableObjects.clear();
	}

	if( object )
	{
		static CHashString hsEntity(_T("CEntity"));

		m_SelectedEditableObjects.push_back( CHashString( object->GetString() ) );

		GETBOUNDINGBOXPARAMS gbbp;
		Vec3 boxPos, boxExtents;
		EulerAngle boxRot;
		gbbp.position = &boxPos;
		gbbp.extents = &boxExtents;
		gbbp.rotation = &boxRot;
		static DWORD msgHash_GetBoundingBox = CHashString(_T("GetBoundingBox")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetBoundingBox, sizeof(GETBOUNDINGBOXPARAMS), &gbbp, object) == MSG_HANDLED)
		{
			Vec3 vPos;
			static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &vPos, object, &hsEntity);

			Vec3 min, max, localPos;
			localPos = boxPos - vPos;
			min = localPos - boxExtents;
			max = localPos + boxExtents;

			ADDOBJECTORIENTEDBOXPARAMS aoobp;
			aoobp.name = object;
			aoobp.min = min;
			aoobp.max = max;
            aoobp.red = 255;
			aoobp.green = 255;
			aoobp.blue = 0;
			
			static DWORD msgHash_AddObjectOrientedBox = CHashString(_T("AddObjectOrientedBox")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_AddObjectOrientedBox, sizeof(ADDOBJECTORIENTEDBOXPARAMS), &aoobp);

			ADDAXISPARAMS aAddAxisParams;
			aAddAxisParams.name = object;

			static DWORD msgHash_AddAxis = CHashString(_T("AddAxis")).GetUniqueID();
			m_ToolBox->SendMessage( msgHash_AddAxis, sizeof(ADDAXISPARAMS), &aAddAxisParams );
		}
	}
}

bool CObjectEditor::SingleClickPaint(float mouseX, float mouseY)
{
	TOOLBARACTIVESTRUCT pToolBarStruct;

	// setup toolbar struct
	pToolBarStruct.cBar = &m_ObjectDockWin.m_PaintBar;
	pToolBarStruct.bActive = false;
	static DWORD msgHash_IsToolBarActive = CHashString(_T("IsToolBarActive")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_IsToolBarActive, sizeof(pToolBarStruct), &pToolBarStruct );

	// don't do anything if paint bar isn't active
	if (pToolBarStruct.bActive == TRUE)
	{
		// if mode is erase
		if (m_ObjectDockWin.m_PaintBar.GetPaintMode() == PM_ERASE)
		{
			// erase object at click point
			EraseObjects(mouseX, mouseY);

			// delete object
			return true;
		}
		else if (m_ObjectDockWin.m_PaintBar.GetPaintMode() == PM_PAINT)
		{
			// paint object at click point
			PaintObjects(mouseX, mouseY);
		}
	}

	return false;
}

void CObjectEditor::MouseMove( MOUSECOORDS *pMouseMove )
{
	OBJECTNAMELIST::iterator iter;
	DELETEOBJECTPARAMS dop;
	// mouse movement is scaled for global sensitivity and then scaled again
	float fSensitivity = m_ObjectDockWin.m_ObjectToolPal.GetSensitivity();
	TOOLBARACTIVESTRUCT pToolBarStruct;

	// setup toolbar struct
	pToolBarStruct.cBar = &m_ObjectDockWin.m_PaintBar;
	pToolBarStruct.bActive = false;
	static DWORD msgHash_IsToolBarActive = CHashString(_T("IsToolBarActive")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_IsToolBarActive, sizeof(pToolBarStruct), &pToolBarStruct );

	if (pToolBarStruct.bActive == TRUE)
	{
		// if object painting is enabled
		if (m_ObjectDockWin.m_PaintBar.GetPaintMode() == PM_PAINT)
		{
			// load brush so we can see what we are painting
			LoadObjectBrush(pMouseMove->m_fXPos, pMouseMove->m_fYPos);
		}
		else
		{
			// if we aren't painting and a brush object was created
			if (_tcscmp(m_BrushName.GetString(), _T("")) != 0)
			{
				// make sure to delete it!
				dop.name = &m_BrushName;
				static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop);
				// reset brush names
				m_BrushName = _T("");
				m_CurrentBrush = _T("");
			}
		}
	}

	// if the left button isn't down don't do anything
	if (!pMouseMove->m_LButtonDown)
	{
		m_undoGroup.EndUndoCommand();
		return;
	}

	if (pToolBarStruct.bActive == TRUE)
	{
		if (abs(pMouseMove->m_lXDelta) >= fSensitivity || abs(pMouseMove->m_lYDelta) >= fSensitivity)
		{
			// if object painting is enabled
			if (m_ObjectDockWin.m_PaintBar.GetPaintMode() == PM_PAINT)
			{
				// paint objects
				PaintObjects(pMouseMove->m_fXPos, pMouseMove->m_fYPos);
			}
			else if (m_ObjectDockWin.m_PaintBar.GetPaintMode() == PM_ERASE)
			{
				// erase objects
				EraseObjects(pMouseMove->m_fXPos, pMouseMove->m_fYPos);
			}
		}
	}

	// make sure that an object is selected
	if(m_SelectedEditableObjects.empty())
	{
		return;
	}

	// setup toolbar struct
	pToolBarStruct.cBar = &m_ObjectDockWin.m_ObjectToolPal;
	pToolBarStruct.bActive = false;
	m_ToolBox->SendMessage(msgHash_IsToolBarActive, sizeof(pToolBarStruct), &pToolBarStruct );

	if (pToolBarStruct.bActive == TRUE)
	{
		for (iter = m_SelectedEditableObjects.begin(); iter != m_SelectedEditableObjects.end(); iter++)
		{
			CHashString currentObject = *iter;

			m_ObjectDockWin.MouseMoved(pMouseMove->m_fXPos, 
									pMouseMove->m_fYPos, 
									pMouseMove->m_lXPos, 
									pMouseMove->m_lYPos, 
									pMouseMove->m_lXDelta, 
									pMouseMove->m_lYDelta, 
									&currentObject);
		}
	}
}

void CObjectEditor::PaintObjects(float mouseX, float mouseY)
{
	float fPosX = 0.f; 
	float fPosY = 0.f;
	float fPosZ = 0.f;
	// get selected resource from resource bar
	std::list<RESOURCEPAGEINFO> ResourceList;

	// wait for a redraw between taking actions on mouse movement
	if (!m_bGlobalUpdateTicked)
	{
		return;
	}
	m_bGlobalUpdateTicked = false;

	// ray cast timer
	// don't update every mousemove msg or it will lag (especially deforming!)
	if (m_iRaycastTimer > GetTickCount())
		return;

	m_iRaycastTimer = GetTickCount() + 50; // ray cast every N ms

	// get intersection point on terrain
	if (GetTerrainIntersection(mouseX, mouseY, fPosX, fPosY, fPosZ))
	{
		// get the resource object that is selected
		static DWORD msgHash_GetSelectedResources = CHashString(_T("GetSelectedResources")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetSelectedResources, sizeof(std::list<RESOURCEPAGEINFO>*), &ResourceList );

		// make sure only one resource is selected
		if (ResourceList.size() == 1)
		{
			// get name of object
			CHashString hszResource( m_ToolBox->GetRelativePath( ResourceList.front().filepath ) );
			// spawn object
			SpawnAtPosition(&hszResource, fPosX, fPosY, fPosZ);
		}
	}
}

void CObjectEditor::EraseObjects(float mouseX, float mouseY)
{
	float fPosX = 0.f; 
	float fPosY = 0.f;
	float fPosZ = 0.f;
	Vec3 hitPos;
	DELETEOBJECTPARAMS dop;

	// wait for a redraw between taking actions on mouse movement
	if (!m_bGlobalUpdateTicked)
	{
		return;
	}
	m_bGlobalUpdateTicked = false;

	// ray cast timer
	// don't update every mousemove msg or it will lag (especially deforming!)
	if (m_iRaycastTimer > GetTickCount())
		return;

	m_iRaycastTimer = GetTickCount() + 50; // ray cast every N ms

	// get brush size
	float fBrushSize = m_ObjectDockWin.m_PaintBar.GetBrushSize();

	// get terrain intersection
	if (GetTerrainIntersection(mouseX, mouseY, fPosX, fPosY, fPosZ))
	{
		// setup hit vector
		hitPos.SetX(fPosX);
		hitPos.SetY(fPosY);
		hitPos.SetZ(fPosZ);

		ODQLIST odqList;
		
		GETOBJECTQUERYPARAMS goqs;
		goqs.vPosition = hitPos;
		goqs.fRadius = fBrushSize * 0.5f;
		goqs.pObjectsList = &odqList;
		static DWORD msgHash_SubmitQuery = CHashString(_T("SubmitQuery")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_SubmitQuery, sizeof(GETOBJECTQUERYPARAMS), &goqs) != MSG_HANDLED)
		{
			return;
		}		

		if (!odqList.empty())
		{
			m_undoGroup.StartUndoCommand();
			// go through the objects we might have hit
			for (ODQLIST::iterator objIter = odqList.begin(); objIter != odqList.end(); ++objIter)
			{
				// put details to the undo stack
				UndoForDeleteObject((*objIter).name);
				// get the name
				dop.name = (*objIter).name;
				// delete the object!
				static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop);
			}
		}
	}
}

void CObjectEditor::LoadObjectBrush(float mouseX, float mouseY)
{
	float fPosX = 0.f; 
	float fPosY = 0.f;
	float fPosZ = 0.f;
	Vec3 pos;
	// get selected resource from resource bar
	std::list<RESOURCEPAGEINFO> ResourceList;
	// params for getting the bounding box
	BBOXPARAMS bbox;
	StdString error;
	DELETEOBJECTPARAMS dop;

	// get intersection point on terrain
	if (GetTerrainIntersection(mouseX, mouseY, fPosX, fPosY, fPosZ))
	{
		// get the resource object selected
		static DWORD msgHash_GetSelectedResources = CHashString(_T("GetSelectedResources")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetSelectedResources, sizeof(std::list<RESOURCEPAGEINFO>*), &ResourceList );

		// make sure only one resource is selected
		if (ResourceList.size() == 1)
		{
			// get name of resource/object
			CHashString hszResource( m_ToolBox->GetRelativePath( ResourceList.front().filepath ) );

			// check if the brush is already loaded
			if (m_CurrentBrush.GetUniqueID() != hszResource.GetUniqueID())
			{
				// if we are changing brushes 
				if (_tcscmp(m_CurrentBrush.GetString(), _T("")) != 0)
				{
					// make sure to delete the old one!
					dop.name = &m_BrushName;
					static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop);
				}
			
				// if not load it
				m_CurrentBrush = hszResource.GetString();
			}
			else  // otherwise just move the current brush
			{
				// make sure we have a valid brush
				if (_tcscmp(m_BrushName.GetString(), _T("")) != 0)
				{
					// we don't want the spawned objects to be half above ground and half underground
					// so get the bounding box for the spawned item
					static DWORD msgHash_GetOOBB = CHashString(_T("GetOOBB")).GetUniqueID();
					if (m_ToolBox->SendMessage(msgHash_GetOOBB, sizeof(bbox), &bbox, &m_BrushName) != MSG_HANDLED)
					{
						// log error
						error = _T("Cannot retrieve bounding box for ");
						error += m_BrushName.GetString();
						error += _T("\n");
						m_ToolBox->SetErrorValue(WARN_OBJECT_NOT_FOUND);
						m_ToolBox->Log(LOGWARNING, error);
						return;
					}

					// set position of brush object
					pos.SetX(fPosX);
					pos.SetY(fPosY);
					pos.SetZ(fPosZ+bbox.dimZ);
					static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), &pos, &m_BrushName);
				}
			}
		}
	}
}

bool CObjectEditor::GetTerrainIntersection(float mouseX, float mouseY, float &x, float &y, float &z)
{
	Vec3 rayPos, rayDir;
	Vec3 vecCursorPos;
	RAYFROMMOUSEPARAMS rayfrommouse;

	// calculate terrain point...
	// fill in...
	rayfrommouse.mouseX = mouseX;
	rayfrommouse.mouseY = mouseY;

	rayfrommouse.rayPos = &rayPos;
	rayfrommouse.rayDir = &rayDir;

	static DWORD msgHash_GetRayFromMouseCoords = CHashString(_T("GetRayFromMouseCoords")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetRayFromMouseCoords, sizeof(rayfrommouse), &rayfrommouse ) == MSG_HANDLED)
	{
		//std::list<RAYCOLLISION> listCollisions;
		RAYCOLLISION listCollisions;
		//unsigned int groupMask = 0;
		//ADDCOLLISIONGROUP( groupMask, TERRAINCOLLISIONGROUP );
		RAYCASTPARAMS raycast;
		raycast.rayPos = rayfrommouse.rayPos;
		raycast.rayDir = rayfrommouse.rayDir;
		raycast.listRayCollisions = &listCollisions;
		//raycast.collisionGroup = groupMask;
		raycast.group = TERRAIN;
		static DWORD msgHash_Raycast = CHashString(_T("Raycast")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_Raycast, sizeof(raycast), &raycast );

		//std::list<RAYCOLLISION>::iterator itr = listCollisions.begin();
		//for (; itr != listCollisions.end(); itr++)
		if(listCollisions.object != NULL)
		{
			//RAYCOLLISION rayCollision = *itr;
			//vecCursorPos.Set(rayCollision.hitPoint[0], rayCollision.hitPoint[1], rayCollision.hitPoint[2]);
			vecCursorPos.Set(listCollisions.hitPoint[0], listCollisions.hitPoint[1], listCollisions.hitPoint[2]);
		
			// save off the collision data
			x = vecCursorPos.GetX();
			y = vecCursorPos.GetY();
			z = vecCursorPos.GetZ();
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool CObjectEditor::SpawnAtPosition(IHashString *name, float x, float y, float z, bool brush)
{
	SPAWNATPOSITONPARAMS sapp;
	CString extension;
	CString tempName;
	CHashString fileName;
	float rotMin;
	float rotMax;
	float scaleMin;
	float scaleMax;
	int rotAxis;
	Vec3 scale;
	Vec3 orientation;
	int randomValue;
	float scaleVal;
	float rotVal;
	int rangeInc;
	int rangeLower;

	srand ( (unsigned int)time(NULL) );

	// spawn object
	// get the extension of the item that is selected
	extension = GetExtension(name->GetString());

	if (extension == ".v3o")
	{
		// strip off the extension of the file
		tempName = GetPathNameNoExt(name->GetString());
		// append .sxl (making it the new extension)
		tempName += ".sxl";
	}
	else
	{
		// file is ok as is
		tempName = name->GetString();
	}

	// hash filename
	fileName = tempName.GetString();
	// setup params
	sapp.name = &fileName;
	sapp.x = x;
	sapp.y = y;
	sapp.z = z;

	// if the object we are spawning is not the brush
	if (!brush)
	{
		// if we want to spawn with a random rotation
		if (m_ObjectDockWin.m_PaintBar.RotationEnabled())
		{	
			// grab min and max
			rotMin = m_ObjectDockWin.m_PaintBar.GetRotMin();
			rotMax = m_ObjectDockWin.m_PaintBar.GetRotMax();

			if (rotMin > rotMax)
			{
				// display warning window here??
				return false;
			}

			// grab axis
			rotAxis = m_ObjectDockWin.m_PaintBar.GetRotationAxis();

			// calculate the range increment
			rangeInc = (int)(rotMax - rotMin);
			// set the lower end of the range
			rangeLower = (int)rotMin;

			// see if the range is valid
			if (rangeInc != 0)
			{
				// generate a random value
				randomValue = rand()%rangeInc+rangeLower;
			}
			else
			{
				// otherwise just use the number selected
				randomValue = rangeLower;
			}

			rotVal = (float)randomValue;

			// depending on the rotation axis setup the orientation vector
			switch (rotAxis)
			{
				case AXIS_XYZ:
					orientation.SetX(rotVal);
					orientation.SetY(rotVal);
					orientation.SetZ(rotVal);
					break;
				case AXIS_X:
					orientation.SetX(0.0f);
					orientation.SetY(0.0f);
					orientation.SetZ(rotVal);
					break;
				case AXIS_Y:
					orientation.SetX(0.0f);
					orientation.SetY(rotVal);
					orientation.SetZ(0.0f);
					break;
				case AXIS_Z:
					orientation.SetX(rotVal);
					orientation.SetY(0.0f);
					orientation.SetZ(0.0f);
					break;
				case AXIS_XY:
					orientation.SetX(0.0f);
					orientation.SetY(rotVal);
					orientation.SetZ(rotVal);
					break;
				case AXIS_YZ:
					orientation.SetX(rotVal);
					orientation.SetY(rotVal);
					orientation.SetZ(0.0f);
					break;
				case AXIS_XZ:
					orientation.SetX(rotVal);
					orientation.SetY(0.0f);
					orientation.SetZ(rotVal);
					break;
				default:
					break;
			}
			
			// set orientation parameter
			sapp.orientation = &orientation;
		}
		
		// if we want to spawn with a random scale
		if (m_ObjectDockWin.m_PaintBar.ScaleEnabled())
		{
			// grab min and max
			scaleMin = (m_ObjectDockWin.m_PaintBar.GetScaleMin() * 100.0f);
			scaleMax = (m_ObjectDockWin.m_PaintBar.GetScaleMax() * 100.0f);

			if (scaleMin > scaleMax)
			{
				// display warning?
				return false;
			}

			// calculate the range increment
			rangeInc = (int)(scaleMax - scaleMin);
			// set the lower end of the range
			rangeLower = (int)scaleMin;

			// see if the range is valid
			if (rangeInc != 0)
			{
				// generate a random value
				randomValue = rand()%rangeInc+rangeLower;
			}
			else
			{
				// otherwise just use the number selected
				randomValue = rangeLower;
			}

			// scale down
			scaleVal = randomValue / 100.0f;

			// setup scale vector
			scale.SetX(scaleVal);
			scale.SetY(scaleVal);
			scale.SetZ(scaleVal);
			// set parameter
			sapp.scale = &scale;
		}
	}

	// send spawn message!
	static DWORD msgHash_SpawnItemAtPosition = CHashString(_T("SpawnItemAtPosition")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_SpawnItemAtPosition, sizeof(sapp), &sapp) != MSG_HANDLED)
	{
		// couldn't spawn specified file
		return false;
	}

	// name of spawned object
	m_undoGroup.StartUndoCommand();
	CHashString spawnedName = UndoForObjectSpawns();

	// if the object we are spawning is not the brush
	if (!brush && m_ObjectDockWin.m_PaintBar.TerrainLevelerEnabled())
	{
		Vec3 vec(x, y, z);
		LevelTerrainUnderObject(&spawnedName, &vec);
	}

	return true;
}

CString CObjectEditor::GetExtension(const TCHAR *filepath)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath( filepath, drive, dir, fname, ext );
	CString e(ext);
	return CString(e);
}

// get the pathname without the extension
CString CObjectEditor::GetPathNameNoExt(const TCHAR *filepath)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath( filepath, drive, dir, fname, ext );
	CString d(drive);
	CString dr(dir);
	CString f(fname);
	return CString(d+dr+f);
}

void CObjectEditor::OnGlobalUpdateTick()
{
	m_bGlobalUpdateTicked = true;
}

bool CObjectEditor::LevelTerrainUnderObject(IHashString *name, Vec3 *hitPos)
{
	// params for getting the bounding box
	BBOXPARAMS bbox;
	StdString error;
	std::list<IHashString*> SectorList;
	GETAFFECTEDOBJECTSPARAMS gats;
	std::list<IHashString*>::iterator itr;
	IHashString *pSectorName;
	BRUSHPARAMS brush;
	bool bWorldModified = false;
	// current orientation of the object
	//Vec3 rot;
	FLOATPOINT footPrint[4];
	FLOATPOINT newFootPrint[4];
	POINT points[4];
	float rads;
	DWORD i;
	//const float BITMAPSIZE = 128.0f;
	// brush data size
	int brushDataSize = (int)(BITMAPSIZE*BITMAPSIZE);
	// array to hold brush data
	float *brushData = NULL;
	// TESTING ONLY (for 1 pixel map)
	//float *brushData = new float[1];
	float maxX = -9999999.f;
	float minX = 999999.f;
	float maxY = -9999999.f;
	float minY = 999999.f;
	float width;
	float height;
	float ratio;
	float offsetY[4];
	float offsetX[4];
	float offsetDistance;
	// biggest distance (width or height)
	float bd;
	// buffer for bitmap
	unsigned char *pData;
	CBitmap *oldBitmap;

	// get the bounding box for the object
	static DWORD msgHash_GetOOBB = CHashString(_T("GetOOBB")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetOOBB, sizeof(bbox), &bbox, name, &CHashString(_T("CPhysicsObject"))) != MSG_HANDLED)
	{
		// log error
		error = _T("Cannot retrieve bounding box for ");
		error += name->GetString();
		error += _T("\n");
		m_ToolBox->SetErrorValue(WARN_OBJECT_NOT_FOUND);
		m_ToolBox->Log(LOGWARNING, error);
		return false;
	}
	
	/// get basic footprint from the bounding box
	// left top
	footPrint[0].x= -bbox.dimX;
	footPrint[0].y = bbox.dimY;
	// right top
	footPrint[1].x = bbox.dimX;
	footPrint[1].y = bbox.dimY;
	// right bottom
	footPrint[2].x = bbox.dimX;
	footPrint[2].y = -bbox.dimY;
	// left bottom
	footPrint[3].x = -bbox.dimX;
	footPrint[3].y = -bbox.dimY;

	/*
	footPrint[0].x= -45.2548385f;
	footPrint[0].y = 45.2548385f;
	// right top
	footPrint[1].x = 45.2548385f;
	footPrint[1].y = 45.2548385f;
	// right bottom
	footPrint[2].x = 45.2548385f;
	footPrint[2].y = -45.2548385f;
	// left bottom
	footPrint[3].x = -45.2548385f;
	footPrint[3].y = -45.2548385f;
	*/
	

	// set position of the bitmap window
	//bitmapDlg.SetWindowPos(NULL, 0,0,0,0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW); 
	
	// get device contexts
	CDC *pDC = bitmapDlg.m_BitMapPreview.GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	// select the bitmap
	oldBitmap = memDC.SelectObject(m_ObjectFootprintBitmap);

	// fill the bitmap with black
	memDC.FillSolidRect(0,0,(int)BITMAPSIZE,(int)BITMAPSIZE,RGB(0,0,0));

	// get current yaw 
	rads = (bbox.rotX * ((float)M_PI/180.0f));
	//rads = (45.0f * ((float)M_PI/180.0f));

	// rotate the points in the footprint 
	newFootPrint[0].x = (footPrint[0].x * cos(rads)) - (footPrint[0].y * sin(rads));
	newFootPrint[0].y = (footPrint[0].y * cos(rads)) + (footPrint[0].x * sin(rads));
	newFootPrint[1].x = (footPrint[1].x * cos(rads)) - (footPrint[1].y * sin(rads));
	newFootPrint[1].y = (footPrint[1].y * cos(rads)) + (footPrint[1].x * sin(rads));
	newFootPrint[2].x = (footPrint[2].x * cos(rads)) - (footPrint[2].y * sin(rads));
	newFootPrint[2].y = (footPrint[2].y * cos(rads)) + (footPrint[2].x * sin(rads));
	newFootPrint[3].x = (footPrint[3].x * cos(rads)) - (footPrint[3].y * sin(rads));
	newFootPrint[3].y = (footPrint[3].y * cos(rads)) + (footPrint[3].x * sin(rads));
	
	// find min/max extents
	for (i = 0; i < 4; ++i)
	{
		if (newFootPrint[i].x > maxX)
		{
			maxX = newFootPrint[i].x;
		}
		if (newFootPrint[i].x < minX)
		{
			minX = newFootPrint[i].x;
		}
		if (newFootPrint[i].y > maxY)
		{
			maxY = newFootPrint[i].y;
		}
		if (newFootPrint[i].y < minY)
		{
			minY = newFootPrint[i].y;
		}
	}

	// calculate the width and height
	width = maxX - minX;
	height = maxY - minY;
	
	// compute the x offsets	
	offsetX[0] = (newFootPrint[0].x - minX) / width;
	offsetX[1] = (newFootPrint[1].x - minX) / width;
	offsetX[2] = (newFootPrint[2].x - minX) / width;
	offsetX[3] = (newFootPrint[3].x - minX) / width;

	// compute the y offsets
	offsetY[0] = (newFootPrint[0].y - minY) / height;
	offsetY[1] = (newFootPrint[1].y - minY) / height;
	offsetY[2] = (newFootPrint[2].y - minY) / height;
	offsetY[3] = (newFootPrint[3].y - minY) / height;
	
	// transform world space coords into pixel coords for the bitmap
	// if width is larger than the height
	if (width > height)
	{
		// calculate the height/width ratio
		ratio = height / width;
		// height (in pixels) of the bounding rectangle
		offsetDistance = ((BITMAPSIZE - 1.0f) * height) / width;
		for (i = 0; i < 4; ++i)
		{
			if (newFootPrint[i].x == maxX)
			{
				// this point is the top, set it to 255
				newFootPrint[i].x = 0.0f; 
			}
			else if (newFootPrint[i].x == minX)
			{
				// this point is the bottom, set it to zero
				newFootPrint[i].x = BITMAPSIZE - 1.0f;
			}
			else
			{
				// neither the top or the bottom so calculate point using the offset
				newFootPrint[i].x = (BITMAPSIZE - 1.0f) * offsetX[i];
			}

			if (newFootPrint[i].y == maxY)
			{
				// this point is the top, set it to half the height + half the offsetDistance
				newFootPrint[i].y = (((BITMAPSIZE / 2.0f) - 1.0f) - (offsetDistance / 2));
			}
			else if (newFootPrint[i].y == minY)
			{
				// this point is the top, set it to half the height - half the offsetDistance
				newFootPrint[i].y = (((BITMAPSIZE / 2.0f) - 1.0f) + (offsetDistance / 2));
			}
			else
			{
				// neither the top or the bottom so calculate point using the offset
				newFootPrint[i].y = (offsetDistance*offsetY[i]) + (((BITMAPSIZE - 1.0f) - offsetDistance) / 2);
			}
		}
	}
	else if (width < height)
	{
		// calculate the height/width ratio
		ratio = width / height;

		// width (in pixels) of the bounding rectangle
		offsetDistance = ((BITMAPSIZE - 1.0f) * width) / height;
		
		for (i = 0; i < 4; ++i)
		{
			if (newFootPrint[i].y == maxY)
			{
				// this point is the top, set it to 255
				newFootPrint[i].y = 0.0f; 
			}
			else if (newFootPrint[i].y == minY)
			{
				// this point is the bottom, set it to zero
				newFootPrint[i].y = (BITMAPSIZE - 1.0f);
			}
			else
			{
				// neither the top or the bottom so calculate point using the offset
				newFootPrint[i].y = ((BITMAPSIZE - 1.0f)*offsetY[i]);
			}

			if (newFootPrint[i].x == maxX)
			{
				// this point is the top, set it to half the height + half the offsetDistance
				newFootPrint[i].x = (((BITMAPSIZE / 2.0f) - 1.0f) - (offsetDistance / 2));
			}
			else if (newFootPrint[i].x == minX)
			{
				// this point is the top, set it to half the height - half the offsetDistance
				newFootPrint[i].x = (((BITMAPSIZE / 2.0f) - 1.0f) + (offsetDistance / 2));
			}
			else
			{
				// neither the top or the bottom so calculate point using the offset
				newFootPrint[i].x = (offsetDistance*offsetX[i]) + (((BITMAPSIZE - 1.0f) - offsetDistance) / 2);
			}
		}

	}
	else
	{
		// footprint isn't a rectangle, it's a square
		// so just transform the points based on the offsets
		newFootPrint[0].x = ((BITMAPSIZE - 1.0f)*offsetX[0]); //rtX;
		newFootPrint[0].y = ((BITMAPSIZE - 1.0f)*offsetY[0]); //rtY;
		newFootPrint[1].x = ((BITMAPSIZE - 1.0f)*offsetX[1]); //ltX;
		newFootPrint[1].y = ((BITMAPSIZE - 1.0f)*offsetY[1]); //ltY;
		newFootPrint[2].x = ((BITMAPSIZE - 1.0f)*offsetX[2]); //lbX;
		newFootPrint[2].y = ((BITMAPSIZE - 1.0f)*offsetY[2]); //lbY;
		newFootPrint[3].x = ((BITMAPSIZE - 1.0f)*offsetX[3]); //rbX;
		newFootPrint[3].y = ((BITMAPSIZE - 1.0f)*offsetY[3]); //rbY;
	}

	// compute biggest distance
	bd = max(width, height);

	// create and select a solid white brush
    CBrush brushWhite(RGB(255, 255, 255));
    memDC.SelectObject(&brushWhite);

	// convert points to longs and store in POINT class
	points[0].x = (long)newFootPrint[0].x;
	points[0].y = (long)newFootPrint[0].y;
	points[1].x = (long)newFootPrint[1].x;
	points[1].y = (long)newFootPrint[1].y;
	points[2].x = (long)newFootPrint[2].x;
	points[2].y = (long)newFootPrint[2].y;
	points[3].x = (long)newFootPrint[3].x;
	points[3].y = (long)newFootPrint[3].y;

	// create polygon
	memDC.Polygon(points, 4);

	// select the old bitmap
	memDC.SelectObject(oldBitmap);
	
	// setup the bitmap info
	char *pBitmapInfoBuffer = new char[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];
	LPBITMAPINFO lpbi = reinterpret_cast<LPBITMAPINFO>(pBitmapInfoBuffer);
	lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
	lpbi->bmiHeader.biBitCount = 0;
	lpbi->bmiHeader.biPlanes = 1;
	// get the bits to fill in the bitmap info struct
	GetDIBits((HDC)memDC, (HBITMAP)m_ObjectFootprintBitmap->GetSafeHandle(), 0, BITMAPSIZE, NULL, lpbi, DIB_RGB_COLORS);

	// get last error
	DWORD lastError = GetLastError();

	// allocate space for bit buffer
	pData = new unsigned char[lpbi->bmiHeader.biSizeImage];

	// get the bits again to actually get the bits
	GetDIBits((HDC)memDC, (HBITMAP)m_ObjectFootprintBitmap->GetSafeHandle(), 0, BITMAPSIZE, pData, lpbi, DIB_RGB_COLORS);

	
	brushData = new float[brushDataSize];
	// if we got some bits
	if (pData)
	{
		// convert to values between 0 and 1
		for (int i=0; i < brushDataSize; i++)
		{
			brushData[i] = (float)pData[i*(lpbi->bmiHeader.biBitCount>>3)] / 255.0f;
		}
	}
	
	/*
	// TEMP
	brushData[0] = 1.0f;
	*/

	// delete bitmap info struct
	delete []pBitmapInfoBuffer;

	// release device context
	bitmapDlg.m_BitMapPreview.ReleaseDC(pDC);

	// get all sectors affected by this brush
	gats.fRadius = bd * 0.5f;
	gats.pPosition = hitPos;
	gats.pAffectedObjectsList = &SectorList;
	static DWORD msgHash_GetAffectedTerrainSectors = CHashString(_T("GetAffectedTerrainSectors")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetAffectedTerrainSectors, sizeof(gats), &gats, &CHashString(_T("CTerrainManager")) );
	itr = SectorList.begin();
	while (itr != SectorList.end())
	{
		pSectorName = *itr;

		m_undoGroup.AddUndoObject(pSectorName);

		// setup brush params
		brush.m_pPosition = hitPos;
		brush.m_fSize = bd;
		brush.m_fStrength = hitPos->GetZ();
		brush.m_eBrushOp = BRUSH_OP_MASK_SET;
		brush.m_bMaxCap = false;
		brush.m_bMinCap = false;
		brush.m_pBrushData = brushData;
		brush.m_iBrushDataSize = (int)BITMAPSIZE;
		static DWORD msgHash_TerrainSectorDeform = CHashString(_T("TerrainSectorDeform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TerrainSectorDeform, sizeof(brush), &brush, pSectorName, NULL );

		// save off Terrain sector modified in a list
		static DWORD msgHash_SetSectorModified = CHashString(_T("SetSectorModified")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetSectorModified, sizeof(IHashString), pSectorName);
		UndoForSectorModified(pSectorName);
		bWorldModified = true;
		itr++;
	}

	/*
	// TESTING ONLY (for 1 pixel map)
	Vec3 tempVec;

	tempVec.SetVec(hitPos->GetVec());

	tempVec.SetX(tempVec.GetX() - bbox.dimX);
	tempVec.SetY(tempVec.GetY() + bbox.dimY);

	// get all sectors affected by this brush
	gats.fBrushSize = 1.0f;
	gats.pPosition = &tempVec;
	gats.pAffectedTerrainSectorList = &SectorList;
	static DWORD msgHash_GetAffectedTerrainSectors"), sizeof(gats), &gats, &CHashString(_T("CTerrainManager = CHashString(_T("GetAffectedTerrainSectors"), sizeof(gats), &gats, &CHashString(_T("CTerrainManager"))).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetAffectedTerrainSectors"), sizeof(gats), &gats, &CHashString(_T("CTerrainManager, NULL );
	itr = SectorList.begin();
	while (itr != SectorList.end())
	{
		pSectorName = *itr;

		// setup brush params
		brush.m_pPosition = &tempVec;
		brush.m_fSize = 1.0f;
		brush.m_fStrength = 15; 
		brush.m_eBrushOp = BRUSH_OP_MASK_SET;
		brush.m_bMaxCap = false;
		brush.m_bMinCap = false;
		brush.m_pBrushData = brushData;
		brush.m_iBrushDataSize = 1;
		static DWORD msgHash_TerrainSectorDeform = CHashString(_T("TerrainSectorDeform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TerrainSectorDeform, sizeof(brush), &brush, pSectorName, NULL );

		// save off Terrain sector modified in a list
		static DWORD msgHash_SetSectorModified = CHashString(_T("SetSectorModified")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetSectorModified, sizeof(IHashString), pSectorName);
		bWorldModified = true;
		itr++;
	}
	/////////////////////////////
	tempVec.SetVec(hitPos->GetVec());

	tempVec.SetX(tempVec.GetX() + bbox.dimX);
	tempVec.SetY(tempVec.GetY() + bbox.dimY);

	// get all sectors affected by this brush
	gats.fBrushSize = 1.0f;
	gats.pPosition = &tempVec;
	gats.pAffectedTerrainSectorList = &SectorList;
	static DWORD msgHash_GetAffectedTerrainSectors"), sizeof(gats), &gats, &CHashString(_T("CTerrainManager = CHashString(_T("GetAffectedTerrainSectors"), sizeof(gats), &gats, &CHashString(_T("CTerrainManager"))).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetAffectedTerrainSectors"), sizeof(gats), &gats, &CHashString(_T("CTerrainManager, NULL );
	itr = SectorList.begin();
	while (itr != SectorList.end())
	{
		pSectorName = *itr;

		// setup brush params
		brush.m_pPosition = &tempVec;
		brush.m_fSize = 1.0f;
		brush.m_fStrength = 15; 
		brush.m_eBrushOp = BRUSH_OP_MASK_SET;
		brush.m_bMaxCap = false;
		brush.m_bMinCap = false;
		brush.m_pBrushData = brushData;
		brush.m_iBrushDataSize = 1;
		static DWORD msgHash_TerrainSectorDeform = CHashString(_T("TerrainSectorDeform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TerrainSectorDeform, sizeof(brush), &brush, pSectorName, NULL );

		// save off Terrain sector modified in a list
		static DWORD msgHash_SetSectorModified = CHashString(_T("SetSectorModified")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetSectorModified, sizeof(IHashString), pSectorName);
		bWorldModified = true;
		itr++;
	}
	///////////////////////////////
	tempVec.SetVec(hitPos->GetVec());

	tempVec.SetX(tempVec.GetX() + bbox.dimX);
	tempVec.SetY(tempVec.GetY() - bbox.dimY);

	// get all sectors affected by this brush
	gats.fBrushSize = 1.0f;
	gats.pPosition = &tempVec;
	gats.pAffectedTerrainSectorList = &SectorList;
	static DWORD msgHash_GetAffectedTerrainSectors"), sizeof(gats), &gats, &CHashString(_T("CTerrainManager = CHashString(_T("GetAffectedTerrainSectors"), sizeof(gats), &gats, &CHashString(_T("CTerrainManager"))).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetAffectedTerrainSectors"), sizeof(gats), &gats, &CHashString(_T("CTerrainManager, NULL );
	itr = SectorList.begin();
	while (itr != SectorList.end())
	{
		pSectorName = *itr;

		// setup brush params
		brush.m_pPosition = &tempVec;
		brush.m_fSize = 1.0f;
		brush.m_fStrength = 15; 
		brush.m_eBrushOp = BRUSH_OP_MASK_SET;
		brush.m_bMaxCap = false;
		brush.m_bMinCap = false;
		brush.m_pBrushData = brushData;
		brush.m_iBrushDataSize = 1;
		static DWORD msgHash_TerrainSectorDeform = CHashString(_T("TerrainSectorDeform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TerrainSectorDeform, sizeof(brush), &brush, pSectorName, NULL );

		// save off Terrain sector modified in a list
		static DWORD msgHash_SetSectorModified = CHashString(_T("SetSectorModified")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetSectorModified, sizeof(IHashString), pSectorName);
		bWorldModified = true;
		itr++;
	}
	//////////////////////////////
	tempVec.SetVec(hitPos->GetVec());

	tempVec.SetX(tempVec.GetX() - bbox.dimX);
	tempVec.SetY(tempVec.GetY() - bbox.dimY);

	// get all sectors affected by this brush
	gats.fBrushSize = 1.0f;
	gats.pPosition = &tempVec;
	gats.pAffectedTerrainSectorList = &SectorList;
	static DWORD msgHash_GetAffectedTerrainSectors"), sizeof(gats), &gats, &CHashString(_T("CTerrainManager = CHashString(_T("GetAffectedTerrainSectors"), sizeof(gats), &gats, &CHashString(_T("CTerrainManager"))).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetAffectedTerrainSectors"), sizeof(gats), &gats, &CHashString(_T("CTerrainManager, NULL );
	itr = SectorList.begin();
	while (itr != SectorList.end())
	{
		pSectorName = *itr;

		// setup brush params
		brush.m_pPosition = &tempVec;
		brush.m_fSize = 1.0f;
		brush.m_fStrength = 15; 
		brush.m_eBrushOp = BRUSH_OP_MASK_SET;
		brush.m_bMaxCap = false;
		brush.m_bMinCap = false;
		brush.m_pBrushData = brushData;
		brush.m_iBrushDataSize = 1;
		static DWORD msgHash_TerrainSectorDeform = CHashString(_T("TerrainSectorDeform")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TerrainSectorDeform, sizeof(brush), &brush, pSectorName, NULL );

		// save off Terrain sector modified in a list
		static DWORD msgHash_SetSectorModified = CHashString(_T("SetSectorModified")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetSectorModified, sizeof(IHashString), pSectorName);
		bWorldModified = true;
		itr++;
	}
	*/

	// if the world has been modified
	if (bWorldModified)
	{
		// send message
		static DWORD msgHash_WorldModified = CHashString(_T("WorldModified")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_WorldModified, sizeof(BOOL), (void *)true);
	}

	/*
	// show bitmap dialog
	bitmapDlg.m_BitMapPreview.SetBitmap((HBITMAP)m_ObjectFootprintBitmap->GetSafeHandle());
	bitmapDlg.RunModalLoop(MLF_SHOWONIDLE);
	bitmapDlg.ShowWindow(SW_HIDE);
	*/
	
	delete []brushData;

	// delete bit buffer
	delete [] pData;
	pData = NULL;

	// success! success!
	return true;
}

void CObjectEditor::MoveObject( IHashString *pObjectName, Vec3 vecDelta, bool bUseMoveAccum, float mouseX, float mouseY )
{
	static MathUtil math;
	float fDeltaMoveGridStepThreshold = 1.1f; // amount required in a delta to move object by grid step
	float fGridSize = GetObjectGridToolGridSize();

	// Get the current position of the object
	Vec3 currentPosition;
	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &currentPosition, pObjectName) != MSG_HANDLED)
		return;

	m_vecMoveAccum += vecDelta;

	// Get Camera name
	CHashString hszCameraName;
	IHashString *pCameraName = NULL;
	CONTEXTCAMPARAMS ccp;
	ccp.pRC = NULL;
	ccp.szCameraName = &hszCameraName;
	static DWORD msgHash_GetActiveCamera = CHashString(_T("GetActiveCamera")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetActiveCamera, sizeof(CONTEXTCAMPARAMS), &ccp ) != MSG_HANDLED)
		return;

	pCameraName = ccp.szCameraName;

	// get camera basis vectors
	CHashString ccamTypeName(_T("CCamera"));
	Vec3 vecCamView, vecCamUp, vecCamRight;
	CAMERAVECTORSPARAMS camVectors;
	camVectors.vViewVector = &vecCamView;
	camVectors.vUpVector = &vecCamUp;
	camVectors.vRightVector = &vecCamRight;
	static DWORD msgHash_GetCameraVectors = CHashString(_T("GetCameraVectors")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetCameraVectors, sizeof(camVectors), &camVectors, pCameraName, &ccamTypeName ) != MSG_HANDLED)
		return;

	// this is only used when doing incremental moves like with a mouse
	if (!bUseMoveAccum)
		m_vecMoveAccum.Set( 0, 0, 0 );

	// calc delta according to locked axes
	if (GetObjectMoveLockIndex() == LOCK_X)
	{
		Vec3 planePos( currentPosition );
		Vec3 planeNorm(1.0f, 0.0f, 0.0f);
		Vec3 hitPos;
		if (PlaneMouseRayIntersection( planePos, planeNorm, mouseX, mouseY, hitPos ))
		{
			vecDelta.SetX(0.0f);
			vecDelta.SetY(hitPos.GetY() - currentPosition.GetY());
			vecDelta.SetZ(hitPos.GetZ() - currentPosition.GetZ());
		}

		if( m_ObjectDockWin.m_ObjectToolPal.GetLockValueCheck() )
  		{
  			vecDelta.x = m_ObjectDockWin.m_ObjectToolPal.GetLockValue();
  		}
  		else
  		{
              vecDelta.x = 0;
   		}
  		m_vecMoveAccum.x = 0;
	}
	else if (GetObjectMoveLockIndex() == LOCK_Y)
	{
		Vec3 planePos( currentPosition );
		Vec3 planeNorm(0.0f, 1.0f, 0.0f);
		Vec3 hitPos;
		if (PlaneMouseRayIntersection( planePos, planeNorm, mouseX, mouseY, hitPos ))
		{
			vecDelta.SetX(hitPos.GetX() - currentPosition.GetX());
			vecDelta.SetY(0.0f);
			vecDelta.SetZ(hitPos.GetZ() - currentPosition.GetZ());
		}

		if( m_ObjectDockWin.m_ObjectToolPal.GetLockValueCheck() )
  		{
  			vecDelta.y = m_ObjectDockWin.m_ObjectToolPal.GetLockValue();
  		}
  		else
  		{
              vecDelta.y = 0;
   		}
  		m_vecMoveAccum.y = 0;
	}
	else if (GetObjectMoveLockIndex() == LOCK_Z)
	{
		Vec3 planePos( currentPosition );
		Vec3 planeNorm(0.0f, 0.0f, 1.0f);
		Vec3 hitPos;
		if (PlaneMouseRayIntersection( planePos, planeNorm, mouseX, mouseY, hitPos ))
		{
			vecDelta.SetX(hitPos.GetX() - currentPosition.GetX());
			vecDelta.SetY(hitPos.GetY() - currentPosition.GetY());
			vecDelta.SetZ(0.0f);
		}

		if( m_ObjectDockWin.m_ObjectToolPal.GetLockValueCheck() )
  		{
  			vecDelta.z = m_ObjectDockWin.m_ObjectToolPal.GetLockValue();
  		}
  		else
  		{
              vecDelta.z = 0;
   		}
  		m_vecMoveAccum.z = 0;
	}
	else if(GetObjectMoveLockIndex() == LOCK_XY)
	{
		Vec3 planePos( currentPosition );
		Vec3 planeNorm(1.0f, 0.0f, 0.0f);
		if (abs( planeNorm.Dot(vecCamView) ) < 0.707f) // find which plane is facing camera
			planeNorm = Vec3(0.0f, 1.0f, 0.0f);
		Vec3 hitPos;
		if (PlaneMouseRayIntersection( planePos, planeNorm, mouseX, mouseY, hitPos ))
		{
			vecDelta.SetX(0.0f);
			vecDelta.SetY(0.0f);
			vecDelta.SetZ(hitPos.GetZ() - currentPosition.GetZ());
		}

		m_vecMoveAccum.x = 0;
 		m_vecMoveAccum.y = 0;
	}
	else if(GetObjectMoveLockIndex() == LOCK_YZ)
	{
		Vec3 planePos( currentPosition );
		Vec3 planeNorm(0.0f, 1.0f, 0.0f);
		if (abs( planeNorm.Dot(vecCamView) ) < 0.707f) // find which plane is facing camera
			planeNorm = Vec3(0.0f, 0.0f, 1.0f);
		Vec3 hitPos;
		if (PlaneMouseRayIntersection( planePos, planeNorm, mouseX, mouseY, hitPos ))
		{
			vecDelta.SetX(hitPos.GetX() - currentPosition.GetX());
			vecDelta.SetY(0.0f);
			vecDelta.SetZ(0.0f);
		}

		m_vecMoveAccum.y = 0;
  		m_vecMoveAccum.z = 0;
	}
	else if(GetObjectMoveLockIndex() == LOCK_XZ)
	{
		Vec3 planePos( currentPosition );
		Vec3 planeNorm(1.0f, 0.0f, 0.0f);
		if (abs( planeNorm.Dot(vecCamView) ) < 0.707f) // find which plane is facing camera
			planeNorm = Vec3(0.0f, 0.0f, 1.0f);
		Vec3 hitPos;
		if (PlaneMouseRayIntersection( planePos, planeNorm, mouseX, mouseY, hitPos ))
		{
			vecDelta.SetX(0.0f);
			vecDelta.SetY(hitPos.GetY() - currentPosition.GetY());
			vecDelta.SetZ(0.0f);
		}

		m_vecMoveAccum.x = 0;
  		m_vecMoveAccum.z = 0;
	}
	else if(GetObjectMoveLockIndex() == LOCK_NONE)
	{
		Vec3 planePos( currentPosition );
		Vec3 planeNorm(-vecCamView.GetX(), -vecCamView.GetY(), -vecCamView.GetZ());
		planeNorm.Normalize();
		Vec3 hitPos;
		if (PlaneMouseRayIntersection( planePos, planeNorm, mouseX, mouseY, hitPos ))
		{
			vecDelta.SetX(hitPos.GetX() - currentPosition.GetX());
			vecDelta.SetY(hitPos.GetY() - currentPosition.GetY());
			vecDelta.SetZ(hitPos.GetZ() - currentPosition.GetZ());
		}
	}

	// Calculate the new position
	if (IsObjectGridToolActive())
	{
		if (bUseMoveAccum)
		{
			if (abs(m_vecMoveAccum.x) > fDeltaMoveGridStepThreshold)
			{
				vecDelta.x = (m_vecMoveAccum.x > 0) ? fGridSize : -fGridSize;
				m_vecMoveAccum.x = 0;
			}
			else
				vecDelta.x = 0;

			if (abs(m_vecMoveAccum.y) > fDeltaMoveGridStepThreshold)
			{
				vecDelta.y = (m_vecMoveAccum.y > 0) ? fGridSize : -fGridSize;
				m_vecMoveAccum.y = 0;
			}
			else
				vecDelta.y = 0;

			if (abs(m_vecMoveAccum.z) > fDeltaMoveGridStepThreshold)
			{
				vecDelta.z = (m_vecMoveAccum.z > 0) ? fGridSize : -fGridSize;
				m_vecMoveAccum.z = 0;
			}
			else
				vecDelta.z = 0;
		}

		currentPosition += vecDelta;

		switch (GetObjectMoveLockIndex())
		{
		case LOCK_X:
			currentPosition.y = math.RoundToNearestMultiple( currentPosition.y, fGridSize );
			currentPosition.z = math.RoundToNearestMultiple( currentPosition.z, fGridSize );
			break;
		case LOCK_Y:
			currentPosition.x = math.RoundToNearestMultiple( currentPosition.x, fGridSize );
			currentPosition.z = math.RoundToNearestMultiple( currentPosition.z, fGridSize );
			break;
		case LOCK_Z:
			currentPosition.x = math.RoundToNearestMultiple( currentPosition.x, fGridSize );
			currentPosition.y = math.RoundToNearestMultiple( currentPosition.y, fGridSize );
			break;
		case LOCK_XY:
			currentPosition.z = math.RoundToNearestMultiple( currentPosition.z, fGridSize );
			break;
		case LOCK_YZ:
			currentPosition.x = math.RoundToNearestMultiple( currentPosition.x, fGridSize );
			break;
		case LOCK_XZ:
			currentPosition.y = math.RoundToNearestMultiple( currentPosition.y, fGridSize );
			break;
		case LOCK_NONE:
			currentPosition.x = math.RoundToNearestMultiple( currentPosition.x, fGridSize );
			currentPosition.y = math.RoundToNearestMultiple( currentPosition.y, fGridSize );
			currentPosition.z = math.RoundToNearestMultiple( currentPosition.z, fGridSize );
			break;
		}
	}
	else
	{
		currentPosition += vecDelta;
	}

    // send a SetPosition message to the selected CEntity
	static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), &currentPosition, pObjectName );
	m_ObjectDockWin.UpdateOptionTree();

	// if hug axis is selected
	if(m_ObjectDockWin.m_ObjectToolPal.GetCollisionCheck() && m_ObjectDockWin.m_ObjectToolPal.GetHugIndex() != HUG_NONE)
	{
		MoveHugged(pObjectName);
	}
}


void CObjectEditor::MoveHugged(IHashString *selectedObject)
{
	// Make sure that a hux axis is selected
	if(m_ObjectDockWin.m_ObjectToolPal.GetHugIndex() == HUG_NONE)
	{
		return;
	}

	// Get the current position of the object
	Vec3 currentPosition;
	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	if(m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &currentPosition, selectedObject) != MSG_HANDLED)
	{
		return;
	}

	// Get the selected hug axis
	int hugAxis = m_ObjectDockWin.m_ObjectToolPal.GetHugIndex();
	Vec3 hugVector;

	// Set the raycast vector from the hug axis
	switch( hugAxis )
	{
	case HUG_NONE:
			return;
		break;
	case HUG_NEG_X:
			hugVector = Vec3(-1.0f, 0.0f, 0.0f);
			currentPosition.SetX( currentPosition.GetX() + 1000 );
		break;
	case HUG_POS_X:
			hugVector = Vec3(1.0f, 0.0f, 0.0f);
			currentPosition.SetX( currentPosition.GetX() - 1000 );
		break;
	case HUG_NEG_Y:
			hugVector = Vec3(0.0f, -1.0f, 0.0f);
			currentPosition.SetY( currentPosition.GetY() + 1000 );
		break;
	case HUG_POS_Y:
			hugVector = Vec3(0.0f, 1.0f, 0.0f);
			currentPosition.SetY( currentPosition.GetY() - 1000 );
		break;
	case HUG_NEG_Z:
			hugVector = Vec3(0.0f, 0.0f, -1.0f);
			currentPosition.SetZ( currentPosition.GetZ() + 1000 );
		break;
	case HUG_POS_Z:
			hugVector = Vec3(0.0f, 0.0f, 1.0f);
			currentPosition.SetZ( currentPosition.GetZ() - 1000 );
		break;
	default:
		break;
	}

	// Raycast
	RAYCOLLISION collision;
	RAYCASTPARAMS raycast;
	raycast.rayPos = &currentPosition;
	raycast.rayDir = &hugVector;
	raycast.listRayCollisions = &collision;
	raycast.group = TERRAIN;
	static DWORD msgHash_Raycast = CHashString( _T("Raycast") ).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_Raycast, sizeof(raycast), &raycast );

	if ( collision.object != NULL )
	{
		RAYCOLLISION rayCollision = collision;
		float shortestDistance = rayCollision.distance;

		hugVector.SetX( hugVector.GetX() * shortestDistance );
		hugVector.SetY( hugVector.GetY() * shortestDistance );
		hugVector.SetZ( hugVector.GetZ() * shortestDistance );

		// Add the current position and the hug move vector to get the new position
		currentPosition.SetX( currentPosition.GetX() + hugVector.GetX() );
		currentPosition.SetY( currentPosition.GetY() + hugVector.GetY() );
		currentPosition.SetZ( currentPosition.GetZ() + hugVector.GetZ() );

		// send a SetPosition message to the selected CEntity
		static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), &currentPosition, selectedObject );
		m_ObjectDockWin.UpdateOptionTree();
	}
	else
	{
		// Did not hit anything
		m_ToolBox->Log( LOGWARNING, _T("Nothing to hug :(\n") );
	}
}

bool CObjectEditor::IsObjectToolActive()
{
	TOOLBARACTIVESTRUCT pToolBarStruct;
	pToolBarStruct.cBar = &m_ObjectDockWin.m_ObjectToolPal;
	pToolBarStruct.bActive = false;
	static DWORD msgHash_IsToolBarActive = CHashString(_T("IsToolBarActive")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_IsToolBarActive, sizeof(pToolBarStruct), &pToolBarStruct );
	return pToolBarStruct.bActive == TRUE;
}

///	\brief	helper function for saving object state to undo stack. 
///			It starts new undo command as well
///	\param	pObjectName - pointer to IHashString with name of saving object
void CObjectEditor::AddUndoObject(IHashString *pObjectName)
{
	m_undoGroup.AddUndoObject(pObjectName);
}

///	\brief	helper function for ending current undo command
void CObjectEditor::EndUndoCommand()
{
	m_undoGroup.EndUndoCommand();
}

bool CObjectEditor::PlaneMouseRayIntersection( const Vec3 &planePos, const Vec3 &planeNorm, float mouseX, float mouseY, Vec3 &hitPos )
{
	RAYFROMMOUSEPARAMS rayfrommouse;
	rayfrommouse.mouseX = mouseX;
	rayfrommouse.mouseY = mouseY;
	Vec3 rayPos, rayDir;
	rayfrommouse.rayPos = &rayPos;
	rayfrommouse.rayDir = &rayDir;
	static DWORD msgHash_GetRayFromMouseCoords = CHashString(_T("GetRayFromMouseCoords")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_GetRayFromMouseCoords, sizeof(rayfrommouse), &rayfrommouse ) == MSG_HANDLED)
	{
		// collide with plane
		float planeD = -planePos.Dot(planeNorm);
		float Vd = planeNorm.Dot(rayDir);
		if (Vd == 0.0f)
			return false; // ray parallel to plane
		float V0 = -(planeNorm.Dot(rayPos) + planeD);
		float t = V0 / Vd;
		hitPos = rayPos + rayDir * t;
		return (t > 0.0f);
	}
	return false;
}
