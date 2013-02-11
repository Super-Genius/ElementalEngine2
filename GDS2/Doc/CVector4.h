//-----------------------------------------------------------------------------
//
// File:          CVector4.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CVector4'
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

#ifndef _CVECTOR4_H
#define _CVECTOR4_H

//@START_USER1
//@END_USER1


/*@NOTE_4738
C:\Development\Game Dev System\Plugins\src\include\Vector.h(57)

 {25610296-9ECB-4d28-9B2B-C883D6DEA871}
*/

class CVector4
    : public CBaseObject
{

//@START_USER2
//@END_USER2

//
// Group: ClassBuilder methods
//

private:
    void ConstructorInclude();
    void DestructorInclude();

//
// Non-Grouped Members
//

private:
    float m_Vector[4];
    float m_NormVector[4];
    BOOL isNormalValid;

//
// Non-Grouped Methods
//

public:
    CVector4();
    CVector4(LPCSTR name);
    CVector4(CVector4& src);
    CVector4(LPCSTR name, CVector4& src);
    virtual ~CVector4();
    CVector4& CrossProduct(CVector4& src);
    float Length();
    void Normalize();
    CVector4& operator(CVector4& src);
    BOOL operator(CVector4& src);
    BOOL operator!(CVector4& src);
    CVector4& operator*(CVector4& src);
    CVector4& operator*(CVector4& src);
    float operator*();
    CVector4& operator+(CVector4& src);
    CVector4& operator+(CVector4& src);
    CVector4& operator-(CVector4& src);
    CVector4& operator-(CVector4& src);
    CVector4& operator/(CVector4& src);
    CVector4& operator/(CVector4& src);
    float operator[](int index);
    float SqrLength();
};

#endif


#ifdef CB_INLINES
#ifndef _CVECTOR4_H_INLINES
#define _CVECTOR4_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
