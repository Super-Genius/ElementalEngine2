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

#include "StdAfx.h"
#include "GraphCtrlDockBar.h"

IMPLEMENT_DYNCREATE(CGraphCtrlDockBar, CGuiControlBar)

BEGIN_MESSAGE_MAP(CGraphCtrlDockBar, CGuiControlBar)
	//{{AFX_MSG_MAP(ParticleEditorTreeDock)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CGraphCtrlDockBar::CGraphCtrlDockBar()
{
//	m_PropSheet.AddPage(&m_ParticleTreePropertyPage);
	m_ToolBox = EngineGetToolBox();
}

CGraphCtrlDockBar::~CGraphCtrlDockBar()
{
}

int CGraphCtrlDockBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText(_T("Particle Graph Dock"));

	RECT r;
	GetClientRect(&r);

	m_Graph.Create(r, this, ID_VIEW_GRAPH);
	SetBorders(4, 4, 6, 4);

	StyleDispl(GUISTYLE_XP);

	return 0;
}
void CGraphCtrlDockBar::UpdateGraph(CParticleDoc * pDoc)
{
	if (pDoc)
		m_Graph.SetData(pDoc->GetKeyData());
}

void CGraphCtrlDockBar::ApplyChanges(float lasteffectedkeytime)
{
	CParticleDoc * doc = CParticleDoc::GetActiveDocument();
	if (doc)
		doc->UpdateParticle(lasteffectedkeytime);
}
/*
void CGraphCtrlDockBar::ApplyChangesAndRestart()
{
	CParticleDoc * doc = CParticleDoc::GetActiveDocument();
	if (doc)
	{
		doc->UpdateParticle(lasteffectedkeytime);
		doc->RestartParticle();
	}
}
*/
void CGraphCtrlDockBar::OnSize(UINT nType, int cx, int cy) 
{
	CGuiControlBar::OnSize(nType, cx, cy);
	RECT r;
	GetClientRect(&r);
	m_Graph.MoveWindow(&r, TRUE); 
}
