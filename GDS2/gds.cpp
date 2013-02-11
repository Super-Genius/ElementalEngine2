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
#include "SplashDialog.h"

#ifdef NVPERF
#include "nv_perfauth.h"
#endif

#include "afxwin.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static TCHAR OpenFileName[2048];
static TCHAR SaveFileName[2048];
static TCHAR *OpenFilter = NULL;
static TCHAR *SaveFilter = NULL;

static TCHAR allFiles[] = _T(("All Files (*.*)\0" "*.*\0"
								"\0\0"));

static OPENFILENAME Ofn = {
	sizeof(OPENFILENAME),
	0,
	NULL,
	OpenFilter,
	NULL,
	0,
	0,
	OpenFileName,
	2048,
	NULL,
	0,
	NULL,
	NULL,
	OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
	0,
	0,
	NULL,
	NULL,
	NULL,
	NULL
};

// User-defined predicate function for sorting.
// this has to be defined locally or compiler put's it in whoever
// includes the header file.  Make sure both functions match.
struct DLLLessThan : public greater<IDLL *>
{
    bool operator()(IDLL* &x, IDLL* &y) const
    { 
		return (x->GetPriority() < y->GetPriority()); 
    };

};


/////////////////////////////////////////////////////////////////////////////
// CGDSApp

BEGIN_MESSAGE_MAP(CGDSApp, CWinApp)
	//{{AFX_MSG_MAP(CGDSApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	// MRU - most recently used file menu
	ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, OnOpenRecentFile)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGDSApp construction

//
#ifdef _DEBUG
bool g_bMemoryBreakAlloc = false;
#endif

