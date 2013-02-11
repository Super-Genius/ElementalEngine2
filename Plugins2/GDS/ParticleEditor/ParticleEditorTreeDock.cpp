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
#include "ParticleEditorTreeDock.h"

IMPLEMENT_DYNCREATE(CParticleEditorTreeDock, CGuiControlBar)

BEGIN_MESSAGE_MAP(CParticleEditorTreeDock, CGuiControlBar)
	//{{AFX_MSG_MAP(ParticleEditorTreeDock)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CParticleEditorTreeDock::CParticleEditorTreeDock()
{
	m_PropSheet.AddPage(&m_ParticleTreePropertyPage);
	m_ToolBox = EngineGetToolBox();
}

CParticleEditorTreeDock::~CParticleEditorTreeDock()
{
}

int CParticleEditorTreeDock::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CTabCtrl* tab;
	TCITEM tItem;
	CRect rect;

	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText(_T("Particle Tree Dock"));

	// create the property sheet
	if (!m_PropSheet.Create(this, WS_CHILD | WS_VISIBLE, 0))
	{
		m_ToolBox->Log(LOGERROR, _T("Failed to create CParticleEditorTreeDock::m_PropSheet \n"));
		return -1;
	}
	
	tab = m_PropSheet.GetTabControl();

	// Put the tabs at the bottom
	const DWORD dwRemove = WS_BORDER | WS_THICKFRAME | WS_DLGFRAME | WS_CAPTION | TCS_MULTILINE;
	const DWORD dwAdd = TCS_BOTTOM | TCS_FIXEDWIDTH | WS_CHILD;
	tab->ModifyStyle(dwRemove, dwAdd);

	// now set the tab strings/icons
	tItem.pszText = _T("Particle Tree");
	tItem.mask = TCIF_TEXT;
	tab->SetItem(0, &tItem);

	SetBorders(4, 4, 6, 4);

	StyleDispl(GUISTYLE_XP);

	return 0;
}

void CParticleEditorTreeDock::OnSize(UINT nType, int cx, int cy) 
{
	CTabCtrl* tab;
	CRect rect;

	CGuiControlBar::OnSize(nType, cx, cy);

	GetClientRect(rect);

	m_PropSheet.MoveWindow(rect);
	m_PropSheet.GetClientRect(rect);
	tab = m_PropSheet.GetTabControl();
	tab->MoveWindow(rect);

	// adjust PopertyPage inside of CTabCtrl
	tab->AdjustRect(FALSE, rect);

	CPropertyPage *pPage = m_PropSheet.GetActivePage();
	if (pPage)
	{
		pPage->MoveWindow(rect);
	}
}
