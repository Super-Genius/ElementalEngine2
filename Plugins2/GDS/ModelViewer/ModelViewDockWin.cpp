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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CModelViewDockWin, CGuiControlBar)
BEGIN_MESSAGE_MAP(CModelViewDockWin, CGuiControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

CModelViewDockWin::CModelViewDockWin()
{
	m_ToolBox = EngineGetToolBox();
}

CModelViewDockWin::~CModelViewDockWin()
{
}

void CModelViewDockWin::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
}

int CModelViewDockWin::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_ModelViewDialog.Create( m_ModelViewDialog.IDD, this ))
		return -1;

	return 0;
}

void CModelViewDockWin::OnSize(UINT nType, int cx, int cy)
{
	m_ModelViewDialog.SetWindowPos(NULL, 0, 0, cx, cy, 0);
	m_ModelViewDialog.Invalidate();
}
