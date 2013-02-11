///==========================================================================
/// \file	DefaultRayImplementation.cpp
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
#include "DefaultRayImplementation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPSILON2 0.000001
//make sure the cross is in the right direction
#define CROSS(dest,v1,v2) \
          dest.x=v1.y*v2.z-v1.z*v2.y; \
          dest.y=v1.z*v2.x-v1.x*v2.z; \
          dest.z=v1.x*v2.y-v1.y*v2.x;

IRayImplementation *CDefaultRayImplementation::s_instance = NULL;

void CDefaultRayImplementation::Init( void )
{
}

void CDefaultRayImplementation::CleanUp( void )
{
}

bool CDefaultRayImplementation::IntersectTriangle( Ray &ray, Vec3 &vert0, Vec3 &vert1, Vec3 &vert2,
								double *t, double *u, double *v)
{
	Vec3 edge1, edge2, tvec, pvec, qvec;
	float det,inv_det;

	// find vectors for two edges sharing vert0
	edge1 = vert1 - vert0;
	edge2 = vert2 - vert0;

	// begin calculating determinant - also used to calculate U parameter

	CROSS(pvec, ray.m_Direction, edge2);

	// if determinant is near zero, ray lies in plane of triangle
	det = edge1.Dot( pvec );

	// calculate distance from vert0 to ray origin 
	tvec = ray.m_Origin - vert0;
	inv_det = 1.0f / det;
		
	CROSS(qvec, tvec, edge1);
		    
	if (det > EPSILON2)
	{
		*u = tvec.Dot( pvec );
		if (*u < 0.0 || *u > det)
		return 0;
		        
		// calculate V parameter and test bounds 
		*v = ray.m_Direction.Dot( qvec );
		if (*v < 0.0 || *u + *v > det)
		return false;
		    
	}
	else if(det < -EPSILON2)
	{
		// calculate U parameter and test bounds 
		*u = tvec.Dot( pvec );
		if (*u > 0.0 || *u < det)
		return 0;
		    
		// calculate V parameter and test bounds 
		*v = ray.m_Direction.Dot( qvec ) ;
		if (*v > 0.0 || *u + *v < det)
		return false;
	}
	else return false;  // ray is parallell to the plane of the triangle 

	*t = edge2.Dot(qvec) * inv_det;
	(*u) *= inv_det;
	(*v) *= inv_det;

	return true;
}