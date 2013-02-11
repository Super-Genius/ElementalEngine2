///============================================================================
/// \file		SplineComponent.cpp
/// \brief		Header file for Spline Form
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
// CSplineFrm frame


class CSplineFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSplineFrm)
protected:
	
	CSplineFrm();           // protected constructor used by dynamic creation

// Attributes
public:
	
	CSplineToolPal m_SplineToolPal;
	
	HINSTANCE m_AppInst;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplineFrm)
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	virtual ~CSplineFrm();

	// Generated message map functions
	//{{AFX_MSG(CSplineFrm)
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
