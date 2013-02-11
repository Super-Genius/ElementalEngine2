///==========================================================================
/// \file	DefaultPlaneImplementation.cpp
/// \brief	
/// \date	08/16/2006
/// \author	Kyle Swiam
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
#include "DefaultPlaneImpl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IPlaneImplementation *CDefaultPlaneImplementation::s_instance = NULL;

void CDefaultPlaneImplementation::Init( void )
{
}

void CDefaultPlaneImplementation::CleanUp( void )
{
}

void CDefaultPlaneImplementation::GeneratePlane( Plane& plane, const Vec3 &PointA, const Vec3 &PointB, const Vec3 &PointC )
{
	//cross product the two vectors to get a normal
	Vec3 v1 = PointA - PointB;
	Vec3 v2 = PointA - PointC;		
	Vec3 normal = v1.Cross( v2 );
    plane.GeneratePlane( normal, PointA );
}

void CDefaultPlaneImplementation::GeneratePlane( Plane& plane, const Vec3 &normal, const Vec3 &point )
{
	//aX + bY + cZ + D = 0
	//should normalize normals first here
	Vec3 norm( normal.x, normal.y, normal.z );
	norm.Normalize();
	plane.pConstA = norm.x;
	plane.pConstB = norm.y;
	plane.pConstC = norm.z;
	plane.pConstD = -(plane.pConstA*point.x) - (plane.pConstB*point.y) - (plane.pConstC*point.z);
}

PLANECOLLISION CDefaultPlaneImplementation::SphereCollision( Plane& plane, const Vec3 &position, const float radius )
{
	//take dot product of normal with sphere position to get distance of sphere
	float distance = position.x*plane.pConstA + position.y*plane.pConstB + position.z*plane.pConstC + plane.pConstD;
	if (distance > radius)
	{
		return PLANECOLLISION_NORMALSIDE;
	}
	else if( distance < -radius )
	{
		return PLANECOLLISION_NONNORMALSIDE;
	}
	else
	{
		return PLANECOLLISION_INTERSECT;
	}
}

PLANECOLLISION CDefaultPlaneImplementation::PointCollision( Plane& plane, const Vec3 &position )
{
	//take dot product of normal with sphere position to get distance of sphere
	float distance = position.x*plane.pConstA + position.y*plane.pConstB + position.z*plane.pConstC + plane.pConstD;
	if( distance < 0 )
	{
		return PLANECOLLISION_NONNORMALSIDE;
	}
	else if( distance > 0 )
	{
		return PLANECOLLISION_NORMALSIDE;
	}
	else
	{
		return PLANECOLLISION_INTERSECT;
	}
}
