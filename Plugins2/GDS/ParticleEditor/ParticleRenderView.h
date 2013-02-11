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

#ifndef PARTICLERENDERVIEW_H
#define PARTICLERENDERVIEW_H

#include "stdafx.h"
#include "ParticleDoc.h"

class CParticleEditorToolPal;

class CParticleRenderView : public CView
{
// Macros and MFC Functions
	DECLARE_DYNCREATE(CParticleRenderView)
	DECLARE_MESSAGE_MAP()
public:
	CParticleRenderView();           // protected constructor used by dynamic creation
	virtual ~CParticleRenderView();

	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

	void ToggleParticleSystem( bool bShow );
	void Update();

	void MoveParticle();
	
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnToggleParticleSystem();

private:
	void OnActivate();
	void OnDeactivate();

	CParticleDoc *GetDocument()
	{
		return static_cast<CParticleDoc *>(CView::GetDocument());
	}
	
	static CParticleEditorToolPal *GetToolBar();

private:
	IToolBox*					m_ToolBox;
	Vec3						m_v3CameraPos;
	EulerAngle					m_eulCameraRot;
	Vec3						m_v3ParticlePos;
	EulerAngle					m_v3ParticleRotation;
	CPoint						m_pointLastMousePos;

	bool						m_bShowParticles;
	bool						m_bMove;
	float						m_fMoveSpeed;
	float						m_fLastTime_Seconds;
	float						m_fCurrentTime_Seconds;
	float						m_fDistanceMoved;

	CViewCamera					m_ViewCamera;
	CRenderContext				m_RenderContext;
	bool						m_bActive;
};
#endif /* #ifndef PARTICLERENDERVIEW_H */

