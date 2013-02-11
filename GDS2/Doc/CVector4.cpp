//-----------------------------------------------------------------------------
//
// File:          CVector4.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CVector4'
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
*     Added method '~CVector4'
*     Added method 'CVector4'
*     Added method 'CVector4'
*     Added method 'CVector4'
*     Added method 'CVector4'
*     Added inheritance 'CBaseObject'
*     Added member 'isNormalValid'
*     Added member 'm_NormVector[4]'
*     Added member 'm_Vector[4]'
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




/*@NOTE_4746
Constructor method.
*/
CVector4::CVector4() //@INIT_4746
: CBaseObject(&GUID_VECTOR4) 
{//@CODE_4746
    ConstructorInclude();

    // Put in your own code
}//@CODE_4746


/*@NOTE_4747
Constructor method.
*/
CVector4::CVector4(LPCSTR name) //@INIT_4747
: CBaseObject(&GUID_VECTOR4, name) 
{//@CODE_4747
    ConstructorInclude();

    // Put in your own code
}//@CODE_4747


/*@NOTE_4749
Constructor method.
*/
CVector4::CVector4(CVector4& src) //@INIT_4749
: CBaseObject(&GUID_VECTOR4) 
{//@CODE_4749
    ConstructorInclude();

    // Put in your own code
}//@CODE_4749


/*@NOTE_4751
Constructor method.
*/
CVector4::CVector4(LPCSTR name, CVector4& src) //@INIT_4751
: CBaseObject(&GUID_VECTOR4, name) 
{//@CODE_4751
    ConstructorInclude();

    // Put in your own code
}//@CODE_4751


/*@NOTE_4739
Destructor method.
*/
CVector4::~CVector4()
{//@CODE_4739
    DestructorInclude();

    // Put in your own code
}//@CODE_4739


CVector4& CVector4::CrossProduct(CVector4& src)
{//@CODE_4780
    CVector4 value;

    return value;
}//@CODE_4780


float CVector4::Length()
{//@CODE_4782
    float value;

    return value;
}//@CODE_4782


void CVector4::Normalize()
{//@CODE_4779
}//@CODE_4779


CVector4& CVector4::operator(CVector4& src)
{//@CODE_4773
    CVector4 value;

    return value;
}//@CODE_4773


BOOL CVector4::operator(CVector4& src)
{//@CODE_4775
    BOOL value;

    return value;
}//@CODE_4775


BOOL CVector4::operator!(CVector4& src)
{//@CODE_4777
    BOOL value;

    return value;
}//@CODE_4777


CVector4& CVector4::operator*(CVector4& src)
{//@CODE_4758
    CVector4 value;

    return value;
}//@CODE_4758


CVector4& CVector4::operator*(CVector4& src)
{//@CODE_4766
    CVector4 value;

    return value;
}//@CODE_4766


float CVector4::operator*()
{//@CODE_4772
    float value;

    return value;
}//@CODE_4772


CVector4& CVector4::operator+(CVector4& src)
{//@CODE_4754
    CVector4 value;

    return value;
}//@CODE_4754


CVector4& CVector4::operator+(CVector4& src)
{//@CODE_4762
    CVector4 value;

    return value;
}//@CODE_4762


CVector4& CVector4::operator-(CVector4& src)
{//@CODE_4756
    CVector4 value;

    return value;
}//@CODE_4756


CVector4& CVector4::operator-(CVector4& src)
{//@CODE_4764
    CVector4 value;

    return value;
}//@CODE_4764


CVector4& CVector4::operator/(CVector4& src)
{//@CODE_4760
    CVector4 value;

    return value;
}//@CODE_4760


CVector4& CVector4::operator/(CVector4& src)
{//@CODE_4768
    CVector4 value;

    return value;
}//@CODE_4768


float CVector4::operator[](int index)
{//@CODE_4770
    float value;

    return value;
}//@CODE_4770


float CVector4::SqrLength()
{//@CODE_4783
    float value;

    return value;
}//@CODE_4783


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4740
Method which must be called first in a constructor.
*/
void CVector4::ConstructorInclude()
{
}


/*@NOTE_4741
Method which must be called first in a destructor.
*/
void CVector4::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
