///==========================================================================
/// \file	DefaultVec3Implementation.cpp
/// \brief	
/// \date	08/10/2005
/// \author	Brian Bazyk
///         Copyright (c)  2005-2008 Signature Devices, Inc.
///
///         This code is redistributable under the terms of the EE License.
///
///         This code is distributed without warranty or implied warranty of
///			merchantability or fitness for a particular purpose. See the 
///			EE License for more details.
///
///         You should have received a copy of the EE License along with this
///			code; If not, write to Signature Devices, Inc.,
///			3200 Bridge Parkway Suite 102, Redwood City, CA 94086 USA.

///==========================================================================

#include "stdafx.h"
#include <math.h>
#include "DefaultVec3Implementation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IVec3Implementation *CDefaultVec3Implementation::s_instance = NULL;

void CDefaultVec3Implementation::Init( void )
{
}

void CDefaultVec3Implementation::CleanUp( void )
{
}

bool CDefaultVec3Implementation::Equals(const Vec3& a, const Vec3& b) const
{
	return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}

bool CDefaultVec3Implementation::NotEquals(const Vec3& a, const Vec3& b) const
{
	return ((a.x != b.x) || (a.y != b.y) || (a.z != b.z));
}

bool CDefaultVec3Implementation::LessThan(const Vec3& a, const Vec3& b) const
{
	return ((a.x < b.x) && (a.y < b.y) && (a.z < b.z));
}
bool CDefaultVec3Implementation::GreaterThan(const Vec3& a, const Vec3& b) const
{
	return ((a.x > b.x) && (a.y > b.y) && (a.z > b.z));
}

Vec3 CDefaultVec3Implementation::Add( const Vec3& a, const Vec3& b ) const
{
	return Vec3( 
		a.x + b.x, 
		a.y + b.y, 
		a.z + b.z );
}

Vec3& CDefaultVec3Implementation::AddEquals( Vec3& a, const Vec3& b ) const
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

Vec3 CDefaultVec3Implementation::Subtract(const Vec3& a, const Vec3& b) const
{
	return Vec3(
		a.x - b.x, 
		a.y - b.y, 
		a.z - b.z );
}

Vec3& CDefaultVec3Implementation::SubtractEquals( Vec3& a, const Vec3& b ) const
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

Vec3 CDefaultVec3Implementation::Times(const Vec3& a, const Vec3& b) const
{
	return Vec3(
		a.x * b.x, 
		a.y * b.y, 
		a.z * b.z );
}

const Vec3& CDefaultVec3Implementation::TimesEquals(Vec3& a, const Vec3& b) const
{
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	return a;
}

Vec3 CDefaultVec3Implementation::Times(const Vec3& a, float scalar) const
{
	return Vec3(
		a.x * scalar, 
		a.y * scalar, 
		a.z * scalar );
}

const Vec3& CDefaultVec3Implementation::TimesEquals(Vec3& a, float scalar) const
{
	a.x *= scalar;
	a.y *= scalar;
	a.z *= scalar;
	return a;
}

Vec3 CDefaultVec3Implementation::Times(const Vec3& a, const Matrix3x3& m) const
{
	Vec3 temp;
	temp.x = (a.x * m.m[0]) + (a.y * m.m[3]) + (a.z * m.m[6]);
	temp.y = (a.x * m.m[1]) + (a.y * m.m[4]) + (a.z * m.m[7]);
	temp.z = (a.x * m.m[2]) + (a.y * m.m[5]) + (a.z * m.m[8]);
	return temp;
}

const Vec3& CDefaultVec3Implementation::TimesEquals(Vec3& a, const Matrix3x3& m) const
{
	Vec3 t;
	t.x = a.x * m.m[0] + a.y * m.m[3] + a.z * m.m[6];
	t.y = a.x * m.m[1] + a.y * m.m[4] + a.z * m.m[7];
	t.z = a.x * m.m[2] + a.y * m.m[5] + a.z * m.m[8];
	a = t;
	return a;
}

