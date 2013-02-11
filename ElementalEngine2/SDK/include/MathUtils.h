///==========================================================================
/// \file	MathUtils.h
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

#ifndef MATHUTILS_H__
#define MATHUTILS_H__

enum INTERSECTION_RESULT {
	INTERSECTION_OUTSIDE, 
	INTERSECTION_PARTIAL, 
	INTERSECTION_INSIDE, 
};

#include "IMathUtilImplementation.h"

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

#define PI          3.14159265358979323846f
#define DIVPI       0.31830988618379067153f
#define DIV2PI      0.15915494309189533577f
#define DIV360      0.00277777777777777778f
#define PIDIV2      1.57079632679489661923f
#define PI2         6.28318530717958647692f

#define EPSILON		0.0000001f

namespace ElementalEngine
{

// Math Utilities

/// Clamps any type to a [min,max]
/// \param i - number to clamp
/// \param min - the min
/// \param max - the max
/// \return int - Returns i clamped to [min,max]
template <typename T>
inline T Clamp( T i, T min, T max )
{
	return (i < min) ? min : ((i > max) ? max : i);
}

// REPLACE THESE!!!
#define EEDEG2RAD (PI/180)
#define EERAD2DEG (180.0/PI)

int SetMathUtilImplementation();

class MathUtil
{
private:
	friend int SetMathUtilImplementation();
	static IMathUtilImplementation* impl;

public:

	static void Init(IMathUtilImplementation* mathImpl)
	{
		impl = mathImpl;
	}

	MathUtil()
	{
		if (impl == NULL)
		{
			SetMathUtilImplementation();
		}
	}
	/// Find the log base 2 of a number
	inline float Log2( float x )
	{
		return impl->Log2(x);
	}

	// Find the log with an arbitrary base of x
	/// \param x - the number
	/// \param fLogBase - base of the log
	/// \return float - Returns the log base fLogBase of x
	inline float LogBase( float x, float fLogBase )
	{
		return impl->LogBase(x, fLogBase);
	}

	/// Test if an int is a power of two
	/// \param x - int to test
	/// \return bool - result of test, true if x is a power of two and false otherwise
	inline bool IsPowerOfTwo(int x)
	{
		return impl->IsPowerOfTwo(x);
	}

	/// Find the int nearest to a given int that is a power of two (greater than 0).
	/// For ints directly between two powers of two the bias is towards the greater power of two.
	/// \param x - the int
	/// \return int - Returns the int nearest to x that is a power of two
	inline int NearestPowerOfTwo(int x)
	{
		return impl->NearestPowerOfTwo(x);
	}

	/// Find the nearest int greater than a given int that is a power of two (greater than 0).
	/// \param x - the int
	/// \return int - power of two int
	inline int UpperPowerOfTwo(int x)
	{
		return impl->UpperPowerOfTwo(x);
	}

	/// \brief	Convert radians to degrees
	/// \param	radValue - The radian angle
	/// \return	The value in degrees
	inline float RadToDeg(float radValue)
	{
		return impl->RadToDeg(radValue);
	}

	/// \brief	Convert degrees to radians
	/// \param	degValue - The degree angle
	/// \return	The value in radians
	inline float DegToRad(float degValue)
	{
		return impl->DegToRad(degValue);
	}

	/// \brief	Calculate the arccosine
	/// \param	value - Value to find to arccosine of
	/// \return	The arccosine of value
	inline float GetACos(float value)
	{
		return impl->GetACos(value);
	}
	/// \brief	Calculate the arctangent
	/// \param	value - Value to find to arctangent of
	/// \return	The arctangent of value
	inline float GetATan(float rise, float run)
	{
		return impl->GetATan( rise, run );
	}
	
	/// \brief	Calculate the tangent
	/// \param	value - Value to find to tangent of
	/// \return	The tangent of value
	inline float GetTan(float value) 
	{
		return impl->GetTan( value );
	}

