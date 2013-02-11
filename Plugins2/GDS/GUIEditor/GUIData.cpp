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
#include "GUIData.h"
#include ".\guidata.h"


// CGUIData dialog

IMPLEMENT_DYNAMIC(CGUIData, CDialog)
CGUIData::CGUIData(CWnd* pParent /*=NULL*/)
	: CDialog(CGUIData::IDD, pParent)
{
	m_Doc = NULL;
}

CGUIData::~CGUIData()
{
}

void CGUIData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESCOMBO, m_ResCombo);
}


BEGIN_MESSAGE_MAP(CGUIData, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_RESCOMBO, OnCbnSelchangeRescombo)
END_MESSAGE_MAP()


// CGUIData message handlers

void CGUIData::OnBnClickedOk()
{
	//OnOK();
}

void CGUIData::OnBnClickedCancel()
{
	//OnCancel();
}

void CGUIData::SetMousePosition(int x, int y)
{
	CWnd *xWnd = GetDlgItem(IDC_MOUSEX);
	assert(xWnd);
	
	CWnd *yWnd = GetDlgItem(IDC_MOUSEY);
	assert(yWnd);
	
	TCHAR buf[10];
	_stprintf(buf, "%d", x);
	xWnd->SetWindowText(buf);

	_stprintf(buf, "%d", y);
	yWnd->SetWindowText(buf);
}

void CGUIData::SetZoom(float z)
{
	CWnd *zWnd = GetDlgItem(IDC_ZOOM);
	assert(zWnd);

	z *= 100;
	TCHAR buf [10];
	_stprintf(buf, "%.1f%%", z);
	zWnd->SetWindowText(buf);
}

void CGUIData::AddResolution(UINT x, UINT y)
{
	TCHAR buf[15];
	_stprintf(buf, "%dx%d", x, y);
	for (UINT i=0; i<m_vResolutions.size(); i++)
	{
		// remove redundancy
		if (_tcscmp(buf, (const TCHAR*)m_vResolutions[i]) == 0)
		{
			return;		
		}
	}	
	m_ResCombo.AddString(buf);
	m_vResolutions.push_back(buf);
}

BOOL CGUIData::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(false);
	
	// check ini for last used resolution
	StdString rX, rY, fullRes;
	ReadINIKey(_T("LastUsedResolution"), fullRes, _T("GUIEditor"), _T("800x600"));

	fullRes.GetToken(_T("x"), rX);
	fullRes.GetToken(_T("x"), rY);
	UINT iX, iY;
	iX = (UINT)atoi(rX);
	iY = (UINT)atoi(rY);

	AddResolution(iX, iY);		

	// load resolutions from file
	CREATEARCHIVE ca;
	CHashString streamType = _T("File");
	StdString fullPath, resourceDir;
	EngineGetToolBox()->GetDirectories(&fullPath, &resourceDir);
	
	fullPath += _T("\\guiproperties\\GUISettings.xgp");
	
	ca.streamData = (void*)fullPath.c_str();
	ca.streamType = &streamType;
	ca.mode = STREAM_MODE_READ;
	ca.archive = NULL;
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca);
	if (retval = MSG_HANDLED)
	{
		IXMLArchive *Archive;
		if (!ca.archive)
		{
			return FALSE;
		}
		Archive = dynamic_cast<IXMLArchive*>(ca.archive);
		
		StdString name;
		int numRes, x, y;
		Archive->GetNode(name);
		Archive->Read(numRes);
		for (int i=0; i<numRes; i++)
		{
			Archive->GetNode(name);
			Archive->Read(x);
			Archive->Read(y);	
			
			AddResolution(x, y);			
		}
		Archive->Close();
	}
	m_ResCombo.SetCurSel(0);

	return TRUE;
}

void CGUIData::OnCbnSelchangeRescombo()
{
	if (m_Doc)
	{
		StdString res;
		CString curText;
		m_ResCombo.GetLBText(m_ResCombo.GetCurSel(), curText);
		res = curText;
		m_Doc->ResolutionUpdated(res);
	}
}

void CGUIData::SetResolution(UINT x, UINT y)
{
	AddResolution(x, y);
	TCHAR buf[15];
	_stprintf(buf, "%dx%d", x, y);
	m_ResCombo.SetCurSel(m_ResCombo.FindStringExact(0, buf));
}