//-----------------------------------------------------------------------------
//
// File:          CVector3.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CVector3'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'SqrLength'
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
*     Added method 'Normalize'
*     Added method 'Length'
*     Added method 'CrossProduct'
*     Added method '~CVector3'
*     Added method 'CVector3'
*     Added method 'CVector3'
*     Added method 'CVector3'
*     Added method 'CVector3'
*     Added inheritance 'CBaseObject'
*     Added member 'isNormalValid'
*     Added member 'm_NormVector[3]'
*     Added member 'm_Vector[3]'
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




/*@NOTE_4700
Constructor method.
*/
CVector3::CVector3() //@INIT_4700
: CBaseObject(&GUID_VECTOR3) 
{//@CODE_4700
    ConstructorInclude();

    // Put in your own code
}//@CODE_4700


/*@NOTE_4701
Constructor method.
*/
CVector3::CVector3(LPCSTR name) //@INIT_4701
: CBaseObject(&GUID_VECTOR3, name) 
{//@CODE_4701
    ConstructorInclude();

    // Put in your own code
}//@CODE_4701


/*@NOTE_4703
Constructor method.
*/
CVector3::CVector3(CVector3& src) //@INIT_4703
: CBaseObject(&GUID_VECTOR3) 
{//@CODE_4703
    ConstructorInclude();

    // Put in your own code
}//@CODE_4703


/*@NOTE_4705
Constructor method.
*/
CVector3::CVector3(LPCSTR name, CVector3& src) //@INIT_4705
: CBaseObject(&GUID_VECTOR3, name) 
{//@CODE_4705
    ConstructorInclude();

    // Put in your own code
}//@CODE_4705


/*@NOTE_4693
Destructor method.
*/
CVector3::~CVector3()
{//@CODE_4693
    DestructorInclude();

    // Put in your own code
}//@CODE_4693


CVector3& CVector3::CrossProduct(CVector3& src)
{//@CODE_4734
    CVector3 value;

    return value;
}//@CODE_4734


float CVector3::Length()
{//@CODE_4736
    float value;

    return value;
}//@CODE_4736


void CVector3::Normalize()
{//@CODE_4733
}//@CODE_4733


CVector3& CVector3::operator(CVector3& src)
{//@CODE_4727
    CVector3 value;

    return value;
}//@CODE_4727


BOOL CVector3::operator(CVector3& src)
{//@CODE_4729
    BOOL value;

    return value;
}//@CODE_4729


BOOL CVector3::operator!(CVector3& src)
{//@CODE_4731
    BOOL value;

    return value;
}//@CODE_4731


CVector3& CVector3::operator*(CVector3& src)
{//@CODE_4712
    CVector3 value;

    return value;
}//@CODE_4712


CVector3& CVector3::operator*(CVector3& src)
{//@CODE_4720
    CVector3 value;

    return value;
}//@CODE_4720


float CVector3::operator*()
{//@CODE_4726
    float value;

    return value;
}//@CODE_4726


CVector3& CVector3::operator+(CVector3& src)
{//@CODE_4708
    CVector3 value;

    return value;
}//@CODE_4708


CVector3& CVector3::operator+(CVector3& src)
{//@CODE_4716
    CVector3 value;

    return value;
}//@CODE_4716


CVector3& CVector3::operator-(CVector3& src)
{//@CODE_4710
    CVector3 value;

    return value;
}//@CODE_4710


CVector3& CVector3::operator-(CVector3& src)
{//@CODE_4718
    CVector3 value;

    return value;
}//@CODE_4718


CVector3& CVector3::operator/(CVector3& src)
{//@CODE_4714
    CVector3 value;

    return value;
}//@CODE_4714


CVector3& CVector3::operator/(CVector3& src)
{//@CODE_4722
    CVector3 value;

    return value;
}//@CODE_4722


float CVector3::operator[](int index)
{//@CODE_4724
    float value;

    return value;
}//@CODE_4724


float CVector3::SqrLength()
{//@CODE_4737
    float value;

    return value;
}//@CODE_4737


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4694
Method which must be called first in a constructor.
*/
void CVector3::ConstructorInclude()
{
}


/*@NOTE_4695
Method which must be called first in a destructor.
*/
void CVector3::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
