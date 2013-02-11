///============================================================================
/// \file		CTerrainManager.h
/// \brief		Header file for the Terrain
/// \date		03-24-2005
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

#ifndef	CTERRAIN_H
#define	CTERRAIN_H

#include "CManager.h"

class CTerrainManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CTerrainManager);
	/// \brief The Default Constructor
	/// \return void
	CTerrainManager();
public:
	/// \brief The Default Deconstructor
	/// \return void
	virtual ~CTerrainManager();

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent *Create(int nArgs, va_list argptr);

	virtual bool Init();
	virtual void Update(DWORD tickCount);

	TerrainMetrics_t GetMetrics();
	IEffect* GetDetailEffect( bool bLightmap );
	IEffect* GetDistantEffect();

	bool LightTerrain() { return m_bLightTerrain; }
	void SetSectorExistance( IHashString *pSectorName, int iSectorX, int iSectorY, bool bExists );
	bool TerrainDeform( BRUSHPARAMS &brush );
	bool TerrainGetBrushAreaInfo( TERRAINBRUSHAREAINFO &info );
	bool GetAffectedTerrainSectors( std::list<IHashString*> &sectorList, float fRadius, Vec3 pos );
	__forceinline UINT GetBaseTextureStage(){
		return m_BaseTextureStage;
	}
	__forceinline UINT GetMaskTextureStageStart(){
		return m_MaskTextureStageStart;
	}
	__forceinline UINT GetLightmapStage()
	{
		return m_LightmapTextureStage;
	}

	// ------------------ Messages ------------------ //
	DWORD OnTerrainDeform(DWORD size, void *in_params);
	DWORD OnGetAffectedTerrainSectors(DWORD size, void *in_params);
	DWORD OnSetTerrainConfig(DWORD size, void *in_params);
	DWORD OnGetLODDistance(DWORD size, void *in_params);
	DWORD OnCreateTerrain( DWORD size, void * in_params );
	DWORD OnModifyConfig( DWORD size, void * in_params );
	DWORD OnTerrainSectorRemoveHeightmap( DWORD size, void * in_params );
	DWORD OnGetTerrainDescriptor( DWORD size, void * in_params );
	DWORD OnGetTerrainSectorNeighborInfo( DWORD size, void * in_params );
	DWORD OnCreateTerrainDecal(DWORD size, void *param);

	///	\brief	Start the manager
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	///						error
	DWORD OnStart(DWORD size, void *data);

	///	\brief	Stop the manager
	///	\param	size = 0
	///	\param	data = void pointer to NULL
	///	\return DWROD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED,
	///						error
	DWORD OnStop(DWORD size, void *data);

	DWORD PumpHandler(DWORD size, void *data, IHashString *name, IHashString *compType, MSGFUNCTION msgFunction);

private:
	void SetConfig(CTerrainConfig *pConfig);
	/// FreeTerrain 
	void FreeTerrain();
	//blank terrain creation:
	bool CalculateTerrainAABSquare( int &XStart, int &YStart, int &XEnd, int &YEnd );
	void CreateTerrain( int width, int height, int sectorresolution, float sectorsize, float zscale, float LODDistance, 
									StdString &TerrainPath, StdString &TerrainName, StdString &TerrainBaseTexture );
	void BuildTerrainSectorRenderObject( StdString &ParentName, StdString &TerrainBaseTexture, int SectorX, int SectorY, float sectorsize );
	void BuildTerrainHeightmap( IHashString *pParentName, IArchive *MemArchive, StdString &TerrainPath, StdString &TerrainName, int iSectorX, int iSectorY, int iSectorResolution );
	CHashString *BuildTerrainSector( IHashString *pParentName, IArchive *MemArchive, StdString &TerrainPath, StdString &TerrainName, StdString &TerrainBaseTexture, int SectorX, int SectorY, float sectorsize );
	void BuildTerrainSectorPhysicsObject( StdString ParentName, int SectorX, int SectorY );
	void BuildTerrainSectorWaterObject( StdString ParentName, int SectorX, int SectorY, float sectorsize );
	void ModifyTerrainSectorRenderObject( StdString &ParentName, StdString &TerrainBaseTexture );
	IHashString* GetSectorName( int X, int Y );
	void StitchSectors( IHashString * toStitch, int sourcex, int sourcey,
									IHashString * withStitch, int destx, int desty );

	/// Returns a unique index for a terrain sector given the sector's x and y indices
	inline int GetTerrainSectorIndex( int x, int y )
	{
		return (((x+1024)<<16) | (y+1024));
	}

	bool m_bIsRunning;

	/// Map of unique sector index to sector
	typedef std::map<int, IHashString*> SECTORINDEXTONAMEMAP;
	typedef std::map<DWORD, int> SECTORNAMETOINDEXMAP;
	SECTORINDEXTONAMEMAP m_SectorIndexToNameMap;
	SECTORNAMETOINDEXMAP m_SectorNameToIndexMap;

	// properties read in

	/// resolution of a sector (grid resolution of squares, not points!)
	int m_iSectorResolution;
	/// Size of a sector in world space
	float m_fSectorSize;
	/// Terrain Z scale in world space
	float m_fHeightMapZScale;
	/// LOD Distance to switch between Splat and Single Texture
	float m_fLODDistance;

	/// Height map class string to check for inclusion into map
	CHashString m_HeightMapClass;
	/// Terrain Sector class string to check for inclusion into map
	CHashString m_TerrainSectorClass;
	CHashString m_TerrainSectorRenderObjectClass;
	CHashString m_TerrainDecalRenderObjectClass;

	CHashString m_ConfigName;

	//keeps track of base texture scale:
	float m_BaseTextureScaleX;
	float m_BaseTextureScaleY;

	/// Flag whether terrain is rendered with lighting or not.
	bool m_bLightTerrain;

	/// Prevent LOD update calculations every frame
	int m_iSectorLODUpdateTimer;

	CHashString	m_DistantEffect;
	IEffect *m_pDetailEffect;
	IEffect *m_pDetailEffectLightmap;
	IEffect *m_pDistantEffect;

	UINT m_BaseTextureStage;
	UINT m_MaskTextureStageStart;
	UINT m_LightmapTextureStage;

	lua_State* m_LuaState;
};

#endif
