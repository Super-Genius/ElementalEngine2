///============================================================================
/// \file		WorldEventEditor.cpp
/// \brief		Implementation of WorldEvent Editor
/// \date		01-30-2007
/// \author		Brian Bazyk
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
#include "WorldEventEditor.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
// major version in upper 16 bits
// low word =  minor version, low bit debug =0, release=1
#define INTERFACEDLLVERSION 0x00006100
#else
#define INTERFACEDLLVERSION 0x00006101
#endif

#define WORLDEVENTEDITOR_PRIORITY 14500

static AFX_EXTENSION_MODULE WorldEventEditorDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("WorldEventEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(WorldEventEditorDLL, hInstance))
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

		new CDynLinkLibrary(WorldEventEditorDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("WorldEventEditor.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(WorldEventEditorDLL);
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
	return SINGLETONINSTANCE(CWorldEventEditor);
}

CWorldEventEditor::CWorldEventEditor() : 
	m_hsWorldEventRenderObjectClass(_T("CWorldEventRenderObject")),
	m_hsWorldEventRenderObjectName(_T("WorldEventRenderObject"))
{
	SINGLETONINSTANCE(CWorldEventEditorComponent)->SetParent(this);
	m_ToolBox = EngineGetToolBox();
	m_hAccel = NULL;
	m_hMenu = NULL;

	m_bSelectedObject = false;
	m_bWaitingForObjectSelection = false;
	m_bRenderObjectShown = false;
}

CWorldEventEditor::~CWorldEventEditor()
{
	SetVisualization(false);

	if (m_hAccel != NULL)
	{
		DestroyAcceleratorTable(m_hAccel);
		m_hAccel = NULL;
	}
	if (m_hMenu != NULL)
	{
		DestroyMenu(m_hMenu);
		m_hMenu = NULL;
	}
}

BOOL CWorldEventEditor::Initialize()
{
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
/*
	// walk all templates in the application
	CMultiDocTemplate* pTemplate;
	pTemplate = (CMultiDocTemplate *)FindDocumentTemplate(IDR_RENDERVIEWERTYPE);
	assert(pTemplate != NULL);

	// now tell the application to look in dll for resources
	AfxSetResourceHandle(WorldEventEditorDLL.hModule);

	// merge our menu/accelerator into the renderviewer menu
	m_hMenu = LoadMenu(WorldEventEditorDLL.hModule, MAKEINTRESOURCE(IDR_RENDERWORLDEVENTVIEWER));
	// we have no accelarators for this
	m_hAccel = LoadAccelerators(WorldEventEditorDLL.hModule, MAKEINTRESOURCE(IDR_RENDERWORLDEVENTVIEWER));
	ASSERT(m_hMenu != NULL);
	ASSERT(m_hAccel != NULL);

	// source has to what we merge into
	VERIFY(MergeSubMenus(&pTemplate->m_hMenuShared, m_hMenu));
	// you have to merge accelerators with new menu
	VERIFY(MergeAccelerators(&pTemplate->m_hAccelTable, m_hAccel));
*/
	AfxSetResourceHandle(appInst);


	return TRUE;
}

// our onCreate needs to catch the 
void CWorldEventEditor::OnCreate(CMDIFrameWnd *mainWnd)
{
	if (!m_WorldEventToolPal.Create(mainWnd, CWorldEventToolPal::IDD,
		CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY, CWorldEventToolPal::IDD))
	{
		TRACE0("Failed to create dockbar\n");
		return;      // fail to create
	}

	TOOLBARSTRUCT atbs;

	atbs.hIcon = (HICON)LoadImage(WorldEventEditorDLL.hModule, 
		MAKEINTRESOURCE(IDR_WORLDEVENTICON), IMAGE_ICON, 0, 0, 0);
	atbs.Text = _T("World Event Editor");
	atbs.cBar = &m_WorldEventToolPal;
	static DWORD msgHash_AddToolBar = CHashString(_T("AddToolBar")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddToolBar, sizeof(TOOLBARSTRUCT), &atbs);

	// Dock Win
	if (m_WorldEventDockWin.Create(_T("World Events"), WS_CHILD | WS_VISIBLE, mainWnd, IDD_WORLDEVENT_DOCK_DLG))
	{
		CRect winRect;
		mainWnd->GetWindowRect(&winRect);
		m_WorldEventDockWin.EnableDocking(CBRS_ALIGN_ANY);
		m_WorldEventDockWin.SetBarStyle(m_WorldEventDockWin.GetBarStyle() |
			CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		mainWnd->DockControlBar(&m_WorldEventDockWin, AFX_IDW_DOCKBAR_RIGHT, winRect);
		m_WorldEventDockWin.LockPushPin(FALSE);
	}
}

BOOL CWorldEventEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (m_WorldEventCmdTarget.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	if (m_WorldEventDockWin.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	return FALSE;
}

CString* CWorldEventEditor::GetName( void )
{
	static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(WorldEventEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

DWORD CWorldEventEditor::GetPriority()
{
	return WORLDEVENTEDITOR_PRIORITY;
}

void CWorldEventEditor::SetSelectedObject( IHashString *pObjectName )
{
	if (!m_bWaitingForObjectSelection)
	{
		if (pObjectName)
		{
			m_bSelectedObject = true;
			m_SelectedObjectName.Init( pObjectName->GetString() );
		}
		else
		{
			m_bSelectedObject = false;
			m_SelectedObjectName.Init( _T("") );
		}
	}

	if (m_WorldEventDockWin.m_hWnd)
	{
		m_WorldEventDockWin.SetSelectedObject( pObjectName );
	}
}

bool CWorldEventEditor::AddWorldEventToEntity( IHashString *pSrcEntityName, IHashString *pSrcStateEventName, IHashString *pDstEntityName, IHashString *pDstStateEventName )
{
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	static DWORD msgHash_AddObject = CHashString(_T("AddObject")).GetUniqueID();
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	static DWORD msgHash_QueryEvents = CHashString(_T("QueryStateMachineEvents")).GetUniqueID();
	static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
	static DWORD msgHash_FindWorldEventTrigger = CHashString(_T("FindWorldEventTrigger")).GetUniqueID();
	CHashString hszStateObjectType(_T("CQHStateObject"));
	CHashString hszSceneName( _T("World") );

	static CHashString hszWorldEventType = CHashString(_T("CWorldEvent"));
	static CHashString hszWorldEventTriggerType = CHashString(_T("CWorldEventTrigger"));

	static CHashString hszWorldEventBaseName = CHashString(_T("WorldEvent"));
	static CHashString hszWorldEventTriggerBaseName = CHashString(_T("WorldEventTrigger"));

	DWORD result;

	QUERYEVENTSPARAMS qeparams;
	result = m_ToolBox->SendMessage(msgHash_QueryEvents, sizeof(QUERYEVENTSPARAMS), &qeparams, pDstEntityName, &hszStateObjectType );
	// return immediatly if there are no state events at all on the dst object
	if (qeparams.QueryEventsList.size() == 0)
		return false;

	// if no dst state event name is specified, use the first one found as a default
	if (pDstStateEventName == NULL)
		pDstStateEventName = qeparams.QueryEventsList.begin()->EventName;
	else
	{
		// make sure target entity has the state event to link to
		QUERYEVENTSLIST::iterator itr;
		itr = qeparams.QueryEventsList.begin();
		while( itr != qeparams.QueryEventsList.end() )
		{
			if (*itr->EventName == *pDstStateEventName)
				break;
			itr++;
		}
		// state event was not found on the target entity
		if (itr == qeparams.QueryEventsList.end())
			return false;
	}

	// find an existing world event trigger we can attach to
	CHashString hszWorldEventTriggerName;
	FINDWORLDEVENTPARAMS fwep;
	fwep.pEntityName = pSrcEntityName;
	fwep.pEntityType = NULL;
	fwep.pEventName = pSrcStateEventName;
	fwep.pFoundTriggerName = &hszWorldEventTriggerName;
	result = m_ToolBox->SendMessage(msgHash_FindWorldEventTrigger, sizeof(fwep), &fwep );
	// if an existing trigger is not found, make a new one
	if (!fwep.bFound)
	{
		// create a unique name for the world event trigger
		GENERATEUNIQUEOBJECTNAMEPARAMS generateNameParams;
		generateNameParams.name = &hszWorldEventTriggerBaseName;
		generateNameParams.newname = &hszWorldEventTriggerName;
		DWORD result = m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(generateNameParams), &generateNameParams );
		if (result != MSG_HANDLED)
			return false;

		CREATEOBJECTPARAMS cop;
		cop.parentName = &hszSceneName;
		cop.typeName = &hszWorldEventTriggerType;
		cop.name = &hszWorldEventTriggerName;
 		m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(cop), &cop);

//		m_ToolBox->SendMessage(msgHash_AddObject, sizeof(cop), &cop);

		CREATEARCHIVE ca;
		CHashString streamType(_T("Memory"));
		ca.streamData = NULL;
		ca.streamSize = 0;
		ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
		ca.streamType = &streamType;
		m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(ca), &ca);
		IArchive *MemArchive = (ca.archive);

		MemArchive->Write( pSrcEntityName->GetString() );
		MemArchive->Write( _T("") );
		MemArchive->Write( pSrcStateEventName->GetString() );

		SERIALIZEOBJECTPARAMS sop;
		sop.name = &hszWorldEventTriggerName;
		sop.archive = MemArchive;
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop);

		INITOBJECTPARAMS iop;
		iop.name = &hszWorldEventTriggerName;
		result = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(iop), &iop);

		MemArchive->Close();

		if (result != MSG_HANDLED)
			return false;
	}

	// create a unique name for the world event
	GENERATEUNIQUEOBJECTNAMEPARAMS generateNameParams;
	CHashString hszWorldEventName;
	generateNameParams.name = &hszWorldEventBaseName;
	generateNameParams.newname = &hszWorldEventName;
	result = m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(generateNameParams), &generateNameParams );
	if (result != MSG_HANDLED)
		return false;

	CREATEOBJECTPARAMS cop;
	cop.parentName = &hszSceneName;
	cop.typeName = &hszWorldEventType;
	cop.name = &hszWorldEventName;
 	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

//	m_ToolBox->SendMessage(msgHash_AddObject, sizeof(cop), &cop);

	CREATEARCHIVE ca;
	CHashString streamType(_T("Memory"));
	ca.streamData = NULL;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamType = &streamType;
	m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(ca), &ca);
	IArchive *MemArchive = (ca.archive);

	MemArchive->Write( hszWorldEventTriggerName.GetString() );	// trigger name
	MemArchive->Write( pDstEntityName->GetString() );			// entity name
	MemArchive->Write( _T("") );								// entity group
	MemArchive->Write( pDstStateEventName->GetString() );		// event name
	MemArchive->Write( _T("") );								// event param

	SERIALIZEOBJECTPARAMS sop;
	sop.name = &hszWorldEventName;
	sop.archive = MemArchive;
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop);

	INITOBJECTPARAMS iop;
	iop.name = &hszWorldEventName;
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(iop), &iop);

	MemArchive->Close();

	if (result != MSG_HANDLED)
		return false;

	return true;
}

