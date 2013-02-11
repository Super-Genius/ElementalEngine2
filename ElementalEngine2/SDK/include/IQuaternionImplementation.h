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

#ifndef	IQUATERNIONIMPL_H__
#define	IQUATERNIONIMPL_H__

namespace ElementalEngine
{

class Quaternion;
class Matrix4x4;
class Vec3;
class EulerAngle;

///==========================================================================
/// \class	IQuaternionImplementation
/// \brief	
///==========================================================================
class __declspec(novtable) IQuaternionImplementation
{
public:
	/// Return true if two Quaternions are equal
	/// \param a - The first Quaternion to test
	/// \param b - The second Quaternion to test
	virtual bool Equals(const Quaternion& a, const Quaternion& b) const = 0;

	/// Add two Quaternions
	/// \param	a - The first Quaternion to add
	/// \param	b - The second Quaternion to add
	virtual	Quaternion Add(const Quaternion& a, const Quaternion& b) const = 0;

	/// Multiply two Quaternions
	/// \param	a - The first Quaternion to multiply
	/// \param	b - The second Quaternion to multiply
	virtual Quaternion Times(const Quaternion& a, const Quaternion& b) const = 0;
	/// Multiply two Quaternions and store the results in this 
	/// \param	a - The first Quaternion to multiply
	/// \param	b - The second Quaternion to multiply
	virtual void TimesEquals(Quaternion& a, const Quaternion& b) = 0;
	/// Multiply a Quaternion and a scalar
	/// \param	a - The first Quaternion to multiply
	/// \param	b - Scalar value
	virtual Quaternion Times(const Quaternion& a, float f) const = 0;
	/// Multiply a Quaternion and a scalar and store the results in this 
	/// \param	a - The first Quaternion to multiply
	/// \param	b - Scalar value
	virtual void TimesEquals(Quaternion& a, float f) = 0;

	/// Divide two Quaternions
	/// \param	a - The first Quaternion to divide
	/// \param	b - The second Quaternion to divide
	virtual Quaternion Divide(const Quaternion& a, const Quaternion& b) const = 0;
	/// Divide two Quaternions and store the results in this 
	/// \param	a - The first Quaternion to divide
	/// \param	b - The second Quaternion to divide
	virtual void DivideEquals(Quaternion& a, const Quaternion& b) = 0;

	/// Set the Quaternion from X,Y,Z,W tuple
	/// \param quat - The Quaternion to set
	/// \param newX - The X value
	/// \param newY - The Y value
	/// \param newZ - The Z value
	/// \param newW - The W value
	virtual void Set(Quaternion& quat, float newX, float newY, float newZ, float newW) = 0;
	/// Calculate the dot product of twi Quaternions
	/// \param a - The first Quaternion
	/// \param b - The second Quaternion
	/// \reutrn The dot product of Quaternions a and b
	virtual float DotProduct(const Quaternion& a, const Quaternion& b) const = 0;
	/// Normalize a given Quaternion.
	/// \param quat - Quaternion to normalize
	virtual void Normalize(Quaternion& quat) = 0;
	/// Convert a Quaternion to a Matrix4x4
	/// \param	quat - Quaternion to convert
	/// \return The converted Matrix4x4
	virtual void GetMatrix(const Quaternion& quat, Matrix4x4& out) const = 0;
	/// Set a quaternion from a Matrix3x3
	/// \param	quat - converted quaternion
	/// \return Matrix4x4 to convert
	virtual void SetFromMatrix( Quaternion& quat, const Matrix3x3& in ) = 0;
	/// Set a quaternion from a Matrix4x4
	/// \param	quat - converted quaternion
	/// \return Matrix4x4 to convert
	virtual void SetFromMatrix( Quaternion& quat, const Matrix4x4& in ) = 0;

	/// Invert the Quaternion
	/// \param quat - Quaternion to invert
	virtual void MakeInverse(Quaternion& quat) = 0;
	/// Calculate the Spherical Linear Interpolation between two Quaternions
	/// \param a - The first Quaternion to interpolate between
	/// \parma b - The second Quaternion to interpolate between
	/// \param time - Value between 0 and 1 that determines how far along the interpolation is
	/// \return The SLERP'd Quaternion
	virtual Quaternion Slerp(Quaternion a, Quaternion b, float time) = 0;
	/// Convert a Quaternion to euler angles
	/// \param quat - The Quaternion to convert
	/// \param euler - A Vec3 receiving the converted euler angles
	virtual void ToEuler(const Quaternion& quat, EulerAngle& euler) = 0;
	/// Construct a Quaternion from euler angles
	/// \param quat - The Quaternion to store the converted euler angles
	/// \param euler - A Vec3 with the euler angles
	virtual void FromEuler(Quaternion& quat, EulerAngle& euler) = 0;
};

} //namespace ElementalEngine

#endif // IQUATERNIONIMPL_H__