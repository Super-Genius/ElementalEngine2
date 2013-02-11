//---------------------------------------------------------------------
// File:		Matrix.h
// Description:	Base matrix class
// Created:		5/27/02
// Author:		Kenneth L. Hurley
//---------------------------------------------------------------------

#ifndef MATRIX_H
#define MATRIX_H

#ifndef BASEOBJECT_H
#include "BaseObject.h"
#endif

#ifndef VECTOR_H
#include "Vector.h"
#endif

#ifndef QUATERNION_H
#include "Quaternion.h"
#endif

// {B4775C6A-DE01-4ed9-9F9D-9439AE064CE4}
static const GUID GUID_MATRIX3 = 
{ 0xb4775c6a, 0xde01, 0x4ed9, { 0x9f, 0x9d, 0x94, 0x39, 0xae, 0x6, 0x4c, 0xe4 } };

class CMatrix3 : public CBaseObject
{
	// attributes
private:
	float		m_Matrix[3][3];		// the 3x3 matrix

	// operations
public:
	CMatrix3() : CBaseObject(&GUID_MATRIX3) {};
	CMatrix3(LPCSTR name) : CBaseObject(&GUID_MATRIX3, name) {};
	CMatrix3(CMatrix3& src) : CBaseObject(&GUID_MATRIX3) {};
	CMatrix3(LPCSTR name, CMatrix3& src) : CBaseObject(&GUID_MATRIX3, name) {};
	CMatrix3(CVector3 row1, CVector3 row2, CVector3 row3) : CBaseObject(&GUID_MATRIX3) {};
	CMatrix3(LPCSTR name, CVector3 row1, CVector3 row2, CVector3 row3) 
			: CBaseObject(&GUID_MATRIX3, name) {};
	CMatrix3& operator +(CMatrix3& src);
	CMatrix3& operator -(CMatrix3& src);
	CMatrix3& operator *(CMatrix3& src);
	CVector3& operator *(CVector3& src);
	float* operator[](int index);
	void operator=(CMatrix3& src);
	BOOL operator==(CMatrix3& src);
	BOOL operator!=(CMatrix3& src);
	void SetIdentity();
	void YawPitchRoll(float yaw, float pitch, float roll);
	void Roll(float angle);
	void Yaw(float angle);
	void Pitch(float angle);
	void Rotate(CVector3& axis, float angle);
	void Rotate(CQuaternion& orient);
	void Scale(float scale);
};


// {42514E51-4B2B-4a68-9DC4-73D101AD0AA6}
static const GUID GUID_MATRIX4 = 
{ 0x42514e51, 0x4b2b, 0x4a68, { 0x9d, 0xc4, 0x73, 0xd1, 0x1, 0xad, 0xa, 0xa6 } };

class CMatrix4 : public CBaseObject
{
	// attributes
private:
	float		m_Matrix[4][4];		// the 3x3 matrix

	// operations
public:
	CMatrix4() : CBaseObject(&GUID_MATRIX4) {};
	CMatrix4(LPCSTR name) : CBaseObject(&GUID_MATRIX4, name) {};
	CMatrix4(CMatrix4& src) : CBaseObject(&GUID_MATRIX4) {};
	CMatrix4(LPCSTR name, CMatrix4& src) : CBaseObject(&GUID_MATRIX4, name) {};
	CMatrix4(CVector4& row1, CVector4& row2, CVector4& row3, CVector4& row4) 
			: CBaseObject(&GUID_MATRIX4) {};
	CMatrix4(LPCSTR name, CVector4& row1, CVector4& row2, CVector4& row3, CVector4& row4) 
		: CBaseObject(&GUID_MATRIX4, name) {};

	CMatrix4& operator +(CMatrix4& src);
	CMatrix4& operator -(CMatrix4& src);
	CMatrix4& operator *(CMatrix4& src);
	CVector3& operator *(CVector3& src);
	CVector4& operator *(CVector4& src);
	float* operator[](int index);
	void operator=(CMatrix4& src);
	BOOL operator==(CMatrix4& src);
	BOOL operator!=(CMatrix4& src);
	void SetIdentity();
	void YawPitchRoll(float yaw, float pitch, float roll);
	void Roll(float angle);
	void Yaw(float angle);
	void Pitch(float angle);
	void Rotate(CVector3& axis, float angle);
	void Rotate(CQuaternion& orient);
	void Translate(CVector4& translation);
	void Scale(float scale);
};

#endif	// #ifndef MATRIX_H
