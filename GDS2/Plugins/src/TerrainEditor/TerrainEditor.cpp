//=====================================================================
// File:		TerrainEditor.cpp
// Description: Defines the initialization routines for the DLL.
// Started:		05/12/2005
// Original:	Brian Bazyk
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TERRAIN_FILE_DESCRIPTION _T("Terrain Object")
AFX_EXTENSION_MODULE TerrainEditorDLL = { false, NULL, NULL };

static UINT AddOptionPageMessage = RegisterWindowMessage(ADD_OPTION_PAGE_MSG);
// moved this here, if in .h file will make multiple copies.
const static StdString formatFile = _T("TerrainEditor.bxd");
const static StdString formatDir = _T("\\brushobjects\\");

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("TerrainEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(TerrainEditorDLL, hInstance))
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

		new CDynLinkLibrary(TerrainEditorDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("TerrainEditor.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(TerrainEditorDLL);
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
	return SINGLETONINSTANCE(CTerrainEditor);
}

//---------------------------------------------------------------------
// Function:	GetPriority
// Description:	DLL's priority
// Parameters:	.
// Returns:		DWORD = priority of DLL loaded
//---------------------------------------------------------------------
DWORD CTerrainEditor::GetPriority()
{
    return IDR_TERRAINTYPE;
}

//---------------------------------------------------------------------
// Function:	CTerrainEditor
// Description:	Standard Constructor
// Parameters:	pi = programs information structure
// Returns:		.
//---------------------------------------------------------------------
CTerrainEditor::CTerrainEditor() : 
	m_BrushName(_T("TerrainBrushRenderObject")),
	m_BrushTypeName(_T("CTerrainDecalRenderObject"))
{
	SINGLETONINSTANCE(CTerrainComponent)->SetParent(this);
	m_ToolBox = EngineGetToolBox();
	m_SelectedSector.Init("");
	m_vecBrushPosition.Set(0,0,0);
}

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CTerrainEditor::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(TerrainEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	DLL's Initialization function, like InitInstance
// Parameters:	.
// Returns:		TRUE, if successful.
//---------------------------------------------------------------------
BOOL CTerrainEditor::Initialize()
{
	//set to true, to send out the default size at the beginning 
	m_bDecalCleared = true;

    return TRUE;
}

//---------------------------------------------------------------------
// Function:	~CTerrainEditor
// Description:	Standard Destructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CTerrainEditor::~CTerrainEditor()
{
	// clean up brush render object
	DELETEOBJECTPARAMS dop;
	dop.name = &m_BrushName;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop );

	// clean up brush objects
	BRUSHOBJECTLIST::iterator itr = m_vBrushObjects.begin();
	while (itr != m_vBrushObjects.end())
	{
		itr->Destroy();
		itr++;
	}

	if (m_ObjCollection.hIcon != NULL)
	{
		DestroyIcon(m_ObjCollection.hIcon);
		m_ObjCollection.hIcon = NULL;
	}
}

//---------------------------------------------------------------------
// Function:	OnCreate
// Description:	Called from the main windows create function
// Parameters:	CMDIFrameWnd *mainWnd = main frames CMDIFrameWnd pointer
// Returns:		.
//---------------------------------------------------------------------
void CTerrainEditor::OnCreate(CMDIFrameWnd *mainWnd)
{
	m_iRaycastTimer = 0;
	m_bGlobalUpdateTicked = false;

	CBitmap menuBitmap;

	// get the apps instance handle for loading resources
	HINSTANCE appInst = AfxGetResourceHandle();

	AfxSetResourceHandle(TerrainEditorDLL.hModule);

	StdString basePath;
	m_ToolBox->GetDirectories(&basePath, NULL);
	basePath += formatDir;
	LoadBrushObjects( basePath, formatFile );

	if (!m_TerrainToolPal.Create(mainWnd, CTerrainToolPal::IDD,
      CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY, CTerrainToolPal::IDD))
	{
		TRACE0("Failed to create dockbar\n");
		return;      // fail to create
	}

//	m_TerrainToolPal.EnableDocking(CBRS_ORIENT_ANY);
	
	TOOLBARSTRUCT atbs;

	atbs.hIcon = (HICON)LoadImage(TerrainEditorDLL.hModule, 
		MAKEINTRESOURCE(IDR_TERRAINTYPE), IMAGE_ICON, 0, 0, 0);
	atbs.Text = _T("Terrain");
	atbs.cBar = &m_TerrainToolPal;
	static DWORD msgHash_AddToolBar = CHashString(_T("AddToolBar")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddToolBar, sizeof(TOOLBARSTRUCT), &atbs);

	m_TerrainPropDlg.Create( IDD_SECTOR_TEXTURES, AfxGetApp()->GetMainWnd() );	

	////////////////////////////////////////////////////////////////////////////
	// Dock Win
	if (m_TerrainLayerMatDocWnd.Create(_T("Layer Material Type"), WS_CHILD | WS_VISIBLE, mainWnd, IDD_LAYER_MATERIAL_TYPE))
	{
		CRect winRect;
		mainWnd->GetWindowRect(&winRect);
		m_TerrainLayerMatDocWnd.EnableDocking(CBRS_ALIGN_ANY);
		m_TerrainLayerMatDocWnd.SetBarStyle(m_TerrainLayerMatDocWnd.GetBarStyle() |
			CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		mainWnd->DockControlBar(&m_TerrainLayerMatDocWnd, AFX_IDW_DOCKBAR_LEFT, winRect);
		m_TerrainLayerMatDocWnd.LockPushPin(FALSE);
	}
	////////////////////////////////////////////////////////////////////////////
	
	ShowTerrainPropertiesDialog( false );

	//register our filetype
	FILETYPECREATIONINFO fileInfo;
	fileInfo.m_Description = TERRAIN_FILE_DESCRIPTION;
	fileInfo.m_CreationCallback = CTerrainEditor::TerrainCreationCallback;
	static DWORD msgHash_RegisterFileTypeCreationInfo = CHashString(_T("RegisterFileTypeCreationInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterFileTypeCreationInfo, sizeof( FILETYPECREATIONINFO ),&fileInfo);

	// add terrain option property page to gds
	AfxGetMainWnd()->SendMessage( AddOptionPageMessage, 0, (LPARAM)&m_TerrainOptionsDlg );

	AfxSetResourceHandle(appInst);

	// create terrain brush render object
	CREATEOBJECTPARAMS cop;
	cop.name = &m_BrushName;
	cop.parentName = NULL;
	cop.typeName = &m_BrushTypeName;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(cop), &cop );
	INITOBJECTPARAMS iop;
	iop.name = &m_BrushName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(iop), &iop );

	// brush render object doesn't belong in hierarchy
	static DWORD msgHash_RemoveObject = CHashString(_T("RemoveObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveObject, sizeof(IHashString), cop.name);

	// Zsolt: not sure, that this is the correct place, but I am sure, that this does not need 
	// to be sent out every frame as it was 
	Vec4 brushColor( 1.0f, 0.0f, 0.0f, 0.5f );
	static DWORD msgSetDecalColor = CHashString(_T("SetDecalColor")).GetUniqueID();
	m_ToolBox->SendMessage( msgSetDecalColor, sizeof(Vec4), &brushColor, &m_BrushName, &m_BrushTypeName );
}

//---------------------------------------------------------------------
// Function:	OnCmdMsg
// Description:	Called from the main windows OnCmdMsg function.
//				Routes commands through doc/view architecture
// Returns:		TRUE if handled
//---------------------------------------------------------------------
BOOL CTerrainEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	m_TerrainToolPal.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	return FALSE;
}

//---------------------------------------------------------------------
// Function:	EditDocument
// Description:	Edit an terrain Document retrieving initial data 
//					from a archive
// Parameters:	ar = archive to load into document
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CTerrainEditor::EditDocument( /* IArchive &ar */)
{
	return NO_ERROR;
}

void CTerrainEditor::ClickTerrain( Vec3 *pHitPos, bool bRButton )
{
	TOOLBARACTIVESTRUCT pToolBarStruct;
	pToolBarStruct.cBar = &m_TerrainToolPal;
	pToolBarStruct.bActive = FALSE;
	static DWORD msgHash_IsToolBarActive = CHashString(_T("IsToolBarActive")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_IsToolBarActive, sizeof(pToolBarStruct), &pToolBarStruct );
	if( pToolBarStruct.bActive == FALSE )
	{
		return;
	}

	BRUSHPARAMS brushParams;
	brushParams = GetTerrainCursorBrush();
	brushParams.m_pPosition = &m_vecBrushPosition;
	if (bRButton)
	{
		brushParams.m_fStrength = -brushParams.m_fStrength;
	}

	// get all sectors affected by this brush
	std::list<IHashString*> SectorList;
	GETAFFECTEDOBJECTSPARAMS gats;
	gats.fRadius = brushParams.m_fSize * 0.5f;
	gats.pPosition = pHitPos;
	gats.pAffectedObjectsList = &SectorList;
	static DWORD msgHash_GetAffectedTerrainSectors = CHashString(_T("GetAffectedTerrainSectors")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetAffectedTerrainSectors, sizeof(gats), &gats, &CHashString(_T("CTerrainManager")), NULL );

	bool bWorldModified = false;
	if (m_TerrainToolPal.GetTerrainTool() == kTerrainToolDeform)
	{
		if (brushParams.m_pBrushData == NULL)
			return;

		for (std::list<IHashString*>::iterator itr = SectorList.begin(); itr != SectorList.end(); ++itr)
		{
			// save current heights for terrain sector
			if (m_undoGroup.AddUndoObject(*itr))
			{
				UndoForSectorModified(*itr);
			}
		}

		static DWORD msgHash_TerrainDeform = CHashString(_T("TerrainDeform")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_TerrainDeform, sizeof(BRUSHPARAMS), &brushParams, &CHashString(_T("CTerrainManager")) ) != MSG_HANDLED)
			return;

		bWorldModified = true;
		for (std::list<IHashString*>::iterator itr = SectorList.begin(); itr != SectorList.end(); ++itr)
		{
			SetSectorModified(*itr);
		}
	}
	else if (m_TerrainToolPal.GetTerrainTool() == kTerrainToolPaint)
	{
		// get selected resource from resource bar
		std::list<RESOURCEPAGEINFO> ResourceList;
		static DWORD msgHash_GetSelectedResources = CHashString(_T("GetSelectedResources")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetSelectedResources, sizeof(std::list<RESOURCEPAGEINFO>*), &ResourceList );
		// make sure only one resource is selected
		if (ResourceList.size() == 1)
		{
			CHashString hszResource( m_ToolBox->GetRelativePath( ResourceList.front().filepath ) );

			/* TODO:
			save the number of layers in the current selected terrain sector

			send a terrainpaint message

			if the layer cound of the selected terrain sector changed then reload the sector dialog
			by calling the dialogs set sector name function (not the editors, the editor probably
			exits out early)
			*/
			std::list<IHashString*>::iterator itr = SectorList.begin();
			while (itr != SectorList.end())
			{
				IHashString *pSectorName = *itr;

				CHashString hszROName = GetSectorRenderObject(pSectorName);
				if (m_undoGroup.AddUndoObject(&hszROName))
				{
					UndoForSectorModified(pSectorName);
				}

				TERRAINSECTORPAINTPARAMS tsp;
				tsp.m_Brush = brushParams;
				tsp.m_pSplatTextureName = &hszResource;
				tsp.fNewLayerScale = m_TerrainToolPal.m_fNewLayerScale;
				static DWORD msgHash_TerrainSectorPaint = CHashString(_T("TerrainSectorPaint")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_TerrainSectorPaint, sizeof(tsp), &tsp, pSectorName);

				// save off Terrain sector modified in a list
				SetSectorModified(pSectorName);
				bWorldModified = true;
				itr++;
			}
		}
	}
	else
	{
		//TODO: Put Some Selection Code Here
	}

	if (!m_bWorldIsModified && bWorldModified)
	{
		static DWORD msgHash_WorldModified = CHashString(_T("WorldModified")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_WorldModified, sizeof(BOOL), (void *)true);
	}
}

void CTerrainEditor::SetSelectedSector( IHashString *pSectorName )
{
	// if we don't have a selected sector and we aren't selecting a new sector, do nothing
	if (m_SelectedSector.IsEmpty() && pSectorName == NULL)
		return;

	if (pSectorName)
	{
		// if we already have this sector selected, do nothing
		if (m_SelectedSector == *pSectorName)
			return;

		// set the newly selected sector
		m_SelectedSector.Init( pSectorName->GetString() );
	}
	else
	{
		// set no selection
		m_SelectedSector.Init( _T("") );
	}
	m_TerrainLayerMatDocWnd.m_TerrainLayerMaterialType.SetSectorName( &m_SelectedSector );
}

// save off any changes needed
void CTerrainEditor::OnWorldSave()
{
	VISITHIERARCHYDATA vhd;
	vhd.name = NULL;
	vhd.visitor = &m_SectorRenameVisitor;
	StdString szSavePath, szSaveName;
	GetSavePathName( szSavePath, szSaveName );
	m_SectorRenameVisitor.SetBaseName( szSavePath );
	static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(VISITHIERARCHYDATA), &vhd);


	DWORD retVal;

	SECTORLIST::iterator ssIter;
	for (ssIter = m_SectorModifiedList.begin(); 
			ssIter != m_SectorModifiedList.end(); ssIter++)
	{
		CHashString sectorName(m_ToolBox->GetHashString(*ssIter));
		CHashString hszHeightmapName = GetHeightmapPath(&sectorName);
		if (!hszHeightmapName.IsEmpty())
		{
			// save off the heightmap object
			static DWORD msgHash_SaveFileByExtension = CHashString(_T("SaveFileByExtension")).GetUniqueID();
			retVal = m_ToolBox->SendMessage(msgHash_SaveFileByExtension, sizeof(LPTSTR), (void*)hszHeightmapName.GetString());
			if (retVal != MSG_HANDLED)
			{
				m_ToolBox->Log( LOGERROR, _T("%s(%i): Failed to save heightmap object.\n"), __FILE__, __LINE__ );
			}
		}

		TERRAINLAYERMASKVEC layerMaskList = GetMaskTexturePaths(&sectorName);
		for (TERRAINLAYERMASKVEC::iterator tlmIter = layerMaskList.begin();
									tlmIter != layerMaskList.end(); tlmIter++)
		{
			// save off masks texture
			static DWORD msgHash_SaveFileByExtension = CHashString(_T("SaveFileByExtension")).GetUniqueID();
			retVal =  m_ToolBox->SendMessage(msgHash_SaveFileByExtension, sizeof(LPTSTR), (void *)tlmIter->szMaskTexture->GetString());
			if (retVal != MSG_HANDLED)
			{
				m_ToolBox->Log( LOGERROR, _T("%s(%i): Unable to save mask file."), __FILE__, __LINE__ );
			}
		}
	}
}

// handler for world modification flag
void CTerrainEditor::OnWorldModified(BOOL isModified)
{
	m_bWorldIsModified = isModified;
	if (!m_bWorldIsModified)
	{
		m_SectorModifiedList.clear();
	}

	m_TerrainLayerMatDocWnd.m_TerrainLayerMaterialType.SetSectorName( &m_SelectedSector );
}

void CTerrainEditor::ShowTerrainPropertiesDialog( bool bShow )
{
	if (bShow)
	{
		//m_TerrainPropDlg.SetSectorName(&m_SelectedSector);
		m_TerrainPropDlg.ShowWindow( SW_SHOWNORMAL );
		m_TerrainPropDlg.SetActiveWindow();
		m_TerrainPropDlg.BringWindowToTop();
	}
	else
	{
		m_TerrainPropDlg.ShowWindow( SW_HIDE );
	}
}

void CTerrainEditor::TerrainCreationCallback(void *userData)
{
	CTerrainEditor * pTerrainInstance = SINGLETONINSTANCE(CTerrainEditor);
	if( pTerrainInstance )
	{
		pTerrainInstance->CreateNewTerrain();
	}
}

void CTerrainEditor::CreateNewTerrain()
{
	//creation dialog
	if( !m_NewTerrainDlg.GetSafeHwnd() )
	{
		m_NewTerrainDlg.Create( IDD_TERRAIN_SIZE, AfxGetApp()->GetMainWnd() );
	}
	else
	{
		m_NewTerrainDlg.ShowWindow( SW_SHOW );
		m_NewTerrainDlg.BringWindowToTop();
		m_NewTerrainDlg.UpdateTerrainData();
	}
}

void CTerrainEditor::OnWorldChangeName(WORLDCHANGEFILENAMEPARAMS *nameParms)
{
	VISITHIERARCHYDATA vhd;
	vhd.name = NULL;
	vhd.visitor = &m_SectorRenameVisitor;
	m_SectorRenameVisitor.SetBaseName(nameParms->newName);

	static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(VISITHIERARCHYDATA), &vhd);
}

// set a sector modified by name to our list
void CTerrainEditor::SetSectorModified(IHashString *sectorName)
{
	m_SectorModifiedList.insert(sectorName->GetUniqueID());
}

void CTerrainEditor::SetAllSectorsModified()
{
	CSectorModifyVisitor visitor(this);

	VISITHIERARCHYDATA vhd;
	vhd.name = NULL;
	vhd.visitor = &visitor;
	static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(VISITHIERARCHYDATA), &vhd);
}

