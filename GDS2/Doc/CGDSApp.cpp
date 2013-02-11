//-----------------------------------------------------------------------------
//
// File:          CGDSApp.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CGDSApp'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'StaticBroadcastMessage'
*     Added method 'SearchDLLS'
*     Added method 'OnOpenRecentFile'
*     Added method 'OnIdle'
*     Added method 'OnFileOpen'
*     Added method 'OnAppAbout'
*     Added method 'LoadDLLS'
*     Added method 'InitInstance'
*     Added method 'InitAllDLLS'
*     Added method 'GetRecentFileList'
*     Added method 'GetFilterStrLength'
*     Added method 'GetALLInterfaces'
*     Added method 'FindExtensionMatch'
*     Added method 'ExitInstance'
*     Added method 'BuildFilterLists'
*     Added method 'BroadcastMessage'
*     Added method '~CGDSApp'
*     Added method 'CGDSApp'
*     Added inheritance 'CWinApp'
*     Added member 'm_CurAccel'
*     Added member 'm_CurMenu'
*     Added member 'm_ProgramInfo'
*     Added member 'm_DLLHandles'
*     Added member 'm_DLLS'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

//@START_USER1
//@END_USER1


// Master include file
#include "StdAfx.h"


//@START_USER2
//@END_USER2




/*@NOTE_3619
Constructor method.
*/
CGDSApp::CGDSApp() //@INIT_3619
{//@CODE_3619

	m_ProgramInfo = NULL;

	m_ProgramInfo = new CProgramInfo(CGDSApp::StaticBroadcastMessage);
	ASSERT(m_ProgramInfo != NULL);
	m_ProgramInfo->curMenu = NULL;
	m_CurMenu = NULL;
	m_ProgramInfo->curAccel = NULL;
	m_CurAccel = NULL;
	sprintf(OpenFileName, "*.*");
	Ofn.nFilterIndex = 1;
    gLogger = NULL;

	// Set to get a break event when tracing memory leaks
	//_CrtSetBreakAlloc(8896);
}//@CODE_3619


/*@NOTE_3613
Destructor method.
*/
CGDSApp::~CGDSApp()
{//@CODE_3613
    DestructorInclude();

    // Put in your own code
}//@CODE_3613


/*@NOTE_3642
BroadcastMessage is used to send message to all plug-ins.  The plug-ins get a pointer
to the static implementation of this function, which in turn calls this function through the 
main applications class.
*/
BOOL CGDSApp::BroadcastMessage(UINT nID, void* pExtra)
{//@CODE_3642

	STDDLLLIST::iterator DLLit;

	// First try the resources
	for (DLLit = m_DLLS.begin(); DLLit != m_DLLS.end(); DLLit++)
	{
		CDLLInterface *di;
		di = (CDLLInterface *)*DLLit;

		if (di->BroadcastDLLMessage(nID, pExtra))
			return TRUE;

	}
	
	CMainFrame *mainFrm;

	mainFrm = DYNAMIC_DOWNCAST(CMainFrame, m_pMainWnd);
	// possible message to windows
	if (mainFrm->BroadcastMessage(nID, 0, (LPARAM)pExtra))
		return TRUE;

	return FALSE;
}//@CODE_3642


