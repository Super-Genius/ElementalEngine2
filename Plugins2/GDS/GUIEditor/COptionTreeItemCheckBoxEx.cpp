///============================================================================
/// \file		COptionTreeItemCheckBoxEx.cpp
/// \brief		Implementation file for COptionTreeItemCheckBoxEx
/// \date		05-03-2005
/// \author		Brian Bazyk
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
#include "DLL.h"
#include "COptionTreeItemCheckBoxEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(COptionTreeItemCheckBoxEx, COptionTreeItemCheckBox)
	//{{AFX_MSG_MAP(COptionTreeItemCheckBoxEx)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void COptionTreeItemCheckBoxEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	BOOL bPrevCheck = GetCheck();
	COptionTreeItemCheckBox::OnLButtonUp(nFlags,point);
	if (bPrevCheck != GetCheck())
	{
		CommitChanges();
	}
}
