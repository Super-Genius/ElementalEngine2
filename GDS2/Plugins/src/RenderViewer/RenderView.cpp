///============================================================================
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
#endif

static UINT ResourceDropMessage = RegisterWindowMessage(RESOURCE_DROP_MSG_STR);

// CRenderView

IMPLEMENT_DYNCREATE(CRenderView, CView)

CRenderView::CRenderView() : 
	m_hszSceneName(_T("World")),
	m_bIsActive(false)
{
	m_ToolBox = EngineGetToolBox();
	m_bInitialized = false;

	m_iViewWidth = 0;
	m_iViewHeight = 0;
	m_iContextID = 0;

	m_bLDragFlag = false;
	m_bWireframemode = false;

	m_bPhysVisOn = false;
	CreateUndoStack(&m_hszSceneName);
}

CRenderView::~CRenderView()
{
	DeleteUndoStack(&m_hszSceneName);
	static DWORD msgHash_DeleteRenderContext = CHashString(_T("DeleteRenderContext")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteRenderContext, sizeof(m_iContextID), &m_iContextID, NULL, NULL );
}

BEGIN_MESSAGE_MAP(CRenderView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateFileClose)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_WIREFRAME, OnUpdateRendererWireframe)
	ON_UPDATE_COMMAND_UI(ID_RENDER_RELOADSHADERS, OnUpdateReloadShaders)
	ON_COMMAND(ID_RENDER_RELOADSHADERS, OnReloadShaders)
	ON_COMMAND(ID_RENDERER_WIREFRAME, OnRendererWireframe)
	ON_REGISTERED_MESSAGE(ResourceDropMessage, OnDropMessage)
END_MESSAGE_MAP()

// CRenderView drawing

int CRenderView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetCursorPos(&m_OldMousePos);
	return 0;
}

void CRenderView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	static DWORD msgHash_DrawToRenderContext = CHashString(_T("DrawToRenderContext")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DrawToRenderContext, sizeof(m_iContextID), &m_iContextID, 
		NULL, NULL);
}


// CRenderView diagnostics

#ifdef _DEBUG
void CRenderView::AssertValid() const
{
	CView::AssertValid();
}

void CRenderView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

void CRenderView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	CWinApp *pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	CGuiMDIFrame *mainWnd = DYNAMIC_DOWNCAST(CGuiMDIFrame, pApp->GetMainWnd());

	if (m_bIsActive != (bActivate != FALSE))
	{
		m_bIsActive = (bActivate != FALSE);
		SetActiveScene(&m_hszSceneName);
		m_ViewCamera.SetMoveSensitivity( SINGLETONINSTANCE(CRenderViewer)->GetCameraMoveSensitivity() );
		if (bActivate)
		{
			mainWnd->LoadDocumentBars(_T("RenderViewer"));
		}
		else
		{
			mainWnd->SaveDocumentBars(_T("RenderViewer"));
		}
	}
}

void CRenderView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	if (m_bInitialized)
		return;
	m_bInitialized = true;

	RECT winRect;
	HWND hWnd = GetSafeHwnd();
	CWnd * pcwnd = GetDesktopWindow();
	if( pcwnd )
	{
		pcwnd->GetClientRect( &winRect );        
	}
	else
	{
		GetClientRect(&winRect);
	}
	
	m_iViewWidth =  (winRect.right - winRect.left);
	m_iViewHeight = (winRect.bottom - winRect.top);

	

	// create render context msg
	CREATERENDERCONTEXTPARAMS paramsContext;
	paramsContext.fullscreen = false;
	paramsContext.depthbits = 24;
	paramsContext.colorbits = 32;
	paramsContext.width = m_iViewWidth;
	paramsContext.height = m_iViewHeight;
	paramsContext.hwindow = hWnd;
//	paramsContext.retVal_RenderID = &m_iContextID;

	static DWORD msgHash_CreateRenderContext = CHashString(_T("CreateRenderContext")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateRenderContext, sizeof(paramsContext), &paramsContext );
	m_ToolBox->CreateComponent( &CHashString(_T("CSceneVisibilityManager")), NULL, NULL );

	//Load shader directory
	LOADSHADERDIRECTORY lsd;
	StdString basedir;
	StdString resdir;
	m_ToolBox->GetDirectories( &basedir, &resdir );
	resdir += _T("/Shaders/");
	lsd.dir = (TCHAR* )((const TCHAR * )resdir);
	static DWORD msgHash_LoadShaderPaths = CHashString(_T("LoadShaderPaths")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_LoadShaderPaths, sizeof( lsd ), &lsd );

	InitializeCamera();

	RegisterViewUndoStack(this, &m_hszSceneName);
}

