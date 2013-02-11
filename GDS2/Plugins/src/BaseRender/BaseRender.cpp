// BaseRender.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


AFX_EXTENSION_MODULE BaseRenderDLL = { NULL, NULL };
CBaseRender *gBaseRender;
CLogger *gLogger;

// User-defined predicate function for sorting.
// this has to be defined locally or compiler put's it in whoever
// includes the header file.  Make sure both functions match.
struct lessThan : public greater<CDLLInterface *>
{
    bool operator()(const CDLLInterface* &x, const CDLLInterface* &y) const
    { 
		return (x->m_Priority < y->m_Priority); 
    };

};

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BASERENDER.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BaseRenderDLL, hInstance))
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

		new CDynLinkLibrary(BaseRenderDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BASERENDER.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(BaseRenderDLL);
		delete gBaseRender;
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
	gBaseRender = new CBaseRender(pi);
	return gBaseRender;
}

//---------------------------------------------------------------------
// Function:	CBaseRender
// Description:	Standard Constructor
// Parameters:	pi = pointer to program's information structure
// Returns:		.
//---------------------------------------------------------------------
CBaseRender::CBaseRender(CProgramInfo *ProgramInfo)
{
	m_ProgramInfo = ProgramInfo;

	m_LoadStrings = NULL;
	m_SaveStrings = NULL;

    m_Priority = IDR_BASERENDERTYPE;
    m_DocTemplate = NULL;
    gLogger = NULL;
}

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CBaseRender::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(BaseRenderDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	Initialization function for DLL, like InitInstance
// Parameters:	.
// Returns:		TRUE, if initialized successfully
//---------------------------------------------------------------------
BOOL CBaseRender::Initialize()
{
	CString tName;
	HINSTANCE appInst;
	CMDIFrameWnd *mainWnd;

	CLogger::Initialize();

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
    if (pApp == NULL)
    {
        return FALSE;
    }

	mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(BaseRenderDLL.hModule);

	// use my string / icon resources
	// *** note IDR_TEXTURE type needs to be unique ID even between
	// dlls

	m_DocTemplate = new CMultiDocTemplate(IDR_BASERENDERTYPE,
			RUNTIME_CLASS(CRenderDoc),
			RUNTIME_CLASS(CRenderFrm),
			RUNTIME_CLASS(CRenderView));

	pApp->AddDocTemplate(m_DocTemplate);

    // load the rendering DLLS available
    LoadRenderDLLS();
    // now build a list of render DLLS into the Render Menu

	VERIFY(MergeMenus(&m_DocTemplate->m_hMenuShared, *(m_ProgramInfo->curMenu), _T("&Window")));	
	VERIFY(MergeAccelerators(&m_DocTemplate->m_hAccelTable, *(m_ProgramInfo->curAccel)));
   
	AfxSetResourceHandle(appInst);

    return TRUE;
}

//---------------------------------------------------------------------
// Function:	OnCreate
// Description:	Called from the main windows create function
// Parameters:	mainWnd = main frames CMDIFrame pointer
// Returns:		.
//---------------------------------------------------------------------
void CBaseRender::OnCreate(CMDIFrameWnd *mainWnd)
{
    CRenderDoc *pDoc;
    CRenderView *pView;
    POSITION pos;
    STDDLLLIST::iterator itDLL;
    CRenderDLLInterface *rDLL;

    // might as well create a rendering window
    pDoc = DYNAMIC_DOWNCAST(CRenderDoc, m_DocTemplate->OpenDocumentFile(NULL));
    
    if (pDoc == NULL)
    {
        gLogger->Print(_T("Unable to get RenderDoc pointer"));
        return;
    }

    // only 1 view per document in this case
    pos = pDoc->GetFirstViewPosition();
    pView = DYNAMIC_DOWNCAST(CRenderView, pDoc->GetNextView(pos));
    ASSERT_VALID(pView);

    // and initialize the first render, or last renderer selected
    itDLL = m_RenderDLLS.begin();
    if (itDLL != m_RenderDLLS.end())
    {
        // it's really a CRenderDLLInterface
        rDLL = (CRenderDLLInterface *)(*itDLL);
        ASSERT(rDLL != NULL);

        // make sure DLL is up and running
        rDLL->Initialize();

        // now set the renderer for the view, which initializes it
        pView->SetRenderer(rDLL->GetRenderer(), rDLL->GetAPIName());

        // save the active renderer
        m_ActiveRenderer = pView;

    }
}

//---------------------------------------------------------------------
// Function:	OnCmdMsg
// Description:	Called from the main windows OnCmdMsg function.
//				Routes commands through doc/view architecture
// Returns:		TRUE if handled
//---------------------------------------------------------------------
BOOL CBaseRender::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
    
    return FALSE;

}

//---------------------------------------------------------------------
// Function:	~CBaseRender
// Description:	Standard Desctructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CBaseRender::~CBaseRender()
{
    HANDLELIST::iterator itHandle;

    for (itHandle = m_RenderDLLHandles.begin(); itHandle != 
                m_RenderDLLHandles.end(); itHandle++)
	{
		AfxFreeLibrary(*itHandle);
	}

    if (gLogger != NULL)
    {
        gLogger->Release();
        gLogger = NULL;
    }
}

