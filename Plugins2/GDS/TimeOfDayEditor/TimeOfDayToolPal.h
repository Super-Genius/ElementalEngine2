///============================================================================
/// \file		TimeOfDayToolPal.h
/// \brief		Declaration of TimeOfDay Tool Palatte
/// \date		08-12-2005
/// \author		D. Patrick Ghiocel
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

#ifndef _TIMEOFDAYTOOLPAL_H
#define _TIMEOFDAYTOOLPAL_H

// CTerrainToolPal dialog
class CTimeOfDayToolPal : public CGuiDialogBar
{
	DECLARE_DYNAMIC(CTimeOfDayToolPal)
public:
	CTimeOfDayToolPal(); // standard constructor
	virtual ~CTimeOfDayToolPal();

	void OnGlobalUpdateTick( void );

	enum { IDD = IDD_TIMEOFDAY_TOOL_PALETTE };

private:
	IToolBox *m_ToolBox;
	CTimeOfDayEditor *m_pEditor;

	int m_iTimeSlider;
	float m_fTimeRate;

	CSliderCtrl m_ctrlTimeSlider;
	CEdit m_ctrlTimeRateEdit;
	CSpinButtonCtrl m_ctrlTimeRateSpinner;
	CButton m_ctrlTODMotionButton;
	CButton m_ctrlTODLightButton;
	CButton m_ctrlTODDialogButton;
	/// Weather effects button
	CButton m_ctrlWeatherDialogButton;
	CButton m_ctrlShadowToggleButton;
protected:
	void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LONG OnInitDialog ( UINT, LONG );
	afx_msg void OnEnKillfocusTimerateEdit();
	afx_msg void OnDeltaposTimerateSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedTimeofdayMotion();
	afx_msg void OnBnClickedTimeofdayLight();
	afx_msg void OnBnClickedShadowToggle();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnUpdateButtons( CCmdUI* pCmdUI );
	afx_msg void OnBnClickedToddialogButton();
	afx_msg void OnBnClickedTodWeatherButton();
};

#endif // _TIMEOFDAYTOOLPAL_H
