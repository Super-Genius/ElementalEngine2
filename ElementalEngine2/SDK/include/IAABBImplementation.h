///============================================================================
/// \file	IAABBImplementation.h
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
///============================================================================ 

#ifndef IAABBIMPLEMENTATION_H
#define IAABBIMPLEMENTATION_H

namespace ElementalEngine
{

class AABB;

/// IAABBImplementation
class __declspec(novtable) IAABBImplementation
{	
public:
	/// Intersects a ray with an AABB
	/// \param bb - AABB Bounding Box
	/// \param Ray - ray to use for colliding
	/// \param t0 - float min value
	/// \param t1 - float max value
	/// \return - true on intersection
	virtual bool IntersectRay( AABB &bb, const Ray &r, float &t0, float &t1)  = 0;

	/// Intersects a triangle with an AABB
	/// \param bb - AABB Bounding Box
	/// \param a_V0 - vec3 of 1st side of the triangle
	/// \param a_V1 - vec3 of 2nd side of the triangle
	/// \param a_V2 - vec3 of 3rd side of the triangle
	/// \return - true on intersection
	virtual bool IntersectTri( AABB &bb, Vec3& a_V0, Vec3& a_V1, Vec3& a_V2 )  = 0;

	/// Determines type of intersect between two AABB
	/// \param bb - AABB Bounding Box
	/// \param vMin - vec3 min value
	/// \param vMax - vec3 max value
	/// \return - enum INTERSECTION_RESULT of intersection type (INTERSECTION_INSIDE, INTERSECTION_PARTIAL, INTERSECTION_OUTSIDE) 
	virtual INTERSECTION_RESULT IntersectAABB( AABB &bb, const Vec3 &vMin, const Vec3 &vMax ) = 0;

	/// Determines type of intersect between a sphere and an AABB
	/// \param bb - AABB Bounding Box
	/// \param vCenter - vec3 center of the sphere
	/// \param fRadius - float radius of the sphere
	/// \return - enum INTERSECTION_RESULT of intersection type (INTERSECTION_INSIDE, INTERSECTION_PARTIAL, INTERSECTION_OUTSIDE) 
	virtual INTERSECTION_RESULT IntersectSphere( AABB &bb, const Vec3 &vCenter, const float &fRadius ) = 0;
};

} // namespace EE

#endif // IAABBIMPLEMENTATION_H