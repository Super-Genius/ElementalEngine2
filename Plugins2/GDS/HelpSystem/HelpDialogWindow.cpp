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
#include "HelpDialogWindow.h"
#include ".\helpdialogwindow.h"

// CHelpDialogWindow

IMPLEMENT_DYNAMIC(CHelpDialogWindow, CWnd)

CHelpDialogWindow::CHelpDialogWindow(CHelpSystemCore * helpCore)
:	m_helpCore(helpCore), m_bIsExists(false),
	m_contents(helpCore), m_index(helpCore), m_search(helpCore)
{
	m_contents.SetHelpDialog(this);
	m_index.SetHelpDialog(this);
	m_search.SetHelpDialog(this);
}

CHelpDialogWindow::~CHelpDialogWindow()
{
}


BEGIN_MESSAGE_MAP(CHelpDialogWindow, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CHelpDialogWindow message handlers


int CHelpDialogWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	struct ResourceHandleGuard
	{
		HINSTANCE hPrevModule;
		ResourceHandleGuard(HINSTANCE hModule)
		{
			hPrevModule = AfxGetResourceHandle();
			AfxSetResourceHandle(hModule);
		}
		~ResourceHandleGuard()
		{
			AfxSetResourceHandle(hPrevModule);
		}
	} guard(HelpSystemDLL.hModule);

	if (!m_toolBarImages.Create(IDB_TOOLBAR, 20, 0, 0xFF00FF))
	{
		return -1;
	}
	if (!m_toolBar.CreateEx(
		0, WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_WRAPABLE,
		CRect(), this, IDB_TOOLBAR
	))
	{
		return -1;
	}
	m_toolBar.SetImageList(&m_toolBarImages);
	TBBUTTON tbbuttons[] =
	{
		{BMN_SHOW, ID_SHOW, TBSTATE_HIDDEN, TBSTYLE_BUTTON | BTNS_SHOWTEXT, {}, 0, m_toolBar.AddString(ID_SHOW)},
		{BMN_HIDE, ID_HIDE, TBSTATE_ENABLED, TBSTYLE_BUTTON | BTNS_SHOWTEXT, {}, 0, m_toolBar.AddString(ID_HIDE)},
		{BMN_SYNC, ID_SYNC, TBSTATE_ENABLED, TBSTYLE_BUTTON | BTNS_SHOWTEXT, {}, 0, m_toolBar.AddString(ID_SYNC)},
		{BMN_BROWSE_PREVIOUS, ID_BROWSE_PREVIOUS, TBSTATE_ENABLED, TBSTYLE_BUTTON | BTNS_SHOWTEXT, {}, 0, m_toolBar.AddString(ID_BROWSE_PREVIOUS)},
		{BMN_BROWSE_NEXT, ID_BROWSE_NEXT, TBSTATE_ENABLED, TBSTYLE_BUTTON | BTNS_SHOWTEXT, {}, 0, m_toolBar.AddString(ID_BROWSE_NEXT)},
		{BMN_BROWSE_BACK, ID_BROWSE_BACK, TBSTATE_ENABLED, TBSTYLE_BUTTON | BTNS_SHOWTEXT, {}, 0, m_toolBar.AddString(ID_BROWSE_BACK)},
		{BMN_BROWSE_FORWARD, ID_BROWSE_FORWARD, TBSTATE_ENABLED, TBSTYLE_BUTTON | BTNS_SHOWTEXT, {}, 0, m_toolBar.AddString(ID_BROWSE_FORWARD)},
		{BMN_BROWSE_STOP, ID_BROWSE_STOP, TBSTATE_ENABLED, TBSTYLE_BUTTON | BTNS_SHOWTEXT, {}, 0, m_toolBar.AddString(ID_BROWSE_STOP)},
		{BMN_BROWSE_REFRESH, ID_BROWSE_REFRESH, TBSTATE_ENABLED, TBSTYLE_BUTTON | BTNS_SHOWTEXT, {}, 0, m_toolBar.AddString(ID_BROWSE_REFRESH)},
		{BMN_BROWSE_HOME, ID_BROWSE_HOME, TBSTATE_ENABLED, TBSTYLE_BUTTON | BTNS_SHOWTEXT, {}, 0, m_toolBar.AddString(ID_BROWSE_HOME)},
//		{16, ID_BROWSE_FONT, TBSTATE_ENABLED, TBSTYLE_BUTTON | BTNS_SHOWTEXT, {}, 0, m_toolBar.AddString(ID_BROWSE_FONT)},
//		{7,  ID_BROWSE_PRINT, TBSTATE_ENABLED, TBSTYLE_BUTTON | BTNS_SHOWTEXT, {}, 0, m_toolBar.AddString(ID_BROWSE_PRINT)}
	};
	if (!m_toolBar.AddButtons(sizeof(tbbuttons) / sizeof(TBBUTTON), tbbuttons))
	{
		return -1;
	}
	m_toolBar.AutoSize();

	m_leftSheet.AddPage(&m_contents);
	m_leftSheet.AddPage(&m_index);
	m_leftSheet.AddPage(&m_search);
	if (!m_leftSheet.Create(this, WS_CHILD | WS_VISIBLE))
	{
		return -1;
	}

	if (!m_browserPane.Create(this))
	{
		return -1;
	}

	m_sizeBar.Create(this);
	m_sizeBar.SetBounds(192, 100);
	m_sizeBar.SetPosition(200);
	m_sizeBar.SetControlled(&m_leftSheet, &m_browserPane);

	m_bIsExists = true;

	return 0;
}

