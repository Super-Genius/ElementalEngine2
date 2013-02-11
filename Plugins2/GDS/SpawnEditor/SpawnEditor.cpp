//=====================================================================
// File:		SpawnEditor.cpp
// Description: Spawn Editor implementation
// Date:		08-15-2005
// Author:		Josh Lulewicz
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

#define SPAWNER_EXTENSIONS "sxl"

AFX_EXTENSION_MODULE SpawnEditorDLL = { false, NULL, NULL };

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SpawnEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SpawnEditorDLL, hInstance))
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

		new CDynLinkLibrary(SpawnEditorDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SpawnEditor.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SpawnEditorDLL);
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
	return SINGLETONINSTANCE(CSpawnEditor);
}

static LPCTSTR OpenFilter = {
	"Spawn Files (*.sxl)\0" "*.sxl\0"
	"\0\0"
};

static LPCTSTR SaveFilter = {
	"Spawn Files (*.sxl)\0" "*.sxl\0"
	"\0\0"
};

//---------------------------------------------------------------------
// Function:	GetPriority
// Description:	DLL's priority
// Parameters:	.
// Returns:		DWORD = priority of DLL loaded
//---------------------------------------------------------------------
DWORD CSpawnEditor::GetPriority()
{
    return IDR_SPAWNTYPE;
}

//---------------------------------------------------------------------
// Function:	CSpawnEditor
// Description:	Standard Constructor
// Parameters:	pi = programs information structure
// Returns:		.
//---------------------------------------------------------------------
CSpawnEditor::CSpawnEditor()
{
	SINGLETONINSTANCE(CSpawnComponent)->SetParent(this);
	m_ToolBox = EngineGetToolBox();
}

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CSpawnEditor::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(SpawnEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	DLL's Initialization function, like InitInstance
// Parameters:	.
// Returns:		TRUE, if successful.
//---------------------------------------------------------------------
BOOL CSpawnEditor::Initialize()
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
	AfxSetResourceHandle(SpawnEditorDLL.hModule);

	// use my string / icon resources
	// Note IDR_EXAMPLETYPE has to be a unique id even between DLLS
	// to get the ICONS right on the Window
	m_DocTemplate = new CMultiDocTemplate(IDR_SPAWNTYPE,
			RUNTIME_CLASS(CSpawnDoc),
			RUNTIME_CLASS(CSpawnFrm),
			RUNTIME_CLASS(CSpawnView));

	// put this after setting the menu and accelerators so it won't allocate it's own
	pApp->AddDocTemplate(m_DocTemplate);

	// construct resource info
	m_ObjCollection.name.LoadString(IDS_RESOURCE_NAME);
	m_ObjCollection.pageIndex = &m_PageIndex;
	m_ObjCollection.iView = LVS_LIST;

	m_ObjCollection.hIcon = (HICON)LoadImage(SpawnEditorDLL.hModule, MAKEINTRESOURCE(IDR_SPAWNTYPE), 
		IMAGE_ICON, 0, 0, LR_CREATEDIBSECTION);
	VERIFY(m_ObjCollection.hIcon != NULL);

	// value should be 1 - 1000
	m_ObjCollection.priority = IDR_SPAWNTYPE;
	m_ObjCollection.extensions = SPAWNER_EXTENSIONS;
	AfxSetResourceHandle(appInst);

    return TRUE;
}

