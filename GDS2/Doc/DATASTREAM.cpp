//-----------------------------------------------------------------------------
//
// File:          DATASTREAM.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'DATASTREAM'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '~DATASTREAM'
*     Added member 'm_Description'
*     Added member 'm_Data'
*     Added member 'm_Count'
*     Added member 'm_type'
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




/*@NOTE_4384
Destructor method.
*/
DATASTREAM::~DATASTREAM()
{//@CODE_4384
    DestructorInclude();

    // Put in your own code
}//@CODE_4384


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4385
Method which must be called first in a constructor.
*/
void DATASTREAM::ConstructorInclude()
{
}


/*@NOTE_4386
Method which must be called first in a destructor.
*/
void DATASTREAM::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
