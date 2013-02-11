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
// CExampleView

IMPLEMENT_DYNCREATE(CExampleView, CView)

CExampleView::CExampleView()
	: m_bIsActive(false)
{
}

CExampleView::~CExampleView()
{
}


BEGIN_MESSAGE_MAP(CExampleView, CView)
	//{{AFX_MSG_MAP(CExampleView)
	ON_UPDATE_COMMAND_UI(ID_EXAMPLE_TEST1, OnUpdateExampleTest1)
	ON_UPDATE_COMMAND_UI(ID_EXAMPLE_TEST2, OnUpdateExampleTest2)
	ON_UPDATE_COMMAND_UI(ID_EXAMPLE_TEST3, OnUpdateExampleTest3)
	ON_COMMAND(ID_EXAMPLE_TEST1, OnExampleTest1)
	ON_COMMAND(ID_EXAMPLE_TEST2, OnExampleTest2)
	ON_COMMAND(ID_EXAMPLE_TEST3, OnExampleTest3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExampleView drawing

void CExampleView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CExampleView diagnostics

#ifdef _DEBUG
void CExampleView::AssertValid() const
{
	CView::AssertValid();
}

void CExampleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CExampleView message handlers

void CExampleView::OnUpdateExampleTest1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}

void CExampleView::OnUpdateExampleTest2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}

void CExampleView::OnUpdateExampleTest3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}

void CExampleView::OnExampleTest1() 
{
	MessageBox(_T("Test Menu Item 1 was selected.\n"), _T("Example"));
}

void CExampleView::OnExampleTest2() 
{
	MessageBox(_T("Test Menu Item 2 was selected.\n"), _T("Example"));	
}

void CExampleView::OnExampleTest3() 
{
	MessageBox(_T("Test Menu Item 3 was selected.\n"), _T("Example"));
}

void CExampleView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CExampleFrm *parent = DYNAMIC_DOWNCAST(CExampleFrm, GetParentFrame());
	CGuiMDIFrame *mainWnd = DYNAMIC_DOWNCAST(CGuiMDIFrame, AfxGetMainWnd());

	AfxSetResourceHandle(ExampleEditorDLL.hModule);
	// don't use is window visible.
	mainWnd->ShowControlBar(&parent->m_ExampleToolPal, TRUE, TRUE);
}

void CExampleView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CExampleFrm *parent = DYNAMIC_DOWNCAST(CExampleFrm, GetParentFrame());
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	AfxSetResourceHandle(ExampleEditorDLL.hModule);

	if (m_bIsActive != (bActivate != FALSE))
	{
		m_bIsActive = (bActivate != FALSE);
		CGuiMDIFrame *mainWnd = DYNAMIC_DOWNCAST(CGuiMDIFrame, AfxGetMainWnd());
		if (bActivate)
		{
			mainWnd->LoadDocumentBars(_T("ExampleEditor"));
		}
		else
		{
			mainWnd->SaveDocumentBars(_T("ExampleEditor"));
		}
	}

	AfxSetResourceHandle(parent->m_AppInst);
}