BRUSHOBJECTLIST* CTerrainEditor::GetBrushObjects( void )
{
	return (&m_vBrushObjects);
}

bool CTerrainEditor::LoadBrushObjects( StdString wszBasePath,  StdString wszXMLFileName )
{
	IXMLArchive *ar;
	CREATEARCHIVE ca;
	TCHAR temp[256];
	_tcscpy( temp, wszBasePath );
	//_tcscat( temp, _T("\\") );
	_tcscat( temp, wszXMLFileName );
	
	ca.streamData = &temp;
	ca.mode = STREAM_MODE_READ;

	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return false;
	}
	ar = dynamic_cast<IXMLArchive *>(ca.archive);

	StdString tmpTerrainEditorBrushObjects;
	StdString tmpBrushObject;
	StdString wszFileName;
	StdString wszName;
	StdString wszOperator;


	ar->GetNode( tmpTerrainEditorBrushObjects );
	if( _tcscmp( tmpTerrainEditorBrushObjects, _T("TerrainEditorBrushObjects") ) != 0 )
	{
		return false;
	}

	while( ar->GetNode(tmpBrushObject) && _tcscmp(tmpBrushObject, _T("BrushObject")) == 0 )
	{
		ar->Read( wszName, _T("Name")  );
		ar->Read( wszFileName, _T("FileName") );
		ar->Read( wszOperator, _T("Operator") );
	
		StdString tempFilePath = wszBasePath + wszFileName;

		CBrushObject tmpObject(  wszName, tempFilePath, wszOperator );

		// the brush object is initialized in its constructor
		m_vBrushObjects.push_back( tmpObject );
	}

	ar->Close();
	return true;
}

