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

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CPropPageSearch dialog

class CPropPageSearch : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPageSearch)

public:
	CPropPageSearch(CHelpSystemCore * helpCore);
	virtual ~CPropPageSearch();

	void SetHelpDialog(CHelpDialogWindow * helpDialog);

// Dialog Data
	enum { IDD = IDD_PROPPAGE_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnLbnDblclkListResults();
	afx_msg void OnCbnEditchangeComboSearch();
	afx_msg void OnLbnSelchangeListResults();
	afx_msg void OnBnClickedButtonShow();

private:
	void PerformSearch();
	void UpdateSearchResult();
	CHelpSystemCore::CSearchMatch * SelectedMatch();
	void DisplaySelectedMatch();

	CStatic m_static2;
	CComboBox m_comboSearch;
	CButton m_buttonSearch;
	CButton m_buttonShow;
	CListBox m_listResults;

	CHelpSystemCore * m_helpCore;
	CHelpDialogWindow * m_helpDialog;
	CHelpSystemCore::SEARCHRESULT m_searchResult;
};