//---------------------------------------------------------------------
// Function:	~CSpawnEditor
// Description:	Standard Desctructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CSpawnEditor::~CSpawnEditor()
{
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
void CSpawnEditor::OnCreate(CMDIFrameWnd *mainWnd)
{
	MENUINFOMESSAGE mim;
	CBitmap menuBitmap;
	HINSTANCE appInst;

	static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE),
		&mim);

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(SpawnEditorDLL.hModule);

	// we need to mergeMenus
	VERIFY(MergeMenus(&m_DocTemplate->m_hMenuShared, mim.m_hMenu, _T("&Window")));	
	// and acclerators
	VERIFY(MergeAccelerators(&m_DocTemplate->m_hAccelTable, mim.m_hAccel));

	mim.m_hAccel = NULL;
	mim.m_hMenu = NULL;
	// load up hi-res toolbar icon
	menuBitmap.LoadBitmap(IDB_TOOLBAR);
	// we don't really have to have to toolbar visible do we?
	mim.m_MenuRsrc = IDR_SPAWNTYPE;
	mim.m_MenuBitmap = &menuBitmap;

	static DWORD msgHash_SetMenuInfo = CHashString(_T("SetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetMenuInfo, sizeof(MENUINFOMESSAGE),
		&mim);

	static DWORD msgHash_RegisterResourceCollection = CHashString(_T("RegisterResourceCollection")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterResourceCollection, sizeof(RESOURCECOLLECTION),
		&m_ObjCollection);

	AfxSetResourceHandle(appInst);
}

//---------------------------------------------------------------------
// Function:	OnCmdMsg
// Description:	Called from the main windows OnCmdMsg function.
//				Routes commands through doc/view architecture
// Returns:		TRUE if handled
//---------------------------------------------------------------------
BOOL CSpawnEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return FALSE;
}

//---------------------------------------------------------------------
// Function:	EditDocument
// Description:	Edit an example Document retrieving initial data 
//					from a archive
// Parameters:	ar = archive to load into document
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CSpawnEditor::EditDocument( /* IArchive &ar */)
{
	
	CSpawnDoc *createDoc;
	// create blank document, with frame
	createDoc = DYNAMIC_DOWNCAST(CSpawnDoc, m_DocTemplate->OpenDocumentFile(NULL));

	// now load the document
	createDoc->LoadDocument( /* ar */ );

	return NO_ERROR;
}

