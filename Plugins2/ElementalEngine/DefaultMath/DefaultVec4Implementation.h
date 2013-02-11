///==========================================================================
/// \file	DefaultVec4Implementation.h
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

#ifndef DEFAULTVEC4IMPLEMENTATION_H__
#define DEFAULTVEC4IMPLEMENTATION_H__

#include "IVec4Implementation.h"
#include "Vec4.h"

namespace ElementalEngine
{

class CDefaultVec4Implementation : public IVec4Implementation
{
private:
	void Init( void );
	void CleanUp( void );

	CDefaultVec4Implementation()
	{
		Init();
	}

	virtual ~CDefaultVec4Implementation()
	{
		CleanUp();
	}

public:
	static IVec4Implementation* Instance( void )
	{
		if ( s_instance == NULL )
		{
			s_instance = new CDefaultVec4Implementation();
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

	/// Return true if two Vec4 are equal
	/// \param a - The first Vec4 to test
	/// \param b - The second Vec4 to test
	virtual bool Equals(const Vec4& a, const Vec4& b) const;

	/// Return true if two Vec4 are not equal
	/// \param a - The first Vec4 to test
	/// \param b - The second Vec4 to test
	virtual bool NotEquals(const Vec4& a, const Vec4& b) const;

	virtual bool LessThan(const Vec4& a, const Vec4& b) const;

	virtual bool GreaterThan(const Vec4& a, const Vec4& b) const;

	/// \brief	Add two vectors
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	virtual	Vec4 Add( const Vec4& a, const Vec4& b ) const;
	/// \brief	Add two vectors and store the results in this
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	/// \return	The result of adding a and b
	virtual	Vec4& AddEquals( Vec4& a, const Vec4& b ) const;

	/// \brief	Subtract two vectors
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	virtual	Vec4 Subtract(const Vec4& a, const Vec4& b) const;
	
	/// \brief	Add two vectors
	/// \param	a - The first vector to add
	/// \param	b - The second vector to add
	virtual	Vec4& SubtractEquals( Vec4& a, const Vec4& b ) const;

	/// \brief	Multiply two vectors
	/// \param	a - The first vector to multiply
	/// \param	b - The second vector to multiply
	virtual	Vec4 Times(const Vec4& a, const Vec4& b) const;

	/// \brief	Multiply two vectors and store the results in this
	/// \param	a - The first vector to multiply
	/// \param	b - The second vector to multiply
	/// \return	The result of multiplying a and b
	virtual	const Vec4& TimesEquals(Vec4& a, const Vec4& b) const;

	/// \brief	Multiply a vector by a scalar
	/// \param	a - The vector to multiply
	/// \param	scalar - The scalar to multiply
	virtual	Vec4 Times(const Vec4& a, float scalar) const;

	/// \brief	Multiply a vector by a scalar and store the results in this
	/// \param	a - The vector to multiply
	/// \param	scalar - The scalar to multiply
	/// \return	The result of multiplying a and b
	virtual	const Vec4& TimesEquals(Vec4& a, float scalar) const;

	/// \brief	Divide a vector by another vector
	/// \param	a - The vector to divide
	/// \param	b - The vector to divide by
	virtual	Vec4 Divide(const Vec4& a, const Vec4& b) const;

	/// \brief	Divide a vector by another vector and store the results in this
	/// \param	a - The vector to divide
	/// \param	b - The vector to divide by
	/// \return	The result of dividing a by b
	virtual	const Vec4& DivideEquals(Vec4& a, const Vec4& b) const;

	/// \brief	Divide a vector by a scalar
	/// \param	a - The vector to divide
	/// \param	scalar - The scalar to divide by
	virtual	Vec4 Divide(const Vec4& a,  float scalar) const;

	/// \brief	Divide a vector by a scalar and store the results in this
	/// \param	a - The vector to divide
	/// \param	scalar - The scalar to divide by
	/// \return	The result of dividing a by b
	virtual	const Vec4& DivideEquals(Vec4& a,  float scalar) const;

	/// Return the length of the vector
	/// \param vec - The vector
	/// \return the length of the vector
	virtual float Length(const Vec4& vec) const;

	/// Return the length squared of the vector
	/// \param vec - The vector
	/// \return the length squared  of the vector
	virtual float LengthSQ(const Vec4& vec) const;

	/// Set the length of the vector
	/// \param vec - The vector
	/// \param newLength - The length to set
	virtual void SetLength(Vec4& vec, float newLength);

	/// Calculate the dot product of two vectors
	/// \param a - The first vector
	/// \param b - The second vector
	/// \return The dot product of vectors a and b
	virtual float Dot(const Vec4& a, const Vec4& b) const;

	/// Normalize a given vector.
	/// \param v - vector to normalize
	/// \return The length of the given vector before normalizing
	virtual void Normalize(Vec4 &v) const;

private:
	static IVec4Implementation* s_instance;
};

} //namespace ElementalEngine

#endif // DEFAULTVEC4IMPLEMENTATION_H__