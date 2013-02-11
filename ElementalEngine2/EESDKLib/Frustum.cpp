///============================================================================
/// \file	Frustum.cpp
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

#include "Stdafx.h"
#include "Frustum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool CFrustum::bitfieldgenerated = false;
int CFrustum::s_planebitfieldlookup[ 3 ][ 2 ][ 2 ][ 2 ];

void CFrustum::GenerateBitField()
{
	//these return a value between 0-5 to index into this array:
	//float bbox[ 6 ];
	//bbox[ 0 ] = aabbMin.x;
	//bbox[ 1 ] = aabbMin.y;
	//bbox[ 2 ] = aabbMin.z;
	//bbox[ 3 ] = aabbMax.x;
	//bbox[ 4 ] = aabbMax.y;
	//bbox[ 5 ] = aabbMax.z;
	//A = ( min.x, max.y, min.z ) = ( bbox[ 0 ], bbox[ 4 ], bbox[ 2 ] )
	//B = ( min.x, min.y, min.z ) = ( 0, 1, 2 )
	//C = ( max.x, max.y, min.z ) = ( 3, 4, 2 )
	//D = ( max.x, min.y, min.z ) = ( 3, 1, 2 )
	//E = ( min.x, max.y, max.z ) = ( 0, 4, 5 )
	//F = ( min.x, min.y, max.z ) = ( 0, 1, 5 )
	//G = ( max.x, max.y, max.z ) = ( 3, 4, 5 )
	//H = ( max.x, min.y, max.z ) = ( 3, 1, 5 )
	//for computing the N vertex, the vertex closest to the frustum based on the diagonal
	//of a bounding box that most closely aligned with the plane's normal
	//[ 1 ][ 1 ][ 1 ] = G;//negative direction
	//[ 0 ][ 1 ][ 1 ] = E;//x positive, yz negative
	//[ 0 ][ 0 ][ 1 ] = F;//xy positive, z negative
	//[ 0 ][ 1 ][ 0 ] = A;//xz positive, y negative
	//[ 1 ][ 0 ][ 1 ] = H;//y positive, xz negative
	//[ 1 ][ 0 ][ 0 ] = D;//yz positive, x negative
	//[ 1 ][ 1 ][ 0 ] = C;//z positive, xy negative
	//[ 0 ][ 0 ][ 0 ] = B;//positive direction
	
	//Column:			 axis    bitfield (000, 100, etc) for negative/positive axis directions for plane normals
	s_planebitfieldlookup[ 0 ]	[ 1 ][ 1 ][ 1 ] = 3;//negative direction
	s_planebitfieldlookup[ 1 ]	[ 1 ][ 1 ][ 1 ] = 4;
	s_planebitfieldlookup[ 2 ]	[ 1 ][ 1 ][ 1 ] = 5;
	
	s_planebitfieldlookup[ 0 ]	[ 0 ][ 1 ][ 1 ] = 0;//x positive, yz negative
	s_planebitfieldlookup[ 1 ]	[ 0 ][ 1 ][ 1 ] = 4;
	s_planebitfieldlookup[ 2 ]	[ 0 ][ 1 ][ 1 ] = 5;
	
	s_planebitfieldlookup[ 0 ]	[ 0 ][ 0 ][ 1 ] = 0;//xy positive, z negative
	s_planebitfieldlookup[ 1 ]	[ 0 ][ 0 ][ 1 ] = 1;
	s_planebitfieldlookup[ 2 ]	[ 0 ][ 0 ][ 1 ] = 5;
	
	s_planebitfieldlookup[ 0 ]	[ 0 ][ 1 ][ 0 ] = 0;//xz positive, y negative
	s_planebitfieldlookup[ 1 ]	[ 0 ][ 1 ][ 0 ] = 4;	
	s_planebitfieldlookup[ 2 ]	[ 0 ][ 1 ][ 0 ] = 2;	
	
	s_planebitfieldlookup[ 0 ]	[ 1 ][ 0 ][ 1 ] = 3;//y positive, xz negative
	s_planebitfieldlookup[ 1 ]	[ 1 ][ 0 ][ 1 ] = 1;
	s_planebitfieldlookup[ 2 ]	[ 1 ][ 0 ][ 1 ] = 5;
	
	s_planebitfieldlookup[ 0 ]	[ 1 ][ 0 ][ 0 ] = 3;//yz positive, x negative
	s_planebitfieldlookup[ 1 ]	[ 1 ][ 0 ][ 0 ] = 1;	
	s_planebitfieldlookup[ 2 ]	[ 1 ][ 0 ][ 0 ] = 2;	
	
	s_planebitfieldlookup[ 0 ]	[ 1 ][ 1 ][ 0 ] = 3;//z positive, xy negative
	s_planebitfieldlookup[ 1 ]	[ 1 ][ 1 ][ 0 ] = 4;
	s_planebitfieldlookup[ 2 ]	[ 1 ][ 1 ][ 0 ] = 2;
	
	s_planebitfieldlookup[ 0 ]	[ 0 ][ 0 ][ 0 ] = 0;//positive direction
	s_planebitfieldlookup[ 1 ]	[ 0 ][ 0 ][ 0 ] = 1;
	s_planebitfieldlookup[ 2 ]	[ 0 ][ 0 ][ 0 ] = 2;
	bitfieldgenerated = true;
}

