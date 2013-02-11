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
//#include "afxpriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpawnView

IMPLEMENT_DYNCREATE(CSpawnView, CView)

CSpawnView::CSpawnView()
{
}

CSpawnView::~CSpawnView()
{
}


BEGIN_MESSAGE_MAP(CSpawnView, CView)
	//{{AFX_MSG_MAP(CSpawnView)
	ON_UPDATE_COMMAND_UI(ID_SPAWN_TEST1, OnUpdateSpawnTest1)
	ON_UPDATE_COMMAND_UI(ID_SPAWN_TEST2, OnUpdateSpawnTest2)
	ON_UPDATE_COMMAND_UI(ID_SPAWN_TEST3, OnUpdateSpawnTest3)
	ON_COMMAND(ID_SPAWN_TEST1, OnSpawnTest1)
	ON_COMMAND(ID_SPAWN_TEST2, OnSpawnTest2)
	ON_COMMAND(ID_SPAWN_TEST3, OnSpawnTest3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpawnView drawing

void CSpawnView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CSpawnView diagnostics

#ifdef _DEBUG
void CSpawnView::AssertValid() const
{
	CView::AssertValid();
}

void CSpawnView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSpawnView message handlers

void CSpawnView::OnUpdateSpawnTest1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}

void CSpawnView::OnUpdateSpawnTest2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}

void CSpawnView::OnUpdateSpawnTest3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}

void CSpawnView::OnSpawnTest1() 
{
	MessageBox(_T("Test Menu Item 1 was selected.\n"), _T("Spawn"));
}

void CSpawnView::OnSpawnTest2() 
{
	MessageBox(_T("Test Menu Item 2 was selected.\n"), _T("Spawn"));	
}

void CSpawnView::OnSpawnTest3() 
{
	MessageBox(_T("Test Menu Item 3 was selected.\n"), _T("Spawn"));
}

void CSpawnView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CWinApp* pApp;
	CMDIFrameWnd *mainWnd;
	CSpawnFrm *parent;

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);

	parent = DYNAMIC_DOWNCAST(CSpawnFrm, this->GetParentFrame());

	AfxSetResourceHandle(SpawnEditorDLL.hModule);
	// don't use is window visible.
	if (parent->m_SpawnToolPal.IsVisible() != bActivate)
	{
		mainWnd->ShowControlBar(&parent->m_SpawnToolPal, bActivate, TRUE);
	}
	
	AfxSetResourceHandle(parent->m_AppInst);
}
