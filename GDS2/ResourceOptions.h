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

#if !defined(AFX_RESOURCEOPTIONS_H__DDECD0D6_10B8_4055_8D95_B769489058F7__INCLUDED_)
#define AFX_RESOURCEOPTIONS_H__DDECD0D6_10B8_4055_8D95_B769489058F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResourceOptions.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResourceOptions dialog

class CResourceOptions : public CPropertyPage
{
// Construction
public:
	CResourceOptions();   // standard constructor
	virtual BOOL OnApply();
	virtual void OnOK();
	virtual void OnCancel();

// Dialog Data
	//{{AFX_DATA(CResourceOptions)
	enum { IDD = IDD_OBJECT_OPTIONS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResourceOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CResourceOptions)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCullobjectsflag();
	afx_msg void OnEnKillfocusCullobjectsdistance();
	
	
	int m_iCullDistance;
	CEdit m_ctrlCullObjectDistance;
	CButton m_ctrlCullObjectFlag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESOURCEOPTIONS_H__DDECD0D6_10B8_4055_8D95_B769489058F7__INCLUDED_)
