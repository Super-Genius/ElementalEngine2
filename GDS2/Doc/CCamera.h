//-----------------------------------------------------------------------------
//
// File:          CCamera.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CCamera'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'Yaw'
*     Added method 'SetViewMatrix'
*     Added method 'SetProjectionType'
*     Added method 'SetProjectionMatrix'
*     Added method 'SetPosition'
*     Added method 'SetOrientation'
*     Added method 'SetNearClip'
*     Added method 'SetFOV'
*     Added method 'SetFarClip'
*     Added method 'SetAspectRatio'
*     Added method 'Rotate'
*     Added method 'Rotate'
*     Added method 'Roll'
*     Added method 'Pitch'
*     Added method 'MoveTo'
*     Added method 'LookAt'
*     Added method 'GetViewMatrix'
*     Added method 'GetProjectionType'
*     Added method 'GetProjectionMatrix'
*     Added method 'GetPosition'
*     Added method 'GetOrientation'
*     Added method 'GetNearClip'
*     Added method 'GetFrustumPlanes'
*     Added method 'GetFOV'
*     Added method 'GetFarClip'
*     Added method 'GetAspectRatio'
*     Added method '~CCamera'
*     Added method 'CCamera'
*     Added method 'CCamera'
*     Added inheritance 'CBaseObject'
*     Added member 'm_ProjectionMatrix'
*     Added member 'm_AspectRatio'
*     Added member 'm_ProjectionType'
*     Added member 'm_NearClip'
*     Added member 'm_FarClip'
*     Added member 'm_Orientation'
*     Added member 'm_Position'
*     Added member 'm_FrustumPlanes[6]'
*     Added member 'm_FOV'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _CCAMERA_H
#define _CCAMERA_H

//@START_USER1
//@END_USER1


/*@NOTE_3990
C:\Development\Game Dev System\Plugins\src\include\Camera.h(41)

 {6E3AB6E5-1101-4077-9ACB-D13133BB41E8}
*/

class CCamera
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
    float m_FOV;
    CPlane m_FrustumPlanes[6];
    CVector4 m_Position;
    CQuaternion m_Orientation;
    float m_FarClip;
    float m_NearClip;
    PROJECTION_TYPE m_ProjectionType;
    float m_AspectRatio;
    CMatrix4 m_ProjectionMatrix;

//
// Non-Grouped Methods
//

public:
    CCamera();
    CCamera(LPCSTR name);
    virtual ~CCamera();
    float GetAspectRatio();
    float GetFarClip();
    float GetFOV();
    CPlane* GetFrustumPlanes();
    float GetNearClip();
    CQuaternion& GetOrientation();
    CVector4& GetPosition();
    CMatrix4& GetProjectionMatrix();
    PROJECTION_TYPE GetProjectionType();
    CMatrix4& GetViewMatrix();
    void LookAt(CVector4& target);
    void MoveTo(CVector4& pos);
    void Pitch(float angle);
    void Roll(float angle);
    void Rotate(CVector4& axis, float angle);
    void Rotate(CQuaternion& quat);
    void SetAspectRatio(float aspect);
    void SetFarClip(float dist);
    void SetFOV(float fov);
    void SetNearClip(float dist);
    void SetOrientation(CQuaternion& orient);
    void SetPosition(CVector4& position);
    void SetProjectionMatrix(CMatrix4& viewMat);
    BOOL SetProjectionType(PROJECTION_TYPE projection);
    void SetViewMatrix(CMatrix4& viewMat);
    void Yaw(float angle);
};

#endif


#ifdef CB_INLINES
#ifndef _CCAMERA_H_INLINES
#define _CCAMERA_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
