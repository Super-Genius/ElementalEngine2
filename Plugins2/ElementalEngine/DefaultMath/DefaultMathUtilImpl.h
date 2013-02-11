///==========================================================================
/// \file	DefaultMathUtilImpl.h
/// \brief	
/// \date	08/03/2006
/// \author	Kyle Swaim
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

#ifndef DEFAULTMATHUTILIMPL_H__
#define DEFAULTMATHUTILIMPL_H__

#include "IMathUtilImplementation.h"
#include "MathUtils.h"

namespace ElementalEngine
{

class CDefaultMathUtilImplementation : public IMathUtilImplementation
{
private:
	void Init( void );
	void CleanUp( void );

	CDefaultMathUtilImplementation()
	{
		Init();
	}

	virtual ~CDefaultMathUtilImplementation()
	{
		CleanUp();
	}

public:
	static IMathUtilImplementation* Instance( void )
	{
		if ( s_instance == NULL )
		{
			s_instance = new CDefaultMathUtilImplementation();
		}
		return s_instance;
	}
	static void Destroy( void )
	{
		if ( s_instance != NULL )
		{
			delete s_instance;
		}
		s_instance = NULL;
	}


	/// Find the log base 2 of a number
	virtual float Log2(float x);
	// Find the log with an arbitrary base of x
	/// \param x - the number
	/// \param fLogBase - base of the log
	/// \return float - Returns the log base fLogBase of x
	virtual float LogBase(float x, float fLogBase);
	/// Test if an integer is a power of two
	/// \param x - integer to test
	/// \return bool - result of test, true if x is a power of two and false otherwise
	virtual bool IsPowerOfTwo(int x);
	/// Find the integer nearest to a given integer that is a power of two (greater than 0).
	/// For integers directly between two powers of two the bias is towards the greater power of two.
	/// \param x - the integer
	/// \return int - Returns the integer nearest to x that is a power of two
	virtual int NearestPowerOfTwo(int x);
	/// Find the nearest integer greater than a given integer that is a power of two (greater than 0).
	/// \param x - the integer
	/// \return int - power of two integer
	virtual int UpperPowerOfTwo(int x);
	/// \brief	Convert radians to degrees
	/// \param	radValue - The radian angle
	/// \return	The value in degrees
	virtual float RadToDeg(float radValue);
	/// \brief	Convert degrees to radians
	/// \param	degValue - The degree angle
	/// \return	The value in radians
	virtual float DegToRad(float degValue);
	/// \brief	Calculate the arctangent
	/// \param	value - Value to find to arctangent of
	/// \return	The arctangent of value
	virtual float GetATan(float rise, float run);	
	/// \brief	Calculate the tangent
	/// \param	value - Value to find to tangent of
	/// \return	The tangent of value
	virtual float GetTan(float value);
	/// \brief	Calculate the arccosine
	/// \param	value - Value to find to arccosine of
	/// \return	The arccosine of value
	virtual float GetACos(float value);
	/// \brief	Calculate the arcsine
	/// \param	value - Value to find to arcsine of
	/// \return	The arcsine of value
	virtual float GetASin(float value);
	/// \brief	Calculate the sine of a radian value
	/// \param	radAngle - Radian value to find to sine of
	/// \return	The sine of value
	virtual float GetSin(float radAngle);
	/// \brief	Calculate the cosine of a radian value
	/// \param	radAngle - Radian value to find to cosine of
	/// \return	The cosine of value
	virtual float GetCos(float radAngle);
	/// \brief	Generates a rotation matrix
	/// \param	vPos = The origin
	/// \param	vLookAt = The look at direction
	/// \param	vUp = The up vector
	virtual void MatrixLookAt( const Vec3 &vPos, const Vec3 &vLookAt, const Vec3 &vUp, Matrix3x3 &dest );
	/// \brief	Generates a rotation matrix
	/// \param	vPos = The origin
	/// \param	vLookAt = The look at direction
	virtual void MatrixLookAt( const Vec3& vPos, const Vec3& vLookAt, Matrix3x3& dest );
	/// \brief	Convert a rotation matrix to radian euler angles
	/// \param	matrix - The rotation matrix
	/// \param	angles - The array of floats to store the euler angles
	virtual void MatrixToRadEuler( const Matrix3x3 &matrix, EulerAngle& angles );	
	/// \brief	Convert a rotation matrix to degree euler angles
	/// \param	matrix - The rotation matrix
	/// \param	angles - The array of floats to store the euler angles
	virtual void MatrixToEuler( const Matrix3x3 &matrix, EulerAngle& angles );
	/// \brief	Convert a rotation matrix to degree euler angles (from a 4x4)
	/// \param	matrix - The rotation matrix
	/// \param	angles - The array of floats to store the euler angles
	virtual void MatrixToEuler( const Matrix4x4 &matrix, EulerAngle& angles );
	/// \brief	Generate euler angles looking at a point from a position
	/// \param	pos - point of origin
	/// \param	lookAt - point we are looking at
	/// \param	angles - output for euler angles
	virtual void EulerLookAt( const Vec3 &pos, const Vec3 &lookAt, EulerAngle &angles );
	/// Generate euler angles given a vector direction
	/// \param	direction - direction vector to look at
	/// \param	angles - output for euler angles
	virtual void VectorToEuler( const Vec3 &direction, EulerAngle &angles );
	/// Generate euler angles given a vector direction
	/// \param	angles - euler angles
	/// \param	direction - direction vector to look at
	virtual void EulerToVector( const EulerAngle &angles, Vec3 &direction );
	/// \brief	Convert euler angles in degrees to a rotation matrix
	/// \param	angles - The array of floats in degrees
	/// \param	matrix - The matrix to store the euler angles
	virtual void EulerToMatrix( const EulerAngle& angles, Matrix3x3& matrix );
	virtual void EulerToMatrix( const EulerAngle& angles, Matrix4x4& matrix );
	/// \brief	Convert euler angles in degrees to a rotation matrix
	/// \param	yaw - Yaw in degrees
	/// \param	pitch - Pitch in degrees
	/// \param	roll - Roll in degrees
	/// \param	matrix - The matrix to store the euler angles
	virtual void EulerToMatrix( float yaw, float pitch, float roll, Matrix3x3& matrix );
	/// \brief	Convert euler angles in radians to a rotation matrix
	/// \param	angles - The array of floats in radians
	/// \param	matrix - The matrix to store the euler angles
	virtual void RadEulerToMatrix( const EulerAngle& angles, Matrix3x3& matrix );
		/// \brief	Convert euler angles in radians to a rotation matrix
	/// \param	angles - The array of floats in radians
	/// \param	matrix - The matrix to store the euler angles
	virtual void RadEulerToMatrix( const EulerAngle& angles, Matrix4x4& matrix );
	/// \brief	Convert euler angles in radians to a rotation matrix
	/// \param	yaw - Yaw in radians
	/// \param	pitch - Pitch in radians
	/// \param	roll - Roll in radians
	/// \param	matrix - The matrix to store the euler angles
	virtual void RadEulerToMatrix( float yaw, float pitch, float roll, Matrix3x3& matrix );
	/// \brief	Convert a quaternion to euler angles
	/// \param	euler - The euler angles to convert
	/// \param	quat - The Quaternion to store the result	
	virtual void EulerToQuaternion( EulerAngle& euler, Quaternion& quat );
	/// \brief	Convert a radian euler angles to quaternion
	/// \param	euler - The euler angles to convert
	/// \param	quat - The Quaternion to store the result
	virtual void RadEulerToQuaternion( EulerAngle& euler, Quaternion& quat );

