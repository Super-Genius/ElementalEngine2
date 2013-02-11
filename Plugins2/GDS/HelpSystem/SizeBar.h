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


// CSizeBar

class CSizeBar : public CWnd
{
	DECLARE_DYNAMIC(CSizeBar)

public:
	CSizeBar();
	virtual ~CSizeBar();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create(
		CWnd *pParentWnd, bool bHorizontal = false,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE, LONG lSize = 4
	);

	void SetBounds(LONG lLeftBound, LONG lRightBound);
	void SetPosition(LONG lPos);
	void SetControlled(CWnd *pLeft, CWnd *pRight);

	bool GetLeftVisible();
	void SetLeftVisible(bool bVisible);

	void UpdateArea(CRect const &area);

private:
	void UpdateControlled(
		CRect const &area, LONG lLeftBound, LONG lRightBound, LONG lPos,
		bool bLeftVisible, bool lForce = false
	);

	CRect m_area;
	LONG m_lLeftBound;
	LONG m_lRightBound;
	LONG m_lPos;
	bool m_bLeftVisible;

	bool m_bHorizontal;
	LONG m_lSize;
	CWnd *m_pLeft;
	CWnd *m_pRight;

	LONG m_lWPos;
	bool m_bIsDrag;
	HWND m_prevCapture;
	POINT m_startPoint;
	LONG m_startPos;
	HCURSOR m_hCursor;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};


