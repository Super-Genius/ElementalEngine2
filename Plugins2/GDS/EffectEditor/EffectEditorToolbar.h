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
#include "afxwin.h"


// CEffectEditorToolbar dialog

class CEffectEditorToolbar : public CGuiDialogBar
//class CEffectEditorToolbar : public CDialog
{
	DECLARE_DYNAMIC(CEffectEditorToolbar)

public:
	CEffectEditorToolbar();   // standard constructor
	virtual ~CEffectEditorToolbar();

// Dialog Data
	enum { IDD = IDD_TOOLBAR_EFFECTEDITOR };

protected:
	IToolBox *m_ToolBox;
	CEffectEditor *m_pEditor;

	NAMETOEFFECTMAP m_EffectMap;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnInitDialog(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedCheckCameralight();
	afx_msg void OnBnClickedCheckGround();
	afx_msg void OnBnClickedCheckScenelight();
	afx_msg void OnBnClickedCheckEffectmodel();
	afx_msg void OnCbnSelchangeComboEffects();
protected:
	CComboBox m_ComboEffects;
public:
	afx_msg void OnCbnDropdownComboEffects();
};
