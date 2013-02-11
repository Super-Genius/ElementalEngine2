// TerrainView.cpp : implementation file
//

#include "stdafx.h"
#include "TerrainEditor.h"
#include "TerrainView.h"
#include ".\terrainview.h"


// CTerrainView

IMPLEMENT_DYNCREATE(CTerrainView, CView)

CTerrainView::CTerrainView()
{
}

CTerrainView::~CTerrainView()
{
}

BEGIN_MESSAGE_MAP(CTerrainView, CView)
END_MESSAGE_MAP()


// CTerrainView drawing

void CTerrainView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CTerrainView diagnostics

#ifdef _DEBUG
void CTerrainView::AssertValid() const
{
	CView::AssertValid();
}

void CTerrainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CTerrainView message handlers

void CTerrainView::OnBnClickedRaise()
{
	// TODO: Add your control notification handler code here
}

void CTerrainView::OnBnClickedLower()
{
	// TODO: Add your control notification handler code here
}

void CTerrainView::OnEnChangeSizeEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CTerrainView::OnEnHscrollSizeEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CView::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_SCROLL flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here
}

void CTerrainView::OnEnVscrollSizeEdit()
{
	// TODO: Add your control notification handler code here
}

void CTerrainView::OnEnChangeStrengthEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CTerrainView::OnEnHscrollStrengthEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CView::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_SCROLL flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here
}

void CTerrainView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	CMDIFrameWnd *mainWnd;
		
	// Register the doc templates we provide to the app
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);

	// get the apps instance handle for loading resources
	HINSTANCE appInst = AfxGetResourceHandle();
	// don't use is window visible.
	if (m_TerrainToolPal.IsVisible() != bActivate)
	{
		mainWnd->ShowControlBar(&m_TerrainToolPal, bActivate, TRUE);
		if (bActivate)
		{
			AfxSetResourceHandle(TerrainEditorDLL.hModule);
		}
		else
		{
			AfxSetResourceHandle(appInst);
		}
	}
}

void CTerrainView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	HINSTANCE appInst;

	CMDIFrameWnd *mainWnd;
		
	// Register the doc templates we provide to the app
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	mainWnd = DYNAMIC_DOWNCAST(CMDIFrameWnd, pApp->m_pMainWnd);

	// get the apps instance handle for loading resources
	appInst = AfxGetResourceHandle();

	// now tell the application to look in dll for resources
	AfxSetResourceHandle(TerrainEditorDLL.hModule);

	if (!m_TerrainToolPal.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP, 0))
	{
		TRACE0("Failed to create dockbar\n");
		return;      // fail to create
	}

//	m_TerrainToolPal
	m_TerrainToolPal.EnableDocking(CBRS_ORIENT_ANY);
	
	mainWnd->DockControlBar(&m_TerrainToolPal);

//	CRect tcRect;
//	m_TerrainToolPal.GetToolBarCtrl().SetRows(1, FALSE, tcRect);

	mainWnd->ShowControlBar(&m_TerrainToolPal, FALSE, FALSE);
	
	AfxSetResourceHandle(appInst);
}
