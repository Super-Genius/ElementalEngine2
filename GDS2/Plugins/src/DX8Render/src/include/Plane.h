//---------------------------------------------------------------------
// File:		Plane.h
// Description:	Base Plane class
// Created:		5/27/02
// Author:		Kenneth L. Hurley
//---------------------------------------------------------------------

#ifndef PLANE_H
#define PLANE_H

#ifndef BASEOBJECT_H
#include "BaseObject.h"
#endif

#ifndef VECTOR_H
#include "Vector.h"
#endif

// {ADFC0F96-69C0-4aec-BBB3-D4D600CF169F}
static const GUID GUID_PLANE = 
{ 0xadfc0f96, 0x69c0, 0x4aec, { 0xbb, 0xb3, 0xd4, 0xd6, 0x0, 0xcf, 0x16, 0x9f } };

class CPlane : public CBaseObject
{
	// attributes
private:
	// these are just dummys for now....
	float		A, B, C, D;

	// operations
public:
	CPlane() : CBaseObject(&GUID_PLANE) {};
	CPlane(LPCSTR name) : CBaseObject(&GUID_PLANE, name) {};
	CPlane(CPlane& src) : CBaseObject(&GUID_PLANE) {};
	CPlane(LPCSTR name, CPlane& src) : CBaseObject(&GUID_PLANE, name) {};

	CPlane& operator+ (CPlane& src);
	CPlane& operator- (CPlane& src);
	CPlane& operator* (CPlane& src);
	CPlane& operator/ (CPlane& src);
	CPlane& operator+= (CPlane& src);
	CPlane& operator-= (CPlane& src);
	CPlane& operator*= (CPlane& src);
	CPlane& operator/= (CPlane& src);
	void operator= (CMatrix4& dest);
	float operator[] (int index);
	float operator* ();
	CPlane& operator= (CPlane& src);
	BOOL operator== (CPlane& src);
	BOOL operator!= (CPlane& src);
};

#endif	// #ifndef PLANE_H
