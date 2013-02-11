///============================================================================
/// \file	Frustum.h
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

#ifndef _CFRUSTUM_H_
#define _CFRUSTUM_H_

namespace ElementalEngine
{

#define NUMFRUSTUMPLANES 6

//Class Frustum
class CFrustum
{
public:
	
	/// \brief Builds a frustum from the parameters
	/// \param origin = the origin of the viewpoint
	/// \param direction = the viewvector
	/// \param upvector =the upvector of the frustum
	/// \param angle = the Field of View angle
	/// \param nearcap = the near clipping plane distance from the origin
	/// \param farcap = the far clipping plane distance from the origin
	void Build( const Vec3 &origin, const Vec3 &direction, const Vec3 &upvector,
					float horizontalfov , float verticalfov, float nearcap, float farcap );

	/// \brief Checks if a sphere lies within the frustum
	/// \param position = the position of the center of the sphere in world space
	/// \param radius = the radius of the sphere
	/// \return bool = returns true if the sphere is visible
	inline bool IsSphereInView( const Vec3 &position, const float radius )
	{
		int iPlaneBits;
		return IsSphereInView( position, radius, iPlaneBits );
	}
	/// \param iPlaneBits = bits corresponding to the plane tests where a 1 bit means on the normal side of the plane
	bool IsSphereInView( const Vec3 &position, const float radius, int &iPlaneBits );

	/// \brief Checks if an axis aligned bounding box is visible within the frustum
	/// \param aabbMin = the min coord of the AABB
	/// \param aabbMax = the  max coord of the AABB
	/// \param iPlaneBits = bits corresponding to previous plane tests where a 1 bit means on the normal side of the plane.  Tests all planes by default
	/// \return bool = returns true if the AABB is visible
	INTERSECTION_RESULT IsAABBInView( const Vec3 &aabbMin, const Vec3 &aabbMax, int iPlaneBits=DEFAULTPLANEBITS );

	Vec3 GetOrigin() const { return m_FrustumTranslation; }

private:
	//clip planes
	Plane	m_ClipPlanes[ NUMFRUSTUMPLANES ];
	Vec3	m_FrustumTranslation;
	//2 because bit can be on or off
	int		m_AABBNVertexLookup[ 3 ][ NUMFRUSTUMPLANES ];
	static bool bitfieldgenerated; 
	static int s_planebitfieldlookup[ 3 ][ 2 ][ 2 ][ 2 ];
	static void GenerateBitField();
};

}//namespace ElementalEngine

#endif