bool CSpawnEditor::Spawn(LOADFILEEXTPARAMS *lfep)
{
	CHashString spawnedName(_T(""));
	DWORD blankID = spawnedName.GetUniqueID();

	// create a blank scene if necessary
	static DWORD msgHash_CreateBlankScene = CHashString(_T("CreateBlankScene")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateBlankScene, 0, NULL);

	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), lfep) == MSG_HANDLED)
	{
		// get last entity created
		static DWORD msgHash_GetSpawnedName = CHashString(_T("GetSpawnedName")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetSpawnedName, sizeof(IHashString), &spawnedName);

		if (spawnedName.GetUniqueID() != blankID)
		{
			// set newly spawned object as the currently selected object
			SELECTOBJECTPARAMS sop;
			sop.object = &spawnedName;
			static DWORD msgHash_SelectObject = CHashString(_T("SelectObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SelectObject, sizeof(sop), &sop );

			// move object in front of camera
			if (!MoveObjectInFrontOfCamera(&spawnedName))
			{
				return false;
			}
		}
		else
		{
			// log error?
			return false;
		}
	}
	else
	{
		// log error
		StdString error;
		error = lfep->fileName;
		error += _T(" not found, when attempting to load.\n");
		m_ToolBox->SetErrorValue(WARN_OBJECT_NOT_FOUND);
		m_ToolBox->Log(LOGWARNING, error);
		return false;
	}

	return true;
}

bool CSpawnEditor::SpawnAtPosition(SPAWNATPOSITONPARAMS *sapp)
{
	LOADFILEEXTPARAMS lfep;
	IHashString *name;
	// new position for the object
	Vec3 newPos;
	// name of spawned object
	CHashString spawnedName(_T(""));

	StdString error;
	CString extension;
	// current scaling of the object
	Vec3 currentScale;
	// new scale value
	Vec3 *scale;
	// current orientation of the object
	Vec3 currentRotation;
	// new rotation value
	Vec3 *rotation;

	// grab name
	name = sapp->name;

	// get the extension of the item that is selected
	extension = GetExtension(name->GetString());

	// make sure that the extension is Spawner
	if (extension != ".sxl")
	{
		// message not for the spawn component move on
		return false;
	}

	// setup load file by extension struct
	lfep.fileName = (TCHAR*)name->GetString();
	lfep.bInternalLoad = true;
	
	// if load is successful...
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(lfep), &lfep) == MSG_HANDLED)
	{
		// get last entity created
		static DWORD msgHash_GetSpawnedName = CHashString(_T("GetSpawnedName")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetSpawnedName, sizeof(IHashString), &spawnedName);

		// if we need to scale the spawned object
		if (sapp->scale != NULL)
		{
			// get current scale
			static DWORD msgHash_GetGlobalScale = CHashString(_T("GetGlobalScale")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_GetGlobalScale, sizeof(Vec3), &currentScale, &spawnedName) != MSG_HANDLED)
			{
				return false;
			}

			// grab the scale value from our message
			scale = sapp->scale;

			// multiply current scale value by new scale value
			scale->SetX(currentScale.GetX() * scale->GetX());
			scale->SetY(currentScale.GetY() * scale->GetY());
			scale->SetZ(currentScale.GetZ() * scale->GetZ());

			// clamp values (shouldn't be necessary but just to be safe)
			if( scale->GetX() < 0.00f )
			{
				scale->SetX( 0.00f );
			}
			if( scale->GetY() < 0.00f )
			{
				scale->SetY( 0.00f );
			}
			if( scale->GetZ() < 0.00f )
			{
				scale->SetZ( 0.00f );
			}

			// set scale for the object
			static DWORD msgHash_SetGlobalScale = CHashString(_T("SetGlobalScale")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_SetGlobalScale, sizeof(Vec3), scale, &spawnedName) != MSG_HANDLED)
			{
				return false;
			}
		}

		// if we need to scale the spawned object
		if (sapp->orientation != NULL)
		{
			static DWORD msgHash_GetGlobalEuler = CHashString(_T("GetGlobalEuler")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_GetGlobalEuler, sizeof(Vec3), &currentRotation, &spawnedName) != MSG_HANDLED)
			{
				return false;
			}
			
			// grab the rotation value from our message
			rotation = sapp->orientation;

			// Calculate the new orientation
			rotation->SetX(rotation->GetX() + currentRotation.GetX());
			rotation->SetY(rotation->GetY() + currentRotation.GetY());
			rotation->SetZ(rotation->GetZ() + currentRotation.GetZ());

			// set orientation for the object
			static DWORD msgHash_SetGlobalEuler = CHashString(_T("SetGlobalEuler")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_SetGlobalEuler, sizeof(Vec3), rotation, &spawnedName) != MSG_HANDLED)
			{
				return false;
			}
		}

		// set position of the entity
		newPos.SetX(sapp->x);
		newPos.SetY(sapp->y);		
		newPos.SetZ(sapp->z);
		static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), &newPos, &spawnedName);
	}

	return true;
}

DWORD CSpawnEditor::SpawnFromFileDrop(DROPOBJECTINFO *doi)
{
	LOADFILEEXTPARAMS lfep;
	CString extension;

	// grab resource page info
	RESOURCEPAGEINFO *rpi = doi->pResourcePageInfo;

	// get the extension of the item that was dropped
	extension = GetExtension(rpi->filepath);

	// make sure that the extension is Spawner
	if (extension != ".sxl")
	{
		// message not for the spawn component move on
		return MSG_NOT_HANDLED;
	}

	// send load message to load the spawner
	lfep.fileName = rpi->filepath;
	lfep.bInternalLoad = true;
	// spawn the object
	if (Spawn(&lfep))
	{
		CUndoCommandGuard undoCommand;
		UndoForObjectSpawns();
		return MSG_HANDLED_STOP;
	}

	// failed return error
	return MSG_ERROR;
}

