///============================================================================
/// \file		ReturnEdit.cpp
/// \brief		Edit control that loses focus when enter is pressed
/// \date		06/11/2007
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

BEGIN_MESSAGE_MAP(CReturnEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()

void CReturnEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RETURN)
	{
		AfxGetMainWnd()->SetFocus();
	}
	else
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}