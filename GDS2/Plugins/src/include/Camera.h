//---------------------------------------------------------------------
// File:		Camera.h
// Description:	Base Camera class
// Created:		5/27/02
// Author:		Kenneth L. Hurley
//---------------------------------------------------------------------

#ifndef CAMERA_H
#define CAMERA_H

#ifndef BASEOBJECT_H
#include "BaseObject.h"
#endif

#ifndef MATRIX_H
#include "Matrix.h"
#endif

#ifndef QUATERNION_H
#include "Quaternion.h"
#endif

#ifndef VECTOR_H
#include "Vector.h"
#endif

#ifndef PLANE_H
#include "Plane.h"
#endif

enum PROJECTION_TYPE
{
	PT_PROJECTION,
	PT_ORTHOGRAPHIC
};

// {6E3AB6E5-1101-4077-9ACB-D13133BB41E8}
static const GUID GUID_CAMERA = 
{ 0x6e3ab6e5, 0x1101, 0x4077, { 0x9a, 0xcb, 0xd1, 0x31, 0x33, 0xbb, 0x41, 0xe8 } };

class CCamera : public CBaseObject
{
private:
		float		m_FOV;				// the field of view for the camera
		CPlane		m_FrustumPlanes[6];	// view frustum clip planes
		CVector4	m_Position;			// position in world homogeneous
		CQuaternion	m_Orientation;		// orientation in quaternion form
		float		m_FarClip;			// far clip plane
		float		m_NearClip;			// near clip plane
		PROJECTION_TYPE m_ProjectionType; // projection type of camera
		float		m_AspectRatio;		// aspect ration for camera
		CMatrix4	m_ProjectionMatrix;	// the projection matrix for the camera

public:
		CCamera() : CBaseObject(&GUID_CAMERA) {};
		CCamera(LPCSTR name) : CBaseObject(&GUID_CAMERA, name) {};

		void LookAt(CVector4& target);
		BOOL SetProjectionType(PROJECTION_TYPE projection);
		PROJECTION_TYPE GetProjectionType();
		void SetFOV(float fov);
		float GetFOV();
		void SetPosition(CVector4& position);
		CVector4& GetPosition();
		void MoveTo(CVector4& pos);
		void Roll(float angle);
		void Yaw(float angle);
		void Pitch(float angle);
		void Rotate(CVector4& axis, float angle);
		void Rotate(CQuaternion& quat);
		void SetViewMatrix(CMatrix4& viewMat);
		CMatrix4& GetViewMatrix();
		void SetProjectionMatrix(CMatrix4& viewMat);
		CMatrix4& GetProjectionMatrix();
		void SetNearClip(float dist);
		float GetNearClip();
		void SetFarClip(float dist);
		float GetFarClip();
		void SetOrientation(CQuaternion& orient);
		CQuaternion& GetOrientation();
		CPlane* GetFrustumPlanes();
		void SetAspectRatio(float aspect);
		float GetAspectRatio();
};

#endif	// #ifdef CAMERA_H
