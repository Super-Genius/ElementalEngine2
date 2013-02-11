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

#ifndef _C_GUITABBAR_H_
#define _C_GUITABBAR_H_

// CGUITabBar dialog

class CGUITabBar : public CGuiDialogBar
{
	DECLARE_DYNAMIC(CGUITabBar)

public:
	CGUITabBar(); 
	virtual ~CGUITabBar();

// Dialog Data
	enum { IDD = IDD_EXAMPLE_TOOL_PALETTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void OnUpdateButtons( CCmdUI* pCmdUI ) {}
	bool m_bLoaded;
	int m_iXPos;
	int m_iYPos;
	float m_fZoom;

	CButton m_NewPageBt;
	CButton m_NewGroupBt;
	CButton m_NewButtonBt;
	CButton m_NewTextBt;
	CButton m_NewStaticBt;
	CButton m_ChangeBGBt;
	CButton m_FullScreenBt;
	CButton m_PlayBt;
	CButton m_ItemInst;
	CButton m_InstLock;
	CButton m_NewAnimBt;
	CButton m_PlayAnimBt;
	CButton m_StopAnimBt;
	vector<StdString> m_vResolutions;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg LONG OnInitDialog(UINT wParam, LONG lParam);		
};

#endif
