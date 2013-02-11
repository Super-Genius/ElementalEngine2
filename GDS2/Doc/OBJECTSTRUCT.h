//-----------------------------------------------------------------------------
//
// File:          OBJECTSTRUCT.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'OBJECTSTRUCT'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '~OBJECTSTRUCT'
*     Added member 'object'
*     Added member 'rsrcIndex'
*     Added member 'pageIndex'
*     Added member 'fileName'
*     Added member 'name'
*     Added member 'thumbNail'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _OBJECTSTRUCT_H
#define _OBJECTSTRUCT_H

//@START_USER1
//@END_USER1



class OBJECTSTRUCT
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
    CBitmap* thumbNail;
    CString* name;
    CString* fileName;
    UINT* pageIndex;
    UINT* rsrcIndex;
    CBaseObject* object;

//
// Non-Grouped Methods
//

public:
    virtual ~OBJECTSTRUCT();
};

#endif


#ifdef CB_INLINES
#ifndef _OBJECTSTRUCT_H_INLINES
#define _OBJECTSTRUCT_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
