 //=====================================================================
// File:		BaseSceneDL.cpp
// Description: Defines the initialization routines for the DLL.
// Started:		10/31/2001
// Original:	Kenneth Hurley
//=====================================================================

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AFX_EXTENSION_MODULE BaseSceneDLDLL = { NULL, NULL };
CBaseSceneDL *gBaseSceneDL = NULL;
CLogger *gLogger;

static LPTSTR OpenFilter = {
	"Scene Files (*.sfs)\0" "*.sfs\0"
	"\0\0"
};

static LPTSTR SaveFilter = {
	"SceneDL Files (*.sfs)\0" "*.sfs\0"
	"\0\0"
};

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BaseSceneDL.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BaseSceneDLDLL, hInstance))
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

		new CDynLinkLibrary(BaseSceneDLDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BaseSceneDL.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BaseSceneDLDLL);
		delete gBaseSceneDL;
		gBaseSceneDL = NULL;
	}
	return 1;   // ok
}

extern "C" DWORD GetDLLVersion()
{
	return INTERFACEDLLVERSION;
}

// Exported DLL initialization is run in context of running application
extern "C" CDLLInterface *GetDLLInterface(CProgramInfo *pi)
{
	// this has to be done after module is intialized.
	gBaseSceneDL = new CBaseSceneDL(pi);
	return gBaseSceneDL;
}

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CBaseSceneDL::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(BaseSceneDLDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	CBaseSceneDL
// Description:	Standard Constructor
// Parameters:	pi = program info
// Returns:		.
//---------------------------------------------------------------------
CBaseSceneDL::CBaseSceneDL(CProgramInfo *programInfo)
{
	m_ProgramInfo = programInfo;

	m_hMenu = NULL;
	m_hAccel = NULL;

	m_LoadStrings = OpenFilter;
	m_SaveStrings = SaveFilter;

    m_Priority = IDR_SCENEDLTYPE;

    gLogger = NULL;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	Initialization function for DLL, like InitInstance
// Parameters:	.
// Returns:		TRUE, if initialized successfully
//---------------------------------------------------------------------
BOOL CBaseSceneDL::Initialize()
{
	CString tName;
	HINSTANCE appInst;
	CMDIFrameWnd *mainWnd;
	HMENU tMenu;
	HACCEL tAccel;

	gLogger = CLogger::Attach();
	if( gLogger == NULL )
	{
		TRACE0("Could not attach to CLogger object!\n");
        return FALSE;
	}
    else
    {
        gLogger->SetFormat( LOG_PROCNAME | LOG_TIMESTAMP | LOG_THREADID );
        gLogger->SetName((LPTSTR)(LPCSTR)*GetName());
    }

	// Register the doc templates we provide to the app
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(BaseSceneDLDLL.hModule);

	m_hMenu = LoadMenu(BaseSceneDLDLL.hModule, MAKEINTRESOURCE(IDR_SCENEDLTYPE));
	m_hAccel = LoadAccelerators(BaseSceneDLDLL.hModule, MAKEINTRESOURCE(IDR_SCENEDLTYPE));
	ASSERT(m_hMenu != NULL);
	ASSERT(m_hAccel != NULL);
	VERIFY(MergeSubMenus(&m_hMenu, *(m_ProgramInfo->curMenu)));
	VERIFY(MergeAccelerators(&m_hAccel, *(m_ProgramInfo->curAccel)));

	tMenu = LoadMenu(BaseSceneDLDLL.hModule, MAKEINTRESOURCE(IDR_SCENEDLTYPE_ADD));
	tAccel = LoadAccelerators(BaseSceneDLDLL.hModule, MAKEINTRESOURCE(IDR_SCENEDLTYPE_ADD));

	ASSERT(tMenu != NULL);
	ASSERT(tAccel != NULL);

	VERIFY(MergeMenus(&tMenu, m_hMenu, _T("&Window")));
	VERIFY(MergeAccelerators(&tAccel, m_hAccel));

	*(m_ProgramInfo->curMenu) = tMenu;
	*(m_ProgramInfo->curAccel) = tAccel;

	AfxSetResourceHandle(appInst);
	
    return TRUE;
}

//---------------------------------------------------------------------
// Function:	OnCreate
// Description:	Called from the main windows create function
// Parameters:	mainWnd = main frames CMDIFrame pointer
// Returns:		.
//---------------------------------------------------------------------
void CBaseSceneDL::OnCreate(CMDIFrameWnd *mainWnd)
{
	CRect winRect;
	if (!m_SceneDLDockWin.Create("", WS_CHILD | WS_VISIBLE | WS_VISIBLE, mainWnd, 
		IDD_SCENEDL_DLG))
	{
        gLogger->Print(_T("Failed to create scene dockable window bar"));
        return;
    }

	mainWnd->GetWindowRect(&winRect);
	m_SceneDLDockWin.EnableDocking(CBRS_ALIGN_ANY);
	m_SceneDLDockWin.SetBarStyle(m_SceneDLDockWin.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	mainWnd->DockControlBar(&m_SceneDLDockWin, AFX_IDW_DOCKBAR_LEFT, winRect);

}

//---------------------------------------------------------------------
// Function:	OnCmdMsg
// Description:	Called from the main windows OnCmdMsg function.
//				Routes commands through doc/view architecture
// Returns:		TRUE if handled
//---------------------------------------------------------------------
BOOL CBaseSceneDL::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{

	return m_SceneDLDockWin.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

}

//---------------------------------------------------------------------
// Function:	Save
// Description:	Save data to a file
// Parameters:	filename = name of file to export
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CBaseSceneDL::Save(LPTSTR filename)
{
	return NO_ERROR;
}

//---------------------------------------------------------------------
// Function:	Load
// Description:	Load data from a file
// Parameters:	filename = name of file to export
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CBaseSceneDL::Load(LPTSTR filename)
{
	return m_SceneDLDockWin.OpenScene(filename);
}

//---------------------------------------------------------------------
// Function:	~CBaseSceneDL
// Description:	Standard Desctructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CBaseSceneDL::~CBaseSceneDL()
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

    if (gLogger != NULL)
    {
        gLogger->Release();
        gLogger = NULL;
    }

}