// static exception callback
LONG NTAPI QuietExitVectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
	// eesdk function call
	if (pExceptionInfo->ExceptionRecord->ExceptionCode != 0x40010006) // OutputDebugString exception
	{
		HandleException(pExceptionInfo);
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

CGDSApp::CGDSApp()
{
	AddVectoredExceptionHandler( 0, QuietExitVectoredExceptionHandler );

	sprintf(OpenFileName, "*.*");
	Ofn.nFilterIndex = 1;
    m_Logger = NULL;
	// hook into Elemental Engine message system
	SINGLETONINSTANCE(CGDSComponent)->SetParent(this);

	// Set to get a break event when tracing memory leaks
#ifdef _DEBUG
	_CrtSetDbgFlag( _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF );
	if( g_bMemoryBreakAlloc == false )
	{
		g_bMemoryBreakAlloc = true;
//		_CrtSetBreakAlloc(206605);
	}


#endif
}

CGDSApp::~CGDSApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGDSApp object

CGDSApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGDSApp initialization

BOOL CGDSApp::InitInstance()
{
	// change to the path so dlls can load dlls directory
	TCHAR curDir[_MAX_PATH+1];
	GetCurrentDirectory( _MAX_PATH, curDir );
	m_AppPath = curDir;

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if _MSC_VER < 1300				// deprecated
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	// for some reason this has to be called for richedit stuff
	AfxInitRichEdit2();
//	AfxInitRichEdit();

	m_ToolBox = EngineGetToolBox();
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.

	// Enable Duplicate Object Checking
	static DWORD msgHash_FlagDuplicateObjectChecking = CHashString(_T("FlagDuplicateObjectChecking")).GetUniqueID();
	bool DuplicateObjectCheck = true;
	m_ToolBox->SendMessage(msgHash_FlagDuplicateObjectChecking,sizeof(DuplicateObjectCheck),&DuplicateObjectCheck);

	SetRegistryKey(_T("Signature Devices, Inc"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
/*
	// PREVIOUS SOLUTIONS
	// UNDONE: remove resource path stuff and use a listing of paths for plugins
	// and a listing of paths for resources

	//	m_ResourcePath = GetProfileString(_T(""), _T("Base Path"), curDir);	
	//	ReadINIKey(INI_LastProjectPath, szResourcePath, INI_SubSection);
*/
//	TCHAR strPath[_MAX_PATH];
	
	// get the Common Files Path
//	SHGetSpecialFolderPath( 0, strPath, CSIDL_PROGRAM_FILES_COMMON, FALSE );
//	StdString szResourcePath( strPath );
//	szResourcePath += "\\Elemental Engine II";

	StdString szDelay;
	ReadINIKey(INI_ToolbarScrollDelay, szDelay, INI_SubSection, _T("1000"));
	CGuiDockToolBarEx::SetScrollDelay(_ttoi(szDelay));

	// WE DONT USE THE INI KEYS ANY MORE
	// Key Value "ResourceDirectory" in [Graphics Dev System] in 
	// data\\basedata\\init.ini doesn't exist!!!
/*
	if( szResourcePath.empty() )
	{
		BROWSEINFO bi = { 0 };
		bi.lpszTitle = _T("Pick A Resource Directory");

		LPITEMIDLIST pidl = SHBrowseForFolder( &bi );

		if( pidl != 0 )
		{	
			// get the name of the folder
			TCHAR szPathBuffer[MAX_PATH];
			if( SHGetPathFromIDList( pidl, szPathBuffer ) )
			{
				m_ResourcePath = szPathBuffer;
				// write to file				
				WriteINIKey(INI_LastProjectPath, szPathBuffer, INI_SubSection);				
			}
			// else use current working directory
			else
			{
				m_ResourcePath = m_AppPath;
			}

			// free directory selection stuff
			IMalloc* imalloc = 0;
			if( SUCCEEDED( SHGetMalloc( &imalloc ) ) )
			{
				imalloc->Free( pidl );
				imalloc->Release( );
			}
		}
	}
*/
//	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
//	m_ProgramInfo->ObjectManager = new CObjectManager(m_ProgramInfo);

    // enable logger class
	CLogger::Initialize();

	m_Logger = CLogger::Attach();
	if( m_Logger == NULL )
	{
		TRACE0("Could not attach to CLogger object!\n");
	}
    else
    {
        m_Logger->SetFormat( LOG_PROCNAME | LOG_TIMESTAMP | LOG_THREADID );
    }

	m_splashDlg.Create(CSplashDialog::IDD);
	m_splashDlg.EnableOK(false);
	m_splashDlg.SetLoadingText(_T(""));
	m_splashDlg.ShowWindow(SW_SHOW);
	m_splashDlg.RedrawWindow();

	// set the EE paths 
	StdString szAppPath = m_AppPath;
	m_ToolBox->SetDirectories( &szAppPath, NULL );

	// Load in base DLLS into memory -> we need the DLLs first then use them for
	// getting environment values...
	LoadDLLS();

	// get the default resource directory from the environment
	static DWORD msgHash_getDefResPath = CHashString(_T("GetEnvironmentDefaultResourcePath")).GetUniqueID();
	CHashString hszEnvironmentManager(_T("CWin32EnvironmentManager"));
	StdString strDefEnvResourcePath;
	// m_toolbox return value can be checked for error handling
	DWORD retVal = m_ToolBox->SendMessage(msgHash_getDefResPath, sizeof(StdString), &strDefEnvResourcePath, NULL, &hszEnvironmentManager);

	// error check
	if ( retVal != MSG_HANDLED )
	{
		// if we are here: nothing handled our message, so no EnvironmentManager is Present.
		CString envErrorMessage;
		envErrorMessage.Format(_T("Can't find the environment manager. This will cause other errors,  (Is the 'Working directory' set for the GDS project?)") );
		MessageBox(NULL, envErrorMessage,  _T( "Environment Error!" ), MB_OK | MB_ICONERROR );

		envErrorMessage.Format(_T("GDS should have a resource path.\nThis message already indicates an error: the environment manager can't be reached to get the resouce path on your system. You will be forwarded to select a directory, where you store your resources.\n(This is usually 'Program files\\Common Files\\Elemental Engine II\\' or if you did not run the copyToCommonFiles.bat yet, than'\\\\Development\\Common\\')") );
		MessageBox(NULL, envErrorMessage,  _T( "Environment Error!" ), MB_OK | MB_ICONERROR );

		// lets make the user pick a directory
		BROWSEINFO bi = { 0 };
		bi.lpszTitle = _T("Pick A Resource Directory");

		LPITEMIDLIST pidl = SHBrowseForFolder( &bi );

		if( pidl != 0 )
		{	
			// get the name of the folder
			TCHAR szPathBuffer[MAX_PATH];
			if( SHGetPathFromIDList( pidl, szPathBuffer ) )
			{
				strDefEnvResourcePath = m_ResourcePath = szPathBuffer;			
			}
			// else use current working directory
			else
			{
				strDefEnvResourcePath = m_ResourcePath = m_AppPath;
			}

			// free directory selection stuff
			IMalloc* imalloc = 0;
			if( SUCCEEDED( SHGetMalloc( &imalloc ) ) )
			{
				imalloc->Free( pidl );
				imalloc->Release( );
			}
		}
	}
	else
	{
		// set the member variable with the recieved value
		m_ResourcePath = strDefEnvResourcePath;
	}

	// set EE2 Default resource directory
	m_ToolBox->SetDirectories( NULL, &strDefEnvResourcePath );

	// set cwd to resource path for loading resources after the dlls are loaded.
	if (!SetCurrentDirectory( strDefEnvResourcePath ) )
	{
		LPVOID lpMsgBuf;
		CString Mess;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf, 0, NULL);
		Mess.Format(_T("Can't locate the environment directory. It is set to: '%s'.\nGDS won't function correctly without finding the resource files.\n (Did you run the 'Developmet\\Common\\copyToCommonFiles.bat' to copy necessary files to their required place?)\n"), strDefEnvResourcePath.c_str(), lpMsgBuf );
		MessageBox(NULL, Mess,  _T( "Environment Error!" ), MB_OK | MB_ICONERROR );
		//m_Logger->(LOGERROR, _T("Setting the 'Environment Directory' failed. Did you run the 'Developmet\\Common\\copyToCommonFiles.bat'?\n"));
		m_Logger->Print(_T("Setting the 'Environment Directory' failed. Did you run the 'Developmet\\Common\\copyToCommonFiles.bat'?\n") );
		ASSERT(0);
	}

	// Initialize EE render mgr
	m_ToolBox->CreateComponent(&CHashString(_T("CRenderManager")), 0);

	// set resource directories for renderer	
	SETWORKINGDIRECTORIESPARAMS swdp;
	swdp.ResourceDirectory = &strDefEnvResourcePath;
	swdp.BaseDirectory = &szAppPath;
	static DWORD msgHash_SetDirectories = CHashString(_T("SetDirectories")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetDirectories, sizeof(SETWORKINGDIRECTORIESPARAMS), &swdp );
	static DWORD msgHash_InitTime = CHashString(_T("InitTime")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitTime,0,NULL);
	
	GuiDrawLayer::m_Style=GUISTYLE_XP;

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	// put this here
	m_pMainWnd = pMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		return FALSE;
	}

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;

	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_splashDlg.DestroyWindow();

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// initial tick count
	m_iPreviousTickCount = GetTickCount();

	return TRUE;
}

// App command to run the dialog
void CGDSApp::OnAppAbout()
{
	m_splashDlg.EnableOK();
	m_splashDlg.DoModal();
}

const CString& CGDSApp::GetAppPath()
{
	return m_AppPath;
}

//void CGDSApp::SetResourcePath( const TCHAR *path)
//{
//	m_ResourcePath = path;	
//	WriteINIKey(INI_LastProjectPath, path, INI_SubSection);
//}

const TCHAR * CGDSApp::GetResourcePath()
{
	// direct call to get the EE resource path. 
	// we used our copy earlier, but it always needs to be updated when
	// EE resource path is updated
	m_ToolBox->GetDirectories( NULL, &m_ResourcePath );
	return m_ResourcePath;
}

/////////////////////////////////////////////////////////////////////////////
// CGDSApp message handlers


int CGDSApp::ExitInstance() 
{
	DeInitDLLS();

	HANDLELIST::iterator itHandle;
	itHandle = m_DLLHandles.begin();
	while (itHandle != m_DLLHandles.end())
	{
		AfxFreeLibrary(*itHandle);
		itHandle++;
	}	

	if (OpenFilter != NULL)
	{
		delete OpenFilter;
		OpenFilter = NULL;
	}

	if (SaveFilter != NULL)
	{
		delete SaveFilter;
		SaveFilter = NULL;
	}

	if (m_Logger != NULL)
	{
		m_Logger->Release();
		m_Logger->Shutdown();
		m_Logger = NULL;
	}

	WriteProfileString(_T(""), _T("Base Path"), m_ResourcePath);

	return CWinApp::ExitInstance();
}

void CGDSApp::DeInitDLLS()
{
	HANDLELIST::iterator dllIter = m_DLLHandles.begin();
	
	for (; dllIter != m_DLLHandles.end(); dllIter++)
	{
		DEINITDLL deInit = (DEINITDLL)GetProcAddress((*dllIter), _T("DeInitDLL"));
		if (deInit != NULL)
		{
			// call the deinit func if it exists
			(*deInit)();
		}
	}
}

//---------------------------------------------------------------------
// Function:	GetALLInterfaces
// Description:	This function gets the last loaded DLL information and
//              retrieves a DLL interface.  It then adds it to the
//              list of all DLLS loaded
// Parameters:	pathName = name of DLL Loaded.
// Returns:		.
//---------------------------------------------------------------------
void CGDSApp::GetALLInterfaces(const CString& pathName)
{
	HINSTANCE dllHandle;
	GETDLLVERSION GetDLLVersion;
	GETDLLINTERFACE GetDLLInterface;
	GETPRIORITY GetPriority;
	DWORD priority;
	INITDLL initDLL;
	CString Mess;

	DWORD version;
	IDLL *ri;
	

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
		Mess.Format(_T("Error Loading DLL:%s\n%s\n"), pathName, lpMsgBuf);
		MessageBox(NULL, Mess, "Error", MB_OK | MB_ICONINFORMATION );
		// Free the buffer.
		LocalFree( lpMsgBuf );
		return;
	}

	// make sure this dll is ok
	GetDLLVersion = (GETDLLVERSION)GetProcAddress(dllHandle, _T("GetDLLVersion"));
	GetDLLInterface = (GETDLLINTERFACE)GetProcAddress(dllHandle, _T("GetDLLInterface"));
	if (GetDLLVersion == NULL)
	{
		Mess.Format(_T("Unable to load %s\n"
			"Unable to locate version function.\n"), pathName);
		MessageBox(NULL, Mess, _T("DLL Load Error!"), MB_OK | MB_ICONINFORMATION);
		AfxFreeLibrary(dllHandle);
		return;
	}

	version = (*GetDLLVersion)() & ~1;
	if (version != (INTERFACEDLLVERSION & ~1))
	{
		Mess.Format(_T("Unable to load %s\n"
			"version %x doesn't match expected %x.\n"), pathName, version, INTERFACEDLLVERSION);
		MessageBox(NULL, Mess, _T("DLL Load Error!"),MB_OK | MB_ICONINFORMATION);
		FreeLibrary(dllHandle);
		return;
	}

	if (GetDLLInterface != NULL)
	{
		ri = GetDLLInterface();
		if (ri == NULL)
		{
			Mess.Format(_T("Unable to load %s\n"
				"GetDLLInterface routine failed.\n"), pathName);
			MessageBox(NULL, Mess, _T("DLL Load Error!"), MB_OK | MB_ICONINFORMATION);
			AfxFreeLibrary(dllHandle);
			return;
		}

		if (ri->GetPriority() == 0)
		{
			m_Logger->Print(_T("Priority should have been set in %s::GetDLLInterface()"),
				pathName);
		}

		m_DLLS.push_back(ri);
	}
	else
	{
		// get the init function
		initDLL = (INITDLL)GetProcAddress(dllHandle, _T("InitDLL"));

		if (initDLL == NULL)
		{
			Mess.Format(_T("Unable to load %s\n"
				"GetDLLInit routine failed.\n"), pathName);
			MessageBox(NULL, Mess, _T("DLL Load Error!"), MB_OK | MB_ICONINFORMATION);
			AfxFreeLibrary(dllHandle);
			return;
		}

		// get the priority function
		GetPriority = (GETPRIORITY)GetProcAddress(dllHandle, _T("GetPriority"));

		if (GetPriority == NULL)
		{
			Mess.Format(_T("Unable to load %s\n"
					"GetDLLPriority routine failed.\n"), pathName);
			MessageBox(NULL, Mess, _T("DLL Load Error!"), MB_OK | MB_ICONINFORMATION);
			AfxFreeLibrary(dllHandle);
			return;
		}

		priority = (*GetPriority)();

		DLLINITMAP::iterator itrDLL = m_DLLInits.find(priority);
		if ( itrDLL == m_DLLInits.end() )
		{
			TCHAR fileName[_MAX_FNAME];
			TCHAR ext[_MAX_EXT];
			GDSINITDLLINFO curDLLInfo;

			_tsplitpath(pathName, NULL, NULL, fileName, ext);
			curDLLInfo.dllName = fileName;
			curDLLInfo.dllName += ext;
			curDLLInfo.initDLL = initDLL;
			m_DLLInits.insert( pair<DWORD, GDSINITDLLINFO>( priority, curDLLInfo ) );
		}
		else
		{
			Mess.Format(_T("Unable to load %s\n"
					"Priority conflicts with another DLL\n"), pathName);
			MessageBox(NULL, Mess, _T("DLL Load Error!"), MB_OK | MB_ICONINFORMATION);
			AfxFreeLibrary(dllHandle);
			return;
		}
	}

	m_DLLHandles.push_back(dllHandle);
}

