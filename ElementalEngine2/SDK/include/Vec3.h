///==========================================================================
/// \file	Vec3.h
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

#ifndef VEC3_H__
#define VEC3_H__

#include "IVec3Implementation.h"
#include <float.h>

namespace ElementalEngine
{

int SetVec3Implementation();

// tolua_begin
class Vec3
{
public:
	float x, y, z;

	//...

	inline void Set( float ax, float ay, float az )
	{
		x = ax;
		y = ay;
		z = az;
	}

	inline void Set( const Vec3& v )
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	inline void Set( const float v[3] )
	{
		x = v[0];
		y = v[1];
		z = v[2];
	}
	//..

	inline void SetX( float newX )
	{
		x = newX;
	}

	inline void SetY( float newY )
	{
		y = newY;
	}

	inline void SetZ( float newZ )
	{
		z = newZ;
	}
	//...

	inline float GetX() const
	{
		return x;
	}

	inline float GetY() const
	{
		return y;
	}

	inline float GetZ() const
	{
		return z;
	}
	//...

	inline bool operator<= ( const Vec3& b ) const
	{
		return impl->LessThan( *this, b );
	}

	inline Vec3 operator+ ( const Vec3& b ) const
	{
		return impl->Add( *this, b );
	}

	inline Vec3 operator- ( const Vec3& b ) const
	{
		return impl->Subtract( *this, b );
	}

	inline Vec3 operator* ( const Vec3& b ) const
	{
		return impl->Times( *this, b);
	}

	inline Vec3 operator* ( float f ) const
	{
		return impl->Times( *this, f);
	}

	inline Vec3 operator* ( const Matrix3x3& m ) const
	{
		return impl->Times( *this, m);
	}

	inline Vec3 operator/ ( const Vec3& b ) const
	{
		return impl->Divide( *this, b );
	}

	inline Vec3 operator/ ( float f ) const
	{
		return impl->Divide( *this, f );
	}

	inline Vec3 operator- () const
	{
		return Vec3(-x, -y, -z);
	}
	//...

	inline void Normalize()
	{
		impl->Normalize( *this );
	}

	inline float Length() const
	{
		return impl->Length( *this );
	}

	inline float LengthSQ() const
	{
		return impl->LengthSQ( *this );
	}

	inline void SetLength( float f )
	{
		impl->SetLength( *this, f );
	}

	inline float Dot( const Vec3& b ) const
	{
		return impl->Dot( *this, b );
	}

	inline Vec3 Cross( const Vec3& b ) const
	{
		return impl->Cross( *this, b );
	}

	inline float DistanceBetween( const Vec3& b ) const
	{
		return impl->DistanceBetween( *this, b );
	}

	inline float DistanceBetweenSQ( const Vec3& b ) const
	{
		return impl->DistanceBetweenSQ( *this, b );
	}

	inline void Invert()
	{
		impl->Invert( *this );
	}

	inline bool IsValid()
	{
		// security check for good numbers
		if (_isnan(x) ||
			_isnan(y) ||
			_isnan(z) )
		{
			return false;
		}

		if (!_finite(x) ||
			!_finite(y) ||
			!_finite(z) )
		{
			return false;
		}

		return true;
	}

	inline void RotateAboutAxisX(float degRotate)
	{
		impl->RotateAboutX(*this, degRotate);
	}

	inline void RotateAboutAxisY(float degRotate)
	{
		impl->RotateAboutY(*this, degRotate);
	}

	inline void RotateAboutAxisZ(float degRotate)
	{
		impl->RotateAboutZ(*this, degRotate);
	}

// tolua_end
	inline float& operator[](int i) { return (&x)[i]; }
	inline const float& operator[](int i) const { return (&x)[i]; }

	inline void operator= ( const Vec3& b )
	{
		x = b.x;
		y = b.y;
		z = b.z;
	}

	inline bool operator== ( const Vec3& b ) const
	{
		return impl->Equals( *this, b );
	}

	inline bool operator!= ( const Vec3& b ) const
	{
		return impl->NotEquals( *this, b );
	}

	inline bool operator>= ( const Vec3& b ) const
	{
		return impl->GreaterThan( *this, b );
	}

	inline Vec3& operator+= ( const Vec3& b )
	{
		impl->AddEquals( *this, b );
		return *this;
	}

	inline Vec3& operator-= ( const Vec3& b )
	{
		impl->SubtractEquals( *this, b );
		return *this;
	}

	inline Vec3& operator*= ( const Vec3& b )
	{
		impl->TimesEquals( *this, b );
		return *this;
	}

	inline Vec3& operator*= ( float f )
	{
		impl->TimesEquals( *this, f );
		return *this;
	}

	inline Vec3& operator*= ( const Matrix3x3& m )
	{
		impl->TimesEquals( *this, m );
		return *this;
	}

	inline Vec3& operator/= ( const Vec3& b )
	{
		impl->DivideEquals( *this, b );
		return *this;
	}

	inline Vec3& operator/= ( float f )
	{
		impl->DivideEquals( *this, f );
		return *this;
	}

	static void Init(IVec3Implementation *vectorImplementation)
	{
		impl = vectorImplementation;
	}

	Vec3() : 
		x( 0 ), 
		y( 0 ), 
		z( 0 )			
	{
		if (impl == NULL)
		{
			SetVec3Implementation();
		}
	}

	Vec3( float ax, float ay, float az ) : 
		x( ax ), 
		y( ay ), 
		z( az )	
	{
		if (impl == NULL)
		{
			SetVec3Implementation();
		}
	}
	
	Vec3( const float vec[3] ) :
		x( vec[0] ),
		y( vec[1] ),
		z( vec[2] )
	{
		if (impl == NULL)
		{
			SetVec3Implementation();
		}
	}

private:
	friend int SetVec3Implementation();
	static IVec3Implementation *impl;
// tolua_begin
};
// tolua_end

} //namespace ElementalEngine

#endif // VEC3_H__