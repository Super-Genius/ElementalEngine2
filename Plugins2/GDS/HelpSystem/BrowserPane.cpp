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
#include "HelpSystem.h"
#include "BrowserPane.h"
#include ".\browserpane.h"


// CBrowserPane dialog

const CString CBrowserPane::MSITSPREFIX1(_T("mk:@MSITStore:"));
const CString CBrowserPane::MSITSPREFIX2(_T("ms-its:"));

IMPLEMENT_DYNAMIC(CBrowserPane, CDialog)

CBrowserPane::CBrowserPane(CWnd* pParent /*=NULL*/)
:	CDialog(CBrowserPane::IDD, pParent), m_bInit(false)
{
}

CBrowserPane::~CBrowserPane()
{
}

void CBrowserPane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BROWSER, m_browser);
}


BEGIN_MESSAGE_MAP(CBrowserPane, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CBrowserPane, CDialog)
	ON_EVENT(CBrowserPane, IDC_BROWSER, 271, NavigateErrorBrowser, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()

// CBrowserPane message handlers


BOOL CBrowserPane::Create(CWnd *pParent)
{
	return CDialog::Create(CBrowserPane::IDD, pParent);
}

BOOL CBrowserPane::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bInit = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBrowserPane::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (m_browser.GetSafeHwnd())
	{
		m_browser.MoveWindow(0, 0, cx, cy);
	}
}

void CBrowserPane::OnOK()
{
	// Do nothing.
}

void CBrowserPane::OnCancel()
{
	// Do nothing.
}

void CBrowserPane::NavigateErrorBrowser(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Frame, VARIANT* StatusCode, BOOL* Cancel)
{
	// TODO: Add your message handler code here
}

bool CBrowserPane::DisplayCHM(const CString &chmFilename, const CString &path)
{
	if (!m_bInit)
		return false;
	CString url(MakeCHMURL(chmFilename, path));
	try
	{
		m_browser.Navigate(url, NULL, NULL, NULL, NULL);
		return true;
	}
	catch(COleException *e)
	{
		e->Delete();
		return false;
	}
	catch(COleDispatchException *e)
	{
		e->Delete();
		return false;
	}
}

CString CBrowserPane::MakeCHMURL(const CString &chmFilename, const CString &path)
{
	// 'mk:@MSITStore:' is preferred because it supported by earlier versions
	// of IE.
	CString url(MSITSPREFIX1);
	url += chmFilename;
	url += "::";
	url += path;
	return url;
}

bool CBrowserPane::GetCHM(CString &chmFilename, CString &path)
{
	CString url;
	if (!GetURL(url))
	{
		return false;
	}
	int prefixLen;
	if ((url.Left(MSITSPREFIX1.GetLength()) == MSITSPREFIX1))
	{
		prefixLen = MSITSPREFIX1.GetLength();
	}
	else if ((url.Left(MSITSPREFIX2.GetLength()) == MSITSPREFIX2))
	{
		prefixLen = MSITSPREFIX2.GetLength();
	}
	else
	{
		// Non-MSITStore URL.
		return false;
	}
	int chmFileEnd = url.Find("::", prefixLen);
	if (chmFileEnd < 0)
	{
		// Invalid URL.
		return false;
	}
	chmFilename = url.Mid(prefixLen, chmFileEnd - prefixLen);
	path = url.Mid(chmFileEnd + 2);
	return true;
}

bool CBrowserPane::DisplayResource(const CString &name)
{
	if (!m_bInit)
		return false;
	if (DLLModuleName.IsEmpty())
	{
		return false;
	}
	CString url = _T("res://") + DLLModuleName + _T("/") + name;
	try
	{
		m_browser.Navigate(url, NULL, NULL, NULL, NULL);
		return true;
	}
	catch(COleException *e)
	{
		e->Delete();
		return false;
	}
	catch(COleDispatchException *e)
	{
		e->Delete();
		return false;
	}
}

bool CBrowserPane::GetURL(CString &url)
{
	if (!m_bInit)
		return false;
	try
	{
		url = m_browser.get_LocationURL();
		return true;
	}
	catch(COleException *e)
	{
		e->Delete();
		return false;
	}
	catch(COleDispatchException *e)
	{
		e->Delete();
		return false;
	}
}

bool CBrowserPane::GoBack()
{
	try
	{
		m_browser.GoBack();
		return true;
	}
	catch(COleException *e)
	{
		e->Delete();
		return false;
	}
	catch(COleDispatchException *e)
	{
		e->Delete();
		return false;
	}
}

bool CBrowserPane::GoForward()
{
	try
	{
		m_browser.GoForward();
		return true;
	}
	catch(COleException *e)
	{
		e->Delete();
		return false;
	}
	catch(COleDispatchException *e)
	{
		e->Delete();
		return false;
	}
}

bool CBrowserPane::Stop()
{
	try
	{
		m_browser.Stop();
		return true;
	}
	catch(COleException *e)
	{
		e->Delete();
		return false;
	}
	catch(COleDispatchException *e)
	{
		e->Delete();
		return false;
	}
}

bool CBrowserPane::Refresh()
{
	try
	{
		m_browser.Refresh();
		return true;
	}
	catch(COleException *e)
	{
		e->Delete();
		return false;
	}
	catch(COleDispatchException *e)
	{
		e->Delete();
		return false;
	}
}
