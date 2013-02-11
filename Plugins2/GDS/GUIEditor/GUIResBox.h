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

#ifndef _C_GUIRESBOX_H_
#define _C_GUIRESBOX_H_


// CGUIResBox dialog

class CGUIEditor;

class CGUIResBox : public CDialog
{
	DECLARE_DYNAMIC(CGUIResBox)

public:
	CGUIResBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGUIResBox();

// Dialog Data
	enum { IDD = IDD_RESBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()	
	CGUIEditor *m_pEditorParent;
public:
	afx_msg void OnBnClickedOk();
	UINT m_iXRes;
	UINT m_iYRes;
	void SetEditorParent(CGUIEditor *value) { m_pEditorParent = value; }
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

#endif