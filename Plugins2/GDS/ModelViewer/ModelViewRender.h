///============================================================================
/// \file		ModelViewRender.h
/// \brief		view declaration for ModelViewer
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

#ifndef _GDS_MODELVIEWRENDER_H_
#define _GDS_MODELVIEWRENDER_H_

// CModelViewRender view

class CModelViewRender : public CView
{
	DECLARE_DYNCREATE(CModelViewRender)

protected:
	CModelViewRender();           // protected constructor used by dynamic creation
	virtual ~CModelViewRender();

public:
	IHashString *GetEntityName();
	bool HasAnimation() const;
	int GetCurrentAnimationID() const;
	void PlayNextAnimation();
	void Update();
	void SetFlagPlayNextInSequence() { m_bPlayNextInSequence = true; }

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
public:
	afx_msg void PositionChanged();
	afx_msg void RotationChanged();

	afx_msg void ResetPosition();
	afx_msg void ResetRotation();
	afx_msg void ZoomToExtents();

	void PlayAnimation( bool bLoop, int nAnimationId );
	void PlaySequence( bool bLoop, ANIMATIONSEQUENCE &animationSequence );
	void StopSequence();

	afx_msg void OnUpdatePlayAnimation(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlaySequence(CCmdUI* pCmdUI);

private:
	/// \brief	helper for getting get Model View toolbar
	/// \return	pointer to the Model View toolbar instance
	static CModelViewToolBar *GetToolBar();
	static CModelViewDlg* GetDockDlg();

	void ActivateView();
	void DeactivateView();

	void PlaySequence(const ANIMATIONSEQUENCE &sequence);

	void InitRenderScene();
	void LoadModel();
	void FillAnimationList();
	void SetActiveScene();
	void CreateScene();
	void CalcScaleValues();
	
	void UpdatePosition();
	void UpdateRotation();

private:
	static CModelViewRender	   *sm_pActiveRenderer;

	CPoint						m_ptLastMousePos;
	IToolBox*					m_ToolBox;
	CViewCamera					m_ViewCamera;
	CRenderContext				m_RenderContext;
	Vec3						m_v3CameraPos;
	EulerAngle					m_v3Rotation;			// Model's rotation HACK
	Vec3						m_v3Position;			// Model's rotation HACK

	float						m_fZoomDistance;
	float						m_fScaleOffset;
	float						m_fScaleZoom;

	// scene object names
	CHashString					m_hszSceneName;
	CHashString					m_hszModelName;
	CHashString					m_hszEntityName;
	CHashString					m_hszLightEntityName;
	CHashString					m_hszLightObjectName;

	ANIMATIONLIST				m_AnimationList;
	int							m_nCurrentAnimationId;
	ANIMATIONSEQUENCE			m_AnimationSequence;
	vector<IArchive*>			m_AnimationCallbackArchives;
	
	ANIMATIONSEQUENCE			m_PlayingAnimationSequence;
	ANIMATIONSEQUENCE::const_iterator	m_itCurrentAnimationID;
	bool						m_bLoop;

	bool						m_bActivated;
	bool						m_bPlayNextInSequence;
};

#endif //_GDS_MODELVIEWRENDER_H_
