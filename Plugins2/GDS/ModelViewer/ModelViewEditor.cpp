//=====================================================================
// File:		ModelViewEditor.cpp
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

#define MODELVIEW_EXTENSION_DESCRIPTION _T("Cal3D Model CFG File")
#define MODELVIEW_EXTENSIONS _T("cfg\\hrc")

AFX_EXTENSION_MODULE ModelViewEditorDLL = { false, NULL, NULL };

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("ModelViewEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(ModelViewEditorDLL, hInstance))
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

		new CDynLinkLibrary(ModelViewEditorDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("ModelViewEditor.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(ModelViewEditorDLL);
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
	return SINGLETONINSTANCE(CModelViewEditor);
}

//---------------------------------------------------------------------
// Function:	GetPriority
// Description:	DLL's priority
// Parameters:	.
// Returns:		DWORD = priority of DLL loaded
//---------------------------------------------------------------------
DWORD CModelViewEditor::GetPriority()
{
    return IDR_MODELVIEWTYPE;
}

//---------------------------------------------------------------------
// Function:	CModelViewEditor
// Description:	Standard Constructor
// Parameters:	pi = programs information structure
// Returns:		.
//---------------------------------------------------------------------
CModelViewEditor::CModelViewEditor()
{
	SINGLETONINSTANCE(CModelViewComponent)->SetParent(this);
	m_ToolBox = EngineGetToolBox();
	m_bToolBarIsActive = FALSE;
}

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CModelViewEditor::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(ModelViewEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	DLL's Initialization function, like InitInstance
// Parameters:	.
// Returns:		TRUE, if successful.
//---------------------------------------------------------------------
BOOL CModelViewEditor::Initialize()
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
	AfxSetResourceHandle(ModelViewEditorDLL.hModule);

	// use my string / icon resources
	// Note IDR_MODELVIEWTYPE has to be a unique id even between DLLS
	// to get the ICONS right on the Window
	m_DocTemplate = new CMultiDocTemplate(IDR_MODELVIEWTYPE,
			RUNTIME_CLASS(CModelViewDoc),
			RUNTIME_CLASS(CModelViewFrm),
			RUNTIME_CLASS(CModelViewRender));

	// put this after setting the menu and accelerators so it won't allocate it's own
	pApp->AddDocTemplate(m_DocTemplate);

	// construct resource info
	m_ObjCollection.name.LoadString(IDS_RESOURCE_NAME);
	m_ObjCollection.pageIndex = &m_PageIndex;

	m_ObjCollection.hIcon = (HICON)LoadImage(ModelViewEditorDLL.hModule, MAKEINTRESOURCE(IDR_MODELVIEWTYPE), 
		IMAGE_ICON, 0, 0, LR_CREATEDIBSECTION);
	VERIFY(m_ObjCollection.hIcon != NULL);

	// value should be 1 - 1000
	m_ObjCollection.priority = IDR_MODELVIEWTYPE;
	m_ObjCollection.extensions = MODELVIEW_EXTENSIONS;
	AfxSetResourceHandle(appInst);

    return TRUE;
}

//---------------------------------------------------------------------
// Function:	~CModelViewEditor
// Description:	Standard Destructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CModelViewEditor::~CModelViewEditor()
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
void CModelViewEditor::OnCreate(CMDIFrameWnd *mainWnd)
{
	MENUINFOMESSAGE mim;
	CBitmap menuBitmap;
	HINSTANCE appInst;

	static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(ModelViewEditorDLL.hModule);

	// we need to mergeMenus
	VERIFY(MergeMenus(&m_DocTemplate->m_hMenuShared, mim.m_hMenu, _T("&Window")));	
	// and accelerators
	VERIFY(MergeAccelerators(&m_DocTemplate->m_hAccelTable, mim.m_hAccel));

	mim.m_hAccel = NULL;
	mim.m_hMenu = NULL;
	// load up hi-res toolbar icon
	menuBitmap.LoadBitmap(IDB_TOOLBAR);
	// we don't really have to have to toolbar visible do we?
	mim.m_MenuRsrc = IDR_MODELVIEWTYPE;
	mim.m_MenuBitmap = &menuBitmap;

	static DWORD msgHash_SetMenuInfo = CHashString(_T("SetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	static DWORD msgHash_RegisterResourceCollection = CHashString(_T("RegisterResourceCollection")).GetUniqueID();
	DWORD result = m_ToolBox->SendMessage(msgHash_RegisterResourceCollection, sizeof(RESOURCECOLLECTION), &m_ObjCollection);

	if (result != MSG_HANDLED)
	{
		assert(0);
	}

	// We do not need to create Cal3D objects. Opening them seems to be enough

	//FILETYPECREATIONINFO fileInfo; 
	//fileInfo.m_Description = MODELVIEW_EXTENSION_DESCRIPTION; 
	//fileInfo.m_CreationCallback = CModelViewEditor::ModelViewEditorCreationCallback; 
	//static DWORD msgHash_RegisterFileTypeCreationInfo = CHashString(_T("RegisterFileTypeCreationInfo")).GetUniqueID();
	//result = m_ToolBox->SendMessage(msgHash_RegisterFileTypeCreationInfo, sizeof( FILETYPECREATIONINFO ), &fileInfo, NULL, NULL );

	//if (result != MSG_HANDLED)
	//{
	//	assert(0);
	//}

	AfxSetResourceHandle(appInst);
}

void CModelViewEditor::DisplayToolbar(bool display)
{
	// Doesn't exist, create.
	if (display && !m_bToolBarIsActive)
	{
		if (!m_ModelViewToolBar.Create(AfxGetApp()->GetMainWnd(), CModelViewToolBar::IDD,
		CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY, CModelViewToolBar::IDD))
		{
			TRACE0("Failed to create dockbar\n");
			return;      // fail to create
		}

		m_bToolBarIsActive = TRUE;
		TOOLBARSTRUCT atbs;
		atbs.Text = _T("Model View");
		/*
		atbs.hIcon = (HICON)LoadImage(ParticleEditorDLL.hModule, 
			MAKEINTRESOURCE(IDR_PARTICLETYPE), IMAGE_ICON, 0, 0, 0);
		*/
		atbs.cBar = &m_ModelViewToolBar;
		static DWORD msgHash_AddToolBar = CHashString(_T("AddToolBar")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_AddToolBar, sizeof(TOOLBARSTRUCT), &atbs);

		TOOLBARACTIVESTRUCT tbas;
		tbas.bActive = true;
		tbas.cBar = (CControlBar*)&m_ModelViewToolBar;
		static DWORD msgHash_SetActiveToolBar = CHashString(_T("SetActiveToolBar")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetActiveToolBar, sizeof(TOOLBARACTIVESTRUCT), &tbas);
	}
	// Exists, bring to front.
	else if (display && m_bToolBarIsActive)
	{
		TOOLBARACTIVESTRUCT tbas;
		tbas.bActive = true;
		tbas.cBar = (CControlBar*)&m_ModelViewToolBar;
		static DWORD msgHash_SetActiveToolBar = CHashString(_T("SetActiveToolBar")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetActiveToolBar, sizeof(TOOLBARACTIVESTRUCT), &tbas);
	}
	// Hide
	else
	{
		// If no more pages exist, destroy:
		if (m_DocTemplate->GetFirstDocPosition() == NULL)
		{
			// TODO: There is no any public interface for deleting toolbar
			// CGuiToolBarTabbed and GDS should be extended for this functionality for this
			//m_ParticleEditorToolPal.ShowWindow(SW_HIDE);
			m_ModelViewToolBar.DestroyWindow();
		}
	}
}

//---------------------------------------------------------------------
// Function:	GetToolBar
// Description:	Called from the doc to get the pointer to the toolbar for
//				data transfer.
// Returns:		TRUE if handled
//---------------------------------------------------------------------
CModelViewToolBar * CModelViewEditor::GetToolBar()
{
	return &m_ModelViewToolBar;
}

CModelViewDlg* CModelViewEditor::GetDockDlg()
{
	return &(m_ModelViewDockWin.m_ModelViewDialog);
}

//---------------------------------------------------------------------
// Function:	OnCmdMsg
// Description:	Called from the main windows OnCmdMsg function.
//				Routes commands through doc/view architecture
// Returns:		TRUE if handled
//---------------------------------------------------------------------
BOOL CModelViewEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return FALSE;
}

//---------------------------------------------------------------------
// Function:	CModelViewEditor
// Description:	Create a new ModelView  for editing 
// Parameters:	userData = void pointer, we don't use at the moment
// Returns:		void
//---------------------------------------------------------------------
void CModelViewEditor::ModelViewEditorCreationCallback(void *userData)
{
	CModelViewEditor * pModelViewEditorInstance = SINGLETONINSTANCE(CModelViewEditor);
	if( pModelViewEditorInstance )
	{
		pModelViewEditorInstance->EditDocument(NULL);
	}
}

//---------------------------------------------------------------------
// Function:	EditDocument
// Description:	Edit an ModelView Document retrieving initial data 
//					from a archive
// Parameters:	szFilename = name of file to load
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CModelViewEditor::EditDocument(LPCTSTR szFilename)
{
	HINSTANCE appInst;
		
	// Register the doc templates we provide to the app
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
    if (pApp == NULL)
    {
		return ERROR_MOD_NOT_FOUND;
    }

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(ModelViewEditorDLL.hModule);

	// ORDER: toolbar should be created before model view
	// model view needs access to toolbar functions during initialization
	DisplayToolbar(true);
	
	if (!IsWindow(m_ModelViewDockWin))
	{
		// Dock Win
		CGuiMDIFrame *mainWnd = DYNAMIC_DOWNCAST(CGuiMDIFrame, AfxGetMainWnd());
		if (m_ModelViewDockWin.Create(_T("Model View"), WS_CHILD | WS_VISIBLE, mainWnd, IDD_MODELVIEWDLG))
		{
			CRect winRect;
			mainWnd->GetWindowRect(&winRect);
			m_ModelViewDockWin.EnableDocking(CBRS_ALIGN_ANY);
			m_ModelViewDockWin.SetBarStyle(m_ModelViewDockWin.GetBarStyle() |
				CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
			mainWnd->DockControlBar(&m_ModelViewDockWin, AFX_IDW_DOCKBAR_RIGHT, winRect);
			m_ModelViewDockWin.LockPushPin(FALSE);
		}
	}

	// create blank document, with frame
	m_DocTemplate->OpenDocumentFile(szFilename);

	AfxSetResourceHandle(appInst);
	return NO_ERROR;
}

CModelViewRender *CModelViewEditor::FindView(LPCTSTR szModelName)
{
	CHashString hszModelName = szModelName;
	DWORD dwModelNameID = hszModelName.GetUniqueID();
	POSITION posDoc = m_DocTemplate->GetFirstDocPosition();
	while (posDoc != NULL)
	{
		CDocument *pDoc = m_DocTemplate->GetNextDoc(posDoc);
		POSITION posView = pDoc->GetFirstViewPosition();
		if (posView != NULL)
		{
			CView *pView = pDoc->GetNextView(posView);
			CModelViewRender *pModelView = dynamic_cast<CModelViewRender *>(pView);
			if (pModelView != NULL)
			{
				if (pModelView->GetEntityName()->GetUniqueID() == dwModelNameID)
				{
					return pModelView;
				}
			}
		}
	}
	return NULL;
}

void CModelViewEditor::Update()
{
	POSITION posDoc = m_DocTemplate->GetFirstDocPosition();
	while (posDoc != NULL)
	{
		CDocument *pDoc = m_DocTemplate->GetNextDoc(posDoc);
		POSITION posView = pDoc->GetFirstViewPosition();
		if (posView != NULL)
		{
			CView *pView = pDoc->GetNextView(posView);
			CModelViewRender *pModelView = dynamic_cast<CModelViewRender *>(pView);
			if (pModelView != NULL)
			{
				pModelView->Update();
			}
		}
	}
}

CModelViewRender* CModelViewEditor::GetActiveView()
{
	CMDIFrameWnd *mainFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	CMDIChildWnd *childFrame = mainFrame->MDIGetActive();
	return DYNAMIC_DOWNCAST(CModelViewRender, childFrame->GetActiveView());
}
