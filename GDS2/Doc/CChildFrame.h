//-----------------------------------------------------------------------------
//
// File:          CChildFrame.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CChildFrame'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'PreCreateWindow'
*     Added method 'Dump'
*     Added method 'AssertValid'
*     Added method '~CChildFrame'
*     Added method 'CChildFrame'
*     Added inheritance 'CMDIChildWnd'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _CCHILDFRAME_H
#define _CCHILDFRAME_H

//@START_USER1
//@END_USER1



class CChildFrame
    : public CMDIChildWnd
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

//
// Non-Grouped Methods
//

public:
    CChildFrame();
    virtual ~CChildFrame();
    void AssertValid();
    void Dump(CDumpContext& dc);
    BOOL PreCreateWindow(CREATESTRUCT& cs);
};

#endif


#ifdef CB_INLINES
#ifndef _CCHILDFRAME_H_INLINES
#define _CCHILDFRAME_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
