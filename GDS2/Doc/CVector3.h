//-----------------------------------------------------------------------------
//
// File:          CVector3.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CVector3'
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

#ifndef _CVECTOR3_H
#define _CVECTOR3_H

//@START_USER1
//@END_USER1


/*@NOTE_4692
C:\Development\Game Dev System\Plugins\src\include\Vector.h(19)

 {A1961C53-E9A1-497b-A9A1-B2F265C2F1B1}
*/

class CVector3
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
    float m_Vector[3];
    float m_NormVector[3];
    BOOL isNormalValid;

//
// Non-Grouped Methods
//

public:
    CVector3();
    CVector3(LPCSTR name);
    CVector3(CVector3& src);
    CVector3(LPCSTR name, CVector3& src);
    virtual ~CVector3();
    CVector3& CrossProduct(CVector3& src);
    float Length();
    void Normalize();
    CVector3& operator(CVector3& src);
    BOOL operator(CVector3& src);
    BOOL operator!(CVector3& src);
    CVector3& operator*(CVector3& src);
    CVector3& operator*(CVector3& src);
    float operator*();
    CVector3& operator+(CVector3& src);
    CVector3& operator+(CVector3& src);
    CVector3& operator-(CVector3& src);
    CVector3& operator-(CVector3& src);
    CVector3& operator/(CVector3& src);
    CVector3& operator/(CVector3& src);
    float operator[](int index);
    float SqrLength();
};

#endif


#ifdef CB_INLINES
#ifndef _CVECTOR3_H_INLINES
#define _CVECTOR3_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
