///==========================================================================
/// \file	DefaultAABBImplementation.cpp
/// \brief	
/// \date	08/10/2005
/// \author	Marvin Gouw
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

#include "stdafx.h"
#include <math.h>
#include "DefaultAABBImplementation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FINDMINMAX( x0, x1, x2, min, max ) \
  min = max = x0; if(x1<min) min=x1; if(x1>max) max=x1; if(x2<min) min=x2; if(x2>max) max=x2;
// X-tests
#define AXISTEST_X01( a, b, fa, fb )											\
	p0 = a * v0.y - b * v0.z, p2 = a * v2.y - b * v2.z; \
    if (p0 < p2) { min = p0; max = p2;} else { min = p2; max = p0; }			\
	rad = fa * halfsize.y + fb * halfsize.z;				\
	if (min > rad || max < -rad) return 0;
#define AXISTEST_X2( a, b, fa, fb )												\
	p0 = a * v0.y - b * v0.z, p1 = a * v1.y - b * v1.z;	\
    if (p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0;}			\
	rad = fa * halfsize.y + fb * halfsize.z;				\
	if(min>rad || max<-rad) return 0;
// Y-tests
#define AXISTEST_Y02( a, b, fa, fb )											\
	p0 = -a * v0.x + b * v0.z, p2 = -a * v2.x + b * v2.z; \
    if(p0 < p2) { min = p0; max = p2; } else { min = p2; max = p0; }			\
	rad = fa * halfsize.x + fb * halfsize.z;				\
	if (min > rad || max < -rad) return 0;
#define AXISTEST_Y1( a, b, fa, fb )												\
	p0 = -a * v0.x + b * v0.z, p1 = -a * v1.x + b * v1.z; \
    if (p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0; }			\
	rad = fa * halfsize.x + fb * halfsize.z;				\
	if (min > rad || max < -rad) return 0;
// Z-tests
#define AXISTEST_Z12( a, b, fa, fb )											\
	p1 = a * v1.x - b * v1.y, p2 = a * v2.x - b * v2.y; \
    if(p2 < p1) { min = p2; max = p1; } else { min = p1; max = p2; }			\
	rad = fa * halfsize.x + fb * halfsize.y;				\
	if (min > rad || max < -rad) return 0;
#define AXISTEST_Z0( a, b, fa, fb )												\
	p0 = a * v0.x - b * v0.y, p1 = a * v1.x - b * v1.y;	\
    if(p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0; }			\
	rad = fa * halfsize.x + fb * halfsize.y;				\
	if (min > rad || max < -rad) return 0;

IAABBImplementation *CDefaultAABBImplementation::s_instance = NULL;

void CDefaultAABBImplementation::Init( void )
{
}

void CDefaultAABBImplementation::CleanUp( void )
{
}

bool CDefaultAABBImplementation::IntersectRay( AABB &bb, const Ray &ray, float &t0, float &t1)
{
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	tmin = ( bb.m_Bounds[ ray.m_Sign[0] ].x - ray.m_Origin.x) * ray.m_InvDirection.x;
	tmax = ( bb.m_Bounds[ 1-ray.m_Sign[0] ].x - ray.m_Origin.x) * ray.m_InvDirection.x;
	tymin = ( bb.m_Bounds[ ray.m_Sign[1] ].y - ray.m_Origin.y) * ray.m_InvDirection.y;
	tymax = ( bb.m_Bounds[ 1-ray.m_Sign[1] ].y - ray.m_Origin.y) * ray.m_InvDirection.y;
	if ( (tmin > tymax) || (tymin > tmax) )
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	tzmin = ( bb.m_Bounds[ ray.m_Sign[2] ].z - ray.m_Origin.z) * ray.m_InvDirection.z;
	tzmax = ( bb.m_Bounds[ 1-ray.m_Sign[2] ].z - ray.m_Origin.z) * ray.m_InvDirection.z;
	if ( (tmin > tzmax) || (tzmin > tmax) )
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
	t0 = tmin;
	t1 = tmax;
	return true;//( (tmin < t1) && (tmax > t0) );
}

