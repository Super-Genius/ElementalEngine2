///============================================================================
/// \file		FlackSquad.cpp
/// \brief		Main Entry for application Red Phoenix
/// \date		05-25-2005
/// \author		Kenneth Hurley
/// \note		Copyright (c) 2005 Signature Devices, Inc.
///============================================================================

#include "stdafx.h"

// main application
CGFXApp *gMainApp;
bool gbAppExiting = false;

bool gbUseZipData = false;

#ifdef _DEBUG
bool g_bMemoryBreakAlloc = false;
bool g_bEnableAllocHook = false;
#endif

#define DEBUG_MOUSE

int DebugAllocHook(int nAllocType, void *pvData,
      size_t nSize, int nBlockUse, long lRequest,
      const unsigned char * szFileName, int nLine )
{
#ifdef _DEBUG
	const int spread = 10;
	if (
//		(nAllocType == _HOOK_ALLOC) && 
//		lRequest == 416685 && 
//		(lRequest >= 1211954-spread && lRequest <= 1211954+spread) && 
//		pvData == (void*)0x056C9FD0 && 
//		(nLine == 160) && 
//		(szFileName == (const unsigned char *)0x01adc840) && 
//		strcmp( (const char*)szFileName, "c:\\development\\elementalengine2\\src\\carchivefactory.cpp" ) == 0 && 
		(nSize == 3528) &&
		(g_bEnableAllocHook) &&
		true)
	{
		assert( false );
	}
#endif
	return TRUE; // continue with operation
}

// contructor
CGFXApp::CGFXApp()
{
	#ifdef _DEBUG
	_CrtSetDbgFlag( _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF );

	if( g_bMemoryBreakAlloc == false )
	{
		g_bMemoryBreakAlloc = true;
//		_CrtSetBreakAlloc(105892);
//		_CrtSetAllocHook(DebugAllocHook);
	}
	#endif
	
	m_EnableLogging = false;

	// hookup the singleton component
	SINGLETONINSTANCE(GameComponent)->SetParent(this);	
}

// destructor
CGFXApp::~CGFXApp()
{
}

void CGFXApp::ParseCommandLine( LPTSTR lpCmdLine )
{
	SINGLETONINSTANCE(GameComponent)->ParseCommandLine( lpCmdLine );
}

void CGFXApp::WriteGlobalAttributes()
{
	SINGLETONINSTANCE(GameComponent)->WriteGlobalAttributes();
}

void CGFXApp::Run()
{
	BOOL bQuit = FALSE;

	// Main message loop:
	while (!bQuit)
	{
#ifdef _SHELL
		PollGVRIO();										
#endif
		Update();
#ifdef WIN32
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message != WM_QUIT)
			{
				if (!TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg)) 
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				bQuit = TRUE;
			}
		}
#endif//WIN32
	}
}

void CGFXApp::Quit()
{
#ifdef WIN32
	PostQuitMessage(0);
#else
	exit(0);
#endif//WIN32
}

// come here for constant updates to the system
void CGFXApp::Update()
{
	// due to unsigned arithmetic this code should work even when GetTickCount wraps to zero
	DWORD iCurrentTickCount = GetTickCount();
	DWORD iDeltaTickCount = iCurrentTickCount - m_iPreviousTickCount;
	m_iPreviousTickCount = iCurrentTickCount;

	GLOBALTICKPARAMS gtp;
	gtp.tickCount = 0;
	static DWORD msgGlobalUpdateTick = CHashString( _T("GlobalUpdateTick") ).GetUniqueID();
	m_ToolBox->SendMessage(msgGlobalUpdateTick, sizeof(GLOBALTICKPARAMS), &gtp, NULL, NULL);
	// other processing by application if need be
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
#ifdef WIN32
ATOM CGFXApp::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_FLACKSQUAD);

	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_FLACKSQUAD;
	wcex.lpszClassName	= m_szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_FLACKSQUAD);

	return RegisterClassEx(&wcex);
}
#endif//WIN32

