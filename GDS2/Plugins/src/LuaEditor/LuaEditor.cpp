//=====================================================================
// File:		LuaEditor.cpp
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

#define LUA_EXTENSIONS "lua"

#define LUA_FILE_DESCRIPTION _T("Lua File")

AFX_EXTENSION_MODULE LuaEditorDLL = { false, NULL, NULL };

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("LuaEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(LuaEditorDLL, hInstance))
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

		new CDynLinkLibrary(LuaEditorDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("LuaEditor.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(LuaEditorDLL);
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
	return SINGLETONINSTANCE(CLuaEditor);
}

static LPCTSTR OpenFilter = {
	"Lua Files (*.lua)\0" "*.lua\0"
	"\0\0"
};

static LPCTSTR SaveFilter = {
	"Lua Files (*.lua)\0" "*.lua\0"
	"\0\0"
};

//---------------------------------------------------------------------
// Function:	GetPriority
// Description:	DLL's priority
// Parameters:	.
// Returns:		DWORD = priority of DLL loaded
//---------------------------------------------------------------------
DWORD CLuaEditor::GetPriority()
{
    return IDR_LUATYPE;
}

//---------------------------------------------------------------------
// Function:	CLuaEditor
// Description:	Standard Constructor
// Parameters:	pi = programs information structure
// Returns:		.
//---------------------------------------------------------------------
CLuaEditor::CLuaEditor()
{
	SINGLETONINSTANCE(CLuaComponent)->SetParent(this);
	m_ToolBox = EngineGetToolBox();
}

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CLuaEditor::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(LuaEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	DLL's Initialization function, like InitInstance
// Parameters:	.
// Returns:		TRUE, if successful.
//---------------------------------------------------------------------
BOOL CLuaEditor::Initialize()
{
	CString tName;
	HINSTANCE appInst;
		
	m_hScintilla = NULL;
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
	AfxSetResourceHandle(LuaEditorDLL.hModule);

	// use my string / icon resources
	// Note IDR_LUATYPE has to be a unique id even between DLLS
	// to get the ICONS right on the Window
	m_DocTemplate = new CMultiDocTemplate(IDR_LUATYPE,
			RUNTIME_CLASS(CLuaDoc),
			RUNTIME_CLASS(CLuaFrame),
			RUNTIME_CLASS(CLuaView));

	// put this after setting the menu and accelerators so it won't allocate it's own
	pApp->AddDocTemplate(m_DocTemplate);

	// construct resource info
	m_ObjCollection.name.LoadString(IDS_RESOURCE_NAME);
	m_ObjCollection.pageIndex = &m_PageIndex;

	m_ObjCollection.hIcon = (HICON)LoadImage(LuaEditorDLL.hModule, MAKEINTRESOURCE(IDR_LUATYPE), 
		IMAGE_ICON, 0, 0, LR_CREATEDIBSECTION);
	VERIFY(m_ObjCollection.hIcon != NULL);

	// value should be 1 - 1000
	m_ObjCollection.priority = IDR_LUATYPE;
	m_ObjCollection.extensions = LUA_EXTENSIONS;
	AfxSetResourceHandle(appInst);

	AfxInitRichEdit2();

	m_hScintilla = AfxLoadLibrary(_T("SciLexer.DLL"));
	if ( !m_hScintilla )
	{
		AfxMessageBox("Can't load Scintilla dll 'SciLexer.dll'",
			MB_OK|MB_ICONERROR);
		return FALSE;
	}

	AfxEnableControlContainer();

    return TRUE;
}

//---------------------------------------------------------------------
// Function:	~CLuaEditor
// Description:	Standard Desctructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CLuaEditor::~CLuaEditor()
{
	if (m_ObjCollection.hIcon != NULL)
	{
		DestroyIcon(m_ObjCollection.hIcon);
		m_ObjCollection.hIcon = NULL;
	}

	if (m_hScintilla != NULL)
	{
		AfxFreeLibrary(m_hScintilla);
		m_hScintilla = NULL;
	}
}

//---------------------------------------------------------------------
// Function:	OnCreate
// Description:	Called from the main windows create function
// Parameters:	CMDIFrameWnd *mainWnd = main frames CMDIFrameWnd pointer
// Returns:		.
//---------------------------------------------------------------------
void CLuaEditor::OnCreate(CMDIFrameWnd *mainWnd)
{
	MENUINFOMESSAGE mim;
	CBitmap menuBitmap;
	HINSTANCE appInst;
	HMENU tMenu;
	HMENU tEditMenu;
	HACCEL tAccel;

	static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(LuaEditorDLL.hModule);

	// load lua view menu
	tMenu = LoadMenu(LuaEditorDLL.hModule, MAKEINTRESOURCE(IDR_LUA_VIEW));
	ASSERT(tMenu != NULL);
	// merge lua view menu into main view menu
	VERIFY(MergeSubMenus(&tMenu, mim.m_hMenu));

	// create edit menu
	tEditMenu = LoadMenu(LuaEditorDLL.hModule, MAKEINTRESOURCE(IDR_LUA_EDIT));
	tAccel = LoadAccelerators(LuaEditorDLL.hModule, MAKEINTRESOURCE(IDR_LUA_EDIT));
	ASSERT(tEditMenu != NULL);
	ASSERT(tAccel != NULL);

	// merge edit menu in
	VERIFY(MergeMenus(&tEditMenu, tMenu, _T("&View")));
	// mege build menu in
	VERIFY(MergeMenus(&m_DocTemplate->m_hMenuShared, tEditMenu, _T("&Window")));	
	// and acclerators
	VERIFY(MergeAccelerators(&tAccel, mim.m_hAccel));
	VERIFY(MergeAccelerators(&m_DocTemplate->m_hAccelTable, tAccel));

	mim.m_hAccel = NULL;
	mim.m_hMenu = NULL;
	// load up hi-res toolbar icon
	menuBitmap.LoadBitmap(IDB_TOOLBAR);
	// we don't really have to have to toolbar visible do we?
	mim.m_MenuRsrc = IDR_LUATYPE;
	mim.m_MenuBitmap = &menuBitmap;

	static DWORD msgHash_SetMenuInfo = CHashString(_T("SetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	static DWORD msgHash_RegisterResourceCollection = CHashString(_T("RegisterResourceCollection")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterResourceCollection, sizeof(RESOURCECOLLECTION), &m_ObjCollection);

	//register our filetype
	FILETYPECREATIONINFO fileInfo;
	fileInfo.m_Description = LUA_FILE_DESCRIPTION;
	fileInfo.m_CreationCallback = CLuaEditor::LuaCreationCallback;
	static DWORD msgHash_RegisterFileTypeCreationInfo = CHashString(_T("RegisterFileTypeCreationInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterFileTypeCreationInfo, sizeof( FILETYPECREATIONINFO ), &fileInfo, NULL, NULL );

	AfxSetResourceHandle(appInst);

}

//---------------------------------------------------------------------
// Function:	OnCmdMsg
// Description:	Called from the main windows OnCmdMsg function.
//				Routes commands through doc/view architecture
// Returns:		TRUE if handled
//---------------------------------------------------------------------
BOOL CLuaEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return FALSE;
}

//---------------------------------------------------------------------
// Function:	EditDocument
// Description:	Edit an lua Document retrieving initial data 
//					from a archive
// Parameters:	ar = archive to load into document
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CLuaEditor::EditDocument( LPCTSTR lpszPathName)
{
	CLuaDoc *createDoc;

	// activate exiting document if it is opened already
	if (ActivateOpenedDocument(m_DocTemplate, lpszPathName))
	{
		return NO_ERROR;
	}

	if (!IsWindow(m_ToolDockBar))
	{
		CFrameWnd* pFrame = STATIC_DOWNCAST(CFrameWnd, AfxGetMainWnd());
		// create output window
		if(!m_ToolDockBar.Create(_T("Tools"), WS_CHILD, pFrame, IDD_LUA_TOOL_TAB))
		{
			TRACE0("Failed to create Output Window Dock Bar\n");
			return GetLastError();
		}
		m_ToolDockBar.SetBarStyle(m_ToolDockBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		m_ToolDockBar.EnableDocking(CBRS_ALIGN_ANY);
		pFrame->DockControlBar(&m_ToolDockBar, AFX_IDW_DOCKBAR_BOTTOM);
		pFrame->ShowControlBar(&m_ToolDockBar, TRUE, TRUE);
	}

	// if the file isn't already open
	// create blank document, with frame
	createDoc = DYNAMIC_DOWNCAST(CLuaDoc, m_DocTemplate->OpenDocumentFile(lpszPathName));

	return NO_ERROR;
}

//---------------------------------------------------------------------
// Function:	LuaCreationCallback
// Description:	Create a new lua file for editing 
// Returns:		void
//---------------------------------------------------------------------
void CLuaEditor::LuaCreationCallback(void *userData)
{
	CLuaEditor * pLuaEditorInstance = SINGLETONINSTANCE(CLuaEditor);
	if( pLuaEditorInstance )
	{
		pLuaEditorInstance->EditDocument(NULL);
	}
}

void CLuaEditor::GetDocSize(int &size)
{
	CMDIFrameWnd *mainFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	CMDIChildWnd *childFrame = mainFrame->MDIGetActive();
	// get view
	CLuaView *view = DYNAMIC_DOWNCAST(CLuaView, childFrame->GetActiveView());
	// get editor
	CLuaEditCtrl* pEditor = view->GetEditor();

	// get file size
	size = pEditor->GetDataSize();
}

void CLuaEditor::GetDocData(char *data)
{
	CMDIFrameWnd *mainFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	CMDIChildWnd *childFrame = mainFrame->MDIGetActive();
	// get view
	CLuaView *view = DYNAMIC_DOWNCAST(CLuaView, childFrame->GetActiveView());
	// get editor
	CLuaEditCtrl* pEditor = view->GetEditor();

	if (data)
	{
		// get the data from the document
		pEditor->GetData(data);
	}
	else
	{
		// log error
	}
}