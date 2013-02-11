//-----------------------------------------------------------------------------
//
// File:          lessThan.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'lessThan'
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

//@START_USER1
//@END_USER1


// Master include file
#include "StdAfx.h"


//@START_USER2
//@END_USER2




/*@NOTE_3908
Destructor method.
*/
lessThan::~lessThan()
{//@CODE_3908
    DestructorInclude();

    // Put in your own code
}//@CODE_3908


bool operator lessThan::()(const CDLLInterface* x, const CDLLInterface* y)
{//@CODE_3914
 
		return (x->m_Priority < y->m_Priority); 
   
}//@CODE_3914


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3909
Method which must be called first in a constructor.
*/
void lessThan::ConstructorInclude()
{
}


/*@NOTE_3910
Method which must be called first in a destructor.
*/
void lessThan::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
