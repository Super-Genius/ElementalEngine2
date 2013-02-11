///==========================================================================
/// \file	DefaultQuaternionImpl.h
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

#ifndef DEFAULTQUATERNIONIMPL_H__
#define DEFAULTQUATERNIONIMPL_H__

#include "IQuaternionImplementation.h"
#include "Quaternion.h"

namespace ElementalEngine
{

class CDefaultQuaternionImplementation : public IQuaternionImplementation
{
private:
	void Init( void );
	void CleanUp( void );

	CDefaultQuaternionImplementation()
	{
		Init();
	}

	virtual ~CDefaultQuaternionImplementation()
	{
		CleanUp();
	}

public:
	static IQuaternionImplementation* Instance( void )
	{
		if ( s_instance == NULL )
		{
			s_instance = new CDefaultQuaternionImplementation();
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

	/// Return true if two Quaternions are equal
	/// \param a - The first Quaternion to test
	/// \param b - The second Quaternion to test
	virtual bool Equals(const Quaternion& a, const Quaternion& b) const;

	/// Add two Quaternions
	/// \param	a - The first Quaternion to add
	/// \param	b - The second Quaternion to add
	virtual	Quaternion Add(const Quaternion& a, const Quaternion& b) const;

	/// Multiply two Quaternions
	/// \param	a - The first Quaternion to multiply
	/// \param	b - The second Quaternion to multiply
	virtual Quaternion Times(const Quaternion& a, const Quaternion& b) const;

	/// Multiply two Quaternions and store the results in this 
	/// \param	a - The first Quaternion to multiply
	/// \param	b - The second Quaternion to multiply
	virtual void TimesEquals(Quaternion& a, const Quaternion& b);

	/// Multiply a Quaternion and a scalar
	/// \param	a - The first Quaternion to multiply
	/// \param	b - Scalar value
	virtual Quaternion Times(const Quaternion& a, float f) const;

	/// Multiply a Quaternion and a scalar and store the results in this 
	/// \param	a - The first Quaternion to multiply
	/// \param	b - Scalar value
	virtual void TimesEquals(Quaternion& a, float s);

	/// Divide two Quaternions
	/// \param	a - The first Quaternion to divide
	/// \param	b - The second Quaternion to divide
	virtual Quaternion Divide(const Quaternion& a, const Quaternion& b) const;

	/// Multiply two Quaternions and store the results in this 
	/// \param	a - The first Quaternion to multiply
	/// \param	b - The second Quaternion to multiply
	virtual void DivideEquals(Quaternion& a, const Quaternion& b);

	/// Set the Quaternion from X,Y,Z,W tuple
	/// \param quat - The Quaternion to set
	/// \param newX - The X value
	/// \param newY - The Y value
	/// \param newZ - The Z value
	/// \param newW - The W value
	virtual void Set(Quaternion& quat, float newX, float newY, float newZ, float newW);

	/// Calculate the dot product of twi Quaternions
	/// \param a - The first Quaternion
	/// \param b - The second Quaternion
	/// \reutrn The dot product of Quaternions a and b
	virtual float DotProduct(const Quaternion& a, const Quaternion& b) const;

	/// Normalize a given Quaternion.
	/// \param quat - Quaternion to normalize
	virtual void Normalize(Quaternion& quat);

	/// Convert a Quaternion to a Matrix4x4
	/// \param	quat - Quaternion to convert
	/// \return The converted Matrix4x4
	virtual void GetMatrix(const Quaternion& quat, Matrix4x4& out) const;

	/// Set a quaternion from a Matrix3x3
	/// \param	quat - converted quaternion
	/// \return Matrix4x4 to convert
	virtual void SetFromMatrix( Quaternion& quat, const Matrix3x3& in );

	/// Set a quaternion from a Matrix4x4
	/// \param	quat - converted quaternion
	/// \return Matrix4x4 to convert
	virtual void SetFromMatrix( Quaternion& quat, const Matrix4x4& in );

	/// Invert the Quaternion
	/// \param quat - Quaternion to invert
	virtual void MakeInverse(Quaternion& quat);

	/// Calculate the Spherical Linear Interpolation between two Quaternions
	/// \param a - The first Quaternion to interpolate between
	/// \parma b - The second Quaternion to interpolate between
	/// \param time - Value between 0 and 1 that determines how far along the interpolation is
	/// \return The SLERP'd Quaternion
	virtual Quaternion Slerp(Quaternion a, Quaternion b, float time);

	/// Convert a Quaternion to euler angles
	/// \param quat - The Quaternion to convert
	/// \param euler - A Vec3 receiving the converted euler angles
	virtual void ToEuler(const Quaternion& quat, EulerAngle& euler);

	/// Construct a Quaternion from euler angles
	/// \param quat - The Quaternion to store the converted euler angles
	/// \param euler - A Vec3 with the euler angles
	virtual void FromEuler(Quaternion& quat, EulerAngle& euler);

private:
	static IQuaternionImplementation* s_instance;

};

} // namespace ElementalEngine

#endif // DEFAULTQUATERNIONIMPL_H__