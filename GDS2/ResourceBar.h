///============================================================================
/// \file		ResourceBar.h
/// \brief		Header file for the Resource Bar
/// \date		05-20-2005
/// \author		Josh Lulewicz
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

#if !defined(AFX_RESOURCEBAR_H__615BEF7C_DCE0_4256_827A_24F31061DCFC__INCLUDED_)
#define AFX_RESOURCEBAR_H__615BEF7C_DCE0_4256_827A_24F31061DCFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResourceBar.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CResourceBar window
using namespace std;

class CResourceDoc;

typedef list<RESOURCECOLLECTION *> RESOURCECOLLECTIONLIST;

class CResourceBar : public CGuiControlBar
{
// Construction
public:
	CResourceBar();

// Attributes
private:

	CResourceOptions *obOptions;

// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResourceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CResourceBar();

// Attributes
private:
	// splitter window
	CSplitterWnd	m_SplitterWnd;

	CResourceDoc	m_ResourceDoc;

	// Generated message map functions
protected:
	//{{AFX_MSG(CResourceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

void GetDLLInterface(const CString& pathName, void *thisPtr);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESOURCEBAR_H__615BEF7C_DCE0_4256_827A_24F31061DCFC__INCLUDED_)