// this should be moved to the math classes
// NEED NEW VEC CLASS
Vec3 rotateVectorAxisAngle( const Vec3 &vec, const Vec3 &axis, const float angle )
{
	Matrix3x3 mat;
	mat.SetIdentity();
	mat.SetFromAxisAngle( axis, angle );
	Vec3 transformed;
	//need to check the matrix type here carefully
	transformed.x = mat.m[0]*vec.x + mat.m[1]*vec.y + mat.m[2]*vec.z;
	transformed.y = mat.m[3]*vec.x + mat.m[4]*vec.y + mat.m[5]*vec.z;
	transformed.z = mat.m[6]*vec.x + mat.m[7]*vec.y + mat.m[8]*vec.z;
	return transformed;	
}

/// \brief Builds a frustum from the parameters
//there might be a more efficient way to build this
void CFrustum::Build( const Vec3 &origin, const Vec3 &dir, const Vec3 &upvector,
					 float horizontalfov, float verticalfov, float nearcap, float farcap )
{
	if( !bitfieldgenerated )
	{
		GenerateBitField();
	}
	float halfanglevert = verticalfov*0.5f;
	float halfanglehorizon = horizontalfov*0.5f;
	// Need to fix this, having Vec3's can bloat code
	Vec3 direction( dir.x, dir.y, dir.z );
	Vec3 upvect( upvector.x, upvector.y, upvector.z );
	Vec3 orig( origin.x, origin.y, origin.z );
	direction.Normalize();
	//first plane, near cap, second plane: farcap
	Vec3 nearplanePos =  orig + direction*nearcap;
	Vec3 farplanePos =  orig + direction*farcap;
	m_ClipPlanes[0].GeneratePlane( direction, nearplanePos );
	m_ClipPlanes[1].GeneratePlane( -direction, farplanePos );
	m_FrustumTranslation = orig;
	// ^(upvector)
	// |
	// |----------------------------->(farclip)
	//  \(a)     |     /  a + b  = 90
	//     \     |    /   upvector rotated by is the normal to a clip plane
	//       \(b)|(a)/    

	//construct pyramid based on directions
	Vec3 binormal = direction.Cross( upvect );
	//Build the normals of each plane. Most efficient way I could come up with 
	Vec3 normalPlane1 = rotateVectorAxisAngle( -upvect, binormal, halfanglevert );//top
	Vec3 normalPlane2 = rotateVectorAxisAngle( upvect, binormal, -halfanglevert );//bottom
	Vec3 normalPlane3 = rotateVectorAxisAngle( binormal, upvect, halfanglehorizon );//left
	Vec3 normalPlane4 = rotateVectorAxisAngle( -binormal, upvect, -halfanglehorizon );//right

	m_ClipPlanes[2].GeneratePlane( normalPlane1,  orig );
	m_ClipPlanes[3].GeneratePlane( normalPlane2,  orig );
	m_ClipPlanes[4].GeneratePlane( normalPlane3,  orig );
	m_ClipPlanes[5].GeneratePlane( normalPlane4,  orig );

	//Generate AABB fast testing Bit fields
	for( int i = 0; i < NUMFRUSTUMPLANES; i++ )
	{
		Vec3 planenormal = m_ClipPlanes[ i ].GetNormal();
		int signx = planenormal.x < 0 ? 0 : 1;
		int signy = planenormal.y < 0 ? 0 : 1;
		int signz = planenormal.z < 0 ? 0 : 1;
		m_AABBNVertexLookup[ 0 ][ i ] = s_planebitfieldlookup[ 0 ][ signx ][ signy ][ signz ];
		m_AABBNVertexLookup[ 1 ][ i ] = s_planebitfieldlookup[ 1 ][ signx ][ signy ][ signz ];
		m_AABBNVertexLookup[ 2 ][ i ] = s_planebitfieldlookup[ 2 ][ signx ][ signy ][ signz ];
	}
}

