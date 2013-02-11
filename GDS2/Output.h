#if !defined(AFX_OUTPUT_H__B2F357EB_3A4A_4B3D_B0E0_C8747D7C290C__INCLUDED_)
#define AFX_OUTPUT_H__B2F357EB_3A4A_4B3D_B0E0_C8747D7C290C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Output.h : header file
//
#include "NotifierPropSheet.h"

/////////////////////////////////////////////////////////////////////////////
// COutput dialog

#include "BuildOutput.h"
#include "DebugOutput.h"
#include "FindOutput.h"
#include "DLLInterface.h"
#include "Messages.h"

class COutput : public CSizingControlBarG
{
// Construction
public:
	COutput();				// standard constructor
	~COutput();				// standard destructor
	
// Dialog Data
	//{{AFX_DATA(COutput)
	enum { IDD = IDD_OUTPUTPANE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// this was created so that messages will come through
	CNotifierPropSheet m_PropSheet;
	CBuildOutput m_BuildOutput;
	CDebugOutput m_DebugOutput;
	CFindOutput m_FindOutput;

	// Generated message map functions
	//{{AFX_MSG(COutput)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnSelectPage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClearPage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPrintLine(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

extern const UINT SelectPageMessage;
extern const UINT ClearPageMessage;
extern const UINT PrintLineMessage;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_OUTPUT_H__B2F357EB_3A4A_4B3D_B0E0_C8747D7C290C__INCLUDED_)
