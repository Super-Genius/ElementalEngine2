//---------------------------------------------------------------------
// File:		Vector.h
// Description:	Base matrix clas
// Created:		5/27/02
// Author:		Kenneth L. Hurley
//---------------------------------------------------------------------

#ifndef VECTOR_H
#define VECTOR_H

#ifndef BASEOBJECT_H
#include "BaseObject.h"
#endif

// {A1961C53-E9A1-497b-A9A1-B2F265C2F1B1}
static const GUID GUID_VECTOR3 = 
{ 0xa1961c53, 0xe9a1, 0x497b, { 0xa9, 0xa1, 0xb2, 0xf2, 0x65, 0xc2, 0xf1, 0xb1 } };

class CVector3 : public CBaseObject
{
	// attributes
private:
	float		m_Vector[3];		// the 3 vector floats
	float		m_NormVector[3];	// the normalized 3 vector floats
	BOOL		isNormalValid;	// if the normal vector is valid

	// operations
public:
	CVector3() : CBaseObject(&GUID_VECTOR3) {};
	CVector3(LPCSTR name) : CBaseObject(&GUID_VECTOR3, name) {};
	CVector3(CVector3& src) : CBaseObject(&GUID_VECTOR3) {};
	CVector3(LPCSTR name, CVector3& src) : CBaseObject(&GUID_VECTOR3, name) {};

	CVector3& operator+ (CVector3& src);
	CVector3& operator- (CVector3& src);
	CVector3& operator* (CVector3& src);
	CVector3& operator/ (CVector3& src);
	CVector3& operator+= (CVector3& src);
	CVector3& operator-= (CVector3& src);
	CVector3& operator*= (CVector3& src);
	CVector3& operator/= (CVector3& src);
	float operator[] (int index);
	float operator* ();
	CVector3& operator= (CVector3& src);
	BOOL operator== (CVector3& src);
	BOOL operator!= (CVector3& src);
	void Normalize();
	CVector3& CrossProduct(CVector3& src);
	float Length();
	float SqrLength();
};

// {25610296-9ECB-4d28-9B2B-C883D6DEA871}
static const GUID GUID_VECTOR4 = 
{ 0x25610296, 0x9ecb, 0x4d28, { 0x9b, 0x2b, 0xc8, 0x83, 0xd6, 0xde, 0xa8, 0x71 } };

class CVector4 : public CBaseObject
{
	// attributes
private:
	float		m_Vector[4];		// the 4 vector floats
	float		m_NormVector[4];	// the normalized 4 vector floats
	BOOL		isNormalValid;		// if the normal vector is valid

	// operations
public:
	CVector4() : CBaseObject(&GUID_VECTOR4) {};
	CVector4(LPCSTR name) : CBaseObject(&GUID_VECTOR4, name) {};
	CVector4(CVector4& src) : CBaseObject(&GUID_VECTOR4) {};
	CVector4(LPCSTR name, CVector4& src) : CBaseObject(&GUID_VECTOR4, name) {};

	CVector4& operator+ (CVector4& src);
	CVector4& operator- (CVector4& src);
	CVector4& operator* (CVector4& src);
	CVector4& operator/ (CVector4& src);
	CVector4& operator+= (CVector4& src);
	CVector4& operator-= (CVector4& src);
	CVector4& operator*= (CVector4& src);
	CVector4& operator/= (CVector4& src);
	float operator[] (int index);
	float operator* ();
	CVector4& operator= (CVector4& src);
	BOOL operator== (CVector4& src);
	BOOL operator!= (CVector4& src);
	void Normalize();
	CVector4& CrossProduct(CVector4& src);
	float Length();
	float SqrLength();
};

#endif	// #ifndef VECTOR_H
