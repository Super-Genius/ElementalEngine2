//-----------------------------------------------------------------------------
//
// File:          CHardwareVertexStream.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CHardwareVertexStream'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'UnlockHardwareStream'
*     Added method 'LockHardwareStream'
*     Added method '~CHardwareVertexStream'
*     Added method 'CHardwareVertexStream'
*     Added method 'CHardwareVertexStream'
*     Added inheritance 'CBaseObject'
*     Added inheritance 'CHardwareStream'
*     Added member 'm_hVertexStream'
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




/*@NOTE_4181
Constructor method.
*/
CHardwareVertexStream::CHardwareVertexStream() //@INIT_4181
: CHardwareStream(), CBaseObject(&GUID_HARDVS) 
{//@CODE_4181
    ConstructorInclude();

    // Put in your own code
}//@CODE_4181


/*@NOTE_4182
Constructor method.
*/
CHardwareVertexStream::CHardwareVertexStream(LPCSTR name) //@INIT_4182
: CHardwareStream(),
			CBaseObject(&GUID_HARDVS, name) 
{//@CODE_4182
    ConstructorInclude();

    // Put in your own code
}//@CODE_4182


/*@NOTE_4175
Destructor method.
*/
CHardwareVertexStream::~CHardwareVertexStream()
{//@CODE_4175
    DestructorInclude();

    // Put in your own code
}//@CODE_4175


void* CHardwareVertexStream::LockHardwareStream()
{//@CODE_4184
}//@CODE_4184


BOOL CHardwareVertexStream::UnlockHardwareStream()
{//@CODE_4185
    BOOL value;

    return value;
}//@CODE_4185


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4176
Method which must be called first in a constructor.
*/
void CHardwareVertexStream::ConstructorInclude()
{
}


/*@NOTE_4177
Method which must be called first in a destructor.
*/
void CHardwareVertexStream::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