void CWorldEventEditor::SetVisualization( bool bShow )
{
	if (bShow && !m_bRenderObjectShown)
	{
		CREATEOBJECTPARAMS cop;
		cop.name = &m_hsWorldEventRenderObjectName;
		cop.parentName = NULL;
		cop.typeName = &m_hsWorldEventRenderObjectClass;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		if( m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop ) != MSG_HANDLED )
		{
			m_ToolBox->Log( LOGERROR, _T("CWorldEventEditor: Failed to create world event render object!\n") );
		}

		INITOBJECTPARAMS iop;
		iop.name = &m_hsWorldEventRenderObjectName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop );

		m_bRenderObjectShown = true;
	}
	else if (!bShow && m_bRenderObjectShown)
	{
		DELETEOBJECTPARAMS dop;
		dop.name = &m_hsWorldEventRenderObjectName;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop );

		m_bRenderObjectShown = false;
	}
}
/*
void CWorldEventEditor::CreateGUIPage()
{
}

void CWorldEventEditor::CreateGUITextType()
{
	local oldVersion = CLuaHashString:new_local()
	local currentVersion = CLuaHashString:new_local("2.0a")
	oldVersion = GetFileVersion()
	SetFileVersion(currentVersion)
	
	local instanceName = string.format("%s_DebugInstance", name)
	local hszInstName = CLuaHashString:new_local(instanceName)
	local hszInstType = CLuaHashString:new_local("CGUIInstance")

	local objectName = string.format("%s_DebugObject", name)
	local hszObjName = CLuaHashString:new_local(objectName)

	-- make static text object
	local staticTextArchive = CLuaArchive:new_local()
	staticTextArchive:InitArchive(3, 512,  tolua.cast(CLuaHashString("Memory"), "IHashString") );
	
	staticTextArchive:WriteTCHARStr("basedata\\editor\\textures\\none.tga") -- NormTexture
	staticTextArchive:WriteTCHARStr("TESTING") -- StaticText
	staticTextArchive:WriteTCHARStr("") -- Filename
	staticTextArchive:WriteBool(false) -- LoadFromFile
	staticTextArchive:WriteTCHARStr("Fonts\\comic.ttf") --FontName
	staticTextArchive:WriteInt(12) -- Fontsize
	staticTextArchive:WriteInt(itextColor) -- NormFontColor
	staticTextArchive:WriteInt(itextColor) -- HighFontColor
	staticTextArchive:WriteInt(itextColor) -- SelectFontColor
	staticTextArchive:WriteInt(itextColor) -- GrayFontColor
	staticTextArchive:WriteInt(0) -- TextOffSetX
	staticTextArchive:WriteInt(0) -- TextOffSetY
	staticTextArchive:WriteBool(false) -- CenterText
	staticTextArchive:WriteBool(true) -- HideTexture

	local stextCompType = CLuaHashString:new_local("CGUIStaticText")

	-- create object
	CreateObject(hszObjName, stextCompType, nil)
	
	-- serialize object
	SerializeObject(hszObjName, staticTextArchive)
	
	-- init object
	InitObject(hszObjName)

}

void CWorldEventEditor::CreateGUITextInstance()

	-- make instance
	local instanceArchive = CLuaArchive:new_local()
	instanceArchive:InitArchive(3, 512,  tolua.cast(CLuaHashString("Memory"), "IHashString") );
	
	-- pos
	instanceArchive:WriteFloat(0.0)
	instanceArchive:WriteFloat(0.0)
	-- size
	instanceArchive:WriteFloat(512.0)
	instanceArchive:WriteFloat(512.0)
	-- z order
	instanceArchive:WriteInt(3)
	-- color 
	instanceArchive:WriteInt(-1)
	-- alpha
	instanceArchive:WriteInt(255) -- test! make 0
	-- angle
	instanceArchive:WriteFloat(0.0)
	-- instance info
	instanceArchive:WriteTCHARStr(objectName)
	instanceArchive:WriteTCHARStr("CGUIStaticText")
	
	-- create object
	CreateObject(hszInstName, hszInstType, nil)	
	
	-- serialize object
	SerializeObject(hszInstName, instanceArchive)

	-- init object
	InitObject(hszInstName)
	
	AddGUIElementToPage("Game HUD", instanceName)
}
*/