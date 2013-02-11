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

BEGIN_MESSAGE_MAP(CCallStack, CWnd)
	//{{AFX_MSG_MAP(CCallStackView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CCallStack::CCallStack(void)
{
}

CCallStack::~CCallStack(void)
{
}

int CCallStack::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	static int nCtrlID = ID_OUTPUT_CTRL1;

	// TODO: Add your specialized creation code here
	m_view.Create(this, nCtrlID++);
	m_view.SetReadOnly(TRUE);
	m_view.SetCallStackMargins();

	Clear();

	return 0;
}

void CCallStack::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if ( ::IsWindow(m_view.m_hWnd) )
		m_view.SetWindowPos(NULL, 0, 0, cx, cy, 0);	
}

void CCallStack::Clear()
{
	m_view.SetReadOnly(FALSE);
	m_view.ClearAll();
	m_view.SetReadOnly(TRUE);

	m_nCurrentLevel = -1;
//	m_lines.RemoveAll();
//	m_files.RemoveAll();
}

void CCallStack::Write(CString strMsg)
{
	m_view.SetReadOnly(FALSE);
	m_view.AddText(strMsg);
	m_view.SetReadOnly(TRUE);
	m_view.GotoLastLine();
}

BOOL CCallStack::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	NMHDR *lpnmhdr = (LPNMHDR) lParam; 

	if (lpnmhdr->hwndFrom == m_view.m_hWnd)
	{
		*pResult = OnSci((SCNotification*)lParam);
		return TRUE;
	}
	
	
	return CWnd::OnNotify(wParam, lParam, pResult);
}

int CCallStack::OnSci(SCNotification* pNotify)
{
	CPoint pt;
	int nLine;
	CString strLine;
	switch (pNotify->nmhdr.code)
	{
		case SCN_DOUBLECLICK:
			GetCursorPos(&pt);
			m_view.ScreenToClient(&pt);
			nLine = m_view.LineFromPoint(pt);
			GotoStackTraceLevel(nLine-1);
		break;
	};

	return 0;
}

void CCallStack::Add(const char *szDesc, const char *szFile, int nLine)
{
	Write(CString(szDesc)+"\n");

//	m_files.Add(szFile);
//	m_lines.Add(nLine);
}

void CCallStack::GotoStackTraceLevel(int nLevel)
{
	/*
	if ( nLevel<0 || nLevel>=m_files.GetSize() )
		return;
	*/

	m_nCurrentLevel = nLevel;

	m_view.SetStackTraceLevel(nLevel);

//	((CMainFrame*)AfxGetMainWnd())->GotoFileLine(m_files[nLevel], m_lines[nLevel]);	
//	((CMainFrame*)AfxGetMainWnd())->GetDebugger()->StackLevelChanged();
}

