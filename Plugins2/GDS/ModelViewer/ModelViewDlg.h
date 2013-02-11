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

#ifndef _CMODELVIEWDLG_H
#define _CMODELVIEWDLG_H

// CModelViewDlg dialog

class CModelViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CModelViewDlg)

public:
	CModelViewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModelViewDlg();

	void OnStopSequence();

	CString GetAnimationPath(int nAnimationId);
	void SetDlgItemFloat(int nDlgItemId, float fValue);
	bool GetDlgItemFloat(int nDlgItemId, float &fValue, bool bCheckDegrees);
	void SetPosition(const Vec3 &position);
	void GetPosition(Vec3 &position);
	void SetRotation(const EulerAngle &rotation);
	void GetRotation(EulerAngle &rotation);
	void SetAnimationList(const ANIMATIONLIST *pAnimationList);
	bool IsAnimationSelected() const;
	int GetCurrentAnimationId() const;
	void SetCurrentAnimationId(int nAnimationId);
	bool IsSequencePresent() const;
	void SetAnimationSequence(const ANIMATIONSEQUENCE *pAnimationSequence);
	void GetAnimationSequence(ANIMATIONSEQUENCE &animationSequence) const;

// Dialog Data
	enum { IDD = IDD_MODELVIEWDLG };

private:
	bool m_bPlayingSelected;
	bool m_bPlayingSequence;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_AddAnimationButton;
	CButton m_RemoveAnimationButton;
	CButton m_ClearAnimationsButton;
	CButton m_PlaySelectionButton;
	CButton m_PlaySequenceButton;
	CDragListBox m_AnimationListBox;
	CComboBox m_AnimationComboBox;
	CButton m_ResetPositionButton;
	CButton m_ResetRotationButton;
	virtual LRESULT OnInitDialog(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedModelviewTbButtonAddanimation();
	afx_msg void OnBnClickedModelviewTbButtonAnimplay();
	afx_msg void OnBnClickedModelviewTbButtonClearanimationlist();
	afx_msg void OnBnClickedModelviewTbButtonPlaysequence();
	afx_msg void OnBnClickedModelviewTbButtonRemoveanim();
	afx_msg void OnBnClickedModelviewTbButtonResetLookat();
	afx_msg void OnBnClickedModelviewTbButtonResetRotation();
	afx_msg void OnEnChangeModelviewTbEditLookoffset();
	afx_msg void OnEnChangeModelviewTbEditRotation();
	afx_msg void OnUpdateAddAnimation(CCmdUI* pCmdUI);
	afx_msg void OnUpdateClearAnimationSequence(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRemoveAnimation(CCmdUI* pCmdUI);
};

#endif //#ifndef _CMODELVIEWDLG_H
