//-----------------------------------------------------------------------------
//
// File:          ltstr.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'ltstr'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '()'
*     Added method '~ltstr'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _LTSTR_H
#define _LTSTR_H

//@START_USER1
//@END_USER1



class ltstr
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

private:
    bool operator ()(LPCSTR s1, LPCSTR s2);

public:
    virtual ~ltstr();
};

#endif


#ifdef CB_INLINES
#ifndef _LTSTR_H_INLINES
#define _LTSTR_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
