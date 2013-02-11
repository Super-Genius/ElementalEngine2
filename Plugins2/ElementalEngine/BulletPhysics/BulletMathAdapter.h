///==========================================================================
/// \file	BulletMathAdapter.h
/// \brief	Bullet Physics adapter conversion of math classes
/// \date	03/31/2009
/// \author	Marivaldo Cabral
///         Copyright (c)  2005-2009 Signature Devices, Inc.
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


#ifndef BULLET_MATH_ADAPTER_H
#define BULLET_MATH_ADAPTER_H

///==========================================================================
/// \class	BtVec3Adapter
/// \brief	Bullet Physics adapter conversion of Vec3 to btVector3. Only for
/// internal physics plugin use.
///==========================================================================
class BtVec3Adapter : public btVector3
{
public:
	/// Default Constructor
	BtVec3Adapter()
	{
		m_x = 0.0f;
		m_y = 0.0f;
		m_z = 0.0f;
	}

		/// Construct from three floats
	BtVec3Adapter( float xVal, float yVal, float zVal )
	{
		m_x = xVal;
		m_y = yVal;
		m_z = zVal;
	}

	/// Construct from a Vec3
	BtVec3Adapter( Vec3 &src )
	{
		m_x = src.x;
		m_y = src.y;
		m_z = src.z;
	}

	/// Construct from a Vec3
	BtVec3Adapter( const Vec3 &src )
	{
		m_x = src.x;
		m_y = src.y;
		m_z = src.z;
	}

	/// Assignment operator
	btVector3 &operator = ( Vec3 &src )
	{
		const btScalar x = btScalar(src.x);
		const btScalar y = btScalar(src.y);
		const btScalar z = btScalar(src.z);
		setValue(x, y, z, 0.0f);
		
		return *this;
	}

	/// Assignment operator
	btVector3 &operator = ( const Vec3 &src )
	{
		setValue(src.x, src.y, src.z, 0.0f);
		return *this;
	}

};

///==========================================================================
/// \class	BtMat33Adapter
/// \brief	Bullet Physics adapter conversion of CMatrix3x3 to btMatrix3x3
///==========================================================================
class BtMat33Adapter : public btMatrix3x3
{
public:

	////Default Construction
	BtMat33Adapter()
	{
		setValue(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}

	/// Construct from a Matrix3x3
	BtMat33Adapter( Matrix3x3 &src )
	{
		setValue(src.m[0], src.m[1], src.m[2], src.m[3], src.m[4], src.m[5], src.m[6], src.m[7], src.m[8]);
	}

	/// Construct from a Matrix3x3
	BtMat33Adapter( const Matrix3x3 &src )
	{
		setValue(src.m[0], src.m[1], src.m[2], src.m[3], src.m[4], src.m[5], src.m[6], src.m[7], src.m[8]);
	}

	/// Assignment Operator
	btMatrix3x3 &operator = ( Matrix3x3 &src )
	{
		setValue(src.m[0], src.m[1], src.m[2], src.m[3], src.m[4], src.m[5], src.m[6], src.m[7], src.m[8]);
		return *this;
	}

	/// Assignment Operator
	btMatrix3x3 &operator = ( const Matrix3x3 &src )
	{
		setValue(src.m[0], src.m[1], src.m[2], src.m[3], src.m[4], src.m[5], src.m[6], src.m[7], src.m[8]);
		return *this;
	}
};

///==========================================================================
/// \class	BtQuatAdapter
/// \brief	Bullet Physics adapter conversion of Quaternion to btQuaternion
///==========================================================================
class BtQuatAdapter : public btQuaternion
{
public:
	/// Default Constructor
	BtQuatAdapter()
	{
		setValue(0.0f, 0.0f, 0.0f, 0.0f);
	}

	/// Construct from a Quaternion.
	BtQuatAdapter( Quaternion &src )
	{
		setValue(src.x, src.y, src.z, src.w);
	}

	/// Construct from a Quaternion.
	BtQuatAdapter( const Quaternion &src )
	{
		setValue(src.x, src.y, src.z, src.w);
	}

	/// Assignment Operator
	btQuaternion &operator = ( Quaternion &src )
	{
		setValue(src.x, src.y, src.z, src.w);
		return *this;
	}

	/// Assignment Operator
	btQuaternion &operator = ( const Quaternion &src )
	{
		setValue(src.x, src.y, src.z, src.w);
		return *this;
	}
};
#endif // BULLET_MATH_ADAPTER_H