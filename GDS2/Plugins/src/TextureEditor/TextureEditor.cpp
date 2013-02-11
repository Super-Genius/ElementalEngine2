//=====================================================================
// File:		TextureEditor.cpp
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

AFX_EXTENSION_MODULE TextureEditorDLL = { NULL, NULL };

#define TEXTURE_EXTENSIONS "dds\\tga\\bmp\\png"

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("TextureEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(TextureEditorDLL, hInstance))
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

		new CDynLinkLibrary(TextureEditorDLL);
		// this has to be done after module is intialized.
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("TextureEditor.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(TextureEditorDLL);
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
	return SINGLETONINSTANCE(CTextureEditor);
}

static LPCTSTR OpenFilter = {
	"DevIL Files (*.oil)\0" "*.oil\0"
	"DirectX Image Files (*.dds)\0" "*.dds\0"
	"Graphics Interchange Format (*.gif)\0" "*.gif\0"
	"Half-Life Model Files (*.mdl)\0" "*.mdl\0"
	"Homeworld Image Files (*.lif)\0" "*.lif\0"
	"Jpeg Files (*.jpe, *.jpg, *.jpeg)\0" "*.jpe;*.jpg;*.jpeg\0"
	"Microsoft Bitmap Files (*.bmp)\0" "*.bmp\0"
	"Microsoft Icon Files (*.ico)\0" "*.ico\0"
	"Portable AnyMap Files (*.pbm, *.pgm, *.pnm, *.ppm)\0" "*.pbm;*.pgm;*.pnm;*.ppm\0"
	"Portable Network Graphics Files (*.png)\0" "*.png\0"
	"Sgi Files (*.sgi)\0" "*.bw;*.rgb;*.rgba;*.sgi\0"
	"Targa Files (*.tga)\0" "*.tga\0"
	"Tiff Files (*.tif)\0" "*.tif;*.tiff\0"
	"Quake Wal Files (*.wal)\0" "*.wal\0"
	"ZSoft Pcx Files (*.pcx)\0" "*.pcx\0"
	"\0\0"
};

static LPTSTR SaveFilter = {
	"C-Style Header (*.h)\0" "*.h\0"
	"DevIL Files (*.oil)\0" "*.oil\0"
	"Direct X Files (*.dds)\0" "*.dds\0"
	"Jpeg Files (*.jpe, *.jpg, *.jpeg)\0" "*.jpe;*.jpg;*.jpeg\0"
	"Microsoft Bitmap Files (*.bmp)\0" "*.bmp\0"
	"Portable AnyMap Files (*.pbm, *.pgm, *.ppm)\0" "*.pbm;*.pgm;*.ppm\0"
	"Portable Network Graphics Files (*.png)\0" "*.png\0"
	"Sgi Files (*.sgi)\0" "*.bw;*.rgb;*.rgba;*.sgi\0"
	"Targa Files (*.tga)\0" "*.tga\0"
	"ZSoft Pcx Files (*.pcx)\0" "*.pcx\0"
	"\0\0"
};

//---------------------------------------------------------------------
// Function:	GetPriority
// Description:	DLL's priority
// Parameters:	.
// Returns:		DWORD = priority of DLL loaded
//---------------------------------------------------------------------
DWORD CTextureEditor::GetPriority()
{
    return IDR_TEXTURETYPE;
}

//---------------------------------------------------------------------
// Function:	CTextureEditorr
// Description:	Standard Constructor
// Parameters:	pi = program's information structure
// Returns:		.
//---------------------------------------------------------------------
CTextureEditor::CTextureEditor()
{
	SINGLETONINSTANCE(CTextureComponent)->SetParent(this);
	m_ToolBox = EngineGetToolBox();
}

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CTextureEditor::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(TextureEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	Initialization like InitInstace
// Parameters:	.
// Returns:		TRUE, if successful
//---------------------------------------------------------------------
BOOL CTextureEditor::Initialize()
{
	CString tName;
	
	// Register the doc templates we provide to the app
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
    if (pApp == NULL)
    {
		return FALSE;
    }

	// get the apps instance handle for loading resources
	HINSTANCE appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(TextureEditorDLL.hModule);

	// use my string / icon resources
	// *** note IDR_TEXTURE type needs to be unique ID even between
	// dlls
	m_DocTemplate = new CMultiDocTemplate(IDR_TEXTURETYPE,
			RUNTIME_CLASS(CTextureDoc),
			RUNTIME_CLASS(CMDIChildWnd),
			RUNTIME_CLASS(CTextureView));
	pApp->AddDocTemplate(m_DocTemplate);

	// construct resource info
	m_ObjCollection.name.LoadString(IDS_OBJECT_NAME);
	m_ObjCollection.pageIndex = &m_PageIndex;
	m_ObjCollection.priority = IDR_TEXTURETYPE; // value should be 1 - 1000
	m_ObjCollection.extensions = TEXTURE_EXTENSIONS;
	m_ObjCollection.hIcon = (HICON)LoadImage(TextureEditorDLL.hModule, MAKEINTRESOURCE(IDR_TEXTURETYPE), IMAGE_ICON, 0, 0, LR_CREATEDIBSECTION);
	VERIFY(m_ObjCollection.hIcon != NULL);
	
	AfxSetResourceHandle(appInst);

	return TRUE;
}

void CTextureEditor::OnCreate( CMDIFrameWnd* mainWnd )
{
	//mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);

	// get the apps instance handle for loading resources
	HINSTANCE appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(TextureEditorDLL.hModule);

	MENUINFOMESSAGE menuInfo;
	static DWORD msgHash_GetMenuInfo = CHashString(_T("GetMenuInfo")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetMenuInfo, sizeof(MENUINFOMESSAGE), &menuInfo) != MSG_HANDLED)
	{
		ASSERT(0);
	}
    
//	m_hMenu = LoadMenu(TextureEditorDLL.hModule, MAKEINTRESOURCE(IDR_TEXTURETYPE));
//	m_hAccel = LoadAccelerators(TextureEditorDLL.hModule, MAKEINTRESOURCE(IDR_TEXTURETYPE));
	VERIFY(MergeMenus(&m_DocTemplate->m_hMenuShared, menuInfo.m_hMenu, _T("&Window")));
	VERIFY(MergeAccelerators(&m_DocTemplate->m_hAccelTable, menuInfo.m_hAccel));
	menuInfo.m_hMenu = m_DocTemplate->m_hMenuShared;
	menuInfo.m_hAccel = m_DocTemplate->m_hAccelTable;
	menuInfo.m_MenuRsrc = NULL;
	menuInfo.m_MenuBitmap = NULL;
	static DWORD msgHash_SetMenuInfo = CHashString(_T("SetMenuInfo")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetMenuInfo, sizeof(MENUINFOMESSAGE), &menuInfo);

	// register an object collection page.
	static DWORD msgHash_RegisterResourceCollection = CHashString(_T("RegisterResourceCollection")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterResourceCollection, sizeof(RESOURCECOLLECTION), &m_ObjCollection);

	//ID_TEXTEDIT_APPERTURE

	AfxSetResourceHandle(appInst);
}

BOOL CTextureEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if( m_DocTemplate->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) )
	{
		return TRUE;
	}

	if (m_TextureEditorCmdTarget.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) )
	{
        return TRUE;
	}

	return FALSE;
}

//---------------------------------------------------------------------
// Function:	~CTextureEditorr
// Description:	Standard Desctructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CTextureEditor::~CTextureEditor()
{

#if 0
	if (m_ObjCollection.icon != NULL)
	{
		DestroyIcon(m_ObjCollection.icon);
		m_ObjCollection.icon = NULL;
	}
#endif
}

DWORD CTextureEditor::OpenTexture( LPTSTR filename )
{
	m_DocTemplate->OpenDocumentFile(filename);

	return NO_ERROR;
}