//---------------------------------------------------------------------
// Function:	InitAllDLLS
// Description:	This function calls all the initialize functionsn, now
//              that the DLLS are sorted
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
void CGDSApp::InitAllDLLS()
{
    STDDLLLIST::iterator itDLL;
	DLLINITMAP::iterator dimIter;
    IDLL *pDLL;
	CString initText;



    for (itDLL = m_DLLS.begin(); itDLL != m_DLLS.end(); itDLL++)
    {
        pDLL = (*itDLL);
		CString *dllName;
		dllName = pDLL->GetName();
		TCHAR fileName[_MAX_FNAME];
		TCHAR ext[_MAX_EXT];
		_tsplitpath(dllName->GetString(), NULL, NULL, fileName, ext);

		initText.Format(_T("Initializing:\n%s%s"), fileName, ext);
		m_splashDlg.SetLoadingText(initText.GetString());
        if (!pDLL->Initialize())
        {
           m_Logger->Print(_T("%s DLL failed to initialized."),
                (LPCSTR)dllName->GetString());
        }
    }

	// init non idlls
	for (dimIter = m_DLLInits.begin(); dimIter != m_DLLInits.end(); ++dimIter)
	{
		GDSINITDLLINFO *dllInfo;
		dllInfo = &dimIter->second;
		initText.Format(_T("Initializing:\n%s"), (LPCSTR)dllInfo->dllName);
		m_splashDlg.SetLoadingText(initText.GetString());
		// initialize the dll
		(*dllInfo->initDLL)();
	}
}

