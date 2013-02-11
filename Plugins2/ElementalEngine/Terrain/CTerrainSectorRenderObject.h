///============================================================================
/// \file		CTerrainSectorRenderObject.h
/// \brief		Header file for a Terrain Sector Render Object
/// \date		04-24-2005
/// \author		Brian Bazyk
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

#ifndef	CTERRAINSECTORRENDEROBJECT_H
#define	CTERRAINSECTORRENDEROBJECT_H

#include "CRenderObject.h"

class CTerrainManager;

//==========================================================================
// Terrain Vertex Format
//==========================================================================
struct TERRAINVERTEX
{
	Vec3 pos;				// position
	float masktex[2];       // set of texture coordinates
	float tiletex[2];       // set of texture coordinates
	Vec3 normal;
	Vec3 tangent;
	float lightmapuv[2];	// lightmap uv, may be different from masktex
};
//==========================================================================

/// Templated terrain buffer class for static buffers that need cleanup on exit.
template <typename BufferData_t>
class CTerrainBuffer
{
public:
	CTerrainBuffer()
	{
		m_iSize = 0;
		m_pData = NULL;
	}
	~CTerrainBuffer()
	{
		if (m_pData != NULL)
			delete[] m_pData;
	}

	int GetSize()
	{
		return m_iSize;
	}

	BufferData_t* GetData()
	{
		return m_pData;
	}

	BufferData_t* GetData( int iSize )
	{
		if (m_pData && (m_iSize == iSize))
			return m_pData;

		if (m_pData)
			delete[] m_pData;

		m_iSize = iSize;
		m_pData = new BufferData_t[iSize];
		return m_pData;
	}

private:
	int m_iSize;
	BufferData_t *m_pData;
};

/// Collection of static terrain buffers
class CTerrainSectorStaticBuffers
{
public:
	unsigned short* GetSectorBaseIndexData()
	{
		return m_BaseIndices.GetData();
	}
	unsigned short* GetSectorBaseIndexData( int iSamples )
	{
		return m_BaseIndices.GetData( iSamples );
	}

	float* GetTempSectorHeightBuffer( int iSamples )
	{
		return m_HeightBuffer.GetData( iSamples );
	}

	Vec3* GetTempSectorNormalBuffer( int iSamples )
	{
		return m_NormalBuffer.GetData( iSamples );
	}

	unsigned char* GetTempSectorByteTextureBuffer( int iBytes )
	{
		return m_ByteTextureBuffer.GetData( iBytes );
	}

	float* GetTempSectorFloatTextureBuffer( int iSamples )
	{
		return m_FloatTextureBuffer.GetData( iSamples );
	}

private:
	CTerrainBuffer<unsigned short> m_BaseIndices;
	CTerrainBuffer<float> m_HeightBuffer;
	CTerrainBuffer<Vec3> m_NormalBuffer;
	CTerrainBuffer<unsigned char> m_ByteTextureBuffer;
	CTerrainBuffer<float> m_FloatTextureBuffer;
};

class CTerrainSectorRenderObject  : public CRenderObject<>
{
protected:
	const float m_fCurrentVersion;
	IArchive* ConvertTerrainArchive( IArchive &ar );

	/// \brief Default Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CTerrainSectorRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier);

	class CTerrainLayer
	{
	public:
		/// Constructor
		CTerrainLayer();
		/// Must be called before removing layer from list.  Destructor does not call free
		/// because it would be called upon added the temp layer to a vector (by reference) 
		/// and then the temp layer goes out of scope.
		void Unload();
		///	\brief	set splat texture name (file path)
		///	\param	name - filepath to texture
		void SetSplatTextureName(LPCTSTR name);

		unsigned char *m_pMaskBuffer;
		int m_iMaskSize;
		CHashString m_szSplatTextureName;
		ITextureObject *m_pSplatTexture;

		float m_fSplatTextureScaleX;
		float m_fSplatTextureScaleY;
		bool m_bLoaded;
	};

public:
	typedef std::vector<CTerrainLayer> TERRAINLAYERLIST;

	/// \briefDefault Destructor
	~CTerrainSectorRenderObject();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief DeInit Function
	virtual void DeInit();

	/// \brief Init Function
	virtual void Init();

	/// \brief serialize
	/// \param ar - The Archive
	virtual void Serialize(IArchive &ar);

	virtual bool Update();

