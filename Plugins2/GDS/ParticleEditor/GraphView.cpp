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
#include "ParticleEditor.h"
#include "GraphView.h"


// CGraphView

IMPLEMENT_DYNCREATE(CGraphView, CGuiView)

CGraphView::CGraphView()
	: m_bActive(false)
{
}

CGraphView::~CGraphView()
{
}

BEGIN_MESSAGE_MAP(CGraphView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CGraphView drawing

void CGraphView::OnDraw(CDC* pDC)
{
	RECT r;
	GetClientRect(&r);
	pDC->MoveTo(0,0);
	pDC->LineTo(r.right, r.bottom);
	pDC->MoveTo(r.left, r.bottom);
	pDC->LineTo(r.right, r.top);
}

void CGraphView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	RECT r;
	GetClientRect(&r);
	m_Graph.MoveWindow(&r, TRUE); 
}

int CGraphView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CView::OnCreate(lpcs) == -1)
		return -1;

	RECT r;

	GetClientRect(&r);
	m_Graph.Create(r, this, ID_VIEW_GRAPH);
	return 0;
}

void CGraphView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (m_bActive != (bActivate != FALSE))
	{
		m_bActive = (bActivate != FALSE);

		CGuiMDIFrame *mainWnd = DYNAMIC_DOWNCAST(CGuiMDIFrame, AfxGetMainWnd());

		if (bActivate)
		{
			mainWnd->LoadDocumentBars(_T("ParticleEditor"));
			SINGLETONINSTANCE(CParticleEditor)->RefreshList(GetDocument());
			GetDocument()->SetActiveScene();
		}
		else
		{
			mainWnd->SaveDocumentBars(_T("ParticleEditor"));
			SINGLETONINSTANCE(CParticleEditor)->RefreshList(NULL);
		}
	}
}

void CGraphView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if ( (unsigned int)lHint & PARTICLE_UPDATE_GRAPHVIEW )
	{
		CParticleDoc * doc = GetDocument();
		if (doc)
			m_Graph.SetData(doc->GetKeyData());
	}
}

void CGraphView::ApplyChanges(float lasteffectedkeytime)
{
	CParticleDoc * doc = GetDocument();
	if (doc)
		doc->UpdateParticle(lasteffectedkeytime);	
}

void CGraphView::GraphUpdateComplete(float lasteffectedkeytime)
{
	CParticleDoc * doc = GetDocument();
//	if (doc)
//		doc->EditorUpdateOptionTree();
}
