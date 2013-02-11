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
#include "afxcmn.h"

class CHelpDialogWindow;

// CPropPageContents dialog

class CPropPageContents : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPageContents)

public:
	CPropPageContents(CHelpSystemCore * helpCore);
	virtual ~CPropPageContents();

	void SetHelpDialog(CHelpDialogWindow * helpDialog);
	bool SelectTopicNode(const CHelpSystemCore::CTopicsTree * node);

// Dialog Data
	enum { IDD = IDD_PROPPAGE_CONTENTS };

	enum { ICON_NORMAL = 0, ICON_EXPANDED = 1, ICON_LEAF = 2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTvnGetdispinfoTreeView(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTreeView(NMHDR *pNMHDR, LRESULT *pResult);

private:
	CTreeCtrl m_treeView;
	CImageList m_icons;

	CHelpSystemCore * m_helpCore;
	CHelpDialogWindow * m_helpDialog;
	bool m_bFilled;
	bool m_bNoUpdate;
};
