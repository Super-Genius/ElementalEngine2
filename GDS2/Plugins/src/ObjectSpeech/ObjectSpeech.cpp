//=====================================================================
// File:		ObjectSpeech.cpp
// Description: Defines the initialization routines for the DLL.
// Started:		10/31/2001
// Original:	Kenneth Hurley
//
// Modified:	06/30/2003
// Editor:		D. Patrick Ghiocel
//
//=====================================================================

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AFX_EXTENSION_MODULE ObjectSpeechDLL = { NULL, NULL };
CObjectSpeech *gObjectSpeech;


extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("ObjectSpeech.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(ObjectSpeechDLL, hInstance))
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

		new CDynLinkLibrary(ObjectSpeechDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("ObjectSpeech.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(ObjectSpeechDLL);
		delete gObjectSpeech;
	}
	return 1;   // ok
}

extern "C" DWORD GetDLLVersion()
{
	return INTERFACEDLLVERSION;
}

// Exported DLL initialization is run in context of running application
extern "C" CDLLInterface *GetDLLInterface(CProgramInfo *pi)
{
	// this has to be done after module is intialized.
	gObjectSpeech = new CObjectSpeech(pi);
	return gObjectSpeech;
}

static LPTSTR OpenFilter = {
	"Speech TXT (*.txt)\0" "*.txt\0"
	"Speech WAV (*.wav)\0" "*.wav\0"
	"Speech XML (*.xml)\0" "*.xml\0"
	"\0\0"
};

static LPTSTR SaveFilter = {
	"Speech TXT (*.txt)\0" "*.txt\0"
	"Speech WAV (*.wav)\0" "*.wav\0"
	"Speech XML (*.xml)\0" "*.xml\0"
	"\0\0"
};

//---------------------------------------------------------------------
// Function:	CObjectSpeech
// Description:	Standard Constructor
// Parameters:	pi = programs information structure
// Returns:		.
//---------------------------------------------------------------------
CObjectSpeech::CObjectSpeech(CProgramInfo *ProgramInfo)
{
	// save program information for later
    m_ProgramInfo = ProgramInfo;
	m_Priority = IDR_SpeechTYPE;

	m_LoadStrings = OpenFilter;
	m_SaveStrings = SaveFilter;
	m_ThumbNail = NULL;					// we'll generate one later WIP

	CoInitialize( NULL );
//	Test_Speech();
}

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CObjectSpeech::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(ObjectSpeechDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	DLL's Initialization function, like InitInstance
// Parameters:	.
// Returns:		TRUE, if successful.
//---------------------------------------------------------------------
BOOL CObjectSpeech::Initialize()
{
	CString tName;
	HINSTANCE appInst;
	CMDIFrameWnd *mainWnd;
		
	// Register the doc templates we provide to the app
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
    if (pApp == NULL)
    {
        return FALSE;
    }

	mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(ObjectSpeechDLL.hModule);

	// use my string / icon resources
	// Note IDR_SpeechTYPE has to be a unique id even between DLLS
	// to get the ICONS right on the Window
	m_DocTemplate = new CMultiDocTemplate(IDR_SpeechTYPE,
			RUNTIME_CLASS(CSpeechDoc),
			RUNTIME_CLASS(CSpeechFrm),
			RUNTIME_CLASS(CSpeechView));

	// put this after setting the menu and accelerators so it won't allocate it's own
	pApp->AddDocTemplate(m_DocTemplate);

	VERIFY(MergeMenus(&m_DocTemplate->m_hMenuShared, *(m_ProgramInfo->curMenu), _T("&Window")));	
	VERIFY(MergeAccelerators(&m_DocTemplate->m_hAccelTable, *(m_ProgramInfo->curAccel)));

	// construct resource info
	m_ObjInfo.name.LoadString(IDS_RESOURCE_NAME);
	m_ObjInfo.pageIndex = &m_PageIndex;

	m_ObjInfo.icon = (HICON)LoadImage(ObjectSpeechDLL.hModule, MAKEINTRESOURCE(IDR_SpeechTYPE), 
		IMAGE_ICON, 0, 0, LR_CREATEDIBSECTION);
	VERIFY(m_ObjInfo.icon != NULL);

	// value should be 1 - 1000
	m_ObjInfo.priority = IDR_SpeechTYPE;
	m_ObjInfo.objFlags = OBJFLAGS_EDITABLE | OBJFLAGS_EXPORTABLE |
		OBJFLAGS_IMPORTABLE | OBJFLAGS_THUMBNAIL;


	AfxSetResourceHandle(appInst);


//	m_Rate = 0;
//	m_Volume = 0;



    return TRUE;
}

//---------------------------------------------------------------------
// Function:	~CObjectSpeech
// Description:	Standard Desctructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CObjectSpeech::~CObjectSpeech()
{
	CoUninitialize();

	if (m_ObjInfo.icon != NULL)
	{
		DestroyIcon(m_ObjInfo.icon);
		m_ObjInfo.icon = NULL;
	}
}

//---------------------------------------------------------------------
// Function:	Save
// Description:	Save data to a file
// Parameters:	filename = name of file to export
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CObjectSpeech::Save(LPTSTR filename)
{
	return NO_ERROR;
}

//---------------------------------------------------------------------
// Function:	Load
// Description:	LOad data from a file
// Parameters:	filename = name of file to export
// Returns:		DWORD = possible error or 0 for no error
//---------------------------------------------------------------------
DWORD CObjectSpeech::Load(LPTSTR filename)
{
	m_DocTemplate->OpenDocumentFile(filename);
	return NO_ERROR;
}

//---------------------------------------------------------------------
// Function:	Edit
// Description:	Invokes editor for this resource
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
void CObjectSpeech::Edit()
{
}

//---------------------------------------------------------------------
// Function:	ChangeProperties
// Description:	Invoke a change properties command? WIP
// Parameters:	itemID = ID of item to change
// Returns:		.
//---------------------------------------------------------------------
void CObjectSpeech::ChangeProperties(DWORD itemID)
{
}

//---------------------------------------------------------------------
// Function:	GetProperties
// Description:	Get a property? WIP
// Parameters:	itemID = ID of item to get
// Returns:		property
//---------------------------------------------------------------------
DWORD CObjectSpeech::GetProperties(DWORD itemID)
{
	return 0;
}