////////////////////// TODO: read in desired resolution from file //////////////////////////////
void CGFXApp::InitRenderer(BOOL fullScreen)
{
	RECT winRect;

	winRect.left = winRect.top = 0;
	winRect.right = 1280;
	winRect.bottom = 720;

#ifdef WIN32
	WINDOWINFO wi;
	BOOL ret = GetWindowInfo(m_hWnd, &wi);
	ret = AdjustWindowRect(&winRect, wi.dwStyle, FALSE);

	// does this because above function sets negative vals in left and top
	winRect.right -= winRect.left;
	winRect.bottom -= winRect.top;

	SetWindowPos(m_hWnd, NULL, winRect.left, 0, winRect.right, winRect.bottom, 0);
	
	// client rect should be 1024 x 768
	GetClientRect(m_hWnd, &winRect);

	m_iViewWidth = (winRect.right - winRect.left);
	m_iViewHeight = (winRect.bottom - winRect.top);
#else
	m_iViewWidth = 1280;
	m_iViewHeight = 720;
#endif//WIN32

	// create render context msg
	CREATERENDERCONTEXTPARAMS paramsContext;
	paramsContext.depthbits = 24;
	paramsContext.colorbits = 32;

	if (fullScreen)
	{
		// set to the base canvase work space
		// ...since the gui elements were created for
		// 1024x768 resolution
		paramsContext.width = 1280;
		paramsContext.height = 720;
		paramsContext.hwindow = m_hWnd;
		paramsContext.fullscreen = true;
	}
	else
	{
		paramsContext.width = m_iViewWidth;
		paramsContext.height = m_iViewHeight;
		paramsContext.hwindow = m_hWnd;
		paramsContext.fullscreen = false;
	}

//	paramsContext.retVal_RenderID = &m_iContextID;

	static DWORD msgCreateRenderContext = CHashString( _T("CreateRenderContext") ).GetUniqueID();
	m_ToolBox->SendMessage(msgCreateRenderContext, sizeof(paramsContext), &paramsContext, 
								NULL, NULL);
	m_ToolBox->CreateComponent( &CHashString(_T("CSceneVisibilityManager")), NULL, NULL );

	// set viewport msg
	VIEWPORTPARAMS paramsSetViewport;
	paramsSetViewport.offsetx = 0;
	paramsSetViewport.offsety = 0;

	static DWORD msgSetViewPort = CHashString( _T("SetViewPort") ).GetUniqueID();

	if (fullScreen)
	{
		// this should be loaded from preferences
		// ...these parameters can be different from
		// the rendercontext's width and height
		paramsSetViewport.width = 1280;
		paramsSetViewport.height = 720;
		m_ToolBox->SendMessage(msgSetViewPort, sizeof(VIEWPORTPARAMS), &paramsSetViewport);
		static DWORD msgSetFullScreen = CHashString( _T("SetFullScreen") ).GetUniqueID();
		m_ToolBox->SendMessage(msgSetFullScreen, 0, NULL);
	}
	else
	{
		paramsSetViewport.width = m_iViewWidth;
		paramsSetViewport.height = m_iViewHeight;

		m_ToolBox->SendMessage(msgSetViewPort, sizeof(VIEWPORTPARAMS), &paramsSetViewport);
	}
	
	// This should all be commented out
	float fWorldExtent = 8192.0f;

	static DWORD msgSetWorldExtents = CHashString( _T("SetWorldExtents") ).GetUniqueID();
	m_ToolBox->SendMessage(msgSetWorldExtents, sizeof(float), &fWorldExtent);

	static DWORD msgInitTime = CHashString( _T("InitTime") ).GetUniqueID();
	m_ToolBox->SendMessage(msgInitTime, 0, NULL);			
}


//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL CGFXApp::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	DWORD ReturnValue;

	BOOL bWindowedMode = FALSE;

	// should come from .ini file
#ifdef _DEBUG
	m_EnableLogging = true;
#endif

#ifdef WIN32
	// change to the path so dlls can load dlls directory
	TCHAR curDir[_MAX_PATH+1];
	TCHAR pathName[_MAX_PATH+1];
	TCHAR drive[_MAX_DRIVE];

	m_hInst = hInstance; // Store instance handle in our global variable
	GetModuleFileName(hInstance, pathName, _MAX_PATH);
	_tsplitpath(pathName, drive, curDir, NULL, NULL);
	sprintf(pathName, _T("%s%s"), drive, curDir);

	m_AppPath = pathName;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FLACKSQUAD, m_szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

