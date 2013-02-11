///==========================================================================
/// \file	DefaultVec4Implementation.cpp
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
#include "DefaultVec4Implementation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IVec4Implementation *CDefaultVec4Implementation::s_instance = NULL;

void CDefaultVec4Implementation::Init( void )
{
}

void CDefaultVec4Implementation::CleanUp( void )
{
}

bool CDefaultVec4Implementation::Equals(const Vec4& a, const Vec4& b) const
{
	return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w));
}

bool CDefaultVec4Implementation::NotEquals(const Vec4& a, const Vec4& b) const
{
	return ((a.x != b.x) || (a.y != b.y) || (a.z != b.z) || (a.w != b.w));
}

bool CDefaultVec4Implementation::LessThan(const Vec4& a, const Vec4& b) const
{
	return ((a.x < b.x) && (a.y < b.y) && (a.z < b.z) && (a.w < b.w));
}
bool CDefaultVec4Implementation::GreaterThan(const Vec4& a, const Vec4& b) const
{
	return ((a.x > b.x) && (a.y > b.y) && (a.z > b.z) && (a.w > b.w));
}

Vec4 CDefaultVec4Implementation::Add( const Vec4& a, const Vec4& b ) const
{
	return Vec4( 
		a.x + b.x, 
		a.y + b.y, 
		a.z + b.z, 
		a.w + b.w );
}

Vec4& CDefaultVec4Implementation::AddEquals( Vec4& a, const Vec4& b ) const
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	a.w += b.w;
	return a;
}

Vec4 CDefaultVec4Implementation::Subtract(const Vec4& a, const Vec4& b) const
{
	return Vec4(
		a.x - b.x, 
		a.y - b.y, 
		a.z - b.z, 
		a.w - b.w );
}

Vec4& CDefaultVec4Implementation::SubtractEquals( Vec4& a, const Vec4& b ) const
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	a.w -= b.w;
	return a;
}

Vec4 CDefaultVec4Implementation::Times(const Vec4& a, const Vec4& b) const
{
	return Vec4(
		a.x * b.x, 
		a.y * b.y, 
		a.z * b.z, 
		a.w * b.w );
}

const Vec4& CDefaultVec4Implementation::TimesEquals(Vec4& a, const Vec4& b) const
{
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	a.w *= b.w;
	return a;
}

Vec4 CDefaultVec4Implementation::Times(const Vec4& a, float scalar) const
{
	return Vec4(
		a.x * scalar, 
		a.y * scalar, 
		a.z * scalar, 
		a.w * scalar );
}

const Vec4& CDefaultVec4Implementation::TimesEquals(Vec4& a, float scalar) const
{
	a.x *= scalar;
	a.y *= scalar;
	a.z *= scalar;
	a.w *= scalar;
	return a;
}

Vec4 CDefaultVec4Implementation::Divide(const Vec4& a, const Vec4& b) const
{
	return Vec4(
		a.x / b.x, 
		a.y / b.y, 
		a.z / b.z, 
		a.w / b.w );
}

const Vec4& CDefaultVec4Implementation::DivideEquals(Vec4& a, const Vec4& b) const
{
	a.x /= b.x;
	a.y /= b.y;
	a.z /= b.z;
	a.w /= b.w;
	return a;
}

Vec4 CDefaultVec4Implementation::Divide(const Vec4& a,  float scalar) const
{
	return Vec4(
		a.x / scalar, 
		a.y / scalar, 
		a.z / scalar, 
		a.w / scalar );
}

const Vec4& CDefaultVec4Implementation::DivideEquals(Vec4& a,  float scalar) const
{
	a.x /= scalar;
	a.y /= scalar;
	a.z /= scalar;
	a.w /= scalar;
	return a;
}

float CDefaultVec4Implementation::Length(const Vec4& vec) const
{
	return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z + vec.w*vec.w);
}

float CDefaultVec4Implementation::LengthSQ(const Vec4& vec) const
{
	return (vec.x*vec.x + vec.y*vec.y + vec.z*vec.z + vec.w*vec.w);
}

void CDefaultVec4Implementation::SetLength(Vec4& vec, float newLength)
{
	Normalize(vec);
	TimesEquals(vec, newLength);
}

float CDefaultVec4Implementation::Dot(const Vec4& a, const Vec4& b) const
{
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w));
}

void CDefaultVec4Implementation::Normalize(Vec4 &v) const
{
	float length = Length(v);
	if( length != 0.0f )
	{
		length = 1/length;
		v.x *= length;
		v.y *= length;
		v.z *= length;
		v.w *= length;
	}
}
