///==========================================================================
/// \file	NovodexMathAdapter.h
/// \brief	Novodex SDK adapter conversion of math classes
/// \date	3/15/2005
/// \date	3/24/2009
/// \author	Kenneth Hurley, Kyle Swaim
/// \author	Marivaldo Cabral
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

#ifndef NOVODEX_MATH_ADAPTER_H
#define	NOVODEX_MATH_ADAPTER_H

#include "NxVec3.h"
#include "NxMat33.h"
#include "NxMat34.h"
#include "NxQuat.h"

///==========================================================================
/// \class	NxVec3Adapter
/// \brief	Novodex SDK adapter conversion of Vec3 to NxVec3. Only for
/// internal physics plugin use.
///==========================================================================
class NxVec3Adapter : public NxVec3
{
public:
	/// Default Constructor
	NxVec3Adapter()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	/// Construct from three floats
	NxVec3Adapter( float xVal, float yVal, float zVal )
	{
		x = xVal;
		y = yVal;
		z = zVal;
	}

	/// Construct from a Vec3
	NxVec3Adapter( Vec3 &src )
	{
		x = src.x;
		y = src.y;
		z = src.z;
	}

	/// Construct from a Vec3
	NxVec3Adapter( const Vec3 &src )
	{
		x = src.x;
		y = src.y;
		z = src.z;
	}

	/// Assignment operator
	NxVec3 &operator = ( Vec3 &src )
	{
		set( src.x, src.y, src.z );
		return *this;
	}

	/// Assignment operator
	NxVec3 &operator = ( const Vec3 &src )
	{
		set( src.x, src.y, src.z );
		return *this;
	}
};

///==========================================================================
/// \class	NxMat33Adapter
/// \brief	Novodex SDK adapter conversion of CMatrix3x3 to NxMat33
///==========================================================================
class NxMat33Adapter : public NxMat33
{
public:
	/// Default Constructor
	NxMat33Adapter()
	{
		zero();
	}

	/// Construct from a Matrix3x3
	NxMat33Adapter( Matrix3x3 &src )
	{
		setRow( 0, NxVec3( src.m[0], src.m[1], src.m[2] ) );
		setRow( 1, NxVec3( src.m[3], src.m[4], src.m[5] ) );
		setRow( 2, NxVec3( src.m[6], src.m[7], src.m[8] ) );
	}

	/// Construct from a Matrix3x3
	NxMat33Adapter( const Matrix3x3 &src )
	{
		setRow( 0, NxVec3( src.m[0], src.m[1], src.m[2] ) );
		setRow( 1, NxVec3( src.m[3], src.m[4], src.m[5] ) );
		setRow( 2, NxVec3( src.m[6], src.m[7], src.m[8] ) );
	}

	/// Assignment Operator
	NxMat33 &operator = ( Matrix3x3 &src )
	{
		setRow( 0, NxVec3( src.m[0], src.m[1], src.m[2] ) );
		setRow( 1, NxVec3( src.m[3], src.m[4], src.m[5] ) );
		setRow( 2, NxVec3( src.m[6], src.m[7], src.m[8] ) );
		return *this;
	}

	/// Assignment Operator
	NxMat33 &operator = ( const Matrix3x3 &src )
	{
		setRow( 0, NxVec3( src.m[0], src.m[1], src.m[2] ) );
		setRow( 1, NxVec3( src.m[3], src.m[4], src.m[5] ) );
		setRow( 2, NxVec3( src.m[6], src.m[7], src.m[8] ) );
		return *this;
	}

	Matrix3x3 ToMatrix3x3() 
	{
		return Matrix3x3( getRow(0).x, getRow(0).y, getRow(0).z,
			              getRow(1).x, getRow(1).y, getRow(1).z,
				   	      getRow(2).x, getRow(2).y, getRow(2).z );
		
	}
};

///==========================================================================
/// \class	NxQuatAdapter
/// \brief	Novodex SDK adapter conversion of Quaternion to NxQuat
///==========================================================================
class NxQuatAdapter : public NxQuat
{
public:
	/// Default Constructor
	NxQuatAdapter()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}

	/// Construct from a Quaternion.
	NxQuatAdapter( Quaternion &src )
	{
		x = src.x;
		y = src.y;
		z = src.z;
		w = src.w;
	}

	/// Construct from a Quaternion.
	NxQuatAdapter( const Quaternion &src )
	{
		x = src.x;
		y = src.y;
		z = src.z;
		w = src.w;
	}

	/// Assignment Operator
	NxQuat &operator = ( Quaternion &src )
	{
		x = src.x;
		y = src.y;
		z = src.z;
		w = src.w;
		return *this;
	}

	/// Assignment Operator
	NxQuat &operator = ( const Quaternion &src )
	{
		x = src.x;
		y = src.y;
		z = src.z;
		w = src.w;
		return *this;
	}
};

///==========================================================================
/// \class	NxMat34Adapter
/// \brief	Novodex SDK adapter conversion of Matrix4x4 to NxMat34
///==========================================================================

class NxMat34Adapter : public NxMat34
{
public:
	/// Default Constructor
	NxMat34Adapter()
	{
		zero();
	}

	/// Construct from a Matrix3x3
	NxMat34Adapter( Matrix4x4 &src )
	{
		setColumnMajor44(src.GetMatrix());
	}

	/// Construct from a NxMat34
	NxMat34Adapter ( NxMat34 &src )
	{
		this->M = src.M;
		this->t = src.t;
	}

	/// Assignment Operator
	NxMat34 &operator = ( Matrix4x4 &src )
	{
		setColumnMajor44( src.GetMatrix() );
		return *this;
	}

	Matrix4x4 ToMatrix4x4()
	{
		return Matrix4x4(M.getRow(0).x, M.getRow(0).y, M.getRow(0).z, 0,
			             M.getRow(1).x, M.getRow(1).y, M.getRow(1).z, 0,
						 M.getRow(2).x, M.getRow(2).y, M.getRow(2).z, 0,
						 t.x, t.y, t.z, 0);
	}

};

#endif	// NOVODEX_MATH_ADAPTER_H