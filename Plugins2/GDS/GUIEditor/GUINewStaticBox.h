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

#ifndef _C_GUINEWSTATIC_H_
#define _C_GUINEWSTATIC_H_

class CGUIEditorDoc;
// CGUINewStaticBox dialog

class CGUINewStaticBox : public CDialog
{
	DECLARE_DYNAMIC(CGUINewStaticBox)

public:
	CGUINewStaticBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGUINewStaticBox();

// Dialog Data
	enum { IDD = IDD_NEWSTATIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CGUIEditorDoc *m_Doc;	
	CHashString m_szStaticType;

	DECLARE_MESSAGE_MAP()
public:
	CString m_szStaticName;
	afx_msg void OnBnClickedStaticartbt();
	afx_msg void OnBnClickedStatusbarbt();
	afx_msg void OnBnClickedCursorbt();
	afx_msg void OnBnClickedOk();
	void SetDocParent(CGUIEditorDoc *value) { m_Doc = value; }
	void SetStaticName(StdString name) 
	{
		m_szStaticName = (const TCHAR*)name;
		UpdateData(false);
	}
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedMoviebt();
};

#endif