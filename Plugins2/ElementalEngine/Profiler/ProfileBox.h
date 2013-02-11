///============================================================================
/// \file		ProfileBox.h
/// \brief		Dialog box with profile data
/// \date		06/11/2007
/// \author		Nick Tourte
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

#ifndef _C_PROFILEBOX_H_
#define _C_PROFILEBOX_H_

#include "afxwin.h"
#include "afxcmn.h"

// CProfileBox dialog

class CProfileBox : public CDialog
{
	DECLARE_DYNAMIC(CProfileBox)

public:
	CProfileBox(PROFILERDISPLAYTYPE type, const TCHAR* name, CWnd* pParent = NULL);
	virtual ~CProfileBox();

// Dialog Data
	enum { IDD = IDD_PROFILEBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CListCtrl m_ListCtrl;
	int m_iCollumnSort;
	bool m_bAscendSort;	

	IProfileDisplayType *m_DisplayType;
	CHashString m_hsTabName;

	PERFORMANCEPROFILERMAP m_InternalPerformanceMap;

	void UpdateInternalMap(PERFORMANCEPROFILERMAP &map);

	bool m_bClearMapOnTick;

	CReturnComboBox m_FilterStringCombo;

	StdString m_szFilterString;

	void FilterMapNames();

	bool m_bExcludeFilter;

	void ClearInternalMap();

public:
	void SetWindowSize(int x, int y, int width, int height);
	void Init();
	void UpdateProfile(PERFORMANCEPROFILERMAP &map);	

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg void OnLvnColumnclickProfilelistctrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedClearmap();
	afx_msg void OnBnClickedCleartickcheck();	
	afx_msg void OnCbnKillfocusFiltercombo();	
	afx_msg void OnCbnSelchangeFiltercombo();
	afx_msg void OnBnClickedRadioinclusive();
	afx_msg void OnBnClickedRadioexclusive();
};

#endif //_C_PROFILEBOX_H_