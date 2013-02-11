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

#pragma once

#include "ViewCamera.h"

// CEffectEditorView view

class CEffectEditorView : public CView
{
	DECLARE_DYNCREATE(CEffectEditorView)

protected:
	CEffectEditorView();           // protected constructor used by dynamic creation
	virtual ~CEffectEditorView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	IToolBox *m_ToolBox;
	CEffectEditor *m_pEditor;
	CHashString m_hszSceneName;
	CHashString m_hszSceneType;
	CHashString m_hszCameraName;
	UINT m_uiRenderContextID;
	IRenderContext *m_pRenderContext;
	CViewCamera m_ViewCamera;

	CHashString m_hszCameraLight;
	bool m_bCameraLightActive;
	CHashString m_hszSceneLight;
	bool m_bSceneLightActive;
	CHashString m_hszGroundModel;
	bool m_bGroundModelActive;
	CHashString m_hszEffectModel;
	bool m_bEffectModelActive;
	bool m_bActive;

	DECLARE_MESSAGE_MAP()
public:
	void ToggleCameraLight();
	void ToggleSceneLight();
	void ToggleGroundModel();
	void ToggleEffectModel();
	void SetEffectModelEffect( IEffect *pEffect );

	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};


