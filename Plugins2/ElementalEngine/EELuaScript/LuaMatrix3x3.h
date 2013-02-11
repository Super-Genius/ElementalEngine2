///============================================================================
/// \file		LuaMatrix3x3.h
/// \brief		Header file for CLuaMatrix3x3
/// \date		08-25-2005
/// \author		Josh Lulewicz
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

///============================================================================

#ifndef _CLUAMATRIX3X3_
#define _CLUAMATRIX3X3_

// tolua_begin
class CLuaMatrix3x3 : public Matrix3x3
{
public:

	CLuaMatrix3x3()
	{
		SetIdentity();
	}

	CLuaMatrix3x3( Matrix3x3& other )
	{
		m[0] = other.m[0]; m[1] = other.m[1]; m[2] = other.m[2];
		m[3] = other.m[3]; m[4] = other.m[4]; m[5] = other.m[5];
		m[6] = other.m[6]; m[7] = other.m[7]; m[8] = other.m[8];
	}

//	CLuaMatrix3x3( float* newM )
//	{
//		m[0] = newM[0]; m[1] = newM[1]; m[2] = newM[2];
//		m[3] = newM[3]; m[4] = newM[4]; m[5] = newM[5];
//		m[6] = newM[6]; m[7] = newM[7]; m[8] = newM[8];
//	}

	CLuaMatrix3x3( float m00, float m01, float m02, 
			   float m10, float m11, float m12, 
			   float m20, float m21, float m22 )
	{
		Set( m00, m01, m02, m10, m11, m12, m20, m21, m22 );
	}

	CLuaMatrix3x3( const Vec3 &vec1, const Vec3 &vec2, const Vec3 &vec3 )
	{
		m[0]=vec1.x; m[1]=vec2.x; m[2]=vec3.x;
		m[3]=vec1.y; m[4]=vec2.y; m[5]=vec3.y;
		m[6]=vec1.z; m[7]=vec2.z; m[8]=vec3.z;
	}

	inline void Set( float m00, float m01, float m02, 
					 float m10, float m11, float m12, 
					 float m20, float m21, float m22 )
	{
		m_Matrix3x3Implementation->Set( *this, m00, m01, m02, m10, m11, m12, m20, m21, m22 );
	}

//	inline void SetFrom3x3( float* newM )
//	{
//		m_Matrix3x3Implementation->SetFrom3x3( *this, newM );
//	}

//	inline void SetFrom4x4( float* newM )
//	{
//		m_Matrix3x3Implementation->SetFrom4x4( *this, newM );
//	}

	inline void SetFromAxisAngle( const Vec3& axis, float angle )
	{
		m_Matrix3x3Implementation->SetFromAxisAngle( *this, axis, angle );
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
		return m_Matrix3x3Implementation->GetAxisX( *this );
	}

	inline Vec3 GetAxisY()
	{
		return m_Matrix3x3Implementation->GetAxisY( *this );
	}

	inline Vec3 GetAxisZ()
	{
		return m_Matrix3x3Implementation->GetAxisZ( *this );
	}

	inline float* GetMatrix()
	{
		return m;
	}

	inline bool Equals( const Matrix3x3& b ) const
	{
		return m_Matrix3x3Implementation->Equals( *this, b );
	}

	inline bool NotEquals( const Matrix3x3& b ) const
	{
		return m_Matrix3x3Implementation->NotEquals( *this, b );
	}

	inline Matrix3x3 Times( const Matrix3x3& b ) const
	{
		return m_Matrix3x3Implementation->Times( *this, b );
	}

	inline Matrix3x3& TimesEquals( const Matrix3x3& b )
	{
		m_Matrix3x3Implementation->TimesEquals( *this, b );
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
		return m_Matrix3x3Implementation->IsIdentity( *this );
	}

	inline void SetRotation( const EulerAngle& euler )
	{
		m_Matrix3x3Implementation->SetRotation( *this, euler );
	}

	inline bool SetInverse()
	{
		return m_Matrix3x3Implementation->SetInverse( *this );
	}

	inline bool GetInverse( Matrix3x3& out )
	{
		return m_Matrix3x3Implementation->GetInverse( *this, out );
	}

//	inline void Normalize()
//	{
//		m_Matrix3x3Implementation->Normalize( *this );
//	}

	inline void Transpose()
	{
		m_Matrix3x3Implementation->Transpose( *this );
	}

// tolua_end

	/// Setup the implemenation for this math lib
	static void InitImplementation();

private:
	/// Pointer to implementation of the math library
	static IMatrix3x3Implementation *m_Matrix3x3Implementation;

// tolua_begin
};
// tolua_end

#endif // #ifndef _CLUAMATRIX3X3_