bool CDefaultAABBImplementation::IntersectTri( AABB &bb, Vec3& a_V0, Vec3& a_V1, Vec3& a_V2 )
{
	Vec3 v0, v1, v2, normal, e0, e1, e2;
	float min, max,  fex, fey, fez;
	//float p0, p1, p2, rad;
	v0 = a_V0 - bb.m_Center;
	v1 = a_V1 - bb.m_Center;
	v2 = a_V2 - bb.m_Center;
	e0 = v1 - v0, e1 = v2 - v1, e2 = v0 - v2;
	fex = fabsf( e0.x );
	fey = fabsf( e0.y );
	fez = fabsf( e0.z );
	/*AXISTEST_X01( e0.z, e0.y, fez, fey );
	AXISTEST_Y02( e0.z, e0.x, fez, fex );
	AXISTEST_Z12( e0.y, e0.x, fey, fex );
	fex = fabsf( e1.x );
	fey = fabsf( e1.y );
	fez = fabsf( e1.z );
	AXISTEST_X01( e1.z, e1.y, fez, fey );
	AXISTEST_Y02( e1.z, e1.x, fez, fex );
	AXISTEST_Z0 ( e1.y, e1.x, fey, fex );
	fex = fabsf( e2.x );
	fey = fabsf( e2.y );
	fez = fabsf( e2.z );
	AXISTEST_X2 ( e2.z, e2.y, fez, fey );
	AXISTEST_Y1 ( e2.z, e2.x, fez, fex );
	AXISTEST_Z12( e2.y, e2.x, fey, fex );*/
	FINDMINMAX( a_V0.x, a_V1.x, a_V2.x, min, max );
	if (min > bb.m_Bounds[1].x || max < bb.m_Bounds[0].x) 
		return false;
	FINDMINMAX( a_V0.y, a_V1.y, a_V2.y, min, max );
	if (min > bb.m_Bounds[1].y || max < bb.m_Bounds[0].y)
		return false;
	FINDMINMAX( a_V0.z, a_V1.z, a_V2.z, min, max );
	if (min > bb.m_Bounds[1].z || max < bb.m_Bounds[0].z) 
		return false;
	//normal = e0.Cross( e1 );
	//if (!PlaneBoxOverlap( normal, v0, halfsize )) return false;
	return true;
}

INTERSECTION_RESULT CDefaultAABBImplementation::IntersectAABB( AABB &bb, const Vec3 &vMin, const Vec3 &vMax )
{
	Vec3 min;
	Vec3 max;
	bb.Get( min, max );

	if( min >= vMin && max <= vMax )
	{
		return INTERSECTION_INSIDE;
	}
	else if( min <= vMax && max >= vMin )
	{
		return INTERSECTION_PARTIAL;
	}
	else
	{
		return INTERSECTION_OUTSIDE;
	}
}

INTERSECTION_RESULT CDefaultAABBImplementation::IntersectSphere( AABB &bb, const Vec3 &vCenter, const float &fRadius )
{
	Vec3 min;
	Vec3 max;
	bb.Get( min, max );
	float totalDistance = 0;

	// check if the sphere is inside the AABB

	// The center of the sphere is inside the AABB.
	if( vCenter <= max && vCenter >= min )
	{
		Vec3 sphereMax( vCenter.x + fRadius, vCenter.y + fRadius, vCenter.z + fRadius );
		Vec3 sphereMin( vCenter.x - fRadius, vCenter.y - fRadius, vCenter.z - fRadius );

		if( sphereMax <= max &&
			sphereMin >= min )
		{
			return INTERSECTION_INSIDE;
		}
	}

	// Accumulate the distance of the sphere's center on each axis

	// If the sphere's center is outside the aabb, we've got distance on this axis
	// Otherwise the sphere's center is within the box on this axis, so the
	// distance will be 0 and we do not need to accumulate anything at all	
	if( vCenter.x < min.x )
	{
		float borderDistance = min.x - vCenter.x;
		totalDistance += borderDistance * borderDistance;
	}
	else if( vCenter.x > max.x )
	{
		float borderDistance = vCenter.x - max.x;
		totalDistance += borderDistance * borderDistance;
	}

	// Y axis
	if( vCenter.y < min.y )
	{
		float borderDistance = min.y - vCenter.y;
		totalDistance += borderDistance * borderDistance;
	}
	else if( vCenter.y > max.y )
	{
		float borderDistance = vCenter.y - max.y;
		totalDistance += borderDistance * borderDistance;
	}

	// Z axis
	if( vCenter.z < min.z )
	{
		float borderDistance = min.z - vCenter.z;
		totalDistance += borderDistance * borderDistance;
	}
	else if( vCenter.z > max.z )
	{
		float borderDistance = vCenter.z - max.z;
		totalDistance += borderDistance * borderDistance;
	}

	// If the distance to the box is lower than the sphere's radius, both are overlapping
	if( totalDistance <= (fRadius * fRadius) )
	{
		return INTERSECTION_PARTIAL;
	}
	else
	{
		return INTERSECTION_OUTSIDE;
	}
}