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

#ifndef _C_GUIUSEITEMBOX_H_
#define _C_GUIUSEITEMBOX_H_

class CGUIEditorDoc;
// CGUIUseItemBox dialog

class CGUIUseItemBox : public CDialog
{
	DECLARE_DYNAMIC(CGUIUseItemBox)

public:
	CGUIUseItemBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGUIUseItemBox();

// Dialog Data
	enum { IDD = IDD_USEITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CGUIEditorDoc *m_Doc;
	vector<CHashString> m_vNames;
	vector<CHashString> m_vTypes;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void SetDocParent(CGUIEditorDoc *value) { m_Doc = value; }
	void AddItemToList(CHashString name, CHashString type)
	{
		m_vNames.push_back(name);
		m_vTypes.push_back(type);
	}
	void RemoveItemFromList(CHashString name);
	CComboBox m_ctUseItem;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CString m_szItemName;
};

#endif