/// \brief Checks if a sphere lies within the frustum
bool CFrustum::IsSphereInView( const Vec3 &position, const float radius, int &iPlaneBits )
{
	iPlaneBits = DEFAULTPLANEBITS;
	for( int i = 0; i < NUMFRUSTUMPLANES; i++ )
	{
		PLANECOLLISION collision = m_ClipPlanes[ i ].SphereCollision( position,radius );
		iPlaneBits |= (collision<<(i<<1));
		if( collision == PLANECOLLISION_NONNORMALSIDE )
			return false;
	}
	return true;
}

/// \brief Checks if an axis aligned bounding box is visible within the frustum
INTERSECTION_RESULT CFrustum::IsAABBInView( const Vec3 &aabbMin, const Vec3 &aabbMax, int iPlaneBits )
{
	Vec3 vVertices[8];
	vVertices[0].Set( aabbMin.x, aabbMin.y, aabbMin.z );
	vVertices[1].Set( aabbMin.x, aabbMin.y, aabbMax.z );
	vVertices[2].Set( aabbMin.x, aabbMax.y, aabbMin.z );
	vVertices[3].Set( aabbMin.x, aabbMax.y, aabbMax.z );
	vVertices[4].Set( aabbMax.x, aabbMin.y, aabbMin.z );
	vVertices[5].Set( aabbMax.x, aabbMin.y, aabbMax.z );
	vVertices[6].Set( aabbMax.x, aabbMax.y, aabbMin.z );
	vVertices[7].Set( aabbMax.x, aabbMax.y, aabbMax.z );
	//Min = Min - m_FrustumTranslation;
	//Max = Max - m_FrustumTranslation;
	// result = inside; // TODO: support three returned states not just two; inside, outside, and intersection
	for (int i=0; i < NUMFRUSTUMPLANES; i++) 
	{
		// early out tests from prior plane tests
		int collisionBits = ((iPlaneBits >> (i<<1)) & 0x03); // get every two bits
		if (collisionBits == PLANECOLLISION_NORMALSIDE)
			continue; // on this side of the plane, skip to next plane

		int out, in;
		out = 0;
		in = 0;
		for (int k=0; k < 8 && (in==0 || out==0); k++)
		{
			if( m_ClipPlanes[i].PointCollision( vVertices[k] ) == PLANECOLLISION_NONNORMALSIDE )
				out++;
			else
				in++;
		}
		if (!in) // if all corners are out
			return INTERSECTION_OUTSIDE;
		else if (out) // if some corners are out and others are in
			return INTERSECTION_PARTIAL;
	}
	// all corners are in
	return INTERSECTION_INSIDE;
}