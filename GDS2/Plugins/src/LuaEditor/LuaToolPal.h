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

#if !defined(AFX_LUATOOLPAL_H__CDA99BDB_2D5F_479F_BC0B_FCC8383C662F__INCLUDED_)
#define AFX_LUATOOLPAL_H__CDA99BDB_2D5F_479F_BC0B_FCC8383C662F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LuaToolPal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLuaToolPal window


class CLuaToolPal : public CGuiToolBarWnd
{
// Construction
public:
	
	CLuaToolPal();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLuaToolPal)
	//}}AFX_VIRTUAL

// Implementation
public:
	
	virtual ~CLuaToolPal();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLuaToolPal)
	afx_msg void On3dButton();
	afx_msg void OnUpdate3dButton(CCmdUI* pCmdUI);
	afx_msg void OnTestPalButton2();
	afx_msg void OnUpdateTestPalButton2(CCmdUI* pCmdUI);
	afx_msg void OnTestPalButton3();
	afx_msg void OnUpdateTestPalButton3(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LUATOOLPAL_H__CDA99BDB_2D5F_479F_BC0B_FCC8383C662F__INCLUDED_)
