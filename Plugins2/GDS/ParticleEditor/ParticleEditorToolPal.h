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

#ifndef PARTICLEEDITORTOOLPAL_H
#define PARTICLEEDITORTOOLPAL_H

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
// ParticleEditorToolPal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParticleEditorToolPal window

class CParticleEditorToolPal : public CGuiDialogBar
{
	DECLARE_DYNAMIC(CParticleEditorToolPal);
// Construction
public:
	CParticleEditorToolPal();
	virtual ~CParticleEditorToolPal();

	enum { IDD = IDD_PARTICLE_TOOL_PALETTE };

protected:
	// Generated message map functions
	virtual LONG OnInitDialog ( UINT, LONG );
	virtual void DoDataExchange(CDataExchange* pDX);

	afx_msg void OnButtonShowParticleClicked();
	afx_msg void OnButtonMoveParticleClicked();
	afx_msg void OnButtonAddBrushClicked();
	afx_msg void OnEnChangeSpeedvalue();
	afx_msg void OnBnClickedParticlesScale();
	afx_msg void OnUpdateButtons( CCmdUI* pCmdUI ) {}

//protected:
	DECLARE_MESSAGE_MAP()
public:
	void SetData(CParticleEditorDataExchange * pParticleToolData);
	CParticleEditorDataExchange * GetData(void);
	void ResetToolBar();

	bool AreParticlesShown() const;
	void ShowParticles(bool bShow);
	float GetMoveSpeed() const;
	void SetMoveSpeed(float fMoveSpeed);

// Attributes
private:
	bool m_bRenderActive;
	bool m_bActive;

	CButton m_ButtonShowParticles;
	CButton m_ButtonMoveParticles;
	CButton m_ButtonAddBrush;
	CButton m_ButtonScaleParticle;
	CEdit m_EditMoveSpeed;
	CParticleEditorDataExchange * m_pControlData;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* PARTICLEEDITORTOOLPAL_H */