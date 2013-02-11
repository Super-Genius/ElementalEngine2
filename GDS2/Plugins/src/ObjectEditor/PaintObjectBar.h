///============================================================================
/// \file		PaintObjectBar.h
/// \brief		Header file for CPaintObjectBar
/// \date		09-20-2005
/// \author		Josh Lulewicz
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

#ifndef _PAINTOBJECTBAR_H_
#define _PAINTOBJECTBAR_H_

// CPaintObjectBar dialog

class CPaintObjectBar : public CGuiDialogBar
{
	DECLARE_DYNAMIC(CPaintObjectBar)

public:
	CPaintObjectBar();   // standard constructor
	virtual ~CPaintObjectBar();

	/// Get the current paint mode
	/// \param void
	/// \return paint mode
	PAINTMODE GetPaintMode() { return m_PaintMode; }

	/// Display terrain cursor
	/// \param void
	/// \return void
	void SendSetTerrainCursorMsg();

	/// Get the current brush size
	/// \param void
	/// \return float - size of the brush
	float GetBrushSize() { return m_fBrushSize; }

	/// Get the current scale max value
	/// \param void
	/// \return float - scale maximum
	float GetScaleMax() { return m_fScaleMax; }

	/// Get the current scale min value
	/// \param void
	/// \return float - scale minimum
	float GetScaleMin() { return m_fScaleMin; }

	/// Get the current rotation max value
	/// \param void
	/// \return float - rotation maximum
	float GetRotMax() { return m_fRotMax; }

	/// Get the current rotation min value
	/// \param void
	/// \return float - rotation minimum
	float GetRotMin() { return m_fRotMin; }

	/// Get the current rotation axis
	/// \param void
	/// \return int - rotation axis
	int GetRotationAxis() { return m_iRotationAxisIndex; }


	/// Find out if the random scale is enabled
	/// \param void
	/// \return true if it is false if not
	bool ScaleEnabled() { return m_bScale; }

	/// Find out if the random rotation is enabled
	/// \param void
	/// \return true if it is false if not
	bool RotationEnabled() { return m_bRotation; }

	/// Find out if level terrain is enabled
	/// \param void
	/// \return true if it is false if not
	bool TerrainLevelerEnabled() { return m_bLevelTerrainMode; }

	/// Set paint mode
	/// \param mode - new mode
	/// \return void
	void SetPaintMode(PAINTMODE mode);

	/// Set specified paint bar option
	/// \param option - paint option
	/// \return void
	void SetPaintOption(PAINTOPTIONS option);

// Dialog Data
	enum { IDD = IDD_OBJECT_PAINT_PALETTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LONG OnInitDialog( UINT, LONG );
private:
	/// Enable object painting
	/// \param void
	/// \return void
	void EnablePaint();

	/// Disable object painting
	/// \param void
	/// \return void
	void DisablePaint();

	// Minimum Scale used when painting objects
	CEdit m_ScaleMinEdit;
	// Enable random scaling for object painting
	CButton m_ScaleEnable;
	// Scaling of painted object maximum, percentage
	CEdit m_ScaleMaxEdit;
	// Enable random rotations for object painting
	CButton m_RotEnable;
	// Minimum rotations values in degrees
	CEdit m_RotMinEdit;
	// Rotation maximum around axis in degrees.
	CEdit m_RotMaxEdit;
	/// Paint Button
	CButton m_PaintToggleButton;
	/// Erase Button
	CButton m_EraseToggleButton;
	/// Paint Button
	CButton m_LevelTerrainToggleButton;
	/// Brush Shape
	int m_iBrushShape;
	/// Brush Size
	float m_fBrushSize;
	/// current paint mode
	PAINTMODE m_PaintMode; 
	/// brush size spinner
	CSpinButtonCtrl m_SpinnerBrushSize;
	/// brush size edit box
	CEdit m_EditBrushSize;
	/// MFC Controls
	CObjectEditor *m_pEditor;
	/// Axis to perform rotation
	CComboBox m_RotationAxisBox;
	/// current rotation axis
	int	m_iRotationAxisIndex;
	// Current maximum scale value
	float m_fScaleMax;
	// Current rotation minimum value
	float m_fRotMin;
	// Current rotation maximum value
	float m_fRotMax;
	// Current minimum scale value
	float m_fScaleMin;
	/// Scale enabled by the user
	bool m_bScale;
	/// Rotation enabled by the user
	bool m_bRotation;
	/// Level terrain under object enable by the user
	bool m_bLevelTerrainMode;
public:
	afx_msg void OnDeltaposObjectspinRotmax(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedObjectcheckScale();
	afx_msg void OnBnClickedObjectPaint();
	afx_msg void OnBnClickedObjectErase();
	afx_msg void OnEnKillfocusEditObjectbrushsize();
	afx_msg void OnEnChangeEditObjectbrushsize();
	afx_msg void OnDeltaposSpinObjectbrushsize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedObjectcheckRot();
public:
	afx_msg void OnDeltaposObjectspinRotmin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposObjectspinScalemax(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposObjectspinScalemin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusObjecteditRotmax();
	afx_msg void OnEnKillfocusObjecteditRotmin();
	afx_msg void OnEnKillfocusObjecteditScalemax();
	afx_msg void OnEnKillfocusObjecteditScalemin();
	afx_msg void OnCbnKillfocusObjectlistRotaxis();
	afx_msg void OnBnClickedObjectLevelterrain();
//	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnEnable(BOOL bEnable);
};

#endif  // #ifndef _PAINTOBJECTBAR_H_