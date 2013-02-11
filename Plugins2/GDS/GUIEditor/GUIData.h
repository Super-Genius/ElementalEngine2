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


// CGUIData dialog

class CGUIData : public CDialog
{
	DECLARE_DYNAMIC(CGUIData)

public:
	CGUIData(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGUIData();

// Dialog Data
	enum { IDD = IDD_GUIEDDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	vector<StdString> m_vResolutions;
	CGUIEditorDoc *m_Doc;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	void SetMousePosition(int x, int y);
	void SetZoom(float z);
	void AddResolution(UINT x, UINT y);
	void SetResolution(UINT x, UINT y);
	void SetDocParent(CGUIEditorDoc *value) { m_Doc = value; }
	CComboBox m_ResCombo;	
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeRescombo();
};