	/// \brief	Calculate the arcsine
	/// \param	value - Value to find to arcsine of
	/// \return	The arcsine of value
	inline float GetASin(float value)
	{
		return impl->GetASin(value);
	}
	/// \brief	Calculate the sine of a radian value
	/// \param	radAngle - Radian value to find to sine of
	/// \return	The sine of value
	inline float GetSin(float radAngle)
	{
		return impl->GetSin(radAngle);
	}
	/// \brief	Calculate the cosine of a radian value
	/// \param	radAngle - Radian value to find to cosine of
	/// \return	The cosine of value
	inline float GetCos(float radAngle)
	{
		return impl->GetCos(radAngle);
	}

	/// \brief	Generates a rotation matrix
	/// \param	vPos = The origin
	/// \param	vLookAt = The look at direction
	inline void MatrixLookAt( const Vec3& pos, const Vec3& lookAt, Matrix3x3& matrix )
	{
		return impl->MatrixLookAt( pos, lookAt, matrix );
	}

	/// \brief	Generates a rotation matrix
	/// \param	vPos = The origin
	/// \param	vLookAt = The look at direction
	inline void MatrixLookAt( const Vec3& pos, const Vec3& lookAt, const Vec3 &up, Matrix3x3& matrix )
	{
		return impl->MatrixLookAt( pos, lookAt, up, matrix );
	}

	/// \brief	Convert a rotation matrix to radian euler angles
	/// \param	matrix - The rotation matrix
	/// \param	angles - The array of floats to store the euler angles
	inline void MatrixToRadEuler( const Matrix3x3 &matrix, EulerAngle& angles )
	{
		impl->MatrixToRadEuler( matrix, angles );
	}

	/// \brief	Convert a rotation matrix to degree euler angles
	/// \param	matrix - The rotation matrix
	/// \param	angles - The array of floats to store the euler angles
	inline void MatrixToEuler( const Matrix3x3 &matrix, EulerAngle& angles )
	{
		impl->MatrixToEuler( matrix, angles );
	}

	/// \brief	Convert a rotation matrix to degree euler angles (4x4)
	/// \param	matrix - The rotation matrix
	/// \param	angles - The array of floats to store the euler angles
	inline void MatrixToEuler( const Matrix4x4 &matrix, EulerAngle& angles )
	{
		impl->MatrixToEuler( matrix, angles );
	}

	/// \brief	Convert euler angles in degrees to a rotation matrix
	/// \param	angles - The array of floats in degrees
	/// \param	matrix - The matrix to store the euler angles
	inline void EulerToMatrix( const EulerAngle& angles, Matrix3x3& matrix )
	{
		impl->EulerToMatrix( angles, matrix );
	}

	/// \brief	Convert euler angles in degrees to a rotation matrix
	/// \param	yaw - Yaw in degrees
	/// \param	pitch - Pitch in degrees
	/// \param	roll - Roll in degrees
	/// \param	matrix - The matrix to store the euler angles
	inline void EulerToMatrix( float yaw, float pitch, float roll, Matrix3x3& matrix )
	{
		impl->EulerToMatrix( yaw, pitch, roll, matrix );
	}

	/// \brief	Convert euler angles in radians to a rotation matrix
	/// \param	angles - The array of floats in radians
	/// \param	matrix - The matrix to store the euler angles
	inline void RadEulerToMatrix( const EulerAngle& angles, Matrix3x3& matrix )
	{
		impl->RadEulerToMatrix( angles, matrix );
	}

	
	/// \brief	Convert euler angles in radians to a rotation matrix
	/// \param	angles - The array of floats in radians
	/// \param	matrix - The matrix to store the euler angles
	inline void RadEulerToMatrix( const EulerAngle& angles, Matrix4x4& matrix )
	{
		impl->RadEulerToMatrix( angles, matrix );
	}

