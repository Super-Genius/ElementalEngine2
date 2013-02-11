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

// Memory Leak Debug Define
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CModelViewToolBar, CGuiDialogBar)

BEGIN_MESSAGE_MAP(CModelViewToolBar, CGuiDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
END_MESSAGE_MAP()

CModelViewToolBar::CModelViewToolBar()
	: m_ToolBox(EngineGetToolBox())
{
}

CModelViewToolBar::~CModelViewToolBar()
{
}

LRESULT CModelViewToolBar::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);
	UpdateData(FALSE);

	m_ZoomExtentsBt.SetIcon((HICON)LoadImage(ModelViewEditorDLL.hModule, 
		MAKEINTRESOURCE(IDI_ZOOMEXTENTS), IMAGE_ICON, 0, 0, 0));	

	return bRet;
}

void CModelViewToolBar::DoDataExchange(CDataExchange* pDX)
{
	CGuiDialogBar::DoDataExchange(pDX);
	// Animation
	DDX_Control(pDX, IDC_MODELVIEW_TB_BUTTON_ZOOMEXTENTS, m_ZoomExtentsBt);
}
