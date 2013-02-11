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

class CHelpDialogWindow;

// CPropPageIndex dialog

class CPropPageIndex : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPageIndex)

public:
	CPropPageIndex(CHelpSystemCore * helpCore);
	virtual ~CPropPageIndex();

	void SetHelpDialog(CHelpDialogWindow * helpDialog);

// Dialog Data
	enum { IDD = IDD_PROPPAGE_INDEX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLbnDblclkListIndex();
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnChangeEditKeyword();
	afx_msg void OnLbnSelchangeListIndex();

private:
	const CHelpSystemCore::CIndexTree * SelectedNode();
	void DisplaySelectedTopic();

	CStatic m_static1;
	CEdit m_editKeyword;
	CListBox m_listIndex;
	CButton m_btShow;

	CHelpSystemCore * m_helpCore;
	CHelpDialogWindow * m_helpDialog;
	bool m_bFilled;
	bool m_noUpdate;
};