	/// \brief	Convert euler angles in radians to a rotation matrix
	/// \param	angles - The array of floats in radians
	/// \param	matrix - The matrix to store the euler angles
	inline void EulerToMatrix( const EulerAngle& angles, Matrix4x4& matrix )
	{
		impl->EulerToMatrix( angles, matrix );
	}

	/// \brief	Convert euler angles in radians to a rotation matrix
	/// \param	yaw - Yaw in radians
	/// \param	pitch - Pitch in radians
	/// \param	roll - Roll in radians
	/// \param	matrix - The matrix to store the euler angles
	inline void RadEulerToMatrix( float yaw, float pitch, float roll, Matrix3x3& matrix )
	{
		impl->RadEulerToMatrix( yaw, pitch, roll, matrix );
	}
	/// \brief	Convert a quaternion to euler angles
	/// \param	quat - The Quaternion to convert
	/// \param	euler - The Vec3 to store the resultant euler angles
	inline void EulerToQuaternion( EulerAngle& euler, Quaternion& quat )
	{
		impl->EulerToQuaternion( euler, quat );
	}
	/// \brief	Convert a quaternion to euler angles
	/// \param	quat - The Quaternion to convert
	/// \param	euler - The Vec3 to store the resultant euler angles
	inline void RadEulerToQuaternion( EulerAngle& euler, Quaternion& quat )
	{
		impl->RadEulerToQuaternion( euler, quat );
	}

	/// Returns floor of float (float without fractional part)
	/// \param value - value to get floor of
	/// \return floor of said value
	inline float Floor( float value )
	{
		return impl->Floor( value );
	}

	/// Returns ceiling of float (float without fractional part, + 1)
	/// \param value - value to get ceiling of
	/// \return ceiling of said value
	inline float Ceil( float value )
	{
		return impl->Ceil( value );
	}

	/// Returns absolute value of a float
	/// \param value - value to get abs of
	/// \return absolute value
	inline float Fabs( float value )
	{
		return impl->Fabs( value );
	}

	/// Returns rounded value
	/// \param value - value to round
	/// \return rounded value
	inline float Round( float value )
	{
		return impl->Round( value );
	}

	/// Returns value rounded to a multiple of 'multiple'
	/// \param value - value to round
	/// \param multiple - multiple to round value to
	/// \return value rounded to multiple of 'multiple'
	inline float RoundToNearestMultiple( float value, float multiple )
	{
		return impl->RoundToNearestMultiple( value, multiple );
	}

	/// Returns intersection result of sphere/sphere
	/// \param sphereCenterA - center of sphere A
	/// \param sphereRadiusA - radius of sphere A
	/// \param sphereCenterB - center of sphere B
	/// \param sphereRadiusB - radius of sphere B
	/// \return intersection result
	INTERSECTION_RESULT IntersectSphereSphere( const Vec3 &sphereCenterA, float sphereRadiusA, const Vec3 &sphereCenterB, float sphereRadiusB )
	{
		return impl->IntersectSphereSphere( sphereCenterA, sphereRadiusA, sphereCenterB, sphereRadiusB );
	}

	/// Returns if tow values are within EPSILON of each other.
	/// of the epsilon value
	/// \param	first - first value to test
	/// \param	second - secont value to test
	/// \return true if equal, false if not
	inline bool EpsilonEqual( float first, float second )
	{
		if( first == second || Fabs(first - second) <= EPSILON )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	inline void VectorToEuler( const Vec3 &direction, EulerAngle &angles )
	{
		impl->VectorToEuler( direction, angles );
	}
	inline void EulerToVector( const EulerAngle &angles, Vec3 &direction )
	{
		impl->EulerToVector( angles, direction );
	}
};

} //namespace ElementalEngine

#ifndef EE_SDK_EXPORTS
#pragma comment(lib, "EE-SDK.lib")
#endif

#endif //MATHUTILS_H__
