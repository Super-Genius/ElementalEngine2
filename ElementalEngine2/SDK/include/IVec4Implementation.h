///==========================================================================
/// \file	IVec4Implementation.h
/// \brief	4D vector math function implementation interface
/// \author	Brian Bazyk
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

#ifndef	IVEC4IMPLEMENTATION_H
#define	IVEC4IMPLEMENTATION_H

namespace ElementalEngine
{

class Vec4;

///==========================================================================
/// \class	IVec4Implementation
/// \brief	Interface for different 3D vector math implementations. Various
///			implementations must be functionally identical but can use
///			different techniques. For example one could implement this
///			interface using MMX and SSE instructions while another
///			implementation could use 3DNOW! or wrap another math library.
///==========================================================================
class __declspec(novtable) IVec4Implementation
{
public:
	/// Return true if two Vec4 are equal
	/// \param a - The first Vec4 to test
	/// \param b - The second Vec4 to test
	virtual bool Equals(const Vec4& a, const Vec4& b) const = 0;
	/// Return true if two Vec4 are not equal
	/// \param a - The first Vec4 to test
	/// \param b - The second Vec4 to test
	virtual bool NotEquals(const Vec4& a, const Vec4& b) const = 0;
	virtual bool LessThan(const Vec4& a, const Vec4& b) const = 0;
	virtual bool GreaterThan(const Vec4& a, const Vec4& b) const = 0;
	/// \brief	Add two vectors
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	virtual	Vec4 Add(const Vec4& a, const Vec4& b) const = 0;
	/// \brief	Add two vectors and store the results in this
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	/// \return	The result of adding a and b
	virtual	const Vec4& AddEquals(Vec4& a, const Vec4& b) const = 0;
	/// \brief	Subtract two vectors
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	virtual	Vec4 Subtract(const Vec4& a, const Vec4& b) const = 0;
	/// \brief	Subtract two vectors and store the results in this
	/// \param	a - The first vector to subtract
	/// \param	b - The second vector to subtract
	/// \return	The result of Subtract a and b
	virtual	const Vec4& SubtractEquals(Vec4& a, const Vec4& b) const = 0;
	/// \brief	Multiply two vectors
	/// \param	a - The first vector to multiply
	/// \param	b - The second vector to multiply
	virtual	Vec4 Times(const Vec4& a, const Vec4& b) const = 0;
	/// \brief	Multiply two vectors and store the results in this
	/// \param	a - The first vector to multiply
	/// \param	b - The second vector to multiply
	/// \return	The result of multiplying a and b
	virtual	const Vec4& TimesEquals(Vec4& a, const Vec4& b) const = 0;
	/// \brief	Multiply two vectors
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	virtual	Vec4 Times(const Vec4& a, float scalar) const = 0;
	/// \brief	Multiply two vectors and store the results in this
	/// \param	a - The first vector to multiply
	/// \param	b - The second vector to multiply
	/// \return	The result of multiplying a and b
	virtual	const Vec4& TimesEquals(Vec4& a, float scalar) const = 0;
	/// \brief	Add two vectors
	/// \param	a - The first vector to multiply
	/// \param	m - Matrix3x3 to translate the Vec4 by
	virtual	Vec4 Divide(const Vec4& a, const Vec4& b) const = 0;
	/// \brief	Add two vectors and store the results in this
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	/// \return	The result of adding a and b
	virtual	const Vec4& DivideEquals(Vec4& a, const Vec4& b) const = 0;
	/// \brief	Add two vectors
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	virtual	Vec4 Divide(const Vec4& a,  float scalar) const = 0;
	/// \brief	Add two vectors and store the results in this
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	/// \return	The result of adding a and b
	virtual	const Vec4& DivideEquals(Vec4& a,  float scalar) const = 0;
	/// Return the length of the vector
	/// \param vec - The vector
	/// \return the length of the vector
	virtual float Length(const Vec4& vec) const = 0;
	/// Return the length squared of the vector
	/// \param vec - The vector
	/// \return the length squared  of the vector
	virtual float LengthSQ(const Vec4& vec) const = 0;
	/// Set the length of the vector
	/// \param vec - The vector
	/// \param newLength - The length to set
	virtual void SetLength(Vec4& vec, float newLength) = 0;
	/// Calculate the dot product of two vectors
	/// \param a - The first vector
	/// \param b - The second vector
	/// \return The dot product of vectors a and b
	virtual float Dot(const Vec4& a, const Vec4& b) const = 0;
	/// Normalize a given vector.
	/// \param v - vector to normalize
	/// \return The length of the given vector before normalizing
	virtual void Normalize(Vec4 &v) const = 0;
};

} //namespace ElementalEngine

#endif // IVEC4IMPLEMENTATION_H