#ifndef _SHELL
	if (MessageBox(NULL, _T("Would you like to run in windowed mode?"),
		_T("Question"), MB_YESNO) == IDYES)
	{
		bWindowedMode = TRUE;
	}
#endif

	if(!bWindowedMode)
	{
		// create a window w/ no borders or title or menu
		m_hWnd = CreateWindow(m_szWindowClass, NULL, WS_POPUP,
			0, 0, 0, 0, NULL, NULL, hInstance, NULL);
		SetMenu(m_hWnd, NULL);
	}
	else
	{
		// create a window w/ borders and tittle
		m_hWnd = CreateWindow(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
		SetMenu(m_hWnd, NULL);
	}

	if (!m_hWnd)
	{
		return FALSE;
	}

	// we are going into full screen, so don't show it.
	if (bWindowedMode)
	{
		ShowWindow(m_hWnd, nCmdShow);
		UpdateWindow(m_hWnd);
	}

	// load accelerators
	m_hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_FLACKSQUAD);
#else
	m_AppPath = _T(".\\");
	m_hWnd = NULL;
#endif

	m_ToolBox = EngineGetToolBox();

#ifdef _DEBUG
	// Enable Duplicate Object Checking
	static DWORD msgHash_FlagDuplicateObjectChecking = CHashString(_T("FlagDuplicateObjectChecking")).GetUniqueID();
	bool DuplicateObjectCheck = true;
	m_ToolBox->SendMessage(msgHash_FlagDuplicateObjectChecking,sizeof(DuplicateObjectCheck),&DuplicateObjectCheck);
#endif

	// set resource directories for renderer
	StdString szAppPath = m_AppPath;
	SETWORKINGDIRECTORIESPARAMS swdp;
	swdp.ResourceDirectory = &szAppPath;
	swdp.BaseDirectory = &szAppPath;
	static DWORD msgSetDirectories = CHashString( _T("SetDirectories") ).GetUniqueID();
	m_ToolBox->SendMessage( msgSetDirectories, sizeof(SETWORKINGDIRECTORIESPARAMS), &swdp );

	/// Load in base DLLS into memory
	LoadDLLS();

	/// Load any ZIP files in the base directory
	if( gbUseZipData )
	{
		static DWORD msgEnableZipFileLoading = CHashString( _T("EnableZipFileLoading") ).GetUniqueID();
		m_ToolBox->SendMessage( msgEnableZipFileLoading, sizeof(bool), &gbUseZipData );
		
		RegisterZipFiles(string(_T("*.zip")));
	}
	
    //this now records a null debug hook, which vision will restore when application exits (see above note).
	InitRenderer(!bWindowedMode);

	// initial tick count
	m_iPreviousTickCount = GetTickCount();

	// start input
	INPUTENGPARAMS inputParams;

	HMODULE mod = GetModuleHandle(NULL);
	inputParams.hMod = mod;
	inputParams.hWnd = m_hWnd;
	inputParams.mouseImmediate = true;
#ifdef DEBUG_MOUSE
	inputParams.mouseExclusive = false;
#else	
	inputParams.mouseExclusive = true;