/*@NOTE_3635
This function builds a filter list, which is a list of filters that each plug-in says it is interested in. 
These are the extension names and the file filter lists such as *.jpg.
*/
void CGDSApp::BuildFilterLists()
{//@CODE_3635

	STDDLLLIST::iterator DLLit;
	DWORD openFilterSize;
	DWORD saveFilterSize;
	DWORD tLen;
	LPTSTR tOStr;
	LPTSTR tSStr;
		
	openFilterSize = 0;
	saveFilterSize = 0;
    for (DLLit = m_DLLS.begin(); DLLit != m_DLLS.end(); DLLit++)
	{
		CDLLInterface *di;

		di = (CDLLInterface *)*DLLit;

		// get length of open/import filters
		openFilterSize += GetFilterStrLength(di->m_LoadStrings);
		// and save/export filters
		saveFilterSize += GetFilterStrLength(di->m_SaveStrings);

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

	for (DLLit = m_DLLS.begin(); DLLit != m_DLLS.end(); DLLit++)
	{
		CDLLInterface *di;

		di = (CDLLInterface *)*DLLit;

		// get length of open/import filters
		tLen = GetFilterStrLength(di->m_LoadStrings);
		memcpy(tOStr, di->m_LoadStrings, tLen);
		tOStr += tLen;

		// get length of open/import filters
		tLen = GetFilterStrLength(di->m_SaveStrings);
		memcpy(tSStr, di->m_SaveStrings, tLen);
		tSStr += tLen;

	}

	// append 2 \0's to open file filter string
	*tOStr++ = '\0';
	*tOStr++ = '\0';

	// append 2 \0's to save file filter string
	*tSStr++ = '\0';
	*tSStr++ = '\0';
}//@CODE_3635


int CGDSApp::ExitInstance()
{//@CODE_3659

	HANDLELIST::iterator itHandle;

	delete m_ProgramInfo->ObjectManager;

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

    if (gLogger != NULL)
    {
        gLogger->Release();
		gLogger->Shutdown();
        gLogger = NULL;
    }

	if (m_ProgramInfo != NULL)
	{
		delete m_ProgramInfo;
		m_ProgramInfo = NULL;
	}

	return CWinApp::ExitInstance();
}//@CODE_3659


/*@NOTE_3636
This function searchs the plugins file filter extensions to find a match for the extenstion of 
the file the user is trying to load.  The match returns a pointer to the DLL that handles
the type of file that is trying to be loaded
*/
CDLLInterface* CGDSApp::FindExtensionMatch(LPTSTR ext, BOOL isLoad)
{//@CODE_3636
    CDLLInterface* value;

    return value;
}//@CODE_3636


void CGDSApp::GetALLInterfaces(const CString& pathName, DLLTYPE type)
{//@CODE_3649

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
		Mess.Format(_T("Error Loading DLL:%s\n%s\n"), pathName, lpMsgBuf);
		MessageBox(NULL, Mess, "Error", MB_OK | MB_ICONINFORMATION );
		// Free the buffer.
		LocalFree( lpMsgBuf );
		return;
	}

	// make sure this dll is ok
	GetDLLVersion = (GETDLLVERSION)GetProcAddress(dllHandle, _T("GetDLLVersion"));
	GetDLLInterface = (GETDLLINTERFACE)GetProcAddress(dllHandle, _T("GetDLLInterface"));
	if ((GetDLLVersion == NULL) || (GetDLLInterface == NULL))
	{
		Mess.Format(_T("Unable to load %s\n"
			"Unable to locate either init or version function.\n"), pathName);
		MessageBox(NULL, Mess, _T("DLL Load Error!"), MB_OK | MB_ICONINFORMATION);
		AfxFreeLibrary(dllHandle);
		return;
	}

	version = (*GetDLLVersion)();
	if (version != INTERFACEDLLVERSION)
	{
		Mess.Format(_T("Unable to load %s\n"
			"version %x doesn't match expected %x.\n"), pathName, version, INTERFACEDLLVERSION);
		MessageBox(NULL, Mess, _T("DLL Load Error!"),MB_OK | MB_ICONINFORMATION);
		FreeLibrary(dllHandle);
		return;
	}

	ri = GetDLLInterface(m_ProgramInfo);
	if (ri == NULL)
	{
		Mess.Format(_T("Unable to load %s\n"
			"GetDLLInterface routine failed.\n"), pathName);
		MessageBox(NULL, Mess, _T("DLL Load Error!"), MB_OK | MB_ICONINFORMATION);
		AfxFreeLibrary(dllHandle);
		return;
	}

    if (ri->m_Priority == 0)
    {
        gLogger->Print(_T("Priority should have been set in %s::GetDLLInterface()"),
            pathName);
    }

    ri->m_Type = type;
	m_DLLHandles.push_back(dllHandle);
	m_DLLS.push_back(ri);
}//@CODE_3649


