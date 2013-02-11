// TreeCtrlEx.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlEx

CTreeCtrlEx::CTreeCtrlEx()
{
}

CTreeCtrlEx::~CTreeCtrlEx()
{
}

static UINT ObjectDropMessage = RegisterWindowMessage(OBJECT_DROP_MSG_STR);

BEGIN_MESSAGE_MAP(CTreeCtrlEx, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeCtrlEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(ObjectDropMessage, OnDropMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlEx message handlers

LRESULT CTreeCtrlEx::OnDropMessage(WPARAM wParam, LPARAM lParam)
{
	return GetParent()->SendMessage(ObjectDropMessage, wParam, lParam);
}

