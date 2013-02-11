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
#include "CameraDockWin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCameraDockWin, CGuiControlBar)
BEGIN_MESSAGE_MAP(CCameraDockWin, CGuiControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()

	// click on the menuItem
	ON_COMMAND(ID_VIEW_CAMERATOOLBAR, OnViewCameraToolbar)

	// check/uncheck menuItem (it is always enabled)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAMERATOOLBAR, OnUpdateViewCameraToolbar)
END_MESSAGE_MAP()

CCameraDockWin::CCameraDockWin()
{
	m_ToolBox = EngineGetToolBox();
	m_CameraDialog = NULL;
}

CCameraDockWin::~CCameraDockWin()
{
	// free the resources
	if ( m_CameraDialog != NULL )
	{
		delete m_CameraDialog;
		m_CameraDialog = NULL;
	}
}

void CCameraDockWin::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
}

int CCameraDockWin::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// create our camera document
	m_CameraDialog = new CCameraDlg();

	// check for error
	if ( m_CameraDialog == NULL )
	{
		return -1;
	}

	// call base's create()
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// call our camera dialog's create()
	if (!m_CameraDialog->Create( m_CameraDialog->IDD, this ) )
		return -1;

	return 0;
}

void CCameraDockWin::OnSize(UINT nType, int cx, int cy)
{
	// Zsolt: I couldn't find difference if we don't call this, but I left it here
	// just to be sure... Why do we call this?
	m_CameraDialog->SetWindowPos(NULL, 0, 0, cx, cy, 0);
	m_CameraDialog->Invalidate();
}

void CCameraDockWin::OnViewCameraToolbar() 
{
	if ( IsVisible() || IsMinimized() )
	{
		// hides our window
		Close();
	}
	else
	{
		// GuiControlBar::Show redocks the controlbar if it
		// was docked or minimized before hiding.
		Show();
	}
}

void CCameraDockWin::OnUpdateViewCameraToolbar( CCmdUI* pCmdUI ) 
{
	// menu is checked if toolbar is visible
	pCmdUI->SetCheck( IsVisible() || IsMinimized() );
	
	// always enabled
	pCmdUI->Enable();
}
