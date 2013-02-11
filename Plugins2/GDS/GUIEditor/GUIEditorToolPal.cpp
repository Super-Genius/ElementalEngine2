// GUIEditorToolPal.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUIEditorToolPal


CGUIEditorToolPal::CGUIEditorToolPal()
{
}


CGUIEditorToolPal::~CGUIEditorToolPal()
{
}

BEGIN_MESSAGE_MAP(CGUIEditorToolPal, CGuiToolBarWnd)
	//{{AFX_MSG_MAP(CGUIEditorToolPal)
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
// CGUIEditorToolPal message handlers


void CGUIEditorToolPal::On3dButton() 
{
	MessageBox(_T("3d Button Pressed"), _T("Button Pressed"));	
}


void CGUIEditorToolPal::OnUpdate3dButton(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}


void CGUIEditorToolPal::OnTestPalButton2() 
{
	MessageBox(_T("Test Palette Button 2 Pressed"), _T("Button Pressed"));		
}


void CGUIEditorToolPal::OnUpdateTestPalButton2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}


void CGUIEditorToolPal::OnTestPalButton3() 
{
	MessageBox(_T("Test Palette Button 3 Pressed"), _T("Button Pressed"));
}


void CGUIEditorToolPal::OnUpdateTestPalButton3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}


int CGUIEditorToolPal::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGuiToolBarWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// make sure we get messages
	GetToolBarCtrl().SetOwner(this);

	StyleDispl(GUISTYLE_XP);
	
	return 0;
}
