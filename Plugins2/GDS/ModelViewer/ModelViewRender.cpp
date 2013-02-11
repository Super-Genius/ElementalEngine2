///============================================================================
/// \file		ModelViewRender.cpp
/// \brief		view implementation for ModelViewer
/// \date		09-14-2007
/// \author		Andrey Ivanov
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

// CModelViewRender

IMPLEMENT_DYNCREATE(CModelViewRender, CView)

CModelViewRender *CModelViewRender::sm_pActiveRenderer = NULL;

CModelViewRender::CModelViewRender()
	: m_nCurrentAnimationId(-1)
	, m_bLoop(false)
	, m_hszSceneName(_T("ModelViewerScene"))
	, m_hszEntityName(_T("ModelViewerEntity"))
	, m_hszLightEntityName(_T("ModelViewLightEntity"))
	, m_hszLightObjectName(_T("ModelViewLight"))
	, m_bActivated(false)
{

	m_itCurrentAnimationID = m_PlayingAnimationSequence.end();

	m_v3Position.Set(0, 0, 0);
	m_v3CameraPos.Set(0.0f, 0.0f, 200.0f);
	m_v3Rotation.Set(0.0f, 0.0f, 0.0f);

	m_ToolBox = EngineGetToolBox();
	m_bPlayNextInSequence = false;
}

CModelViewRender::~CModelViewRender()
{
	// Delete Light Object
	DeleteEEObject(&m_hszLightObjectName);
	// Delete Light Object
	DeleteEEObject(&m_hszLightEntityName);
	// Delete CEntity Object
	DeleteEEObject(&m_hszEntityName);
	// Delete Cal3DRenderObject
	DeleteEEObject(&m_hszModelName);
}

