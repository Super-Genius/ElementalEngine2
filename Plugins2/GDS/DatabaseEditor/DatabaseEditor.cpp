//=====================================================================
// File:		DatabaseEditor.cpp
// Description: Defines the initialization routines for the DLL.
// Started:		10/31/2001
// Original:	Kenneth Hurley
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

#define DATABASE_EXTENSIONS "dxl\\dbx\\fxl"

AFX_EXTENSION_MODULE DatabaseEditorDLL = { false, NULL, NULL };


extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("DatabaseEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(DatabaseEditorDLL, hInstance))
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

		new CDynLinkLibrary(DatabaseEditorDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("DatabaseEditor.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(DatabaseEditorDLL);
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
	return SINGLETONINSTANCE(CDatabaseEditor);
}

//---------------------------------------------------------------------
// Function:	GetPriority
// Description:	DLL's priority
// Parameters:	.
// Returns:		DWORD = priority of DLL loaded
//---------------------------------------------------------------------
DWORD CDatabaseEditor::GetPriority()
{
    return IDR_DATABASETYPE;
}

//---------------------------------------------------------------------
// Function:	CDatabaseEditor
// Description:	Standard Constructor
// Parameters:	pi = programs information structure
// Returns:		.
//---------------------------------------------------------------------
CDatabaseEditor::CDatabaseEditor()
{
	SINGLETONINSTANCE(CDatabaseComponent)->SetParent(this);
	m_ToolBox = EngineGetToolBox();
	if (!m_ControlSchema.ReadXML(_T("DataBaseProperties\\Databases.fxl")))
	{
		MessageBox(NULL, _T("Warning database schema not found\n"
							"no databases will be avaliable."), _T("Warning"),
							MB_OK);
	}
}

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CDatabaseEditor::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(DatabaseEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	DLL's Initialization function, like InitInstance
// Parameters:	.
// Returns:		TRUE, if successful.
//---------------------------------------------------------------------
BOOL CDatabaseEditor::Initialize()
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
	AfxSetResourceHandle(DatabaseEditorDLL.hModule);

	// use my string / icon resources
	// Note IDR_DATABASETYPE has to be a unique id even between DLLS
	// to get the ICONS right on the Window
	m_DocTemplate = new CMultiDocTemplate(IDR_DATABASETYPE,
			RUNTIME_CLASS(CDatabaseDoc),
			RUNTIME_CLASS(CDatabaseFrm),
			RUNTIME_CLASS(CDatabaseView));

	// put this after setting the menu and accelerators so it won't allocate it's own
	pApp->AddDocTemplate(m_DocTemplate);

	// construct resource info
	m_ObjCollection.name.LoadString(IDS_RESOURCE_NAME);
	m_ObjCollection.pageIndex = &m_PageIndex;

	m_ObjCollection.hIcon = (HICON)LoadImage(DatabaseEditorDLL.hModule, MAKEINTRESOURCE(IDR_DATABASETYPE), 
		IMAGE_ICON, 0, 0, LR_CREATEDIBSECTION);
	VERIFY(m_ObjCollection.hIcon != NULL);

	// value should be 1 - 1000
	m_ObjCollection.priority = IDR_DATABASETYPE;
	m_ObjCollection.extensions = DATABASE_EXTENSIONS;
	AfxSetResourceHandle(appInst);

    return TRUE;
}

//---------------------------------------------------------------------
// Function:	AddSchema
// Description:	Checks to see if the schema exists in the engine and forwards the archive to the control schema
// Parameters:	schema = Name of the component
// Returns:		TRUE, if successful.
//---------------------------------------------------------------------
BOOL CDatabaseEditor::AddSchema( IHashString *schema )
{
	BOOL bRetVal = false;

	// TODO <rnguyen>:	This is temporary since all schema items for CDefinedDatabase are
	//					treated as edit items. Once we have in place spinner, combo, etc.
	//					schema item types, we can just hook in the OnGetSchema message for
	//					each one to correctly create the archive.
	if( schema != NULL )
	{
		CHashString streamType(_T("Memory"));	// type of archive to create
		CREATEARCHIVE ca;
		ca.streamSize = 0;
		ca.streamData = NULL;
		ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
		ca.streamType = &streamType;
		static DWORD msgHash_CreateXMLArchiveStream = CHashString(_T("CreateXMLArchiveStream")).GetUniqueID();
		DWORD retVal = m_ToolBox->SendMessage(msgHash_CreateXMLArchiveStream, sizeof(CREATEARCHIVE), &ca);
		if (retVal != MSG_HANDLED)
		{
			m_ToolBox->Log( LOGERROR, _T("%s(%i): Failed to create archive."), __FILE__, __LINE__ );
			return false;
		}
		IXMLArchive *MemArchive = (IXMLArchive*)ca.archive;
		MemArchive->SetIsWriting( true );

		GETSCHEMAPARAMS schemaparams;
		schemaparams.ar = MemArchive;
		static DWORD msgHash_GetSchema = CHashString(_T("GetSchema")).GetUniqueID();
		if( m_ToolBox->SendMessage(msgHash_GetSchema, sizeof(schemaparams), &schemaparams, schema ) == MSG_HANDLED)
		{
			MemArchive->SetIsWriting( false );
			if( m_ControlSchema.AddSchemaFromArchive( schema->GetString(), MemArchive ) != NULL )
			{
				bRetVal = true;
			}
		}
		MemArchive->Close();
	}

	return bRetVal;
}

//---------------------------------------------------------------------
// Function:	~CDatabaseEditor
// Description:	Standard Destructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CDatabaseEditor::~CDatabaseEditor()
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
void CDatabaseEditor::OnCreate(CMDIFrameWnd *mainWnd)
{
	MENUINFOMESSAGE mim;
	CBitmap menuBitmap;
	HINSTANCE appInst;

	static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE),&mim);

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(DatabaseEditorDLL.hModule);

	// we need to mergeMenus
	VERIFY(MergeMenus(&m_DocTemplate->m_hMenuShared, mim.m_hMenu, _T("&Window")));	
	// and acclerators
	VERIFY(MergeAccelerators(&m_DocTemplate->m_hAccelTable, mim.m_hAccel));

	mim.m_hAccel = NULL;
	mim.m_hMenu = NULL;
	// load up hi-res toolbar icon
	menuBitmap.LoadBitmap(IDB_TOOLBAR);
	// we don't really have to have to toolbar visible do we?
	mim.m_MenuRsrc = IDR_DATABASETYPE;
	mim.m_MenuBitmap = &menuBitmap;

	static DWORD msgHash_SetMenuInfo = CHashString(_T("SetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetMenuInfo, sizeof(MENUINFOMESSAGE),&mim);

	static DWORD msgHash_RegisterResourceCollection = CHashString(_T("RegisterResourceCollection")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterResourceCollection, sizeof(RESOURCECOLLECTION), &m_ObjCollection);

	COMPTYPETOVOLISTMAP *compTypes;
	COMPTYPETOVOLISTMAP::iterator cttvlmIter;

	compTypes = m_ControlSchema.GetComponentTypeNames();
	// register all the database types as creatable
	for (cttvlmIter = compTypes->begin(); cttvlmIter != compTypes->end(); ++cttvlmIter)
	{
		//register our filetype
		FILETYPECREATIONINFO fileInfo;
		fileInfo.m_Description = m_ControlSchema.GetComponentName(cttvlmIter->first);
		fileInfo.m_CreationCallback = CDatabaseEditor::DatabaseCreationCallback;
		fileInfo.m_UserData = (void *)(&cttvlmIter->first);
		static DWORD msgHash_RegisterFileTypeCreationInfo = CHashString(_T("RegisterFileTypeCreationInfo")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RegisterFileTypeCreationInfo, sizeof( FILETYPECREATIONINFO ),&fileInfo);
	}


	AfxSetResourceHandle(appInst);
}

//---------------------------------------------------------------------
// Function:	OnCmdMsg
// Description:	Called from the main windows OnCmdMsg function.
//				Routes commands through doc/view architecture
// Returns:		TRUE if handled
//---------------------------------------------------------------------
BOOL CDatabaseEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return FALSE;
}

//---------------------------------------------------------------------
// Function:	EditDocument
// Description:	Edit an Database Document 
// Parameters:	lpszPathName pointer to name of document already loaded
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CDatabaseEditor::EditDocument(LPCTSTR lpszPathName)
{	
	// if we have a valid path
	if (lpszPathName != NULL)
	{
		TCHAR szFullPath[_MAX_PATH];
		// get the full path to use for comparison
		AfxFullPath(szFullPath, lpszPathName);
		if (ActivateOpenedDocument(m_DocTemplate, szFullPath))
		{
			return NO_ERROR;
		}
	}

	// if the file isn't already open then create blank document, with frame
	if (m_DocTemplate->OpenDocumentFile(lpszPathName) != NULL)
	{
		CFrameWnd *mainWnd = DYNAMIC_DOWNCAST(CFrameWnd, AfxGetMainWnd());

		// first time through dock the toolbar
		if (m_DocTemplate->GetFirstDocPosition() != NULL)
		{
			if (IsWindow(m_DatabaseToolPal))
			{
				m_DatabaseToolPal.ShowWindow(SW_SHOW);
			}
			else
			{
				if (!m_DatabaseToolPal.Create(mainWnd, CDatabaseToolPal::IDD,
					CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY, CDatabaseToolPal::IDD))
				{
					TRACE0("Failed to create toolbar\n");
					return 1;      // fail to create
				}
				m_DatabaseToolPal.EnableDocking(CBRS_ORIENT_ANY);
				// make control bar notify us.
				mainWnd->DockControlBar(&m_DatabaseToolPal, CBRS_ALIGN_TOP);
			}
		}
	}
	
	return NO_ERROR;
}

void CDatabaseEditor::HideToolbar()
{
	// if there aren't anymore docs open
	if (m_DocTemplate->GetFirstDocPosition() == NULL)
	{
		if (IsWindow(m_DatabaseToolPal))
		{
			// hide the tool tab bar
			m_DatabaseToolPal.ShowWindow(SW_HIDE);
		}
	}
}

//---------------------------------------------------------------------
// Function:	DatabaseCreationCallback
// Description:	Create a new database file for editing 
// Returns:		void
//---------------------------------------------------------------------
void CDatabaseEditor::DatabaseCreationCallback(void *userData)
{
	CDatabaseEditor * pDatabaseEditorInstance = SINGLETONINSTANCE(CDatabaseEditor);
	if( pDatabaseEditorInstance )
	{
		pDatabaseEditorInstance->SetCurrentSchema((StdString *)userData);
		// set the schema to use for this database
		pDatabaseEditorInstance->EditDocument(NULL);
	}
}