//---------------------------------------------------------------------
// Function:	LoadDLLS
// Description:	This functions searchs and loads all plug-ins
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
void CGDSApp::LoadDLLS()
{
	// for now hardcode path
	SearchDLLS(CString(_T(".\\Plugins\\*.dlb")));

	SearchDLLS(CString(_T(".\\Plugins\\*.dle")));

	SearchDLLS(CString(_T(".\\Plugins\\*.dlo")));

	m_DLLS.sort(DLLLessThan());

    InitAllDLLS();
}

//---------------------------------------------------------------------
// Function:	SearchDLLS
// Description:	Search for DLLS in a specified directory with
//				recursion capability
// Parameters:	path = initial path to search
// Returns:		.
//---------------------------------------------------------------------
void CGDSApp::SearchDLLS(const CString& pathName)
{
	CString path;
	WIN32_FIND_DATA fData;
	HANDLE hFile;
	BOOL done;
	int wildIndex;
	CString wildcard;
	CString dllName;
	CString loadText;

	wildIndex = pathName.ReverseFind('\\');
	path = pathName.Left(wildIndex+1);

	if (! SetCurrentDirectory( m_AppPath ) )
		ASSERT(0);

	hFile = FindFirstFile(pathName, &fData);
	done = (hFile == INVALID_HANDLE_VALUE);

	while (!done)
	{ 
		dllName = path;
		dllName += fData.cFileName;
		loadText.Format(_T("Loading:\n%s"), fData.cFileName);
		m_splashDlg.SetLoadingText(loadText.GetString());
		GetALLInterfaces(dllName);
 
	    if (!FindNextFile(hFile, &fData))
		{
	        done = TRUE;
	    }
	}

	if (hFile != INVALID_HANDLE_VALUE)
	{
		VERIFY(FindClose(hFile));
	}

} 

