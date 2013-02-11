///============================================================================
/// \file		LuaDockToolTab.cpp
/// \brief		Implementation file for CLuaDockToolTab
/// \date		06-14-2005
/// \author		Josh Lulewicz
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

CLuaDockToolTab::CLuaDockToolTab(void)
{
}

CLuaDockToolTab::~CLuaDockToolTab(void)
{
}
BEGIN_MESSAGE_MAP(CLuaDockToolTab, CGuiControlBar)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CLuaDockToolTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if (!m_WorkTabWnd.Create(WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, 124))
		return -1;

	CRect rectClient (0, 0, lpCreateStruct->cx, lpCreateStruct->cy);
	const DWORD dwStyle =  WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
						   ES_MULTILINE | ES_READONLY;
	
	// create build and debug tabs
	m_BuildWnd.Create(dwStyle,rectClient,&m_WorkTabWnd,IDC_BUILD_WND);
	m_DebugWnd.Create(dwStyle,rectClient,&m_WorkTabWnd,IDC_DEBUG_WND);

	// create callstack and watch tabs
	m_CallStackWnd.Create(NULL,"", WS_CHILD | WS_VISIBLE, rectClient, &m_WorkTabWnd, IDC_CALLSTACK_WND);
	m_WatchWnd.Create(WS_TABSTOP|WS_CHILD|WS_VISIBLE|LVS_REPORT, rectClient, &m_WorkTabWnd, IDC_WATCH_WND);

	// add columns to watch window
	m_WatchWnd.InsertColumn(0, _T("Name"), LVCFMT_LEFT,  100);
	m_WatchWnd.InsertColumn(1, _T("Value"), LVCFMT_LEFT, 400);
	m_WatchWnd.AddEmptyRow();

	// add tabs to group
	m_WorkTabWnd.Addtab(&m_BuildWnd, _T("Build"), -1);
	m_WorkTabWnd.Addtab(&m_DebugWnd, _T("Debug"), -1);
	m_WorkTabWnd.Addtab(&m_CallStackWnd, _T("Call Stack"), -1);
	m_WorkTabWnd.Addtab(&m_WatchWnd, _T("Watch"), -1);
	
	return 0;
}

void CLuaDockToolTab::SetText(StdString *string, outputTypes type)
{

	// if type is Build window
	if (type == outputBuild)
	{
		m_BuildWnd.ReplaceSel(string->c_str());
	}
	// else if type is Debug window
	else if (type == outputDebug)
	{
		m_DebugWnd.ReplaceSel(string->c_str());
	}
}

void CLuaDockToolTab::UpdateWatches()
{
	// redraw watches
	m_WatchWnd.Redraw();
}

void CLuaDockToolTab::ClearOutput(outputTypes type)
{
	if (type == outputBuild)
	{
		m_BuildWnd.SetSel(0, -1);
		m_BuildWnd.Clear();
		m_BuildWnd.ReplaceSel(_T(" "));
	}
	else if (type == outputDebug)
	{
		m_DebugWnd.SetSel(0, -1);
		m_DebugWnd.Clear();
		m_DebugWnd.ReplaceSel(_T(" "));
	}
}