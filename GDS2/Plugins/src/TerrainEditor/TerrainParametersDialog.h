///============================================================================
/// \file		TerrainParametersDialog.h
/// \brief		Header file for parameters dialog for terrain layers
/// \date		07-05-2005
/// \author		Kenneth Hurley
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
#ifndef _TERRAINPARAMETERSDIALOG_H_
#define _TERRAINPARAMETERSDIALOG_H_

class CTerrainParametersDialog : public CDialog
{
	DECLARE_DYNAMIC(CTerrainParametersDialog)

public:
	CTerrainParametersDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTerrainParametersDialog();

// Dialog Data
	enum { IDD = IDD_SECTOR_TEXTURES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	
	void SetRightPartEnableType(BOOL type = FALSE);
	
	IToolBox *m_ToolBox;
	IHashString *m_pSectorName;
	ITextureObject * m_PreviewTexture;
	int m_iAccumMask;
	CEdit m_MinHeightEdit;
	CEdit m_MaxHeightEdit;
	CSliderCtrl m_MinSlopeSlider;
	CSliderCtrl m_MaxSlopeSlider;
	int m_iMinHeight;
	int m_iMaxHeight;
	CButton m_GenSlope;
	CTerrainTextureListCtrl m_TextureListControl;
	int m_iCurrentSelectedLayer;
protected:
	virtual void OnOK();

public:
	virtual BOOL OnInitDialog();
	void GeneratePreview();	
	void SetPreviewBitmap( HBITMAP bitmap );
	
	CStatic m_PreviewControl;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedGenerateTexture();

	afx_msg void OnEnKillfocusEditMinheight();
	afx_msg void OnDeltaposSpinMinheight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditMaxheight();
	afx_msg void OnDeltaposSpinMaxheight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditMinheight();
	afx_msg void OnEnChangeEditMaxheight();

	afx_msg void OnBnClickedAccumMask();
	afx_msg void OnNMCustomdrawSliderMaxslope(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAddLayer();
	afx_msg void OnBnClickedSaveLayerAs();
	afx_msg void OnLvnItemchangedGenTexturesList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRemoveGenLayer();

	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


#endif // #ifndef _TERRAINPARAMETERSDIALOG_H_