CRecentFileList *CGDSApp::GetRecentFileList()
{
	ASSERT_VALID(this);
	ASSERT(m_pRecentFileList != NULL);

	return m_pRecentFileList;
}

void CGDSApp::OnFileOpen() 
{
	if (OpenFilter == NULL)
	{
		BuildFilterLists();
	}


	Ofn.lpstrFilter = OpenFilter;
	Ofn.lpstrFile = OpenFileName;
	Ofn.lpstrTitle = "Open File";
	Ofn.hwndOwner = GetMainWnd()->m_hWnd;
	Ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

	if (!GetOpenFileName(&Ofn))
	{
		// cancel selected.
		return;
	}

	LoadFile(Ofn.lpstrFile);
}

//---------------------------------------------------------------------
// Function:	GetFilterStrLength
// Description:	Get the length of the special filter string passed in
// Parameters:	filterStr = pointer to the string
// Returns:		DWORD = size of filter string in bytes
//---------------------------------------------------------------------
DWORD CGDSApp::GetFilterStrLength(LPCTSTR filterStr)
{
	DWORD filterSize;
	DWORD tLen;

	filterSize = 0;
	if (filterStr != NULL)
	{
		while ((*filterStr != '\0') && (*(filterStr+1) != '\0'))
		{
			// get length of filter description string
			tLen = _tcslen(filterStr)+1;
			// bump count by this amount
			filterSize += tLen;
			filterStr += tLen;
			// get actual filter string length
			tLen = _tcslen(filterStr)+1;
			filterSize += tLen;
			filterStr += tLen;
		}
	}

	return filterSize;
}


