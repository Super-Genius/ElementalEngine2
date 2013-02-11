///==========================================================================
/// \file	IVec3Implementation.h
/// \brief	3D vector math function implementation interface
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

#ifndef	IVEC3IMPLEMENTATION_H
#define	IVEC3IMPLEMENTATION_H

namespace ElementalEngine
{

class Vec3;
class Matrix3x3;

///==========================================================================
/// \class	IVec3Implementation
/// \brief	Interface for different 3D vector math implementations. Various
///			implementations must be functionally identical but can use
///			different techniques. For example one could implement this
///			interface using MMX and SSE instructions while another
///			implementation could use 3DNOW! or wrap another math library.
///==========================================================================
class __declspec(novtable) IVec3Implementation
{
public:
	/// Return true if two Vec3 are equal
	/// \param a - The first Vec3 to test
	/// \param b - The second Vec3 to test
	virtual bool Equals(const Vec3& a, const Vec3& b) const = 0;
	/// Return true if two Vec3 are not equal
	/// \param a - The first Vec3 to test
	/// \param b - The second Vec3 to test
	virtual bool NotEquals(const Vec3& a, const Vec3& b) const = 0;
	virtual bool LessThan(const Vec3& a, const Vec3& b) const = 0;
	virtual bool GreaterThan(const Vec3& a, const Vec3& b) const = 0;
	/// \brief	Add two vectors
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	virtual	Vec3 Add(const Vec3& a, const Vec3& b) const = 0;
	/// \brief	Add two vectors and store the results in this
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	/// \return	The result of adding a and b
	virtual	const Vec3& AddEquals(Vec3& a, const Vec3& b) const = 0;
	/// \brief	Subtract two vectors
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	virtual	Vec3 Subtract(const Vec3& a, const Vec3& b) const = 0;
	/// \brief	Subtract two vectors and store the results in this
	/// \param	a - The first vector to subtract
	/// \param	b - The second vector to subtract
	/// \return	The result of Subtract a and b
	virtual	const Vec3& SubtractEquals(Vec3& a, const Vec3& b) const = 0;
	/// \brief	Multiply two vectors
	/// \param	a - The first vector to multiply
	/// \param	b - The second vector to multiply
	virtual	Vec3 Times(const Vec3& a, const Vec3& b) const = 0;
	/// \brief	Multiply two vectors and store the results in this
	/// \param	a - The first vector to multiply
	/// \param	b - The second vector to multiply
	/// \return	The result of multiplying a and b
	virtual	const Vec3& TimesEquals(Vec3& a, const Vec3& b) const = 0;
	/// \brief	Multiply two vectors
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	virtual	Vec3 Times(const Vec3& a, float scalar) const = 0;
	/// \brief	Multiply two vectors and store the results in this
	/// \param	a - The first vector to multiply
	/// \param	b - The second vector to multiply
	/// \return	The result of multiplying a and b
	virtual	const Vec3& TimesEquals(Vec3& a, float scalar) const = 0;
	/// \brief	Multiply a vector by a Matrix3x3
	/// \param	a - The first vector to multiply
	/// \param	b - The second vector to multiply
	virtual	Vec3 Times(const Vec3& a, const Matrix3x3& m) const = 0;
	/// \brief	Multiply a vector by a Matrix3x3
	/// \param	a - The first vector to multiply
	/// \param	m - Matrix3x3 to translate the Vec3 by
	/// \return	The result of multiplying a and m
	virtual	const Vec3& TimesEquals(Vec3& a, const Matrix3x3& m) const = 0;
	/// \brief	Add two vectors
	/// \param	a - The first vector to multiply
	/// \param	m - Matrix3x3 to translate the Vec3 by
	virtual	Vec3 Divide(const Vec3& a, const Vec3& b) const = 0;
	/// \brief	Add two vectors and store the results in this
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	/// \return	The result of adding a and b
	virtual	const Vec3& DivideEquals(Vec3& a, const Vec3& b) const = 0;
	/// \brief	Add two vectors
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	virtual	Vec3 Divide(const Vec3& a,  float scalar) const = 0;
	/// \brief	Add two vectors and store the results in this
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	/// \return	The result of adding a and b
	virtual	const Vec3& DivideEquals(Vec3& a,  float scalar) const = 0;
	/// Return the length of the vector
	/// \param vec - The vector
	/// \return the length of the vector
	virtual float Length(const Vec3& vec) const = 0;
	/// Return the length squared of the vector
	/// \param vec - The vector
	/// \return the length squared  of the vector
	virtual float LengthSQ(const Vec3& vec) const = 0;
	/// Set the length of the vector
	/// \param vec - The vector
	/// \param newLength - The length to set
	virtual void SetLength(Vec3& vec, float newLength) = 0;
	/// Calculate the dot product of two vectors
	/// \param a - The first vector
	/// \param b - The second vector
	/// \return The dot product of vectors a and b
	virtual float Dot(const Vec3& a, const Vec3& b) const = 0;
	/// Calculate the cross product of two vectors
	/// \param a - The first vector
	/// \param b - The second vector
	/// \return The cross product of vectors a and b
	virtual Vec3 Cross(const Vec3& a, const Vec3& b) const = 0;
	/// Calculate the distance between two vectors
	/// \param a - The first vector
	/// \param b - The second vector
	/// \return The distance between vectors a and b
	virtual float DistanceBetween(const Vec3& a, const Vec3& b) = 0;
	/// Calculate the distance squared between two vectors
	/// \param a - The first vector
	/// \param b - The second vector
	/// \return The distance squared between vectors a and b
	virtual float DistanceBetweenSQ(const Vec3& a, const Vec3& b) = 0;
	/// Invert the Vector
	/// \param vec - Vector to invert
	virtual void Invert(Vec3& vec) = 0;
	/// Normalize a given vector.
	/// \param v - vector to normalize
	/// \return The length of the given vector before normalizing
	virtual void Normalize(Vec3 &v) const = 0;
	/// Rotates a vector about the global x axis
	/// \param v - vector to rotate
	/// \param degRotation - degrees to rotate	
	virtual void RotateAboutX(Vec3 &v, float degRotation) = 0;
	/// Rotates a vector about the global y axis
	/// \param v - vector to rotate
	/// \param degRotation - degrees to rotate	
	virtual void RotateAboutY(Vec3 &v, float degRotation) = 0;
	/// Rotates a vector about the global z axis
	/// \param v - vector to rotate
	/// \param degRotation - degrees to rotate	
	virtual void RotateAboutZ(Vec3 &v, float degRotation) = 0;
};

} //namespace ElementalEngine

#endif // IVEC3IMPLEMENTATION_H