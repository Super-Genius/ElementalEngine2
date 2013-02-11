///============================================================================
/// \file		SplineComponent.cpp
/// \brief		Implementation file for Spline Editor
/// \date		11-14-2006
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

#define SPLINE_EXTENSIONS "xsp"

AFX_EXTENSION_MODULE SplineEditorDLL = { false, NULL, NULL };

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SplineEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(SplineEditorDLL, hInstance))
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

		new CDynLinkLibrary(SplineEditorDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SplineEditor.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(SplineEditorDLL);
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
	return SINGLETONINSTANCE(CSplineEditor);
}

static LPCTSTR OpenFilter = {
	"Spline Files (*.xsp)\0" "*.xsp\0"
	"\0\0"
};

static LPCTSTR SaveFilter = {
	"Spline Files (*.xsp)\0" "*.xsp\0"
	"\0\0"
};

//---------------------------------------------------------------------
// Function:	GetPriority
// Description:	DLL's priority
// Parameters:	.
// Returns:		DWORD = priority of DLL loaded
//---------------------------------------------------------------------
DWORD CSplineEditor::GetPriority()
{
    return IDR_SPLINETYPE;
}

//---------------------------------------------------------------------
// Function:	CSplineEditor
// Description:	Standard Constructor
// Parameters:	pi = programs information structure
// Returns:		.
//---------------------------------------------------------------------
CSplineEditor::CSplineEditor()
{
	SINGLETONINSTANCE(CSplineComponent)->SetParent(this);
	m_ToolBox = EngineGetToolBox();
}

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CSplineEditor::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(SplineEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	DLL's Initialization function, like InitInstance
// Parameters:	.
// Returns:		TRUE, if successful.
//---------------------------------------------------------------------
BOOL CSplineEditor::Initialize()
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
	AfxSetResourceHandle(SplineEditorDLL.hModule);

	// use my string / icon resources
	// Note IDR_EXAMPLETYPE has to be a unique id even between DLLS
	// to get the ICONS right on the Window
	m_DocTemplate = new CMultiDocTemplate(IDR_SPLINETYPE,
			RUNTIME_CLASS(CSplineDoc),
			RUNTIME_CLASS(CSplineFrm),
			RUNTIME_CLASS(CSplineView));

	// put this after setting the menu and accelerators so it won't allocate it's own
	pApp->AddDocTemplate(m_DocTemplate);

	// construct resource info
	m_ObjCollection.name.LoadString(IDS_RESOURCE_NAME);
	m_ObjCollection.pageIndex = &m_PageIndex;

	m_ObjCollection.hIcon = (HICON)LoadImage(SplineEditorDLL.hModule, MAKEINTRESOURCE(IDR_SPLINETYPE), 
		IMAGE_ICON, 0, 0, LR_CREATEDIBSECTION);
	VERIFY(m_ObjCollection.hIcon != NULL);

	// value should be 1 - 1000
	m_ObjCollection.priority = IDR_SPLINETYPE;
	m_ObjCollection.extensions = SPLINE_EXTENSIONS;
	AfxSetResourceHandle(appInst);

    return TRUE;
}

//---------------------------------------------------------------------
// Function:	~CSplineEditor
// Description:	Standard Desctructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CSplineEditor::~CSplineEditor()
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
void CSplineEditor::OnCreate(CMDIFrameWnd *mainWnd)
{
	MENUINFOMESSAGE mim;
	CBitmap menuBitmap;
	HINSTANCE appInst;

	static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(SplineEditorDLL.hModule);

	// TODO <rnguyen>
	// we need to mergeMenus
	//VERIFY(MergeMenus(&m_DocTemplate->m_hMenuShared, mim.m_hMenu, _T("&Window")));

	// and acclerators
	VERIFY(MergeAccelerators(&m_DocTemplate->m_hAccelTable, mim.m_hAccel));

	mim.m_hAccel = NULL;
	mim.m_hMenu = NULL;
	// load up hi-res toolbar icon
	menuBitmap.LoadBitmap(IDB_TOOLBAR);
	// we don't really have to have to toolbar visible do we?
	mim.m_MenuRsrc = IDR_SPLINETYPE;
	mim.m_MenuBitmap = &menuBitmap;

	static DWORD msgHash_SetMenuInfo = CHashString(_T("SetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetMenuInfo, sizeof(MENUINFOMESSAGE), &mim);

	static DWORD msgHash_RegisterResourceCollection = CHashString(_T("RegisterResourceCollection")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterResourceCollection, sizeof(RESOURCECOLLECTION), &m_ObjCollection);

	AfxSetResourceHandle(appInst);
}

//---------------------------------------------------------------------
// Function:	OnCmdMsg
// Description:	Called from the main windows OnCmdMsg function.
//				Routes commands through doc/view architecture
// Returns:		TRUE if handled
//---------------------------------------------------------------------
BOOL CSplineEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return FALSE;
}

//---------------------------------------------------------------------
// Function:	EditDocument
// Description:	Edit an example Document retrieving initial data 
//					from a archive
// Parameters:	ar = archive to load into document
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CSplineEditor::EditDocument( /* IArchive &ar */)
{
	
	CSplineDoc *createDoc;
	// create blank document, with frame
	createDoc = DYNAMIC_DOWNCAST(CSplineDoc, m_DocTemplate->OpenDocumentFile(NULL));

	// now load the document
	createDoc->LoadDocument( /* ar */ );

	return NO_ERROR;
}

bool CSplineEditor::LoadSpline(LOADFILEEXTPARAMS *lfep)
{
	CHashString splineName(_T(""));
	DWORD blankID = splineName.GetUniqueID();

	// create a blank scene if necessary
	static DWORD msgHash_CreateBlankScene = CHashString(_T("CreateBlankScene")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateBlankScene, 0, NULL);

	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), lfep) == MSG_HANDLED)
	{
	}
	else
	{
		// log error
		StdString error;
		error = lfep->fileName;
		error += _T(" not found, when attempting to load.\n");
		m_ToolBox->SetErrorValue(WARN_OBJECT_NOT_FOUND);
		m_ToolBox->Log(LOGWARNING, error);
		return false;
	}

	return true;
}

DWORD CSplineEditor::LoadSplineFromFileDrop(DROPOBJECTINFO *doi)
{
	LOADFILEEXTPARAMS lfep;
	CString extension;

	// grab resource page info
	RESOURCEPAGEINFO *rpi = doi->pResourcePageInfo;

	// get the extension of the item that was dropped
	extension = GetExtension(rpi->filepath);

	// make sure that the extension is Spline
	if (extension != ".xsp")
	{
		// message not for the spline component move on
		return MSG_NOT_HANDLED;
	}

	// send load message to load the spline
	lfep.fileName = rpi->filepath;
	lfep.bInternalLoad = true;

	// create spline object
	if (LoadSpline(&lfep))
	{
		return MSG_HANDLED_STOP;
	}
	// failed return error
	return MSG_ERROR;
}

CString CSplineEditor::GetExtension(const TCHAR *filepath)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath( filepath, drive, dir, fname, ext );
	CString e(ext);
	return CString(e);
}
