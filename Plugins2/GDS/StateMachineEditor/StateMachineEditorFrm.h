///============================================================================
/// \file		StateMachineEditorFrm.h
/// \brief		Header file for State Machine Editor form class
/// \date		03-05-2007
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

#if !defined(AFX_STATEMACHINEEDITORFRM_H__D0F99C6B_8296_469D_9CCD_028B2E15383F__INCLUDED_)
#define AFX_STATEMACHINEEDITORFRM_H__D0F99C6B_8296_469D_9CCD_028B2E15383F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStateMachineEditorFrm frame


class CStateMachineEditorFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CStateMachineEditorFrm)
protected:
	
	CStateMachineEditorFrm();           // protected constructor used by dynamic creation

// Attributes
public:
	
	IToolBox *		m_ToolBox;
	
	HINSTANCE m_AppInst;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStateMachineEditorFrm)
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	virtual ~CStateMachineEditorFrm();

	// Generated message map functions
	//{{AFX_MSG(CStateMachineEditorFrm)
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATEMACHINEEDITORFRM_H__D0F99C6B_8296_469D_9CCD_028B2E15383F__INCLUDED_)
