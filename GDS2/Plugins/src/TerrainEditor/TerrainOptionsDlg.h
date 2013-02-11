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

#ifndef TERRAINOPTIONSDLG_H
#define TERRAINOPTIONSDLG_H

// CTerrainOptionsDlg dialog

class CTerrainOptionsDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CTerrainOptionsDlg)

public:
	CTerrainOptionsDlg();
	virtual ~CTerrainOptionsDlg();

	int GetMaxLayers() { return m_iMaxLayers; }
	
// Dialog Data
	enum { IDD = IDD_TERRAIN_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();

	/// Value that is changed by control.
	int m_iTempMaxLayers;
	/// Value that is visible outside of the class and is only set when Ok is pressed.
	int m_iMaxLayers;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeTerrainOptMaxlayerEdit();
	afx_msg void OnDeltaposTerrainOptMaxlayerSpin(NMHDR *pNMHDR, LRESULT *pResult);
};

#endif		//#ifndef TERRAINOPTIONSDLG_H