//---------------------------------------------------------------------
// Function:	BuildFilterLists
// Description:	Build a set of filters lists from all the Object
//				dlls that were loaded, fills in global OpenFilter and
//				global SaveFilter
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
void CGDSApp::BuildFilterLists()
{
	DWORD openFilterSize;
	DWORD saveFilterSize;
	DWORD tLen;
	LPTSTR tOStr;
	LPTSTR tSStr;
	FILTERSTRINGSLIST *fsl;
	FILTERSTRINGSLIST::iterator fslIter;
	FILTERSTRINGS *fs;
	bool validFilters;
		
	openFilterSize = 0;
	saveFilterSize = 0;
	static DWORD msgHash_GetFilterStrings = CHashString(_T("GetFilterStrings")).GetUniqueID();
	validFilters = (m_ToolBox->SendMessage(msgHash_GetFilterStrings, sizeof(FILTERSTRINGSLIST **), &fsl) == MSG_HANDLED);
	// Send Message for this....
	if (validFilters)
	{
	    for (fslIter = fsl->begin(); fslIter != fsl->end(); fslIter++)
		{
			fs = *fslIter;
			// get length of open/import filters
			openFilterSize += GetFilterStrLength(fs->m_LoadFilter);
			// and save/export filters
			saveFilterSize += GetFilterStrLength(fs->m_SaveFilter);
		}
	}

	// add in all files strings and 2 \0's
	openFilterSize += GetFilterStrLength(allFiles)+2;
	OpenFilter = new TCHAR[openFilterSize];
	if (OpenFilter == NULL)
	{
		MessageBox(GetMainWnd()->m_hWnd, _T("Unable to allocate space for open filter strings"), _T("Memory Error!"),
			MB_OK);
		return;
	}

	// add in all files and 2 \0's
	saveFilterSize += GetFilterStrLength(allFiles)+2;
	saveFilterSize += GetFilterStrLength(allFiles);
	SaveFilter = new TCHAR[saveFilterSize];
	if (SaveFilter == NULL)
	{
		// we can't use the dialog safely.
		delete OpenFilter;
		OpenFilter = NULL;
		MessageBox(GetMainWnd()->m_hWnd,_T("Unable to allocate space for save filter strings"), _T("Memory Error!"),
			MB_OK);
		return;
	}

	// now copy all files strings to filters
	tOStr = OpenFilter;
	tLen = GetFilterStrLength(allFiles);
	memcpy(tOStr, allFiles, tLen);
	tOStr += tLen;

	tSStr = SaveFilter;
	tLen = GetFilterStrLength(allFiles);
	memcpy(tSStr, allFiles, tLen);
	tSStr += tLen;

	// sorting the filters alphabetically using a set
	FILTERSET loadFilterSet;
	FILTERSET saveFilterSet;

	// add every separate filter to the set
	// one pointer can point multiple filters delimited by '\0'-s!
	for ( fslIter = fsl->begin(); fslIter != fsl->end(); fslIter++ )
	{
		fs = *fslIter;
		// get length of open/import filters
		tLen = GetFilterStrLength(fs->m_LoadFilter);

		// collect the filters from one filterstring
		DWORD diff = 0;
		
		// to save the original pointer and modify a copy the get the extensions
		TCHAR * currentFilterString = (TCHAR *) fs->m_LoadFilter;

		bool IsExtension = false;

		while (diff < tLen)
		{
			// get the pointed filter and add it to the set
			TCHAR* currentString = (TCHAR*) (currentFilterString + diff);
			diff += strlen(currentString) + 1; 

			if( !IsExtension ) 
			{
				loadFilterSet.insert( currentString );
			}
			IsExtension = !IsExtension;
		}

		// we must have equal number of names and extensions,
		// the next one should be a name here:
		assert( !IsExtension ); 

		// get length of save filters
		tLen = GetFilterStrLength(fs->m_SaveFilter);

		// collect the filters from one filterstring
		diff = 0;
		
		// to save the original pointer and modify a copy the get the extensions
		currentFilterString = (TCHAR *) fs->m_SaveFilter;

		IsExtension = false;

		while (diff < tLen)
		{
			TCHAR* currentString = (TCHAR*) (currentFilterString + diff);
			diff += strlen(currentString) + 1;  

			if( !IsExtension ) 
			{
				saveFilterSet.insert( currentString );
			}
			IsExtension = !IsExtension;
		}
	}

	if (validFilters)
	{
		// get the lenght of the load filter - with extensions
		FILTERSET::iterator loadIterator = loadFilterSet.begin();
		
		// WHY DOES IT RETURN &0xC0C0C0C0???
		//FILTERSET::iterator endIterator = loadFilterSet.end();
		// Rather use for loop instead of while loop...

		for(unsigned int i = 0; i < loadFilterSet.size() ; i++)
		{

			TCHAR *filterName = *loadIterator;
			int filterNameLength =_tcslen(filterName );

			// calculate the pointer to the extension
			TCHAR *extPointer = filterName + filterNameLength + 1;

			// the extension lenght
			int extensionLength = _tcslen( extPointer );
			
			// the ful length is the filterName + Extension + 1 ('\0' between them)
			int copyLength = filterNameLength + extensionLength + 1; 

			// copy the memory of the filter
			memmove(tOStr, filterName, copyLength);
			
			// modify the target pointer
			tOStr += copyLength;

			// \0 to delimit separate filters
			*tOStr++ = '\0';

			// increment the loop iterator
			loadIterator++;
		}

		// everything for the save filters
		FILTERSET::iterator saveIterator = saveFilterSet.begin();
		
		for( unsigned int i = 0; i < saveFilterSet.size() ; i++ )
		{
			int filterNameLength =_tcslen( *saveIterator );

			// calculate the pointer to the extension
			TCHAR *extPointer = (*saveIterator) + filterNameLength + 1;

			// the extension lenght
			int extensionLength = _tcslen( extPointer );
			
			// the ful length is the filterName + Extension + 1 ('\0' between them)
			int copyLength = filterNameLength + extensionLength + 1; 

			// copy the memory of the filter
			memmove(tSStr, *saveIterator, copyLength);
			
			// modify the target pointer
			tSStr += copyLength;

			// \0 to delimit separate filters
			*tSStr++ = '\0';

			// increment the loop iterator
			saveIterator++;
		}
	}

	// AS THE FILTERS ARE ENDED BY '\0'-s, WE ONLY NEED TO ADD ONE
	// NEW TO END THE FILTERLIST BY TWO '\0'-s.

	// append 2 \0's to open file filter string
	*tOStr++ = '\0';

	// append 2 \0's to save file filter string
	*tSStr++ = '\0';
}

