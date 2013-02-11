//-----------------------------------------------------------------------------
//
// File:          CMatrix3.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CMatrix3'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'YawPitchRoll'
*     Added method 'Yaw'
*     Added method 'SetIdentity'
*     Added method 'Scale'
*     Added method 'Rotate'
*     Added method 'Rotate'
*     Added method 'Roll'
*     Added method 'Pitch'
*     Added method 'operator[]'
*     Added method 'operator!'
*     Added method 'operator'
*     Added method 'operator'
*     Added method '-'
*     Added method '+'
*     Added method ''
*     Added method ''
*     Added method '~CMatrix3'
*     Added method 'CMatrix3'
*     Added method 'CMatrix3'
*     Added method 'CMatrix3'
*     Added method 'CMatrix3'
*     Added method 'CMatrix3'
*     Added method 'CMatrix3'
*     Added inheritance 'CBaseObject'
*     Added member 'm_Matrix[3][3]'
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




/*@NOTE_4244
Constructor method.
*/
CMatrix3::CMatrix3() //@INIT_4244
: CBaseObject(&GUID_MATRIX3) 
{//@CODE_4244
    ConstructorInclude();

    // Put in your own code
}//@CODE_4244


/*@NOTE_4245
Constructor method.
*/
CMatrix3::CMatrix3(LPCSTR name) //@INIT_4245
: CBaseObject(&GUID_MATRIX3, name) 
{//@CODE_4245
    ConstructorInclude();

    // Put in your own code
}//@CODE_4245


/*@NOTE_4247
Constructor method.
*/
CMatrix3::CMatrix3(CMatrix3& src) //@INIT_4247
: CBaseObject(&GUID_MATRIX3) 
{//@CODE_4247
    ConstructorInclude();

    // Put in your own code
}//@CODE_4247


/*@NOTE_4249
Constructor method.
*/
CMatrix3::CMatrix3(LPCSTR name, CMatrix3& src) //@INIT_4249
: CBaseObject(&GUID_MATRIX3, name) 
{//@CODE_4249
    ConstructorInclude();

    // Put in your own code
}//@CODE_4249


/*@NOTE_4252
Constructor method.
*/
CMatrix3::CMatrix3(CVector3 row1, CVector3 row2, CVector3 row3) //@INIT_4252
: CBaseObject(&GUID_MATRIX3) 
{//@CODE_4252
    ConstructorInclude();

    // Put in your own code
}//@CODE_4252


/*@NOTE_4256
Constructor method.
*/
CMatrix3::CMatrix3(LPCSTR name, CVector3 row1, CVector3 row2,
                   CVector3 row3) //@INIT_4256
: CBaseObject(&GUID_MATRIX3, name) 
{//@CODE_4256
    ConstructorInclude();

    // Put in your own code
}//@CODE_4256


/*@NOTE_4239
Destructor method.
*/
CMatrix3::~CMatrix3()
{//@CODE_4239
    DestructorInclude();

    // Put in your own code
}//@CODE_4239


CMatrix3 operator*& CMatrix3::(CMatrix3& src)
{//@CODE_4267
    CMatrix3 value;

    return value;
}//@CODE_4267


CVector3 operator*& CMatrix3::(CVector3& src)
{//@CODE_4270
    CVector3 value;

    return value;
}//@CODE_4270


CMatrix3 operator& CMatrix3::+(CMatrix3& src)
{//@CODE_4262
    CMatrix3 value;

    return value;
}//@CODE_4262


CMatrix3 operator& CMatrix3::-(CMatrix3& src)
{//@CODE_4264
    CMatrix3 value;

    return value;
}//@CODE_4264


void CMatrix3::operator(CMatrix3& src)
{//@CODE_4274
}//@CODE_4274


BOOL CMatrix3::operator(CMatrix3& src)
{//@CODE_4276
    BOOL value;

    return value;
}//@CODE_4276


BOOL CMatrix3::operator!(CMatrix3& src)
{//@CODE_4278
    BOOL value;

    return value;
}//@CODE_4278


float* CMatrix3::operator[](int index)
{//@CODE_4272
    float* value;

    return value;
}//@CODE_4272


void CMatrix3::Pitch(float angle)
{//@CODE_4289
}//@CODE_4289


void CMatrix3::Roll(float angle)
{//@CODE_4285
}//@CODE_4285


void CMatrix3::Rotate(CVector3& axis, float angle)
{//@CODE_4291
}//@CODE_4291


void CMatrix3::Rotate(CQuaternion& orient)
{//@CODE_4294
}//@CODE_4294


void CMatrix3::Scale(float scale)
{//@CODE_4296
}//@CODE_4296


void CMatrix3::SetIdentity()
{//@CODE_4280
}//@CODE_4280


void CMatrix3::Yaw(float angle)
{//@CODE_4287
}//@CODE_4287


void CMatrix3::YawPitchRoll(float yaw, float pitch, float roll)
{//@CODE_4281
}//@CODE_4281


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4240
Method which must be called first in a constructor.
*/
void CMatrix3::ConstructorInclude()
{
}


/*@NOTE_4241
Method which must be called first in a destructor.
*/
void CMatrix3::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
