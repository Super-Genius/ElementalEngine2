///==========================================================================
/// \file	DefaultEulerImpl.h
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

#ifndef DEFAULTEulerImpl_H__
#define DEFAULTEulerImpl_H__

#include "IEulerImplementation.h"
#include "EulerAngle.h"

namespace ElementalEngine
{

class CDefaultEulerImplementation : public IEulerImplementation
{
private:
	void Init( void );
	void CleanUp( void );

	CDefaultEulerImplementation()
	{
		Init();
	}

	virtual ~CDefaultEulerImplementation()
	{
		CleanUp();
	}

public:
	static IEulerImplementation* Instance( void )
	{
		if ( s_instance == NULL )
		{
			s_instance = new CDefaultEulerImplementation();
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

	/// Return true if two EulerAngles are equal
	/// \param a - The first EulerAngle to test
	/// \param b - The second EulerAngle to test
	virtual bool Equals(const EulerAngle& a, const EulerAngle& b) const;

	/// Add two EulerAngles
	/// \param	a - The first EulerAngle to add
	/// \param	b - The second EulerAngle to add
	virtual	EulerAngle Add(const EulerAngle& a, const EulerAngle& b) const;

	/// Add two EulerAngles
	/// \param	a - The first EulerAngle to add
	/// \param	b - The second EulerAngle to add
	virtual	EulerAngle Subtract(const EulerAngle& a, const EulerAngle& b) const;

	/// Multiply a EulerAngle and a scalar
	/// \param	a - The first EulerAngle to multiply
	/// \param	b - Scalar value
	virtual EulerAngle Times(const EulerAngle& a, float f) const;

	/// Multiply a EulerAngle and a scalar and store the results in this 
	/// \param	a - The first EulerAngle to multiply
	/// \param	b - Scalar value
	virtual void TimesEquals(EulerAngle& a, float s);

	/// Set the EulerAngle from X,Y,Z,W tuple
	/// \param euler - The EulerAngle to set
	/// \param newX - The X value
	/// \param newY - The Y value
	/// \param newZ - The Z value
	/// \param newW - The W value
	virtual void Set(EulerAngle& euler, float roll, float pitch, float yaw );

	/// Convert a EulerAngle to a Matrix4x4
	/// \param	euler - EulerAngle to convert
	/// \return The converted Matrix4x4
	virtual void GetMatrix(const EulerAngle& euler, Matrix4x4& out) const;

	/// Convert a EulerAngle to a Matrix3x3
	/// \param	euler - EulerAngle to convert
	/// \return The converted Matrix3x3
	virtual void GetMatrix(const EulerAngle& euler, Matrix3x3& out) const;

	/// Set a EulerAngle from a Matrix3x3
	/// \param	euler - converted EulerAngle
	/// \return Matrix4x4 to convert
	virtual void SetFromMatrix( EulerAngle& euler, const Matrix3x3& in );

	/// Set a EulerAngle from a Matrix4x4
	/// \param	euler - converted EulerAngle
	/// \return Matrix4x4 to convert
	virtual void SetFromMatrix( EulerAngle& euler, const Matrix4x4& in );

	/// Invert the EulerAngle
	/// \param euler - EulerAngle to invert
	virtual void MakeInverse(EulerAngle& euler);

	/// Convert a EulerAngle to euler angles
	/// \param euler - The EulerAngle to convert
	/// \param euler - A Vec3 receiving the converted euler angles
	virtual void ToQuaternion( const EulerAngle& euler, Quaternion& quat);

	/// Construct a EulerAngle from euler angles
	/// \param euler - The EulerAngle to store the converted euler angles
	/// \param euler - A Vec3 with the euler angles
	virtual void FromQuaternion( EulerAngle& euler, Quaternion &quat);

private:
	static IEulerImplementation* s_instance;

};

} // namespace ElementalEngine

#endif // DEFAULTEulerImpl_H__