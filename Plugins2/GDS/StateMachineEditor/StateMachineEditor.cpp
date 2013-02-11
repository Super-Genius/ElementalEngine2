///============================================================================
/// \file		StateMachineEditor.cpp
/// \brief		Class implementation of CStateMachineEditor
/// \date		03-05-2007
/// \author		Richard Nguyen
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STATEMACHINE_EXTENSIONS "xms"
#define STATEMACHINE_FILE_DESCRIPTION _T("State Machine")

AFX_EXTENSION_MODULE StateMachineEditorDLL = { false, NULL, NULL };

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("StateMachineEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(StateMachineEditorDLL, hInstance))
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

		new CDynLinkLibrary(StateMachineEditorDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("StateMachineEditor.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(StateMachineEditorDLL);
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
	// this has to be done after module is initialized.
	return SINGLETONINSTANCE(CStateMachineEditor);
}

static LPCTSTR OpenFilter = {
	"State Machine Files (*.xms)\0" "*.xms\0"
	"\0\0"
};

static LPCTSTR SaveFilter = {
	"State Machine Files (*.xms)\0" "*.xms\0"
	"\0\0"
};

void CStateMachineEditor::LoadXMS( IHashString *hszFilePath )
{
	CStateMachineEditorDoc *editorDoc;
	editorDoc = DYNAMIC_DOWNCAST( CStateMachineEditorDoc, m_DocTemplate->OpenDocumentFile(hszFilePath->GetString()) );
}

//---------------------------------------------------------------------
// Function:	GetPriority
// Description:	DLL's priority
// Parameters:	.
// Returns:		DWORD = priority of DLL loaded
//---------------------------------------------------------------------
DWORD CStateMachineEditor::GetPriority()
{
	return IDR_STATEMACHINETYPE;
}

//---------------------------------------------------------------------
// Function:	CStateMachineEditor
// Description:	Standard Constructor
// Parameters:	pi = programs information structure
// Returns:		.
//---------------------------------------------------------------------
CStateMachineEditor::CStateMachineEditor()
{
	SINGLETONINSTANCE(CStateMachineEditorComponent)->SetEditorInstance(this);
	m_ToolBox = EngineGetToolBox();
	m_ToolBarShown = FALSE;
}

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CStateMachineEditor::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(StateMachineEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	DLL's Initialization function, like InitInstance
// Parameters:	.
// Returns:		TRUE, if successful.
//---------------------------------------------------------------------
BOOL CStateMachineEditor::Initialize()
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
	AfxSetResourceHandle(StateMachineEditorDLL.hModule);

	// use my string / icon resources
	// Note IDR_EXAMPLETYPE has to be a unique id even between DLLS
	// to get the ICONS right on the Window
	m_DocTemplate = new CMultiDocTemplate(IDR_STATEMACHINETYPE,
			RUNTIME_CLASS(CStateMachineEditorDoc),
			RUNTIME_CLASS(CStateMachineEditorFrm),
			RUNTIME_CLASS(CStateMachineEditorView));

	//// put this after setting the menu and accelerators so it won't allocate it's own
	pApp->AddDocTemplate(m_DocTemplate);

	// construct resource info
	m_ObjCollection.name.LoadString(IDS_RESOURCE_NAME);
	m_ObjCollection.pageIndex = &m_PageIndex;

	m_ObjCollection.hIcon = (HICON)LoadImage(StateMachineEditorDLL.hModule, MAKEINTRESOURCE(IDR_STATEMACHINETYPE), 
		IMAGE_ICON, 0, 0, LR_CREATEDIBSECTION);
	VERIFY(m_ObjCollection.hIcon != NULL);

	// value should be 1 - 1000
	m_ObjCollection.priority = IDR_STATEMACHINETYPE;
	m_ObjCollection.extensions = STATEMACHINE_EXTENSIONS;

	AfxSetResourceHandle(appInst);

    return TRUE;
}

//---------------------------------------------------------------------
// Function:	~CStateMachineEditor
// Description:	Standard Destructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CStateMachineEditor::~CStateMachineEditor()
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
void CStateMachineEditor::OnCreate(CMDIFrameWnd *mainWnd)
{
	MENUINFOMESSAGE mim;
	CBitmap menuBitmap;
	HINSTANCE appInst;

	static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(StateMachineEditorDLL.hModule);

	// we need to mergeMenus
	VERIFY(MergeMenus(&m_DocTemplate->m_hMenuShared, mim.m_hMenu, _T("&Window")));	
	// and accelerators
	VERIFY(MergeAccelerators(&m_DocTemplate->m_hAccelTable, mim.m_hAccel));

	mim.m_hAccel = NULL;
	mim.m_hMenu = NULL;
	// load up hi-res toolbar icon
	//menuBitmap.LoadBitmap(IDB_TOOLBAR);
	// we don't really have to have to toolbar visible do we?
	//mim.m_MenuRsrc = IDR_STATEMACHINETYPE;
	//mim.m_MenuBitmap = &menuBitmap;

	static DWORD msgHash_SetMenuInfo = CHashString(_T("SetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	static DWORD msgHash_RegisterResourceCollection = CHashString(_T("RegisterResourceCollection")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterResourceCollection, sizeof(RESOURCECOLLECTION), &m_ObjCollection);

	FILETYPECREATIONINFO fileInfo;
	fileInfo.m_Description = STATEMACHINE_FILE_DESCRIPTION;
	fileInfo.m_CreationCallback = CStateMachineEditor::StateMachineEditorCreationCallback;
	static DWORD msgHash_RegisterFileTypeCreationInfo = CHashString(_T("RegisterFileTypeCreationInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterFileTypeCreationInfo, sizeof( FILETYPECREATIONINFO ), &fileInfo );

	AfxSetResourceHandle(appInst);
}

//---------------------------------------------------------------------
// Function:	OnCmdMsg
// Description:	Called from the main windows OnCmdMsg function.
//				Routes commands through doc/view architecture
// Returns:		TRUE if handled
//---------------------------------------------------------------------
BOOL CStateMachineEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return FALSE;
}

//---------------------------------------------------------------------
// Function:	CreateStateMachine
// Description:	Edit an example Document retrieving initial data 
//					from a archive
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CStateMachineEditor::CreateStateMachine()
{
	m_DocTemplate->OpenDocumentFile(NULL);

	return NO_ERROR;
}

void CStateMachineEditor::StateMachineEditorCreationCallback(void *userData)
{
	CStateMachineEditor *instance = SINGLETONINSTANCE(CStateMachineEditor);
	if( instance )
	{
		instance->CreateStateMachine();
	}
}

///	\brief	this function shows or hides state machine editor toolbar
///	\param	show - true if toolbar should be shown or false otherwise
void CStateMachineEditor::ShowToolBar(bool show)
{
	if (show)
	{
		if (!IsWindow(m_ToolBar))
		{
			const DWORD dwStyle = CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY;
			if (!m_ToolBar.Create(AfxGetMainWnd(), CStateMachineToolBar::IDD, dwStyle, CStateMachineToolBar::IDD))
			{
				TRACE0("Failed to create dockbar\n");
				return;
			}
		}

		if (m_ToolBarShown == FALSE)
		{
			TOOLBARSTRUCT atbs;
			atbs.Text = _T("State Machine");
			atbs.hIcon = m_ObjCollection.hIcon;
			atbs.cBar = &m_ToolBar;
			static DWORD msgHash_AddToolBar = CHashString(_T("AddToolBar")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_AddToolBar, sizeof(TOOLBARSTRUCT), &atbs);
			m_ToolBarShown = TRUE;
		}

		TOOLBARACTIVESTRUCT tbas;
		tbas.bActive = true;
		tbas.cBar = &m_ToolBar;
		static DWORD msgHash_SetActiveToolBar = CHashString(_T("SetActiveToolBar")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetActiveToolBar, sizeof(TOOLBARACTIVESTRUCT), &tbas);
	}
	else if (m_DocTemplate->GetFirstDocPosition() == NULL)
	{
		static DWORD msgHash_HideToolBar = CHashString(_T("HideToolBar")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_HideToolBar, sizeof(CControlBar *), &m_ToolBar);
		m_ToolBarShown = FALSE;
	}
}

///	\brief	select object in the editor 
///	\param	name - hash of the object name
void CStateMachineEditor::UpdateViews(IHashString *name, void (CStateMachineEditorView:: *pViewFun)(IHashString *))
{
	// iterate all opened documents
	POSITION posDoc = m_DocTemplate->GetFirstDocPosition();
	while (posDoc != NULL)
	{
		CDocument *pDoc = m_DocTemplate->GetNextDoc(posDoc);
		// iterate each opened view for the document
		POSITION posView = pDoc->GetFirstViewPosition();
		while (posView != NULL)
		{
			CView *p = pDoc->GetNextView(posView);
			CStateMachineEditorView *pView;
			pView = static_cast<CStateMachineEditorView *>(p);
			(pView->*pViewFun)(name);
		}
	}
}

///	\brief	this function updates all state machine editor views with passed
///			object name hash
///	\param	name - hash of the object name
void CStateMachineEditor::OnInitObject(IHashString *name)
{
	UpdateViews(name, &CStateMachineEditorView::OnInitObject);
}

///	\brief	select object in the editor 
///	\param	hashName - hash of the object name
void CStateMachineEditor::SelectObject(IHashString *name)
{
	UpdateViews(name, &CStateMachineEditorView::SelectObject);
}

///	\brief	validate passed data in the editor
///	\param	sop - pointer to serialization parameters
///	\return EE message code
DWORD CStateMachineEditor::ValidateObject(SERIALIZEOBJECTPARAMS *sop)
{
	// iterate all opened documents
	POSITION posDoc = m_DocTemplate->GetFirstDocPosition();
	while (posDoc != NULL)
	{
		CDocument *pDoc = m_DocTemplate->GetNextDoc(posDoc);
		// iterate each opened view for the document
		POSITION posView = pDoc->GetFirstViewPosition();
		while (posView != NULL)
		{
			CView *p = pDoc->GetNextView(posView);
			CStateMachineEditorView *pView;
			pView = static_cast<CStateMachineEditorView *>(p);
			DWORD res = pView->ValidateObject(sop);
			if (MSG_NOT_HANDLED != res)
			{
				return res;
			}
		}
	}
	return MSG_NOT_HANDLED;
}
