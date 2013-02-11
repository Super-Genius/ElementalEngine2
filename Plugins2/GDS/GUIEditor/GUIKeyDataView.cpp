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
#include "GUIEditor.h"
#include "GUIKeyDataView.h"
#include ".\guikeydataview.h"


// CGUIKeyDataView dialog

IMPLEMENT_DYNAMIC(CGUIKeyDataView, CDialog)
CGUIKeyDataView::CGUIKeyDataView(CWnd* pParent /*=NULL*/)
	: CDialog(CGUIKeyDataView::IDD, pParent)
	, m_szObjName(_T(""))
	, m_iKeyNum(0)
	, m_szAttr1(_T(""))
	, m_szAttr2(_T(""))
	, m_szAttr3(_T(""))
	, m_szAttr4(_T(""))
{
	m_Doc = NULL;
	m_Mode = NONE;
}

CGUIKeyDataView::~CGUIKeyDataView()
{
}

void CGUIKeyDataView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_KEYVIEWOBJNAME, m_szObjName);
	DDX_Text(pDX, IDC_KEYVIEWKEYFRAME, m_iKeyNum);
	DDX_Text(pDX, IDC_KEYVIEWATTR1, m_szAttr1);
	DDX_Text(pDX, IDC_KEYVIEWATTR2, m_szAttr2);
	DDX_Text(pDX, IDC_KEYVIEWATTR3, m_szAttr3);
	DDX_Text(pDX, IDC_KEYVIEWATTR4, m_szAttr4);
	DDX_Control(pDX, IDC_EDIT1, m_cAttr1Edit);
	DDX_Control(pDX, IDC_EDIT2, m_cAttr2Edit);
	DDX_Control(pDX, IDC_EDIT3, m_cAttr3Edit);
	DDX_Control(pDX, IDC_EDIT4, m_cAttr4Edit);
}


BEGIN_MESSAGE_MAP(CGUIKeyDataView, CDialog)
	ON_BN_CLICKED(IDC_KEYVIEWUPDATEBT, OnBnClickedKeyviewupdatebt)
END_MESSAGE_MAP()


// CGUIKeyDataView message handlers

void CGUIKeyDataView::SetPosData(StdString objName, UINT keyNum, float xPos, float yPos)
{
	m_szObjName = (const TCHAR*)objName;
	m_iKeyNum = keyNum;
	m_szAttr1 = _T("X Pos");
	m_szAttr2 = _T("Y Pos");
	m_szAttr3 = _T("");
	m_szAttr4 = _T("");
	UpdateData(false);

	TCHAR buf[20];
	_stprintf(buf, "%d", (int)xPos);
	m_cAttr1Edit.SetWindowText(buf);
	_stprintf(buf, "%d", (int)yPos);
	m_cAttr2Edit.SetWindowText(buf);
	
	m_cAttr1Edit.ShowWindow(SW_SHOW);
	m_cAttr2Edit.ShowWindow(SW_SHOW);
	m_cAttr3Edit.ShowWindow(SW_HIDE);
	m_cAttr4Edit.ShowWindow(SW_HIDE);
	m_Mode = POSKEY;
}

void CGUIKeyDataView::SetSizeData(StdString objName, UINT keyNum, float width, float height)
{
	m_szObjName = (const TCHAR*)objName;
	m_iKeyNum = keyNum;
	m_szAttr1 = _T("Width");
	m_szAttr2 = _T("Height");
	m_szAttr3 = _T("");
	m_szAttr4 = _T("");
	UpdateData(false);

	TCHAR buf[20];
	_stprintf(buf, "%d", (int)width);
	m_cAttr1Edit.SetWindowText(buf);
	_stprintf(buf, "%d", (int)height);
	m_cAttr2Edit.SetWindowText(buf);
	
	m_cAttr1Edit.ShowWindow(SW_SHOW);
	m_cAttr2Edit.ShowWindow(SW_SHOW);
	m_cAttr3Edit.ShowWindow(SW_HIDE);
	m_cAttr4Edit.ShowWindow(SW_HIDE);
	m_Mode = SIZEKEY;
}

