///============================================================================
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

#ifndef LIGHTMAPDEFINE_H
#define LIGHTMAPDEFINE_H

namespace ElementalEngine
{
class MeshParameterization;
class LightMapGenerator;
class Ray;

struct MESHTEXTUREPAIRMSG
{
	IHashString * meshname;
	MeshParameterization * mesh;
	ITextureObject * texture;
};

struct GENERATEATLASMESSAGE
{
	bool useIncomingTexCoords;
	GENERATEATLASMESSAGE()
	{
		useIncomingTexCoords = false;
	}
};

struct ADDLIGHTMAPPABLEOBJECT
{
	IHashString * name;
	IHashString * type;
	int		useTextureCoordIndex;
	bool	bUseIncomingTexCoord;
	ADDLIGHTMAPPABLEOBJECT()
	{
		name = NULL;
		type = NULL;
		useTextureCoordIndex = 0;
		bUseIncomingTexCoord = false;
	}
};

struct CREATEMESHPARAMETERIZATION
{
	IHashString * name;
};
struct floatColor
{
	float a;
	float r;
	float g;
	float b;
	floatColor()
	{
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}
};

struct ParameterizationVertex
{
	Vec3  originalPosition;
	Vec3  normal;
	float originalU;
	float originalV;
	float generatedU;
	float generatedV;
};

struct TriangleTextureMapping
{
	//texture pixels
	int u;
	int v;
	//bary centric coordinates
	Vec3 localSpaceCoord;
	Vec3 localNormal;
};

struct OriginalVertex
{
	int	  vertexLink;
};

struct VertexSet
{
	vector< OriginalVertex > vertices;
};

struct TriangleFace
{
	//vertex indices
	unsigned short index[3];
	//actual uv pixel it maps to
	vector< TriangleTextureMapping > m_Pixels;
	DWORD m_Texture;
};


struct LightmapCache
{
	float * m_UVData;
	DWORD * m_RemappedData;
	DWORD * m_RemappedIndices;
	DWORD	m_Hash;
	int		m_NumVertices;
	int		m_NumIndices;
	bool	m_bRemapped;
	void ResizeUVData( int size )
	{
		if( m_UVData )
		{
			delete [] m_UVData;
		}
		m_UVData = new float[ size ];
	}
	void ResizeRemappedData( int size )
	{
		if( m_RemappedData )
		{
			delete [] m_RemappedData;
		}
		m_RemappedData = new DWORD[ size ];
	}
	void ResizeRemappedIndices( int size )
	{
		if( m_RemappedIndices )
		{
			delete [] m_RemappedIndices;
		}
		m_RemappedIndices = new DWORD[ size ];
	}

