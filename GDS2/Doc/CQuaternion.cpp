//-----------------------------------------------------------------------------
//
// File:          CQuaternion.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CQuaternion'
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
*     Added method '~CQuaternion'
*     Added method 'CQuaternion'
*     Added method 'CQuaternion'
*     Added method 'CQuaternion'
*     Added method 'CQuaternion'
*     Added inheritance 'CBaseObject'
*     Added member 'm_QuatAxis'
*     Added member 'm_Quaternion[3][3]'
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




/*@NOTE_4514
Constructor method.
*/
CQuaternion::CQuaternion() //@INIT_4514
: CBaseObject(&GUID_QUATERNION) 
{//@CODE_4514
    ConstructorInclude();

    // Put in your own code
}//@CODE_4514


/*@NOTE_4515
Constructor method.
*/
CQuaternion::CQuaternion(LPCSTR name) //@INIT_4515
: CBaseObject(&GUID_QUATERNION, name) 
{//@CODE_4515
    ConstructorInclude();

    // Put in your own code
}//@CODE_4515


/*@NOTE_4517
Constructor method.
*/
CQuaternion::CQuaternion(CQuaternion& src) //@INIT_4517
: CBaseObject(&GUID_QUATERNION) 
{//@CODE_4517
    ConstructorInclude();

    // Put in your own code
}//@CODE_4517


/*@NOTE_4519
Constructor method.
*/
CQuaternion::CQuaternion(LPCSTR name, CQuaternion& src) //@INIT_4519
: CBaseObject(&GUID_QUATERNION, name) 
{//@CODE_4519
    ConstructorInclude();

    // Put in your own code
}//@CODE_4519


/*@NOTE_4508
Destructor method.
*/
CQuaternion::~CQuaternion()
{//@CODE_4508
    DestructorInclude();

    // Put in your own code
}//@CODE_4508


void CQuaternion::operator(CMatrix4& dest)
{//@CODE_4538
}//@CODE_4538


CQuaternion& CQuaternion::operator(CQuaternion& src)
{//@CODE_4543
    CQuaternion value;

    return value;
}//@CODE_4543


BOOL CQuaternion::operator(CQuaternion& src)
{//@CODE_4545
    BOOL value;

    return value;
}//@CODE_4545


BOOL CQuaternion::operator!(CQuaternion& src)
{//@CODE_4547
    BOOL value;

    return value;
}//@CODE_4547


CQuaternion& CQuaternion::operator*(CQuaternion& src)
{//@CODE_4526
    CQuaternion value;

    return value;
}//@CODE_4526


CQuaternion& CQuaternion::operator*(CQuaternion& src)
{//@CODE_4534
    CQuaternion value;

    return value;
}//@CODE_4534


float CQuaternion::operator*()
{//@CODE_4542
    float value;

    return value;
}//@CODE_4542


CQuaternion& CQuaternion::operator+(CQuaternion& src)
{//@CODE_4522
    CQuaternion value;

    return value;
}//@CODE_4522


CQuaternion& CQuaternion::operator+(CQuaternion& src)
{//@CODE_4530
    CQuaternion value;

    return value;
}//@CODE_4530


CQuaternion& CQuaternion::operator-(CQuaternion& src)
{//@CODE_4524
    CQuaternion value;

    return value;
}//@CODE_4524


CQuaternion& CQuaternion::operator-(CQuaternion& src)
{//@CODE_4532
    CQuaternion value;

    return value;
}//@CODE_4532


CQuaternion& CQuaternion::operator/(CQuaternion& src)
{//@CODE_4528
    CQuaternion value;

    return value;
}//@CODE_4528


CQuaternion& CQuaternion::operator/(CQuaternion& src)
{//@CODE_4536
    CQuaternion value;

    return value;
}//@CODE_4536


float CQuaternion::operator[](int index)
{//@CODE_4540
    float value;

    return value;
}//@CODE_4540


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4509
Method which must be called first in a constructor.
*/
void CQuaternion::ConstructorInclude()
{
}


/*@NOTE_4510
Method which must be called first in a destructor.
*/
void CQuaternion::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
