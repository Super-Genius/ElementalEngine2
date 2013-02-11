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
#include "PropPageIndex.h"
#include ".\proppageindex.h"


// CPropPageIndex dialog

IMPLEMENT_DYNAMIC(CPropPageIndex, CPropertyPage)

CPropPageIndex::CPropPageIndex(CHelpSystemCore * helpCore)
:	CPropertyPage(CPropPageIndex::IDD, CPropPageIndex::IDD),
	m_helpCore(helpCore), m_helpDialog(NULL), m_bFilled(false),
	m_noUpdate(false)
{
	ASSERT(m_helpCore);
}

CPropPageIndex::~CPropPageIndex()
{
}

void CPropPageIndex::SetHelpDialog(CHelpDialogWindow * helpDialog)
{
	m_helpDialog = helpDialog;
}

void CPropPageIndex::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_static1);
	DDX_Control(pDX, IDC_EDIT_KEYWORD, m_editKeyword);
	DDX_Control(pDX, IDC_LIST_INDEX, m_listIndex);
	DDX_Control(pDX, IDOK, m_btShow);
}


BEGIN_MESSAGE_MAP(CPropPageIndex, CPropertyPage)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_LBN_DBLCLK(IDC_LIST_INDEX, OnLbnDblclkListIndex)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_KEYWORD, OnEnChangeEditKeyword)
	ON_LBN_SELCHANGE(IDC_LIST_INDEX, OnLbnSelchangeListIndex)
END_MESSAGE_MAP()


// CPropPageIndex message handlers

BOOL CPropPageIndex::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	ASSERT(m_helpDialog);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPropPageIndex::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	if (m_static1.GetSafeHwnd())
	{
		m_static1.MoveWindow(CRect(8, 8, cx - 8, 24), TRUE);
	}
	if (m_editKeyword.GetSafeHwnd())
	{
		m_editKeyword.MoveWindow(CRect(8, 32, cx - 8, 53), TRUE);
	}
	if (m_listIndex.GetSafeHwnd())
	{
		m_listIndex.MoveWindow(CRect(8, 62, cx - 8, cy - 40), TRUE);
	}
	if (m_btShow.GetSafeHwnd())
	{
		m_btShow.MoveWindow(CRect(cx - 104, cy - 32, cx - 8, cy - 8), TRUE);
	}
}

void CPropPageIndex::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	if (bShow && !m_bFilled)
	{
		m_bFilled = true;
		std::stack<std::pair<CHelpSystemCore::INDEXITERATOR, CHelpSystemCore::INDEXITERATOR> > index;
		index.push(std::make_pair(m_helpCore->GetIndexBegin(), m_helpCore->GetIndexEnd()));
		while (!index.empty())
		{
			while (index.top().first != index.top().second)
			{
				CString title(' ', (index.size() - 1) * 3);
				title += index.top().first->second.GetName().c_str();
				int i = m_listIndex.AddString(title);
				m_listIndex.SetItemData(i, (DWORD_PTR)&(const_cast<CHelpSystemCore::CIndexTree &>(index.top().first->second)));
				index.push(std::make_pair(index.top().first->second.GetChildBegin(), index.top().first->second.GetChildEnd()));
			}
			index.pop();
			if (!index.empty())
			{
				++index.top().first;
			}
		}
	}
}

const CHelpSystemCore::CIndexTree * CPropPageIndex::SelectedNode()
{
	int nIndex = m_listIndex.GetCurSel();
	if (nIndex != LB_ERR)
	{
		DWORD_PTR data = m_listIndex.GetItemData(nIndex);
		if (data != LB_ERR)
		{
			return reinterpret_cast<CHelpSystemCore::CIndexTree *>(data);
		}
	}
	return NULL;
}

void CPropPageIndex::DisplaySelectedTopic()
{
	ASSERT(m_helpDialog);
	const CHelpSystemCore::CIndexTree * node = SelectedNode();
	if (node && node->IsTopic())
	{
		const CHelpSystemCore::CDocument & doc = node->GetTopicBegin()->GetDocument();
		m_helpDialog->DisplayTopic(doc.GetChm(), doc.GetPath());
	}
}

void CPropPageIndex::OnLbnDblclkListIndex()
{
	DisplaySelectedTopic();
}

void CPropPageIndex::OnBnClickedOk()
{
	DisplaySelectedTopic();
}

BOOL CPropPageIndex::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_RETURN:
			{
				if (pMsg->hwnd != m_btShow.GetSafeHwnd())
				{
					DisplaySelectedTopic();
					return TRUE;
				}
				break;
			}
			case VK_UP: case VK_DOWN: case VK_PRIOR: case VK_NEXT:
			{
				if ((pMsg->hwnd == m_editKeyword.GetSafeHwnd()) && (m_listIndex.GetSafeHwnd() != NULL))
				{
					// Route 'Up', 'Down', 'PgUp', 'PgDown' to listbox.
					pMsg->hwnd = m_listIndex.GetSafeHwnd();
				}
				break;
			}
		}
	}
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CPropPageIndex::OnEnChangeEditKeyword()
{
	if (m_noUpdate)
	{
		return;
	}
	CString str;
	m_editKeyword.GetWindowText(str);
	const CHelpSystemCore::CIndexTree *node = m_helpCore->SearchIndex(str.GetString());
	if (node && (m_listIndex.GetCurSel() != node->GetSeqNumber()))
	{
		m_noUpdate = true;
		m_listIndex.SetCurSel(node->GetSeqNumber());
		m_noUpdate = false;
	}
}

void CPropPageIndex::OnLbnSelchangeListIndex()
{
	if (m_noUpdate)
	{
		return;
	}
	const CHelpSystemCore::CIndexTree * node = SelectedNode();
	if (node)
	{
		m_noUpdate = true;
		CString text = node->GetFullName().c_str();
		m_editKeyword.SetWindowText(text);
		m_editKeyword.SetSel(text.GetLength(), text.GetLength(), TRUE);
		m_noUpdate = false;
	}
}
