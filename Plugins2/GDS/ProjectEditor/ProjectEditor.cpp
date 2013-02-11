//=====================================================================
/// File:		ProjectEditor.cpp
/// Description: Defines the initialization routines for the DLL.
/// Started:		12/17/2008
/// Original:	Kenneth Hurley
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

AFX_EXTENSION_MODULE ProjectEditorDLL = { NULL, NULL };



extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("ProjectEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(ProjectEditorDLL, hInstance))
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

		new CDynLinkLibrary(ProjectEditorDLL);
		// this has to be done after module is intialized.
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("ProjectEditor.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(ProjectEditorDLL);
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
	return SINGLETONINSTANCE(CProjectEditor);
}

//---------------------------------------------------------------------
// Function:	GetPriority
// Description:	DLL's priority
// Parameters:	.
// Returns:		DWORD = priority of DLL loaded
//---------------------------------------------------------------------
DWORD CProjectEditor::GetPriority()
{
    return IDR_PROJECTTYPE;
}

//---------------------------------------------------------------------
// Function:	CProjectEditorr
// Description:	Standard Constructor
// Parameters:	pi = program's information structure
// Returns:		.
//---------------------------------------------------------------------
CProjectEditor::CProjectEditor()
{
	SINGLETONINSTANCE(CProjectEditorComponent)->SetParent(this);
	m_ToolBox = EngineGetToolBox();
	registeredComponents = new vector<IComponent*>();
}

///---------------------------------------------------------------------
/// Function:	GetName 
/// Description:	DLL's module name
/// Returns:		CString * = pointer to name of loaded DLL
///---------------------------------------------------------------------
CString *CProjectEditor::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(ProjectEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	Initialization like InitInstace
// Parameters:	.
// Returns:		TRUE, if successful
//---------------------------------------------------------------------
BOOL CProjectEditor::Initialize()
{
	// get the Settings object (singleton)
	m_projectSettings = SINGLETONINSTANCE( CProjectSettings );

	return TRUE;
}


//---------------------------------------------------------------------
// Function:	OnCreate
// Description:	Called after object creation. 
// Parameters:	mainWnd - the parent window
// Returns:		void
//---------------------------------------------------------------------
void CProjectEditor::OnCreate( CMDIFrameWnd* mainWnd )
{
	// get the apps instance handle for loading resources
	HINSTANCE appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(ProjectEditorDLL.hModule);

	// get the current menu information
	MENUINFOMESSAGE menuInfo;
	static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE), &menuInfo) != MSG_HANDLED)
	{
		// error handling
		ASSERT(0);
		AfxSetResourceHandle(appInst);
		return;
	}
    
	// load the new menu elements
	m_hMenuFile = LoadMenu(ProjectEditorDLL.hModule, MAKEINTRESOURCE(IDR_PROJECTTYPE_FILE));
	m_hMenuTools = LoadMenu(ProjectEditorDLL.hModule, MAKEINTRESOURCE(IDR_PROJECTTYPE_TOOLS));
	m_hMenuFileCloseProject = LoadMenu(ProjectEditorDLL.hModule, MAKEINTRESOURCE(IDR_PROJECTTYPE_FILE_CLOSE));

	// load the new accelerators
	m_hAccel = LoadAccelerators(ProjectEditorDLL.hModule, MAKEINTRESOURCE(IDR_PROJECTTYPE_FILE));
	
	// merge the prepared data into the current menu
	VERIFY( MergeSubMenus( &m_hMenuFile, menuInfo.m_hMenu ) );
	VERIFY( MergeSubMenus( &m_hMenuTools, m_hMenuFile ) );
	VERIFY( MergeSubMenus( &m_hMenuFileCloseProject, m_hMenuTools ) );
	
	VERIFY( MergeAccelerators( &m_hAccel, menuInfo.m_hAccel ) );

	// set the message elements with the new updated data
	menuInfo.m_hMenu = m_hMenuFileCloseProject;
	menuInfo.m_hAccel = m_hAccel;
	menuInfo.m_MenuRsrc = NULL;
	menuInfo.m_MenuBitmap = NULL;

	// send back the message
	static DWORD msgHash_SetMenuInfo = CHashString(_T("SetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetMenuInfo, sizeof(MENUINFOMESSAGE), &menuInfo);

	AfxSetResourceHandle(appInst);

	//////////////////////////////////////////////////////////////////////
	// The menu is ready, let's deal with the component functionality...


	// create the property sheet
	LPCTSTR caption = "";  
	m_projectEditorPropertySheet = new CProjectEditorPropertySheet( caption, mainWnd );

	// create the base property page
	m_projectEditorBasePropertyPage = new CProjectEditorDefaultPropertyPage();

	// add the default page to the sheet
	m_projectEditorPropertySheet->AddPage( m_projectEditorBasePropertyPage );

	// modeless options dialog, not visible
	if (!m_projectEditorPropertySheet->Create(mainWnd, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_EX_DLGMODALFRAME | DS_MODALFRAME))
	{
		// no pages causes creation to fail
		delete m_projectEditorBasePropertyPage;
		m_projectEditorPropertySheet = NULL;
		m_ToolBox->Log( LOGERROR, _T("%s(%i):CProjectEditor: The property sheet has no pages. This results that it can't be set visible."), __FILE__, __LINE__);
	
	}
}


//---------------------------------------------------------------------
// Function:	OnCmdMsg
// Description:	Message handler to alter the default message paths. 
//				We intend to handle the ID_NEW_PROJECT message. 
// Parameters:	nID				Contains the command ID. 
//				nCode			Identifies the command notification code. See Remarks for more information about values for nCode. 
//				pExtra			Used according to the value of nCode. See Remarks for more information about pExtra. 
//				pHandlerInfo	If not NULL, OnCmdMsg fills in the pTarget and pmf members of the pHandlerInfo structure instead of dispatching the command. Typically, this parameter should be NULL. 
// Returns:		void
//---------------------------------------------------------------------

BOOL CProjectEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	int nMsg = 0;

	if (nCode != CN_UPDATE_COMMAND_UI)
	{
		nMsg = HIWORD(nCode);
		nCode = LOWORD(nCode);
	}

	// for backward compatibility HIWORD(nCode)==0 is WM_COMMAND
	if (nMsg == 0)
		nMsg = WM_COMMAND;

	// Let's handle the NEW PROJECT and PROJECT SETTINGS menu buttons
	if ( ( nID == ID_NEW_PROJECT )
			&& (nMsg == WM_COMMAND) && (nCode == CN_COMMAND))
	{
		// if the pHandlerInfo is not NULL, MFC just wants it filled in
		if (pHandlerInfo == NULL)
		{
			// we are the addressee, we should handle the message
			DisplayProjectSettingsDialog(nID == ID_NEW_PROJECT);
		}
		//menu update: return true to enable menu item
		return TRUE;
	} 
	else if ( ( nID == ID_TOOLS_PROJECT_SETTINGS ) 
			&& (nMsg == WM_COMMAND) && (nCode == CN_COMMAND) )
	{
		// if the pHandlerInfo is not NULL, MFC just wants it filled in
		if (pHandlerInfo == NULL)
		{
			// we are the addressee, we should handle the message
			// display prj settings without creating a new one (false)
			DisplayProjectSettingsDialog( FALSE );
		}
		// if we are here - menu update: return true to enable menu item
		// enable change settings only if settings are consistent
		
		// Check for consistency
		// if consistent - enable functionality, return false if not.
		if( ( m_projectSettings != NULL ) && ( m_projectSettings->IsConsistentState() ) )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	// let's handle the project close button
	else if ( ( nID == ID_FILE_CLOSEPROJECT )
			&& (nMsg == WM_COMMAND) && (nCode == CN_COMMAND))
	{
		// get the Settings object (singleton)
		CProjectSettings *projectSettings = SINGLETONINSTANCE( CProjectSettings );

		// Check for consistency
		// if consistent - enable functionality, return false if not.
		if( projectSettings->IsConsistentState() )
		{
			// if the pHandlerInfo is not NULL, MFC just wants it filled in
			if (pHandlerInfo == NULL)
			{
				// we are the addressee, we should handle the message
				// close the project settings
				projectSettings->CloseProjectSettings();
				return TRUE;
			} 
			// not a menuclick, just a menu update. return true to enable menu
			return TRUE;
		}
		else
		{
			// not in consistent state -> disable menu button
			return FALSE;
		}
	}
	else
	{
		// we did not handle this message, returning false:
		return FALSE;
	}
}

//---------------------------------------------------------------------
// Function:	~CProjectEditorr
// Description:	Standard Desctructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CProjectEditor::~CProjectEditor()
{
	if ( m_projectEditorPropertySheet != NULL )
	{
		delete m_projectEditorPropertySheet;
		m_projectEditorPropertySheet = NULL;
	}

	if ( m_projectEditorBasePropertyPage != NULL )
	{
		delete m_projectEditorBasePropertyPage;
		m_projectEditorBasePropertyPage = NULL;
	}

	if ( registeredComponents != NULL)
	{
		delete registeredComponents;
		registeredComponents = NULL;
	}
}


DWORD CProjectEditor::DisplayProjectSettingsDialog(bool allowNew)
{
	if( m_projectEditorPropertySheet != NULL )
	{
		// if the new button was clicked and not the edit button
		if( allowNew )
		{
			// this restores the original values
			m_projectSettings->Init();
		}

		// make the property sheet refresh every property page
		m_projectEditorPropertySheet->RefreshPageValues();

		// if the values are correct, we should enable all the inputs on the page
		if( m_projectSettings != NULL && m_projectEditorBasePropertyPage != NULL )
		{
			m_projectEditorBasePropertyPage->EnableInputs( m_projectSettings->IsConsistentState() );
		}
		// show the form
		m_projectEditorPropertySheet->SetModelessProperty( FALSE );
		m_projectEditorPropertySheet->ShowWindow( SW_SHOW );
		m_projectEditorPropertySheet->BringWindowToTop();
		m_projectEditorPropertySheet->SetWindowPos( &CWnd::wndTop, 0, 0 , 0 , 0, SWP_NOMOVE | SWP_NOSIZE );
		m_projectEditorPropertySheet->EnableWindow();
		m_projectEditorPropertySheet->RunModalLoop(MLF_SHOWONIDLE);
	}
	else
	{
			m_ToolBox->Log( LOGERROR, _T("%s(%i):CProjectEditor: Displaying project settings failed. The property sheet is an invalid pointer(NULL)."), __FILE__, __LINE__);
	}

	return NO_ERROR;
}

DWORD CProjectEditor::AddProjectSettingsPropertyPage( IProjectSettingsPropertyPage* p_newPage )
{
	if ( p_newPage != NULL )
	{
		// add the page
		m_projectEditorPropertySheet->AddPage(p_newPage);

		// add its owner to our 'registered components' list
		registeredComponents->push_back( p_newPage->ownerComponent );
	}
	else
	{
		// log a warning
		m_ToolBox->Log(LOGERROR, _T("%s(%i):CProjectEditor: Adding external property sheet pages failed!\nThe page (incomming parameter) is NULL!"), __FILE__, __LINE__);
	}
	return TRUE;
}

void CProjectEditor::Serialize(IArchive &ar)
{
	// call the serialize function of every registere component
	for( unsigned int i = 0; i < registeredComponents->size(); i++ )
	{
		registeredComponents->at(i)->Serialize(ar);
	}
}

BOOL CProjectEditor::AutoLoadProjectSettings()
{
	// handling the last project path
	StdString  lastProjectFilePath = _T("");
	StdString autoloadLastProjectFile = "";

	GDS_SDK_Helpers::ReadINIKey(  INI_LastProjectPath, lastProjectFilePath, INI_ProjectSettings_SubSection );
	GDS_SDK_Helpers::ReadINIKey(  INI_AutoLoadLastProject, autoloadLastProjectFile, INI_ProjectSettings_SubSection, _T("1") );

	// if autoLoad is 1 and there is a path stored to load
	if( ( autoloadLastProjectFile.compare( _T("1") ) == 0 ) && ( lastProjectFilePath.compare( _T("") ) != 0 ) )
	{
		// to import a .gdsprj send the message
		LOADFILEEXTPARAMS lfep;
		StdString fileName = lastProjectFilePath;
		fileName.MakeSafeFileName();
		lfep.fileName = (TCHAR*)fileName.c_str();
		lfep.bInternalLoad = false;

		// attempt to load the selected file
		static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		DWORD retVal = m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(lfep), &lfep);

		if ( retVal != MSG_HANDLED)
		{
			//Log: autoload failed.
			assert(0);
			return FALSE;
		}
		return TRUE;

	}
	return FALSE;
}