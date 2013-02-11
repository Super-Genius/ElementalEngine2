///============================================================================
/// \file		ObjectToolPal.h
/// \brief		Header file for <insert brief description here>
/// \date		07-15-2005
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

#ifndef _OBJECTTOOLPAL_H_
#define _OBJECTTOOLPAL_H_

// CObjectToolPal dialog

class CObjectToolPal : public CGuiDialogBar
{
	DECLARE_DYNAMIC(CObjectToolPal)

public:
//	CObjectToolPal(CWnd* pParent = NULL);   // standard constructor
	CObjectToolPal();   // standard constructor
	virtual ~CObjectToolPal();

	int GetManipulateState() { return m_ObjectManipulateState; }
	void SetManipulateState(MANIPULATE_STATE newState);

	float GetSnapTo()				{ return m_fSnapToGrid; }
	void SetSnapTo(float fSnapTo)
	{
		m_fSnapToGrid = fSnapTo; 
		UpdateData(false);
	}
	
	int GetLockIndex() { return m_iLockIndex; }
	void SetLockIndex(int newIndex)
	{ 
		m_iLockIndex = newIndex; 
		UpdateData(false);
	}

	bool GetLockValueCheck()
	{
		return (m_LockValueCheck.GetCheck() == 1);
	}
	void SetLockValueCheck(bool checked)
	{	
		m_LockValueCheck.SetCheck(checked);
	}

	float GetLockValue() { return m_fLockValue; }
	void SetLockValue( float fLockValue )
	{
		static char sz[12];
		sprintf(sz, "%.2f", fLockValue);
		m_fLockValue = fLockValue;
	}

	int GetHugIndex() { return m_iHugIndex; }
	void SetHugIndex(int newIndex)
	{ 
		m_iHugIndex = newIndex; 
		UpdateData(false);
	}

	bool GetGridCheck()
	{ 
		return (m_CheckGrid.GetCheck() == 1);
	}

	void SetGridCheck(bool checked)
	{
		m_CheckGrid.SetCheck(checked);
	}

	bool GetCollisionCheck()
	{ 
		return (m_EnableCollisions.GetCheck() == 1);
	}

	void SetCollisionCheck(bool checked) 
	{ 
		m_EnableCollisions.SetCheck(checked); 
	}

// Dialog Data
	enum { IDD = IDD_OBJECT_TOOL_PALETTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LONG OnInitDialog( UINT, LONG );

	inline float GetSensitivity() { return m_fSensitivity; }

private:
	CObjectEditor* m_pEditor;

	CButton m_ObjectSelect;
	CButton m_ObjectMove;
	CButton m_ObjectRotate;
	CButton m_ObjectScale;
	CComboBox m_LockMovement;
	CEdit m_LockValue;
	CButton m_LockValueCheck;
	CButton m_ObjectCreate;
	CBitmapButton m_ObjectDelete;
	CComboBox m_HugCombo;
	CButton m_EnableCollisions;
	CBitmapButton m_ObjectZoomTo;
	CEdit m_EditSensitivity;
	CSpinButtonCtrl m_SpinnerSensitivity;
	CButton m_HugNearest;

	CButton m_CheckGrid;
	CEdit m_EditGrid;
	CSpinButtonCtrl m_SpinnerGrid;

	// snap to grid
	float m_fSnapToGrid;

	float m_fSensitivity;
	int m_iLockIndex;
	int	m_iLockPosIndex;
	int	m_iLockRotIndex;
	float m_fLockValue;
	bool m_bButtonRotateClicked;
	bool m_bButtonMoveClicked;
	int m_iHugIndex;

	afx_msg void OnBnClickedObjectEnableCollisions();
	afx_msg void OnBnClickedObjectCheckGrid();
	afx_msg void OnBnClickedObjectPosition();
	afx_msg void OnBnClickedObjectRotation();
	afx_msg void OnGenericUpdateData();

	int m_ObjectManipulateState;
public:
	afx_msg void OnCbnSelchangeLock();
	afx_msg void OnBnClickedLockValueCheck();
	afx_msg void OnEnKilLFocusLockValue();
	afx_msg void OnCbnSelchangeHugnearest();
	afx_msg void OnDeltaposSpinSensitivity(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditSensitivity();
	afx_msg void OnEnKillfocusEditSensitivity();

	afx_msg void OnDeltaposSpinGrid(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditGrid();
};

#endif // #ifndef _OBJECTTOOLPAL_H_
