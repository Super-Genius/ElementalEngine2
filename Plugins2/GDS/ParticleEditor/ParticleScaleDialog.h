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
#include "resource.h"

// CParticleScaleDialog dialog

class CParticleScaleDialog : public CDialog
{
	DECLARE_DYNAMIC(CParticleScaleDialog)

public:
	CParticleScaleDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CParticleScaleDialog();

// Dialog Data
	enum { IDD = IDD_PARTICLE_SCALEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CEdit m_EditScale;
	StdString m_szInitialScale;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeScaledlgedit();
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