#endif
	inputParams.mouseFG = true;
	inputParams.kbImmediate = true;
	inputParams.kbExclusive = false;
	inputParams.kbFG = true;

	// send message to start input engine
	static DWORD msgBegin = CHashString( _T("Begin") ).GetUniqueID();
	m_ToolBox->SendMessage(msgBegin, (DWORD)sizeof(INPUTENGPARAMS), &inputParams, NULL, 
						   &CHashString(_T("CInputManager")));

	static DWORD msgStart = CHashString( _T("Start") ).GetUniqueID();
	m_ToolBox->SendMessage(msgStart, 0, NULL, NULL, 
						   &CHashString(_T("CInputManager")));

	// NOW we init the keybindings
	static DWORD msgInitGameKeybindings = CHashString( _T("InitGameKeybindings") ).GetUniqueID();
	m_ToolBox->SendMessage(msgInitGameKeybindings, 0, NULL);


	EXECUTESCRIPTSTRUCT ess;
	CHashString scriptName(_T(""));

	// set the active scene
	CHashString sceneName(_T("World"));
	GETSETACTIVESCENE gsas;
	gsas.pSceneName = &sceneName;
	static DWORD msgSetActiveScene = CHashString( _T("SetActiveScene") ).GetUniqueID();
	ReturnValue = m_ToolBox->SendMessage(msgSetActiveScene, sizeof(gsas), &gsas);
	if( ReturnValue != MSG_HANDLED )
	{
		m_ToolBox->Log( LOGFATALERROR, "Could not set active scene." );
	}


	// Load global attributes schema
	CHashString globalAttributesGroupName( _T("FlackSquadGlobalAttributes") );
	CHashString globalAttributesSchemaFile( _T("Databases\\GlobalAttributes.fxl") );
	CHashString globalAttributesSchemaName( _T("FlackSquadGlobalAttributes_Schema") );

	LOADGLOBALATTRIBUTESPARAMS loadGlobalAttributesParams;
	loadGlobalAttributesParams.Name = &globalAttributesGroupName;
	loadGlobalAttributesParams.SchemaFile = &globalAttributesSchemaFile;
	loadGlobalAttributesParams.SchemaName = &globalAttributesSchemaName;

	static DWORD msgLoadGlobalAttributes = CHashString( _T("LoadGlobalAttributes") ).GetUniqueID();
	ReturnValue = m_ToolBox->SendMessage(msgLoadGlobalAttributes, sizeof(LOADGLOBALATTRIBUTESPARAMS), &loadGlobalAttributesParams);

	// Write the global values based on command line parameters
	WriteGlobalAttributes();


	// Load and Start the Game Lua
 	ess.scriptName = &scriptName;
	ess.fileName = _T("Scripts\\Game.lua");
	ess.bAutoStart = true;
	static DWORD msgLoadAndStartScript = CHashString( _T("LoadAndStartScript") ).GetUniqueID();
	ReturnValue = m_ToolBox->SendMessage(msgLoadAndStartScript, sizeof(EXECUTESCRIPTSTRUCT), &ess);
	if( ReturnValue != MSG_HANDLED )
	{
		m_ToolBox->Log( LOGFATALERROR, "Could Not Load Lua Script File: %s", ess.fileName );
	}

	// TODO: Add LoadAndInitStateObject message (using "State Machine\\Game.xms")
	LOADANDINITSTATEOBJECTPARAMS liso;
	CHashString StateObject(_T("GameStateObject"));
	CHashString StateMachineName(_T("Game_StateMachine"));
	CHashString StateMachineFileName(_T("State Machine\\Game.xms"));

	liso.StateObjectName = &StateObject;
	liso.StateObjectParentName = NULL;
	liso.StateMachineName = &StateMachineName;
	liso.StateMachineFileName = &StateMachineFileName;
	liso.StateMachineStartState = NULL;
	liso.bIsInHierarchy = false;

	static DWORD msgLoadAndInitStateObject = CHashString( _T("LoadAndInitStateObject") ).GetUniqueID();
	ReturnValue = m_ToolBox->SendMessage( msgLoadAndInitStateObject, sizeof(liso), &liso );
	if( ReturnValue != MSG_HANDLED )
	{
		m_ToolBox->Log( LOGFATALERROR, "Could Not Load State Machine File: %s", StateMachineFileName.GetString() );
	}
	
	return TRUE;
}

void CGFXApp::ExitInstance()
{
	gbAppExiting = true;
	DLLINFOMAP::iterator dimIter;
	DLLINFO info;

	// raise exit event on game statemachine
	CHashString eventName = _T("Game_Exit_Event");
	CHashString StateObject(_T("GameStateObject"));
	TRIGGEREVENTPARAMS eventA;
	eventA.EventName = &eventName;
	eventA.EventParamsArchive = NULL;
	static DWORD msgTriggerEvent = CHashString( _T("TriggerEvent") ).GetUniqueID();
	m_ToolBox->SendMessage( msgTriggerEvent, sizeof(eventA), &eventA, &StateObject, &CHashString(_T("CQHStateObject")) );

	// clear hierarchy
	static DWORD msgClearHierarchy = CHashString( _T("ClearHierarchy") ).GetUniqueID();
	m_ToolBox->SendMessage(msgClearHierarchy, 0, NULL, NULL, NULL);

	// deregister this render context
	static DWORD msgDeleteRenderContext = CHashString( _T("DeleteRenderContext") ).GetUniqueID();
	m_ToolBox->SendMessage( msgDeleteRenderContext, sizeof(m_iContextID), &m_iContextID );

	DeInitDLLS();

	for (dimIter = m_DLLInfos.begin(); dimIter != m_DLLInfos.end(); ++dimIter)
	{
		info = dimIter->second;
		FreeLibrary(info.handle);
	}

}

