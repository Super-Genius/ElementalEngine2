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
#include "HelpSystem.h"
#include "SizeBar.h"
#include ".\sizebar.h"


// CSizeBar

IMPLEMENT_DYNAMIC(CSizeBar, CWnd)
CSizeBar::CSizeBar()
{
}

CSizeBar::~CSizeBar()
{
}


BEGIN_MESSAGE_MAP(CSizeBar, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()



// CSizeBar message handlers

BOOL CSizeBar::Create(
	CWnd * pParentWnd, bool bHorizontal, DWORD dwStyle, LONG lSize
)
{
	m_area = CRect();
	m_lLeftBound = 0;
	m_lRightBound = 0;
	m_lPos = 0;
	m_bLeftVisible = true;
	m_bHorizontal = bHorizontal;
	m_lSize = lSize;
	m_pLeft = NULL;
	m_pRight = NULL;
	m_lWPos = -1;
	m_bIsDrag = false;
	LPCTSTR lpszClassName = AfxRegisterWndClass(0, 0, (HBRUSH)(COLOR_3DFACE + 1), 0);
	m_hCursor = m_bHorizontal ? LoadCursor(NULL, IDC_SIZENS) : LoadCursor(NULL, IDC_SIZEWE);
	return CWnd::CreateEx(
		0, lpszClassName, NULL, dwStyle, CRect(), pParentWnd, 0, 0
	);
}

void CSizeBar::SetBounds(LONG lLeftBound, LONG lRightBound)
{
	UpdateControlled(
		m_area, (lLeftBound < 0) ? 0 : lLeftBound,
		(lRightBound < 0) ? 0 : lRightBound, m_lPos,
		m_bLeftVisible
	);
}

void CSizeBar::SetPosition(LONG lPos)
{
	UpdateControlled(m_area, m_lLeftBound, m_lRightBound, lPos, m_bLeftVisible);
}

void CSizeBar::SetControlled(CWnd *pLeft, CWnd *pRight)
{
	m_pLeft = pLeft;
	m_pRight = pRight;
	UpdateControlled(m_area, m_lLeftBound, m_lRightBound, m_lPos, m_bLeftVisible, true);
}

bool CSizeBar::GetLeftVisible()
{
	return m_bLeftVisible;
}

void CSizeBar::SetLeftVisible(bool bVisible)
{
	UpdateControlled(m_area, m_lLeftBound, m_lRightBound, m_lPos, bVisible);
}

void CSizeBar::UpdateArea(CRect const &area)
{
	UpdateControlled(area, m_lLeftBound, m_lRightBound, m_lPos, m_bLeftVisible);
}

void CSizeBar::UpdateControlled(
	CRect const &area, LONG lLeftBound, LONG lRightBound, LONG lPos, bool bLeftVisible, bool lForce
)
{
	if (m_pLeft->GetSafeHwnd() && m_pRight->GetSafeHwnd() && !area.IsRectEmpty())
	{
		if (bLeftVisible)
		{
			LONG maxPos = (m_bHorizontal ? area.Height() : area.Width()) - lRightBound;
			if (lPos > maxPos)
			{
				lPos = maxPos;
			}
			if (lPos < lLeftBound)
			{
				lPos = lLeftBound;
			}
		}
		if (lForce || (area != m_area) || (lPos != m_lPos) || (bLeftVisible != m_bLeftVisible))
		{
			if (bLeftVisible != m_bLeftVisible)
			{
				if (bLeftVisible)
				{
					m_pLeft->ShowWindow(SW_SHOWNA);
					ShowWindow(SW_SHOWNA);
				}
				else
				{
					m_pLeft->ShowWindow(SW_HIDE);
					ShowWindow(SW_HIDE);
				}
			}
			HDWP hdwp = BeginDeferWindowPos(3);
			if (bLeftVisible)
			{
				if (m_bHorizontal)
				{
					DeferWindowPos(
						hdwp, *this, NULL,
						area.left, area.top + lPos,
						area.right - area.left, m_lSize,
						SWP_NOACTIVATE | SWP_NOREPOSITION | SWP_NOZORDER
					);
					DeferWindowPos(
						hdwp, *m_pLeft, NULL,
						area.left, area.top, area.right - area.left, lPos,
						SWP_NOACTIVATE | SWP_NOREPOSITION | SWP_NOZORDER
					);
					DeferWindowPos(
						hdwp, *m_pRight, NULL,
						area.left, area.top + lPos + m_lSize,
						area.right - area.left, area.bottom - area.top - lPos - m_lSize,
						SWP_NOACTIVATE | SWP_NOREPOSITION | SWP_NOZORDER
					);
				}
				else
				{
					DeferWindowPos(
						hdwp, *this, NULL,
						area.left + lPos, area.top,
						m_lSize, area.bottom - area.top,
						SWP_NOACTIVATE | SWP_NOREPOSITION | SWP_NOZORDER
					);
					DeferWindowPos(
						hdwp, *m_pLeft, NULL,
						area.left, area.top, lPos, area.bottom - area.top,
						SWP_NOACTIVATE | SWP_NOREPOSITION | SWP_NOZORDER
					);
					DeferWindowPos(
						hdwp, *m_pRight, NULL,
						area.left + lPos + m_lSize, area.top,
						area.right - area.left - lPos - m_lSize, area.bottom - area.top,
						SWP_NOACTIVATE | SWP_NOREPOSITION | SWP_NOZORDER
					);
				}
			}
			else
			{
				DeferWindowPos(
					hdwp, *m_pRight, NULL,
					area.left, area.top, area.right - area.left, area.bottom - area.top,
					SWP_NOACTIVATE | SWP_NOREPOSITION | SWP_NOZORDER
				);
			}
			EndDeferWindowPos(hdwp);
		}
	}
	m_area = area;
	m_lLeftBound = lLeftBound;
	m_lRightBound = lRightBound;
	m_lPos = lPos;
	m_bLeftVisible = bLeftVisible;
}


void CSizeBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bIsDrag)
	{
		if (GetCursorPos(&m_startPoint))
		{
			m_startPos = m_lPos;
			m_prevCapture = SetCapture()->GetSafeHwnd();
			m_bIsDrag = true;
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CSizeBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bIsDrag)
	{
		if (m_prevCapture)
		{
			::SetCapture(m_prevCapture);
		}
		else
		{
			ReleaseCapture();
		}
		m_bIsDrag = false;
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CSizeBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bIsDrag)
	{
		POINT mpoint;
		if (GetCursorPos(&mpoint))
		{
			if (m_bHorizontal)
			{
				UpdateControlled(m_area, m_lLeftBound, m_lRightBound, m_startPos + mpoint.y - m_startPoint.y, m_bLeftVisible);
			}
			else
			{
				UpdateControlled(m_area, m_lLeftBound, m_lRightBound, m_startPos + mpoint.x - m_startPoint.x, m_bLeftVisible);
			}
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CSizeBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (!CWnd::OnSetCursor(pWnd, nHitTest, message))
	{
		SetCursor(m_hCursor);
	}
	return TRUE;
}
