//-----------------------------------------------------------------------------
//
// File:          CHardwareIndexStream.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CHardwareIndexStream'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'UnlockHardwareStream'
*     Added method 'LockHardwareStream'
*     Added method '~CHardwareIndexStream'
*     Added method 'CHardwareIndexStream'
*     Added method 'CHardwareIndexStream'
*     Added inheritance 'CBaseObject'
*     Added inheritance 'CHardwareStream'
*     Added member 'm_hIndexStream'
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




/*@NOTE_4192
Constructor method.
*/
CHardwareIndexStream::CHardwareIndexStream() //@INIT_4192
: CHardwareStream(), CBaseObject(&GUID_HARDVS) 
{//@CODE_4192
    ConstructorInclude();

    // Put in your own code
}//@CODE_4192


/*@NOTE_4193
Constructor method.
*/
CHardwareIndexStream::CHardwareIndexStream(LPCSTR name) //@INIT_4193
: CHardwareStream(),
			CBaseObject(&GUID_HARDVS, name) 
{//@CODE_4193
    ConstructorInclude();

    // Put in your own code
}//@CODE_4193


/*@NOTE_4187
Destructor method.
*/
CHardwareIndexStream::~CHardwareIndexStream()
{//@CODE_4187
    DestructorInclude();

    // Put in your own code
}//@CODE_4187


void* CHardwareIndexStream::LockHardwareStream()
{//@CODE_4195
}//@CODE_4195


BOOL CHardwareIndexStream::UnlockHardwareStream()
{//@CODE_4196
    BOOL value;

    return value;
}//@CODE_4196


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4188
Method which must be called first in a constructor.
*/
void CHardwareIndexStream::ConstructorInclude()
{
}


/*@NOTE_4189
Method which must be called first in a destructor.
*/
void CHardwareIndexStream::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
