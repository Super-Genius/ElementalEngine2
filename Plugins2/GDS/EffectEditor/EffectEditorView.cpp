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
#include "EffectEditor.h"
#include "EffectEditorView.h"


// CEffectEditorView

IMPLEMENT_DYNCREATE(CEffectEditorView, CView)

CEffectEditorView::CEffectEditorView() :
	m_hszSceneName(_T("EffectViewerScene")), 
	m_hszSceneType(_T("CWorld")), 
	m_hszCameraName(_T("EffectViewerCamera")),
	m_bActive(false)
{
	m_ToolBox = EngineGetToolBox();
	m_pEditor = SINGLETONINSTANCE(CEffectEditor);

	m_bCameraLightActive = false;
	m_bSceneLightActive = false;
	m_bGroundModelActive = false;
	m_bEffectModelActive = false;
}

CEffectEditorView::~CEffectEditorView()
{
}

void CEffectEditorView::ToggleCameraLight()
{
	m_bCameraLightActive = !m_bCameraLightActive;
	if (m_bCameraLightActive)
	{
		static DWORD msgHash_SetSpawnParentName = CHashString(_T("SetSpawnParentName")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetSpawnParentName, sizeof(IHashString), &m_hszSceneName);

		LOADFILEEXTPARAMS lfep;
		lfep.fileName = _T("Spawners\\Lights\\OmniLight.sxl");
		lfep.bInternalLoad = false;
		static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep) == MSG_HANDLED)
		{
			static DWORD msgHash_GetSpawnedName = CHashString(_T("GetSpawnedName")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetSpawnedName, sizeof(IHashString), &m_hszCameraLight);
		}

		m_ViewCamera.AddFollowingObject( m_hszCameraLight.GetUniqueID() );
	}
	else
	{
		DELETEOBJECTPARAMS dop;
		dop.name = &m_hszCameraLight;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop );

		m_ViewCamera.RemoveFollowingObject( m_hszCameraLight.GetUniqueID() );
	}
}

void CEffectEditorView::ToggleSceneLight()
{
	m_bSceneLightActive = !m_bSceneLightActive;
	if (m_bSceneLightActive)
	{
		static DWORD msgHash_SetSpawnParentName = CHashString(_T("SetSpawnParentName")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetSpawnParentName, sizeof(IHashString), &m_hszSceneName);

		LOADFILEEXTPARAMS lfep;
		lfep.fileName = _T("Spawners\\Lights\\DirectionalLight.sxl");
		lfep.bInternalLoad = false;
		static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep) == MSG_HANDLED)
		{
			static DWORD msgHash_GetSpawnedName = CHashString(_T("GetSpawnedName")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetSpawnedName, sizeof(IHashString), &m_hszSceneLight);
		}
	}
	else
	{
		DELETEOBJECTPARAMS dop;
		dop.name = &m_hszSceneLight;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop );
	}
}

void CEffectEditorView::ToggleGroundModel()
{
	m_bGroundModelActive = !m_bGroundModelActive;
	if (m_bGroundModelActive)
	{
		static DWORD msgHash_SetSpawnParentName = CHashString(_T("SetSpawnParentName")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetSpawnParentName, sizeof(IHashString), &m_hszSceneName);

		LOADFILEEXTPARAMS lfep;
		lfep.fileName = _T("Spawners\\Environments\\Urban\\Streets\\4_lane_road.sxl");
		lfep.bInternalLoad = false;
		static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep) == MSG_HANDLED)
		{
			static DWORD msgHash_GetSpawnedName = CHashString(_T("GetSpawnedName")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetSpawnedName, sizeof(IHashString), &m_hszGroundModel);
		}
	}
	else
	{
		DELETEOBJECTPARAMS dop;
		dop.name = &m_hszGroundModel;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop );
	}
}

