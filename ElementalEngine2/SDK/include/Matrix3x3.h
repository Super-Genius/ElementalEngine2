///==========================================================================
/// \file	Matrix3x3.h
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

#ifndef MATRIX3X3_H__
#define MATRIX3X3_H__

#include "IMatrix3x3Implementation.h"

namespace ElementalEngine
{

int SetMatrix3x3Implementation();

class Matrix3x3
{
private:
	friend int SetMatrix3x3Implementation();
	static IMatrix3x3Implementation *impl;

public:
	float m[9];

	static void Init(IMatrix3x3Implementation* matImpl)
	{
		impl = matImpl;
	}

	Matrix3x3()
	{
		if (impl == NULL)
		{
			SetMatrix3x3Implementation();
		}

		SetIdentity();
	}

	Matrix3x3( float* newM )
	{
		if (impl == NULL)
		{
			SetMatrix3x3Implementation();
		}

		m[0] = newM[0]; m[1] = newM[1]; m[2] = newM[2];
		m[3] = newM[3]; m[4] = newM[4]; m[5] = newM[5];
		m[6] = newM[6]; m[7] = newM[7]; m[8] = newM[8];
	}

	Matrix3x3( float m00, float m01, float m02, 
			   float m10, float m11, float m12, 
			   float m20, float m21, float m22 )
	{
		if (impl == NULL)
		{
			SetMatrix3x3Implementation();
		}

		Set( m00, m01, m02, m10, m11, m12, m20, m21, m22 );
	}

	Matrix3x3( const Vec3 &vec1, const Vec3 &vec2, const Vec3 &vec3 )
	{
		if (impl == NULL)
		{
			SetMatrix3x3Implementation();
		}

		m[0]=vec1.x; m[1]=vec2.x; m[2]=vec3.x;
		m[3]=vec1.y; m[4]=vec2.y; m[5]=vec3.y;
		m[6]=vec1.z; m[7]=vec2.z; m[8]=vec3.z;
	}

	inline void Set( float m00, float m01, float m02, 
					 float m10, float m11, float m12, 
					 float m20, float m21, float m22 )
	{
		impl->Set( *this, m00, m01, m02, m10, m11, m12, m20, m21, m22 );
	}

	inline void SetFrom3x3( float* newM )
	{
		impl->SetFrom3x3( *this, newM );
	}

	inline void SetFrom4x4( float* newM )
	{
		impl->SetFrom4x4( *this, newM );
	}

	inline bool Parity()
	{
		return impl->Parity( *this );
	}

	inline void SetFromAxisAngle( const Vec3& axis, float angle )
	{
		impl->SetFromAxisAngle( *this, axis, angle );
	}
	
	inline float& Get( int row, int col )
	{
		return m[col * 3 + row];
	}

	inline const float& Get( int row, int col ) const
	{
		return m[col * 3 + row];
	}

	inline Vec3 GetAxisX()
	{
		return impl->GetAxisX( *this );
	}

	inline Vec3 GetAxisY()
	{
		return impl->GetAxisY( *this );
	}

	inline Vec3 GetAxisZ()
	{
		return impl->GetAxisZ( *this );
	}

	inline float* GetMatrix()
	{
		return m;
	}

	inline bool Equals( const Matrix3x3& b ) const
	{
		return impl->Equals( *this, b );
	}

	inline bool NotEquals( const Matrix3x3& b ) const
	{
		return impl->NotEquals( *this, b );
	}

	inline Matrix3x3 Times( const Matrix3x3& b ) const
	{
		return impl->Times( *this, b );
	}

	inline Matrix3x3& TimesEquals( const Matrix3x3& b )
	{
		impl->TimesEquals( *this, b );
		return *this;
	}

	inline void SetIdentity()
	{
		m[0] = 1;
		m[1] = 0;
		m[2] = 0;
		m[3] = 0;
		m[4] = 1;
		m[5] = 0;
		m[6] = 0;
		m[7] = 0;
		m[8] = 1;
	}

	inline bool IsIdentity()
	{
		return impl->IsIdentity( *this );
	}

	inline void SetRotation( const EulerAngle& euler )
	{
		impl->SetRotation( *this, euler );
	}

	inline bool SetInverse()
	{
		return impl->SetInverse( *this );
	}

	inline bool GetInverse( Matrix3x3& out )
	{
		return impl->GetInverse( *this, out );
	}

//	inline void Normalize()
//	{
//		impl->Normalize( *this );
//	}

	inline void Transpose()
	{
		impl->Transpose( *this );
	}
	
	inline Vec3 operator*( const Vec3 &vec )
	{
		return impl->TransformVector(*this, vec);
	}
	
	inline Matrix3x3 operator*(const Matrix3x3 &mat)
	{
		return impl->Times( *this, mat );
	}
};

} //namespace ElementalEngine

#ifndef EE_SDK_EXPORTS
#pragma comment(lib, "EE-SDK.lib")
#endif

#endif //MATRIX3X3_H__