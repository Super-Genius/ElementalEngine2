//=====================================================================
// File:		DX8Render.cpp
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

AFX_EXTENSION_MODULE DX8RenderDLL = { NULL, NULL };
CDX8Render *gDX8Render;

CLogger *gLogger;

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("DX8Render.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(DX8RenderDLL, hInstance))
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

		new CDynLinkLibrary(DX8RenderDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("DX8Render.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(DX8RenderDLL);
		delete gDX8Render;
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
	gDX8Render = new CDX8Render(pi);
	return gDX8Render;
}

//---------------------------------------------------------------------
// Function:	GetPriority
// Description:	DLL's priority
// Parameters:	.
// Returns:		DWORD = priority of DLL loaded
//---------------------------------------------------------------------
DWORD CDX8Render::GetPriority()
{
    return IDR_DX8RENDERTYPE;
}

//---------------------------------------------------------------------
// Function:	CDX8Render
// Description:	Standard Constructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CDX8Render::CDX8Render(CProgramInfo *ProgramInfo)
{
	m_ProgramInfo = ProgramInfo;
    m_Renderer = NULL;
    gLogger = NULL;
}

//---------------------------------------------------------------------
// Function:	~CDX8Render
// Description:	Standard Desctructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CDX8Render::~CDX8Render()
{
    if (m_Renderer != NULL)
    {
        delete m_Renderer;
        m_Renderer = NULL;
    }

    if (gLogger != NULL)
    {
        gLogger->Release();
        gLogger = NULL;
    }

}

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CDX8Render::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(DX8RenderDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	GetAPIName 
// Description:	DLL's API supported name
// Parameters:	.
// Returns:		CString * = pointer to string for API Supported
//---------------------------------------------------------------------
CString *CDX8Render::GetAPIName()
{
    static CString APIName(_T("DX8 Renderer"));
    return &APIName;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	Initialization function
// Parameters:	.
// Returns:		TRUE, if successful
//---------------------------------------------------------------------
BOOL CDX8Render::Initialize()
{
	CString tName;
	HINSTANCE appInst;
	CMDIFrameWnd *mainWnd;

	// Register the doc templates we provide to the app
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
    if (pApp == NULL)
    {
        return FALSE;
    }
	mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(DX8RenderDLL.hModule);

	CLogger::Initialize();

	gLogger = CLogger::Attach();
	if( gLogger == NULL )
	{
		TRACE0("Could not attach to CLogger obejct!\n");
        return FALSE;
	}
    else
    {
        gLogger->SetFormat( LOG_PROCNAME | LOG_TIMESTAMP | LOG_THREADID );
        gLogger->SetName((LPTSTR)(LPCSTR)*GetName());
    }


    // create the rendering core
    m_Renderer = new CDX8RenderCore(m_ProgramInfo->ObjectManager);
    if (m_Renderer == NULL)
    {
        // add logging information.
        gLogger->Print(_T("Unable to create DX8 Renderer"));
        return FALSE;
    }

	AfxSetResourceHandle(appInst);
    
    return TRUE;

}

//---------------------------------------------------------------------
// Function:	OnCmdMsg
// Description:	Called from the main windows OnCmdMsg function.
//				Routes commands through doc/view architecture
// Returns:		TRUE if handled
//---------------------------------------------------------------------
BOOL CDX8Render::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return FALSE;
}

//---------------------------------------------------------------------
// Function:	OnIdle
// Description:	Application calls this function when application is
//				idle or allows us to do some processing
// Returns:		TRUE if more processing needed
//---------------------------------------------------------------------
BOOL CDX8Render::OnIdle(LONG lCount)
{
    return FALSE;
}

//---------------------------------------------------------------------
// Function:	~CDX8Render
// Description:	Standard Desctructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CRender *CDX8Render::GetRenderer()
{
    return m_Renderer;
}

