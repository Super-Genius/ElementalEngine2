///==========================================================================
/// \file	IQuaternionImplementation.h
/// \brief	Quaternion math function implementation interface
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

#ifndef	IEULERIMPL_H__
#define	IEULERIMPL_H__

namespace ElementalEngine
{

class EulerAngle;
class Matrix4x4;
class Quaternion;

///==========================================================================
/// \class	IQuaternionImplementation
/// \brief	
///==========================================================================
class __declspec(novtable) IEulerImplementation
{
public:
	/// Return true if two eulerernions are equal
	/// \param a - The first EulerAngle to test
	/// \param b - The second EulerAngle to test
	virtual bool Equals(const EulerAngle& a, const EulerAngle& b) const = 0;

	/// Add two EulerAngles
	/// \param	a - The first EulerAngle to add
	/// \param	b - The second EulerAngle to add
	virtual	EulerAngle Add(const EulerAngle& a, const EulerAngle& b) const = 0;

	/// Add two EulerAngles
	/// \param	a - The first EulerAngle to add
	/// \param	b - The second EulerAngle to add
	virtual	EulerAngle Subtract(const EulerAngle& a, const EulerAngle& b) const = 0;

	/// Multiply a EulerAngle and a scalar
	/// \param	a - The first EulerAngle to multiply
	/// \param	b - Scalar value
	virtual EulerAngle Times(const EulerAngle& a, float f) const = 0;
	/// Multiply a EulerAngle and a scalar and store the results in this 
	/// \param	a - The first EulerAngle to multiply
	/// \param	b - Scalar value
	virtual void TimesEquals(EulerAngle& a, float f) = 0;

	/// Set the EulerAngle from a roll pitch yaw
	/// \param euler - The EulerAngle to set
	/// \param roll - roll angle
	/// \param pitch - pitch angle
	/// \param yaw - yaw angle
	virtual void Set(EulerAngle& euler, float roll, float pitch, float yaw) = 0;

	/// Convert a EulerAngle to a Matrix4x4
	/// \param	euler - EulerAngle to convert
	/// \return The converted Matrix4x4
	virtual void GetMatrix(const EulerAngle& euler, Matrix4x4& out) const = 0;
	
	/// Convert a EulerAngle to a Matrix3x3
	/// \param	euler - EulerAngle to convert
	/// \return The converted Matrix3x3
	virtual void GetMatrix(const EulerAngle& euler, Matrix3x3& out) const = 0;

	/// Set a EulerAngle from a Matrix3x3
	/// \param	euler - converted EulerAngle
	/// \return Matrix4x4 to convert
	virtual void SetFromMatrix( EulerAngle& euler, const Matrix3x3& in ) = 0;
	/// Set a EulerAngle from a Matrix4x4
	/// \param	euler - converted EulerAngle
	/// \return Matrix4x4 to convert
	virtual void SetFromMatrix( EulerAngle& euler, const Matrix4x4& in ) = 0;

	/// Invert the EulerAngle
	/// \param euler - EulerAngle to invert
	virtual void MakeInverse(EulerAngle& euler) = 0;
	/// Convert a EulerAngle to euler angles
	/// \param euler - The EulerAngle to convert
	/// \param euler - A Vec3 receiving the converted euler angles
	virtual void ToQuaternion(const EulerAngle& euler, Quaternion& quat) = 0;
	/// Construct a EulerAngle from euler angles
	/// \param euler - The EulerAngle to store the converted euler angles
	/// \param euler - A Vec3 with the euler angles
	virtual void FromQuaternion(EulerAngle& euler, Quaternion& quat) = 0;
};

} //namespace ElementalEngine

#endif // IEULERIMPL_H__