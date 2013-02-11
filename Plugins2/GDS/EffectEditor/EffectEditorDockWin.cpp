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

IMPLEMENT_DYNAMIC(CEffectEditorDockWin, CGuiControlBar)
BEGIN_MESSAGE_MAP(CEffectEditorDockWin, CGuiControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

CEffectEditorDockWin::CEffectEditorDockWin()
{
	m_ToolBox = EngineGetToolBox();
}

CEffectEditorDockWin::~CEffectEditorDockWin()
{
}

void CEffectEditorDockWin::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
}

int CEffectEditorDockWin::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_EffectEditorDialog.Create( m_EffectEditorDialog.IDD, this ))
		return -1;

	return 0;
}

void CEffectEditorDockWin::OnSize(UINT nType, int cx, int cy)
{
	m_EffectEditorDialog.SetWindowPos(NULL, 0, 0, cx, cy, 0);
	m_EffectEditorDialog.Invalidate();
}
