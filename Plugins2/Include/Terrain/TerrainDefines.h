///============================================================================
/// \file		TerrainDefines.h
/// \brief		Header file for Terrain Definitions
/// \date		10-04-2006
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

#ifndef	TERRAINDEFINES_H
#define	TERRAINDEFINES_H

enum TERRAIN_LAYERMASK_GENERATION
{
	TERRAIN_LAYERMASK_GEN_HEIGHT,
	TERRAIN_LAYERMASK_GEN_SLOPE,
};

struct TERRAINSECTORMODIFYLAYERMASK
{
	int iLayer;
	int iLayerMaskGenMode;
	float fMin;
	float fMax;
	bool bAccumMask;
};

struct TERRAINSECTORGETPOLYDATA
{
	TERRAINSECTORGETPOLYDATA()
	{
		in_bGenerateNormals = false;
		in_bGenerateUVs = false;
		in_iGenerationResolution = 0; // zero for the default resolution of the terrain sector
		pVertexData = NULL;
		iVertexCount= 0;
		pIndexData = NULL;
		iIndexCount = 0;
		pEdgeNormals[0] = NULL;
		pEdgeNormals[1] = NULL;
		pEdgeNormals[2] = NULL;
		pEdgeNormals[3] = NULL;
		pNormalData = NULL;
		pUVStream = NULL;

		vertexInterface = NULL;
		indexInterface = NULL;
	}

	bool in_bGenerateNormals;
	bool in_bGenerateUVs;
	int  in_iGenerationResolution;

	Vec3 *pVertexData;
	int iVertexCount;
	void *pIndexData;
	int iIndexCount;
	Vec3 *pEdgeNormals[4];
	Vec3 *pNormalData;
	float * pUVStream;

	BUFFERALLOCATIONSTRUCT vertexAllocation;
	IVertexBufferObject *vertexInterface;
	BUFFERALLOCATIONSTRUCT indexAllocation;
	IIndexBuffer *indexInterface;
};

struct TERRAINSECTORGETHEIGHTFIELDDATA
{
	int iHeightfieldHeight;
	int iHeightfieldWidth;
	unsigned int* pHeightfieldSamples;
};

struct TERRAINSECTOREXTENTSPARAMS
{
	Vec3 *pMins;
	Vec3 *pMaxs;
};

struct TERRAINDESCRIPTOR
{
	int TerrainWidth;
	int TerrainHeight;
	float BaseTextureScaleX;
	float BaseTextureScaleY;
	int SectorResolution;
	float SectorSize;
	float ZScale;
	StdString TerrainName;
	StdString TerrainBaseTexture;
	TERRAINDESCRIPTOR():
		TerrainWidth(0),
		TerrainHeight(0),
		SectorSize(0)
		{}
};

struct TERRAINCREATIONPARAMS
{
	int TerrainWidth;
	int TerrainHeight;
	bool PositiveResizeX;
	bool PositiveResizeY;
	bool NegativeResizeX;
	bool NegativeResizeY;
	float BaseTextureScaleX;
	float BaseTextureScaleY;
	int SectorResolution;
	float SectorSize;
	float ZScale;
	float LODDistance;
	StdString TerrainPath;
	StdString TerrainName;
	StdString TerrainBaseTexture;
	TERRAINCREATIONPARAMS():
		TerrainWidth(0),
		TerrainHeight(0),
		SectorSize(0)
		{}
};

struct TERRAINSECTORPOSITIONPARAMS
{
	int SectorX;             // index of sector in terrain grid
	int SectorY;             // index of sector in terrain grid
	Vec3 SectorCorner; // corner position of sector in world space
	Vec3 SectorCenter; // center position of sector in world space
	float fSectorSize;       // size of sector in world space
};

struct TERRAINBASETEXTUREPARAMS
{
	StdString BaseTextureName;
	float fBaseTextureScaleX;
	float fBaseTextureScaleY;
};

struct TERRAINSECTORPAINTPARAMS
{
	TERRAINSECTORPAINTPARAMS()
	{
		fNewLayerScale = 32.0f;
	}
	BRUSHPARAMS m_Brush;
	IHashString *m_pSplatTextureName;
	float fNewLayerScale; // scale to use for the layer if it doesn't exist yet
};


struct TERRAINLAYERINFOPARAMS
{
	TERRAINLAYERINFOPARAMS()
	{
		pBaseTextureName = NULL;
		fBaseTextureScaleX = 0.0f;
		fBaseTextureScaleY = 0.0f;
		pTerrainLayerList = NULL;
		pTerrainLayerMaskList = NULL;
	}

	IHashString *pBaseTextureName;
	float fBaseTextureScaleX;
	float fBaseTextureScaleY;

	struct TerrainLayer_t
	{
		IHashString *szDetailTexture;
		float fDetailTextureScaleX;
		float fDetailTextureScaleY;
	};

	std::vector<TerrainLayer_t> *pTerrainLayerList;

	struct TerrainLayerMask
	{
		IHashString *szMaskTexture;
	};

	std::vector<TerrainLayerMask> *pTerrainLayerMaskList;
};

struct TERRAINSECTORINSERTLAYER
{
	IHashString *pSplatInsertLocation;
	IHashString *pSplatTextureName;
	float fSplatScaleX;
	float fSplatScaleY;
};

struct TERRAINSECTORSWAPLAYERS
{
	IHashString *pSplatTextureNameA;
	IHashString *pSplatTextureNameB;
};

enum SECTORSETSTATEFLAGS
{
	SECTORSTATE_LAYERSCALE = (1<<0),
};
struct TERRAINSECTORSETSTATEPARAMS
{
	IHashString *m_pAffectedLayer;
	unsigned int m_iValidFields;
	float m_fLayerScaleX;
	float m_fLayerScaleY;
};

// Neighbor order: 1
//                2*0
//                 3
struct TERRAINSECTORNEIGHBORINFO
{
	int iSourceX, iSourceY;
	IHashString *vNeighborNames[4];	// NULL if no neighbor exists on that side
	int vNeighborLODs[4];			// -1 if no neighbor exists on that side
	TERRAINSECTORGETPOLYDATA vPolyData[4]; // zeroed if no neighbor exists on that side
};

struct TERRAINBRUSHAREAINFO
{
	BRUSHPARAMS m_Brush;
	float fAverageValue;
};

struct TERRAINLAYERBRUSHAREAINFO
{
	TERRAINBRUSHAREAINFO info;
	IHashString *pSplatTextureName;
};

// tolua_begin

struct TERRAINDECALPARAMS
{
	IHashString *hsTextureName;
	float fSize;
	Vec4 *v4Color;
	float fLingerTime;
	float fFadeTime;
	Vec3 *vPosition;
	TERRAINDECALPARAMS()
	{
		hsTextureName = NULL;
		fSize = 1.0f;
		v4Color = NULL;
		fLingerTime = 1.0f;
		fFadeTime = 1.0f;
		vPosition = NULL;
	}
};

// tolua_end

#endif