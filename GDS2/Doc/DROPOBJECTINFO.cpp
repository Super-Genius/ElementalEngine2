//-----------------------------------------------------------------------------
//
// File:          DROPOBJECTINFO.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'DROPOBJECTINFO'
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

//@START_USER1
//@END_USER1


// Master include file
#include "StdAfx.h"


//@START_USER2
//@END_USER2




/*@NOTE_4430
Destructor method.
*/
DROPOBJECTINFO::~DROPOBJECTINFO()
{//@CODE_4430
    DestructorInclude();

    // Put in your own code
}//@CODE_4430


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4431
Method which must be called first in a constructor.
*/
void DROPOBJECTINFO::ConstructorInclude()
{
}


/*@NOTE_4432
Method which must be called first in a destructor.
*/
void DROPOBJECTINFO::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
