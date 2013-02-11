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

#ifndef MESHPARAM_H
#define MESHPARAM_H

#include "Matrix4x4.h"


namespace ElementalEngine
{

	//This class handles Meshes
class MeshParameterization : public CObjectTemplate< IObject>
{
private:
	/// \brief The Default Constructor for LightMapObject
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	MeshParameterization(IHashString *parentName, IHashString *name);

public:
	/// \brief	Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	The Initialization for the Object
	/// \param	IManager* pManager: Parent Manager
	virtual void Init();
	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();
	/// \brief	The DeInit Function for the Object
	virtual void DeInit();
	
	/// \brief either reads or writes the entity information to a file
	/// Currently Loads up the entity completely if reading
	virtual void Serialize(IArchive &ar);

	virtual bool IsKindOf( IHashString * compType );

	~MeshParameterization();
	
	DWORD OnGenerateAtlas( DWORD size, void * params );	
	DWORD OnAddIndexSoup( DWORD size, void * params );
	DWORD OnAddVertexIndexSoup( DWORD size, void * params );
	DWORD SetMeshTransform( DWORD size, void * params );
	DWORD OnSetTextureSize( DWORD size, void * params );
	DWORD GetCache( DWORD size, void * params );
	DWORD SetCache( DWORD size, void * params );
	DWORD OnGetTextureSize( DWORD size, void * params )
	{
		VERIFY_MESSAGE_SIZE( size, sizeof( DWORD )) ;
		DWORD * p = (DWORD*)params;
		if( p )
		{
			*p = m_TexSize;
			return MSG_HANDLED_STOP;
		}
		return MSG_ERROR;
	}
	DWORD OnGetCollapsedMesh( DWORD size, void * params );
	DWORD OnGetTriangleFaces( DWORD size, void * params );
	DWORD OnGetMeshTransform( DWORD size, void * params );
	DWORD OnGetMeshInverseTransform( DWORD size, void * params );
	DWORD OnGetAABB( DWORD size, void * params );
	DWORD OnGenerateBounds( DWORD size, void * params );
	DWORD OnIntersectRayTriangle( DWORD size, void * params );
	DWORD OnGenerateTriangleTexelData( DWORD size, void * params );
	
	//needs to be called if not cached or copied
	DWORD OnSetNoClone( DWORD size, void * params )
	{
		SetNoClone();
		return MSG_HANDLED_STOP;
	}
	DWORD OnGetBaseColorAtTriangleIntersection(  DWORD size, void * params );
public:
	int	GetTextureSize(){
		return m_TexSize;
	}
	void AddVertexSoup( RENDERBUFFERALLOCATION * renderbuffer , bool onlyIndices = false );
	//TEMPORARY
	floatColor * GetTextureBuffer()
	{
		if( !m_TextureBuffer )
		{
			m_TextureBuffer = new floatColor[m_TexSize*m_TexSize];
			for( int i = 0; i < m_TexSize*m_TexSize; i++ )
			{
				m_TextureBuffer[ i ].a = -1; //unlit
			}
		}
		return m_TextureBuffer;
	}
	//An indirect buffer will allow us to sample less while maintaining consistancy
	floatColor * GetIndirectLightBuffer()
	{
		if( !m_IndirectLightBuffer )
		{
			m_IndirectLightBuffer = new floatColor[m_TexSize*m_TexSize];
			for( int i = 0; i < m_TexSize*m_TexSize; i++ )
			{
				m_IndirectLightBuffer[ i ].a = -1; //unlit
			}
		}
		return m_IndirectLightBuffer;
	}
	vector< ParameterizationVertex > *  m_CollapsedMesh;
	vector< TriangleFace > * m_Faces;
	Matrix4x4  * m_Transform;
	Matrix4x4 * m_InverseTransform;
	AABB			m_BoundingBox;
	void GenerateBounds();
	bool IntersectRayTriangle( Ray &ray, int &triIndex, double * t, double * u, double * v );
	
	//Generates the pixels per triangle for later writing of pixels, one triangle at a time
	void GenerateTriangleTexelData();
	//needs to be called if not cached or copied
	void SetNoClone()
	{
		if( !m_Faces )m_Faces = new vector< TriangleFace >;
		if( !m_CollapsedMesh )m_CollapsedMesh = new vector< ParameterizationVertex >;
	}
	void GetBaseColorAtTriangleIntersection( int& face, float &u, float &v, floatColor &color );
private:
	void SetDevice( void * unknownDevice );
	void GenerateCache( LPD3DXMESH mesh, LPD3DXBUFFER remapped );
	void GenerateCache();
	void GenerateKDTree();

	floatColor	* m_TextureBuffer;
	floatColor	* m_IndirectLightBuffer;
	LightmapCache  m_Cache;
	int	m_TexSize;
	set< ITextureObject* > m_MaterialTextures;

	KDTree * m_KDTree;
	Vec3 * m_KDvertexData;
	Vec3 tmax;
	Vec3 tmin;
	LPDIRECT3DDEVICE9 m_pDevice;
public:
	static float TEXTURE_TO_WORLD_SPACE_RATIO; //approx 10 worldspace units boils to 1 texture unit
	static int MIN_TEXTURE_SIZE;
	static int MAX_TEXTURE_SIZE;
};

} //namespace ee
#endif