Vec3 CDefaultVec3Implementation::Divide(const Vec3& a, const Vec3& b) const
{
	return Vec3(
		a.x / b.x, 
		a.y / b.y, 
		a.z / b.z );
}

const Vec3& CDefaultVec3Implementation::DivideEquals(Vec3& a, const Vec3& b) const
{
	a.x /= b.x;
	a.y /= b.y;
	a.z /= b.z;
	return a;
}

Vec3 CDefaultVec3Implementation::Divide(const Vec3& a,  float scalar) const
{
	return Vec3(
		a.x / scalar, 
		a.y / scalar, 
		a.z / scalar );
}

const Vec3& CDefaultVec3Implementation::DivideEquals(Vec3& a,  float scalar) const
{
	a.x /= scalar;
	a.y /= scalar;
	a.z /= scalar;
	return a;
}

float CDefaultVec3Implementation::Length(const Vec3& vec) const
{
	return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

float CDefaultVec3Implementation::LengthSQ(const Vec3& vec) const
{
	return (vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

void CDefaultVec3Implementation::SetLength(Vec3& vec, float newLength)
{
	Normalize(vec);
	TimesEquals(vec, newLength);
}

float CDefaultVec3Implementation::Dot(const Vec3& a, const Vec3& b) const
{
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

Vec3 CDefaultVec3Implementation::Cross(const Vec3& a, const Vec3& b) const
{
	return Vec3((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
}

float CDefaultVec3Implementation::DistanceBetween(const Vec3& a, const Vec3& b)
{
		float vx = a.x - b.x;
		float vy = a.y - b.y;
		float vz = a.z - b.z;
		return sqrt(vx*vx + vy*vy + vz*vz);
}

float CDefaultVec3Implementation::DistanceBetweenSQ(const Vec3& a, const Vec3& b)
{
		float vx = a.x - b.x;
		float vy = a.y - b.y;
		float vz = a.z - b.z;
		return (vx*vx + vy*vy + vz*vz);
}

void CDefaultVec3Implementation::Invert(Vec3& vec)
{
	vec.x = -vec.x;
	vec.y = -vec.y;
	vec.z = -vec.z;
}

void CDefaultVec3Implementation::Normalize(Vec3 &v) const
{
	float length = Length(v);
	if( length != 0.0f )
	{
		length = 1/length;
		v.x *= length;
		v.y *= length;
		v.z *= length;
	}
}

void CDefaultVec3Implementation::RotateAboutX(Vec3 &v, float degRotation)
{
	float radAngle = degRotation * ((float)M_PI / 180.0f);
	float sinTheta = sin(radAngle);
	float cosTheta = cos(radAngle);

	float newY = (v.z * sinTheta) + (v.y * cosTheta);
	float newZ = (v.z * cosTheta) - (v.y * sinTheta);

	v.SetY(newY);
	v.SetZ(newZ);
}
	
void CDefaultVec3Implementation::RotateAboutY(Vec3 &v, float degRotation)
{
	float radAngle = degRotation * ((float)M_PI / 180.0f);
	float sinTheta = sin(radAngle);
	float cosTheta = cos(radAngle);

	float newX = (v.z * sinTheta) + (v.x * cosTheta);
	float newZ = (v.z * cosTheta) - (v.x * sinTheta);

	v.x = newX;
	v.z = newZ;
}

void CDefaultVec3Implementation::RotateAboutZ(Vec3 &v, float degRotation)
{
	float radAngle = degRotation * ((float)M_PI / 180.0f);
	float sinTheta = sin(radAngle);
	float cosTheta = cos(radAngle);

	float newX = (v.y * sinTheta) + (v.x * cosTheta );
	float newY = (v.y * cosTheta) - (v.x * sinTheta );

	v.x = newX;
	v.y = newY;
}