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

#if !defined(AFX_GUIEDITORVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_)
#define AFX_GUIEDITORVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUIEditorView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUIEditorView view


class CGUIEditorView : public CView
{
protected:
	
	CGUIEditorView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGUIEditorView)

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	
// Implementation
protected:
	
	virtual ~CGUIEditorView();
#ifdef _DEBUG
	
	virtual void AssertValid() const;
	
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:

	void Init();

	DECLARE_MESSAGE_MAP()
public:

	void SetResolution(UINT x, UINT y)
	{
		m_iXRes = x;
		m_iYRes = y;
	}

	void SetBGColor(UINT r, UINT g, UINT b)
	{
		if (m_pRC)
		{
			m_pRC->SetBGColor(r, g, b);
		}
	}

	void SetObjectToSize(CHashString objName, CHashString objType)
	{
		m_szObjName = objName;
		m_szObjType = objType;
		m_iState = 1;
	}

	void SetSelectedPage(CHashString pName) { m_szCurrentPage = pName; }
	void SetSelectedGroup(CHashString gName) { m_szCurrentGroup = gName; }
	void SetSelectedObject(CHashString oName, CHashString oType)
	{
		m_szObjName = oName;
		m_szObjType = oType;
	}
	void SetPageOffset(int x, int y)
	{
		m_iPageOffsetX = x;
		m_iPageOffsetY = y;
	}

	void SetFullscreenMode(bool mode) { m_bFullscreenMode = mode; }
	void SetZoom(float value) { m_fZoomFactor = value; }
	void ResetView();
	void SetSystemIndex(UINT value);
private:
	CHashString m_szObjName;
	CHashString m_szObjType;	
	CHashString m_szCurrentPage;
	CHashString m_szCurrentGroup;
	CGUIEditorFrm *m_pParent;
	ITextureObject *m_pRenderTarget;
	bool m_bInitialized;
	IRenderContext *m_pRC;
	UINT m_iRenderContextID;
	UINT m_iXRes, m_iYRes;
	IToolBox *m_ToolBox;
	int m_iLastMousePosX;
	int m_iLastMousePosY;
	int m_iLastActualMousePosX;
	int m_iLastActualMousePosY;
	float m_fElementPosX;
	float m_fElementPosY;

	bool m_bLeftButtonPress;
	bool m_bSpacePressed;
	bool m_bShiftPressed;
	bool m_bCtrlPressed;
	bool m_bDeletePressed;		

	int m_iPageOffsetX;
	int m_iPageOffsetY;
	float m_fZoomFactor;
	int m_iState;
	int m_iHandleIndex;
	bool m_bFullscreenMode;	

	UINT m_iSystemIndex;

	bool m_bLastWasVisible;
	
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCopyItem();
	afx_msg void OnPasteNew();
	afx_msg void OnPasteInst();	
	afx_msg void OnShowHide();
	afx_msg void OnAnimTest();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIEDITORVIEW_H__305F5776_A2DD_4E37_B8E4_2E140F34F290__INCLUDED_)
