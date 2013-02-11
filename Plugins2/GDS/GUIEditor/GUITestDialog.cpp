// GUITestDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "GUITestDialog.h"

const static StdString defaultObjectsFile = _T("DefaultObjects.xgd");
const static StdString formatFile = _T("GUIObjects.xgf");
const static StdString formatDir = _T("\\guiproperties\\");

// CGUITestDialog dialog

IMPLEMENT_DYNAMIC(CGUITestDialog, CDialog)
CGUITestDialog::CGUITestDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGUITestDialog::IDD, pParent)
{
}

CGUITestDialog::~CGUITestDialog()
{
}

void CGUITestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROPERTIES, m_TestProperties);
}


BEGIN_MESSAGE_MAP(CGUITestDialog, CDialog)
END_MESSAGE_MAP()


// CGUITestDialog message handlers

BOOL CGUITestDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_DockWin = new ObjectDockWin(&m_TestProperties);		

	m_DockWin->CreateNewOptionTree(&CHashString(_T("Test Button")), &CHashString(_T("CGUIItem")));
	return TRUE;
}
