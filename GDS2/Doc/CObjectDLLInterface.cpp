//-----------------------------------------------------------------------------
//
// File:          CObjectDLLInterface.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CObjectDLLInterface'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'Save'
*     Added method 'Load'
*     Added method '~CObjectDLLInterface'
*     Added inheritance 'CDLLInterface'
*     Added member 'm_ThumbNail'
*     Added member 'm_ObjInfo'
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




/*@NOTE_4455
Destructor method.
*/
CObjectDLLInterface::~CObjectDLLInterface()
{//@CODE_4455
    DestructorInclude();

    // Put in your own code
}//@CODE_4455


DWORD CObjectDLLInterface::Load(LPTSTR filename)
{//@CODE_4461

		return 0;
}//@CODE_4461


DWORD CObjectDLLInterface::Save(LPTSTR filename)
{//@CODE_4463

		return 0;
}//@CODE_4463


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4456
Method which must be called first in a constructor.
*/
void CObjectDLLInterface::ConstructorInclude()
{
}


/*@NOTE_4457
Method which must be called first in a destructor.
*/
void CObjectDLLInterface::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
