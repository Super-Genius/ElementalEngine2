///==========================================================================
/// \file	Quaternion.h
/// \brief	
/// \date	08/02/2006
/// \author	Kyle Swaim
/// \note   Elemental Engine
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

#ifndef QUATERNION_H__
#define QUATERNION_H__

#include "IQuaternionImplementation.h"

namespace ElementalEngine
{

int SetQuaternionImplementation();

class Quaternion
{
private:
	friend int SetQuaternionImplementation();
	static IQuaternionImplementation *impl;

public:
	float x, y, z, w;

	static void Init(IQuaternionImplementation* quatImpl)
	{
		impl = quatImpl;
	}

	Quaternion() : 
		x( 0 ), 
		y( 0 ), 
		z( 0 ),
		w( 0 )
	{
		if(impl == NULL)
		{
			SetQuaternionImplementation();
		}
	}
	
	Quaternion( float ax, float ay, float az, float aw ) : 
		x( ax ), 
		y( ay ), 
		z( az ),	
		w( aw )
	{
		if(impl == NULL)
		{
			SetQuaternionImplementation();
		}
	}
	
	//...
	inline void Set( float ax, float ay, float az, float aw )
	{
		impl->Set( *this, ax, ay, az, aw );
	}

	inline void Set( const Quaternion& q )
	{
		impl->Set( *this, q.x, q.y, q.z, q.w );
	}
	//...

    inline void operator= ( const Quaternion& b )
	{
		return impl->Set( *this, b.x, b.y, b.z, b.w );
	}

	inline bool operator== ( const Quaternion& b )
	{
		return impl->Equals( *this, b );
	}

	inline Quaternion operator+ ( const Quaternion& b ) const
	{
		return impl->Add( *this, b );
	}

	inline Quaternion operator* ( const Quaternion& b ) const
	{
		return impl->Times( *this, b );
	}

	inline Quaternion& operator*= ( const Quaternion& b )
	{
		impl->TimesEquals( *this, b );
		return *this;
	}

	inline Quaternion operator* (float f) const
	{
		return impl->Times( *this, f );
	}

	inline Quaternion& operator*= (float f)
	{
		impl->TimesEquals( *this, f );
		return *this;
	}

	inline Quaternion operator/ ( const Quaternion& b ) const
	{
		return impl->Divide( *this, b );
	}

	inline Quaternion& operator/= ( const Quaternion& b )
	{
		impl->DivideEquals( *this, b );
		return *this;
	}

	inline float DotProduct( const Quaternion& b ) const
	{
		return impl->DotProduct( *this, b );
	}

	inline void Normalize()
	{
		impl->Normalize( *this );
	}

	inline void GetMatrix(Matrix4x4& out) const
	{
		impl->GetMatrix( *this, out );
	}

	inline void SetFromMatrix( const Matrix3x3& in )
	{
		impl->SetFromMatrix( *this, in );
	}

	inline void SetFromMatrix( const Matrix4x4& in )
	{
		impl->SetFromMatrix( *this, in );
	}

	inline void MakeInverse() // Invert()?
	{
		impl->MakeInverse( *this );
	}

	inline Quaternion Slerp( Quaternion b, float time )
	{
		return impl->Slerp( *this, b, time );
	}

	inline void ToEuler( EulerAngle& euler )
	{
		impl->ToEuler( *this, euler );
	}

	inline void FromEuler( EulerAngle& euler )
	{
		impl->FromEuler( *this, euler );
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

#endif //QUATERNION_H__