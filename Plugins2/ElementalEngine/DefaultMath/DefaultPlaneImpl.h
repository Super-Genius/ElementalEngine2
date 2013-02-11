///==========================================================================
/// \file	DefaultPlaneImplementation.h
/// \brief	
/// \date	08/16/2006
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

#ifndef DEFAULTPLANEIMPLEMENTATION_H__
#define DEFAULTPLANEIMPLEMENTATION_H__

#include "IPlaneImplementation.h"
#include "Plane.h"

namespace ElementalEngine
{

class CDefaultPlaneImplementation : public IPlaneImplementation
{
private:
	void Init( void );
	void CleanUp( void );

	CDefaultPlaneImplementation()
	{
		Init();
	}

	virtual ~CDefaultPlaneImplementation()
	{
		CleanUp();
	}

public:
	static IPlaneImplementation* Instance( void )
	{
		if ( s_instance == NULL )
		{
			s_instance = new CDefaultPlaneImplementation();
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

	/// Takes 3 points and creates the plane
	/// normal is determined by v1-v2 X v1-v3
	/// \param PointA = first point
	/// \param PointB = second point
	/// \param PointC = third point
	virtual void GeneratePlane( Plane& plane, const Vec3 &PointA, const Vec3 &PointB, const Vec3 &PointC );
	
	/// \brief Takes a normal and a point on the plane to generate the plane
	/// \param normal = plane normal
	/// \param point = point on plane
	virtual void GeneratePlane( Plane& plane, const Vec3 &normal, const Vec3 &point );

	/// Fast sphere collision check with the plane
	/// \param position = position of the sphere
	/// \param radius = radius of the sphere
	/// \return PLANECOLLISION = an enumerated flag that describes the type of collision
	///							or where the sphere lies in relation to the plane
	virtual PLANECOLLISION SphereCollision( Plane& plane, const Vec3 &position, const float radius );

	/// Fast Point collision check with the plane
	/// \param position = position of the point
	/// \return PLANECOLLISION = an enumerated flag that describes the type of collision
	///							or where the sphere lies in relation to the plane
	virtual PLANECOLLISION PointCollision( Plane& plane, const Vec3 &position );

private:
	static IPlaneImplementation* s_instance;
};

} //namespace ElementalEngine

#endif // DEFAULTPLANEIMPLEMENTATION_H__