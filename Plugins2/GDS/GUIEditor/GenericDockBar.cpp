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
#include ".\genericdockbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CGenericDockBar, CGuiControlBar)

BEGIN_MESSAGE_MAP(CGenericDockBar, CGuiControlBar)
	ON_WM_CREATE()
	ON_COMMAND(ID_SHOW_HIDE, OnShowHide)
END_MESSAGE_MAP()

CGenericDockBar::CGenericDockBar(CDialog *innerWnd, UINT id, CHashString title)
{
	m_pInnerWnd = innerWnd;
	m_szTitle = title;
	m_iID = id;
}

CGenericDockBar::~CGenericDockBar(void)
{
}

int CGenericDockBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CRect rect;

	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_pInnerWnd->Create(m_iID, this))
	{
		return -1;
	}

	ModifyStyle(0, WS_CHILD);
	m_pInnerWnd->ShowWindow(SW_SHOW);
	SetWindowText(m_szTitle.GetString());
    
	return 0;
}

BOOL CGenericDockBar::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
    return false;
}


void CGenericDockBar::OnShowHide()
{
	printf("test");
}

void CGenericDockBar::OnSize(UINT nType, int cx, int cy)
{
	if (m_pInnerWnd)
	{
		m_pInnerWnd->SetWindowPos(NULL, 0, 0, cx, cy, 0);
		m_pInnerWnd->Invalidate();
	}
}