BEGIN_MESSAGE_MAP(CModelViewRender, CView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_ACTIVATE()
	ON_COMMAND(IDC_MODELVIEW_TB_BUTTON_ZOOMEXTENTS, ZoomToExtents)
END_MESSAGE_MAP()

// CModelViewRender drawing

void CModelViewRender::OnDraw(CDC* pDC)
{
	m_RenderContext.Draw();
}

void CModelViewRender::OnDestroy()
{
	StopSequence();

	// delete scene root
	DeleteEEObject(&m_hszSceneName);

	DeactivateView();

	CView::OnDestroy();
}

IHashString *CModelViewRender::GetEntityName()
{
	return &m_hszEntityName;
}

void CModelViewRender::CreateScene()
{
	Vec3 v3Zero(0.0f, 0.0f, 0.0f);
	EulerAngle eZero;
	Vec3 v3Scale(1.0f,1.0f,1.0f);
	StdString szEntityType(_T("EditorObject"));

	IArchive *pArchive = CreateMemoryArchive();
	pArchive->Write(_T("EditorObject"), _T("EntityType"));
	pArchive->Write(v3Zero, _T("Position"));
	pArchive->Write(eZero, _T("Rotation"));
	pArchive->Write(v3Scale, _T("Scale"));
	pArchive->SetIsWriting(false);

	// Create Instance of CEntity 
	static CHashString hszEntity(_T("CEntity"));
	CreateEEObject(&m_hszSceneName, &hszEntity, m_hszEntityName, pArchive);

	pArchive->Close();

	m_v3Position.Set(0, 0, 0);
	m_v3CameraPos.Set(0.0f, 0.0f, 200.0f);
	m_v3Rotation.Set(0.0f, 0.0f, 0.0f);

	LoadModel();
}

void CModelViewRender::LoadModel()
{
	ASSERT(GetDocument() != NULL);
	CString strPath = GetDocument()->GetPathName();
	LPCTSTR szPath = strPath;
	TCHAR extStr[_MAX_EXT];
	TCHAR fileName[_MAX_FNAME];
	_tsplitpath(szPath, NULL, NULL, fileName, extStr);

	if (strPath.IsEmpty())
	{
		m_ToolBox->Log(LOGWARNING, _T("%s(%i):LoadModel shouldn't have been called with NULL path\n"), __FILE__, __LINE__);
		return;
	}

	StdString szFilename = fileName;
	szFilename += extStr;
	m_hszModelName = szFilename;

	CFileVersionSetter setter( _T("2.5") );
	
	if (0 == _tcsicmp(extStr, _T(".cfg")))
	{
		IArchive *pArchive = CreateMemoryArchive();
		if (pArchive != NULL)
		{
			pArchive->Write(szPath, _T("Filepath"));
			pArchive->SetIsWriting(false);
			static CHashString hszCal3DRenderObject(_T("Cal3DRenderObject"));
			CreateEEObject(&m_hszEntityName, &hszCal3DRenderObject, m_hszModelName, pArchive);
			FillAnimationList();
			pArchive->Close();
		}
	}
	else if (0 == _tcsicmp(extStr, _T(".hrc")))
	{
		IArchive *pArchive = CreateMemoryArchive();
		if (pArchive != NULL)
		{
			pArchive->Write(szPath);
			pArchive->SetIsWriting(false);
			static CHashString hszCal3DRenderObject(_T("CHierarchicalModel"));
			CreateEEObject(&m_hszEntityName, &hszCal3DRenderObject, m_hszModelName, pArchive);
			pArchive->Close();
		}
	}
	else
	{
		m_ToolBox->Log(LOGWARNING, _T("%s(%i):LoadModel was passed %s extension, which is not recognized\n"), __FILE__, __LINE__, extStr);
	}
}

void CModelViewRender::FillAnimationList()
{
	ASSERT(m_ToolBox != NULL);
	ASSERT(m_AnimationList.empty());
	GETANIMATIONLISTPARAMS galp;
	galp.AnimationList = &m_AnimationList;
	static CHashString hszCal3DRenderObject(_T("Cal3DRenderObject"));
	static DWORD msgHash_GetAnimationList = CHashString(_T("GetAnimationList")).GetUniqueID();
	// ignore GetAnimationList message errors. It is not possible to handle them where
	m_ToolBox->SendMessage(msgHash_GetAnimationList, sizeof(GETANIMATIONLISTPARAMS), &galp, &m_hszModelName, &hszCal3DRenderObject);

	if (!m_AnimationList.empty())
	{
		// setup selection for non-empty animation list
		m_nCurrentAnimationId = 0;
	}
}

void CModelViewRender::CalcScaleValues()
{
	DWORD retval;
	GETBOUNDINGBOXPARAMS gbbp;

	Vec3 v3Pos(0.0f, 0.0f, 0.0f);
	Vec3 v3Extents;
	EulerAngle v3Rotation;

	gbbp.position = &v3Pos;
	gbbp.extents = &v3Extents;
	gbbp.rotation = &v3Rotation;

	static DWORD msgHash_GetBoundingBox = CHashString(_T("GetBoundingBox")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_GetBoundingBox, sizeof(gbbp), &gbbp, &m_hszModelName);
	if (retval != MSG_HANDLED)
	{
		m_ToolBox->Log( LOGERROR, _T("%s(%i): GetBoundingBox not handled.\n"), __FILE__, __LINE__ );
		assert(0);
	}

	m_fZoomDistance = 0.0;
	if ( gbbp.extents )
	{
		if (v3Extents.x > v3Extents.z)
			m_fZoomDistance = v3Extents.x;
		else
			m_fZoomDistance = v3Extents.z;

		m_fZoomDistance *= 3.0f;
	}

	m_fScaleZoom = (m_fZoomDistance / 6000);
	m_fScaleOffset = m_fZoomDistance / 480;

	// set model in the center of the view
	m_v3Position.SetX(v3Pos.GetX() * -1);
	m_v3Position.SetY(v3Pos.GetY() * -1);
	// TODO: fit model in the view
}

void CModelViewRender::InitRenderScene()
{
	// set active scene to model view scene so objects created here will be placed in the correct scene
	SetActiveScene();

	//Create Cal3DRenderObject from loaded file
	CreateScene();

	// create a new Memory Archive
	IArchive *pArchive = CreateMemoryArchive();
	if (pArchive == NULL)
	{
		return;
	}

	StdString szEntityType(_T("EditorObject"));
	//Vec3 v3Zero(0.0f, 0.0f, 0.0f);
	Vec3 v3EntityLightPosition(0.0f, 200.0f, 200.0f);
	EulerAngle eaZero;
	Vec3 v3Scale(1.0f, 1.0f, 1.0f);
	pArchive->Write(szEntityType, _T("EntityType"));
	pArchive->Write(v3EntityLightPosition, _T("Position"));
	pArchive->Write(eaZero, _T("Rotation"));
	pArchive->Write(v3Scale, _T("Scale"));
	pArchive->SetIsWriting(false);

	CFileVersionSetter setter;
	// Create the CEntity LightObject Parent /////////////////////////////////////////////////////////
	static CHashString hszCEntity(_T("CEntity"));
	if (CreateEEObject(&m_hszSceneName, &hszCEntity, m_hszLightEntityName, pArchive))
	{
		pArchive->SetIsWriting(true);
		pArchive->SeekTo(0);
		int iVersionNumber = 1000;
		Vec3 v3LightPosition( 0.0, 0.0f, 0.0f );
		Vec3 v3LightDirection( 0.0f, 0.0f, 0.0f );
		float fAttenuation = 99999.0f;
		float fColor[] = { 0.75f, 0.75f, 0.75f, 1.0f };
		StdString szLightType(_T("OMNI_POINT_LIGHT"));
		bool bCastShadows = false;
		UINT numKeys = 0;
		pArchive->Write(iVersionNumber, _T("Version"));
		pArchive->Write(v3LightPosition, _T("Position"));
		pArchive->Write(v3LightDirection, _T("Direction"));
		pArchive->Write(fAttenuation, _T("Attenuation"));
		pArchive->Write(fColor[0], _T("ColorRed") );
		pArchive->Write(fColor[1], _T("ColorGreen") );
		pArchive->Write(fColor[2], _T("ColorBlue") );
		pArchive->Write(fColor[3], _T("ColorAlpha") );
		pArchive->Write(szLightType, _T("LightType") );
		pArchive->Write(bCastShadows, _T("CastShadows") );
		pArchive->Write(numKeys, _T("NumKeyframes") );
		pArchive->SetIsWriting(false);
	    
		// Create the Light Object ////////////////////////////////////////////////////////////////
		static CHashString hszCLightObject(_T("CLightObject"));	
		CreateEEObject(&m_hszLightEntityName, &hszCLightObject, m_hszLightObjectName, pArchive);
	}

	pArchive->Close();
}

void CModelViewRender::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	m_RenderContext.SetWindowSize(cx, cy);
}

