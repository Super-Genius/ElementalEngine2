///============================================================================
/// \file		RenderView.h
/// \brief		Header file Render viewer window
/// \date		07-03-2005
/// \author		Kenneth Hurley
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
#ifndef _RENDERVIEW_H
#define _RENDERVIEW_H

// CRenderView view

class CRenderView : public CView
{
	DECLARE_DYNCREATE(CRenderView)

protected:
	CRenderView();           // protected constructor used by dynamic creation
	virtual ~CRenderView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnUpdateFileClose(CCmdUI *pCmdUI);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);

	// CView
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnInitialUpdate();

	void InitializeCamera();

private:
	///	\brief	Fill MOUSECOORDS structure with current mouse data
	///	\param	nFlags - mouse flags from windows mouse messages
	///	\param	point - current mouse position
	///	\param	pMouseCoords - pointer to MOUSECOORDS structure to fill
	void FillMouseData(UINT nFlags, const CPoint &point, MOUSECOORDS *pMouseCoords);

private:
	IToolBox *m_ToolBox;
	bool m_bInitialized;
	IHashString *m_hszCameraName;
	int m_iViewWidth;
	int m_iViewHeight;
	DWORD m_iContextID;
	bool m_bLDragFlag;
	CPoint m_LDragStartPoint;
	CPoint m_OldMousePos;
	bool m_bWireframemode;

	int m_iRenderViewWidth;
	int m_iRenderViewHeight;

	bool m_bPhysVisOn;

	CHashString m_hszSceneName;
	bool m_bIsActive;

public:
	CViewCamera m_ViewCamera;

	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRendererWireframe(CCmdUI *pCmdUI);
	afx_msg void OnUpdateReloadShaders(CCmdUI * pCmdUI );
	afx_msg void OnReloadShaders();
	afx_msg void OnRendererWireframe();
	LRESULT OnDropMessage(WPARAM wParam, LPARAM lParam);
};


#endif		// #ifndef _RENDERVIEW_H
