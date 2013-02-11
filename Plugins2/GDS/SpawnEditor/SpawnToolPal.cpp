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

/////////////////////////////////////////////////////////////////////////////
// CSpawnToolPal


CSpawnToolPal::CSpawnToolPal()
{
}


CSpawnToolPal::~CSpawnToolPal()
{
}

BEGIN_MESSAGE_MAP(CSpawnToolPal, CGuiToolBarWnd)
	//{{AFX_MSG_MAP(CSpawnToolPal)
	ON_COMMAND(ID_3D_BUTTON, On3dButton)
	ON_UPDATE_COMMAND_UI(ID_3D_BUTTON, OnUpdate3dButton)
	ON_COMMAND(ID_TEST_PAL_BUTTON_2, OnTestPalButton2)
	ON_UPDATE_COMMAND_UI(ID_TEST_PAL_BUTTON_2, OnUpdateTestPalButton2)
	ON_COMMAND(ID_TEST_PAL_BUTTON_3, OnTestPalButton3)
	ON_UPDATE_COMMAND_UI(ID_TEST_PAL_BUTTON_3, OnUpdateTestPalButton3)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpawnToolPal message handlers


void CSpawnToolPal::On3dButton() 
{
	MessageBox(_T("3d Button Pressed"), _T("Button Pressed"));	
}


void CSpawnToolPal::OnUpdate3dButton(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}


void CSpawnToolPal::OnTestPalButton2() 
{
	MessageBox(_T("Test Palette Button 2 Pressed"), _T("Button Pressed"));		
}


void CSpawnToolPal::OnUpdateTestPalButton2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}


void CSpawnToolPal::OnTestPalButton3() 
{
	MessageBox(_T("Test Palette Button 3 Pressed"), _T("Button Pressed"));
}


void CSpawnToolPal::OnUpdateTestPalButton3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}


int CSpawnToolPal::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGuiToolBarWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// make sure we get messages
	GetToolBarCtrl().SetOwner(this);

	StyleDispl(GUISTYLE_XP);
	
	return 0;
}
