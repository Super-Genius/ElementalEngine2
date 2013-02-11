///============================================================================
/// \file		LuaMath.h
/// \brief		Header file for CLuaMath
/// \date		08-24-2005
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

#ifndef	_CLUAMATH_H_
#define	_CLUAMATH_H_

// tolua_begin
class CLuaFloat
{
public:
	CLuaFloat(float f) { x = f; }
	float x;
};

class CLuaMath
{
public:
	CLuaMath() {};
	~CLuaMath() {};
	//...

	inline float GetPI()
	{
		return PI;
	}

	inline float GetHalfPI()
	{
		return PIDIV2;
	}

	inline float Get2PI()
	{
		return PI2;
	}

	inline float GetInversePI()
	{
		return DIVPI;
	}

	inline float GetHalfInversePI()
	{
		return DIV2PI;
	}

	inline float GetInverse360()
	{
		return DIV360;
	}

	inline float GetEpsilon()
	{
		return EPSILON;
	}
	//...

	inline float Log2( float x )
	{
		return m_MathUtilImplementation->Log2( x );
	}
	inline float LogBase( float x, float fLogBase )
	{
		return m_MathUtilImplementation->LogBase( x, fLogBase );
	}
	inline bool IsPowerOfTwo( int x )
	{
		return m_MathUtilImplementation->IsPowerOfTwo( x );
	}

	inline int NearestPowerOfTwo( int x )
	{
		return m_MathUtilImplementation->NearestPowerOfTwo( x );
	}
	//...

	inline float RadToDeg( float radValue )
	{
		return m_MathUtilImplementation->RadToDeg( radValue );
	}
	inline float DegToRad( float degValue )
	{
		return m_MathUtilImplementation->DegToRad( degValue );
	}
	inline float GetACos( float value )
	{
		return m_MathUtilImplementation->GetACos( value );
	}
	inline float GetASin( float value )
	{
		return m_MathUtilImplementation->GetASin( value );
	}
	inline float GetATan( float rise, float run )
	{
		return m_MathUtilImplementation->GetATan( rise, run );
	}
	inline float GetSin( float radAngle )
	{
		return m_MathUtilImplementation->GetSin( radAngle );
	}
	inline float GetCos( float radAngle )
	{
		return m_MathUtilImplementation->GetCos( radAngle );
	}
	inline float GetTan( float radAngle )
	{
		return m_MathUtilImplementation->GetTan( radAngle );
	}
	//...

	inline void MatrixLookAt( const Vec3& pos, const Vec3& lookAt, Matrix3x3& matrix )
	{
		m_MathUtilImplementation->MatrixLookAt( pos, lookAt, matrix );
	}
	inline void MatrixToRadEuler( const Matrix3x3& matrix, EulerAngle& angles )
	{
		m_MathUtilImplementation->MatrixToRadEuler( matrix, angles );
	}
	inline void MatrixToEuler( const Matrix3x3& matrix, EulerAngle& angles )
	{
		m_MathUtilImplementation->MatrixToEuler( matrix, angles );
	}
	inline void EulerLookAt( const Vec3 &pos, const Vec3 &lookAt, EulerAngle &angles )
	{
		m_MathUtilImplementation->EulerLookAt( pos, lookAt, angles );
	}
	inline void VectorToEuler( const Vec3 &direction, EulerAngle &angles )
	{
		m_MathUtilImplementation->VectorToEuler( direction, angles );
	}
	inline void EulerToVector( const EulerAngle &angles, Vec3 &direction )
	{
		m_MathUtilImplementation->EulerToVector( angles, direction );
	}
	inline void EulerToMatrix( const EulerAngle& angles, Matrix3x3& matrix )
	{
		m_MathUtilImplementation->EulerToMatrix( angles, matrix );
	}
	inline void EulerToMatrix( const EulerAngle& angles, Matrix4x4& matrix )
	{
		m_MathUtilImplementation->EulerToMatrix( angles, matrix );
	}
	inline void EulerToMatrix( float yaw, float pitch, float roll, Matrix3x3& matrix )
	{
		m_MathUtilImplementation->EulerToMatrix( yaw, pitch, roll, matrix );
	}
	inline void RadEulerToMatrix( const EulerAngle& angles, Matrix3x3& matrix )
	{
		m_MathUtilImplementation->RadEulerToMatrix( angles, matrix );
	}
	inline void RadEulerToMatrix( float yaw, float pitch, float roll, Matrix3x3 &matrix )
	{
		m_MathUtilImplementation->RadEulerToMatrix( yaw, pitch, roll, matrix );
	}
	
// tolua_end

	/// Setup the implemenation for this math lib
	static void InitImplementation();

private:
	/// Pointer to math util object
	static IMathUtilImplementation* m_MathUtilImplementation;

// tolua_begin
};
// tolua_end

#endif // #ifndef	_CLUAMATH_H_