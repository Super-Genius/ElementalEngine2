//-----------------------------------------------------------------------------
//
// File:          OPTIONINFO.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'OPTIONINFO'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '~OPTIONINFO'
*     Added member 'm_IsDirty'
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




/*@NOTE_4053
Destructor method.
*/
OPTIONINFO::~OPTIONINFO()
{//@CODE_4053
    DestructorInclude();

    // Put in your own code
}//@CODE_4053


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4054
Method which must be called first in a constructor.
*/
void OPTIONINFO::ConstructorInclude()
{
}


/*@NOTE_4055
Method which must be called first in a destructor.
*/
void OPTIONINFO::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
