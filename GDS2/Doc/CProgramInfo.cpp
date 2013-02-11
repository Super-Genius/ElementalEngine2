//-----------------------------------------------------------------------------
//
// File:          CProgramInfo.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CProgramInfo'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'BroadcastMessage'
*     Added method '(BOOL (*srcBroadcastCBPtr)'
*     Added method '(*BroadcastCBPtr)'
*     Added method '~CProgramInfo'
*     Added member 'ObjectManager'
*     Added member 'AppDirectory'
*     Added member 'curAccel'
*     Added member 'curMenu'
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




/*@NOTE_4136
Destructor method.
*/
CProgramInfo::~CProgramInfo()
{//@CODE_4136
    DestructorInclude();

    // Put in your own code
}//@CODE_4136


BOOL CProgramInfo::(*BroadcastCBPtr)(UINT nID, void* pExtra)
{//@CODE_4140
    BOOL value;

    return value;
}//@CODE_4140


CProgramInfo CProgramInfo::(BOOL (*srcBroadcastCBPtr)(UINT nID, void* pExtra)
{//@CODE_4150

		// save off pointer to broadcast member function
		BroadcastCBPtr = srcBroadcastCBPtr;
}//@CODE_4150


BOOL CProgramInfo::BroadcastMessage(UINT nID, void* pExtra)
{//@CODE_4147

		// make a C++ callback to a static function
		return (*BroadcastCBPtr)(nID, pExtra);
}//@CODE_4147


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4137
Method which must be called first in a constructor.
*/
void CProgramInfo::ConstructorInclude()
{
}


/*@NOTE_4138
Method which must be called first in a destructor.
*/
void CProgramInfo::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