void CEffectEditorView::ToggleEffectModel()
{
	m_bEffectModelActive = !m_bEffectModelActive;
	if (m_bEffectModelActive)
	{
		static DWORD msgHash_SetSpawnParentName = CHashString(_T("SetSpawnParentName")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetSpawnParentName, sizeof(IHashString), &m_hszSceneName);

		StdString szBaseDir, szResourceDir, szPath;
		m_ToolBox->GetDirectories(&szBaseDir, &szResourceDir);
		szPath = szResourceDir + _T("\\Models\\ShaderTest\\ShaderTest_Box.sxl");
		LOADFILEEXTPARAMS lfep;
		lfep.fileName = (TCHAR*)(const TCHAR*)szPath;
		lfep.bInternalLoad = false;
		static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep) == MSG_HANDLED)
		{
			static DWORD msgHash_GetSpawnedName = CHashString(_T("GetSpawnedName")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetSpawnedName, sizeof(IHashString), &m_hszEffectModel);
		}
	}
	else
	{
		DELETEOBJECTPARAMS dop;
		dop.name = &m_hszEffectModel;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop );
	}
}

void CEffectEditorView::SetEffectModelEffect( IEffect *pEffect )
{
	if (m_bEffectModelActive)
	{
		IRenderObject *pRenderObject = NULL;
		static DWORD msgHash_GetRenderObjectInterface = CHashString(_T("GetRenderObjectInterface")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetRenderObjectInterface, sizeof(pRenderObject), &pRenderObject, &m_hszEffectModel ) == MSG_HANDLED)
		{
			if (pRenderObject)
			{
				int iNumMaterial = pRenderObject->GetNumMaterial();
				for (int i=0; i < iNumMaterial; i++)
				{
					IMaterial *pMaterial = pRenderObject->GetMaterial(i);
					pMaterial->SetEffect( pEffect );
				}
			}
		}
	}
}


BEGIN_MESSAGE_MAP(CEffectEditorView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CEffectEditorView drawing

void CEffectEditorView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CEffectEditorView diagnostics

#ifdef _DEBUG
void CEffectEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CEffectEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CEffectEditorView message handlers

int CEffectEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	static DWORD msgHash_AddObject = CHashString(_T("AddObject")).GetUniqueID();
	DWORD returnValue;

	// create new scene root
	CREATEOBJECTPARAMS cop;
	cop.name = &m_hszSceneName;
	cop.parentName = NULL;
	cop.typeName = &m_hszSceneType;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	returnValue = EngineGetToolBox()->SendMessage(msgHash_CreateObject, sizeof(cop), &cop );
	if (returnValue != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGERROR, _T("Failed to create Effect Viewer root scene object.\n"));
		return -1;
	}
	m_ToolBox->SendMessage(msgHash_AddObject, sizeof(cop), &cop);

	// create render context
	CREATERENDERCONTEXTPARAMS crcp; 
    crcp.colorbits = 32; 
    crcp.depthbits = 24; 
    crcp.height = 0; // resize later 
    crcp.width = 0; // resize later 
    crcp.hwindow = *this; 
	static DWORD msgHash_CreateRenderContext = CHashString(_T("CreateRenderContext")).GetUniqueID();
    returnValue = m_ToolBox->SendMessage(msgHash_CreateRenderContext, sizeof(CREATERENDERCONTEXTPARAMS), &crcp);
	if (returnValue != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGERROR, _T("Failed to create Effect Viewer render context.\n"));
		return -1;
	}
	m_uiRenderContextID = crcp.uiRenderContextID;
	m_pRenderContext = crcp.context;
	m_pRenderContext->SetBGColor(64,64,64);

	// set scene's render context
	SETSCENERENDERCONTEXTPARAMS ssrcp;
	ssrcp.pSceneName = &m_hszSceneName;
	ssrcp.pRenderContext = m_pRenderContext;
	static DWORD msgHash_SetSceneRenderContext = CHashString(_T("SetSceneRenderContext")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetSceneRenderContext, sizeof(ssrcp), &ssrcp);

	ADDRENDERLISTPARAMS arlp;
	OBJECTLIST ol; // empty object list
	arlp.dwListPriorityKey = RENDERPRIORITY_FRONTBUFFER;
	arlp.dwSortMethod = SORT_FIFO;
	arlp.objList = &ol;
	arlp.renderContext = m_pRenderContext;
	static DWORD msgHash_AddRenderList = CHashString(_T("AddRenderList")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddRenderList, sizeof(ADDRENDERLISTPARAMS), &arlp);	

	// create camera
	cop.name = &m_hszCameraName;
	cop.parentName = &m_hszSceneName;
	static DWORD msgHash_CreateCamera = CHashString(_T("CreateCamera")).GetUniqueID();
    m_ToolBox->SendMessage(msgHash_CreateCamera, sizeof(CREATEOBJECTPARAMS), &cop);

	// attach view camera to camera object
	m_ViewCamera.SetCameraName( m_hszCameraName.GetString() );
	m_ViewCamera.SetMoveSensitivity( 4.0f );
	m_ViewCamera.SetRotateSensitivity( 2.0f );

	return 0;
}

void CEffectEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect);
	if (m_pRenderContext)
		m_pRenderContext->CreateContext(*this, rect.right, rect.bottom, 24, 32);

	VIEWPORTPARAMS gwrp;
	gwrp.offsetx = 0;
	gwrp.offsety = 0;
	gwrp.width = rect.right;
	gwrp.height = rect.bottom;
	static DWORD msgHash_SetWindowSize = CHashString(_T("SetWindowSize")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetWindowSize, sizeof(VIEWPORTPARAMS), &gwrp);
}

void CEffectEditorView::OnDestroy()
{
	CView::OnDestroy();

	// delete scene root
	DELETEOBJECTPARAMS dop;
	dop.name = &m_hszSceneName;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop );

	// delete rendercontext
	static DWORD msgHash_DeleteRenderContext = CHashString(_T("DeleteRenderContext")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteRenderContext, sizeof(UINT), &m_uiRenderContextID);
}

void CEffectEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	m_ViewCamera.OnMouseMove( nFlags, point );

	CView::OnMouseMove(nFlags, point);
}

BOOL CEffectEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_ViewCamera.MoveForward( (float)zDelta / (float)WHEEL_DELTA );

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CEffectEditorView::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_ViewCamera.StartMouseMove( point );

	CView::OnMButtonDown(nFlags, point);
}

void CEffectEditorView::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_ViewCamera.StopMouseMove();

	CView::OnMButtonUp(nFlags, point);
}

void CEffectEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_ViewCamera.OnKeyDown( nChar, nRepCnt, nFlags );

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEffectEditorView::OnKillFocus(CWnd* pNewWnd)
{
	CView::OnKillFocus(pNewWnd);
}

void CEffectEditorView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);
}

void CEffectEditorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	if (m_bActive != (FALSE != bActivate))
	{
		m_bActive = (FALSE != bActivate);

		CGuiMDIFrame *mainWnd = DYNAMIC_DOWNCAST(CGuiMDIFrame, AfxGetMainWnd());
		if (bActivate)
		{
			// set active scene
			GETSETACTIVESCENE gsas;
			gsas.pSceneName = &m_hszSceneName;
			static DWORD msgHash_SetActiveScene = CHashString(_T("SetActiveScene")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SetActiveScene, sizeof(gsas), &gsas );

			// set active camera
			CONTEXTCAMPARAMS ccp;
			ccp.pRC = m_pRenderContext;
			ccp.szCameraName = &m_hszCameraName;
			static DWORD msgHash_SetActiveCamera = CHashString(_T("SetActiveCamera")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SetActiveCamera, sizeof(CONTEXTCAMPARAMS), &ccp);

			mainWnd->LoadDocumentBars(_T("EffectEditor"));
		}
		else
		{
			mainWnd->SaveDocumentBars(_T("EffectEditor"));
		}
	}
}
