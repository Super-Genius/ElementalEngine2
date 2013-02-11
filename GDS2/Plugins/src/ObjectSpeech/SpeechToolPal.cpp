// SpeechToolPal.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpeechToolPal


CSpeechToolPal::CSpeechToolPal()
{
}


CSpeechToolPal::~CSpeechToolPal()
{
}

BEGIN_MESSAGE_MAP(CSpeechToolPal, CGuiToolBarWnd)
	//{{AFX_MSG_MAP(CSpeechToolPal)
	ON_COMMAND(ID_TEST_PAL_BUTTON_3, OnTestPalButton3)
	ON_UPDATE_COMMAND_UI(ID_TEST_PAL_BUTTON_3, OnUpdateTestPalButton3)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpeechToolPal message handlers




/*

void CSpeechToolPal::OnTestPalButton2() 
{
	MessageBox(_T("Parsing Object!!"), _T("Button Pressed"));
		
}


void CSpeechToolPal::OnUpdateTestPalButton2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}
*/ 


void CSpeechToolPal::OnTestPalButton3() 
{
	MessageBox(_T("Test Palette Button 3 Pressed"), _T("Button Pressed"));
}


void CSpeechToolPal::OnUpdateTestPalButton3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}



int CSpeechToolPal::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGuiToolBarWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// make sure we get messages
	GetToolBarCtrl().SetOwner(this);
	
	return 0;
}
