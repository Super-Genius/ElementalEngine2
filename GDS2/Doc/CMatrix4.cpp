//-----------------------------------------------------------------------------
//
// File:          CMatrix4.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CMatrix4'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'YawPitchRoll'
*     Added method 'Yaw'
*     Added method 'Translate'
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
*     Added method ''
*     Added method '~CMatrix4'
*     Added method 'CMatrix4'
*     Added method 'CMatrix4'
*     Added method 'CMatrix4'
*     Added method 'CMatrix4'
*     Added method 'CMatrix4'
*     Added method 'CMatrix4'
*     Added inheritance 'CBaseObject'
*     Added member 'm_Matrix[4][4]'
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




/*@NOTE_4304
Constructor method.
*/
CMatrix4::CMatrix4() //@INIT_4304
: CBaseObject(&GUID_MATRIX4) 
{//@CODE_4304
    ConstructorInclude();

    // Put in your own code
}//@CODE_4304


/*@NOTE_4305
Constructor method.
*/
CMatrix4::CMatrix4(LPCSTR name) //@INIT_4305
: CBaseObject(&GUID_MATRIX4, name) 
{//@CODE_4305
    ConstructorInclude();

    // Put in your own code
}//@CODE_4305


/*@NOTE_4307
Constructor method.
*/
CMatrix4::CMatrix4(CMatrix4& src) //@INIT_4307
: CBaseObject(&GUID_MATRIX4) 
{//@CODE_4307
    ConstructorInclude();

    // Put in your own code
}//@CODE_4307


/*@NOTE_4309
Constructor method.
*/
CMatrix4::CMatrix4(LPCSTR name, CMatrix4& src) //@INIT_4309
: CBaseObject(&GUID_MATRIX4, name) 
{//@CODE_4309
    ConstructorInclude();

    // Put in your own code
}//@CODE_4309


/*@NOTE_4312
Constructor method.
*/
CMatrix4::CMatrix4(CVector4& row1, CVector4& row2, CVector4& row3,
                   CVector4& row4) //@INIT_4312
: CBaseObject(&GUID_MATRIX4) 
{//@CODE_4312
    ConstructorInclude();

    // Put in your own code
}//@CODE_4312


/*@NOTE_4317
Constructor method.
*/
CMatrix4::CMatrix4(LPCSTR name, CVector4& row1, CVector4& row2, CVector4& row3,
                   CVector4& row4) //@INIT_4317
: CBaseObject(&GUID_MATRIX4, name) 
{//@CODE_4317
    ConstructorInclude();

    // Put in your own code
}//@CODE_4317


/*@NOTE_4299
Destructor method.
*/
CMatrix4::~CMatrix4()
{//@CODE_4299
    DestructorInclude();

    // Put in your own code
}//@CODE_4299


CMatrix4 operator*& CMatrix4::(CMatrix4& src)
{//@CODE_4329
    CMatrix4 value;

    return value;
}//@CODE_4329


CVector3 operator*& CMatrix4::(CVector3& src)
{//@CODE_4331
    CVector3 value;

    return value;
}//@CODE_4331


CVector4 operator*& CMatrix4::(CVector4& src)
{//@CODE_4334
    CVector4 value;

    return value;
}//@CODE_4334


CMatrix4 operator& CMatrix4::+(CMatrix4& src)
{//@CODE_4324
    CMatrix4 value;

    return value;
}//@CODE_4324


CMatrix4 operator& CMatrix4::-(CMatrix4& src)
{//@CODE_4326
    CMatrix4 value;

    return value;
}//@CODE_4326


void CMatrix4::operator(CMatrix4& src)
{//@CODE_4338
}//@CODE_4338


BOOL CMatrix4::operator(CMatrix4& src)
{//@CODE_4340
    BOOL value;

    return value;
}//@CODE_4340


BOOL CMatrix4::operator!(CMatrix4& src)
{//@CODE_4342
    BOOL value;

    return value;
}//@CODE_4342


float* CMatrix4::operator[](int index)
{//@CODE_4336
    float* value;

    return value;
}//@CODE_4336


void CMatrix4::Pitch(float angle)
{//@CODE_4353
}//@CODE_4353


void CMatrix4::Roll(float angle)
{//@CODE_4349
}//@CODE_4349


void CMatrix4::Rotate(CVector3& axis, float angle)
{//@CODE_4355
}//@CODE_4355


void CMatrix4::Rotate(CQuaternion& orient)
{//@CODE_4358
}//@CODE_4358


void CMatrix4::Scale(float scale)
{//@CODE_4362
}//@CODE_4362


void CMatrix4::SetIdentity()
{//@CODE_4344
}//@CODE_4344


void CMatrix4::Translate(CVector4& translation)
{//@CODE_4360
}//@CODE_4360


void CMatrix4::Yaw(float angle)
{//@CODE_4351
}//@CODE_4351


void CMatrix4::YawPitchRoll(float yaw, float pitch, float roll)
{//@CODE_4345
}//@CODE_4345


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4300
Method which must be called first in a constructor.
*/
void CMatrix4::ConstructorInclude()
{
}


/*@NOTE_4301
Method which must be called first in a destructor.
*/
void CMatrix4::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
