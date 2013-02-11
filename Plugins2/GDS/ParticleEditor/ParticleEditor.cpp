///============================================================================
/// \file		ParticleEditor.cpp
/// \brief		Implementation of ParticleEditor Editor
/// \date		08-24-2006
/// \author		Marvin Gouw
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
#include <afxdllx.h>
#include "resource.h"
#include "ParticleEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#ifdef _DEBUG

/*
// major version in upper 16 bits
// low word =  minor version, low bit debug =0, release=1
#define INTERFACEDLLVERSION 0x00006100
#else
#define INTERFACEDLLVERSION 0x00006101
#endif
*/
// we don't need loading before anybody, so set priority rather high


#define PARTICLE_EXTENSION_DESCRIPTION _T("Particle XML File")
#define PARTICLE_EXTENSIONS _T("pml")

AFX_EXTENSION_MODULE ParticleEditorDLL = {false, NULL, NULL };

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("ParticleEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(ParticleEditorDLL, hInstance))
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

		new CDynLinkLibrary(ParticleEditorDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("ParticleEditor.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(ParticleEditorDLL);
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
	return SINGLETONINSTANCE(CParticleEditor);
}

static LPCTSTR OpenFilter = {
	"Particle Files (*.pml)\0" "*.pml\0"
	"\0\0"
};

static LPCTSTR SaveFilter = {
	"Particle Files (*.pml)\0" "*.pml\0"
	"\0\0"
};

DWORD CParticleEditor::GetPriority()
{
	return IDR_PARTICLETYPE;
}


CParticleEditor::CParticleEditor( )
	: m_ParticlePropertyDock(_T("ParticleDocBarConfig.xgf"), _T("\\ParticleEditorProperties\\"))
	, m_ParticleTreeDock()
{
	SINGLETONINSTANCE(CParticleEditorComponent)->SetParent(this);
	m_ToolBox = EngineGetToolBox();
	m_AppInst = NULL;
}

CString* CParticleEditor::GetName( void )
{
	static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(ParticleEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

BOOL CParticleEditor::Initialize( )
{
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
    if (pApp == NULL)
    {
        return FALSE;
    }

	// get the apps instance handle for loading resources
	m_AppInst = AfxGetResourceHandle();

	//Direct the application to look in dll for resources
	AfxSetResourceHandle(ParticleEditorDLL.hModule);

	// use my string / icon resources
	// Note IDR_EXAMPLETYPE has to be a unique id even between DLLS
	// to get the ICONS right on the Window
	m_DocTemplate = new CMultiDocTemplate(IDR_PARTICLETYPE,
			RUNTIME_CLASS(CParticleDoc),
			RUNTIME_CLASS(CParticleFrame),
			RUNTIME_CLASS(CParticleRenderView));

	// put this after setting the menu and accelerators so it won't allocate it's own
	pApp->AddDocTemplate(m_DocTemplate);

	// construct resource info
	m_ObjCollection.name.LoadString(IDS_RESOURCE_NAME);
	m_ObjCollection.pageIndex = &m_PageIndex;

	m_ObjCollection.hIcon = (HICON)LoadImage(ParticleEditorDLL.hModule, MAKEINTRESOURCE(IDR_PARTICLETYPE), 
		IMAGE_ICON, 0, 0, LR_CREATEDIBSECTION);
	VERIFY(m_ObjCollection.hIcon != NULL);

	// value should be 1 - 1000
	// NOTE: Against convention this was set at coding to 31000 even the example is set at 5000
	m_ObjCollection.priority = IDR_PARTICLETYPE; 
    m_ObjCollection.extensions = PARTICLE_EXTENSIONS;
    AfxSetResourceHandle(m_AppInst);

	return TRUE;
}

CParticleEditor::~CParticleEditor( )
{
	if (m_ObjCollection.hIcon != NULL)
	{
		DestroyIcon(m_ObjCollection.hIcon);
		m_ObjCollection.hIcon = NULL;
	}
}

void CParticleEditor::OnCreate(CMDIFrameWnd *mainWnd)
{
	MENUINFOMESSAGE mim;
	CBitmap menuBitmap;

	static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);
	
	// get the apps instance handle for loading resources
	m_AppInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(ParticleEditorDLL.hModule);

	// we need to mergeMenus
	VERIFY(MergeMenus(&m_DocTemplate->m_hMenuShared, mim.m_hMenu, _T("&Window")));	
	// and acclerators
	VERIFY(MergeAccelerators(&m_DocTemplate->m_hAccelTable, mim.m_hAccel));

	mim.m_hAccel = NULL;
	mim.m_hMenu = NULL;
	// load up hi-res toolbar icon
	//menuBitmap.LoadBitmap(IDB_TOOLBAR);
	// we don't really have to have to toolbar visible do we?
	mim.m_MenuRsrc = IDR_PARTICLETYPE;
	mim.m_MenuBitmap = NULL;	//&menuBitmap;

	static DWORD msgHash_SetMenuInfo = CHashString(_T("SetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	static DWORD msgHash_RegisterResourceCollection = CHashString(_T("RegisterResourceCollection")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterResourceCollection, sizeof(RESOURCECOLLECTION), &m_ObjCollection);

	FILETYPECREATIONINFO fileInfo; 
	fileInfo.m_Description = PARTICLE_EXTENSION_DESCRIPTION; 
	fileInfo.m_CreationCallback = CParticleEditor::ParticleEditorCreationCallback; 
	static DWORD msgHash_RegisterFileTypeCreationInfo = CHashString(_T("RegisterFileTypeCreationInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterFileTypeCreationInfo, sizeof( FILETYPECREATIONINFO ), &fileInfo );

	AfxSetResourceHandle(m_AppInst);
}

void CParticleEditor::DisplayToolbar(bool display)
{
	bool bIsActive = FALSE != IsWindow(m_ParticleEditorToolPal);
	// Doesn't exist, create.
	if (display && !bIsActive)
	{
		/*
		m_ParticleEditorToolPal.Create(_T("CParticleEditorToolPal"),
										_T("PEToolPal"),
										WS_CHILD | WS_VISIBLE,
										CRect(0,0,300,100), 
										this, 
										IDD_PARTICLE_TOOL_PALETTE);

		*/

//		m_ParticleEditorToolPal.Create(AfxGetApp()->GetMainWnd());
		if (!m_ParticleEditorToolPal.Create(AfxGetApp()->GetMainWnd(), CParticleEditorToolPal::IDD,
		CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY, CParticleEditorToolPal::IDD))
//		if (!m_ParticleEditorToolPal.Create(AfxGetApp()->GetMainWnd(), IDD_PARTICLE_TOOL_PALETTE,
//		CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY, IDD_PARTICLE_TOOL_PALETTE))
		{
			TRACE0("Failed to create dockbar\n");
			return;      // fail to create
		}

		TOOLBARSTRUCT atbs;
		atbs.Text = _T("Particle Edit");
		
		atbs.hIcon = (HICON)LoadImage(ParticleEditorDLL.hModule, 
			MAKEINTRESOURCE(IDR_PARTICLETYPE), IMAGE_ICON, 0, 0, 0);
		
		atbs.cBar = &m_ParticleEditorToolPal;
		static DWORD msgHash_AddToolBar = CHashString(_T("AddToolBar")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_AddToolBar, sizeof(TOOLBARSTRUCT), &atbs);

		TOOLBARACTIVESTRUCT tbas;
		tbas.bActive = true;
		tbas.cBar = (CControlBar*)&m_ParticleEditorToolPal;
		static DWORD msgHash_SetActiveToolBar = CHashString(_T("SetActiveToolBar")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetActiveToolBar, sizeof(TOOLBARACTIVESTRUCT), &tbas);
	}
	// Exists, bring to front.
	else if (display && bIsActive)
	{
		TOOLBARACTIVESTRUCT tbas;
		tbas.bActive = true;
		tbas.cBar = (CControlBar*)&m_ParticleEditorToolPal;
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
		}
	}
}

BOOL CParticleEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return FALSE;
}

//---------------------------------------------------------------------
// Function:	EditDocument
// Description:	Open editor window for particle file
// Parameters:	szFilename - path to file
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CParticleEditor::EditDocument(LPCTSTR szFilename)
{
	DisplayToolbar(true);

	m_DocTemplate->OpenDocumentFile(szFilename);

	return NO_ERROR;
}
//---------------------------------------------------------------------
// Function:	GetToolBar
// Description:	Called from the doc to get the pointer to the toolbar for
//				data transfer.
// Returns:		TRUE if handled
//---------------------------------------------------------------------
CParticleEditorToolPal * CParticleEditor::GetToolBar()
{
	return &m_ParticleEditorToolPal;
}

//---------------------------------------------------------------------
// Function:	SaveDocument
// Description:	Save an example Document retrieving initial data 
//					from a archive
// Parameters:	szFilename - path to saved file
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CParticleEditor::SaveDocument(LPCTSTR szFilename)
{
//    m_DocTemplate->SaveAllModified();
//
	return NO_ERROR;
}

//---------------------------------------------------------------------
// Function:	ParticleCreationCallback
// Description:	Create a new Particle chart for editing 
// Parameters:	userData = void pointer, we don't use at the moment
// Returns:		void
//---------------------------------------------------------------------
void CParticleEditor::ParticleEditorCreationCallback(void *userData)
{
	CParticleEditor * pParticleEditorInstance = SINGLETONINSTANCE(CParticleEditor);
	if( pParticleEditorInstance )
	{
		pParticleEditorInstance->EditDocument(NULL);
	}
}

void CParticleEditor::ResetToolBar()
{
	m_ParticleEditorToolPal.ResetToolBar();
}

bool CParticleEditor::ShowProperties()
{
	// do not create toolbar second time
	if (IsWindow(m_ParticlePropertyDock))
	{
		//m_ParticlePropertyDock.ShowWindow(SW_SHOW);
		return false;
	}
	CMDIFrameWnd *pMainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	ASSERT(pMainWnd != NULL);
	if (!m_ParticlePropertyDock.Create(_T("Item Properties"), WS_CHILD | WS_VISIBLE, pMainWnd, IDC_PARTICLE_DOCKBAR))
	{
		m_ToolBox->Log(LOGERROR, _T("Failed to create m_ParticlePropertyDock window bar\n"));
		return false;
	}
	m_ParticlePropertyDock.EnableDocking(CBRS_ALIGN_ANY);
	const DWORD dwBarStyle = CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC;
	m_ParticlePropertyDock.SetBarStyle(m_ParticlePropertyDock.GetBarStyle() | dwBarStyle);
	// dock property dock bar to on Right
	pMainWnd->DockControlBar(&m_ParticlePropertyDock, AFX_IDW_DOCKBAR_RIGHT, NULL);
	return true;
}

bool CParticleEditor::ShowTree()
{
	// do not create toolbar second time
	if (IsWindow(m_ParticleTreeDock))
	{
		//m_ParticleTreeDock.ShowWindow(SW_SHOW);
		return false;
	}
	CMDIFrameWnd *pMainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	ASSERT(pMainWnd != NULL);
	if (!m_ParticleTreeDock.Create(_T("Particle Tree"), WS_CHILD | WS_VISIBLE, pMainWnd, IDD_PARTICLE_TREEDOCKBAR))
	{
		m_ToolBox->Log(LOGERROR, _T("Failed to create m_ParticleTreeDock window bar\n"));
		return false;
	}
	m_ParticleTreeDock.EnableDocking(CBRS_ALIGN_ANY);
	const DWORD dwBarStyle = CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC;
	m_ParticleTreeDock.SetBarStyle(m_ParticleTreeDock.GetBarStyle() | dwBarStyle);
	// dock property dock bar on left
	pMainWnd->DockControlBar(&m_ParticleTreeDock, AFX_IDW_DOCKBAR_RIGHT, NULL);
	return true;
}

bool CParticleEditor::ShowGraph()
{
	CMDIFrameWnd *pMainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	ASSERT(pMainWnd != NULL);

	// do not create toolbar second time
	if (IsWindow(m_ParticleGraph))
	{
		//BOOL bShown = m_ParticleGraph.ShowWindow(SW_SHOW);
		//if (!bShown)
		//	pMainWnd->DockControlBar(&m_ParticleGraph, AFX_IDW_DOCKBAR_BOTTOM, NULL);
		return false;
	}

	if (!m_ParticleGraph.Create(_T("Particle Graph"), WS_CHILD | WS_VISIBLE, pMainWnd, ID_VIEW_GRAPH))
	{
		m_ToolBox->Log(LOGERROR, _T("Failed to create m_ParticleGraph window bar\n"));
		return false;
	}
	m_ParticleGraph.EnableDocking(CBRS_ALIGN_ANY);
	const DWORD dwBarStyle = CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC;
	m_ParticleGraph.SetBarStyle(m_ParticleGraph.GetBarStyle() | dwBarStyle);
	pMainWnd->DockControlBar(&m_ParticleGraph, AFX_IDW_DOCKBAR_BOTTOM, NULL);
	return true;
}

void CParticleEditor::UpdateDockTree( CView *pSender, LPARAM lHint, CObject *pHint )
{
	m_ParticleTreeDock.OnUpdate(pSender, lHint, pHint);
}
void CParticleEditor::UpdateDockGraph( CParticleDoc * pDoc )
{
	m_ParticleGraph.UpdateGraph(pDoc);
}
