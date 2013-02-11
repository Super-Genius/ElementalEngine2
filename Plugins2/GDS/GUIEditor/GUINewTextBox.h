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

#ifndef _C_GUINEWTEXTBOX_H_
#define _C_GUINEWTEXTBOX_H_

class CGUIEditorDoc;
// CGUINewTextBox dialog

class CGUINewTextBox : public CDialog
{
	DECLARE_DYNAMIC(CGUINewTextBox)

public:
	CGUINewTextBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGUINewTextBox();

// Dialog Data
	enum { IDD = IDD_NEWTEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CHashString m_szTextType;
	CGUIEditorDoc *m_Doc;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_szTextName;
	afx_msg void OnBnClickedStatictextbt();
	afx_msg void OnBnClickedEdittextbt();
	afx_msg void OnBnClickedListboxbt();
	afx_msg void OnBnClickedComboboxbt();
	afx_msg void OnBnClickedSpinnerbt();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void SetDocParent(CGUIEditorDoc *value) { m_Doc = value; }
	void SetTextName(StdString name);
};

#endif
