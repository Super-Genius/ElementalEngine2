///==========================================================================
/// \file	EulerAngle.h
/// \brief	
/// \date	07/16/2007
/// \author	Marvin Gouw
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

#ifndef EULERANGLE_H__
#define EULERANGLE_H__

#include "IEulerImplementation.h"

namespace ElementalEngine
{

int SetEulerImplementation();

enum EULER_ORDER { EULER_ORDER_YPR,
					EULER_ORDER_YRP,
					EULER_ORDER_RPY,
					EULER_ORDER_RYP,
					EULER_ORDER_PRY,
					EULER_ORDER_PYR };
// tolua_begin
class EulerAngle
{
public:
	float roll, pitch, yaw;
    EULER_ORDER order;

	static void Init(IEulerImplementation* eulerImpl)
	{
		impl = eulerImpl;
	}

	EulerAngle() : 
		roll( 0 ), 
		pitch( 0 ), 
		yaw( 0 ),
		order( EULER_ORDER_YPR )
	{
		if(impl == NULL)
		{
			SetEulerImplementation();
		}
	}
	
	EulerAngle( float in_roll, float in_pitch, float in_yaw ):
		roll( in_roll ),
		pitch( in_pitch ),
		yaw( in_yaw ),
		order( EULER_ORDER_YPR )
	{
		if(impl == NULL)
		{
			SetEulerImplementation();
		}
	}
	
	EulerAngle( const EulerAngle& a ) : 
		roll( a.roll ), 
		pitch( a.pitch ), 
		yaw( a.yaw ),
		order( EULER_ORDER_YPR )
	{
		if(impl == NULL)
		{
			SetEulerImplementation();
		}
	}
	inline void SetRoll( float newX )
	{
		roll = newX;
	}

	inline void SetPitch( float newY )
	{
		pitch = newY;
	}

	inline void SetYaw( float newZ )
	{
		yaw = newZ;
	}
	//...

	inline float GetRoll() const
	{
		return roll;
	}

	inline float GetPitch() const
	{
		return pitch;
	}

	inline float GetYaw() const
	{
		return yaw;
	}
	//...
	inline void Set( float in_roll, float in_pitch, float in_yaw  )
	{
		impl->Set( *this, in_roll, in_pitch, in_yaw );
	}

	inline void Set( const EulerAngle& e )
	{
		impl->Set( *this, e.roll, e.pitch, e.yaw );
	}
	//...

	inline void GetMatrix(Matrix4x4& out) const
	{
		impl->GetMatrix( *this, out );
	}

	inline void GetMatrix(Matrix3x3& out) const
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


	inline void ToQuaternion( Quaternion & quat )
	{
		impl->ToQuaternion( *this, quat );
	}

	inline void FromQuaternion( Quaternion &quat )
	{
		impl->FromQuaternion( *this, quat );
	}

	inline bool IsValid()
	{
		// security check for good numbers
		if (_isnan(roll) ||
			_isnan(pitch) ||
			_isnan(yaw))
		{
			return false;
		}

		if (!_finite(roll) ||
			!_finite(pitch) ||
			!_finite(yaw) )
		{
			return false;
		}

		return true;
	}

// tolua_end

	inline void operator= ( const EulerAngle& b )
	{
		return impl->Set( *this, b.roll, b.pitch, b.yaw );
	}

	inline bool operator== ( const EulerAngle & b )
	{
		return impl->Equals( *this, b );
	}

	inline bool operator!= ( const EulerAngle & b )
	{
		return !impl->Equals( *this, b );
	}

	inline EulerAngle operator+ ( const EulerAngle& b ) const
	{
		return impl->Add( *this, b );
	}

	inline EulerAngle operator- ( const EulerAngle& b ) const
	{
		return impl->Subtract( *this, b );
	}

	inline EulerAngle operator* (float f) const
	{
		return impl->Times( *this, f );
	}

	inline EulerAngle& operator*= (float f)
	{
		impl->TimesEquals( *this, f );
		return *this;
	}

private:
	friend int SetEulerImplementation();
	static IEulerImplementation *impl;
// tolua_begin
};
// tolua_end

} //namespace ElementalEngine

#ifndef EE_SDK_EXPORTS
#pragma comment(lib, "EE-SDK.lib")
#endif

#endif //EULERANGLE_H__