//---------------------------------------------------------------------
// Function:	GetALLInterfaces
// Description:	This function gets the last loaded DLL information and
//              retrieves a DLL interface.  It then adds it to the
//              list of all DLLS loaded
// Parameters:	pathName = name of DLL Loaded.
//              type = the DLL type loaded, i.e. Base, Object, etc.
// Returns:		.
//---------------------------------------------------------------------
void CBaseRender::GetALLInterfaces(const CString& pathName, DLLTYPE type)
{
	HINSTANCE dllHandle;
	GETDLLVERSION GetDLLVersion;
	GETDLLINTERFACE GetDLLInterface;
	CString Mess;

	DWORD version;
	CDLLInterface *ri;
	
	// try loading the dll here
	dllHandle = AfxLoadLibrary(pathName);
	if (dllHandle == NULL)
	{
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf, 0, NULL);
		// Display the string.
        gLogger->Print(_T("Error Loading DLL:%s - %s"), pathName, lpMsgBuf);
		// Free the buffer.
		LocalFree( lpMsgBuf );
		return;
	}

	// make sure this dll is ok
	GetDLLVersion = (GETDLLVERSION)GetProcAddress(dllHandle, _T("GetDLLVersion"));
	GetDLLInterface = (GETDLLINTERFACE)GetProcAddress(dllHandle, _T("GetDLLInterface"));
	if ((GetDLLVersion == NULL) || (GetDLLInterface == NULL))
	{
        gLogger->Print(_T("Error Loading DLL:%s "
			"Unable to locate either init or version function.\n"), pathName);
		AfxFreeLibrary(dllHandle);
		return;
	}

	version = (*GetDLLVersion)();
	if (version != INTERFACEDLLVERSION)
	{
        gLogger->Print(_T("Error Loading DLL:%s "
			"version %x doesn't match expected %x.\n"), pathName, version, INTERFACEDLLVERSION);
		FreeLibrary(dllHandle);
		return;
	}

	ri = GetDLLInterface(m_ProgramInfo);
	if (ri == NULL)
	{
        gLogger->Print(_T("Error Loading DLL:%s "
			"GetDLLInterface routine failed.\n"), pathName);
		AfxFreeLibrary(dllHandle);
		return;
	}

    ri->m_Type = type;
	m_RenderDLLHandles.push_back(dllHandle);
	m_RenderDLLS.push_back(ri);
}

//---------------------------------------------------------------------
// Function:	InitRenderDLL
// Description:	This function calls all the initialize functions, now
//              that the DLLS are sorted
// Parameters:	name = name of DLL to initilialize
// Returns:		.
//---------------------------------------------------------------------
void CBaseRender::InitRenderDLL(CString *name)
{
    STDDLLLIST::iterator itDLL;
    CRenderDLLInterface *pDLL;

    for (itDLL = m_RenderDLLS.begin(); itDLL != m_RenderDLLS.end(); itDLL++)
    {
        pDLL = (CRenderDLLInterface *)(*itDLL);
        if (!name->Compare(*(pDLL->GetAPIName())))
        {
            if (!pDLL->Initialize())
            {
                gLogger->Print(_T("%s DLL failed to initialized."),
                    (LPCSTR)pDLL->GetName());
            }
        }
    }
}

//---------------------------------------------------------------------
// Function:	LoadRenderDLLS
// Description:	This functions searchs and loads all plug-ins
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
void CBaseRender::LoadRenderDLLS()
{
	// for now hardcode path
	SearchDLLS(CString(_T(".\\PlugIns\\*.dlr")), DLLTYPE_RENDERER);
	m_RenderDLLS.sort(lessThan());

}

//---------------------------------------------------------------------
// Function:	SearchDLLS
// Description:	Search for DLLS in a specified directory with
//				recursion capability
// Parameters:	path = initial path to search
//              type = type to store with CDLLInterface
// Returns:		.
//---------------------------------------------------------------------
void CBaseRender::SearchDLLS(const CString& pathName, DLLTYPE type)
{
	CString path;
	WIN32_FIND_DATA fData;
	HANDLE hFile;
	BOOL done;
	int wildIndex;
	LPTSTR oldPath;
	DWORD oldPathSize;
	CString wildcard;

	wildIndex = pathName.ReverseFind('\\');
	wildcard = pathName.Mid(wildIndex+1);
	path = pathName.Left(wildIndex+1);

	// change to the path so dlls can load dlls directory
	oldPathSize = GetCurrentDirectory(0, NULL);
	oldPath = (LPTSTR)malloc(oldPathSize+1);
	GetCurrentDirectory(oldPathSize, oldPath);

	SetCurrentDirectory(path);

	hFile = FindFirstFile(wildcard, &fData);
	done = (hFile == INVALID_HANDLE_VALUE);

	while (!done)
	{ 
		GetALLInterfaces(fData.cFileName, type);
 
	    if (!FindNextFile(hFile, &fData))
		{
	
        done = TRUE;
	    }
	}

	if (hFile != INVALID_HANDLE_VALUE)
	{
		VERIFY(FindClose(hFile));
	}

	SetCurrentDirectory(oldPath);
	free(oldPath);

} 

//---------------------------------------------------------------------
// Function:	OnIdle
// Description:	Called when windows gives us a timeslice to operate
//                  in
// Parameters:	lCount = count of OnIdle calls between messages
// Returns:		non-zero to receive more idle time processing
//---------------------------------------------------------------------
BOOL CBaseRender::OnIdle(LONG lCount)
{
	return m_ActiveRenderer->OnIdle(lCount);
}

