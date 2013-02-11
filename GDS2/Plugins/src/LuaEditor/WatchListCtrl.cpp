///============================================================================
/// \file		WatchListCtrl.cpp
/// \brief		Implementation file for CWatchListCtrl
/// \date		06-21-2005
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

// CWatchListCtrl

IMPLEMENT_DYNAMIC(CWatchListCtrl, CListCtrl)

CWatchListCtrl::CWatchListCtrl()
{
}

CWatchListCtrl::~CWatchListCtrl()
{
}


BEGIN_MESSAGE_MAP(CWatchListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnLvnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
END_MESSAGE_MAP()


void CWatchListCtrl::AddEmptyRow()
{
	int nItem = InsertItem(GetItemCount(), "");
	SetItem(nItem, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED, 0);
}

void CWatchListCtrl::AddEditItem(LVITEM &item)
{
	if ( item.iItem == GetItemCount()-1 )
	{
		if ( strlen(item.pszText) == 0 )
			return;

		m_exps.Add(item.pszText);
		AddEmptyRow();
		UpdateRow(item.iItem);
	}
	else
	{
		if ( strlen(item.pszText) == 0 )
		{
			DeleteItem(item.iItem);
			m_exps.RemoveAt(item.iItem);
			return;
		}
		else
		{
			m_exps[item.iItem] = item.pszText;
			UpdateRow(item.iItem);
		}
	}
}

void CWatchListCtrl::UpdateRow(int iItem)
{
	SetItemText(iItem, 0, m_exps[iItem]);

	CMDIFrameWnd *mainFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	CMDIChildWnd *childFrame = mainFrame->MDIGetActive();
	CLuaView *view = DYNAMIC_DOWNCAST(CLuaView, childFrame->GetActiveView());
	CLuaFrame *frame = DYNAMIC_DOWNCAST(CLuaFrame, view->GetParent());
	CDebugger *debugger = frame->GetDebugger();

	if (frame->Debugging())
	{
		SetItemText(iItem, 1, debugger->Eval(m_exps[iItem]));
	}
}


void CWatchListCtrl::Redraw()
{
	CMDIFrameWnd *mainFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	CMDIChildWnd *childFrame = mainFrame->MDIGetActive();
	CLuaView *view = DYNAMIC_DOWNCAST(CLuaView, childFrame->GetActiveView());
	CLuaFrame *frame = DYNAMIC_DOWNCAST(CLuaFrame, view->GetParent());
	CDebugger *debugger = frame->GetDebugger();

	for (int i = 0; i < m_exps.GetSize(); ++i)
	{
		SetItemText(i, 1, debugger->Eval(m_exps[i]));
	}
}


// CWatchListCtrl message handlers

BOOL CWatchListCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= LVS_EDITLABELS|LVS_NOSORTHEADER|LVS_SHOWSELALWAYS|LVS_SINGLESEL;

	return CListCtrl::PreCreateWindow(cs);
}

void CWatchListCtrl::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
 	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	if ( pDispInfo->item.pszText )
	{
		AddEditItem(pDispInfo->item);
	}

	*pResult = 0;
}

void CWatchListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CPoint point(GetMessagePos());

	SetFocus();

	LVHITTESTINFO lvhti;
	lvhti.pt = point;
	SubItemHitTest(&lvhti);

	if (lvhti.flags & LVHT_ONITEMLABEL)
	{
		EditLabel(lvhti.iItem);
	}

	*pResult = 0;
}
