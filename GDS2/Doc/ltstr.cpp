//-----------------------------------------------------------------------------
//
// File:          ltstr.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'ltstr'
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

//@START_USER1
//@END_USER1


// Master include file
#include "StdAfx.h"


//@START_USER2
//@END_USER2




/*@NOTE_3667
Destructor method.
*/
ltstr::~ltstr()
{//@CODE_3667
    DestructorInclude();

    // Put in your own code
}//@CODE_3667


bool operator ltstr::()(LPCSTR s1, LPCSTR s2)
{//@CODE_3672

    return strcmp(s1, s2) < 0;
 
}//@CODE_3672


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3668
Method which must be called first in a constructor.
*/
void ltstr::ConstructorInclude()
{
}


/*@NOTE_3669
Method which must be called first in a destructor.
*/
void ltstr::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
