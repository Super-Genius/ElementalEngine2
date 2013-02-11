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

#ifndef TERRAINSIZEDLG_H
#define TERRAINSIZEDLG_H

enum MEASUREMENT_UNITS
{
	KILOMETERS = 0,
	METERS,
	CENTIMETERS,
	MILES,
	FEET,
	INCHES
};

// CTerrainSizeDlg dialog

class CTerrainSizeDlg : public CDialog
{
	DECLARE_DYNAMIC(CTerrainSizeDlg)

public:
	CTerrainSizeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTerrainSizeDlg();
	void UpdateTerrainData();

// Dialog Data
	enum { IDD = IDD_TERRAIN_SIZE };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	/// creates the terrain from internal parameters
	void CreateTerrain();
	bool CheckValidData();
	void CheckButton(CButton *but);
	float ConvertToMeters(float srcSize);

	void FillTerrainCreationParam(TERRAINCREATIONPARAMS *pParams);
	static bool GetTerrainDescriptor(TERRAINDESCRIPTOR *pDescriptor);
	static void PauseThumbnailGeneration(bool pause);

	afx_msg void OnBnClickedImport();
	afx_msg void OnBnClickedCheckCenterNw();
	afx_msg void OnBnClickedCheckCenterN();
	afx_msg void OnBnClickedCheckCenterNe();
	afx_msg void OnBnClickedCheckCenterW();
	afx_msg void OnBnClickedCheckCenter();
	afx_msg void OnBnClickedCheckCenterE();
	afx_msg void OnBnClickedCheckCenterSw();
	afx_msg void OnBnClickedCheckCenterS();
	afx_msg void OnBnClickedCheckCenterSe();
	afx_msg void OnDeltaposSpinSectorWidth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinSectorZscale(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinSectorTrires(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusTriResolution();
	afx_msg void OnEnKillfocusEditSectorWidth();
	afx_msg void OnEnKillfocusEditSectorZscale();
	afx_msg void OnEnKillfocusEditTerrainHeight();
	afx_msg void OnEnKillfocusEditTerrainWidth();
	afx_msg void OnEnKillfocusTexturescalex();
	afx_msg void OnEnKillfocusTexturescaley();
	afx_msg void OnDeltaposSpinTerrainHeight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinTerrainWidth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedModifyCurrentTerrain();
	afx_msg void OnBnClickedTerrainTextureBrowse();	

	DECLARE_MESSAGE_MAP()

//public:
private:
	CTerrainEditor *m_pEditor;
	int m_TerrainWidth;
	int m_TerrainHeight;
	float m_SectorSize;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_TerrainName;
	float m_SectorZScale;
	CString m_TerrainBaseTexture;

	HICON	m_ArrowIcons[3][3];
	HICON	m_IconBlank;
	int		ResizeStart[2];
	CButton * m_LastButtonChecked;
	CButton * m_CurrentButtonChecked;
	CButton m_ButtonLeftUp;
	CButton m_ButtonUp;
	CButton m_ButtonRightUp;
	CButton m_ButtonRight;
	CButton m_ButtonLeft;
	CButton m_ButtonMiddle;
	CButton m_ButtonLeftDown;
	CButton m_ButtonDown;
	CButton m_ButtonRightDown;
	float m_BaseTextureScaleY;
	float m_BaseTextureScaleX;
	DWORD m_TriResolution;
	CComboBox m_MeasurementUnits;
	CGDALIO m_GDALIO;
};

#endif		// #ifndef TERRAINSIZEDLG_H
