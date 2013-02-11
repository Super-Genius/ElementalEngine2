//-----------------------------------------------------------------------------
//
// File:          HIEROBJMESSINFO.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'HIEROBJMESSINFO'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '~HIEROBJMESSINFO'
*     Added member 'enable'
*     Added member 'object'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _HIEROBJMESSINFO_H
#define _HIEROBJMESSINFO_H

//@START_USER1
//@END_USER1



class HIEROBJMESSINFO
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
    BOOL enable;

//
// Non-Grouped Methods
//

public:
    virtual ~HIEROBJMESSINFO();
};

#endif


#ifdef CB_INLINES
#ifndef _HIEROBJMESSINFO_H_INLINES
#define _HIEROBJMESSINFO_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
