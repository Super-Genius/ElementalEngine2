///============================================================================
/// \file		SplineComponent.cpp
/// \brief		Implementation file for Spline Toolbar
/// \date		11-14-2006
/// \author		Richard Nguyen
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
// CSplineToolPal


CSplineToolPal::CSplineToolPal()
{
}


CSplineToolPal::~CSplineToolPal()
{
}

BEGIN_MESSAGE_MAP(CSplineToolPal, CGuiToolBarWnd)
	//{{AFX_MSG_MAP(CSplineToolPal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplineToolPal message handlers


void CSplineToolPal::On3dButton() 
{
	MessageBox(_T("3d Button Pressed"), _T("Button Pressed"));	
}


void CSplineToolPal::OnUpdate3dButton(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}


void CSplineToolPal::OnTestPalButton2() 
{
	MessageBox(_T("Test Palette Button 2 Pressed"), _T("Button Pressed"));		
}


void CSplineToolPal::OnUpdateTestPalButton2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}


void CSplineToolPal::OnTestPalButton3() 
{
	MessageBox(_T("Test Palette Button 3 Pressed"), _T("Button Pressed"));
}


void CSplineToolPal::OnUpdateTestPalButton3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}


int CSplineToolPal::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGuiToolBarWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// make sure we get messages
	GetToolBarCtrl().SetOwner(this);

	StyleDispl(GUISTYLE_XP);
	
	return 0;
}
