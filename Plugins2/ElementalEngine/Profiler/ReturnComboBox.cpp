///============================================================================
/// \file		ReturnComboBox.cpp
/// \brief		Combo box wrapper that wraps a return edit
/// \date		07/10/2007
/// \author		Nick Tourte
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

BEGIN_MESSAGE_MAP(CReturnComboBox, CComboBox)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

HBRUSH CReturnComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_EDIT)
	{
		if (m_EditCtrl.GetSafeHwnd() == NULL)
		{
			m_EditCtrl.SubclassWindow(pWnd->GetSafeHwnd());
		}
	}
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void CReturnComboBox::OnDestroy()
{
	if (m_EditCtrl.GetSafeHwnd() != NULL)
	{
		m_EditCtrl.UnsubclassWindow();
	}
}