int CModelViewRender::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create new scene root
	static CHashString hszSceneType(_T("CWorld"));
	if (!CreateEEObject(NULL, &hszSceneType, m_hszSceneName, NULL))
	{
		return -1;
	}

	if (!m_RenderContext.Create(*this, false))
	{
		return -1;
	}

	// set model viewer scene's render context
	if (!m_RenderContext.SetScene(&m_hszSceneName))
	{
		return -1;
	}

	if (!m_ViewCamera.CreateCamera(&m_hszSceneName))
	{
		return -1;
	}

	if (!m_RenderContext.SetActiveCamera(m_ViewCamera))
	{
		return -1;
	}

	m_RenderContext.SetCameraPosition(m_v3CameraPos);

	EulerAngle cameraOrientation(0.0, 0.0, 180.0);
	m_RenderContext.SetCameraOrientation(cameraOrientation);

	return 0;
}

void CModelViewRender::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	InitRenderScene();
	CalcScaleValues();
	ZoomToExtents();
}

void CModelViewRender::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	CView::OnLButtonDown(nFlags, point);
}

void CModelViewRender::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (0 == (nFlags & MK_RBUTTON))
	{
		ReleaseCapture();
	}
	CView::OnLButtonUp(nFlags, point);
}

void CModelViewRender::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	CView::OnRButtonDown(nFlags, point);
}