	/// Returns floor of float (float without fractional part)
	/// \param value - value to get floor of
	/// \return floor of said value
	virtual float Floor( float value );
	/// Returns ceiling of float (float without fractional part, + 1)
	/// \param value - value to get ceiling of
	/// \return ceiling of said value
	virtual float Ceil( float value );
	/// Returns absolute value of a float
	/// \param value - value to get abs of
	/// \return absolute value
	virtual float Fabs( float value );	
	/// Returns rounded value
	/// \param value - value to round
	/// \return rounded value
	virtual float Round( float value );
	/// Returns value rounded to a multiple of 'multiple'
	/// \param value - value to round
	/// \param multiple - multiple to round value to
	/// \return value rounded to multiple of 'multiple'
	virtual float RoundToNearestMultiple( float value, float multiple );
	/// Returns intersection result of sphere/sphere
	/// \param sphereCenterA - center of sphere A
	/// \param sphereRadiusA - radius of sphere A
	/// \param sphereCenterB - center of sphere B
	/// \param sphereRadiusB - radius of sphere B
	/// \return intersection result
	virtual INTERSECTION_RESULT IntersectSphereSphere( const Vec3 &sphereCenterA, float sphereRadiusA, const Vec3 &sphereCenterB, float sphereRadiusB );

private:
	static IMathUtilImplementation* s_instance;
};

} // namespace ElementalEngine

#endif // DEFAULTMATHUTILIMPL_H__
