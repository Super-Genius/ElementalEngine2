///============================================================================
/// \file		LuaMatrix4x4.h
/// \brief		Header file for CLuaMatrix4x4
/// \date		12-18-2006
/// \author		Kyle Swaim
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

#ifndef	_CLUAMATRIX4X4_H_
#define	_CLUAMATRIX4X4_H_

// tolua_begin
class CLuaMatrix4x4 : public Matrix4x4
{
public:
	CLuaMatrix4x4()			
	{
		SetIdentity();
	}

	CLuaMatrix4x4( const Matrix4x4& other )			
	{
		m[0]  = other.m[0];  m[1]  = other.m[1];  m[2]  = other.m[2];  m[3]  = other.m[3];
		m[4]  = other.m[4];  m[5]  = other.m[5];  m[6]  = other.m[6];  m[7]  = other.m[7];
		m[8]  = other.m[8];  m[9]  = other.m[9];  m[10] = other.m[10]; m[11] = other.m[11];
		m[12] = other.m[12]; m[13] = other.m[13]; m[14] = other.m[14]; m[15] = other.m[15];
	}

	CLuaMatrix4x4( float m00, float m01, float m02, float m03, 
				   float m10, float m11, float m12, float m13, 
				   float m20, float m21, float m22, float m23,
				   float m30, float m31, float m32, float m33 )
	{
		Set( m00, m01, m02, m03, 
			 m10, m11, m12, m13, 
			 m20, m21, m22, m23,
			 m30, m31, m32, m33 );
	}

	CLuaMatrix4x4(const Vec3 &forwardVec, const Vec3 &upVec, const Vec3 &rightVec, const Vec3 &translation)
	{
		SetFromVectors(forwardVec, upVec, rightVec, translation);
	}

	~CLuaMatrix4x4() {};
	//...

	inline void Set( float m00, float m01, float m02, float m03, 
					 float m10, float m11, float m12, float m13, 
					 float m20, float m21, float m22, float m23,
					 float m30, float m31, float m32, float m33 )
	{
		m_Matrix4x4Implementation->Set( *this, m00, m01, m02, m03, 
												m10, m11, m12, m13, 
												m20, m21, m22, m23,
												m30, m31, m32, m33 );
	}

	inline void SetFromVectors(const Vec3 &forwardVec, const Vec3 &upVec, const Vec3 &rightVec, const Vec3 &translation)
	{
		m_Matrix4x4Implementation->SetFromVectors( *this, forwardVec, upVec, rightVec, translation);
	}

	//inline void SetFrom3x3( float* newM )
	//{
	//	m_Matrix4x4Implementation->SetFrom3x3( *this, newM );
	//}

	//inline void SetFrom4x4( float* newM )
	//{
	//	m_Matrix4x4Implementation->SetFrom4x4( this, newM );
	//}

	inline float& Get( int row, int col )
	{
		return m[col * 4 + row];
	}

	inline const float& Get( int row, int col ) const
	{
		return m[col * 4 + row];
	}

	inline bool Equals( const Matrix4x4& b )
	{
		return m_Matrix4x4Implementation->Equals( *this, b );
	}

	inline bool NotEquals( const Matrix4x4& b )
	{
		return m_Matrix4x4Implementation->NotEquals( *this, b );
	}

	inline Matrix4x4 Times( const Matrix4x4& b )
	{
		return m_Matrix4x4Implementation->Times( *this, b );
	}

	inline const Matrix4x4& TimesEquals( const Matrix4x4& b )
	{
		return m_Matrix4x4Implementation->TimesEquals( *this, b );
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
		return m_Matrix4x4Implementation->IsIdentity( *this );
	}

	inline void SetTranslation( const Vec3& trans )
	{
		m_Matrix4x4Implementation->SetTranslation( *this, trans );
	}

	inline void SetRotation( const EulerAngle& euler )
	{
		m_Matrix4x4Implementation->SetRotation( *this, euler );
	}

	inline void SetScale( const Vec3& scale )
	{
		m_Matrix4x4Implementation->SetScale( *this, scale );
	}

	inline Vec3 GetAxisX()
	{
		return m_Matrix4x4Implementation->GetAxisX( *this );
	}

	inline Vec3 GetAxisY()
	{
		return m_Matrix4x4Implementation->GetAxisY( *this );
	}

	inline Vec3 GetAxisZ()
	{
		return m_Matrix4x4Implementation->GetAxisZ( *this );
	}

	inline Vec3 GetTranslation()
	{
		return m_Matrix4x4Implementation->GetTranslation( *this );
	}

	inline EulerAngle GetRotation()
	{
		return m_Matrix4x4Implementation->GetRotation( *this );
	}

	inline Vec3 GetScale()
	{
		return m_Matrix4x4Implementation->GetScale( *this );
	}

	inline void SetInverse()
	{
		m_Matrix4x4Implementation->SetInverse( *this );
	}

	inline bool GetInverse( Matrix4x4& out )
	{
		return m_Matrix4x4Implementation->GetInverse( *this, out );
	}

	inline void Transpose( Matrix4x4& inout )
	{
		m_Matrix4x4Implementation->Transpose( inout );
	}

	inline Matrix3x3 Get3x3()
	{
		return m_Matrix4x4Implementation->Get3x3( *this );
	}

// tolua_end

	/// Setup the implemenation for this vec3 lib
	static void InitImplementation();

private:

	/// Matrix 4x4 implementation
	static IMatrix4x4Implementation *m_Matrix4x4Implementation;

// tolua_begin
};
// tolua_end

#endif	// _CLUAMATRIX4X4_H_