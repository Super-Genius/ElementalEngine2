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
#include "PropPageSearch.h"
#include ".\proppagesearch.h"


// CPropPageSearch dialog

IMPLEMENT_DYNAMIC(CPropPageSearch, CPropertyPage)

CPropPageSearch::CPropPageSearch(CHelpSystemCore * helpCore)
:	CPropertyPage(CPropPageSearch::IDD, CPropPageSearch::IDD),
	m_helpCore(helpCore)
{
}

CPropPageSearch::~CPropPageSearch()
{
}

void CPropPageSearch::SetHelpDialog(CHelpDialogWindow * helpDialog)
{
	m_helpDialog = helpDialog;
}

void CPropPageSearch::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC2, m_static2);
	DDX_Control(pDX, IDC_COMBO_SEARCH, m_comboSearch);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_buttonSearch);
	DDX_Control(pDX, IDC_BUTTON_SHOW, m_buttonShow);
	DDX_Control(pDX, IDC_LIST_RESULTS, m_listResults);
}


BEGIN_MESSAGE_MAP(CPropPageSearch, CPropertyPage)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnBnClickedButtonSearch)
	ON_LBN_DBLCLK(IDC_LIST_RESULTS, OnLbnDblclkListResults)
	ON_CBN_EDITCHANGE(IDC_COMBO_SEARCH, OnCbnEditchangeComboSearch)
	ON_LBN_SELCHANGE(IDC_LIST_RESULTS, OnLbnSelchangeListResults)
	ON_BN_CLICKED(IDC_BUTTON_SHOW, OnBnClickedButtonShow)
END_MESSAGE_MAP()


// CPropPageSearch message handlers

void CPropPageSearch::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	if (m_static2.GetSafeHwnd())
	{
		m_static2.MoveWindow(CRect(8, 8, cx - 8, 24), TRUE);
	}
	if (m_comboSearch.GetSafeHwnd())
	{
		m_comboSearch.MoveWindow(CRect(8, 32, cx - 8, 108), TRUE);
	}
	if (m_buttonSearch.GetSafeHwnd())
	{
		m_buttonSearch.MoveWindow(CRect(cx - 176, 62, cx - 96, 86), TRUE);
	}
	if (m_buttonShow.GetSafeHwnd())
	{
		m_buttonShow.MoveWindow(CRect(cx - 88, 62, cx - 8, 86), TRUE);
	}
	if (m_listResults.GetSafeHwnd())
	{
		m_listResults.MoveWindow(CRect(8, 94, cx - 8, cy - 8), TRUE);
	}
}

void CPropPageSearch::OnBnClickedButtonSearch()
{
	PerformSearch();
}

CHelpSystemCore::CSearchMatch * CPropPageSearch::SelectedMatch()
{
	int nIndex = m_listResults.GetCurSel();
	if (nIndex != LB_ERR)
	{
		DWORD_PTR data = m_listResults.GetItemData(nIndex);
		if (data != LB_ERR)
		{
			return reinterpret_cast<CHelpSystemCore::CSearchMatch *>(data);
		}
	}
	return NULL;
}

void CPropPageSearch::DisplaySelectedMatch()
{
	ASSERT(m_helpDialog);
	CHelpSystemCore::CSearchMatch * match = SelectedMatch();
	if (match)
	{
		const CHelpSystemCore::CDocument & doc = match->GetDocument();
		m_helpDialog->DisplayTopic(doc.GetChm(), doc.GetPath());
	}
}

void CPropPageSearch::OnLbnDblclkListResults()
{
	DisplaySelectedMatch();
}

void CPropPageSearch::OnCbnEditchangeComboSearch()
{
	if (m_comboSearch.GetWindowTextLength() > 0)
	{
		m_buttonSearch.EnableWindow(TRUE);
	}
	else
	{
		m_buttonSearch.EnableWindow(FALSE);
	}
}

void CPropPageSearch::OnLbnSelchangeListResults()
{
	if (m_listResults.GetCurSel() >= 0)
	{
		m_buttonShow.EnableWindow(TRUE);
	}
	else
	{
		m_buttonShow.EnableWindow(FALSE);
	}
}

void CPropPageSearch::OnBnClickedButtonShow()
{
	DisplaySelectedMatch();
}

BOOL CPropPageSearch::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		if (::IsChild(m_comboSearch.GetSafeHwnd(), pMsg->hwnd))
		{
			PerformSearch();
			return TRUE;
		}
		else if (pMsg->hwnd == m_listResults.GetSafeHwnd())
		{
			DisplaySelectedMatch();
			return TRUE;
		}
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CPropPageSearch::PerformSearch()
{
	CString text;
	m_comboSearch.GetWindowText(text);
	if (!text.IsEmpty())
	{
		// Clean previous results.
		m_listResults.ResetContent();
		m_searchResult.clear();
		OnLbnSelchangeListResults();
		// Perform search.
		m_helpCore->Search(m_searchResult, text.GetString(), false, false);
		UpdateSearchResult();
	}
}

void CPropPageSearch::UpdateSearchResult()
{
	if (m_listResults.GetSafeHwnd())
	{
		for (CHelpSystemCore::SEARCHRESULT::iterator i = m_searchResult.begin(); i != m_searchResult.end(); ++i)
		{
			CString title;
			// Retrieving title to display.
			if (!i->GetTitle().empty())
			{
				title = i->GetTitle().c_str();
			}
			else if (i->GetDocument().GetTopicsTree())
			{
				title = i->GetDocument().GetTopicsTree()->GetTitle().c_str();
			}
			else if (i->GetDocument().GetIndexNode())
			{
				title = i->GetDocument().GetIndexNode()->GetName().c_str();
			}
			else
			{
				title = i->GetKey().c_str();
			}
			int index = m_listResults.AddString(title);
			m_listResults.SetItemData(index, reinterpret_cast<DWORD_PTR>(&(*i)));
		}
	}
}