bool CTerrainEditor::InsertTerrainSectorLayer( IHashString *pSectorName, IHashString *pTextureName, IHashString *pInsertLocationName, float fScaleX, float fScaleY )
{
	// get information from the sector about what we should display
	std::vector<TERRAINLAYERINFOPARAMS::TerrainLayer_t> layerList;
	vector<TERRAINLAYERINFOPARAMS::TerrainLayerMask> layerMaskList;
	TERRAINLAYERINFOPARAMS infoParams;
	infoParams.pTerrainLayerList = &layerList;
	infoParams.pTerrainLayerMaskList = &layerMaskList;
	static DWORD msgHash_TerrainLayerInfo = CHashString(_T("TerrainLayerInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_TerrainLayerInfo, sizeof(infoParams), &infoParams, pSectorName );

	// check for max layer count
	if (layerList.size() >= (unsigned int)GetMaxLayers())
		return false;

	// check for dup layer name
	for (unsigned int i=0; i < layerList.size(); i++)
	{
		if (layerList[i].szDetailTexture->GetUniqueID() == pTextureName->GetUniqueID())
			return false;
	}

	CUndoCommandGuard undoCommand;
	CHashString hszROName = GetSectorRenderObject(pSectorName);
	CObjectsStateUndo objectUndo(&hszROName);

	// send a message to add the layer
	TERRAINSECTORINSERTLAYER layermsg;
	layermsg.pSplatInsertLocation = pInsertLocationName;
	layermsg.pSplatTextureName = pTextureName;
	layermsg.fSplatScaleX = fScaleX;
	layermsg.fSplatScaleY = fScaleY;
	static DWORD msgHash_TerrainSectorInsertLayer = CHashString(_T("TerrainSectorInsertLayer")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_TerrainSectorInsertLayer, sizeof(layermsg), &layermsg, pSectorName );
	return true;
}

void CTerrainEditor::MouseMove( MOUSECOORDS *pMouseMove )
{
	// wait for a redraw between taking actions on mouse movement
	if (!m_bGlobalUpdateTicked)
		return;
	m_bGlobalUpdateTicked = false;

	// ray cast timer
	// don't update every mousemove msg or it will lag (especially deforming!)
	if (m_iRaycastTimer > GetTickCount())
		return;

	m_iRaycastTimer = GetTickCount() + 50; // ray cast every N ms

	// if we have a tool active
	if (m_TerrainToolPal.GetTerrainTool() != kTerrainToolSelect)
	{
		IHashString *pObjectName = NULL;
		Vec3 hitPos;

		// cast a ray at the terrain
		RAYFROMMOUSEPARAMS rayfrommouse;
		rayfrommouse.mouseX = pMouseMove->m_fXPos;
		rayfrommouse.mouseY = pMouseMove->m_fYPos;
		Vec3 rayPos, rayDir;
		rayfrommouse.rayPos = &rayPos;
		rayfrommouse.rayDir = &rayDir;
		static DWORD msgHash_GetRayFromMouseCoords = CHashString(_T("GetRayFromMouseCoords")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetRayFromMouseCoords, sizeof(rayfrommouse), &rayfrommouse) == MSG_HANDLED)
		{
			RAYCOLLISION listCollisions;
			RAYCASTPARAMS raycast;
			raycast.rayPos = rayfrommouse.rayPos;
			raycast.rayDir = rayfrommouse.rayDir;
			raycast.listRayCollisions = &listCollisions;
			raycast.group = TERRAIN;
			static DWORD msgHash_Raycast = CHashString(_T("Raycast")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_Raycast, sizeof(raycast), &raycast );

			if (listCollisions.object != NULL)
			{
				RAYCOLLISION rayCollision = listCollisions;

				hitPos.Set( rayCollision.hitPoint[0],rayCollision.hitPoint[1],rayCollision.hitPoint[2] );
				pObjectName = rayCollision.parentobject;

				m_vecBrushPosition = hitPos;

				BRUSHPARAMS brushParams = GetTerrainCursorBrush();

				// get current brush
				BRUSHOBJECTLIST::iterator itrBrush = GetBrushObjects()->begin();
				advance(itrBrush, m_TerrainToolPal.m_iBrushShape);

				SetDecal(&CHashString(itrBrush->m_wszFileName), brushParams.m_fSize);

				// if we hit something
				if (pObjectName != NULL)
				{
					if (pMouseMove->m_LButtonDown || pMouseMove->m_RButtonDown)
					{
						ClickTerrain( &hitPos, pMouseMove->m_RButtonDown );
					}
				}
			}
			else
			{
				ClearDecal();
			}
		}
		else
		{
			ClearDecal();
		}

		if (!pMouseMove->m_LButtonDown && !pMouseMove->m_RButtonDown)
		{
			m_undoGroup.EndUndoCommand();
		}
	}
}

///	\brief	set visible decal
///	\param	pBrushFileName - pointer to IHashString with brush name
///	\param	fSize - size of decal
void CTerrainEditor::SetDecal( IHashString *pBrushFileName, float fSize )
{
	// if cleardecal() was called, than m_bDecalCleared is true. That means
	// that the decal size is 0. (normally between 1-100).
	// so if it is 0 (perhaps because the mouse leaved the terrain), than we 
	// need to restore its original value before rendering.
	if ( m_bDecalCleared )
	{
		m_bDecalCleared = false;
		m_TerrainToolPal.UpdateBrushSize();
		m_TerrainToolPal.UpdateBrushshape();
	}

	static DWORD msgSetPosition = CHashString(_T("SetPosition")).GetUniqueID();
	m_ToolBox->SendMessage( msgSetPosition, sizeof(m_vecBrushPosition), &m_vecBrushPosition, &m_BrushName, &m_BrushTypeName );
	
	//static DWORD msgSetDecalSize = CHashString(_T("SetDecalSize")).GetUniqueID();
	//m_ToolBox->SendMessage( msgSetDecalSize, sizeof(fSize), &fSize, &m_BrushName, &m_BrushTypeName );
}

///	\brief	clear decal from terrain.
void CTerrainEditor::ClearDecal()
{
	m_bDecalCleared = true;
	float fSize = 0;
	static DWORD msgSetDecalSize = CHashString(_T("SetDecalSize")).GetUniqueID();
	m_ToolBox->SendMessage( msgSetDecalSize, sizeof(fSize), &fSize, &m_BrushName, &m_BrushTypeName );
}

void CTerrainEditor::OnGlobalUpdateTick()
{
	m_bGlobalUpdateTicked = true;
}

void CTerrainEditor::GetSavePathName( StdString &out_Path, StdString &out_Name )
{
	IToolBox *pToolBox = EngineGetToolBox();

	CHashString filePath;
	static DWORD msgHash_GetSceneSavePath = CHashString(_T("GetSceneSavePath")).GetUniqueID();
	pToolBox->SendMessage(msgHash_GetSceneSavePath, sizeof(IHashString*), &filePath );

	TCHAR relPathName[_MAX_PATH];
	_tcscpy( relPathName, filePath.GetString() );
	// split up the path
	TCHAR newPath[_MAX_PATH];
	TCHAR newName[_MAX_FNAME];
	_tsplitpath(pToolBox->GetRelativePath(relPathName), NULL, newPath, newName, NULL);
	// make sure lower case for IHashString Compares
	_tcslwr(newPath);
	_tcslwr(newName);

	out_Path = newPath;
	out_Name = newName;
}

BRUSHPARAMS CTerrainEditor::GetTerrainCursorBrush()
{
	// get current brush
	BRUSHOBJECTLIST::iterator itrBrush = GetBrushObjects()->begin();
	advance(itrBrush, m_TerrainToolPal.m_iBrushShape);

	// start with null brush
	BRUSHPARAMS brushParams;
	brushParams.m_pPosition = &m_vecBrushPosition;
	brushParams.m_iBrushDataSize = 0;
	brushParams.m_pBrushData = NULL;

	// if we aren't active, no brush
	TOOLBARACTIVESTRUCT pToolBarStruct;
	pToolBarStruct.cBar = &m_TerrainToolPal;
	pToolBarStruct.bActive = FALSE;
	static DWORD msgHash_IsToolBarActive = CHashString(_T("IsToolBarActive")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_IsToolBarActive, sizeof(pToolBarStruct), &pToolBarStruct );
	if( pToolBarStruct.bActive == FALSE )
		return brushParams;

	// if select tool, no brush
	if (m_TerrainToolPal.m_eTerrainTool == kTerrainToolSelect)
		return brushParams;

	if (itrBrush != GetBrushObjects()->end())
	{
		brushParams.m_iBrushDataSize = itrBrush->m_iSize;
		brushParams.m_pBrushData = itrBrush->m_pData;
	}

	if (m_TerrainToolPal.m_eTerrainTool == kTerrainToolPaint)
    	brushParams.m_fStrength = m_TerrainToolPal.m_fBrushStrength / 100.0f;
	else if (m_TerrainToolPal.m_eTerrainTool == kTerrainToolDeform && m_TerrainToolPal.m_iBrushOp == BRUSH_OP_SMOOTH)
    	brushParams.m_fStrength = m_TerrainToolPal.m_fBrushStrength / 100.0f;
	else
    	brushParams.m_fStrength = m_TerrainToolPal.m_fBrushStrength;

	if (m_TerrainToolPal.GetModifyDown())
		brushParams.m_fStrength *= -1;

	brushParams.m_fSize = m_TerrainToolPal.m_fBrushSize;

	brushParams.m_eBrushOp = (BRUSH_OP)m_TerrainToolPal.m_iBrushOp;
	brushParams.m_bMaxCap = m_TerrainToolPal.m_bMaxCap != 0;
	brushParams.m_fMaxCap = m_TerrainToolPal.m_fMaxHeight;
	brushParams.m_bMinCap = m_TerrainToolPal.m_bMinCap != 0;
	brushParams.m_fMinCap = m_TerrainToolPal.m_fMinHeight;
	return brushParams;
}

///	\brief	get name of render object for terrain sector
///	\param	pointer to IHashString with name of the sector
///	\return	hash string with name of sector's child object with type
///			CTerrainSectorRenderObject
CHashString CTerrainEditor::GetSectorRenderObject(IHashString *pSectorName)
{
	static CHashString hszCTerrainSectorRenderObject(_T("CTerrainSectorRenderObject"));
	return GetEEObjectChildName(pSectorName, &hszCTerrainSectorRenderObject);
}

///	\brief	get heightmap path from terrain sector name
///	\param	pSectorName - pointer to hash string with sector name
///	\return	path with heightmap
CHashString CTerrainEditor::GetHeightmapPath(IHashString *pSectorName) const
{
	StdString szHeightmapName;
	IArchive *pArchive = GetEEObjectProperties(pSectorName);
	if (pArchive != NULL)
	{
		pArchive->SetIsWriting(false);
		pArchive->SeekTo(0);
		// read from archive
		int iSectorX, iSectorY;
		pArchive->Read(iSectorX); // sector x
		pArchive->Read(iSectorY); // sector y
		pArchive->Read(szHeightmapName); // heightmap name
		pArchive->Close();
	}
	return szHeightmapName.c_str();
}

///	\brief	get mask texture list from terrain render object by sector name
///	\param	pSectorName - pointer to hash string with sector name
///	\return	list with mask texture names
TERRAINLAYERMASKVEC CTerrainEditor::GetMaskTexturePaths(IHashString *pSectorName) const
{
	TERRAINLAYERVEC layerList;
	TERRAINLAYERMASKVEC layerMaskList;
	TERRAINLAYERINFOPARAMS infoParams;

	// now save off paint maps, get information from the sector
	infoParams.pTerrainLayerList = &layerList;
	infoParams.pTerrainLayerMaskList = &layerMaskList;
	static DWORD msgHash_TerrainLayerInfo = CHashString(_T("TerrainLayerInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_TerrainLayerInfo, sizeof(infoParams), &infoParams, pSectorName);
	return layerMaskList;
}

void CTerrainEditor::DoAllSectorsUndo(bool undo)
{
	// finding TerrainEntity object
	static CHashString hszWorld = _T("World");
	static CHashString hszCTerrainConfig = _T("CTerrainConfig");
	CHashString hszTerrainConfig = GetEEObjectChildName(&hszWorld, &hszCTerrainConfig);
	if (hszTerrainConfig.IsEmpty())
	{
		// terrain config was not created, so nothing to save
		return;
	}
	// parent of the terrain config is a terrain entity. We need to save children for this entity
	CHashString hszTerrainEntity = GetComponentParent(&hszTerrainConfig);

	if (undo)
	{
		// this function saves entity children for restore
		UndoForDeleteObject(&hszTerrainEntity);
	}
	else
	{
		// but children traversing is still needed for redo action
		vector<IObject *> children;
		GetEEObjectChildren(&hszTerrainEntity, children);
		for (vector<IObject *>::iterator it = children.begin(); it != children.end(); ++it)
		{
			IObject *pObject = *it;
			IHashString *pName = pObject->GetName();
			IHashString *pType = pObject->GetComponentType();
			const DWORD hashType = pType->GetUniqueID();
			IHashString *pParent = pObject->GetParentName();
			UndoForCreateObject(pParent, pName, pType);
		}
	}
}