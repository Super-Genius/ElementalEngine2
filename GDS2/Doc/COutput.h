//-----------------------------------------------------------------------------
//
// File:          COutput.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'COutput'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'OnSize'
*     Added method 'OnSelectPage'
*     Added method 'OnPrintLine'
*     Added method 'OnCreate'
*     Added method 'OnClearPage'
*     Added method 'DoDataExchange'
*     Added method '~COutput'
*     Added method 'COutput'
*     Added inheritance 'CSizingControlBarG'
*     Added member 'm_FindOutput'
*     Added member 'm_DebugOutput'
*     Added member 'm_BuildOutput'
*     Added member 'm_PropSheet'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _COUTPUT_H
#define _COUTPUT_H

//@START_USER1
//@END_USER1



class COutput
    : public CSizingControlBarG
{

//@START_USER2
//@END_USER2

//
// Group: ClassBuilder methods
//

private:
    void ConstructorInclude();
    void DestructorInclude();

//
// Non-Grouped Members
//

protected:
    CNotifierPropSheet m_PropSheet;
    CBuildOutput m_BuildOutput;
    CDebugOutput m_DebugOutput;
    CFindOutput m_FindOutput;

//
// Non-Grouped Methods
//

protected:
    void DoDataExchange(CDataExchange* pDX);
    afx_msg LRESULT OnClearPage(WPARAM wParam, LPARAM lParam);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT OnPrintLine(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSelectPage(WPARAM wParam, LPARAM lParam);
    afx_msg void OnSize(UINT nType, int cx, int cy);

public:
    COutput();
    virtual ~COutput();
};

#endif


#ifdef CB_INLINES
#ifndef _COUTPUT_H_INLINES
#define _COUTPUT_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