void CModelViewRender::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (0 == (nFlags & MK_LBUTTON))
	{
		ReleaseCapture();
	}
	CView::OnRButtonUp(nFlags, point);
}

BOOL CModelViewRender::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	float change = (float)zDelta * m_fScaleZoom;
	m_v3CameraPos.z -= change;
	m_v3CameraPos.z = max(m_v3CameraPos.z, 0.0f);

	m_RenderContext.SetCameraPosition(m_v3CameraPos);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CModelViewRender::OnMouseMove(UINT nFlags, CPoint point)
{
	CSize offset = point - m_ptLastMousePos;
	if (0 != (nFlags & MK_LBUTTON))
	{
		// processing rotation while left mouse button is pressed
		if (0 != (nFlags & MK_CONTROL))
		{
			m_v3Rotation.roll += offset.cx;
			m_v3Rotation.pitch += offset.cy;

			m_v3Rotation.roll = max(min(m_v3Rotation.roll, 180), -180);
		}
		else
		{
			m_v3Rotation.pitch += offset.cy;
			m_v3Rotation.yaw += offset.cx;

			//m_v3Rotation.yaw = max(min(m_v3Rotation.yaw, 180), -180);
		}

		m_v3Rotation.pitch = max(min(m_v3Rotation.pitch, 180), -180);

		UpdateRotation();
		GetDockDlg()->SetRotation(m_v3Rotation);
	}

	if (0 != (nFlags & MK_RBUTTON))
	{
		// move object position while right mouse button is pressed
		m_v3Position.x -= (point.x - m_ptLastMousePos.x) * m_fScaleOffset;
		m_v3Position.y -= (point.y - m_ptLastMousePos.y) * m_fScaleOffset;

		UpdatePosition();
		GetDockDlg()->SetPosition(m_v3Position);
	}

	m_ptLastMousePos = point;

	CView::OnMouseMove(nFlags, point);
}

void CModelViewRender::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	CGuiMDIFrame *mainWnd = DYNAMIC_DOWNCAST(CGuiMDIFrame, AfxGetMainWnd());
	if (bActivate)
	{
		if (!m_bActivated)
		{
			SetActiveScene();
			ActivateView();
			mainWnd->LoadDocumentBars(_T("ModelView"));
			m_bActivated = true;
		}
	}
	else if (m_bActivated)
	{
		DeactivateView();
		mainWnd->SaveDocumentBars(_T("ModelView"));
		m_bActivated = false;
	}
}

void CModelViewRender::ActivateView()
{
	// filling current object position and rotation
	GetDockDlg()->SetPosition(m_v3Position);
	GetDockDlg()->SetRotation(m_v3Rotation);

	// filling animation
	// ORDER: animation list should be filled before animation sequence
	GetDockDlg()->SetAnimationList(&m_AnimationList);
	GetDockDlg()->SetCurrentAnimationId(m_nCurrentAnimationId);
	GetDockDlg()->SetAnimationSequence(&m_AnimationSequence);
}

void CModelViewRender::DeactivateView()
{
	m_nCurrentAnimationId = GetDockDlg()->GetCurrentAnimationId();
	GetDockDlg()->GetAnimationSequence(m_AnimationSequence);
	GetDockDlg()->SetAnimationList(NULL);
	GetDockDlg()->SetAnimationSequence(NULL);
	sm_pActiveRenderer = NULL;
}

bool CModelViewRender::HasAnimation() const
{
	return m_itCurrentAnimationID != m_PlayingAnimationSequence.end();
}

int CModelViewRender::GetCurrentAnimationID() const
{
	ASSERT(HasAnimation());
	return *m_itCurrentAnimationID;
}

void CModelViewRender::PlayAnimation( bool bLoop, int nAnimationId )
{
	if (0 <= nAnimationId)
	{
		m_bLoop = bLoop;
		ANIMATIONSEQUENCE sequence;
		sequence.push_back(nAnimationId);
		PlaySequence(sequence);
	}
}

void CModelViewRender::PlaySequence( bool bLoop, ANIMATIONSEQUENCE &animationSequence )
{
	m_bLoop = bLoop;
	m_AnimationSequence = animationSequence;
	PlaySequence(m_AnimationSequence);
}