void CRenderView::InitializeCamera()
{
	//iterate through the hierarchy looking for the first camera
	CHashString hszCameraName;
	CONTEXTCAMPARAMS ccp;
	ccp.pRC = NULL;
	ccp.szCameraName = &hszCameraName;
	static DWORD msgHash_GetActiveCamera = CHashString(_T("GetActiveCamera")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetActiveCamera, sizeof(CONTEXTCAMPARAMS), &ccp, NULL, NULL);
	m_hszCameraName = ccp.szCameraName;

	// set initial camera data
	Vec3 position( 0, 50, -50 );
	Vec3 up( 0, 1, 0 );
	Vec3 view( 0, 0, 1 );

	UPDATECAMERAPARAMS camdata;
	camdata.bSave = true; // write to camera
	camdata.vPosition = &position;
	camdata.eRotateType = kCameraRotateSetVectors;
	camdata.vUpVector = &up;
	camdata.vViewVector = &view;
	camdata.fHorizontalFOVAngle = 90.0f;
	camdata.fVerticalFOVAngle = 0.0f;
	camdata.fNearClip = 1.0f;
	//camdata.fFarClip = 4096.0f;
	camdata.fFarClip = 16384.0f;
	static DWORD msgHash_UpdateCamera = CHashString(_T("UpdateCamera")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_UpdateCamera, sizeof(camdata), &camdata, m_hszCameraName, &CHashString(_T("CCamera")) );

	INITOBJECTPARAMS iop;
	iop.name = m_hszCameraName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);


	// set internal camera state
	m_ViewCamera.SetCameraName( m_hszCameraName->GetString() );
	m_ViewCamera.SetMoveSensitivity( SINGLETONINSTANCE(CRenderViewer)->GetCameraMoveSensitivity() );
	m_ViewCamera.SetRotateSensitivity( 2.0f );
}

void CRenderView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize( nType, cx, cy );
	WINDOWINFO info;
	GetWindowInfo(&info); 
	m_iRenderViewWidth = (int)(info.rcClient.right - info.rcClient.left);
	m_iRenderViewHeight = (int)(info.rcClient.bottom - info.rcClient.top);

	VIEWPORTPARAMS windPar;
	windPar.width = m_iRenderViewWidth;
	windPar.height = m_iRenderViewHeight;
	static DWORD msgHash_SetWindowSize = CHashString(_T("SetWindowSize")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetWindowSize, sizeof(VIEWPORTPARAMS), &windPar);
}

// CRenderView message handlers

void CRenderView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// check for ! altkey pressed
	if (GetKeyState(VK_MENU) >= 0)
	{
		m_bLDragFlag = true;
		m_LDragStartPoint = point;

		MOUSECLICKPARAMS msgParams;
		Vec3 normalizedPoint;
		// convert to normalized mouse coords [0,1]
		normalizedPoint.x = (float)point.x / (float)m_iRenderViewWidth;
		normalizedPoint.y = (float)point.y / (float)m_iRenderViewHeight;
		normalizedPoint.z = 0;	

		msgParams.point = &normalizedPoint;
		static DWORD msgHash_LeftClickView = CHashString(_T("LeftClickView")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_LeftClickView, sizeof(msgParams), &msgParams);

		// Add "one click" painting/deform ability for terrain
		MOUSECOORDS mouseCoords;
		FillMouseData(nFlags, point, &mouseCoords);
		static DWORD msgHash_MouseMove = CHashString(_T("MouseMove")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_MouseMove, sizeof(MOUSECOORDS), &mouseCoords, NULL, NULL);
	}
	else
	{
		SetCapture();
		m_ViewCamera.StartMouseMove( point );
	}

	CView::OnLButtonDown(nFlags, point);
}

void CRenderView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	if (m_bLDragFlag)
	{
		m_bLDragFlag = false;

		MOUSECOORDS mouseCoords;
		FillMouseData(nFlags, point, &mouseCoords);
		static DWORD msgHash_MouseMove = CHashString(_T("MouseMove")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_MouseMove, sizeof(MOUSECOORDS), &mouseCoords, NULL, NULL);
	}
	else
	{
		m_ViewCamera.StopMouseMove();
	}

	CView::OnLButtonUp(nFlags, point);
}

void CRenderView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CView::OnLButtonDblClk(nFlags, point);
}

void CRenderView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	
	MOUSECOORDS mouseCoords;
	FillMouseData(nFlags, point, &mouseCoords);
	static DWORD msgHash_MouseMove = CHashString(_T("MouseMove")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_MouseMove, sizeof(MOUSECOORDS), &mouseCoords, NULL, NULL);

	CView::OnRButtonDown(nFlags, point);
}

void CRenderView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	
	MOUSECOORDS mouseCoords;
	FillMouseData(nFlags, point, &mouseCoords);
	static DWORD msgHash_MouseMove = CHashString(_T("MouseMove")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_MouseMove, sizeof(MOUSECOORDS), &mouseCoords, NULL, NULL);
	
	CView::OnRButtonUp(nFlags, point);
}

