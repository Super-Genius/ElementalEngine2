///==========================================================================
/// \file	IMathUtilImplementation.h
/// \brief	Math utility functions implementation interface
/// \date	3\16\2005
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

#ifndef	IMATHUTILIMPL_H__
#define	IMATHUTILIMPL_H__

namespace ElementalEngine
{

class MathUtil;

///==========================================================================
/// \class	IMathUtilImplementation
/// \brief	
///==========================================================================
class __declspec(novtable) IMathUtilImplementation
{
public:
	/// Find the log base 2 of a number
	virtual float Log2(float x) = 0;
	// Find the log with an arbitrary base of x
	/// \param x - the number
	/// \param fLogBase - base of the log
	/// \return float - Returns the log base fLogBase of x
	virtual float LogBase(float x, float fLogBase) = 0;
	/// Test if an int is a power of two
	/// \param x - int to test
	/// \return bool - result of test, true if x is a power of two and false otherwise
	virtual bool IsPowerOfTwo(int x) = 0;
	/// Find the int nearest to a given int that is a power of two (greater than 0).
	/// For ints directly between two powers of two the bias is towards the greater power of two.
	/// \param x - the int
	/// \return int - Returns the int nearest to x that is a power of two
	virtual int NearestPowerOfTwo(int x) = 0;
	/// Find the nearest int greater than a given int that is a power of two (greater than 0).
	/// \param x - the int
	/// \return int - power of two int
	virtual int UpperPowerOfTwo(int x) = 0;
	/// \brief	Convert radians to degrees
	/// \param	radValue - The radian angle
	/// \return	The value in degrees
	virtual float RadToDeg(float radValue) = 0;
	/// \brief	Convert degrees to radians
	/// \param	degValue - The degree angle
	/// \return	The value in radians
	virtual float DegToRad(float degValue) = 0;
	/// \brief	Calculate the arctangent
	/// \param	value - Value to find to arctangent of
	/// \return	The arctangent of value
	virtual float GetATan(float rise, float run) = 0;
	/// \brief	Calculate the arccosine
	/// \param	value - Value to find to arccosine of
	/// \return	The arccosine of value
	virtual float GetACos(float value) = 0;
	/// \brief	Calculate the arcsine
	/// \param	value - Value to find to arcsine of
	/// \return	The arcsine of value
	virtual float GetASin(float value) = 0;
	/// \brief	Calculate the tangent
	/// \param	value - Value to find to tangent of
	/// \return	The tangent of value
	virtual float GetTan(float value) = 0;
	/// \brief	Calculate the sine of a radian value
	/// \param	radAngle - Radian value to find to sine of
	/// \return	The sine of value
	virtual float GetSin(float radAngle) = 0;
	/// \brief	Calculate the cosine of a radian value
	/// \param	radAngle - Radian value to find to cosine of
	/// \return	The cosine of value
	virtual float GetCos(float radAngle) = 0;

