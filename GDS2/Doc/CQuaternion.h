//-----------------------------------------------------------------------------
//
// File:          CQuaternion.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CQuaternion'
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

#ifndef _CQUATERNION_H
#define _CQUATERNION_H

//@START_USER1
//@END_USER1


/*@NOTE_4507
C:\Development\Game Dev System\Plugins\src\include\Quaternion.h(21)

 {E6353765-6DD5-4377-B186-DFC14A2A29A5}
*/

class CQuaternion
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
    float m_Quaternion[3][3];
    float m_QuatAxis;

//
// Non-Grouped Methods
//

public:
    CQuaternion();
    CQuaternion(LPCSTR name);
    CQuaternion(CQuaternion& src);
    CQuaternion(LPCSTR name, CQuaternion& src);
    virtual ~CQuaternion();
    void operator(CMatrix4& dest);
    CQuaternion& operator(CQuaternion& src);
    BOOL operator(CQuaternion& src);
    BOOL operator!(CQuaternion& src);
    CQuaternion& operator*(CQuaternion& src);
    CQuaternion& operator*(CQuaternion& src);
    float operator*();
    CQuaternion& operator+(CQuaternion& src);
    CQuaternion& operator+(CQuaternion& src);
    CQuaternion& operator-(CQuaternion& src);
    CQuaternion& operator-(CQuaternion& src);
    CQuaternion& operator/(CQuaternion& src);
    CQuaternion& operator/(CQuaternion& src);
    float operator[](int index);
};

#endif


#ifdef CB_INLINES
#ifndef _CQUATERNION_H_INLINES
#define _CQUATERNION_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
