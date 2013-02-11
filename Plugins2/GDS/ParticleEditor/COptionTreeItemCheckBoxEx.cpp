///============================================================================
/// \file		COptionTreeItemCheckBoxEx.cpp
/// \brief		Implementation file for COptionTreeItemCheckBoxEx
/// \date		05-03-2005
/// \author		Brian Bazyk
/// \note		Copyright (c) 2004 Signature Devices, Inc.
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
