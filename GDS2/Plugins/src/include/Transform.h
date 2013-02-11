//---------------------------------------------------------------------
// File:		Transform.h
// Description:	Base Transform class, this is used as a layer
//              between controllers and objects.  It can be used for
//              the world matrix, model matrix, etc.
// Created:		9/18/02
// Author:		Kenneth L. Hurley
//---------------------------------------------------------------------

#ifndef TRANSFORM_H
#define TRANSFORM_H

#ifndef BASEOBJECT_H
#include "BaseObject.h"
#endif

#ifndef VECTOR_H
#include "Vector.h"
#endif

#ifndef QUATERNION_H
#include "Quaternion.h"
#endif

// {4A63D1A2-6EC7-4b18-80C4-CBDA77B69BCC}
static const GUID GUID_TRANSFORM = 
{ 0x4a63d1a2, 0x6ec7, 0x4b18, { 0x80, 0xc4, 0xcb, 0xda, 0x77, 0xb6, 0x9b, 0xcc } };

class CTransform : public CBaseObject
{
	// attributes
private:
    CVector4    m_Position;         // position in local coordinates
    CQuaternion m_Orientation;      // orientation in local coordinates
    CMatrix4	m_Matrix;           // 4x4 matrix of orientation
    CVector4    m_Scale;            // scale factor
    BOOL        m_IsDirty;          // whether variables need updating from/to quaternion
    BOOL        m_IsIdentity;       // to save on some calculations

	// operations
public:
    // different ways to construct the object
	CTransform() : CBaseObject(&GUID_TRANSFORM) {};
	CTransform(LPCSTR name) : CBaseObject(&GUID_TRANSFORM, name) {};
	CTransform(CTransform& src) : CBaseObject(&GUID_TRANSFORM) {};
	CTransform(LPCSTR name, CTransform& src) : CBaseObject(&GUID_TRANSFORM, name) {};
	CTransform(CVector4& pos, CMatrix4& mat, CVector4& scale) : CBaseObject(&GUID_TRANSFORM) {};
	CTransform(LPCSTR name, CVector4& pos, CMatrix4& mat, CVector4 scale) 
			: CBaseObject(&GUID_TRANSFORM, name) {};
	CTransform(CQuaternion& quat) : CBaseObject(&GUID_TRANSFORM) {};
	CTransform(LPCSTR name, CQuaternion& quat) : CBaseObject(&GUID_TRANSFORM, name) {};

	void LookAt(CVector4& target);          // look at position
	void SetPosition(CVector4& pos);        // set position local coordinates
    CVector4& GetPosition();                // get the current position
    void MoveTo(CVector4& pos);
    void MoveRelative(CVector4& pos);
    void Roll(float angle);
    void Yaw(float angle);
    void Pitch(float angle);
    void Rotate(CVector4& axis, float angle);
    void Rotate(CQuaternion& quat);
    void SetMatrix(CMatrix4& mat);
    CMatrix4& GetMatrix();
    void SetScale(CVector4& scale);
    CVector4& GetScale();
    void SetOrientation(CQuaternion& orient);
    CQuaternion& GetOrientation();
};


#endif      //#ifndef TRANSFORM_H
