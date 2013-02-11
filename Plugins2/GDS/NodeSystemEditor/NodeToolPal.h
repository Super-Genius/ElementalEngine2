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

#ifndef _NODETOOLPAL_H
#define _NODETOOLPAL_H

class CNodeSystemEditor;

// CNodeToolPal dialog
class CNodeToolPal : public CGuiDialogBar
{
	DECLARE_DYNAMIC(CNodeToolPal)

public:
	CNodeToolPal();   // standard constructor
	virtual ~CNodeToolPal();

	void SetParent( CNodeSystemEditor *parent );   // standard constructor
	void ObjectSelected( IHashString *hszObjectName, IHashString *hszComponentType );

// Dialog Data
	enum { IDD = IDD_NODE_TOOL_PALETTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

	afx_msg LONG OnInitDialog ( UINT, LONG );
	afx_msg void OnCheck1Clicked();
	afx_msg void OnCheck2Clicked();
	afx_msg void OnUpdateButton1(CCmdUI* pCmdUI);
	afx_msg void OnButton1Clicked();

	CButton m_Button1;

private:
	CNodeSystemEditor *m_Parent;
};

#endif		// #ifndef _NODETOOLPAL_H