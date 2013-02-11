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

#ifndef _C_GUISETBGCOLORBOX_H_
#define _C_GUISETBGCOLORBOX_H_

class CGUIEditorDoc;
// CGUISetBGColorBox dialog

class CGUISetBGColorBox : public CDialog
{
	DECLARE_DYNAMIC(CGUISetBGColorBox)

public:
	CGUISetBGColorBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGUISetBGColorBox();

// Dialog Data
	enum { IDD = IDD_SETBGCOLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CGUIEditorDoc *m_Doc;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void SetDocParent(CGUIEditorDoc *value) { m_Doc = value; }
	CComboBox m_ctColorCombo;
	CString m_szColorChoice;
	UINT m_iRVal;
	UINT m_iGVal;
	UINT m_iBVal;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCbnSelchangeColors();
};

#endif