	bool IsBadCache()
	{
		return ( m_NumVertices <= 0 ||
					m_UVData == 0 ||
					( m_bRemapped && (m_RemappedData == 0 || m_RemappedIndices == 0 || m_NumIndices <= 0) ) );
	}
	LightmapCache()
	{
		m_Hash = 0;
		m_UVData = 0;
		m_RemappedData = 0;
		m_RemappedIndices = 0;
		m_NumVertices = 0;
		m_NumIndices = 0;
		m_bRemapped = false;
	}
	void CloneTo( LightmapCache &out, bool shared = true )
	{
		out.Clean();
		out.m_Hash = m_Hash;
		out.m_NumIndices = m_NumIndices;
		out.m_NumVertices = m_NumVertices;
		out.m_bRemapped = m_bRemapped;
		if( shared )
		{
			out.m_UVData = m_UVData;
			out.m_RemappedIndices = m_RemappedIndices;
			out.m_RemappedData = m_RemappedData;
		}else
		{
			out.ResizeUVData( m_NumVertices*2 );
			out.ResizeRemappedIndices( m_NumIndices );
			out.ResizeRemappedData( m_NumVertices );
			memcpy( out.m_UVData, m_UVData, sizeof( float )*m_NumVertices*2 );
			memcpy( out.m_RemappedIndices, m_RemappedIndices, sizeof( DWORD )*m_NumIndices );
			memcpy( out.m_RemappedData, m_RemappedData, sizeof( DWORD )*m_NumVertices );
		}
	}
	void Clean()
	{
		if( m_UVData ){ delete[] m_UVData; }
		if( m_RemappedData ){ delete[] m_RemappedData; }
		if( m_RemappedIndices ){ delete[] m_RemappedIndices; }
		m_Hash = 0;
		m_UVData = 0;
		m_RemappedData = 0;
		m_RemappedIndices = 0;
		m_NumVertices = 0;
		m_NumIndices = 0;
		m_bRemapped = false;
	}
};

struct ResizeCacheMessage
{
	int numUVData;
	int numRemappedData;
	int numRemappedIndices;
	LightmapCache * cache;
	ResizeCacheMessage()
	{
		cache = NULL;
		numUVData = 0;
		numRemappedData = 0;
		numRemappedIndices = 0;
	}

};

struct SETLIGHTMAPDATAMSG
{
	LightmapCache * cache;
	IBaseTextureObject * pTexture;
};

struct GETPARAMETERIZEDVERTICESMSG
{
	vector< ParameterizationVertex > * outList;
	GETPARAMETERIZEDVERTICESMSG()
	{
		outList = NULL;
	}
};

struct GETPARAMETERIZEDTRIANGLESMSG
{
	vector< TriangleFace > * outList;
	GETPARAMETERIZEDTRIANGLESMSG()
	{
		outList = NULL;
	}
};

struct MESHPARAMINTERSECTRAYTRIANGLEMSG
{
	Ray * inRay;
	int outFaceIndex;
	double outIntersectionDistance;
	double outULength;//length from v0-v1 to barycentric
	double outVLength;//length from v0-v2 to barycentric
	bool outCollided;
	MESHPARAMINTERSECTRAYTRIANGLEMSG()
	{
		inRay = NULL;
		outCollided = false;
	};
};

struct MESHPARAMGETBASECOLORATTRIANGLEINTERSECTIONMSG
{
	int inFace;
	float inUBaryCentric;
	float inVBaryCentric;
	floatColor outColor;
};

struct GENERATORPARAMSMSG
{
	int NUMPHOTONS;
	int MAX_BOUNCE_TEST;
	float PHOTON_DIRECT_RANGE_MAX;
	float PHOTON_INDIRECT_RANGE_MAX;
	float PHOTON_WEIGHT_RANGE;
	float PHOTON_RANGE_EXTENSION;
	int PHOTON_SAMPLES;
	float PHOTON_WEIGHT_RANGE_MAX;
	float SHADOW_EPSILON;
	int   SPECULAR_CHANCE;
	float PHOTON_MAX_DISK_ITERATION;
	float TRIANGLE_NORMAL_EPSILON;	//epsilon for averaged normals, making sure we're still lighting if a vertex is somehow
																		//different from the averaged normal in a way that makes it lit when the average normal isn't lit
	float ENERGY_CUTOFF	;
	int NUMBOUNCES;
	float NORMAL_CUTOFF; //anything with dissimilar normals does not get affected, should be > 90 degrees
	float DOT_SHADOW_ANGLE_CUTOFF;
	float energy_loss;
	bool DIRECT_LIGHT_ONLY;
	float TEXTURE_TO_WORLD_SPACE_RATIO; //approx 10 worldspace units boils to 1 texture unit
	int MIN_TEXTURE_SIZE;
	int MAX_TEXTURE_SIZE;
	GENERATORPARAMSMSG()
	{
		TEXTURE_TO_WORLD_SPACE_RATIO = 7.f;
		MIN_TEXTURE_SIZE = 64;
		MAX_TEXTURE_SIZE = 128;
		NUMPHOTONS					=		3000;
		MAX_BOUNCE_TEST			=		300;
		PHOTON_DIRECT_RANGE_MAX	=		30.f;
		PHOTON_INDIRECT_RANGE_MAX	=		50.f;
		PHOTON_WEIGHT_RANGE		=		30.f;
		PHOTON_RANGE_EXTENSION		=		20.f;

		PHOTON_SAMPLES				=		10;
		PHOTON_WEIGHT_RANGE_MAX	=		PHOTON_INDIRECT_RANGE_MAX;
		SHADOW_EPSILON				=		40.1f;
		SPECULAR_CHANCE			=		40;
		PHOTON_MAX_DISK_ITERATION	=		3;
		TRIANGLE_NORMAL_EPSILON	=		-.1f;	//epsilon for averaged normals, making sure we're still lighting if a vertex is somehow
																		//different from the averaged normal in a way that makes it lit when the average normal isn't lit
		ENERGY_CUTOFF				=		.1f;
		NUMBOUNCES					=		6;
		NORMAL_CUTOFF				=		.86f; //anything with dissimilar normals does not get affected, should be > 90 degrees
		DOT_SHADOW_ANGLE_CUTOFF	=		0.4f;
		DIRECT_LIGHT_ONLY		= false;
		energy_loss				=		1.0f;
	}
};

} //namespace EE
#endif