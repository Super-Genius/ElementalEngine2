///============================================================================
/// \file		LuaQuaternion.h
/// \brief		Header file for CLuaQuaternion
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

#ifndef	_CLUAQUATERNION_H_
#define	_CLUAQUATERNION_H_

// tolua_begin
class CLuaQuaternion : public Quaternion
{
public:
	CLuaQuaternion()			
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;
	}
	CLuaQuaternion( float ax, float ay, float az, float aw )
	{
		x = ax;
		y = ay;
		z = az;	
		w = aw;
	}
	CLuaQuaternion( const Quaternion& a )			
	{
		x = a.x;
		y = a.y;
		z = a.z;
		w = a.w;
	}
	CLuaQuaternion( const float vec[3] )
	{
		x = vec[0];
		y = vec[1];
		z = vec[2];
		w = 1.0f;
	}
	~CLuaQuaternion() {};
	//...

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

	inline void SetW( float newW )
	{
		w = newW;
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

	inline float GetW() const
	{
		return w;
	}
	//...

	inline void Set( float ax, float ay, float az, float aw )
	{
		m_QuaternionImplementation->Set( *this, ax, ay, az, aw );
	}

	inline bool Equals( const Quaternion& b )
	{
		return m_QuaternionImplementation->Equals( *this, b );
	}

	inline Quaternion Add( const Quaternion& b )
	{
		return m_QuaternionImplementation->Add( *this, b );
	}

	inline Quaternion Times( const Quaternion& b )
	{
		return m_QuaternionImplementation->Times( *this, b );
	}

	inline void TimesEquals( const Quaternion& b )
	{
		m_QuaternionImplementation->TimesEquals( *this, b );
	}

	inline Quaternion Times( float f )
	{
		return m_QuaternionImplementation->Times( *this, f );
	}

	inline void TimesEquals( float f )
	{
		m_QuaternionImplementation->TimesEquals( *this, f );
	}

	inline float DotProduct( const Quaternion& b )
	{
		return m_QuaternionImplementation->DotProduct( *this, b );
	}

	inline void Normalize()
	{
		m_QuaternionImplementation->Normalize( *this );
	}

	inline void GetMatrix( Matrix4x4& out )
	{
		m_QuaternionImplementation->GetMatrix( *this, out );
	}

	inline void SetFromMatrix( const Matrix3x3& in )
	{
		m_QuaternionImplementation->SetFromMatrix( *this, in );
	}

	inline void SetFromMatrix( const Matrix4x4& in )
	{
		m_QuaternionImplementation->SetFromMatrix( *this, in );
	}

	inline void MakeInverse()
	{
		m_QuaternionImplementation->MakeInverse( *this );
	}

	inline Quaternion Slerp( Quaternion b, float time )
	{
		return m_QuaternionImplementation->Slerp( *this, b, time );
	}

	inline void ToEuler( EulerAngle& euler )
	{
		m_QuaternionImplementation->ToEuler( *this, euler );
	}

	inline void FromEuler( EulerAngle& euler )
	{
		m_QuaternionImplementation->FromEuler( *this, euler );
	}

// tolua_end

	/// Setup the implemenation for this Quaternion lib
	static void InitImplementation();

private:

	/// Quaternion implementation
	static IQuaternionImplementation *m_QuaternionImplementation;

// tolua_begin
};
// tolua_end

#endif	// _CLUAQUATERNION_H_