BOOL CGDSApp::OnOpenRecentFile(UINT nID)
{
	CRecentFileList *appRFL = GetRecentFileList();
	ASSERT(appRFL != NULL);

	ASSERT(nID >= ID_FILE_MRU_FILE1);
	ASSERT(nID < ID_FILE_MRU_FILE1 + (UINT)appRFL->GetSize());
	int nIndex = nID - ID_FILE_MRU_FILE1;
	ASSERT((*appRFL)[nIndex].GetLength() > 0);

	TRACE2("MRU: open file (%d) '%s'.\n", (nIndex) + 1,
			(LPCTSTR)(*appRFL)[nIndex]);

	// get the name of the file that was selected
	return LoadFile(appRFL->m_arrNames[nIndex]);
}

BOOL CGDSApp::OnIdle(LONG lCount) 
{
	CWinApp::OnIdle(lCount);

	// due to unsigned arithmetic this code should work even when GetTickCount wraps to zero
	DWORD iCurrentTickCount = GetTickCount();
	DWORD iDeltaTickCount = iCurrentTickCount - m_iPreviousTickCount;
	m_iPreviousTickCount = iCurrentTickCount;

	GLOBALTICKPARAMS gtp;
	gtp.tickCount = iDeltaTickCount;
	static DWORD msgHash_GlobalUpdateTick = CHashString(_T("GlobalUpdateTick")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GlobalUpdateTick, sizeof(GLOBALTICKPARAMS), &gtp, NULL, NULL);

	// Let other processes get some Processor time. We are not greedy...
	// Or just a little bit... 1ms should be enough for everyone...  
	Sleep(1);

	return TRUE;
}

BOOL CGDSApp::LoadFile(LPCTSTR szFileName)
{
	LOADFILEEXTPARAMS lfep;
	StdString fileName = szFileName;
	fileName.MakeSafeFileName();
	lfep.fileName = (TCHAR*)fileName.c_str();
	lfep.bInternalLoad = false;

	// attempt to load the selected file
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(lfep), &lfep);

	// see if load was successful
	if (retVal != MSG_HANDLED)
	{
		// if not report the error
		CString lastErr = GetLastErrorMessage();
		CString msg = FormatString(IDS_ERR_LOAD_FILE, fileName.c_str(), (LPCTSTR)lastErr);
		CString title = FormatString(IDS_ERROR);
		AfxGetMainWnd()->MessageBox(msg, title, MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}

