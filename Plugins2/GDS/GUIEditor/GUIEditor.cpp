//=====================================================================
// File:		GUIEditor.cpp
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

#define GUIEditor_EXTENSIONS "gxl"

#define GUIEditor_FILE_DESCRIPTION _T("Graphical User Interface")
AFX_EXTENSION_MODULE GUIEditorDLL = { false, NULL, NULL };

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("GUIEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(GUIEditorDLL, hInstance))
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

		new CDynLinkLibrary(GUIEditorDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("GUIEditor.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(GUIEditorDLL);
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
	return SINGLETONINSTANCE(CGUIEditor);
}

static LPCTSTR OpenFilter = {
	"GUIEditor Files (*.gxl)\0" "*.gxl\0"
	"\0\0"
};

static LPCTSTR SaveFilter = {
	"GUIEditor Files (*.gxl)\0" "*.gxl\0"
	"\0\0"
};

//---------------------------------------------------------------------
// Function:	GetPriority
// Description:	DLL's priority
// Parameters:	.
// Returns:		DWORD = priority of DLL loaded
//---------------------------------------------------------------------
DWORD CGUIEditor::GetPriority()
{
    return IDR_EXAMPLETYPE;
}

//---------------------------------------------------------------------
// Function:	CGUIEditor
// Description:	Standard Constructor
// Parameters:	pi = programs information structure
// Returns:		.
//---------------------------------------------------------------------
CGUIEditor::CGUIEditor()
{
	SINGLETONINSTANCE(CGUIEditorComponent)->SetParent(this);
	m_ToolBox = EngineGetToolBox();	
	m_bDocOpen = false;
	m_bCreated = false;
}

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CGUIEditor::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(GUIEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	DLL's Initialization function, like InitInstance
// Parameters:	.
// Returns:		TRUE, if successful.
//---------------------------------------------------------------------
BOOL CGUIEditor::Initialize()
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
	AfxSetResourceHandle(GUIEditorDLL.hModule);

	
	// use my string / icon resources
	// Note IDR_GUIEditorTYPE has to be a unique id even between DLLS
	// to get the ICONS right on the Window
	m_DocTemplate = new CMultiDocTemplate(IDR_EXAMPLETYPE,
			RUNTIME_CLASS(CGUIEditorDoc),
			RUNTIME_CLASS(CGUIEditorFrm),
			RUNTIME_CLASS(CGUIEditorView));

	// put this after setting the menu and accelerators so it won't allocate it's own
	pApp->AddDocTemplate(m_DocTemplate);

	// construct resource info
	m_ObjCollection.name.LoadString(IDS_RESOURCE_NAME);
	m_ObjCollection.pageIndex = &m_PageIndex;

	m_ObjCollection.hIcon = (HICON)LoadImage(GUIEditorDLL.hModule, MAKEINTRESOURCE(IDR_EXAMPLETYPE), 
		IMAGE_ICON, 0, 0, LR_CREATEDIBSECTION);
	VERIFY(m_ObjCollection.hIcon != NULL);

	// value should be 1 - 1000
	m_ObjCollection.priority = IDR_EXAMPLETYPE;
	m_ObjCollection.extensions = GUIEditor_EXTENSIONS;
	AfxSetResourceHandle(appInst);	
    return TRUE;
}

//---------------------------------------------------------------------
// Function:	~CGUIEditor
// Description:	Standard Desctructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CGUIEditor::~CGUIEditor()
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
void CGUIEditor::OnCreate(CMDIFrameWnd *mainWnd)
{
	MENUINFOMESSAGE mim;
	CBitmap menuBitmap;
	HINSTANCE appInst;

	static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE),&mim);

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(GUIEditorDLL.hModule);

	// we need to mergeMenus
	VERIFY(MergeMenus(&m_DocTemplate->m_hMenuShared, mim.m_hMenu, _T("&Window")));	
	// and acclerators
	VERIFY(MergeAccelerators(&m_DocTemplate->m_hAccelTable, mim.m_hAccel));

	mim.m_hAccel = NULL;
	mim.m_hMenu = NULL;
	// load up hi-res toolbar icon
	menuBitmap.LoadBitmap(IDB_TOOLBAR);
	// we don't really have to have to toolbar visible do we?
	mim.m_MenuRsrc = IDR_EXAMPLETYPE;
	mim.m_MenuBitmap = &menuBitmap;

	static DWORD msgHash_SetMenuInfo = CHashString(_T("SetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetMenuInfo, sizeof(MENUINFOMESSAGE),&mim);

	static DWORD msgHash_RegisterResourceCollection = CHashString(_T("RegisterResourceCollection")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterResourceCollection, sizeof(RESOURCECOLLECTION),&m_ObjCollection);

	FILETYPECREATIONINFO fileInfo;
	fileInfo.m_Description = GUIEditor_FILE_DESCRIPTION;
	fileInfo.m_CreationCallback = CGUIEditor::GUIEditorCreationCallback;
	static DWORD msgHash_RegisterFileTypeCreationInfo = CHashString(_T("RegisterFileTypeCreationInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterFileTypeCreationInfo, sizeof( FILETYPECREATIONINFO ),&fileInfo);

	m_ResBox.SetEditorParent(this);

	m_ResBox.Create(IDD_RESBOX, AfxGetApp()->GetMainWnd());

	AfxSetResourceHandle(appInst);	

}

//---------------------------------------------------------------------
// Function:	OnCmdMsg
// Description:	Called from the main windows OnCmdMsg function.
//				Routes commands through doc/view architecture
// Returns:		TRUE if handled
//---------------------------------------------------------------------
BOOL CGUIEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return FALSE;
}

//---------------------------------------------------------------------
// Function:	EditDocument
// Description:	Edit an GUIEditor Document retrieving initial data 
//					from a archive
// Parameters:	ar = archive to load into document
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CGUIEditor::EditDocument()
{
	if (!m_bCreated)
	{
		SetupTabBar();
		m_bCreated = true;
	}
	if (!m_bDocOpen)
	{
		StdString res;
		ReadINIKey(_T("LastUsedResolution"), res, _T("GUIEditor"), _T("800x600"));
		StdString x, y;
		res.GetToken(_T("x"), x);
		res.GetToken(_T("x"), y);
		UINT iX, iY;
		iX = (UINT)atoi(x.c_str());
		iY = (UINT)atoi(y.c_str());
		CreateDocView(iX, iY);
	}
	else
	{
		MessageBox(NULL, _T("Can't have more than one GUI open at a time"), _T("Sorry, mac"), MB_OK);
	}
	return NO_ERROR;
}

void CGUIEditor::LoadGXL(GUIPAGELIST gpl, TCHAR* fileName)
{
	if (!m_bCreated)
	{
		SetupTabBar();
		m_bCreated = true;
	}
	if (!m_bDocOpen)
	{
		// make sure we have widgets
		CreateWidgets();

		CGUIEditorDoc *createDoc;
		// create blank document, with frame
		createDoc = DYNAMIC_DOWNCAST(CGUIEditorDoc, m_DocTemplate->OpenDocumentFile(fileName));
		createDoc->SetToolbar(&m_TabBar);

		GUIWORKINGRES gwr;
		static DWORD msgHash_GetFileResolution = CHashString(_T("GetFileResolution")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_GetFileResolution, sizeof(GUIWORKINGRES), &gwr);		
		createDoc->LoadGXL(gpl, gwr);
		
		SIZEDATA sd;
		sd.m_fXSize = (float)gwr.iSizeX;
		sd.m_fYSize = (float)gwr.iSizeY;
		static DWORD msgHash_SetSize = CHashString(_T("SetSize")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetSize, sizeof(SIZEDATA), &sd, &CHashString(_T("WorkingCanvas")), &CHashString(_T("CGUIBackground")));	
	}
	else
	{
		MessageBox(NULL, _T("Can't have more than one GUI open at a time"), _T("Sorry, mac"), MB_OK);
	}
}

void CGUIEditor::CreateDocView(UINT xRes, UINT yRes)
{
	CreateWidgets();	
	CGUIEditorDoc *createDoc;
	// create blank document, with frame
	createDoc = DYNAMIC_DOWNCAST(CGUIEditorDoc, m_DocTemplate->OpenDocumentFile(NULL));
	createDoc->SetToolbar(&m_TabBar);

	createDoc->NewGUISystem(xRes, yRes);
		
    SIZEDATA sd;
	sd.m_fXSize = (float)xRes;
	sd.m_fYSize = (float)yRes;
	static DWORD msgHash_SetSize = CHashString(_T("SetSize")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetSize, sizeof(SIZEDATA), &sd, &CHashString(_T("WorkingCanvas")), &CHashString(_T("CGUIBackground")));		
}

void CGUIEditor::GUIEditorCreationCallback(void *userData)
{
	CGUIEditor * pGUIEditorInstance = SINGLETONINSTANCE(CGUIEditor);
	if( pGUIEditorInstance )
	{
		pGUIEditorInstance->EditDocument();
	}	
}

void CGUIEditor::CreateWidgets()
{

	// create editor specific widgets
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	DWORD retval;

	CHashString cName, pName, rName, cType, rType;
	cName = _T("WorkingCanvas");
	pName = _T("World");
	rName = _T("SelectRect");
	cType = _T("CGUIBackground");
	rType = _T("CGUIRect");

	IToolBox *m_ToolBox = EngineGetToolBox();

	cop.name = &cName;
	cop.parentName = &pName;
	cop.typeName = &cType;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
	if (retval = MSG_HANDLED)
	{
		iop.name = &cName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
	}

	cop.name = &rName;
	cop.parentName = &pName;
	cop.typeName = &rType;
	retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
	if (retval = MSG_HANDLED)
	{
		iop.name = &rName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
	}

	POSITIONDATA pd;
	pd.m_fXPos = 0.f;
	pd.m_fYPos = 0.f;
	static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &pd, &cName, &cType);	

	bool editorMode = true;
	static DWORD msgHash_SetGUIEditorMode = CHashString(_T("SetGUIEditorMode")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_SetGUIEditorMode, sizeof(bool), &editorMode, NULL, NULL);
	CHashString verNum = _T("2.0c");
	static DWORD msgHash_SetGUIVersion = CHashString(_T("SetGUIVersion")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_SetGUIVersion, sizeof(CHashString), &verNum, NULL, NULL);

	bool active = false;
	static DWORD msgHash_SetActiveMode = CHashString(_T("SetActiveMode")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetActiveMode, sizeof(bool), &active, NULL, NULL);
}

void CGUIEditor::SetupTabBar()
{
	CWinApp *app = AfxGetApp();
	assert(app);
	CWnd *mainWnd = app->GetMainWnd();
	assert(mainWnd);
	m_TabBar.Create(mainWnd, CGUITabBar::IDD, CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY, CGUITabBar::IDD);
	
	TOOLBARSTRUCT atbs;
	atbs.Text = _T("GUI");
	atbs.cBar = &m_TabBar;
	atbs.hIcon = (HICON)LoadImage(GUIEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_EDITORICO), IMAGE_ICON, 0, 0, 0);
	static DWORD msgHash_AddToolBar = CHashString(_T("AddToolBar")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_AddToolBar, sizeof(TOOLBARSTRUCT), &atbs);	
}

void CGUIEditor::UpdateBarPos(UINT val)
{
	POSITION pos = m_DocTemplate->GetFirstDocPosition();
	CGUIEditorDoc *loadedDoc;	
	loadedDoc = DYNAMIC_DOWNCAST(CGUIEditorDoc, m_DocTemplate->GetNextDoc(pos));
	assert(loadedDoc);

	loadedDoc->SetAnimBarPos(val);
}