void CRenderView::OnMouseMove(UINT nFlags, CPoint point)
{
	MOUSECOORDS mouseparam;
	FillMouseData(nFlags, point, &mouseparam);
	m_OldMousePos = point;

	m_ViewCamera.OnMouseMove(nFlags, point);

	static DWORD msgHash_MouseMove = CHashString(_T("MouseMove")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_MouseMove, sizeof(mouseparam), &mouseparam, NULL,  NULL );
}

///	\brief	Fill MOUSECOORDS structure with current mouse data
///	\param	nFlags - mouse flags from windows mouse messages
///	\param	point - current mouse position
///	\param	pMouseCoords - pointer to MOUSECOORDS structure to fill
void CRenderView::FillMouseData(UINT nFlags, const CPoint &point, MOUSECOORDS *pMouseCoords)
{
	CPoint mouseDelta = point - m_OldMousePos;
	pMouseCoords->m_fXPos = (float)point.x / (float)m_iRenderViewWidth;
	pMouseCoords->m_fYPos = (float)point.y / (float)m_iRenderViewHeight;
	pMouseCoords->m_lXPos = point.x;
	pMouseCoords->m_lYPos = point.y;
	pMouseCoords->m_lXDelta = mouseDelta.x;
	pMouseCoords->m_lYDelta = mouseDelta.y;
	pMouseCoords->m_LButtonDown = m_bLDragFlag;
	pMouseCoords->m_RButtonDown = (nFlags & MK_RBUTTON) != 0;
}

void CRenderView::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	static DWORD msgHash_SelectObject = CHashString(_T("SelectObject")).GetUniqueID();

	switch (nChar)
	{
		case VK_ESCAPE:		
			{
			SELECTOBJECTPARAMS sop;
			sop.compType = NULL;
			sop.object = NULL;
			m_ToolBox->SendMessage(msgHash_SelectObject, sizeof(sop), &sop );
			}
			break;
		case VK_F2:
			CHashString physObject = (_T("CPhysicsObject"));
			if( m_bPhysVisOn )
			{
				m_bPhysVisOn = false;
				static DWORD msgHash_DisableVisualization = CHashString("DisableVisualization").GetUniqueID();
				m_ToolBox->SendMessage(msgHash_DisableVisualization, 0, NULL, NULL, &physObject);
			}
			else
			{
				m_bPhysVisOn = true;
				static DWORD msgHash_EnableVisualization = CHashString("EnableVisualization").GetUniqueID();
				m_ToolBox->SendMessage(msgHash_EnableVisualization, 0, NULL, NULL, &physObject);
			}
			break;
	}

	m_ViewCamera.OnKeyDown( nChar, nRepCnt, nFlags );
	CView::OnKeyDown(nChar,nRepCnt,nFlags);
}

void CRenderView::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CView::OnKeyUp(nChar,nRepCnt,nFlags);
}

void CRenderView::OnMButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	m_ViewCamera.StartMouseMove( point );

	CView::OnMButtonDown(nFlags, point);
}

BOOL CRenderView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// move camera in/out
	m_ViewCamera.MoveForward( (float)zDelta / (float)WHEEL_DELTA );

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CRenderView::OnMButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_ViewCamera.StopMouseMove();

	CView::OnMButtonUp(nFlags, point);
}

void CRenderView::OnUpdateFileClose(CCmdUI *pCmdUI)
{
	// always disable the close option on this view
	pCmdUI->Enable(false);
}
void CRenderView::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	// when in render mode, we don't save as
	pCmdUI->Enable(false);
}

void CRenderView::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
	// when in render mode, we don't save here.
	pCmdUI->Enable(false);
}

void CRenderView::OnUpdateRendererWireframe(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bWireframemode);
}

void CRenderView::OnUpdateReloadShaders(CCmdUI *pCmdUI)
{
	// when in render mode, we don't save here.
	//pCmdUI->Enable(false);
}

void CRenderView::OnReloadShaders( )
{
	static DWORD msgHash_ReloadAllShaders = CHashString(_T("ReloadAllShaders")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_ReloadAllShaders, 0, 0 );
}

void CRenderView::OnRendererWireframe()
{
	// TODO: Add your command handler code here
	m_bWireframemode = !m_bWireframemode;
	static DWORD msgHash_SetWireFrameMode = CHashString(_T("SetWireFrameMode")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetWireFrameMode, sizeof(m_bWireframemode), &m_bWireframemode );
}

LRESULT CRenderView::OnDropMessage(WPARAM wParam, LPARAM lParam)
{
	DROPOBJECTINFO *pInfo = (DROPOBJECTINFO*)lParam;
	
	// forward message to EE
	static DWORD msgHash_ItemDropped = CHashString(_T("ItemDropped")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_ItemDropped, sizeof(DROPOBJECTINFO), pInfo);

	return 0;
}