	/// \brief	Generates a rotation matrix
	/// \param	vPos = The origin
	/// \param	vLookAt = The look at direction
	/// \param	vUp = The up vector
	virtual void MatrixLookAt( const Vec3 &vPos, const Vec3 &vLookAt, const Vec3 &vUp, Matrix3x3 &dest ) = 0;
	/// \brief	Generates a rotation matrix
	/// \param	vPos = The origin
	/// \param	vLookAt = The look at direction
	virtual void MatrixLookAt( const Vec3& vPos, const Vec3& vLookAt, Matrix3x3& dest ) = 0;
	/// \brief	Convert a rotation matrix to radian euler angles
	/// \param	matrix - The rotation matrix
	/// \param	angles - The array of floats to store the euler angles
	virtual void MatrixToRadEuler( const Matrix3x3 &matrix, EulerAngle& angles ) = 0;	
	/// \brief	Convert a rotation matrix to degree euler angles
	/// \param	matrix - The rotation matrix
	/// \param	angles - The array of floats to store the euler angles
	virtual void MatrixToEuler( const Matrix3x3 &matrix, EulerAngle& angles ) = 0;
	/// \brief	Convert a rotation matrix to degree euler angles (from a 4x4)
	/// \param	matrix - The rotation matrix
	/// \param	angles - The array of floats to store the euler angles
	virtual void MatrixToEuler( const Matrix4x4 &matrix, EulerAngle& angles ) = 0;
	/// \brief	Generate euler angles looking at a point from a position
	/// \param	pos - point of origin
	/// \param	lookAt - point we are looking at
	/// \param	angles - output for euler angles
	virtual void EulerLookAt( const Vec3 &pos, const Vec3 &lookAt, EulerAngle &angles ) = 0;
	/// Generate euler angles given a vector direction
	/// \param	direction - direction vector to look at
	/// \param	angles - output for euler angles
	virtual void VectorToEuler( const Vec3 &direction, EulerAngle &angles ) = 0;
	/// Generate euler angles given a vector direction
	/// \param	angles - euler angles
	/// \param	direction - direction vector to look at
	virtual void EulerToVector( const EulerAngle &angles, Vec3 &direction ) = 0;
	/// \brief	Convert euler angles in degrees to a rotation matrix
	/// \param	angles - The array of floats in degrees
	/// \param	matrix - The matrix to store the euler angles
	virtual void EulerToMatrix( const EulerAngle& angles, Matrix3x3& matrix ) = 0;
	virtual void EulerToMatrix( const EulerAngle& angles, Matrix4x4& matrix ) = 0;
	/// \brief	Convert euler angles in degrees to a rotation matrix
	/// \param	yaw - Yaw in degrees
	/// \param	pitch - Pitch in degrees
	/// \param	roll - Roll in degrees
	/// \param	matrix - The matrix to store the euler angles
	virtual void EulerToMatrix( float yaw, float pitch, float roll, Matrix3x3& matrix ) = 0;
	/// \brief	Convert euler angles in radians to a rotation matrix
	/// \param	angles - The array of floats in radians
	/// \param	matrix - The matrix to store the euler angles
	virtual void RadEulerToMatrix( const EulerAngle& angles, Matrix3x3& matrix ) = 0;
	/// \brief	Convert euler angles in radians to a rotation matrix
	/// \param	angles - The array of floats in radians
	/// \param	matrix - The matrix to store the euler angles
	virtual void RadEulerToMatrix( const EulerAngle& angles, Matrix4x4& matrix ) = 0;
	/// \brief	Convert euler angles in radians to a rotation matrix
	/// \param	yaw - Yaw in radians
	/// \param	pitch - Pitch in radians
	/// \param	roll - Roll in radians
	/// \param	matrix - The matrix to store the euler angles
	virtual void RadEulerToMatrix( float yaw, float pitch, float roll, Matrix3x3& matrix ) = 0;
	/// \brief	Convert a quaternion to euler angles
	/// \param	euler - The euler angles to convert
	/// \param	quat - The Quaternion to store the result	
	virtual void EulerToQuaternion( EulerAngle& euler, Quaternion& quat ) = 0;
	/// \brief	Convert a radian euler angles to quaternion
	/// \param	euler - The euler angles to convert
	/// \param	quat - The Quaternion to store the result
	virtual void RadEulerToQuaternion( EulerAngle& euler, Quaternion& quat ) = 0;

	/// Returns floor of float (float without fractional part)
	/// \param value - value to get floor of
	/// \return floor of said value
	virtual float Floor( float value ) = 0;
	/// Returns ceiling of float (float without fractional part, + 1)
	/// \param value - value to get ceiling of
	/// \return ceiling of said value
	virtual float Ceil( float value ) = 0;
	/// Returns absolute value of a float
	/// \param value - value to get abs of
	/// \return absolute value
	virtual float Fabs( float value ) = 0;	
	/// Returns rounded value
	/// \param value - value to round
	/// \return rounded value
	virtual float Round( float value ) = 0;
	/// Returns value rounded to a multiple of 'multiple'
	/// \param value - value to round
	/// \param multiple - multiple to round value to
	/// \return value rounded to multiple of 'multiple'
	virtual float RoundToNearestMultiple( float value, float multiple ) = 0;
	/// Returns intersection result of sphere/sphere
	/// \param sphereCenterA - center of sphere A
	/// \param sphereRadiusA - radius of sphere A
	/// \param sphereCenterB - center of sphere B
	/// \param sphereRadiusB - radius of sphere B
	/// \return intersection result
	virtual INTERSECTION_RESULT IntersectSphereSphere( const Vec3 &sphereCenterA, float sphereRadiusA, const Vec3 &sphereCenterB, float sphereRadiusB ) = 0;
};

} //namespace ElementalEngine

#endif // IMATHUTILIMPL_H__
