//-----------------------------------------------------------------------------
//
// File:          CPlane.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CPlane'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'operator[]'
*     Added method 'operator/'
*     Added method 'operator/'
*     Added method 'operator-'
*     Added method 'operator-'
*     Added method 'operator+'
*     Added method 'operator+'
*     Added method 'operator*'
*     Added method 'operator*'
*     Added method 'operator*'
*     Added method 'operator!'
*     Added method 'operator'
*     Added method 'operator'
*     Added method 'operator'
*     Added method '~CPlane'
*     Added method 'CPlane'
*     Added method 'CPlane'
*     Added method 'CPlane'
*     Added method 'CPlane'
*     Added inheritance 'CBaseObject'
*     Added member 'D'
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




/*@NOTE_4472
Constructor method.
*/
CPlane::CPlane() //@INIT_4472
: CBaseObject(&GUID_PLANE) 
{//@CODE_4472
    ConstructorInclude();

    // Put in your own code
}//@CODE_4472


/*@NOTE_4473
Constructor method.
*/
CPlane::CPlane(LPCSTR name) //@INIT_4473
: CBaseObject(&GUID_PLANE, name) 
{//@CODE_4473
    ConstructorInclude();

    // Put in your own code
}//@CODE_4473


/*@NOTE_4475
Constructor method.
*/
CPlane::CPlane(CPlane& src) //@INIT_4475
: CBaseObject(&GUID_PLANE) 
{//@CODE_4475
    ConstructorInclude();

    // Put in your own code
}//@CODE_4475


/*@NOTE_4477
Constructor method.
*/
CPlane::CPlane(LPCSTR name, CPlane& src) //@INIT_4477
: CBaseObject(&GUID_PLANE, name) 
{//@CODE_4477
    ConstructorInclude();

    // Put in your own code
}//@CODE_4477


/*@NOTE_4466
Destructor method.
*/
CPlane::~CPlane()
{//@CODE_4466
    DestructorInclude();

    // Put in your own code
}//@CODE_4466


void CPlane::operator(CMatrix4& dest)
{//@CODE_4496
}//@CODE_4496


CPlane& CPlane::operator(CPlane& src)
{//@CODE_4501
    CPlane value;

    return value;
}//@CODE_4501


BOOL CPlane::operator(CPlane& src)
{//@CODE_4503
    BOOL value;

    return value;
}//@CODE_4503


BOOL CPlane::operator!(CPlane& src)
{//@CODE_4505
    BOOL value;

    return value;
}//@CODE_4505


CPlane& CPlane::operator*(CPlane& src)
{//@CODE_4484
    CPlane value;

    return value;
}//@CODE_4484


CPlane& CPlane::operator*(CPlane& src)
{//@CODE_4492
    CPlane value;

    return value;
}//@CODE_4492


float CPlane::operator*()
{//@CODE_4500
    float value;

    return value;
}//@CODE_4500


CPlane& CPlane::operator+(CPlane& src)
{//@CODE_4480
    CPlane value;

    return value;
}//@CODE_4480


CPlane& CPlane::operator+(CPlane& src)
{//@CODE_4488
    CPlane value;

    return value;
}//@CODE_4488


CPlane& CPlane::operator-(CPlane& src)
{//@CODE_4482
    CPlane value;

    return value;
}//@CODE_4482


CPlane& CPlane::operator-(CPlane& src)
{//@CODE_4490
    CPlane value;

    return value;
}//@CODE_4490


CPlane& CPlane::operator/(CPlane& src)
{//@CODE_4486
    CPlane value;

    return value;
}//@CODE_4486


CPlane& CPlane::operator/(CPlane& src)
{//@CODE_4494
    CPlane value;

    return value;
}//@CODE_4494


float CPlane::operator[](int index)
{//@CODE_4498
    float value;

    return value;
}//@CODE_4498


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4467
Method which must be called first in a constructor.
*/
void CPlane::ConstructorInclude()
{
}


/*@NOTE_4468
Method which must be called first in a destructor.
*/
void CPlane::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