	virtual OBJECTSORTTYPES GetSceneSortID() const { return RENDEROBJECT3D; }

	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass, IEffect * override );

	/// \brief This is for render priorities between classes
	/// will change as soon as a new architecture can be designed to better incorporate
	/// different renderobject types being pushed on the same sorted list
	/// \return DWORD = the priority key this returns, the higher the number,
	/// the higher its priority
	virtual DWORD GetRenderPriority();

	/// \brief Number of passes this render object has to go through
	/// \return UINT = returns one pass for this implementation
	virtual UINT GetNumPass() { return 1; };

	/// \brief Sets the position of the render object
	/// \param position Vec3 class reference to fill in
	virtual void SetPosition( const Vec3 &vec );

	/// \brief Gets the position of the render object
	/// \return Vec3 = position f the object
	virtual Vec3 &GetPosition();

	/// \brief Gets bounding sphere of object. This is a 3D interface call that should be implemented for only
	///			3D objects. Might want to separate more of this later
	/// \param position = vector that will be filled the center of the sphere
	/// \param radius = float that will be filled with the radius of the sphere
	virtual void GetBoundingSphere( Vec3 &position, float &radius );

	virtual ISceneSettings* GetSceneSettings(){ return m_SceneSettings; }

	virtual UINT GetNumGeometry(){ return 0; }
	virtual IGeometry* GetGeometry( int index ) { return (index==0) ? m_pGeometry : NULL; }
	virtual UINT GetNumMaterial( ) { return 1; }
	virtual IMaterial* GetMaterial( int index ) { return (index==0) ? m_pMaterial : NULL; }

	// ** HEIGHTMAP FUNCTIONS ** //
	/// \brief Reconstruct all internal data given a new heightmap and it's size (samples along an edge)
	/// \param in_pHeightMap - The heightmap data
	/// \param in_iHeightMapSize - The size of the heightmap
	void ResampleDataFromHeightMap( float *in_pHeightMap, int in_iHeightMapSize );
	/// \brief Reconstruct vertices given height data and normal data
	/// \param pHeightData - the height data
	/// \param pNormalData - the normal data
	void SetVertexData( float *pHeightData );

	// ** MESSAGES ** //
	DWORD OnTerrainSectorSetData(DWORD size, void *in_params);
	DWORD OnTerrainSectorPaint(DWORD size, void *in_params);
	DWORD OnTerrainRemoveUnusedLayers(DWORD size, void *in_params);
	DWORD OnTerrainLayerInfo(DWORD size, void *in_params );

	DWORD OnTerrainSectorInsertLayer( DWORD size, void *in_params );
	DWORD OnTerrainSectorSwapLayers( DWORD size, void *in_params );
	DWORD OnTerrainSectorRemoveLayer( DWORD size, void *in_params );	

	DWORD OnTerrainSectorSetState( DWORD size, void *in_params );

	DWORD OnTerrainSectorSetBaseTexture( DWORD size, void *in_params );
	DWORD OnTerrainSectorGetBaseTexture( DWORD size, void *in_params );
	DWORD OnTerrainSectorGetExtents( DWORD size, void *in_params );

	DWORD OnGetTerrainSectorLOD( DWORD size, void *in_params );
	DWORD OnTerrainSectorUpdateNeighborLOD( DWORD size, void *in_params );
	DWORD OnTerrainSectorUpdateLOD( DWORD size, void *in_params );

	DWORD OnTerrainSectorGetPolyData( DWORD size, void *in_params );
	DWORD OnTerrainSectorFreePolyData( DWORD size, void *in_params );
	DWORD OnTerrainSectorModifyLayerMask( DWORD size, void *in_params );
	DWORD OnTerrainSectorLayerGetBrushAreaInfo( DWORD size, void *in_params );
	DWORD OnGetGeometryAllocations( DWORD size, void * params );
	DWORD OnSetLightmapData( DWORD size, void *params );
	DWORD OnGetLocalMatrixTransform( DWORD size, void * params );

	DWORD OnTerrainSetPath( DWORD size, void * params );

	virtual void AttachLight( ILightObject * light );
	virtual void DetachAllLights();

	/// Gives the min and max bounds of the terrain's geometry
	/// \param aabbMin - vec to store the min bounds
	/// \param aabbMax - vec to store the max bounds
	void GetBounds( Vec3 &aabbMin, Vec3 &aabbMax );

	void GenerateUVs( int iVertexResolution, float * outUVStream );

	void UpdateLayerMasksFromTextures();
	void UpdateTextureMasksFromLayers();

	BUFFERALLOCATIONSTRUCT GetVertexBufferAllocation() { return m_VertexBufferAllocation; }
	BUFFERALLOCATIONSTRUCT GetIndexBufferAllocation() { return m_IndexBufferAllocation; }

	IMaterial* CreateMaterial( const TCHAR *pszSuffix );

