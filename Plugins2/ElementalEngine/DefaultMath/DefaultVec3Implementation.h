///==========================================================================
/// \file	DefaultVec3Implementation.h
/// \brief	
/// \date	08/10/2005
/// \author	Brian Bazyk
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

#ifndef DEFAULTVEC3IMPLEMENTATION_H__
#define DEFAULTVEC3IMPLEMENTATION_H__

#include "IVec3Implementation.h"
#include "Vec3.h"

namespace ElementalEngine
{

class CDefaultVec3Implementation : public IVec3Implementation
{
private:
	void Init( void );
	void CleanUp( void );

	CDefaultVec3Implementation()
	{
		Init();
	}

	virtual ~CDefaultVec3Implementation()
	{
		CleanUp();
	}

public:
	static IVec3Implementation* Instance( void )
	{
		if ( s_instance == NULL )
		{
			s_instance = new CDefaultVec3Implementation();
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

	/// Return true if two Vec3 are equal
	/// \param a - The first Vec3 to test
	/// \param b - The second Vec3 to test
	virtual bool Equals(const Vec3& a, const Vec3& b) const;

	/// Return true if two Vec3 are not equal
	/// \param a - The first Vec3 to test
	/// \param b - The second Vec3 to test
	virtual bool NotEquals(const Vec3& a, const Vec3& b) const;

	virtual bool LessThan(const Vec3& a, const Vec3& b) const;

	virtual bool GreaterThan(const Vec3& a, const Vec3& b) const;

	/// \brief	Add two vectors
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	virtual	Vec3 Add( const Vec3& a, const Vec3& b ) const;
	/// \brief	Add two vectors and store the results in this
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	/// \return	The result of adding a and b
	virtual	Vec3& AddEquals( Vec3& a, const Vec3& b ) const;

	/// \brief	Subtract two vectors
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	virtual	Vec3 Subtract(const Vec3& a, const Vec3& b) const;
	
	/// \brief	Add two vectors
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	virtual	Vec3& SubtractEquals( Vec3& a, const Vec3& b ) const;

	/// \brief	Multiply two vectors
	/// \param	a - The first vector to multiply
	/// \param	b - The second vector to multiply
	virtual	Vec3 Times(const Vec3& a, const Vec3& b) const;

	/// \brief	Multiply two vectors and store the results in this
	/// \param	a - The first vector to multiply
	/// \param	b - The second vector to multiply
	/// \return	The result of multiplying a and b
	virtual	const Vec3& TimesEquals(Vec3& a, const Vec3& b) const;

	/// \brief	Multiply a vector by a scalar
	/// \param	a - The vector to multiply
	/// \param	scalar - The scalar to multiply
	virtual	Vec3 Times(const Vec3& a, float scalar) const;

	/// \brief	Multiply a vector by a scalar and store the results in this
	/// \param	a - The vector to multiply
	/// \param	scalar - The scalar to multiply
	/// \return	The result of multiplying a and b
	virtual	const Vec3& TimesEquals(Vec3& a, float scalar) const;

	/// \brief	Multiply a vector by a Matrix3x3
	/// \param	a - The first vector to multiply
	/// \param	b - The second vector to multiply
	virtual	Vec3 Times(const Vec3& a, const Matrix3x3& m) const;

	/// \brief	Multiply a vector by a Matrix3x3
	/// \param	a - The first vector to multiply
	/// \param	m - Matrix3x3 to translate the Vec3 by
	/// \return	The result of multiplying a and m
	virtual	const Vec3& TimesEquals(Vec3& a, const Matrix3x3& m) const;

	/// \brief	Divide a vector by another vector
	/// \param	a - The vector to divide
	/// \param	b - The vector to divide by
	virtual	Vec3 Divide(const Vec3& a, const Vec3& b) const;

	/// \brief	Divide a vector by another vector and store the results in this
	/// \param	a - The vector to divide
	/// \param	b - The vector to divide by
	/// \return	The result of dividing a by b
	virtual	const Vec3& DivideEquals(Vec3& a, const Vec3& b) const;

	/// \brief	Divide a vector by a scalar
	/// \param	a - The vector to divide
	/// \param	scalar - The scalar to divide by
	virtual	Vec3 Divide(const Vec3& a,  float scalar) const;

	/// \brief	Divide a vector by a scalar and store the results in this
	/// \param	a - The vector to divide
	/// \param	scalar - The scalar to divide by
	/// \return	The result of dividing a by b
	virtual	const Vec3& DivideEquals(Vec3& a,  float scalar) const;

	/// Return the length of the vector
	/// \param vec - The vector
	/// \return the length of the vector
	virtual float Length(const Vec3& vec) const;

	/// Return the length squared of the vector
	/// \param vec - The vector
	/// \return the length squared  of the vector
	virtual float LengthSQ(const Vec3& vec) const;

	/// Set the length of the vector
	/// \param vec - The vector
	/// \param newLength - The length to set
	virtual void SetLength(Vec3& vec, float newLength);

	/// Calculate the dot product of two vectors
	/// \param a - The first vector
	/// \param b - The second vector
	/// \return The dot product of vectors a and b
	virtual float Dot(const Vec3& a, const Vec3& b) const;

	/// Calculate the cross product of two vectors
	/// \param a - The first vector
	/// \param b - The second vector
	/// \return The cross product of vectors a and b
	virtual Vec3 Cross(const Vec3& a, const Vec3& b) const;

	/// Calculate the distance between two vectors
	/// \param a - The first vector
	/// \param b - The second vector
	/// \return The distance between vectors a and b
	virtual float DistanceBetween(const Vec3& a, const Vec3& b);

	/// Calculate the distance squared between two vectors
	/// \param a - The first vector
	/// \param b - The second vector
	/// \return The distance squared between vectors a and b
	virtual float DistanceBetweenSQ(const Vec3& a, const Vec3& b);

	/// Invert the Vector
	/// \param vec - Vector to invert
	virtual void Invert(Vec3& vec);

	/// Normalize a given vector.
	/// \param v - vector to normalize
	/// \return The length of the given vector before normalizing
	virtual void Normalize(Vec3 &v) const;

	/// Rotates a vector about the global x axis
	/// \param v - vector to rotate
	/// \param degRotation - degrees to rotate	
	virtual void RotateAboutX(Vec3 &v, float degRotation);
	/// Rotates a vector about the global y axis
	/// \param v - vector to rotate
	/// \param degRotation - degrees to rotate	
	virtual void RotateAboutY(Vec3 &v, float degRotation);
	/// Rotates a vector about the global z axis
	/// \param v - vector to rotate
	/// \param degRotation - degrees to rotate	
	virtual void RotateAboutZ(Vec3 &v, float degRotation);

private:
	static IVec3Implementation* s_instance;
};

} //namespace ElementalEngine

#endif // DEFAULTVEC3IMPLEMENTATION_H__