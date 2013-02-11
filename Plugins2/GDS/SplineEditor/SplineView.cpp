///============================================================================
/// \file		SplineComponent.cpp
/// \brief		Implementation file for Spline View
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
//#include "afxpriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplineView

IMPLEMENT_DYNCREATE(CSplineView, CView)

CSplineView::CSplineView()
{
}

CSplineView::~CSplineView()
{
}


BEGIN_MESSAGE_MAP(CSplineView, CView)
	//{{AFX_MSG_MAP(CSplineView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplineView drawing

void CSplineView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CSplineView diagnostics

#ifdef _DEBUG
void CSplineView::AssertValid() const
{
	CView::AssertValid();
}

void CSplineView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSplineView message handlers

void CSplineView::OnUpdateSplineTest1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}

void CSplineView::OnUpdateSplineTest2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}

void CSplineView::OnUpdateSplineTest3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}

void CSplineView::OnSplineTest1() 
{
	MessageBox(_T("Test Menu Item 1 was selected.\n"), _T("Spline"));
}

void CSplineView::OnSplineTest2() 
{
	MessageBox(_T("Test Menu Item 2 was selected.\n"), _T("Spline"));	
}

void CSplineView::OnSplineTest3() 
{
	MessageBox(_T("Test Menu Item 3 was selected.\n"), _T("Spline"));
}

void CSplineView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CWinApp* pApp;
	CMDIFrameWnd *mainWnd;
	CSplineFrm *parent;

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);

	parent = DYNAMIC_DOWNCAST(CSplineFrm, this->GetParentFrame());

	AfxSetResourceHandle(SplineEditorDLL.hModule);
	// don't use is window visible.
	if (parent->m_SplineToolPal.IsVisible() != bActivate)
	{
		mainWnd->ShowControlBar(&parent->m_SplineToolPal, bActivate, TRUE);
	}
	
	AfxSetResourceHandle(parent->m_AppInst);
}
