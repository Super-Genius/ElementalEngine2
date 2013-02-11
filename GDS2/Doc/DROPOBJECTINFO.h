//-----------------------------------------------------------------------------
//
// File:          DROPOBJECTINFO.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'DROPOBJECTINFO'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '~DROPOBJECTINFO'
*     Added member 'xy'
*     Added member 'object'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _DROPOBJECTINFO_H
#define _DROPOBJECTINFO_H

//@START_USER1
//@END_USER1



class DROPOBJECTINFO
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
    CBaseObject* object;
    CPoint xy;

//
// Non-Grouped Methods
//

public:
    virtual ~DROPOBJECTINFO();
};

#endif


#ifdef CB_INLINES
#ifndef _DROPOBJECTINFO_H_INLINES
#define _DROPOBJECTINFO_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
