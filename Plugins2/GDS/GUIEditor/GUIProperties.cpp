// GUIProperties.cpp : implementation file
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "GUIProperties.h"
#include ".\guiproperties.h"

const static StdString defaultObjectsFile = _T("DefaultObjects.xgd");
const static StdString formatFile = _T("GUIObjects.xgf");
const static StdString formatDir = _T("\\guiproperties\\");

// CGUIProperties dialog

IMPLEMENT_DYNAMIC(CGUIProperties, CDialog)
CGUIProperties::CGUIProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CGUIProperties::IDD, pParent)
{
}

CGUIProperties::~CGUIProperties()
{
}

void CGUIProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROPERTIES_SPOT, m_PropertiesSpot);
}


BEGIN_MESSAGE_MAP(CGUIProperties, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(OT_NOTIFY_ITEMCHANGED, IDC_OPTIONSTREE_OPTIONS, OnUpdateProperties)	
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CGUIProperties message handlers

void CGUIProperties::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
//	OnOK();
}

BOOL CGUIProperties::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_DockWin = new ObjectDockWin(&m_PropertiesSpot);
	return TRUE;
}

void CGUIProperties::OnUpdateProperties(NMHDR* pNotifyStruct, LRESULT* plResult)
{
	m_DockWin->OnUpdateProperties(pNotifyStruct, plResult);
}

void CGUIProperties::OnBnClickedButton1()
{
	MessageBox("Works");
}