// Comment
BOOL CHelpDialogWindow::CreateHelpDialogWindow()
{
	LPCTSTR lpszClassName = AfxRegisterWndClass(
		0, ::LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_3DFACE + 1), 0
	);
	return CreateEx(
		0, lpszClassName, _T("Help"), WS_OVERLAPPEDWINDOW,
		200, 200, 800, 600, 0, 0
	);
}

void CHelpDialogWindow::PopupWindow()
{
	if (!IsWindowVisible())
	{
		ShowWindow(SW_SHOWNORMAL);
	}
	else if (IsIconic())
	{
		ShowWindow(SW_RESTORE);
	}
	BringWindowToTop();
}

void CHelpDialogWindow::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if ((cx != 0) && (cy != 0))
	{
		m_toolBar.AutoSize();
		RECT tbrect;
		m_toolBar.GetWindowRect(&tbrect);
		ScreenToClient(&tbrect);
		m_sizeBar.UpdateArea(CRect(0, tbrect.bottom + 1, cx, cy));
	}
}

void CHelpDialogWindow::OnClose()
{
	CWnd::OnClose();
}

void CHelpDialogWindow::OnDestroy()
{
	CWnd::OnDestroy();
	m_bIsExists = false;
}

bool CHelpDialogWindow::DisplayTopic(const CHelpSystemCore::CChm &chmRef, const std::string &path)
{
	return m_browserPane.DisplayCHM(chmRef.GetFileName(), path.c_str());
}

bool CHelpDialogWindow::DisplayResource(const CString &name)
{
	return m_browserPane.DisplayResource(name);
}

BOOL CHelpDialogWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case ID_BROWSE_BACK:
		{
			m_browserPane.GoBack();
			return TRUE;
		}
		case ID_BROWSE_FORWARD:
		{
			m_browserPane.GoForward();
			return TRUE;
		}
		case ID_BROWSE_STOP:
		{
			m_browserPane.Stop();
			return TRUE;
		}
		case ID_BROWSE_REFRESH:
		{
			m_browserPane.Refresh();
			return TRUE;
		}
		case ID_SYNC: return OnSync();
		case ID_SHOW: case ID_HIDE: return OnShowHide();
	}
	return CWnd::OnCommand(wParam, lParam);
}

BOOL CHelpDialogWindow::OnSync()
{
	CString chmFilename;
	CString path;
	if (m_browserPane.GetCHM(chmFilename, path))
	{
		const CHelpSystemCore::CChm * pchm = m_helpCore->FindChmByFullFilename(chmFilename);
		if (pchm)
		{
			const CHelpSystemCore::CDocument * pdoc = pchm->FindDocument(path.GetString());
			if (pdoc)
			{
				if (m_contents.SelectTopicNode(pdoc->GetTopicsTree()))
				{
					m_leftSheet.SetActivePage(&m_contents);
					if (!m_sizeBar.GetLeftVisible())
					{
						OnShowHide();
					}
				}
			}
		}
	}
	return TRUE;
}

BOOL CHelpDialogWindow::OnShowHide()
{
	m_sizeBar.SetLeftVisible(!m_sizeBar.GetLeftVisible());
	if (m_sizeBar.GetLeftVisible())
	{
		m_toolBar.SetState(ID_SHOW, TBSTATE_HIDDEN);
		m_toolBar.SetState(ID_HIDE, TBSTATE_ENABLED);
	}
	else
	{
		m_toolBar.SetState(ID_SHOW, TBSTATE_ENABLED);
		m_toolBar.SetState(ID_HIDE, TBSTATE_HIDDEN);
	}
	return TRUE;
}
