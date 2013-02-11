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
#include "ParticleRenderView.h"
#include "ObjectHelpers.h"

// CParticleRenderView

IMPLEMENT_DYNCREATE(CParticleRenderView, CView)

CParticleRenderView::CParticleRenderView()
	: m_v3CameraPos(0.0, 0.0, 0.0)
	, m_eulCameraRot(0.0, 0.0, 0.0)
	, m_v3ParticlePos(0.0, 0.0, 500.0)
	, m_bShowParticles(false)
	, m_bMove(false)
	, m_fMoveSpeed(0.0f)
	, m_fLastTime_Seconds(0.0f)
	, m_fCurrentTime_Seconds(0.0f)
	, m_fDistanceMoved(0.0f)
	, m_ToolBox(EngineGetToolBox())
	, m_bActive(false)
{
	SINGLETONINSTANCE(CParticleEditorComponent)->RegisterView(this);
}

void CParticleRenderView::Update()
{
	//OnToggleParticleSystem();
	MoveParticle();
}

CParticleRenderView::~CParticleRenderView()
{
	SINGLETONINSTANCE(CParticleEditorComponent)->DeRegisterView(this);
}

BEGIN_MESSAGE_MAP(CParticleRenderView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_PARTICLES_SHOWPARTICLES, OnToggleParticleSystem)
END_MESSAGE_MAP()

int CParticleRenderView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CView::OnCreate (lpCreateStruct) == -1)
	{
		return -1;
	}
	GetDocument()->CreateScene();
	if (!m_RenderContext.Create(*this, false))
	{
		return -1;
	}
	// set model viewer scene's render context
	if (!m_RenderContext.SetScene(GetDocument()->GetSceneName()))
	{
		return -1;
	}

	if (!m_ViewCamera.CreateCamera(GetDocument()->GetSceneName()))
	{
		return -1;
	}

	if (!m_RenderContext.SetActiveCamera(m_ViewCamera))
	{
		return -1;
	}

	m_RenderContext.SetCameraPosition(m_v3CameraPos);

	m_RenderContext.SetCameraOrientation(m_eulCameraRot);

	static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetGlobalPosition,
									sizeof(m_v3ParticlePos),
									&m_v3ParticlePos,
									GetDocument()->GetEntityName());

	return 0;
}


void CParticleRenderView::OnToggleParticleSystem()
{
	ToggleParticleSystem(GetToolBar()->AreParticlesShown());
	static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetGlobalPosition,
									sizeof(m_v3ParticlePos),
									&m_v3ParticlePos,
									GetDocument()->GetEntityName());
}

// This Destroys or Creates the Particle system and 
// sends a message to EE to hook or unhook the object
// packer to forward particles too the editor's render
// context.
void CParticleRenderView::ToggleParticleSystem( bool bShow )
{
	// TODO: This logic should be reviewed for multiple opened particle files
	PARTICLEEDITORRENDERMESSAGE perm;
	perm.render = bShow;
	perm.rendercontext = m_RenderContext.GetRenderContext();

	if( !m_bShowParticles && bShow )
	{
		static DWORD msgHash_EditorDisplay = CHashString(_T("EditorDisplay")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_EditorDisplay, sizeof(PARTICLEEDITORRENDERMESSAGE), &perm);
		GetDocument()->CreateParticleSystem();
		m_bShowParticles = true;
	}
	else if( m_bShowParticles && !bShow )
	{
		static DWORD msgHash_EditorDisplay = CHashString(_T("EditorDisplay")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_EditorDisplay, sizeof(PARTICLEEDITORRENDERMESSAGE), &perm);
		GetDocument()->RemoveParticleSystem();
		m_bShowParticles = false;
	}
}

void CParticleRenderView::OnDraw(CDC* pDC)
{
	m_RenderContext.Draw();
}

// CParticleRenderView message handlers
void CParticleRenderView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize (nType, cx, cy);
	m_RenderContext.SetWindowSize(cx, cy);
}

void CParticleRenderView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	GetDocument()->SetActiveScene();

	OnToggleParticleSystem();
}

BOOL CParticleRenderView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	float change = (float)zDelta / 120.0f;
	m_v3ParticlePos.z -= change * 15;

	if (m_v3ParticlePos.z < 0.0)
	{
		m_v3ParticlePos.z = 0.0;		
	}


	DWORD result;
	static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_SetGlobalPosition,
									sizeof(m_v3ParticlePos),
									&m_v3ParticlePos,
									GetDocument()->GetEntityName());

	if (result != MSG_HANDLED)
	{
		// output error message here.
	}

