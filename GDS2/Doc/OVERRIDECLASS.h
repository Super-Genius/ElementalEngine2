//-----------------------------------------------------------------------------
//
// File:          OVERRIDECLASS.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'OVERRIDECLASS'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '~OVERRIDECLASS'
*     Added member 'overrideClass'
*     Added member 'className'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _OVERRIDECLASS_H
#define _OVERRIDECLASS_H

//@START_USER1
//@END_USER1


/*@NOTE_4092
C:\Development\Game Dev System\Plugins\src\include\DLLInterface.h(12)

 structure to override objects CreateInstance calls
*/

class OVERRIDECLASS
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
    LPCTSTR className;
    PFNCLASSOVERRIDE overrideClass;

//
// Non-Grouped Methods
//

public:
    virtual ~OVERRIDECLASS();
};

#endif


#ifdef CB_INLINES
#ifndef _OVERRIDECLASS_H_INLINES
#define _OVERRIDECLASS_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
