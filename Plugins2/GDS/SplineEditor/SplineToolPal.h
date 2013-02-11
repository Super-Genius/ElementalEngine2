///============================================================================
/// \file		SplineComponent.cpp
/// \brief		Header file for Spline Toolbar
/// \date		11-14-2006
/// \author		Richard Nguyen
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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSplineToolPal window


class CSplineToolPal : public CGuiToolBarWnd
{
// Construction
public:
	
	CSplineToolPal();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplineToolPal)
	//}}AFX_VIRTUAL

// Implementation
public:
	
	virtual ~CSplineToolPal();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSplineToolPal)
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