void CGUIKeyDataView::SetColorData(StdString objName, UINT keyNum, float a, float r, float g, float b)
{
	m_szObjName = (const TCHAR*)objName;
	m_iKeyNum = keyNum;
	m_szAttr1 = _T("Alpha");
	m_szAttr2 = _T("R Val");
	m_szAttr3 = _T("G Val");
	m_szAttr4 = _T("B Val");
	UpdateData(false);

	TCHAR buf[20];
	_stprintf(buf, "%d", (int)a);
	m_cAttr1Edit.SetWindowText(buf);
	_stprintf(buf, "%d", (int)r);
	m_cAttr2Edit.SetWindowText(buf);
	_stprintf(buf, "%d", (int)g);
	m_cAttr3Edit.SetWindowText(buf);
	_stprintf(buf, "%d", (int)b);
	m_cAttr4Edit.SetWindowText(buf);		
	m_Mode = COLORKEY;

	m_cAttr1Edit.ShowWindow(SW_SHOW);
	m_cAttr2Edit.ShowWindow(SW_SHOW);
	m_cAttr3Edit.ShowWindow(SW_SHOW);
	m_cAttr4Edit.ShowWindow(SW_SHOW);
}

void CGUIKeyDataView::SetRotationData(StdString objName, UINT keyNum, float rot)
{
	m_szObjName = (const TCHAR*)objName;
	m_iKeyNum = keyNum;
	m_szAttr1 = _T("Angle");
	m_szAttr2 = _T("");
	m_szAttr3 = _T("");
	m_szAttr4 = _T("");
	UpdateData(false);

	TCHAR buf[20];
	_stprintf(buf, "%d", (int)rot);
	m_cAttr1Edit.SetWindowText(buf);		
	m_Mode = ROTKEY;

	m_cAttr1Edit.ShowWindow(SW_SHOW);
	m_cAttr2Edit.ShowWindow(SW_HIDE);
	m_cAttr3Edit.ShowWindow(SW_HIDE);
	m_cAttr4Edit.ShowWindow(SW_HIDE);
}

void CGUIKeyDataView::OnBnClickedKeyviewupdatebt()
{
	if (m_Doc)
	{
		UINT keyType;
		float v1=0.0f, v2=0.0f, v3=0.0f, v4=0.0f;
		if (m_Mode == POSKEY)
		{
			CString szVal;
			m_cAttr1Edit.GetWindowText(szVal);
			v1 = (float)_tstof(szVal);
			m_cAttr2Edit.GetWindowText(szVal);
			v2 = (float)_tstof(szVal);
			keyType = 0;
		}
		else if (m_Mode == SIZEKEY)
		{
			CString szVal;
			m_cAttr1Edit.GetWindowText(szVal);
			v1 = (float)_tstof(szVal);
			m_cAttr2Edit.GetWindowText(szVal);
			v2 = (float)_tstof(szVal);
			keyType = 1;
		}
		else if (m_Mode == COLORKEY)
		{
			CString szVal;
			m_cAttr1Edit.GetWindowText(szVal);
			v1 = (float)_tstof(szVal);
			m_cAttr2Edit.GetWindowText(szVal);
			v2 = (float)_tstof(szVal);
			m_cAttr3Edit.GetWindowText(szVal);
			v3 = (float)_tstof(szVal);
			m_cAttr4Edit.GetWindowText(szVal);
			v4 = (float)_tstof(szVal);
			keyType = 2;
		}
		if (m_Mode == ROTKEY)
		{
			CString szVal;
			m_cAttr1Edit.GetWindowText(szVal);
			v1 = (float)_tstof(szVal);			
			keyType = 3;
		}
		m_Doc->UpdateKeyData(m_iKeyNum, (const TCHAR*)m_szObjName, keyType, v1, v2, v3, v4);
	}
	OnOK();
}
