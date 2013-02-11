//-----------------------------------------------------------------------------
//
// File:          CPlane.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CPlane'
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
*     Added method '~CPlane'
*     Added method 'CPlane'
*     Added method 'CPlane'
*     Added method 'CPlane'
*     Added method 'CPlane'
*     Added inheritance 'CBaseObject'
*     Added member 'D'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _CPLANE_H
#define _CPLANE_H

//@START_USER1
//@END_USER1


/*@NOTE_4465
C:\Development\Game Dev System\Plugins\src\include\Plane.h(23)

 {ADFC0F96-69C0-4aec-BBB3-D4D600CF169F}
*/

class CPlane
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
    C D;

//
// Non-Grouped Methods
//

public:
    CPlane();
    CPlane(LPCSTR name);
    CPlane(CPlane& src);
    CPlane(LPCSTR name, CPlane& src);
    virtual ~CPlane();
    void operator(CMatrix4& dest);
    CPlane& operator(CPlane& src);
    BOOL operator(CPlane& src);
    BOOL operator!(CPlane& src);
    CPlane& operator*(CPlane& src);
    CPlane& operator*(CPlane& src);
    float operator*();
    CPlane& operator+(CPlane& src);
    CPlane& operator+(CPlane& src);
    CPlane& operator-(CPlane& src);
    CPlane& operator-(CPlane& src);
    CPlane& operator/(CPlane& src);
    CPlane& operator/(CPlane& src);
    float operator[](int index);
};

#endif


#ifdef CB_INLINES
#ifndef _CPLANE_H_INLINES
#define _CPLANE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