private:
	/// ???
	void UpdateBoundingData();

	///  render debug normals
	void RenderNormals();

	/// \brief ???
	bool LoadLayer( CTerrainLayer *pLayer );

	/// 
	void UnloadAllLayers();
	/// \brief ???
	/// \param pLayer - ???
	/// \return ???
	/// 
	void InsertLayer( IHashString *pSplatInsertLocation, IHashString *pSplatTextureName, float fSplatScaleX, float fSplatScaleY );
	/// \brief ???
	/// \param pSplatTextureName - ???
	/// \return On success the requested layer otherwise NULL.
	CTerrainLayer* FindTerrainLayer( IHashString *pSplatTextureName );
	///
	/// \return On success the requested layer iterator otherwise end of list.
	TERRAINLAYERLIST::iterator FindTerrainLayerItr( IHashString *pSplatTextureName );

	/// \brief Render terrain as fog color
	void DistantRender( UINT pass );
	/// \brief Render terrain with splatting
	void DetailRender( UINT pass );

	/// Generates normals for the sector without using neighbor information into a buffer
	void GenerateNormals( int iVertexResolution, const Vec3 *pVertices, Vec3 *pNormals );
	
	/// Generates normals for the sector without using neighbor information
	void GenerateNormals();
	/// Stitched normals for the sector using neighbor information
	void StitchNormals();

	__forceinline void GenerateTangent( TERRAINVERTEX *pTerrainVertices, unsigned short v0, unsigned short v1, unsigned short v2 );
	void GenerateTangents( TERRAINVERTEX *pTerrainVertices, int iVertexCount, unsigned short *pTerrainIndices, int iIndexCount );

	/// Generates terrain indices for all lods, only needs to happen once.
	void CreateLODIndices();

	/// Sets current lod
	/// \param iLOD - lod
	/// \param vLODEdges - flags of whether that edge needs an lod transition
	/// \param pIndexData - pointer to buffer to write indices to or NULL
	/// \return int - if pIndexData is NULL then return value is an upper bound on the number of indices required for that lod
	int WriteIndicesLOD( int iLOD, int vLODEdges[4], unsigned short *pIndexData );
	/// Sets current lod
	/// \param iLOD - lod level to set
	/// \param bNeighborLODChange - flag whether to check geometry due to a neighbor lod change
	void SetLOD( int iLOD, bool bNeighborLODChange, bool bNotifyNeighbors );
	/// Calculates the desired lod.
	/// \return int - current lod
	int CalculateLOD();
	/// Calculates the current lod distance to use for lod calculations.  Use this calculated value 
	/// instead of the terrain property's lod distance.
	float CalculateLODDistance();

	void CreateEffects();
	void CreateDefaultMaskTexture( int iMaskTextureSize );

	///	\brief	remove mask texture from texture manager and clear pointer to texture object
	void RemoveMaskTexture();

	// Toolbox
	IRenderer *m_pRenderer;
	CTerrainManager *m_pTerrainMgr;
	static CTerrainSectorStaticBuffers m_StaticBuffers;
	static int m_iBaseIndexCount;

	// Vars
	Vec3 m_Position;	
	Vec3 m_CenterPosition;
	float m_fBoundingSphereRadius;
	Vec3 m_BoundingMins;
	Vec3 m_BoundingMaxs;
	float m_fMinHeight;
	float m_fMaxHeight;

	int m_iSectorX;
	int m_iSectorY;

	ITextureObject *m_pBaseTexture;
	ITextureObject *m_pMaskTexture0;
	CHashString m_szBaseTexture;
	CHashString m_szMaskTexture0;
	float m_fBaseTextureScaleX;
	float m_fBaseTextureScaleY;
	TERRAINLAYERLIST m_Layers;
	// this list contains layers information from serialization archive
	// it is used during init stage
	TERRAINLAYERLIST m_ReadedLayers;

	bool AllocateVertices( int iVertexCount );
	bool AllocateIndices( int iIndexCount );
	BUFFERALLOCATIONSTRUCT m_VertexBufferAllocation;
	IVertexBufferObject *m_VertexBufferInterface;
	BUFFERALLOCATIONSTRUCT m_IndexBufferAllocation;
	IIndexBuffer *m_IndexBufferInterface;

	ITextureObject *m_LightmapTexture;

	bool m_bDirtyNormals;
	bool m_bDirtyNormalEdges;
	DWORD m_iNormalGenerationTimer;

	int m_iCurrentLOD;
	int m_vCurrentLODEdges[4];

	UINT m_CurrentLODIndexCount;

	// Internal Geometry Buffers
	Vec3 *m_pVertexData;
	int m_iVertexCount;

	Vec3 *m_pEdgeNormals[4];

	bool m_bAddToHierarchy;
	bool m_bGeneratedLightmap;

	ISceneSettings *m_SceneSettings;
	IGeometry *m_pGeometry;
	IMaterial *m_pMaterial;
	IGeometry *m_pDistantGeometry;
	IMaterial *m_pDistantMaterial;

	// Lighting
	ILightObject *m_Lights[MAX_LIGHTS];
	float m_LightPriorities[MAX_LIGHTS];
	int m_NumLights;
};

#endif