void CModelViewRender::PlaySequence(const ANIMATIONSEQUENCE &sequence)
{
	// make a copy of the requested sequence
	m_PlayingAnimationSequence.clear();
	std::copy(sequence.begin(), sequence.end(), std::back_inserter(m_PlayingAnimationSequence));
	m_itCurrentAnimationID = m_PlayingAnimationSequence.begin();

	// create all the callbacks at once for the sequence
	set<int> uniqueCallbackSet;
	for (ANIMATIONSEQUENCE::iterator itrAnim = m_PlayingAnimationSequence.begin(); itrAnim != m_PlayingAnimationSequence.end(); itrAnim++)
	{
		// skip duplicate animations (only need to make one callback per animation)
		if (uniqueCallbackSet.find( *itrAnim ) != uniqueCallbackSet.end())
			continue;

		IArchive* pArchive = CreateMemoryArchive();
		pArchive->Write(m_hszEntityName.GetString(), _T("EntityName"));
		pArchive->Write(*itrAnim);
		pArchive->SetIsWriting(false);
		m_AnimationCallbackArchives.push_back( pArchive );

		static CHashString hszCal3DRenderObject = _T("Cal3DRenderObject");
		static CHashString hszCModelViewComponent(_T("CModelViewComponent"));
		static CHashString hszPlayAnimationSequenceStep(_T("PlayAnimationSequenceStep"));

		REGISTERCAL3DANIMATIONCALLBACK ac;
		ac.AnimationId = *itrAnim;
		ac.bTriggerOnComplete = true;
		ac.bTriggerOnStop = false;
		ac.StateObjectName = &hszCModelViewComponent;
		ac.EventName = &hszPlayAnimationSequenceStep;
		ac.EventParamsArchive = pArchive;

		static DWORD msgHash_RegisterAnimationCallback = CHashString(_T("RegisterAnimationCallback")).GetUniqueID();
		DWORD res = m_ToolBox->SendMessage(msgHash_RegisterAnimationCallback, sizeof(ac), &ac, &m_hszEntityName, &hszCal3DRenderObject);
		if (MSG_HANDLED != res)
		{
			m_ToolBox->Log(LOGERROR, _T("%s(%i): Cannot register animation callback. \n"), __FILE__, __LINE__);
		}
		uniqueCallbackSet.insert( *itrAnim );
	}

	if (HasAnimation())
	{
		SINGLETONINSTANCE(CModelViewComponent)->PlayAnimation(this);
	}
}

void CModelViewRender::StopSequence()
{
	m_bPlayNextInSequence = false;

	static CHashString hszCal3DRenderObject = _T("Cal3DRenderObject");

	// unregister all animation callbacks
	static DWORD msgHash_UnregisterAllAnimationCallbacks = CHashString(_T("UnregisterAllAnimationCallbacks")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_UnregisterAllAnimationCallbacks, 0, NULL, &m_hszEntityName, &hszCal3DRenderObject);
	if (MSG_HANDLED != res)
	{
		m_ToolBox->Log(LOGERROR, _T("%s(%i): Cannot unregister animation callbacks.\n"), __FILE__, __LINE__);
	}
	// clean up all the callback archives
	vector<IArchive*>::iterator itrCallback = m_AnimationCallbackArchives.begin();
	while (itrCallback != m_AnimationCallbackArchives.end())
	{
		(*itrCallback)->Close();
		itrCallback++;
	}
	m_AnimationCallbackArchives.clear();

	// stop all animations
	static DWORD msgStopAllAnimationActions = CHashString(_T("StopAllAnimationActions")).GetUniqueID();
	m_ToolBox->SendMessage( msgStopAllAnimationActions, 0, NULL, &m_hszEntityName, &hszCal3DRenderObject );

	if (HasAnimation())
	{
		m_PlayingAnimationSequence.clear();
		m_itCurrentAnimationID = m_PlayingAnimationSequence.end();
		m_bLoop = false;

		// initiate delayed deletion for cal3Dmodel callbacks with Update event
		// this prevents memory leaks with EventParamsArchive
		// TODO. This may be fixed in cal3DModel probably
		AfxGetApp()->OnIdle(2);
	}

	GetDockDlg()->OnStopSequence();
}

