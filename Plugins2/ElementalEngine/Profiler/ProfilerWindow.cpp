///============================================================================
/// \file		ProfilerWindow.cpp
/// \brief		Window class for profiler window
/// \date		06/11/2007
/// \author		Nick Tourte
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
#include ".\profilerwindow.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CProfilerWindow, CWnd)
CProfilerWindow::CProfilerWindow():
	CWnd()
{
	m_TabControlBox = NULL;	

	CURSORINFO cInfo;
	cInfo.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&cInfo);

	if (cInfo.flags == 0)
	{
		m_bHiddenCursor = true;
	}
	else
	{
		m_bHiddenCursor = false;
	}
	ShowCursor(TRUE);
}

CProfilerWindow::~CProfilerWindow()
{
	if (m_TabControlBox)
	{
		m_TabControlBox->DestroyWindow();
		delete m_TabControlBox;
		m_TabControlBox = NULL;
	}
}


BEGIN_MESSAGE_MAP(CProfilerWindow, CWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()	
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

int CProfilerWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	RECT winRect;
	GetClientRect(&winRect);
	m_TabControlBox = new CProfilerTabCtrl();
	m_TabControlBox->Create(IDD_TABBOX, this);	
	m_TabControlBox->ResizeWindow(winRect.left, winRect.top, winRect.right, winRect.bottom);

	return 0;
}

void CProfilerWindow::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);	
}

void CProfilerWindow::UpdateProfiles(PERFORMANCEPROFILERTYPEMAP &map)
{
	m_TabControlBox->UpdateProfiles(map);
}

void CProfilerWindow::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (m_TabControlBox)
	{
		RECT rect;
		GetClientRect(&rect);
		m_TabControlBox->ResizeWindow(rect.left, rect.top, rect.right, rect.bottom);
	}
}
void CProfilerWindow::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	if (bShow == TRUE)
	{
		ShowCursor(TRUE);
	}
	else
	{
		if (m_bHiddenCursor)
		{
			ShowCursor(FALSE);
		}
	}
}

void CProfilerWindow::UpdateFPS(float fps)
{
	if (m_TabControlBox)
	{
		m_TabControlBox->UpdateFPS(fps);
	}
}