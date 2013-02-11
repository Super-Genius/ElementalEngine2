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

// CTerrainParametersDialog dialog
struct FLOATPAIR
{
	float X;
	float Y;
};

typedef map<DWORD, FLOATPAIR> TEXTURESCALEMAP;

// CTerrainLayerMaterialType dialog 

class CTerrainLayerMaterialType : public CDialog
{
	DECLARE_DYNCREATE(CTerrainLayerMaterialType)

public:
	CTerrainLayerMaterialType();
	~CTerrainLayerMaterialType();
	void SetSectorName( IHashString *pSectorName );
	void InsertTextureListItem( int iPosition, const TCHAR *szFilePath, float fTextureScaleX, float fTextureScaleY, bool bSendToTerrain );

	// Dialog Data
	enum { IDD = IDD_LAYER_MATERIAL_TYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP()
private:
	CStatic m_StatusText;
	CTerrainEditor *m_pEditor;
	IHashString *m_pSectorName;
	IToolBox *m_ToolBox;
	CTerrainTextureListCtrl m_TextureListCtrl;
	FLOATPAIR m_SplatScale;
	int m_iSplatScaleProportional;
	TEXTURESCALEMAP m_TextureListScaleMap;
	int m_iCurrentSelectedLayer;

	/// Image of item dragged from this list
	CImageList *m_pDragImage;
	// Dragging flag
	bool m_bDragging;
	int m_nDragIndex;
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedRemoveTexture();
	afx_msg void OnLvnItemchangedSectorTexturesList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBegindragSectorTexturesList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeSplatscalexEdit();
	afx_msg void OnDeltaposSplatscaleySpinner(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSplatscalexSpinner(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSplatscaleProportional();
	afx_msg void OnEnChangeSplatscaleyEdit();
private:
	void AdjustControlSize(CWnd *pControl, int cx);
};
