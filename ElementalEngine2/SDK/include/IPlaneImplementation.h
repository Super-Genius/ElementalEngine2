///==========================================================================
/// \file	IPlaneImplementation.h
/// \brief	Plane math function implementation interface
/// \date	8\16\2006
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

#ifndef	IPLANEIMPL_H__
#define	IPLANEIMPL_H__

namespace ElementalEngine
{

class Plane;
class Vec3;

// The enumeration describing if an object intersects or lies on either side of the
// plane
enum PLANECOLLISION { PLANECOLLISION_INTERSECT,
					  PLANECOLLISION_NORMALSIDE,
					  PLANECOLLISION_NONNORMALSIDE };

///==========================================================================
/// \class	IPlaneImplementation
/// \brief	
///==========================================================================
class __declspec(novtable) IPlaneImplementation
{
public:
	/// Takes 3 points and creates the plane
	/// normal is determined by v1-v2 X v1-v3
	/// \param PointA = first point
	/// \param PointB = second point
	/// \param PointC = third point
	virtual void GeneratePlane( Plane& plane, const Vec3 &PointA, const Vec3 &PointB, const Vec3 &PointC ) = 0;
	
	/// \brief Takes a normal and a point on the plane to generate the plane
	/// \param normal = plane normal
	/// \param point = point on plane
	virtual void GeneratePlane( Plane& plane, const Vec3 &normal, const Vec3 &point ) = 0;

	/// Fast sphere collision check with the plane
	/// \param position = position of the sphere
	/// \param radius = radius of the sphere
	/// \return PLANECOLLISION = an enumerated flag that describes the type of collision
	///							or where the sphere lies in relation to the plane
	virtual PLANECOLLISION SphereCollision( Plane& plane, const Vec3 &position, const float radius ) = 0;

	/// Fast Point collision check with the plane
	/// \param position = position of the point
	/// \return PLANECOLLISION = an enumerated flag that describes the type of collision
	///							or where the sphere lies in relation to the plane
	virtual PLANECOLLISION PointCollision( Plane& plane, const Vec3 &position ) = 0;

};

} //namespace ElementalEngine

#endif // IPLANEIMPL_H__