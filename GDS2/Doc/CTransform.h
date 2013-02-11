//-----------------------------------------------------------------------------
//
// File:          CTransform.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CTransform'
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

#ifndef _CTRANSFORM_H
#define _CTRANSFORM_H

//@START_USER1
//@END_USER1


/*@NOTE_4630
C:\Development\Game Dev System\Plugins\src\include\Transform.h(29)

 {4A63D1A2-6EC7-4b18-80C4-CBDA77B69BCC}
*/

class CTransform
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
    CVector4 m_Position;
    CQuaternion m_Orientation;
    CMatrix4 m_Matrix;
    CVector4 m_Scale;
    BOOL m_IsDirty;
    BOOL m_IsIdentity;

//
// Non-Grouped Methods
//

public:
    CTransform();
    CTransform(LPCSTR name);
    CTransform(CTransform& src);
    CTransform(LPCSTR name, CTransform& src);
    CTransform(CVector4& pos, CMatrix4& mat, CVector4& scale);
    CTransform(LPCSTR name, CVector4& pos, CMatrix4& mat, CVector4 scale);
    CTransform(CQuaternion& quat);
    CTransform(LPCSTR name, CQuaternion& quat);
    virtual ~CTransform();
    CMatrix4& GetMatrix();
    CQuaternion& GetOrientation();
    CVector4& GetPosition();
    CVector4& GetScale();
    void LookAt(CVector4& target);
    void MoveRelative(CVector4& pos);
    void MoveTo(CVector4& pos);
    void Pitch(float angle);
    void Roll(float angle);
    void Rotate(CVector4& axis, float angle);
    void Rotate(CQuaternion& quat);
    void SetMatrix(CMatrix4& mat);
    void SetOrientation(CQuaternion& orient);
    void SetPosition(CVector4& pos);
    void SetScale(CVector4& scale);
    void Yaw(float angle);
};

#endif


#ifdef CB_INLINES
#ifndef _CTRANSFORM_H_INLINES
#define _CTRANSFORM_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