bool CSpawnEditor::MoveObjectInFrontOfCamera(IHashString *objName)
{
	// get active camera name
	CHashString hszCameraName;
	IHashString *pCameraName = NULL;
	CONTEXTCAMPARAMS ccp;
	ccp.pRC = NULL;
	ccp.szCameraName = &hszCameraName;
	static DWORD msgHash_GetActiveCamera = CHashString(_T("GetActiveCamera")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetActiveCamera, sizeof(CONTEXTCAMPARAMS), &ccp ) != MSG_HANDLED)
	{
		// log error
		StdString error;
		error = _T("Camera not found.\n");
		m_ToolBox->SetErrorValue(WARN_OBJECT_NOT_FOUND);
		m_ToolBox->Log(LOGWARNING, error);
		return false; // no camera
	}

    pCameraName = ccp.szCameraName;
	// get active camera name
	Vec3 camOrigin;
	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &camOrigin, pCameraName ) != MSG_HANDLED)
	{
		// log error
		StdString error;
		error = _T("Could not retrieve origin of the active camera.\n");
		m_ToolBox->SetErrorValue(WARN_OBJECT_NOT_FOUND);
		m_ToolBox->Log(LOGWARNING, error);
		return false;
	}

	// get bounding box of object
	Vec3 bboxDim, bboxOrigin;
	//BBOXPARAMS bbox;
	Vec3 pos(0,0,0);
	Vec3 ext(0,0,0);
	EulerAngle rot(0,0,0);
	GETBOUNDINGBOXPARAMS bbox;
	bbox.position = &pos;
	bbox.extents = &ext;
	bbox.rotation = &rot;

	static DWORD msgHash_GetBoundingBox = CHashString(_T("GetBoundingBox")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetBoundingBox, sizeof(bbox), &bbox, objName ) != MSG_HANDLED)
	{
	}

	// get camera basis vectors
	Vec3 vecCamView, vecCamUp, vecCamRight;
	CAMERAVECTORSPARAMS camVectors;
	camVectors.vViewVector = &vecCamView;
	camVectors.vUpVector = &vecCamUp;
	camVectors.vRightVector = &vecCamRight;
	static DWORD msgHash_GetCameraVectors = CHashString(_T("GetCameraVectors")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetCameraVectors, sizeof(camVectors), &camVectors, pCameraName ) != MSG_HANDLED)
	{
		// log error
		StdString error;
		error = _T("Error retrieving camera vectors.\n");
		m_ToolBox->SetErrorValue(WARN_OBJECT_NOT_FOUND);
		m_ToolBox->Log(LOGWARNING, error);
		return false;
	}

	// get the distance that the camera should be from object
	// twice the diameter of a sphere surrounding the bbox seems to be a decent distance
	float fDistance; //= 2.0f * sqrt(	bbox.dimX*bbox.dimX + 
					//							bbox.dimY*bbox.dimY + 
					//							bbox.dimZ*bbox.dimZ);

	bbox.extents->GetX();
	if (bbox.extents->GetZ() > bbox.extents->GetX() && bbox.extents->GetZ() > bbox.extents->GetY())
	{
		fDistance = bbox.extents->GetZ() * 5.0f;
	}
	else if (bbox.extents->GetX() > bbox.extents->GetZ() && bbox.extents->GetX() > bbox.extents->GetY())
	{
		fDistance = bbox.extents->GetX() * 5.0f;
	}
	else 
	{
		fDistance = bbox.extents->GetY() * 5.0f;
	}
	fDistance = max( fDistance, 50.0f );


	// get new object position from camera
	// to find camera position, back off calculated distance along the camera's view vector 
	// from the object's origin
	Vec3 newPos;
	newPos.SetX( camOrigin.GetX() + vecCamView.GetX() * fDistance );
	newPos.SetY( camOrigin.GetY() + vecCamView.GetY() * fDistance );
	newPos.SetZ( camOrigin.GetZ() + vecCamView.GetZ() * fDistance );

	// set object position
	Vec3 vecOriginalPos(0,0,0);
	m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &vecOriginalPos, objName);
	// move the object using the editor's settings
	EDITORMOVEOBJECTPARAMS emop;
	emop.pObjectName = objName;
	emop.vecMove = newPos - vecOriginalPos;;
	emop.bUseMoveAccum = false;
	static DWORD msgHash_EditorMoveObject = CHashString(_T("EditorMoveObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_EditorMoveObject, sizeof(emop), &emop );

	return true;
}

CString CSpawnEditor::GetExtension(const TCHAR *filepath)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath( filepath, drive, dir, fname, ext );
	CString e(ext);
	return CString(e);
}
