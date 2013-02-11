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
#ifndef _PARTICLERENAMEDIALOG_H__
#define _PARTICLERENAMEDIALOG_H__

// CParticleRenameDialog dialog

class CParticleRenameDialog : public CDialog
{
	DECLARE_DYNAMIC(CParticleRenameDialog)

public:
	CParticleRenameDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CParticleRenameDialog();

// Dialog Data
	enum { IDD = IDD_PARTICLE_RENAME_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CEdit m_EditRename;
	int m_iBrushNum;
	StdString m_szInitalpath;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeParticleRenameEdit();
	virtual BOOL OnInitDialog();
	void SetBrushNumber(int brushnum);

protected:
	virtual void OnOK();
};

#endif // _PARTICLERENAMEDIALOG_H__