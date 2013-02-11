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

#ifndef _C_GUIGROUPNAMEBOX_H_
#define _C_GUIGROUPNAMEBOX_H_

class CGUIEditorDoc;

class CGUIGroupNameBox : public CDialog
{
	DECLARE_DYNAMIC(CGUIGroupNameBox)

public:
	CGUIGroupNameBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGUIGroupNameBox();

// Dialog Data
	enum { IDD = IDD_GROUPNAMEBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CGUIEditorDoc *m_Doc;
	HTREEITEM page;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void SetDocParent(CGUIEditorDoc *value) { m_Doc = value; }
	void SetGroupName(StdString name)
	{
		m_szGroupName = (const TCHAR*)name;
		UpdateData(false);		
	}
	CString m_szGroupName;

};

#endif