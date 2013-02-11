///==========================================================================
/// \file	Vec4.h
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

#ifndef VEC4_H__
#define VEC4_H__

#include "IVec4Implementation.h"
#include <float.h>

namespace ElementalEngine
{

int SetVec4Implementation();

class Vec4
{
private:
	friend int SetVec4Implementation();
	static IVec4Implementation *impl;

public:
	float x, y, z, w;
	static void Init(IVec4Implementation *vectorImplementation)
	{
		impl = vectorImplementation;
	}

	Vec4() : 
		x( 0 ), 
		y( 0 ), 
		z( 0 ),
		w( 0 )
	{
		if (impl == NULL)
		{
			SetVec4Implementation();
		}
	}
	
	Vec4( float ax, float ay, float az, float aw ) : 
		x( ax ), 
		y( ay ), 
		z( az ), 
		w( aw )
	{
		if (impl == NULL)
		{
			SetVec4Implementation();
		}
	}
	
	Vec4( const float vec[4] ) :
		x( vec[0] ),
		y( vec[1] ),
		z( vec[2] ),
		w( vec[3] )
	{
		if (impl == NULL)
		{
			SetVec4Implementation();
		}
	}
	//...

	inline void Set( float ax, float ay, float az, float aw )
	{
		x = ax;
		y = ay;
		z = az;
		w = aw;
	}

	inline void Set( const Vec4& v )
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	inline void Set( const float v[4] )
	{
		x = v[0];
		y = v[1];
		z = v[2];
		w = v[3];
	}
	//..
	inline void SetX( float newX ){ x = newX; }
	inline void SetY( float newY ){ y = newY; }
	inline void SetZ( float newZ ){ z = newZ; }
	inline void SetW( float newW ){ w = newW; }
	//...
	inline float GetX() const { return x; }
	inline float GetY() const { return y; }
	inline float GetZ() const { return z; }
	inline float GetW() const { return w; }
	//...

	inline void operator= ( const Vec4& b )
	{
		x = b.x;
		y = b.y;
		z = b.z;
		w = b.w;
	}

	inline bool operator== ( const Vec4& b ) const
	{
		return impl->Equals( *this, b );
	}

	inline bool operator!= ( const Vec4& b ) const
	{
		return impl->NotEquals( *this, b );
	}

	inline bool operator<= ( const Vec4& b ) const
	{
		return impl->LessThan( *this, b );
	}

	inline bool operator>= ( const Vec4& b ) const
	{
		return impl->GreaterThan( *this, b );
	}
	//...

	inline Vec4 operator+ ( const Vec4& b ) const
	{
		return impl->Add( *this, b );
	}

	inline Vec4& operator+= ( const Vec4& b )
	{
		impl->AddEquals( *this, b );
		return *this;
	}

	inline Vec4 operator- ( const Vec4& b ) const
	{
		return impl->Subtract( *this, b );
	}

	inline Vec4& operator-= ( const Vec4& b )
	{
		impl->SubtractEquals( *this, b );
		return *this;
	}

	inline Vec4 operator* ( const Vec4& b ) const
	{
		return impl->Times( *this, b);
	}

	inline Vec4& operator*= ( const Vec4& b )
	{
		impl->TimesEquals( *this, b );
		return *this;
	}

	inline Vec4 operator* ( float f ) const
	{
		return impl->Times( *this, f);
	}

	inline Vec4& operator*= ( float f )
	{
		impl->TimesEquals( *this, f );
		return *this;
	}

	inline Vec4 operator/ ( const Vec4& b ) const
	{
		return impl->Divide( *this, b );
	}

	inline Vec4& operator/= ( const Vec4& b )
	{
		impl->DivideEquals( *this, b );
		return *this;
	}

	inline Vec4 operator/ ( float f ) const
	{
		return impl->Divide( *this, f );
	}

	inline Vec4& operator/= ( float f )
	{
		impl->DivideEquals( *this, f );
		return *this;
	}

	inline Vec4 operator- () const
	{
		return Vec4(-x, -y, -z, -w);
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

	inline float Dot( const Vec4& b ) const
	{
		return impl->Dot( *this, b );
	}

	inline bool IsValid()
	{
		// security check for good numbers
		if (_isnan(x) ||
			_isnan(y) ||
			_isnan(z) ||
			_isnan(w) )
		{
			return false;
		}

		if (!_finite(x) ||
			!_finite(y) ||
			!_finite(z) ||
			!_finite(w) )
		{
			return false;
		}

		return true;
	}
};

} //namespace ElementalEngine

#ifndef EE_SDK_EXPORTS
#pragma comment(lib, "EE-SDK.lib")
#endif

#endif // VEC4_H__