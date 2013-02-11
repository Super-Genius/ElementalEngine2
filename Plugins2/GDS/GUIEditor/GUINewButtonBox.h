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

#ifndef _C_GUINEWBUTTONBOX_H_
#define _C_GUINEWBUTTONBOX_H_

class CGUIEditorDoc;
// CGUINewButtonBox dialog

class CGUINewButtonBox : public CDialog
{
	DECLARE_DYNAMIC(CGUINewButtonBox)

public:
	CGUINewButtonBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGUINewButtonBox();

// Dialog Data
	enum { IDD = IDD_NEWBUTTON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CGUIEditorDoc *m_Doc;
	CHashString m_szButtonType;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_szButtonName;
	afx_msg void OnBnClickedStandardbt();
	afx_msg void OnBnClickedRadiobt();
	afx_msg void OnBnClickedCheckboxbt();
	afx_msg void OnBnClickedSliderbt();
	void SetDocParent(CGUIEditorDoc *value) { m_Doc = value; }
	void SetButtonName(StdString value)
	{
		m_szButtonName = (const TCHAR*)value;
		UpdateData(false);
	}
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

#endif