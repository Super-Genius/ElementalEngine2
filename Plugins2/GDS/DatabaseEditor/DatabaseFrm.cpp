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

#define NUMICONS 4
#define NICONSWIDE 4

/////////////////////////////////////////////////////////////////////////////
// CDatabaseFrm

IMPLEMENT_DYNCREATE(CDatabaseFrm, CMDIChildWnd)

CDatabaseFrm::CDatabaseFrm()
{
}

CDatabaseFrm::~CDatabaseFrm()
{
}


BEGIN_MESSAGE_MAP(CDatabaseFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CDatabaseFrm)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDatabaseFrm message handlers

int CDatabaseFrm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
	AfxSetResourceHandle(DatabaseEditorDLL.hModule);

	mainWnd->GetClientRect(&rectMain);

	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	AfxSetResourceHandle(m_AppInst);
	
	return 0;
}