//---------------------------------------------------------------------
// Function:	LoadDLLS
// Description:	This functions searchs and loads all plug-ins
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
void CGFXApp::LoadDLLS()
{
	// for now hardcode path
	SearchDLLS(string(_T(".\\Plugins\\*.dlo")));
	InitDLLS();
}

//---------------------------------------------------------------------
// Function:	SearchDLLS
// Description:	Search for DLLS in a specified directory with
//				recursion capability
// Parameters:	path = initial path to search
// Returns:		.
//---------------------------------------------------------------------
void CGFXApp::SearchDLLS(const string& pathName)
{
	string path;
	WIN32_FIND_DATA fData;
	HANDLE hFile;
	BOOL done;
	size_t wildIndex;
	string wildcard;
	string dllName;
	

	wildIndex = pathName.rfind('\\');
	path = pathName.substr(0, wildIndex+1);

#ifdef WIN32
	if (! SetCurrentDirectory( m_AppPath.c_str() ) )
	{
		// log error
		return;
	}
#endif

	hFile = FindFirstFile(pathName.c_str(), &fData);
	done = (hFile == INVALID_HANDLE_VALUE);

	while (!done)
	{ 
		dllName = path;
		dllName += fData.cFileName;
		
		GetALLInterfaces(dllName);
 
	    if (!FindNextFile(hFile, &fData))
		{
	        done = TRUE;
	    }
	}

	if (hFile != INVALID_HANDLE_VALUE)
	{
		FindClose(hFile);
	}
}

//---------------------------------------------------------------------
// Function:	RegisterZipFiles
// Description:	This functions searchs and registers all ZIP files
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
void CGFXApp::RegisterZipFiles(const string& pathName)
{
	string path;
	WIN32_FIND_DATA fData;
	HANDLE hFile;
	BOOL done;
	size_t wildIndex;
	string wildcard;
	string zipName;
	

	wildIndex = pathName.rfind('\\');
	path = pathName.substr(0, wildIndex+1);

#ifdef WIN32
	if (! SetCurrentDirectory( m_AppPath.c_str() ) )
	{
		// log error
		return;
	}
#endif

	hFile = FindFirstFile(pathName.c_str(), &fData);
	done = (hFile == INVALID_HANDLE_VALUE);

	while (!done)
	{ 
		zipName = fData.cFileName;

		// Specify Zipped Data Folders
		static DWORD msgHash_AddZipFile = CHashString(_T("AddZipFile")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_AddZipFile,sizeof(zipName.c_str()),(void*)zipName.c_str());
			
	    if (!FindNextFile(hFile, &fData))
		{
	        done = TRUE;
	    }
	}

	if (hFile != INVALID_HANDLE_VALUE)
	{
		FindClose(hFile);
	}
}

