//=====================================================================
// File:		NodeSystemEditor.cpp
// Description: Defines the initialization routines for the DLL.
// Started:		08/1/2007
// Original:	Richard Nguyen
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
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AFX_EXTENSION_MODULE NodeSystemEditorDLL = { false, NULL, NULL };

static UINT AddOptionPageMessage = RegisterWindowMessage(ADD_OPTION_PAGE_MSG);

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("NodeSystemEditor.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(NodeSystemEditorDLL, hInstance))
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

		new CDynLinkLibrary(NodeSystemEditorDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("NodeSystemEditor.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(NodeSystemEditorDLL);
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
	return SINGLETONINSTANCE(CNodeSystemEditor);
}

CNodeSystemEditor::CNodeSystemEditor()
{
	SINGLETONINSTANCE(CNodeComponent)->SetParent(this);
	m_NodeToolPal.SetParent( this );
	m_ToolBox = EngineGetToolBox();
	m_pMainWnd = NULL;
	m_bNodePlot = false;
	m_bConnectionPlot = false;
	m_hszLastNodeName = _T("");
	m_hszSelectedObjectName = _T("");
	m_hszSelectedObjectType = _T("");
}

CNodeSystemEditor::~CNodeSystemEditor()
{
	if (m_ObjCollection.hIcon != NULL)
	{
		DestroyIcon(m_ObjCollection.hIcon);
		m_ObjCollection.hIcon = NULL;
	}
}

//---------------------------------------------------------------------
// Function:	GetPriority
// Description:	DLL's priority
// Parameters:	.
// Returns:		DWORD = priority of DLL loaded
//---------------------------------------------------------------------
DWORD CNodeSystemEditor::GetPriority()
{
    return IDR_NODESYSTEMTYPE;
}

//---------------------------------------------------------------------
// Function:	CNodeSystemEditor
// Description:	Standard Constructor
// Parameters:	pi = programs information structure
// Returns:		.
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// Function:	GetName 
// Description:	DLL's module name
// Parameters:	.
// Returns:		CString * = pointer to name of loaded DLL
//---------------------------------------------------------------------
CString *CNodeSystemEditor::GetName()
{
    static CString name;
    LPTSTR tBuff;

    tBuff = name.GetBuffer(_MAX_PATH+1);
	GetModuleFileName(NodeSystemEditorDLL.hModule, tBuff, _MAX_PATH);
    name.ReleaseBuffer();

    return &name;
}

//---------------------------------------------------------------------
// Function:	Initialize
// Description:	DLL's Initialization function, like InitInstance
// Parameters:	.
// Returns:		TRUE, if successful.
//---------------------------------------------------------------------
BOOL CNodeSystemEditor::Initialize()
{
	static DWORD msgHash_EnableNodeSystemRender = CHashString(_T("EnableNodeSystemRender")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_EnableNodeSystemRender, 0, NULL );

    return TRUE;
}

//---------------------------------------------------------------------
// Function:	~CNodeSystemEditor
// Description:	Standard Desctructor
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------

// our onCreate needs to catch the 
void CNodeSystemEditor::OnCreate(CMDIFrameWnd *mainWnd)
{
	m_pMainWnd = mainWnd;

	if (!m_NodeToolPal.Create(mainWnd, CNodeToolPal::IDD,
      CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY, CNodeToolPal::IDD))
	{
		TRACE0("Failed to create dockbar\n");
		return;      // fail to create
	}

	TOOLBARSTRUCT atbs;

	atbs.Text = _T("Node System");
	atbs.cBar = &m_NodeToolPal;
	static DWORD msgHash_AddToolBar = CHashString(_T("AddToolBar")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddToolBar, sizeof(TOOLBARSTRUCT), &atbs);
}

BOOL CNodeSystemEditor::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return FALSE;
}

void CNodeSystemEditor::ObjectSelected( IHashString *hszObjectName, IHashString *hszComponentType )
{
	if( hszObjectName != NULL )
	{
		m_hszSelectedObjectName = hszObjectName;
	}

	if( hszComponentType != NULL )
	{
		m_hszSelectedObjectType = hszComponentType;
	}

	m_NodeToolPal.ObjectSelected( hszObjectName, hszComponentType );
}

void CNodeSystemEditor::GetParentObjectName( IHashString *hszParentObjectName, IHashString *hszObjectName, IHashString *hszComponentType )
{
	if( hszParentObjectName != NULL )
	{
		if( _tcscmp( hszComponentType->GetString(), _T("CNode") ) == 0 ||
			_tcscmp( hszComponentType->GetString(), _T("CNodeConnection") ) == 0 )
		{
			CHashString hszComponent( _T("CEntity") );
			static DWORD msgHash_GetParentName = CHashString(_T("GetParentName")).GetUniqueID();
			m_ToolBox->SendMessage( msgHash_GetParentName, sizeof(IHashString), hszParentObjectName, hszObjectName, &hszComponent );
		}
	}
}

void CNodeSystemEditor::ActivateNodePlot()
{
	m_bNodePlot = true;
}

void CNodeSystemEditor::DeactivateNodePlot()
{
	m_bNodePlot = false;
}

void CNodeSystemEditor::ActivateConnectionPlot()
{
	m_bConnectionPlot = true;
}

void CNodeSystemEditor::DeactivateConnectionPlot()
{
	m_bConnectionPlot = false;
}

void CNodeSystemEditor::PlotNode()
{
	CHashString hszRootName( _T("Node") );
	CHashString hszName( _T("") );
	CHashString hszParentName( _T("") );

	if( _tcscmp( m_hszSelectedObjectType.GetString(), _T("CNodeSystem") ) == 0 )
	{
		hszParentName = m_hszSelectedObjectName;
	}
	else
	{
		GetParentObjectName( &hszParentName, &m_hszSelectedObjectName, &m_hszSelectedObjectType );
	}

	GENERATEUNIQUEOBJECTNAMEPARAMS guonp;
	guonp.name = &hszRootName;
	guonp.newname = &hszName;
	static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_GenerateUniqueObjectName, sizeof(guonp), &guonp );

	// create new scene root
	CREATEOBJECTPARAMS cop;
	cop.name = &hszName;
	cop.parentName = &hszParentName;

	CHashString hszNodeType( _T("CNode") );
	cop.typeName = &hszNodeType;

	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(cop), &cop );

	static DWORD msgHash_AddObject = CHashString(_T("AddObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddObject, sizeof(cop), &cop);

	INITOBJECTPARAMS iop;
	iop.name = &hszName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(iop), &iop);

	SELECTOBJECTPARAMS sop;
	sop.object = &hszName;
	sop.compType = &hszNodeType;
	static DWORD msgHash_SelectObject = CHashString(_T("SelectObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SelectObject, sizeof(sop), &sop );

	MoveObjectInFrontOfCamera(&hszName);

	if( m_bConnectionPlot && _tcscmp( m_hszLastNodeName.GetString(), _T("") ) != 0 )
	{
		PlotConnection( &hszParentName, &hszName );
	}
	m_hszLastNodeName = hszName;
}

void CNodeSystemEditor::PlotNode( Vec3 *plotPosition )
{
	if( m_bNodePlot )
	{
		CHashString hszRootName( _T("Node") );
		CHashString hszName( _T("") );
		CHashString hszParentName( _T("") );

		if( _tcscmp( m_hszSelectedObjectType.GetString(), _T("CNodeSystem") ) == 0 )
		{
			hszParentName = m_hszSelectedObjectName;
		}
		else
		{
			GetParentObjectName( &hszParentName, &m_hszSelectedObjectName, &m_hszSelectedObjectType );
		}

		GENERATEUNIQUEOBJECTNAMEPARAMS guonp;
		guonp.name = &hszRootName;
		guonp.newname = &hszName;
		static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_GenerateUniqueObjectName, sizeof(guonp), &guonp );

		// create new scene root
		CREATEOBJECTPARAMS cop;
		cop.name = &hszName;
		cop.parentName = &hszParentName;

		CHashString hszNodeType( _T("CNode") );
		cop.typeName = &hszNodeType;

		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(cop), &cop );

		static DWORD msgHash_AddObject = CHashString(_T("AddObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_AddObject, sizeof(cop), &cop);

		INITOBJECTPARAMS iop;
		iop.name = &hszName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(iop), &iop);

		SELECTOBJECTPARAMS sop;
		sop.object = &hszParentName;
		sop.compType = &m_hszSelectedObjectType;
		static DWORD msgHash_SelectObject = CHashString(_T("SelectObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SelectObject, sizeof(sop), &sop );

		static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), plotPosition, &hszName );

		if( m_bConnectionPlot && _tcscmp( m_hszLastNodeName.GetString(), _T("") ) != 0 )
		{
			PlotConnection( &hszParentName, &hszName );
		}
		m_hszLastNodeName = hszName;
	}
}

void CNodeSystemEditor::PlotConnection( IHashString *hszParentName, IHashString *hszConnectedNode )
{
	CHashString hszRootName("NodeConnection");
	CHashString hszConnectionName( _T("NodeConnection") );
	GENERATEUNIQUEOBJECTNAMEPARAMS guonp;
	guonp.name = &hszRootName;
	guonp.newname = &hszConnectionName;
	static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_GenerateUniqueObjectName, sizeof(guonp), &guonp );

	CREATEOBJECTPARAMS cop;
	cop.name = &hszConnectionName;
	cop.parentName = hszParentName;
	CHashString hszNodeConnectionType("CNodeConnection");
	cop.typeName = &hszNodeConnectionType;

	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(cop), &cop );
	static DWORD msgHash_AddObject = CHashString(_T("AddObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddObject, sizeof(cop), &cop);

	CREATEARCHIVE ca;
	CHashString streamType(_T("Memory"));
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamType = &streamType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(ca), &ca);
	IArchive *MemArchive = (ca.archive);

	MemArchive->Write( 0.0f );
	MemArchive->Write(  m_hszLastNodeName.GetString() );
	MemArchive->Write( hszConnectedNode->GetString() );

	SERIALIZEOBJECTPARAMS sop;
	sop.name = &hszConnectionName;
	sop.archive = MemArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop);
	MemArchive->Close();

	INITOBJECTPARAMS iop;
	iop.name = &hszConnectionName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(iop), &iop);
}

