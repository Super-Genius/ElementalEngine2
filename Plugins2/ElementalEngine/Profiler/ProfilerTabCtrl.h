///============================================================================
/// \file		ProfilerTabCtrl.h
/// \brief		Dialog box with tabs 
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

#ifndef _C_PROFILERTABCTRL_H_
#define _C_PROFILERTABCTRL_H_

#include "afxcmn.h"
#include "afxwin.h"

// CProfilerTabCtrl dialog

class CProfileBox;

typedef vector<CProfileBox*> PROFILEBOXVEC;
typedef map<DWORD, int> NAMEINDEXMAP;

class CProfileBox;



// tab dialog class
class CProfilerTabCtrl : public CDialog
{
	DECLARE_DYNAMIC(CProfilerTabCtrl)

public:
	CProfilerTabCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProfilerTabCtrl();

	void AddTab(const TCHAR *name, PROFILERDISPLAYTYPE type);
	
	void ResizeWindow(int x, int y, int width, int height);

	void UpdateProfiles(PERFORMANCEPROFILERTYPEMAP &map);

// Dialog Data
	enum { IDD = IDD_TABBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void ResizeInternalBox(CProfileBox *box);

	DECLARE_MESSAGE_MAP()

	PROFILEBOXVEC m_InternalBoxes;
	NAMEINDEXMAP m_TabNames;

	float m_fUpdateTime;

	CReturnEdit m_StrobeEdit;
	CSpinButtonCtrl m_StrobeSpinner;
	CTabCtrl m_TabControl;

public:
	
	afx_msg void OnTcnSelchangeTabctrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposStrobespin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusStrobeedit();	
	afx_msg void OnBnClickedHidewindow();
	CStatic m_FPSCtrl;

	void UpdateFPS(float fps);
};

#endif //_C_PROFILERTABCTRL_H_