//---------------------------------------------------------------------
// Function:	GetALLInterfaces
// Description:	This function gets the last loaded DLL information and
//              retrieves a DLL interface.  It then adds it to the
//              map of all DLLS loaded
// Parameters:	pathName = name of DLL Loaded.
// Returns:		.
//---------------------------------------------------------------------
void CGFXApp::GetALLInterfaces(const string& pathName)
{
	HINSTANCE dllHandle;
	GETDLLVERSION GetDLLVersion;
	GETPRIORITY GetPriority;
	StdString Mess;
	DLLINFO info;
	DWORD priority;
	DWORD version;


	// try loading the dll here
	dllHandle = LoadLibrary(pathName.c_str());
	if (dllHandle == NULL)
	{
#ifdef WIN32
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf, 0, NULL);
		// Display the string.
		Mess = _T("Error Loading DLL: ");
		Mess += pathName;
		Mess += _T("\n");
		Mess += (LPTSTR)lpMsgBuf;
		Mess += _T("\n");
		MessageBox(NULL, Mess.c_str(), "Error", MB_OK | MB_ICONINFORMATION );
		// Free the buffer.
		LocalFree( lpMsgBuf );
#endif//WIN32
		m_ToolBox->Log( LOGFATALERROR, _T("Error loading dll %s.\n"), pathName.c_str() );
		return;
	}

	// make sure this dll is ok
	GetDLLVersion = (GETDLLVERSION)GetProcAddress(dllHandle, _T("GetDLLVersion"));

	if (GetDLLVersion == NULL)
	{
		Mess = _T("Unable to load ");
		Mess += pathName;
		Mess += _T("\nUnable to locate version function.\n");
		m_ToolBox->Log( LOGFATALERROR, Mess.c_str() );
		FreeLibrary(dllHandle);
		return;
	}

	version = (*GetDLLVersion)() & ~1;

	if (version != (INTERFACEDLLVERSION & ~1))
	{
		Mess = _T("Unable to load ");
		Mess += pathName;
		Mess += _T("\nVersion doesn't match expected.\n");
		m_ToolBox->Log( LOGFATALERROR, Mess.c_str() );
		FreeLibrary(dllHandle);
		return;
	}

	// get the init function
	info.initDLL = (INITDLL)GetProcAddress(dllHandle, _T("InitDLL"));

	if (info.initDLL == NULL)
	{
		Mess = _T("Unable to load ");
		Mess += pathName;
		Mess += _T("\nUnable to locate init function.\n");
		m_ToolBox->Log( LOGFATALERROR, Mess.c_str() );
		FreeLibrary(dllHandle);
		return;
	}

	info.file = pathName.c_str();

	// get the priority function
	GetPriority = (GETPRIORITY)GetProcAddress(dllHandle, _T("GetPriority"));

	if (GetPriority == NULL)
	{
		Mess = _T("Unable to load ");
		Mess += pathName;
		Mess += _T("\nUnable to locate priority function.\n");
		m_ToolBox->Log( LOGFATALERROR, Mess.c_str() );
		FreeLibrary(dllHandle);
		return;
	}
	
	info.handle = dllHandle;	
	priority = (*GetPriority)();
	DLLINFOMAP::iterator itrDLL = m_DLLInfos.find(priority);
	if ( itrDLL == m_DLLInfos.end() )
	{
		m_DLLInfos.insert( pair<DWORD, DLLINFO>( priority, info ) );
	}
	else
	{
		StdString error;
		error = _T( __FILE__ " GetALLInterfaces(): plugin priority conflict\n" );
		assert( "GetALLInterfaces(): plugin priority conflict" && 0 );
		EngineGetToolBox()->SetErrorValue( WARN_INVALID_OPERATION );
		EngineGetToolBox()->Log( LOGWARNING, error );
	}

}

//---------------------------------------------------------------------
// Function:	InitDLLS
// Description:	This function calls all the initialize functions, now
//              that the DLLS are sorted
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
void CGFXApp::InitDLLS()
{
	DLLINFOMAP::iterator dimIter;
	DLLINFO info;

	for (dimIter = m_DLLInfos.begin(); dimIter != m_DLLInfos.end(); ++dimIter)
	{
		info = dimIter->second;
		// initialize the dll
		(*info.initDLL)();
	}
}

void CGFXApp::DeInitDLLS()
{
	DLLINFOMAP::iterator dimIter;
	DLLINFO info;

	for (dimIter = m_DLLInfos.begin(); dimIter != m_DLLInfos.end(); ++dimIter)
	{
		info = dimIter->second;
		
		// try to find the deinit address
		DEINITDLL deInit;
		deInit = (DEINITDLL)GetProcAddress(info.handle, _T("DeInitDLL"));

		if (deInit != NULL)
		{
			// it exists, so call it!
			(*deInit)();
		}
	}
}

