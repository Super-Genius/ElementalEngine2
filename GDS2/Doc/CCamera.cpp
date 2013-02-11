//-----------------------------------------------------------------------------
//
// File:          CCamera.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CCamera'
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

//@START_USER1
//@END_USER1


// Master include file
#include "StdAfx.h"


//@START_USER2
//@END_USER2




/*@NOTE_4006
Constructor method.
*/
CCamera::CCamera() //@INIT_4006
: CBaseObject(&GUID_CAMERA) 
{//@CODE_4006
    ConstructorInclude();

    // Put in your own code
}//@CODE_4006


/*@NOTE_4007
Constructor method.
*/
CCamera::CCamera(LPCSTR name) //@INIT_4007
: CBaseObject(&GUID_CAMERA, name) 
{//@CODE_4007
    ConstructorInclude();

    // Put in your own code
}//@CODE_4007


/*@NOTE_3991
Destructor method.
*/
CCamera::~CCamera()
{//@CODE_3991
    DestructorInclude();

    // Put in your own code
}//@CODE_3991


float CCamera::GetAspectRatio()
{//@CODE_4051
    float value;

    return value;
}//@CODE_4051


float CCamera::GetFarClip()
{//@CODE_4044
    float value;

    return value;
}//@CODE_4044


float CCamera::GetFOV()
{//@CODE_4016
    float value;

    return value;
}//@CODE_4016


CPlane* CCamera::GetFrustumPlanes()
{//@CODE_4048
    CPlane* value;

    return value;
}//@CODE_4048


float CCamera::GetNearClip()
{//@CODE_4041
    float value;

    return value;
}//@CODE_4041


CQuaternion& CCamera::GetOrientation()
{//@CODE_4047
    CQuaternion value;

    return value;
}//@CODE_4047


CVector4& CCamera::GetPosition()
{//@CODE_4019
    CVector4 value;

    return value;
}//@CODE_4019


CMatrix4& CCamera::GetProjectionMatrix()
{//@CODE_4038
    CMatrix4 value;

    return value;
}//@CODE_4038


PROJECTION_TYPE CCamera::GetProjectionType()
{//@CODE_4013
    PROJECTION_TYPE value;

    return value;
}//@CODE_4013


CMatrix4& CCamera::GetViewMatrix()
{//@CODE_4035
    CMatrix4 value;

    return value;
}//@CODE_4035


void CCamera::LookAt(CVector4& target)
{//@CODE_4009
}//@CODE_4009


void CCamera::MoveTo(CVector4& pos)
{//@CODE_4020
}//@CODE_4020


void CCamera::Pitch(float angle)
{//@CODE_4026
}//@CODE_4026


void CCamera::Roll(float angle)
{//@CODE_4022
}//@CODE_4022


void CCamera::Rotate(CVector4& axis, float angle)
{//@CODE_4028
}//@CODE_4028


void CCamera::Rotate(CQuaternion& quat)
{//@CODE_4031
}//@CODE_4031


void CCamera::SetAspectRatio(float aspect)
{//@CODE_4049
}//@CODE_4049


void CCamera::SetFarClip(float dist)
{//@CODE_4042
}//@CODE_4042


void CCamera::SetFOV(float fov)
{//@CODE_4014
}//@CODE_4014


void CCamera::SetNearClip(float dist)
{//@CODE_4039
}//@CODE_4039


void CCamera::SetOrientation(CQuaternion& orient)
{//@CODE_4045
}//@CODE_4045


void CCamera::SetPosition(CVector4& position)
{//@CODE_4017
}//@CODE_4017


void CCamera::SetProjectionMatrix(CMatrix4& viewMat)
{//@CODE_4036
}//@CODE_4036


BOOL CCamera::SetProjectionType(PROJECTION_TYPE projection)
{//@CODE_4011
    BOOL value;

    return value;
}//@CODE_4011


void CCamera::SetViewMatrix(CMatrix4& viewMat)
{//@CODE_4033
}//@CODE_4033


void CCamera::Yaw(float angle)
{//@CODE_4024
}//@CODE_4024


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3992
Method which must be called first in a constructor.
*/
void CCamera::ConstructorInclude()
{
}


/*@NOTE_3993
Method which must be called first in a destructor.
*/
void CCamera::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
