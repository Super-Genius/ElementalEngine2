//---------------------------------------------------------------------
// File:		Quaternion.h
// Description:	Base Quaternion class
// Created:		5/27/02
// Author:		Kenneth L. Hurley
//---------------------------------------------------------------------

#ifndef QUATERNION_H
#define QUATERNION_H

#ifndef BASEOBJECT_H
#include "BaseObject.h"
#endif

class CMatrix4;

// {E6353765-6DD5-4377-B186-DFC14A2A29A5}
static const GUID GUID_QUATERNION = 
{ 0xe6353765, 0x6dd5, 0x4377, { 0xb1, 0x86, 0xdf, 0xc1, 0x4a, 0x2a, 0x29, 0xa5 } };

class CQuaternion : public CBaseObject
{
	// attributes
private:
	// these are just dummys for now....
	float		m_Quaternion[3][3];		// the 3 vector rotation
	float 		m_QuatAxis;				// axis??

	// operations
public:
	CQuaternion() : CBaseObject(&GUID_QUATERNION) {};
	CQuaternion(LPCSTR name) : CBaseObject(&GUID_QUATERNION, name) {};
	CQuaternion(CQuaternion& src) : CBaseObject(&GUID_QUATERNION) {};
	CQuaternion(LPCSTR name, CQuaternion& src) : CBaseObject(&GUID_QUATERNION, name) {};

	CQuaternion& operator+ (CQuaternion& src);
	CQuaternion& operator- (CQuaternion& src);
	CQuaternion& operator* (CQuaternion& src);
	CQuaternion& operator/ (CQuaternion& src);
	CQuaternion& operator+= (CQuaternion& src);
	CQuaternion& operator-= (CQuaternion& src);
	CQuaternion& operator*= (CQuaternion& src);
	CQuaternion& operator/= (CQuaternion& src);
	void operator= (CMatrix4& dest);
	float operator[] (int index);
	float operator* ();
	CQuaternion& operator= (CQuaternion& src);
	BOOL operator== (CQuaternion& src);
	BOOL operator!= (CQuaternion& src);
};

#endif	// #ifndef QUATERNION_H