void CGFXApp::SetMouseExclusivity(bool value)
{
#ifndef DEBUG_MOUSE	
	// make sure we don't try to send messages when the window is trying to close down
	if (!gbAppExiting)
	{
		INPUTENGPARAMS inputParams;

		// get the input
		static DWORD msgHash_GetCurrentInputMode = CHashString(_T("GetCurrentInputMode")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_GetCurrentInputMode,(DWORD)sizeof(INPUTENGPARAMS), &inputParams, NULL, &CHashString(_T("CInputManager")));
		
		// set the mouse, restart input engine
		inputParams.mouseExclusive = value;
		static DWORD msgHash_Begin = CHashString(_T("Begin")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_Begin, (DWORD)sizeof(INPUTENGPARAMS), &inputParams, NULL, &CHashString(_T("CInputManager")));	
	}
#endif
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
#ifdef WIN32
LRESULT CALLBACK CGFXApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(gMainApp->GetInstanceHandle(), (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:                                    
		PostQuitMessage(0);
		break;
	case WM_KILLFOCUS:
		SetMouseExclusivity(false);
		break;
	case WM_SETFOCUS:
		SetMouseExclusivity(true);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
LRESULT CALLBACK CGFXApp::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

LONG NTAPI QuietExitVectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
	// eesdk function call
	if (pExceptionInfo->ExceptionRecord->ExceptionCode != 0x40010006) // OutputDebugString exception
	{
		HandleException(pExceptionInfo);
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

#endif//WIN32

void TerminateApp( int iReturnCode )
{
	if (gMainApp)
	{
		gMainApp->ExitInstance();
		delete gMainApp;
	}
	exit(iReturnCode);
}

#ifdef _SHELL
void CGFXApp::InitGVRLibrary()
{
	SINGLETONINSTANCE(GameComponent)->InitGVRLibrary();
}

void CGFXApp::ShutdownGVRLibrary()
{
	SINGLETONINSTANCE(GameComponent)->ShutdownGVRLibrary();
}

void CGFXApp::WriteGameData()
{
	SINGLETONINSTANCE(GameComponent)->WriteGameData();
}

void CGFXApp::LogCmdLineParse()
{
	SINGLETONINSTANCE(GameComponent)->LogCmdLineParse();
}

void CGFXApp::InitGVRIO()
{
	SINGLETONINSTANCE(GameComponent)->InitGVRIO(m_hWnd, (MessageCallbackFN)CGFXApp::GVRIO_Callback);
}

MessageCallbackFN CGFXApp::GVRIO_Callback(sMessage *pMsg)
{
	static DWORD msgGetGVRInputMessage = CHashString(_T("GetGVRInputMessage")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgGetGVRInputMessage, sizeof(sMessage), pMsg);
	return NULL;
}

void CGFXApp::ShutdownGVRIO()
{
	SINGLETONINSTANCE(GameComponent)->ShutdownGVRIO();
}

void CGFXApp::PollGVRIO()
{
	SINGLETONINSTANCE(GameComponent)->PollGVRIO();
}

bool CGFXApp::CheckAndDecrementStartCredits()
{
	int credits = SINGLETONINSTANCE(GameComponent)->GetCreditCount();
	int startCredits = SINGLETONINSTANCE(GameComponent)->GetStartCreditCount();
	if (credits >= startCredits)
	{
		SINGLETONINSTANCE(GameComponent)->DeductCredits(startCredits);
		return true;
	}
	return false;
}
#endif

// main windows entry point
#ifdef WIN32
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
#elif defined XBOX
VOID __cdecl main()
#endif
{
#ifdef WIN32
	AddVectoredExceptionHandler( 0, QuietExitVectoredExceptionHandler );

	gMainApp = new CGFXApp();

	//Need to do some parsing of the lpCmdLine
	gMainApp->ParseCommandLine( lpCmdLine );

#ifdef _SHELL
	gMainApp->InitGVRLibrary();	

	gMainApp->InitGVRIO();

	if (gMainApp->CheckAndDecrementStartCredits())
	{
		//if (gMainApp->CheckDongle())
#endif
		{
			if (!gMainApp->InitInstance(hInstance, nCmdShow))
			{
				return FALSE;
			}
#ifdef _SHELL
			gMainApp->LogCmdLineParse();
#endif
			gMainApp->Run();
		}

#ifdef _SHELL
		gMainApp->WriteGameData();

		gMainApp->ShutdownGVRIO();

		gMainApp->ShutdownGVRLibrary();
	}
#endif//_SHELL

	TerminateApp(0);
	return 0;

#elif defined XBOX
	gMainApp = new CGFXApp();

	if (!gMainApp->InitInstance(0, 0))
	{
		TerminateApp(0);
		return;
	}
	gMainApp->Run();

	TerminateApp(0);
#endif//XBOX
}
