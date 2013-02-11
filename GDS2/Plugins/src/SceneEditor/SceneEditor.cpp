 //=====================================================================
// File:		SceneEditor.cpp
// Description: Defines the initialization routines for the DLL.
// Started:		10/31/2001
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

#define SCENE_FILE_DESCRIPTION _T("Scene")

AFX_EXTENSION_MODULE SceneEditorDLL = { NULL, NULL };

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SceneEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SceneEditorDLL, hInstance))
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

		new CDynLinkLibrary(SceneEditorDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SceneEditor.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SceneEditorDLL);
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
	return SINGLETONINSTANCE(CSceneEditor);
}

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CSceneEditor::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(SceneEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	GetPriority
// Description:	DLL's priority
// Parameters:	.
// Returns:		DWORD = priority of DLL loaded
//---------------------------------------------------------------------
DWORD CSceneEditor::GetPriority()
{
    return IDR_SCENEDLTYPE;
}

//---------------------------------------------------------------------
// Function:	CSceneEditor
// Description:	Standard Constructor
// Parameters:	pi = program info
// Returns:		.
//---------------------------------------------------------------------
CSceneEditor::CSceneEditor()
{
	m_hMenu = NULL;
	m_hAccel = NULL;
 	m_pCameraDockWin = NULL;

	m_ToolBox = EngineGetToolBox();
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	Initialization function for DLL, like InitInstance
// Parameters:	.
// Returns:		TRUE, if initialized successfully
//---------------------------------------------------------------------
BOOL CSceneEditor::Initialize()
{
	// Set physics to force move everything
	bool setForceMove = true;
	CHashString hszPhysManager( _T("CPhysicsManager") );
	static DWORD msgHash_SetForceMove = CHashString(_T("SetForceMoveActors")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_SetForceMove, sizeof(bool), &setForceMove, NULL, NULL );

    return TRUE;
}

//---------------------------------------------------------------------
// Function:	OnCreate
// Description:	Called from the main windows create function
// Parameters:	mainWnd = main frames CMDIFrameWnd pointer
// Returns:		.
//---------------------------------------------------------------------
void CSceneEditor::OnCreate(CMDIFrameWnd *mainWnd)
{
	CRect winRect;
	HMENU tMenu;
	HACCEL tAccel;
	MENUINFOMESSAGE mim;

	static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	// merge our menu/accelerator into the main menu
	m_hMenu = LoadMenu(SceneEditorDLL.hModule, MAKEINTRESOURCE(IDR_SCENEDLTYPE));
	m_hAccel = LoadAccelerators(SceneEditorDLL.hModule, MAKEINTRESOURCE(IDR_SCENEDLTYPE));
	ASSERT(m_hMenu != NULL);
	ASSERT(m_hAccel != NULL);

	VERIFY(MergeSubMenus(&m_hMenu, mim.m_hMenu));
	VERIFY(MergeAccelerators(&m_hAccel, mim.m_hAccel));

	tMenu = LoadMenu(SceneEditorDLL.hModule, MAKEINTRESOURCE(IDR_SCENEDLTYPE_ADD));
	tAccel = LoadAccelerators(SceneEditorDLL.hModule, MAKEINTRESOURCE(IDR_SCENEDLTYPE_ADD));
	ASSERT(tMenu != NULL);
	ASSERT(tAccel != NULL);

	VERIFY(MergeMenus(&tMenu, m_hMenu, _T("&Window")));
	VERIFY(MergeAccelerators(&tAccel, m_hAccel));

	mim.m_hMenu = tMenu;
	mim.m_hAccel = tAccel;
	mim.m_MenuRsrc = NULL;
	mim.m_MenuBitmap = NULL;

	static DWORD msgHash_SetMenuInfo = CHashString(_T("SetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	if (!m_SceneDLDockWin.Create("", WS_CHILD | WS_VISIBLE, mainWnd, 
		IDD_SCENEDL_DLG))
	{
        m_ToolBox->Log(LOGERROR, _T("Failed to create scene dockable window bar\n"));
        return;
    }

	mainWnd->GetWindowRect(&winRect);
	m_SceneDLDockWin.EnableDocking(CBRS_ALIGN_ANY);
	m_SceneDLDockWin.SetBarStyle(m_SceneDLDockWin.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	mainWnd->DockControlBar(&m_SceneDLDockWin, AFX_IDW_DOCKBAR_LEFT, winRect);

	//register our filetype
	FILETYPECREATIONINFO fileInfo;
	fileInfo.m_Description = SCENE_FILE_DESCRIPTION;
	fileInfo.m_CreationCallback = CSceneEditor::CreateScene;
	static DWORD msgHash_RegisterFileTypeCreationInfo = CHashString(_T("RegisterFileTypeCreationInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterFileTypeCreationInfo, sizeof( FILETYPECREATIONINFO ),
			&fileInfo, NULL, NULL );

	// Dock Win
	m_pCameraDockWin = new CCameraDockWin();
	if (m_pCameraDockWin->Create(_T("Camera"), WS_CHILD | WS_VISIBLE, mainWnd, IDD_CAMERADLG))
	{
		CRect winRect;
		mainWnd->GetWindowRect(&winRect);
		m_pCameraDockWin->EnableDocking(CBRS_ALIGN_ANY);
		m_pCameraDockWin->SetBarStyle(m_pCameraDockWin->GetBarStyle() |
			CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		mainWnd->DockControlBar(m_pCameraDockWin, AFX_IDW_DOCKBAR_RIGHT, winRect);
		m_pCameraDockWin->LockPushPin(FALSE);
	}
}

//---------------------------------------------------------------------
// Function:	OnCmdMsg
// Description:	Called from the main windows OnCmdMsg function.
//				Routes commands through doc/view architecture
// Returns:		TRUE if handled
//---------------------------------------------------------------------
BOOL CSceneEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// pass to camera window
	if ( m_pCameraDockWin != NULL )
	{
		if ( m_pCameraDockWin->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) )
		{
			return TRUE;
		}
	}

	return m_SceneDLDockWin.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

#if 0
//---------------------------------------------------------------------
// Function:	Save
// Description:	Save data to a file
// Parameters:	filename = name of file to export
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CSceneEditor::Save(LPTSTR filename)
{
	return NO_ERROR;
}

//---------------------------------------------------------------------
// Function:	Load
// Description:	Load data from a file
// Parameters:	filename = name of file to export
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CSceneEditor::Load(LPTSTR filename)
{
	return m_SceneDLDockWin.OpenScene(filename);
}

#endif

//---------------------------------------------------------------------
// Function:	~CSceneEditor
// Description:	Standard Destructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CSceneEditor::~CSceneEditor()
{
	if (m_hMenu != NULL) 
	{
		DestroyMenu(m_hMenu);
		m_hMenu = NULL;
	}

	if (m_hAccel != NULL)
	{
		DestroyAcceleratorTable(m_hAccel);
		m_hAccel = NULL;
	}

	if ( m_pCameraDockWin != NULL )
	{
		delete m_pCameraDockWin;
		m_pCameraDockWin = NULL;
	}
}

void CSceneEditor::CreateScene(void *userData)	
{
	CSceneComponent * pComponent = SINGLETONINSTANCE(CSceneComponent);
	static DWORD msgHash_CreateBlankScene = CHashString(_T("CreateBlankScene")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_CreateBlankScene, NULL, NULL, NULL, pComponent->GetComponentType() );
	//m_SceneDLDockWin.CreateScene();
}
