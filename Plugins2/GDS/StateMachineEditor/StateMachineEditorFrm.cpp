///============================================================================
/// \file		StateMachineEditorFrm.cpp
/// \brief		Class implementation for CStateMachineEditorFrm
/// \date		03-06-2007
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
// CStateMachineEditorFrm

IMPLEMENT_DYNCREATE(CStateMachineEditorFrm, CMDIChildWnd)

CStateMachineEditorFrm::CStateMachineEditorFrm()
{
	m_ToolBox = EngineGetToolBox();
}

CStateMachineEditorFrm::~CStateMachineEditorFrm()
{
}


BEGIN_MESSAGE_MAP(CStateMachineEditorFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CStateMachineEditorFrm)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStateMachineEditorFrm message handlers

int CStateMachineEditorFrm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CMDIFrameWnd *mainWnd;
	RECT rectMain;
		
	// Register the doc templates we provide to the app
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);

	// get the apps instance handle for loading resources
	m_AppInst = AfxGetResourceHandle();
	// now tell the application to look in dll for resources
	AfxSetResourceHandle(StateMachineEditorDLL.hModule);

	mainWnd->GetClientRect(&rectMain);

	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect winRect;
	mainWnd->GetWindowRect( &winRect );
	AfxSetResourceHandle(m_AppInst);
	
	return 0;
}