void CModelViewRender::PlayNextAnimation()
{
	static CHashString hszCal3DRenderObject(_T("Cal3DRenderObject"));
	static DWORD msgStopAllAnimationActions = CHashString(_T("StopAllAnimationActions")).GetUniqueID();
	m_ToolBox->SendMessage( msgStopAllAnimationActions, 0, NULL, &m_hszEntityName, &hszCal3DRenderObject );

	if (m_itCurrentAnimationID != m_PlayingAnimationSequence.end())
		m_itCurrentAnimationID++;

	if (m_bLoop && m_itCurrentAnimationID == m_PlayingAnimationSequence.end())
	{
		m_itCurrentAnimationID = m_PlayingAnimationSequence.begin();
	}

	if (HasAnimation())
	{
		SINGLETONINSTANCE(CModelViewComponent)->PlayAnimation(this);
	}
}

void CModelViewRender::Update()
{
	if (m_bPlayNextInSequence)
	{
		PlayNextAnimation();
		m_bPlayNextInSequence = false;
	}
}

void CModelViewRender::ZoomToExtents()
{
	m_v3CameraPos.Set(0.0, 0.0, +m_fZoomDistance);
	m_RenderContext.SetCameraPosition(m_v3CameraPos);
}

void CModelViewRender::ResetPosition()
{
	m_v3Position.Set(0, 0, 0);
	UpdatePosition();
	GetDockDlg()->SetPosition(m_v3Position);
}

void CModelViewRender::ResetRotation()
{
	m_v3Rotation.Set(0, 0, 0);
	UpdateRotation();
	GetDockDlg()->SetRotation(m_v3Rotation);
}

void CModelViewRender::PositionChanged()
{
	GetDockDlg()->GetPosition(m_v3Position);
	UpdatePosition();
}

void CModelViewRender::RotationChanged()
{
	GetDockDlg()->GetRotation(m_v3Rotation);
	UpdateRotation();
}

CModelViewToolBar *CModelViewRender::GetToolBar()
{
	CModelViewEditor *pEditor = SINGLETONINSTANCE(CModelViewEditor);
	ASSERT(pEditor != NULL);
	CModelViewToolBar *pToolbar = pEditor->GetToolBar();
	ASSERT(pToolbar != NULL);
	return pToolbar;
}

CModelViewDlg* CModelViewRender::GetDockDlg()
{
	CModelViewEditor *pEditor = SINGLETONINSTANCE(CModelViewEditor);
	ASSERT(pEditor != NULL);
	CModelViewDlg *pDlg = pEditor->GetDockDlg();
	ASSERT(pDlg != NULL);
	return pDlg;
}

void CModelViewRender::UpdatePosition()
{
	static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
	if (MSG_HANDLED != m_ToolBox->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), &m_v3Position, &m_hszEntityName))
	{
		m_ToolBox->Log( LOGERROR, _T("%s(%i): SetGlobalPosition not handled. \n"), __FILE__, __LINE__ );
	}
}

void CModelViewRender::UpdateRotation()
{
	static DWORD msgHash_SetGlobalEuler = CHashString(_T("SetGlobalEuler")).GetUniqueID();
	if (MSG_HANDLED != m_ToolBox->SendMessage(msgHash_SetGlobalEuler, sizeof(EulerAngle), &m_v3Rotation, &m_hszEntityName))
	{
		m_ToolBox->Log( LOGERROR, _T("%s(%i): SetGlobalEuler not handled. \n"), __FILE__, __LINE__ );
	}
}

void CModelViewRender::SetActiveScene()
{
	GETSETACTIVESCENE gsas;
	gsas.pSceneName = &m_hszSceneName;
	static DWORD msgHash_SetActiveScene = CHashString(_T("SetActiveScene")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetActiveScene, sizeof(gsas), &gsas );
}
