// GUIAddElementBox.cpp : implementation file
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "GUIAddElementBox.h"
#include ".\guiaddelementbox.h"


// CGUIAddElementBox dialog

IMPLEMENT_DYNAMIC(CGUIAddElementBox, CDialog)
CGUIAddElementBox::CGUIAddElementBox(CWnd* pParent /*=NULL*/)
	: CDialog(CGUIAddElementBox::IDD, pParent)
	, m_szElementName(_T(""))
	, m_szElementType(_T(""))
{
	m_FrmParent = NULL;
	m_iItemNumber = 0;
}

CGUIAddElementBox::~CGUIAddElementBox()
{
}

void CGUIAddElementBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GUIELEMENTTYPE, m_ctElementType);
	DDX_Text(pDX, IDC_GUINAME, m_szElementName);
	DDX_CBString(pDX, IDC_GUIELEMENTTYPE, m_szElementType);
}


BEGIN_MESSAGE_MAP(CGUIAddElementBox, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CGUIAddElementBox message handlers

void CGUIAddElementBox::OnBnClickedOk()
{
	UpdateData(true);
	OnOK();
}

void CGUIAddElementBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	m_ctElementType.ResetContent();
	
	m_ctElementType.AddString(_T("Button"));
}