/*@NOTE_3632
This function is used to get the total filter string lengths for all the plugins.  This is done so
that the open dialog box can use all the extensions and filters.
*/
DWORD CGDSApp::GetFilterStrLength(LPTSTR filterStr)
{//@CODE_3632

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
}//@CODE_3632


/*@NOTE_3621
The GetRecentFileList function returns a list of recently loaded files so the user can
quickly choose from a recently used file list.
*/
CRecentFileList* CGDSApp::GetRecentFileList()
{//@CODE_3621
    CRecentFileList* value;

    return value;
}//@CODE_3621


/*@NOTE_3654
After the DLLS have been determined and the priorties sorted, each of the DLLS is called
in order of priority to initialize themselves.
*/
void CGDSApp::InitAllDLLS()
{//@CODE_3654

    STDDLLLIST::iterator itDLL;
    CDLLInterface *pDLL;

    for (itDLL = m_DLLS.begin(); itDLL != m_DLLS.end(); itDLL++)
    {
        pDLL = (*itDLL);
        if (!pDLL->Initialize())
        {
            gLogger->Print(_T("%s DLL failed to initialized."),
                (LPCSTR)pDLL->GetName());
        }
    }
}//@CODE_3654


BOOL CGDSApp::InitInstance()
{//@CODE_3658

	// change to the path so dlls can load dlls directory
	TCHAR curDir[_MAX_PATH+1];

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
	AfxInitRichEdit();
	
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Signature Devices, Inc"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	GetCurrentDirectory(_MAX_PATH, curDir);
	m_ProgramInfo->AppDirectory = curDir;
	
	// we need menus/accelerators preloaded
	m_CurMenu = LoadMenu(m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
	m_ProgramInfo->curMenu = &m_CurMenu;
	m_CurAccel = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
	m_ProgramInfo->curAccel = &m_CurAccel;

	m_ProgramInfo->ObjectManager = new CObjectManager(m_ProgramInfo);

    // enable logger class
	CLogger::Initialize();

	gLogger = CLogger::Attach();
	if( gLogger == NULL )
	{
		TRACE0("Could not attach to CLogger obejct!\n");
	}
    else
    {
        gLogger->SetFormat( LOG_PROCNAME | LOG_TIMESTAMP | LOG_THREADID );
    }

	// Load in base DLLS into memory
	LoadDLLS();

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

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}//@CODE_3658


/*@NOTE_3648
The LoadDLLS function calls the searchDLLS function with base DLLS and then 
Object DLLS, so that base DLLS will get loaded first.  Base DLLS have a higher
priority than the Object DLLS.
*/
void CGDSApp::LoadDLLS()
{//@CODE_3648

	// for now hardcode path
	SearchDLLS(CString(_T(".\\PlugIns\\*.dlb")), DLLTYPE_BASE);

	SearchDLLS(CString(_T(".\\PlugIns\\*.dlo")), DLLTYPE_OBJECT);

	m_DLLS.sort(lessThan());

    InitAllDLLS();

}//@CODE_3648


afx_msg void CGDSApp::OnAppAbout()
{//@CODE_3664

	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}//@CODE_3664


/*@NOTE_3665
The OnFileOpen function was overridden so that the main application can match extensions
with the plugin DLLs and then pass the file open command to the correct plugin DLL.
*/
afx_msg void CGDSApp::OnFileOpen()
{//@CODE_3665

	LPTSTR tExt;
	CDLLInterface *ri;

	if (OpenFilter == NULL)
	{
		BuildFilterLists();
	}

	Ofn.lpstrFilter = OpenFilter;
	Ofn.lpstrFile = OpenFileName;
	Ofn.lpstrTitle = "Open File";
	Ofn.hwndOwner = GetMainWnd()->m_hWnd;
	Ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	if (!GetOpenFileName(&Ofn))
	{
		// cancel selected.
		return;
	}

	// figure out type of resource wanted, by extension
	// get last . to find start of extension
	tExt = 	_tcsrchr(Ofn.lpstrFile, '.');

	// if not found, then no extension
	if ((tExt == NULL) || ((ri = FindExtensionMatch(tExt, TRUE)) == NULL))
	{
		MessageBox(GetMainWnd()->m_hWnd,_T("Unable to match extension to resource type.\n"), _T("Extension Error!"),
			MB_OK);
		return;
	}

	// Load/Create new resource
	ri->Load(Ofn.lpstrFile);		
}//@CODE_3665


/*@NOTE_3660
OnIdle is being used to call each of the DLLS that have registered, so that they can
update anything that they need to.  This can be a good time for RenderDLLS to render
the 3D scene
*/
BOOL CGDSApp::OnIdle(LONG lCount)
{//@CODE_3660

	BOOL bMore = CWinApp::OnIdle(lCount);

	STDDLLLIST::iterator DLLit;

	// Allow the render DLL's to request Idle time
	DLLit = m_DLLS.begin();
	while (DLLit != m_DLLS.end())
	{
 		CDLLInterface *di;
 		di = (CDLLInterface *)*DLLit;

 		bMore |= di->OnIdle(lCount);

 		DLLit++;
	}

	return bMore;
}//@CODE_3660


/*@NOTE_3639
The OnOpenRectFile function search through the DLLS and extensions specified to pass
the plugin DLL the name of the file to load.
*/
BOOL CGDSApp::OnOpenRecentFile(UINT nID)
{//@CODE_3639

	CRecentFileList *appRFL;
	LPTSTR tExt;
	CDLLInterface *ri;

	appRFL = GetRecentFileList();
	ASSERT(appRFL != NULL);

	ASSERT(nID >= ID_FILE_MRU_FILE1);
	ASSERT(nID < ID_FILE_MRU_FILE1 + (UINT)appRFL->GetSize());
	int nIndex = nID - ID_FILE_MRU_FILE1;
	ASSERT((*appRFL)[nIndex].GetLength() != 0);

	TRACE2("MRU: open file (%d) '%s'.\n", (nIndex) + 1,
			(LPCTSTR)(*appRFL)[nIndex]);

	// figure out type of resource wanted, by extension
	// get last . to find start of extension
	tExt = 	_tcsrchr((*appRFL)[nIndex], '.');

	// if not found, then no extension
	if ((tExt == NULL) || ((ri = FindExtensionMatch(tExt, TRUE)) == NULL))
	{
		MessageBox(GetMainWnd()->m_hWnd,_T("Unable to match extension for MRU file type.\n"), _T("Extension Error!"),
			MB_OK);
		return FALSE;
	}

	// Load/Create new resource
	if (!ri->Load((LPTSTR)(LPCTSTR)(*appRFL)[nIndex]))
		appRFL->Remove(nIndex);

	return TRUE;
}//@CODE_3639


/*@NOTE_3655
The SearchDLLS searchs a certain directory for DLLs of a known type.  These are determined
by the extension of the DLL and also a Magic Value and interface versions.
*/
void CGDSApp::SearchDLLS(const CString& pathName, DLLTYPE type)
{//@CODE_3655

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

}//@CODE_3655


/*@NOTE_3645
One way to have callbacks in C++ is to make the function static.  We do this so that DLLS
can make callbacks to the application and use the message sending functions.
*/
BOOL CGDSApp::StaticBroadcastMessage(UINT nID, void* pExtra)
{//@CODE_3645

	return theApp.BroadcastMessage(nID, pExtra);
}//@CODE_3645


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3614
Method which must be called first in a constructor.
*/
void CGDSApp::ConstructorInclude()
{
}


/*@NOTE_3615
Method which must be called first in a destructor.
*/
void CGDSApp::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
