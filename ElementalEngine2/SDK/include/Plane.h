///==========================================================================
/// \file	Plane.h
/// \brief	
/// \date	08/16/2006
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

#ifndef PLANE_H__
#define PLANE_H__

#include "IPlaneImplementation.h"

namespace ElementalEngine
{

int SetPlaneImplementation();

#define DEFAULTPLANEBITS (0)

class Plane
{
private:
	friend int SetPlaneImplementation();
	static IPlaneImplementation *impl;

public:
	static void Init(IPlaneImplementation* planeImpl)
	{
		impl = planeImpl;
	}

	Plane() : 
		pConstA( 0 ), 
		pConstB( 0 ), 
		pConstC( 1 ),
		pConstD( 0 )
	{
		if(impl == NULL)
		{
			SetPlaneImplementation();
		}
	}

public:
	/// these constants make up the plane equation
	float pConstA, pConstB, pConstC, pConstD;

	/// Plane constructor, takes 3 points and creates the plane
	/// normal is determined by v1-v2 X v1-v3
	/// \param PointA = first point
	/// \param PointB = second point
	/// \param PointC = third point
	Plane( const Vec3 &PointA, const Vec3 &PointB, const Vec3 &PointC )
	{
		GeneratePlane( PointA, PointB, PointC );
	}

	/// Constructor that takes a normal and a point on the plane to generate the plane
	/// \param normal = plane normal
	/// \param point = point on plane
	Plane( const Vec3 &normal, const Vec3 &point )
	{
		GeneratePlane( normal, point );
	}
	//...

	/// Takes 3 points and creates the plane
	/// normal is determined by v1-v2 X v1-v3
	/// \param PointA = first point
	/// \param PointB = second point
	/// \param PointC = third point
	void GeneratePlane( const Vec3& pointA, const Vec3& pointB, const Vec3& pointC )
	{
		impl->GeneratePlane( *this, pointA, pointB, pointC );
	}
	
	/// \brief Takes a normal and a point on the plane to generate the plane
	/// \param normal = plane normal
	/// \param point = point on plane
	void GeneratePlane( const Vec3& normal, const Vec3& point )
	{
		impl->GeneratePlane( *this, normal, point );
	}

	/// Fast sphere collision check with the plane
	/// \param position = position of the sphere
	/// \param radius = radius of the sphere
	/// \return PLANECOLLISION = an enumerated flag that describes the type of collision
	///							or where the sphere lies in relation to the plane
	PLANECOLLISION SphereCollision( const Vec3& position, const float radius )
	{
		return impl->SphereCollision( *this, position, radius );
	}

	/// Fast Point collision check with the plane
	/// \param position = position of the point
	/// \return PLANECOLLISION = an enumerated flag that describes the type of collision
	///							or where the sphere lies in relation to the plane
	PLANECOLLISION PointCollision( const Vec3& position )
	{
		return impl->PointCollision( *this, position );
	}
	
	/// Gets the normal of the plane
	/// \return Vec3 = the normal of the plane
	inline Vec3 GetNormal()
	{
		return Vec3( pConstA, pConstB, pConstC );
	};
	
};

} //namespace ElementalEngine

#ifndef EE_SDK_EXPORTS
#pragma comment(lib, "EE-SDK.lib")
#endif

#endif //PLANE_H__