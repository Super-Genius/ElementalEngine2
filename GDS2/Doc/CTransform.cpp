//-----------------------------------------------------------------------------
//
// File:          CTransform.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CTransform'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'Yaw'
*     Added method 'SetScale'
*     Added method 'SetPosition'
*     Added method 'SetOrientation'
*     Added method 'SetMatrix'
*     Added method 'Rotate'
*     Added method 'Rotate'
*     Added method 'Roll'
*     Added method 'Pitch'
*     Added method 'MoveTo'
*     Added method 'MoveRelative'
*     Added method 'LookAt'
*     Added method 'GetScale'
*     Added method 'GetPosition'
*     Added method 'GetOrientation'
*     Added method 'GetMatrix'
*     Added method '~CTransform'
*     Added method 'CTransform'
*     Added method 'CTransform'
*     Added method 'CTransform'
*     Added method 'CTransform'
*     Added method 'CTransform'
*     Added method 'CTransform'
*     Added method 'CTransform'
*     Added method 'CTransform'
*     Added inheritance 'CBaseObject'
*     Added member 'm_IsIdentity'
*     Added member 'm_IsDirty'
*     Added member 'm_Scale'
*     Added member 'm_Matrix'
*     Added member 'm_Orientation'
*     Added member 'm_Position'
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




/*@NOTE_4641
Constructor method.
*/
CTransform::CTransform() //@INIT_4641
: CBaseObject(&GUID_TRANSFORM) 
{//@CODE_4641
    ConstructorInclude();

    // Put in your own code
}//@CODE_4641


/*@NOTE_4642
Constructor method.
*/
CTransform::CTransform(LPCSTR name) //@INIT_4642
: CBaseObject(&GUID_TRANSFORM, name) 
{//@CODE_4642
    ConstructorInclude();

    // Put in your own code
}//@CODE_4642


/*@NOTE_4644
Constructor method.
*/
CTransform::CTransform(CTransform& src) //@INIT_4644
: CBaseObject(&GUID_TRANSFORM) 
{//@CODE_4644
    ConstructorInclude();

    // Put in your own code
}//@CODE_4644


/*@NOTE_4646
Constructor method.
*/
CTransform::CTransform(LPCSTR name, CTransform& src) //@INIT_4646
: CBaseObject(&GUID_TRANSFORM, name) 
{//@CODE_4646
    ConstructorInclude();

    // Put in your own code
}//@CODE_4646


/*@NOTE_4649
Constructor method.
*/
CTransform::CTransform(CVector4& pos, CMatrix4& mat,
                       CVector4& scale) //@INIT_4649
: CBaseObject(&GUID_TRANSFORM) 
{//@CODE_4649
    ConstructorInclude();

    // Put in your own code
}//@CODE_4649


/*@NOTE_4653
Constructor method.
*/
CTransform::CTransform(LPCSTR name, CVector4& pos, CMatrix4& mat,
                       CVector4 scale) //@INIT_4653
: CBaseObject(&GUID_TRANSFORM, name) 
{//@CODE_4653
    ConstructorInclude();

    // Put in your own code
}//@CODE_4653


/*@NOTE_4658
Constructor method.
*/
CTransform::CTransform(CQuaternion& quat) //@INIT_4658
: CBaseObject(&GUID_TRANSFORM) 
{//@CODE_4658
    ConstructorInclude();

    // Put in your own code
}//@CODE_4658


/*@NOTE_4660
Constructor method.
*/
CTransform::CTransform(LPCSTR name, CQuaternion& quat) //@INIT_4660
: CBaseObject(&GUID_TRANSFORM, name) 
{//@CODE_4660
    ConstructorInclude();

    // Put in your own code
}//@CODE_4660


/*@NOTE_4631
Destructor method.
*/
CTransform::~CTransform()
{//@CODE_4631
    DestructorInclude();

    // Put in your own code
}//@CODE_4631


CMatrix4& CTransform::GetMatrix()
{//@CODE_4685
    CMatrix4 value;

    return value;
}//@CODE_4685


CQuaternion& CTransform::GetOrientation()
{//@CODE_4691
    CQuaternion value;

    return value;
}//@CODE_4691


CVector4& CTransform::GetPosition()
{//@CODE_4667
    CVector4 value;

    return value;
}//@CODE_4667


CVector4& CTransform::GetScale()
{//@CODE_4688
    CVector4 value;

    return value;
}//@CODE_4688


void CTransform::LookAt(CVector4& target)
{//@CODE_4663
}//@CODE_4663


void CTransform::MoveRelative(CVector4& pos)
{//@CODE_4670
}//@CODE_4670


void CTransform::MoveTo(CVector4& pos)
{//@CODE_4668
}//@CODE_4668


void CTransform::Pitch(float angle)
{//@CODE_4676
}//@CODE_4676


void CTransform::Roll(float angle)
{//@CODE_4672
}//@CODE_4672


void CTransform::Rotate(CVector4& axis, float angle)
{//@CODE_4678
}//@CODE_4678


void CTransform::Rotate(CQuaternion& quat)
{//@CODE_4681
}//@CODE_4681


void CTransform::SetMatrix(CMatrix4& mat)
{//@CODE_4683
}//@CODE_4683


void CTransform::SetOrientation(CQuaternion& orient)
{//@CODE_4689
}//@CODE_4689


void CTransform::SetPosition(CVector4& pos)
{//@CODE_4665
}//@CODE_4665


void CTransform::SetScale(CVector4& scale)
{//@CODE_4686
}//@CODE_4686


void CTransform::Yaw(float angle)
{//@CODE_4674
}//@CODE_4674


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4632
Method which must be called first in a constructor.
*/
void CTransform::ConstructorInclude()
{
}


/*@NOTE_4633
Method which must be called first in a destructor.
*/
void CTransform::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
