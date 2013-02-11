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
#include "PropPageContents.h"
#include ".\proppagecontents.h"


// CPropPageContents dialog

IMPLEMENT_DYNAMIC(CPropPageContents, CPropertyPage)

CPropPageContents::CPropPageContents(CHelpSystemCore * helpCore)
:	CPropertyPage(CPropPageContents::IDD, CPropPageContents::IDD),
	m_helpCore(helpCore), m_helpDialog(NULL), m_bFilled(false),
	m_bNoUpdate(false)
{
	ASSERT(m_helpCore);
}

CPropPageContents::~CPropPageContents()
{
}

void CPropPageContents::SetHelpDialog(CHelpDialogWindow * helpDialog)
{
	m_helpDialog = helpDialog;
}

bool CPropPageContents::SelectTopicNode(const CHelpSystemCore::CTopicsTree * node)
{
	if (node)
	{
		std::stack<const CHelpSystemCore::CTopicsTree *> nodes;
		while (node)
		{
			nodes.push(node);
			node = node->GetParent();
		}
		HTREEITEM hItem = m_treeView.GetRootItem();
		while (!nodes.empty() && (hItem != NULL))
		{
			if (reinterpret_cast<void *>(m_treeView.GetItemData(hItem)) == nodes.top())
			{
				nodes.pop();
				if (!nodes.empty())
				{
					hItem = m_treeView.GetChildItem(hItem);
				}
			}
			else
			{
				hItem = m_treeView.GetNextSiblingItem(hItem);
			}
		}
		if (hItem != NULL)
		{
			m_bNoUpdate = true;
			bool ok = (m_treeView.Select(hItem, TVGN_CARET) == TRUE);
			m_bNoUpdate = false;
			return ok;
		}
	}
	return false;
}

void CPropPageContents::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_VIEW, m_treeView);
}


BEGIN_MESSAGE_MAP(CPropPageContents, CPropertyPage)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TVN_GETDISPINFO, IDC_TREE_VIEW, OnTvnGetdispinfoTreeView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_VIEW, OnTvnSelchangedTreeView)
END_MESSAGE_MAP()


// CPropPageContents message handlers

BOOL CPropPageContents::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	ASSERT(m_helpDialog);

	if (!m_icons.Create(IDB_ICONS, 16, 0, 0xFF00FF))
	{
		// TODO: Warning
		return TRUE;
	}

	if (!m_treeView.SetImageList(&m_icons, TVSIL_NORMAL))
	{
		// TODO: Warning
		return TRUE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPropPageContents::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);
	if (m_treeView.GetSafeHwnd())
	{
		m_treeView.MoveWindow(CRect(8, 8, cx - 8, cy - 8), TRUE);
	}
}

struct STACK
{
	CHelpSystemCore::TOPICITERATOR begin;
	CHelpSystemCore::TOPICITERATOR end;
	HTREEITEM parent;

	STACK(CHelpSystemCore::TOPICITERATOR a_begin, CHelpSystemCore::TOPICITERATOR a_end, HTREEITEM a_parent)
	:	begin(a_begin), end(a_end), parent(a_parent)
	{}
};

void CPropPageContents::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	if (bShow && !m_bFilled)
	{
		m_bFilled = true;
		std::stack<STACK> topics;
		topics.push(STACK(m_helpCore->GetTreeBegin(), m_helpCore->GetTreeEnd(), TVI_ROOT));
		while (!topics.empty())
		{
			while (topics.top().begin != topics.top().end)
			{
				HTREEITEM item = m_treeView.InsertItem(
					TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM,
                    CString(topics.top().begin->GetTitle().c_str()),
					I_IMAGECALLBACK, I_IMAGECALLBACK, 0, 0,
					(LPARAM)&const_cast<CHelpSystemCore::CTopicsTree &>(*topics.top().begin),
                    topics.top().parent, TVI_LAST
				);
				if (item == NULL)
				{
					// TODO: Error while adding tree node.
					continue;
				}
				topics.push(STACK(topics.top().begin->GetChildBegin(), topics.top().begin->GetChildEnd(), item));
			}
			topics.pop();
			if (!topics.empty())
			{
				++topics.top().begin;
			}
		}
	}
}

void CPropPageContents::OnTvnGetdispinfoTreeView(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	if ((pTVDispInfo->item.mask & TVIF_IMAGE) || (pTVDispInfo->item.mask & TVIF_SELECTEDIMAGE))
	{
		int image = ICON_LEAF;
		if (m_treeView.ItemHasChildren(pTVDispInfo->item.hItem))
		{
			if (m_treeView.GetItemState(pTVDispInfo->item.hItem, TVIS_EXPANDED) & TVIS_EXPANDED)
			{
				image = ICON_EXPANDED;
			}
			else
			{
				image = ICON_NORMAL;
			}
		}
		if (pTVDispInfo->item.mask & TVIF_IMAGE)
		{
			pTVDispInfo->item.iImage = image;
		}
		if (pTVDispInfo->item.mask & TVIF_SELECTEDIMAGE)
		{
			pTVDispInfo->item.iSelectedImage = image;
		}
	}
	*pResult = 0;
}


void CPropPageContents::OnTvnSelchangedTreeView(NMHDR *pNMHDR, LRESULT *pResult)
{
	ASSERT(m_helpDialog);
	if (!m_bNoUpdate)
	{
		LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
		DWORD_PTR data = m_treeView.GetItemData(pNMTreeView->itemNew.hItem);
		if (data)
		{
			const CHelpSystemCore::CTopicsTree * node = reinterpret_cast<CHelpSystemCore::CTopicsTree *>(data);
			if (node->GetDocument())
			{
				m_helpDialog->DisplayTopic(node->GetDocument()->GetChm(), node->GetDocument()->GetPath());
				m_treeView.SetFocus();
			}
		}
	}
	*pResult = 0;
}
