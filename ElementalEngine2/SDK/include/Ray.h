///============================================================================
/// \file	Ray.h
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

#ifndef RAY_H
#define RAY_H

#include "IRayImplementation.h"
#include <float.h>
#include "Vec3.h"

namespace ElementalEngine
{

int SetRayImplementation();

class Ray 
{
private:
	friend int SetRayImplementation();
	static IRayImplementation * impl;

public:
	Vec3 m_Origin;
	Vec3 m_Direction;
	Vec3 m_InvDirection;
	int m_Sign[3];
	
	Ray()
	{
	}
	Ray( Vec3 &origin, Vec3 &direction ) 
	{
		Set( origin, direction );
	}
	inline void Set( Vec3 &origin, Vec3 &direction ) 
	{
		m_Origin = origin;
		m_Direction = direction;
		m_InvDirection = Vec3(1/direction.x, 1/direction.y, 1/direction.z);
		m_Sign[0] = (m_InvDirection.x < 0);
		m_Sign[1] = (m_InvDirection.y < 0);
		m_Sign[2] = (m_InvDirection.z < 0);
	}

	/// Return true if this ray intersects a triangle
	/// \param vert0, vert1, vert2 - the 3 vertices of the triangle
	/// \param t - the distance between the ray and the point of intersection
	/// \param u - the intersection point along the axis of the v0-v1 edge where v0 = 0.0 and v1 = 1.0
	/// \param v - the intersection point along the axis of the v0-v2 edge where v0 = 0.0 and v2 = 1.0
	inline bool IntersectTriangle( Vec3 &vert0, Vec3 &vert1, Vec3 &vert2,
				double *t, double *u, double *v)
	{
		return impl->IntersectTriangle( *this, vert0, vert1, vert2, t, u, v );
	}
};

} //namespace EE

#endif