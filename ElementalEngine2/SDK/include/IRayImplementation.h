///============================================================================
/// \file	IRayImplementation.h
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

#ifndef RAYIMPLEMENTATION_H
#define RAYIMPLEMENTATION_H

namespace ElementalEngine
{

class Ray;

class __declspec(novtable) IRayImplementation
{
public:

	/// Return true if this ray intersects a triangle
	/// \param ray - the ray to compare with
	/// \param vert0, vert1, vert2 - the 3 vertices of the triangle
	/// \param t - the distance between the ray and the point of intersection
	/// \param u - the intersection point along the axis of the v0-v1 edge where v0 = 0.0 and v1 = 1.0
	/// \param v - the intersection point along the axis of the v0-v2 edge where v0 = 0.0 and v2 = 1.0
	virtual bool IntersectTriangle( Ray &ray, Vec3 &vert0, Vec3 &vert1, Vec3 &vert2,
									double *t, double *u, double *v) = 0;

};

} //namespace EE

#endif // RAYIMPLEMENTATION_H