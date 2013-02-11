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

#ifndef _C_GUIFULLSCREEN_H_
#define _C_GUIFULLSCREEN_H_

class CGUIEditorDoc;
// CGUIFullScreen

class CGUIFullScreen : public CWnd
{
	DECLARE_DYNAMIC(CGUIFullScreen)

public:
	CGUIFullScreen();
	virtual ~CGUIFullScreen();
	void SetDocParent(CGUIEditorDoc *value) { m_Doc = value; }
	void SetResolution(UINT x, UINT y)
	{
		m_iXRes = x;
		m_iYRes = y;
	}
	void SetColor(UINT r, UINT g, UINT b);
	
protected:
	DECLARE_MESSAGE_MAP()
	CGUIEditorDoc *m_Doc;
	IToolBox *m_ToolBox;
	IRenderContext *m_RC;
	UINT m_iRenderContextID;
	UINT m_iXRes;
	UINT m_iYRes;
	int m_iLastMouseX;
	int m_iLastMouseY;
	bool m_bLeftButtonDown;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};


#endif