//	m_RenderContext.SetCameraPosition(m_v3CameraPos);
//	m_ToolBox->Log(LOGWARNING, _T("m_v3ParticlePos (%f, %f, %f)\n"), m_v3ParticlePos.x, m_v3ParticlePos.y, m_v3ParticlePos.z );

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CParticleRenderView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (0 != (nFlags & MK_RBUTTON))
	{
		m_v3ParticleRotation.yaw += point.x - m_pointLastMousePos.x;
		m_v3ParticleRotation.pitch += point.y - m_pointLastMousePos.y;

		if (m_v3ParticleRotation.yaw > 180)
		{
			m_v3ParticleRotation.yaw = 180;
		}
		else if (m_v3ParticleRotation.yaw < -180)
		{
			m_v3ParticleRotation.yaw = -180;
		}
		if (m_v3ParticleRotation.pitch > 180)
		{
			m_v3ParticleRotation.pitch = 180;
		}
		else if (m_v3ParticleRotation.pitch < -180)
		{
			m_v3ParticleRotation.pitch = -180;
		}
	
		static DWORD msgHash_SetGlobalEuler = CHashString(_T("SetGlobalEuler")).GetUniqueID();
		if (MSG_HANDLED != m_ToolBox->SendMessage(msgHash_SetGlobalEuler,
												  sizeof(m_v3ParticleRotation),
												  &m_v3ParticleRotation,
												  GetDocument()->GetEntityName()))
		{
			m_ToolBox->Log(LOGWARNING, _T("Failed to update object position\n"));
		}
	}

	m_pointLastMousePos = point;
	CView::OnMouseMove(nFlags, point);
}

void CParticleRenderView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	CView::OnRButtonDown(nFlags, point);
}

void CParticleRenderView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	CView::OnRButtonUp(nFlags, point);
}

void CParticleRenderView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (m_bActive != (bActivate != FALSE))
	{
		m_bActive = (bActivate != FALSE);
		
		CGuiMDIFrame *mainWnd = DYNAMIC_DOWNCAST(CGuiMDIFrame, AfxGetMainWnd());

		if (bActivate)
		{
			mainWnd->LoadDocumentBars(_T("ParticleEditor"));
			GetDocument()->SetActiveScene();
			OnActivate();
			SINGLETONINSTANCE(CParticleEditor)->RefreshList(GetDocument());
		}
		else
		{
			mainWnd->SaveDocumentBars(_T("ParticleEditor"));
			OnDeactivate();
		}
	}
}

void CParticleRenderView::MoveParticle()
{
	DWORD result;
	m_fLastTime_Seconds = m_fCurrentTime_Seconds;

	static DWORD msgHash_GetTime = CHashString(_T("GetTime")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetTime, sizeof(float), (void*)&m_fCurrentTime_Seconds);

	m_fMoveSpeed = GetToolBar()->GetMoveSpeed();
	if (m_fMoveSpeed > 0.0f)
	{
		// seconds this frame.
		float fDeltaTime_Seconds = m_fCurrentTime_Seconds - m_fLastTime_Seconds;
		// m = m/s * s
		float fMoveDist = m_fMoveSpeed * fDeltaTime_Seconds;
		
		m_fDistanceMoved += fMoveDist;

		float fSpinRadius = m_fMoveSpeed;
		if (fSpinRadius < 100.0f)
			fSpinRadius = 100.0f;
		// Move Particle
		m_v3ParticlePos.y = fSpinRadius * sin ( m_fDistanceMoved / m_fMoveSpeed );
		m_v3ParticlePos.x = fSpinRadius * cos ( m_fDistanceMoved / m_fMoveSpeed );

		static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
		result = m_ToolBox->SendMessage(msgHash_SetGlobalPosition,
										sizeof(m_v3ParticlePos),
										&m_v3ParticlePos,
										GetDocument()->GetEntityName());

		if (result != MSG_HANDLED)
		{
			// output error message here.
		}
	}
	else 
	{
		m_fDistanceMoved = 0.0f;
		m_v3ParticlePos.y = 0.0;
		m_v3ParticlePos.x = 0.0;

		static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
		result = m_ToolBox->SendMessage(msgHash_SetGlobalPosition,
										sizeof(m_v3ParticlePos),
										&m_v3ParticlePos,
										GetDocument()->GetEntityName());

		if (result != MSG_HANDLED)
		{
			// output error message here.
		}
	}
}

CParticleEditorToolPal * CParticleRenderView::GetToolBar()
{
	return SINGLETONINSTANCE(CParticleEditor)->GetToolBar();
}

void CParticleRenderView::OnActivate()
{
	CParticleEditorToolPal *pToolBar = GetToolBar();
	pToolBar->ShowParticles(m_bShowParticles);
	pToolBar->SetMoveSpeed(m_fMoveSpeed);
}

void CParticleRenderView::OnDeactivate()
{
	CParticleEditorToolPal *pToolBar = GetToolBar();
	m_bShowParticles = pToolBar->AreParticlesShown();
	m_fMoveSpeed = pToolBar->GetMoveSpeed();
	SINGLETONINSTANCE(CParticleEditor)->RefreshList(NULL);
}