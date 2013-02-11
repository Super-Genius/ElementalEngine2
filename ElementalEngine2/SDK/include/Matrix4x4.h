///==========================================================================
/// \file	Matrix4x4
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

#ifndef MATRIX4X4_H__
#define MATRIX4X4_H__

#include "Vec3.h"
#include "Vec4.h"
#include "EulerAngle.h"
#include "IMatrix4x4Implementation.h"
#include "Matrix3x3.h"

namespace ElementalEngine
{

int SetMatrix4x4Implementation();

class Matrix4x4
{
private:
	friend int SetMatrix4x4Implementation();
	static IMatrix4x4Implementation *impl;

public:
	float m[16];

	static void Init( IMatrix4x4Implementation* matImpl )
	{
		impl = matImpl;
	}

	Matrix4x4()
	{
		if (impl == NULL)
		{
			SetMatrix4x4Implementation();
		}

		SetIdentity();
	}

	Matrix4x4( float* newM )
	{
		if (impl == NULL)
		{
			SetMatrix4x4Implementation();
		}

		m[0]  = newM[0];  m[1]  = newM[1];  m[2]  = newM[2];  m[3]  = newM[3]; 
		m[4]  = newM[4];  m[5]  = newM[5];  m[6]  = newM[6];  m[7]  = newM[7]; 
		m[8]  = newM[8];  m[9]  = newM[9];  m[10] = newM[10]; m[11] = newM[11]; 
		m[12] = newM[12]; m[13] = newM[13]; m[14] = newM[14]; m[15] = newM[15];
	}

	Matrix4x4( float m00, float m01, float m02, float m03,
			   float m10, float m11, float m12, float m13,
			   float m20, float m21, float m22, float m23,
			   float m30, float m31, float m32, float m33 )
	{
		if (impl == NULL)
		{
			SetMatrix4x4Implementation();
		}

		Set( m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33 );
	}
	
	Matrix4x4(const Vec3 &forwardVec, const Vec3 &upVec, const Vec3 &rightVec, const Vec3 &translation)
	{
		if (impl == NULL)
		{
			SetMatrix4x4Implementation();
		}

		SetFromVectors(forwardVec, upVec, rightVec, translation);
	}

	inline void Set( float m00, float m01, float m02, float m03,
					 float m10, float m11, float m12, float m23,
					 float m20, float m21, float m22, float m13,
					 float m30, float m31, float m32, float m33 )
	{
		impl->Set( *this, m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33 );
	}

	inline void SetFrom3x3( float* newM )
	{
		impl->SetFrom3x3( *this, newM );
	}

	inline void SetFrom4x4( float* newM )
	{
		impl->SetFrom4x4( *this, newM );
	}

	inline void SetFromVectors(const Vec3 &forwardVec, const Vec3 &upVec, 
								const Vec3 &rightVec, const Vec3 &translation)
	{
		impl->SetFromVectors(*this, forwardVec, upVec, rightVec, translation);
	}

	inline float& Get( int row, int col )
	{
		return m[col * 4 + row];
	}

	inline const float& Get( int row, int col ) const
	{
		return m[col * 4 + row];
	}

	inline float* GetMatrix()
	{
		return m;
	}

	inline bool operator== ( const Matrix4x4& b ) const
	{
		return impl->Equals( *this, b );
	}

	inline Matrix4x4 operator= ( const Matrix4x4& b )
	{
		impl->SetFrom4x4( *this, (float*)b.m );
		return (*this);
	}

	inline bool operator!= ( const Matrix4x4& b ) const
	{
		return impl->NotEquals( *this, b );
	}

	inline Matrix4x4 operator* ( const Matrix4x4& b ) const
	{
		return impl->Times( *this, b );
	}

	inline Vec3 operator*( const Vec3 &vec )
	{
		return impl->TransformVector( *this, vec );
	}

	inline Vec4 operator*( const Vec4 &vec )
	{
		return impl->TransformVector( *this, vec );
	}

	inline void SetTranspose()
	{
		return impl->Transpose( *this );
	}

	inline Matrix4x4& operator*= ( const Matrix4x4& b )
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
		m[4] = 0;
		m[5] = 1;
		m[6] = 0;
		m[7] = 0;
		m[8] = 0;
		m[9] = 0;
		m[10] = 1;
		m[11] = 0;
		m[12] = 0;
		m[13] = 0;
		m[14] = 0;
		m[15] = 1;
	}

	inline bool IsIdentity()
	{
		return impl->IsIdentity( *this );
	}

	inline void SetTranslation( const Vec3& trans )
	{
		impl->SetTranslation( *this, trans );
	}

	inline void SetRotation( const EulerAngle& euler )
	{
		impl->SetRotation( *this, euler );
	}

	inline void SetScale( const Vec3& scale )
	{
		impl->SetScale( *this, scale );
	}

	inline void SetScaleWithTranslation( const Vec3 &scale )
	{
		impl->SetScaleWithTranslation(*this, scale);
	}

	inline void ResetScale()
	{
		impl->ResetScale(*this);
	}

	inline void ApplyScale( const Vec3 &scale )
	{
		impl->ApplyScale(*this, scale);
	}

	inline bool SetInverse()
	{
		return impl->SetInverse( *this );
	}

	inline bool GetInverse( Matrix4x4& out )
	{
		return impl->GetInverse( *this, out );
	}

	inline Vec3 GetTranslation()
	{
		return impl->GetTranslation( *this );
	}

	inline EulerAngle GetRotation()
	{
		return impl->GetRotation( *this );
	}

	inline Vec3 GetScale()
	{
		return impl->GetScale( *this );
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

	inline Vec3 TransformDirection(const Vec3 &vec)
	{
		return impl->TransformDirection( *this, vec );
	}

	inline bool Parity()
	{
		return impl->Parity( *this );
	}

	inline Matrix3x3 Get3x3()
	{
		return impl->Get3x3(*this);
	}

	inline Vec4 GetRow( int row )
	{
		return impl->GetRow( *this, row );
	}
};

} //namespace ElementalEngine

#ifndef EE_SDK_EXPORTS
#pragma comment(lib, "EE-SDK.lib")
#endif

#endif //MATRIX4X4_H__