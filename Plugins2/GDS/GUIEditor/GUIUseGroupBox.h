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

#ifndef _C_GUIUSEGROUPBOX_H_
#define _C_GUIUSEGROUPBOX_H_


// CGUIUseGroupBox dialog

class CGUIEditorDoc;

class CGUIUseGroupBox : public CDialog
{
	DECLARE_DYNAMIC(CGUIUseGroupBox)

public:
	CGUIUseGroupBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGUIUseGroupBox();

// Dialog Data
	enum { IDD = IDD_USEGROUPBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CGUIEditorDoc *m_Doc;
	vector<CHashString> m_vGroupNames;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void SetDocParent(CGUIEditorDoc *value) { m_Doc = value; }
	void AddGroupName(CHashString name) { m_vGroupNames.push_back(name); }
	CComboBox m_ctUseGroup;
	CString m_szGroupName;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

#endif