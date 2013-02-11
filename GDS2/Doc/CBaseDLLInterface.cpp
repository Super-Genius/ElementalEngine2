//-----------------------------------------------------------------------------
//
// File:          CBaseDLLInterface.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CBaseDLLInterface'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '~CBaseDLLInterface'
*     Added inheritance 'CDLLInterface'
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




/*@NOTE_3937
Destructor method.
*/
CBaseDLLInterface::~CBaseDLLInterface()
{//@CODE_3937
    DestructorInclude();

    // Put in your own code
}//@CODE_3937


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3938
Method which must be called first in a constructor.
*/
void CBaseDLLInterface::ConstructorInclude()
{
}


/*@NOTE_3939
Method which must be called first in a destructor.
*/
void CBaseDLLInterface::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
