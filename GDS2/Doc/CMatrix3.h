//-----------------------------------------------------------------------------
//
// File:          CMatrix3.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CMatrix3'
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

#ifndef _CMATRIX3_H
#define _CMATRIX3_H

//@START_USER1
//@END_USER1


/*@NOTE_4238
C:\Development\Game Dev System\Plugins\src\include\Matrix.h(27)

 {B4775C6A-DE01-4ed9-9F9D-9439AE064CE4}
*/

class CMatrix3
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
    float m_Matrix[3][3];

//
// Non-Grouped Methods
//

public:
    CMatrix3();
    CMatrix3(LPCSTR name);
    CMatrix3(CMatrix3& src);
    CMatrix3(LPCSTR name, CMatrix3& src);
    CMatrix3(CVector3 row1, CVector3 row2, CVector3 row3);
    CMatrix3(LPCSTR name, CVector3 row1, CVector3 row2, CVector3 row3);
    virtual ~CMatrix3();
    CMatrix3 operator*& (CMatrix3& src);
    CVector3 operator*& (CVector3& src);
    CMatrix3 operator& +(CMatrix3& src);
    CMatrix3 operator& -(CMatrix3& src);
    void operator(CMatrix3& src);
    BOOL operator(CMatrix3& src);
    BOOL operator!(CMatrix3& src);
    float* operator[](int index);
    void Pitch(float angle);
    void Roll(float angle);
    void Rotate(CVector3& axis, float angle);
    void Rotate(CQuaternion& orient);
    void Scale(float scale);
    void SetIdentity();
    void Yaw(float angle);
    void YawPitchRoll(float yaw, float pitch, float roll);
};

#endif


#ifdef CB_INLINES
#ifndef _CMATRIX3_H_INLINES
#define _CMATRIX3_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
