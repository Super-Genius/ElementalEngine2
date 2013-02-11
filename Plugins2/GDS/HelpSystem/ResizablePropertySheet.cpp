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
#include "HelpSystem.h"
#include "ResizablePropertySheet.h"
#include ".\resizablepropertysheet.h"


// CResizablePropertySheet

IMPLEMENT_DYNAMIC(CResizablePropertySheet, CPropertySheet)

CResizablePropertySheet::CResizablePropertySheet()
{
}

CResizablePropertySheet::CResizablePropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CResizablePropertySheet::CResizablePropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CResizablePropertySheet::~CResizablePropertySheet()
{
}


BEGIN_MESSAGE_MAP(CResizablePropertySheet, CPropertySheet)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CResizablePropertySheet message handlers

void CResizablePropertySheet::OnSize(UINT nType, int cx, int cy)
{
	CPropertySheet::OnSize(nType, cx, cy);

	CTabCtrl *pTab = GetTabControl();
	if (pTab->GetSafeHwnd())
	{
		RECT trect;
		GetClientRect(&trect);
		pTab->MoveWindow(&trect, true);
		pTab->AdjustRect(FALSE, &trect);
		if (GetActivePage()->GetSafeHwnd())
		{
			GetActivePage()->MoveWindow(&trect, TRUE);
		}
	}
}
