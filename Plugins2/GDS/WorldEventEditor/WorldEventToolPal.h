///============================================================================
/// \file		WorldEventToolPal.h
/// \brief		Declaration of World Event Editor Tool Palatte
/// \date		05-01-2007
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

#ifndef _WORLDEVENTTOOLPAL_H
#define _WORLDEVENTTOOLPAL_H

// CTerrainToolPal dialog
class CWorldEventToolPal : public CGuiDialogBar
{
	DECLARE_DYNAMIC(CWorldEventToolPal)
public:
	CWorldEventToolPal(); // standard constructor
	virtual ~CWorldEventToolPal();

	void OnGlobalUpdateTick( void );

	enum { IDD = IDD_WORLDEVENT_TOOL_PALETTE };
	
private:
	IToolBox *m_ToolBox;
	CWorldEventEditor* m_pEditor;

	// World Event Trigger
	CComboBox m_Name;
	CComboBox m_TriggerName;
	CEdit m_EntityName;
	CEdit m_EntityType;
	CComboBox m_EventName;
	CEdit m_EventParam;
	
	CButton m_Create;
	CButton m_WET_Activate;
	CButton m_WE_Activate;
	CButton m_CheckVisualization;

	void WorldEventCreate( void );
	void WorldEventTriggerCreate( void );

	void UpdateWorldEventList( void );
	void UpdateWorldEventTriggerList( void );

	void UpdateWorldEvent( void );
	void UpdateWorldEventTrigger( void );

protected:
	void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LONG OnInitDialog ( UINT, LONG );
	afx_msg void OnUpdateCreate( CCmdUI* pCmdUI ) {}
	afx_msg void OnEnKillfocusTimerateEdit();
	afx_msg void OnBnClickedWETActivate();
	afx_msg void OnBnClickedWEActivate();
	afx_msg void OnBnClickedCreate();
	afx_msg void OnCbnDropdownTriggerName();
	afx_msg void OnCbnDropdownName();
	afx_msg void OnCbnSelchangeName();
	afx_msg void OnCbnDropdownEventname();
	afx_msg void OnBnClickedCheckVisualization();
};

#endif // _WORLDEVENTTOOLPAL_H