DWORD CNodeSystemEditor::HandleEditorLeftClick( Vec3 *hitPosition )
{
	DWORD retVal = MSG_NOT_HANDLED;
	TOOLBARACTIVESTRUCT pToolBarStruct;

	// setup toolbar struct
	pToolBarStruct.cBar = &m_NodeToolPal;
	pToolBarStruct.bActive = false;
	static DWORD msgHash_IsToolBarActive = CHashString(_T("IsToolBarActive")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_IsToolBarActive, sizeof(pToolBarStruct), &pToolBarStruct );

	if( pToolBarStruct.bActive == TRUE )
	{
		PlotNode( hitPosition );
		retVal = MSG_HANDLED_STOP;
	}

	return retVal;
}

void CNodeSystemEditor::MoveObjectInFrontOfCamera(IHashString *objName)
{
	CHashString hszCameraName;
	CONTEXTCAMPARAMS ccp;
	ccp.pRC = NULL;
	ccp.szCameraName = &hszCameraName;
	static DWORD msgHash_GetActiveCamera = CHashString(_T("GetActiveCamera")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetActiveCamera, sizeof(CONTEXTCAMPARAMS), &ccp ) == MSG_HANDLED)
	{
		IHashString *pCameraName = ccp.szCameraName;
		// get active camera name
		Vec3 camOrigin;
		static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &camOrigin, pCameraName ) == MSG_HANDLED)
		{
			// get bounding box of object
			Vec3 bboxDim, bboxOrigin;
			Vec3 pos(0,0,0);
			Vec3 ext(0,0,0);
			EulerAngle rot(0,0,0);
			GETBOUNDINGBOXPARAMS bbox;
			bbox.position = &pos;
			bbox.extents = &ext;
			bbox.rotation = &rot;

			static DWORD msgHash_GetBoundingBox = CHashString(_T("GetBoundingBox")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetBoundingBox, sizeof(bbox), &bbox, objName );

			// get camera basis vectors
			Vec3 vecCamView, vecCamUp, vecCamRight;
			CAMERAVECTORSPARAMS camVectors;
			camVectors.vViewVector = &vecCamView;
			camVectors.vUpVector = &vecCamUp;
			camVectors.vRightVector = &vecCamRight;
			static DWORD msgHash_GetCameraVectors = CHashString(_T("GetCameraVectors")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_GetCameraVectors, sizeof(camVectors), &camVectors, pCameraName ) != MSG_HANDLED)
			{
				// log error
				StdString error;
				error = _T("Error retrieving camera vectors.\n");
				m_ToolBox->SetErrorValue(WARN_OBJECT_NOT_FOUND);
				m_ToolBox->Log(LOGWARNING, error);
			}

			// get the distance that the camera should be from object
			// twice the diameter of a sphere surrounding the bbox seems to be a decent distance
			float fDistance; //= 2.0f * sqrt(	bbox.dimX*bbox.dimX + 
							//							bbox.dimY*bbox.dimY + 
							//							bbox.dimZ*bbox.dimZ);

			bbox.extents->GetX();
			if (bbox.extents->GetZ() > bbox.extents->GetX() && bbox.extents->GetZ() > bbox.extents->GetY())
			{
				fDistance = bbox.extents->GetZ() * 5.0f;
			}
			else if (bbox.extents->GetX() > bbox.extents->GetZ() && bbox.extents->GetX() > bbox.extents->GetY())
			{
				fDistance = bbox.extents->GetX() * 5.0f;
			}
			else 
			{
				fDistance = bbox.extents->GetY() * 5.0f;
			}
			fDistance = max( fDistance, 50.0f );


			// get new object position from camera
			// to find camera position, back off calculated distance along the camera's view vector 
			// from the object's origin
			Vec3 newPos;
			newPos.SetX( camOrigin.GetX() + vecCamView.GetX() * fDistance );
			newPos.SetY( camOrigin.GetY() + vecCamView.GetY() * fDistance );
			newPos.SetZ( camOrigin.GetZ() + vecCamView.GetZ() * fDistance );

			// set object position
			Vec3 vecOriginalPos(0,0,0);
			m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &vecOriginalPos, objName);

			// move the object using the editor's settings
			EDITORMOVEOBJECTPARAMS emop;
			emop.pObjectName = objName;
			emop.vecMove = newPos - vecOriginalPos;;
			emop.bUseMoveAccum = false;
			static DWORD msgHash_EditorMoveObject = CHashString(_T("EditorMoveObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_EditorMoveObject, sizeof(emop), &emop );
		}
	}
}