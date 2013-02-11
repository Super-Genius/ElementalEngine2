//-----------------------------------------------------------------------------
//
// File:          lessThan.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'lessThan'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '()'
*     Added method '~lessThan'
*     Added inheritance 'greater<CDLLInterface *>'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _LESSTHAN_H
#define _LESSTHAN_H

//@START_USER1
//@END_USER1


/*@NOTE_3907
C:\Development\Game Dev System\gds.cpp(47)

 User-defined predicate function for sorting.
C:\Development\Game Dev System\gds.cpp(47)

 this has to be defined locally or compiler put's it in whoever
C:\Development\Game Dev System\gds.cpp(47)

 includes the header file.  Make sure both functions match.
*/

class lessThan
    : public greater<CDLLInterface *>
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
    bool operator ()(const CDLLInterface* x, const CDLLInterface* y);

public:
    virtual ~lessThan();
};

#endif


#ifdef CB_INLINES
#ifndef _LESSTHAN_H_INLINES
#define _LESSTHAN_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
