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


// CLightMapEditorDialog dialog

class CLightMapEditorDialog : public CDialog
{
	DECLARE_DYNAMIC(CLightMapEditorDialog)

public:
	CLightMapEditorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLightMapEditorDialog();

// Dialog Data
	enum { IDD = IDD_LIGHTMAPEDITORDIALOG };

	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_NumPhotons;
	CEdit m_Bounces;
	CEdit m_PhotonSamples;
	CEdit m_PhotonSampleRange;
	CEdit m_ShadowEpsilon;
	CEdit m_EnergyCutoff;
	CEdit m_UnitsPerTexel;
	CEdit m_MinTextureSize;
	CEdit m_MaxTextureSize;
	CEdit m_SparseDataRange;
	CEdit m_BlurDirect;
	CEdit m_BlurIndirect;
	CEdit m_OutputDirectory;
	CButton	m_DirectCheck;
	afx_msg void OnBnClickedButtonGenerate();
	afx_msg void OnBnClickedUseindirect();
};
