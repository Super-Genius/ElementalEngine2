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

#ifndef _TERRAINTOOLPAL_H
#define _TERRAINTOOLPAL_H

// Limits
const float g_fMaxBrushSize = 10000.0f;
const float g_fMinBrushSize = 1.0f;
const float g_fMaxStrength = 100.0f;
const float g_fMinStrength = 1.0f;

// CTerrainToolPal dialog
class CTerrainToolPal : public CGuiDialogBar
{
	DECLARE_DYNAMIC(CTerrainToolPal)

public:
	CTerrainToolPal(); // standard constructor
	virtual ~CTerrainToolPal();

	float GetBrushSize()       { return m_fBrushSize; }
	float GetBrushStrength()   { return m_fBrushStrength; }
	bool GetModifyDown()            { return m_bDeformLower; }
	EnumTerrainTool GetTerrainTool() { return m_eTerrainTool; }
	int GetBrushIndex() { return m_iBrushShape; }

	// Dialog Data
	enum { IDD = IDD_TERRAIN_TOOL_PALETTE };
	// TODO: This is ugly, needs to be changed
	float m_fBrushSize;
	float m_fBrushStrength;

	float m_fBrushSizeDeform;
	float m_fBrushSizePaint;
	float m_fBrushStrengthDeform;
	float m_fBrushStrengthPaint;	
	// End TODO

	float m_fMaxHeight;
	int m_bMaxCap;
	float m_fMinHeight;
	int m_bMinCap;

	int m_iBrushShape;
	int m_iBrushOp;

	// toolbox to send message and log
	IToolBox *m_ToolBox;

	/// Flag true if brush lowers terrain, false to raise
	bool m_bDeformLower;

	/// Currently selected tool to use on terrain
	EnumTerrainTool m_eTerrainTool;

	/// The tool selected before changing to other palette (when we get back
	/// we want to restore this
	EnumTerrainTool m_eTerrainToolLastInUse;


	float m_fNewLayerScale;

	/// \brief Sends a message about brush size change, Call this, if the size needs to be updated 
	///		necessary, because we need to handle the spinner buttons, and the written value
	///		changes as well
	/// \param clearBrush (bool) if true, the size 0 is set out to clear the current brush.
	//// \return true on success
	bool UpdateBrushSize(bool clearBrush = false);

	/// \brief Sends a message about the current brush shape(texture).
	void UpdateBrushshape();

protected:
	void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
	afx_msg LONG OnInitDialog ( UINT, LONG );
	afx_msg void OnBnClickedPaint();
	afx_msg void OnBnClickedDeform();
	afx_msg void OnBnClickedRaise();
	afx_msg void OnBnClickedLower();
	afx_msg void OnEnChangeSizeEdit();
	afx_msg void OnDeltaposSizeSpinner(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeStrengthEdit();
	afx_msg void OnDeltaposStrengthSpinner(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnUpdatePaint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeform(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRaise(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLower(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLightmapGenerate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTerrainProp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTerrainClean(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTerrainModify(CCmdUI * pCmdUI );
	//afx_msg void OnUpdateNavMesh(CCmdUI * pCmdUI );
	afx_msg void OnBnClickedTerrainProp();
	afx_msg void OnBnClickedTerrainClean();
	afx_msg void OnBnClickedMaxheightEnable();
	afx_msg void OnEnChangeMaxheightEdit();
	afx_msg void OnDeltaposMaxheightSpinner(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMinheightEnable();
	afx_msg void OnEnChangeMinheightEdit();
	afx_msg void OnDeltaposMinheightSpinner(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeBrushshape();
	afx_msg void OnCbnSelchangeBrushop();
	afx_msg void OnBnClickedTerrainModify();
	//afx_msg void OnBnClickedNavmesh();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnEnChangeNewlayersizeEdit();

	/// Disable the toolbar
	/// \param void
	/// \return void
	//void Disable();

	/// MFC Controls
	CTerrainEditor *m_pEditor;

	CButton m_DeformToggleButton;
	CButton m_LowerToggleButton;
	CButton	m_LightmapGenerateButton;
	CButton m_MaxHeightEnable;
	CButton m_MinHeightEnable;
	CButton m_PaintToggleButton;
	CButton m_RaiseToggleButton;
	CComboBoxEx m_BrushShape;
	CComboBoxEx m_BrushOp;
	CImageList m_BrushShapeImages;
	CEdit m_BrushSizeEdit;
	CEdit m_BrushStrengthEdit;
	CEdit m_MaxHeightEdit;
	CEdit m_MinHeightEdit;
	CSpinButtonCtrl m_MaxHeightSpinner;
	CSpinButtonCtrl m_MinHeightSpinner;
};

#endif		// #ifndef _TERRAINTOOLPAL_H
