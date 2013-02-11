///============================================================================
/// \file	AABB.h
/// \brief  Axis Aligned Bounding Box
/// \date   Date
/// \author Name
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

#ifndef AABB_H
#define AABB_H

#include "IAABBImplementation.h"

namespace ElementalEngine
{

int SetAABBImplementation();

class AABB 
{	
private:
	static IAABBImplementation * impl;
	friend int SetAABBImplementation();

public:	
	Vec3 m_HalfSize;
	Vec3 m_Bounds[2];
	Vec3 m_Center;

	AABB(){};

	AABB( const Vec3 &vMin, const Vec3 &vMax )
	{
		Set( vMin, vMax );
	}

	inline void Set( const Vec3 &vMin, const Vec3 &vMax )
	{
		m_Bounds[0] = vMin;
		m_Bounds[1] = vMax;
		Vec3 diff = vMax - vMin;
		m_HalfSize = diff;
		float length = diff.Length();
		diff.Normalize();
		m_Center = vMin + diff*(length*.5f);
		m_HalfSize.x = fabs( m_HalfSize.x )/2.f;
		m_HalfSize.y = fabs( m_HalfSize.y )/2.f;
		m_HalfSize.z = fabs( m_HalfSize.z )/2.f;
	}
	
	inline void Get( Vec3 &vMin, Vec3 &vMax )
	{
		vMin.Set(m_Center - m_HalfSize);
		vMax.Set(m_Center + m_HalfSize);
	}

	inline bool IntersectRay( const Ray &r, float &t0, float &t1 )  
	{
		return impl->IntersectRay( *this, r, t0, t1 );		
	}

	inline bool IntersectTri( Vec3& a_V0, Vec3& a_V1, Vec3& a_V2 ) 
	{
		return impl->IntersectTri( *this, a_V0, a_V1, a_V2 );		
	}

	inline INTERSECTION_RESULT IntersectAABB( const Vec3 &vMin, const Vec3 &vMax )
	{
		return impl->IntersectAABB( *this, vMin, vMax );
	}
	
	inline INTERSECTION_RESULT IntersectSphere( const Vec3 &vCenter, const float &fRadius )
	{
		return impl->IntersectSphere( *this, vCenter, fRadius );
	}
};

} //namespace ElementalEngine

#ifndef EE_SDK_EXPORTS
#pragma comment(lib, "EE-SDK.lib")
#endif

#endif // AABB_H