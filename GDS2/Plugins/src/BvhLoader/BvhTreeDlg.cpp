// BvhTreeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BvhLoader.h"
#include "BvhTreeDlg.h"


// CBvhTreeDlg

IMPLEMENT_DYNAMIC(CBvhTreeDlg, CDialogBar)
CBvhTreeDlg::CBvhTreeDlg()
{
}

CBvhTreeDlg::~CBvhTreeDlg()
{
}


BEGIN_MESSAGE_MAP(CBvhTreeDlg, CDialogBar)
END_MESSAGE_MAP()



// CBvhTreeDlg message handlers


BOOL CBvhTreeDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
}

BOOL CBvhTreeDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogBar::OnCommand(wParam, lParam);
}
