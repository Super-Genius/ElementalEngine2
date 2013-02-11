//-----------------------------------------------------------------------------
//
// File:          OBJECTINFO.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'OBJECTINFO'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '~OBJECTINFO'
*     Added member 'pageIndex'
*     Added member 'objFlags'
*     Added member 'priority'
*     Added member 'icon'
*     Added member 'name'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _OBJECTINFO_H
#define _OBJECTINFO_H

//@START_USER1
//@END_USER1



class OBJECTINFO
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

private:
    CString name;
    HICON icon;
    DWORD priority;
    DWORD objFlags;
    UINT* pageIndex;

//
// Non-Grouped Methods
//

public:
    virtual ~OBJECTINFO();
};

#endif


#ifdef CB_INLINES
#ifndef _OBJECTINFO_H_INLINES
#define _OBJECTINFO_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
