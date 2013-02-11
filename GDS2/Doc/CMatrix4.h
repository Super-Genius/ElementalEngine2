//-----------------------------------------------------------------------------
//
// File:          CMatrix4.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CMatrix4'
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

#ifndef _CMATRIX4_H
#define _CMATRIX4_H

//@START_USER1
//@END_USER1


/*@NOTE_4298
C:\Development\Game Dev System\Plugins\src\include\Matrix.h(65)

 {42514E51-4B2B-4a68-9DC4-73D101AD0AA6}
*/

class CMatrix4
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
    float m_Matrix[4][4];

//
// Non-Grouped Methods
//

public:
    CMatrix4();
    CMatrix4(LPCSTR name);
    CMatrix4(CMatrix4& src);
    CMatrix4(LPCSTR name, CMatrix4& src);
    CMatrix4(CVector4& row1, CVector4& row2, CVector4& row3, CVector4& row4);
    CMatrix4(LPCSTR name, CVector4& row1, CVector4& row2, CVector4& row3,
             CVector4& row4);
    virtual ~CMatrix4();
    CMatrix4 operator*& (CMatrix4& src);
    CVector3 operator*& (CVector3& src);
    CVector4 operator*& (CVector4& src);
    CMatrix4 operator& +(CMatrix4& src);
    CMatrix4 operator& -(CMatrix4& src);
    void operator(CMatrix4& src);
    BOOL operator(CMatrix4& src);
    BOOL operator!(CMatrix4& src);
    float* operator[](int index);
    void Pitch(float angle);
    void Roll(float angle);
    void Rotate(CVector3& axis, float angle);
    void Rotate(CQuaternion& orient);
    void Scale(float scale);
    void SetIdentity();
    void Translate(CVector4& translation);
    void Yaw(float angle);
    void YawPitchRoll(float yaw, float pitch, float roll);
};

#endif


#ifdef CB_INLINES
#ifndef _CMATRIX4_H_INLINES
#define _CMATRIX4_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
