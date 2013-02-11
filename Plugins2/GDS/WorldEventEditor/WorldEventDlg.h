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
#include "afxwin.h"


// CWorldEventDlg dialog

class CWorldEventDlg : public CDialog
{
	DECLARE_DYNAMIC(CWorldEventDlg)

public:
	CWorldEventDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWorldEventDlg();

// Dialog Data
	enum { IDD = IDD_WORLDEVENT_DLG };

	void SetSelectedObject( IHashString *pObjectName );

protected:
	void UpdateSelectedObject( IHashString *pObjectName );
	void ShowAddGameEventPopupMenu( CPoint& point );
	void LoadGlobalStateObject( StdString stateObjectName, StdString stateObjectParentName,  StdString stateMachineName, StdString stateMachineFileName, StdString startStateName, bool isInHierarchy );

	IToolBox *m_ToolBox;
	CWorldEventEditor* m_pEditor;
	CWorldEventTree m_ObjWorldEventTree;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchangedObjWorldeventTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnDeleteitemObjWorldeventTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	CEdit m_WorldEventParamEdit;
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnBnClickedButtonAddgameevent();
protected:
	CButton m_ButtonAddGameEvent;
public:
	afx_msg void OnBnClickedButtonLoadgamescript();
};
