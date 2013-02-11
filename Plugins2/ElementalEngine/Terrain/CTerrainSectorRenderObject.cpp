///==========================================================================
/// \file	CTerrainSectorRenderObject.cpp
/// \brief	Implementation of CTerrainSectorRenderObject for Terrain
/// \date	04-24-2005
/// \author	Brian Bazyk
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
///==========================================================================

#include "StdAfx.h"
#include "PerformanceProfiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void SampleHeightMapData( float in_fSectorSize, int in_iSampleResolution, const float *in_pHeightMap, int in_iHeightMapSize, float *out_pHeightMapSamples );
bool SectorSpaceBrushBlitter( Vec3 position, float *pDataMap, int iDataMapSize, BRUSHPARAMS *pBrush, float fAvgValue=0 );
bool TerrainSectorBrushAreaInfo( Vec3 sectorPos, float *pDataMap, int iDataMapSize, TERRAINBRUSHAREAINFO &info );

//==========================================================================
// Terrain Layer Class
//==========================================================================
CTerrainSectorRenderObject::CTerrainLayer::CTerrainLayer()
{
	m_pMaskBuffer = NULL;
	m_iMaskSize = 0;
	m_pSplatTexture = NULL;
	m_fSplatTextureScaleX = 1.0f;
	m_fSplatTextureScaleY = 1.0f;
	m_bLoaded = false;
}

void CTerrainSectorRenderObject::CTerrainLayer::Unload()
{
	SAFE_DELETE_ARRAY( m_pMaskBuffer );
	m_iMaskSize = 0;

	if (m_pSplatTexture)
	{
		TEXTUREOBJECTPARAMS tobj;
		tobj.bLoad = false;
		tobj.Name = &m_szSplatTextureName;
		tobj.TextureObjectInterface = NULL;
		static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
		m_pSplatTexture = NULL;
	}

	m_bLoaded = false;
}

///	\brief	set splat texture name (file path)
///	\param	name - filepath to texture
void CTerrainSectorRenderObject::CTerrainLayer::SetSplatTextureName(LPCTSTR name)
{
	// keep file paths lowercase for hash string compares
	StdString szSplatFile(name);
	szSplatFile.MakeSafeFileName();
	m_szSplatTextureName = szSplatFile;
}

//==========================================================================


REGISTER_COMPONENT(CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainSectorPaint, OnTerrainSectorPaint, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainRemoveUnusedLayers, OnTerrainRemoveUnusedLayers, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainLayerInfo, OnTerrainLayerInfo, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainSectorSetData, OnTerrainSectorSetData, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainSectorInsertLayer, OnTerrainSectorInsertLayer, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainSectorSwapLayers, OnTerrainSectorSwapLayers, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainSectorRemoveLayer, OnTerrainSectorRemoveLayer, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainSectorSetState, OnTerrainSectorSetState, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainSectorSetBaseTexture, OnTerrainSectorSetBaseTexture, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainSectorGetBaseTexture, OnTerrainSectorGetBaseTexture, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainSectorGetExtents, OnTerrainSectorGetExtents, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(GetTerrainSectorLOD, OnGetTerrainSectorLOD, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainSectorUpdateNeighborLOD, OnTerrainSectorUpdateNeighborLOD, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainSectorUpdateLOD, OnTerrainSectorUpdateLOD, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainSectorGetPolyData, OnTerrainSectorGetPolyData, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainSectorFreePolyData, OnTerrainSectorFreePolyData, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainSectorModifyLayerMask, OnTerrainSectorModifyLayerMask, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(TerrainSectorLayerGetBrushAreaInfo, OnTerrainSectorLayerGetBrushAreaInfo, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(SetLightmapData, OnSetLightmapData, CTerrainSectorRenderObject);
REGISTER_MESSAGE_HANDLER(GetGeometryAllocations, OnGetGeometryAllocations, CTerrainSectorRenderObject );
REGISTER_MESSAGE_HANDLER(GetLocalMatrixTransform, OnGetLocalMatrixTransform, CTerrainSectorRenderObject );
REGISTER_MESSAGE_HANDLER(TerrainSetPath, OnTerrainSetPath, CTerrainSectorRenderObject );

CTerrainSectorStaticBuffers CTerrainSectorRenderObject::m_StaticBuffers;
int CTerrainSectorRenderObject::m_iBaseIndexCount = 0;

IComponent *CTerrainSectorRenderObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new CTerrainSectorRenderObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CTerrainSectorRenderObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CTerrainSectorRenderObject::CTerrainSectorRenderObject(IHashString *parentName, IHashString *name, bool bAddToHier)
:	CRenderObject<>(_T("CTerrainManager"), _T("CTerrainSectorRenderObject"), parentName, name), 
m_fCurrentVersion(1.1f)
{
	m_bTransformIsDirty = true;
	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}
	m_bGeneratedLightmap = false;

	static CHashString TMTypeName(_T("CTerrainManager"));
	m_pTerrainMgr = dynamic_cast<CTerrainManager*>(m_ToolBox->GetComponent(&TMTypeName));

	GETRENDERERINTERFACEPARAMS renderInterface;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface))
		m_pRenderer = renderInterface.m_RendererInterface;
	else
		m_pRenderer = NULL;

	m_VertexBufferInterface = NULL;
	m_IndexBufferInterface = NULL;

	m_pVertexData = NULL;
	m_iVertexCount = 0;
	for (int i=0; i < 4; i++)
		m_pEdgeNormals[i] = NULL;

	m_CurrentLODIndexCount = 0;

	m_Position.Set( 0, 0, 0 );
	m_CenterPosition.Set( 0, 0, 0 );
	m_fBoundingSphereRadius = 0;
	m_BoundingMins.Set( 0, 0, 0 );
	m_BoundingMaxs.Set( 0, 0, 0 );

	m_bDirtyNormals = false;
	m_bDirtyNormalEdges = false;
	m_iNormalGenerationTimer = 0;

	m_szMaskTexture0 = _T("");
	m_pBaseTexture = NULL;
	m_pMaskTexture0 = NULL;
	m_LightmapTexture = NULL;

	// LOD
	m_iCurrentLOD = -1;
	for (int i=0; i < 4; i++)
		m_vCurrentLODEdges[i] = -1;

	// scene settings
	m_SceneSettings = NULL;
	static DWORD msgHash_CreateSceneSettings = CHashString(_T("CreateSceneSettings")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateSceneSettings, sizeof( ISceneSettings ), &m_SceneSettings ) != MSG_HANDLED)
	{
		m_ToolBox->Log( LOGERROR, _T("TerrainSectorRenderObject failed to create scene settings."));
		return;
	}
	m_SceneSettings->SetShaderFlags( SHADOW_RECEIVE );

	// material
	m_pMaterial = CreateMaterial( _T("_Mat") );
	m_pMaterial->SetRenderState( RENDERSTATE_CULLMODE, RENDERSTATEPARAM_CULLREGULAR );
	m_pMaterial->SetRenderState( RENDERSTATE_ZENABLE, RENDERSTATEPARAM_ZTRUE );
	m_pMaterial->SetRenderState( RENDERSTATE_ZWRITEENABLE, RENDERSTATEPARAM_ZTRUE );

	// distant terrain material
	m_pDistantMaterial = CreateMaterial( _T("_DistantMat") );
	m_pDistantMaterial->SetRenderState( RENDERSTATE_CULLMODE, RENDERSTATEPARAM_CULLREGULAR );
	m_pDistantMaterial->SetRenderState( RENDERSTATE_ZENABLE, RENDERSTATEPARAM_ZTRUE );
	m_pDistantMaterial->SetRenderState( RENDERSTATE_ZWRITEENABLE, RENDERSTATEPARAM_ZTRUE );
	m_pDistantMaterial->SetEffect( m_pTerrainMgr->GetDistantEffect() );

	// geometry
	m_pGeometry = NULL;
	static DWORD msgHash_CreateGeometry = CHashString(_T("CreateGeometry")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateGeometry, sizeof( IGeometry** ), &m_pGeometry ) != MSG_HANDLED)
	{
		m_ToolBox->Log( LOGERROR, _T("TerrainSectorRenderObject failed to create geometry."));
		return;
	}
	m_pGeometry->SetSceneSettings( m_SceneSettings );
	m_pGeometry->SetMaterial( m_pMaterial );

	// distant geometry
	m_pDistantGeometry = NULL;
	if (m_ToolBox->SendMessage(msgHash_CreateGeometry, sizeof( IGeometry** ), &m_pDistantGeometry ) != MSG_HANDLED)
	{
		m_ToolBox->Log( LOGERROR, _T("TerrainSectorRenderObject failed to create distant geometry."));
		return;
	}
	m_pDistantGeometry->SetSceneSettings( m_SceneSettings );
	m_pDistantGeometry->SetMaterial( m_pDistantMaterial );
}

CTerrainSectorRenderObject::~CTerrainSectorRenderObject()
{
	// remove bounding boxed object
	RemoveBoundingObject();

	static DWORD msgHash_ParentRemoveBoundingBox = CHashString(_T("ParentRemoveBoundingBox")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_ParentRemoveBoundingBox, 0, NULL, &m_ParentName);

	static DWORD msgHash_RemoveVertexAllocation = CHashString(_T("RemoveVertexAllocation")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveVertexAllocation, sizeof(m_VertexBufferAllocation), &m_VertexBufferAllocation );
	m_VertexBufferAllocation.m_Size = 0;
	static DWORD msgHash_RemoveIndexAllocation = CHashString(_T("RemoveIndexAllocation")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RemoveIndexAllocation, sizeof(m_IndexBufferAllocation), &m_IndexBufferAllocation );
	m_IndexBufferAllocation.m_Size = 0;

	SAFE_DELETE_ARRAY( m_pVertexData );
	m_iVertexCount = 0;

	for (int i=0; i < 4; i++) {
		SAFE_DELETE_ARRAY( m_pEdgeNormals[i] );
	}

	// destroy scene settings
	if( m_SceneSettings != NULL )
	{
		static DWORD msgHash_DestroySceneSettings = CHashString(_T("DestroySceneSettings")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DestroySceneSettings, sizeof( ISceneSettings ), &m_SceneSettings );
		m_SceneSettings = NULL;
	}

	// destroy geometry
	if( m_pGeometry != NULL )
	{
		static DWORD msgHash_DestroyGeometry = CHashString(_T("DestroyGeometry")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DestroyGeometry, sizeof( IGeometry** ), &m_pGeometry );
		m_pGeometry = NULL;
	}

	// destroy distant geometry
	if( m_pDistantGeometry != NULL )
	{
		static DWORD msgHash_DestroyGeometry = CHashString(_T("DestroyGeometry")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DestroyGeometry, sizeof( IGeometry** ), &m_pDistantGeometry );
		m_pDistantGeometry = NULL;
	}

	// destroy material
	if( m_pMaterial != NULL )
	{
		DELETEOBJECTPARAMS dop;
		dop.name = m_pMaterial->GetName();
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop );
		m_pMaterial = NULL;
	}

	// destroy distant material
	if( m_pDistantMaterial != NULL )
	{
		DELETEOBJECTPARAMS dop;
		dop.name = m_pDistantMaterial->GetName();
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop );
		m_pDistantMaterial = NULL;
	}

	DeInit();
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

void CTerrainSectorRenderObject::DeInit()
{	
	UnloadAllLayers();

	if (m_pMaskTexture0)
	{
		// is it correct logic?
		m_szMaskTexture0 = _T("");
	}

	RemoveMaskTexture();
}

void CTerrainSectorRenderObject::Init()
{
	UnloadAllLayers();
	if (*m_szMaskTexture0.GetString() == _T('\0'))
	{
		RemoveMaskTexture();
	}

	HEIGHTMAPDATAPARAMS hmdata;
	static DWORD msgHash_TerrainSectorGetData = CHashString(_T("TerrainSectorGetData")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_TerrainSectorGetData, sizeof(hmdata), &hmdata, GetParentName() );
	if (hmdata.pData)
	{
		ResampleDataFromHeightMap( (float*)hmdata.pData, hmdata.iWidth );
		GenerateNormals();
	}

	// Load Base Texture
	if (!m_pBaseTexture)
		m_pBaseTexture = dynamic_cast<ITextureObject*>( LoadTexture( m_szBaseTexture.GetString() ) );

	// Load Mask
	if (!m_pMaskTexture0 && (*m_szMaskTexture0.GetString() != '\0'))
	{
		m_pMaskTexture0 = dynamic_cast<ITextureObject*>( LoadTexture( m_szMaskTexture0.GetString() ) );
		if (m_pMaskTexture0 == NULL)
		{
			m_ToolBox->Log(LOGERROR, _T("Terrain sector mask texture '%s' does not exist\n"), m_szMaskTexture0.GetString());
			m_szMaskTexture0 = _T("");
		}
	}

	// refresh layers
	swap(m_Layers, m_ReadedLayers);
	m_ReadedLayers.clear();

	// Load Layers
	TERRAINLAYERLIST::iterator itrLayer;
	for (itrLayer = m_Layers.begin(); itrLayer != m_Layers.end(); itrLayer++)
	{
		LoadLayer( &*itrLayer );
	}

	UpdateLayerMasksFromTextures();

	m_iNormalGenerationTimer = 0;

	// setup base indices for use by physics
	if (m_iBaseIndexCount == 0)
	{
		int vLODEdges[4] = { 0,0,0,0 };
		int iMaxIndexCount = WriteIndicesLOD( 0, vLODEdges, NULL );
		unsigned short *pIndexData = m_StaticBuffers.GetSectorBaseIndexData( iMaxIndexCount );
		m_iBaseIndexCount = WriteIndicesLOD( 0, vLODEdges, pIndexData );
	}

	if (m_iCurrentLOD == -1)
	{
		SetLOD( 0, false, false );
	}

	DetachAllLights();

	bool bLightmaps = (m_LightmapTexture != NULL);
	m_pMaterial->SetEffect( m_pTerrainMgr->GetDetailEffect( bLightmaps ) );
}

void CTerrainSectorRenderObject::Serialize(IArchive &ar)
{
	IArchive *pArchive = &ar;
	if (pArchive->IsReading())
	{
		pArchive = ConvertTerrainArchive( ar ); // reads a version float
		bool bArchiveConverted = (pArchive != NULL);
		if (!pArchive)
			pArchive = &ar;

		unsigned int iLayerCount = 0;
		StdString szBaseTexture, szMaskTexture0;

		// read data
		pArchive->Read( m_iSectorX );
		pArchive->Read( m_iSectorY );
		pArchive->Read( szBaseTexture );
		pArchive->Read( m_fBaseTextureScaleX );
		pArchive->Read( m_fBaseTextureScaleY );
		pArchive->Read( szMaskTexture0, _T("MaskTexture") );
		pArchive->Read( iLayerCount );
		szBaseTexture.MakeSafeFileName();
		szMaskTexture0.MakeSafeFileName();
		m_szBaseTexture = szBaseTexture;
		m_szMaskTexture0 = szMaskTexture0;

		// serialize is used to transmit data more frequently than on initialize
		// so we ignore layer information unless we are currently empty
		// otherwise it would get very complicated removing and adding layers here
		// without reinitializing them
		// so list of loaded layers is saved until Init() call

		unsigned int minLayer = (iLayerCount < 4) ? iLayerCount : 4;
		m_ReadedLayers.resize( minLayer);
		for (TERRAINLAYERLIST::iterator itLayers = m_ReadedLayers.begin();
			 itLayers != m_ReadedLayers.end(); ++itLayers)
		{
			StdString szSplatTextureName;
			pArchive->Read( szSplatTextureName );
			itLayers->SetSplatTextureName( szSplatTextureName.c_str() );

			pArchive->Read( itLayers->m_fSplatTextureScaleX);
			pArchive->Read( itLayers->m_fSplatTextureScaleY );
		}

		// close the converted archive
		if (bArchiveConverted)
			pArchive->Close();
	}
	else
	{
		int nLayers = (int)m_Layers.size();
		pArchive->Write( m_fCurrentVersion, _T("Version") );
		pArchive->Write( m_iSectorX, _T("SectorX") );
		pArchive->Write( m_iSectorY, _T("SectorY") );
		pArchive->Write( m_szBaseTexture.GetString(), _T("BaseTexture") );
		pArchive->Write( m_fBaseTextureScaleX, _T("BaseScaleX") );
		pArchive->Write( m_fBaseTextureScaleY, _T("BaseScaleY") );
		pArchive->Write( m_szMaskTexture0.GetString(), _T("MaskTexture") );
		pArchive->Write( nLayers, _T("NumLayers") );

		for (int i=0; i < nLayers; i++)
		{
			char szSplatTextureLabel[64];
			sprintf( szSplatTextureLabel, "SplatTexture%i", i );
			char szSplatScaleLabelX[64];
			sprintf( szSplatScaleLabelX, "SplatScale%iX", i );
			char szSplatScaleLabelY[64];
			sprintf( szSplatScaleLabelY, "SplatScale%iY", i );

			CTerrainLayer &rLayer = m_Layers[i];
			pArchive->Write( rLayer.m_szSplatTextureName.GetString(), StdString(szSplatTextureLabel) );
			pArchive->Write( rLayer.m_fSplatTextureScaleX, StdString(szSplatScaleLabelX) );
			pArchive->Write( rLayer.m_fSplatTextureScaleY, StdString(szSplatScaleLabelY) );
		}
	}
}

DWORD CTerrainSectorRenderObject::OnTerrainSectorLayerGetBrushAreaInfo(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TERRAINLAYERBRUSHAREAINFO));
	TERRAINLAYERBRUSHAREAINFO *pParams = (TERRAINLAYERBRUSHAREAINFO*)in_params;

	CHashString tempSplatName;
	tempSplatName = m_ToolBox->GetRelativePath((TCHAR *)pParams->pSplatTextureName->GetString());

	// find layer by splat name
	CTerrainLayer *pLayer = FindTerrainLayer( &tempSplatName );
	if (!pLayer)
		return MSG_ERROR;

	if (pLayer->m_pMaskBuffer == NULL)
		return MSG_ERROR;

	int nSamples = pLayer->m_iMaskSize * pLayer->m_iMaskSize;

	float *pFloatData = m_StaticBuffers.GetTempSectorFloatTextureBuffer( nSamples );

	// convert byte buffer to float buffer
	for (int i=0; i < nSamples; i++)
	{
		pFloatData[i] = (float)pLayer->m_pMaskBuffer[i] / 255.0f;
	}

	BRUSHPARAMS brush = pParams->info.m_Brush;
	if (brush.m_bMaxCap)
	{
		// be sure to clamp to at least [0,1]
		brush.m_fMaxCap = Clamp( brush.m_fMaxCap, 0.0f, 1.0f );
	}
	else
	{
		brush.m_bMaxCap = true;
		brush.m_fMaxCap = 1.0f;
	}
	if (brush.m_bMinCap)
	{
		// be sure to clamp to at least [0,1]
		brush.m_fMinCap = Clamp( brush.m_fMinCap, 0.0f, 1.0f );
	}
	else
	{
		brush.m_bMinCap = true;
		brush.m_fMinCap = 0.0f;
	}

	TerrainSectorBrushAreaInfo( m_Position, pFloatData, pLayer->m_iMaskSize, pParams->info );

	return MSG_HANDLED_PROCEED;
}

DWORD CTerrainSectorRenderObject::OnTerrainSectorPaint(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TERRAINSECTORPAINTPARAMS));
	TERRAINSECTORPAINTPARAMS *pParams = (TERRAINSECTORPAINTPARAMS*)in_params;

	CHashString tempSplatName;
	tempSplatName = m_ToolBox->GetRelativePath((TCHAR *)pParams->m_pSplatTextureName->GetString());

	// find layer by splat name
	CTerrainLayer *pLayer = FindTerrainLayer( &tempSplatName );
	if (!pLayer)
	{
		// if not found, try to insert and load it
		InsertLayer( NULL, &tempSplatName, pParams->fNewLayerScale, pParams->fNewLayerScale );
		pLayer = FindTerrainLayer( &tempSplatName );
		if (pLayer)
			LoadLayer( pLayer );
		else
			return MSG_ERROR;
	}

	if (!m_pMaskTexture0)
	{
		UpdateTextureMasksFromLayers();
	}

	if (pLayer->m_pMaskBuffer == NULL)
	{
		int iLayerBufferSize = m_pMaskTexture0->GetWidth() * m_pMaskTexture0->GetWidth();
		pLayer->m_pMaskBuffer = new unsigned char[iLayerBufferSize];
		pLayer->m_iMaskSize = m_pMaskTexture0->GetWidth();
		memset( pLayer->m_pMaskBuffer, 0, iLayerBufferSize );
	}

	int nSamples = pLayer->m_iMaskSize * pLayer->m_iMaskSize;

	// convert byte buffer to float buffer for painting
	float *pFloatData = m_StaticBuffers.GetTempSectorFloatTextureBuffer( nSamples );
	for (int i=0; i < nSamples; i++)
	{
		pFloatData[i] = (float)pLayer->m_pMaskBuffer[i] / 255.0f;
	}

	// paint
	BRUSHPARAMS brush = pParams->m_Brush;
	if (brush.m_bMaxCap)
	{
		// be sure to clamp to at least [0,1]
		brush.m_fMaxCap = Clamp( brush.m_fMaxCap, 0.0f, 1.0f );
	}
	else
	{
		brush.m_bMaxCap = true;
		brush.m_fMaxCap = 1.0f;
	}
	if (brush.m_bMinCap)
	{
		// be sure to clamp to at least [0,1]
		brush.m_fMinCap = Clamp( brush.m_fMinCap, 0.0f, 1.0f );
	}
	else
	{
		brush.m_bMinCap = true;
		brush.m_fMinCap = 0.0f;
	}
	// blit!
	if (!SectorSpaceBrushBlitter( m_Position, pFloatData, pLayer->m_iMaskSize, &brush ))
	{
		// nothing changed so don't bother with the texture write
		return MSG_HANDLED_PROCEED;
	}

	// copy float buffer back to byte buffer for texture upload
	for (int i=0; i < nSamples; i++)
	{
		pLayer->m_pMaskBuffer[i] = (unsigned char)(pFloatData[i] * 255.0f);
	}

	UpdateTextureMasksFromLayers();

	return MSG_HANDLED_PROCEED;
}

bool TerrainSectorBrushAreaInfo( Vec3 sectorPos, float *pDataMap, int iDataMapSize, TERRAINBRUSHAREAINFO &info )
{
	static CHashString TMTypeName(_T("CTerrainManager"));
	CTerrainManager *pTerrainMgr = (CTerrainManager*)EngineGetToolBox()->GetComponent( &TMTypeName );
	assert( pTerrainMgr != NULL );

	BRUSHPARAMS &brush = info.m_Brush;
	float *pBrushData = brush.m_pBrushData;
	float fBrushStrength = brush.m_fStrength;

	float fSectorSize = pTerrainMgr->GetMetrics().fSectorSize;
	float fSectorToDataMapScale = (float)(iDataMapSize-1) / fSectorSize;

	// size of brush in datamap space (number of pixels)
	int iTerrainBrushRad = (int)(brush.m_fSize * fSectorToDataMapScale * 0.5f);
	// convert brush position from world to sector to datamap space
	int iTerrainBrushSize = iTerrainBrushRad * 2 + 1;

	float fSectorBrushPosX = brush.m_pPosition->x - sectorPos.x;
	float fSectorBrushPosY = brush.m_pPosition->z - sectorPos.z;
	int iTerrainBrushPosX = (int)(g_Math.Round(fSectorBrushPosX * fSectorToDataMapScale));
	int iTerrainBrushPosY = (int)(g_Math.Round(fSectorBrushPosY * fSectorToDataMapScale));

	// calculate brush bounds in datamap space
	int iStartX = (iTerrainBrushPosX) - iTerrainBrushRad;
	int iEndX   = (iTerrainBrushPosX) + iTerrainBrushRad;
	int iStartY = (iTerrainBrushPosY) - iTerrainBrushRad;
	int iEndY   = (iTerrainBrushPosY) + iTerrainBrushRad;
	// if brush bounds are completely outside of datamap space, do nothing
	if ((iStartX > (iDataMapSize-1)) || (iStartY > (iDataMapSize-1)))
		return false;
	if ((iEndX < 0) || (iEndY < 0))
		return false;
	// clamp brush bounds to datamap bounds
	int iSectorStartX = Clamp( iStartX, 0, iDataMapSize-1 );
	int iSectorEndX   = Clamp(   iEndX, 0, iDataMapSize-1 );
	int iSectorStartY = Clamp( iStartY, 0, iDataMapSize-1 );
	int iSectorEndY   = Clamp(   iEndY, 0, iDataMapSize-1 );

	info.fAverageValue = 0;
	for (int y=iSectorStartY; y <= iSectorEndY; y++)
	{
		for (int x=iSectorStartX; x <= iSectorEndX; x++)
		{
			int i = y * iDataMapSize + x;
			info.fAverageValue += pDataMap[i];
		}
	}
	info.fAverageValue /= ((iSectorEndY-iSectorStartY)+1)*((iSectorEndX-iSectorStartX)+1);

	return true;
}

/// Brush params are in world space but blit takes place in sector space and write out to 
/// a buffer.
bool SectorSpaceBrushBlitter( Vec3 position, float *pDataMap, int iDataMapSize, BRUSHPARAMS *pBrush, float fAvgValue )
{
	static CHashString TMTypeName(_T("CTerrainManager"));
	CTerrainManager *pTerrainMgr = (CTerrainManager*)EngineGetToolBox()->GetComponent( &TMTypeName );
	assert( pTerrainMgr != NULL );

	float *pBrushData = pBrush->m_pBrushData;
	float fBrushStrength = pBrush->m_fStrength;

	float fSectorSize = pTerrainMgr->GetMetrics().fSectorSize;
	float fSectorToDataMapScale = (float)(iDataMapSize-1) / fSectorSize;

	// size of brush in datamap space (number of pixels)
	int iTerrainBrushRad = (int)(pBrush->m_fSize * fSectorToDataMapScale * 0.5f);
	// convert brush position from world to sector to datamap space
	int iTerrainBrushSize = iTerrainBrushRad * 2 + 1;

	float fSectorBrushPosX = pBrush->m_pPosition->x - position.x;
	float fSectorBrushPosY = pBrush->m_pPosition->z - position.z;
	int iTerrainBrushPosX = (int)(g_Math.Round(fSectorBrushPosX * fSectorToDataMapScale));
	int iTerrainBrushPosY = (int)(g_Math.Round(fSectorBrushPosY * fSectorToDataMapScale));

	// calculate brush bounds in datamap space
	int iStartX = (iTerrainBrushPosX) - iTerrainBrushRad;
	int iEndX   = (iTerrainBrushPosX) + iTerrainBrushRad;
	int iStartY = (iTerrainBrushPosY) - iTerrainBrushRad;
	int iEndY   = (iTerrainBrushPosY) + iTerrainBrushRad;
	// if brush bounds are completely outside of datamap space, do nothing
	if ((iStartX > (iDataMapSize-1)) || (iStartY > (iDataMapSize-1)))
		return false;
	if ((iEndX < 0) || (iEndY < 0))
		return false;
	// clamp brush bounds to datamap bounds
	int iSectorStartX = Clamp( iStartX, 0, iDataMapSize-1 );
	int iSectorEndX   = Clamp(   iEndX, 0, iDataMapSize-1 );
	int iSectorStartY = Clamp( iStartY, 0, iDataMapSize-1 );
	int iSectorEndY   = Clamp(   iEndY, 0, iDataMapSize-1 );

	// loop thru brush
	float fTerrainBrushSize = Clamp( (float)(iTerrainBrushSize-1), 1.0f, 999999.9f );
	for (int y=iSectorStartY; y <= iSectorEndY; y++)
	{
		float ny = (float)(y-iStartY) / fTerrainBrushSize;
		for (int x=iSectorStartX; x <= iSectorEndX; x++)
		{
			float nx = (float)(x-iStartX) / fTerrainBrushSize;

			int i = y * iDataMapSize + x;

			// code to go from a coord in our texture brush to a sample in the source brush
			int xBrush = (int)(nx * (pBrush->m_iBrushDataSize-1)); // [0,1] -> [0,31] (if brushsize==32)
			int yBrush = (int)(ny * (pBrush->m_iBrushDataSize-1)); // [0,1] -> [0,31]
			int iBrush = yBrush * pBrush->m_iBrushDataSize + xBrush; // [0,31*32 + 31]

			float fData = pDataMap[i];

			// UNDONE: optimize by duping loops and moving if's outside loops
			if (pBrush->m_eBrushOp == BRUSH_OP_ADD)
				fData += fBrushStrength * pBrushData[iBrush];
			else if (pBrush->m_eBrushOp == BRUSH_OP_MUL)
				fData *= fBrushStrength * pBrushData[iBrush];
			else if (pBrush->m_eBrushOp == BRUSH_OP_MASK_SET)
			{
				if (pBrushData[iBrush] != 0.0f)
				{
					fData = fBrushStrength;
				}
			}
			else if (pBrush->m_eBrushOp == BRUSH_OP_SMOOTH)
			{
				float fBrushPoint = pBrushData[iBrush] * fBrushStrength;
				fData = (fBrushPoint)*fAvgValue + (1.0f-fBrushPoint)*fData;
			}

			// clamp max
			if (pBrush->m_bMaxCap && fData > pBrush->m_fMaxCap)
                fData = pBrush->m_fMaxCap;
			// clamp min
			if (pBrush->m_bMinCap && fData < pBrush->m_fMinCap)
                fData = pBrush->m_fMinCap;

			pDataMap[i] = fData;
		}
	}

	return true;
}

DWORD CTerrainSectorRenderObject::OnTerrainRemoveUnusedLayers(DWORD size, void *in_params)
{
	TERRAINLAYERLIST::iterator itrLayers = m_Layers.begin();
	while (itrLayers != m_Layers.end())
	{
		CTerrainLayer &rLayer = *itrLayers;

		// remove unloaded layers (likely a texture failed to load)
		if (!rLayer.m_bLoaded)
		{
			rLayer.Unload();
			itrLayers = m_Layers.erase( itrLayers );
			continue;
		}

		int nSamples = rLayer.m_iMaskSize * rLayer.m_iMaskSize;
		unsigned char *pSourceData = rLayer.m_pMaskBuffer;
		if (pSourceData)
		{
			int i = 0;
			for (i=0; i < nSamples; i++)
			{
				if (pSourceData[i] != 0)
					break;
			}

			if (i == nSamples)
			{
				// layer is empty, remove it
				rLayer.Unload();
				itrLayers = m_Layers.erase( itrLayers );
				continue;
			}
		}

		itrLayers++;
	}

	UpdateTextureMasksFromLayers();

	return MSG_HANDLED_PROCEED;
}

void CTerrainSectorRenderObject::ResampleDataFromHeightMap( float *in_pHeightMap, int in_iHeightMapSize )
{
	float fSectorSize = m_pTerrainMgr->GetMetrics().fSectorSize;
	int iVertexResolution = m_pTerrainMgr->GetMetrics().iSectorResolution + 1;
	int nSamples = iVertexResolution*iVertexResolution;

	float *pHeightData = m_StaticBuffers.GetTempSectorHeightBuffer( nSamples );

	SampleHeightMapData( fSectorSize, iVertexResolution, in_pHeightMap, in_iHeightMapSize, pHeightData );

	SetVertexData( pHeightData );

	UpdateBoundingObject();

	// update physics
	static DWORD msgHash_ReinitPhysics = CHashString(_T("ReinitPhysics")).GetUniqueID();
	static CHashString hsPhysicsObjType(_T("CPhysicsObject"));
	m_ToolBox->SendMessage(msgHash_ReinitPhysics, 0, NULL, GetParentName(), &hsPhysicsObjType );
}

void CTerrainSectorRenderObject::SetVertexData( float *pHeightData )
{
	float fSectorSize = m_pTerrainMgr->GetMetrics().fSectorSize;
	float fZScale = m_pTerrainMgr->GetMetrics().fZScale;
	int iVertexResolution = m_pTerrainMgr->GetMetrics().iSectorResolution + 1; // resolution of vertices (samples)
	int iVertexCount = iVertexResolution * iVertexResolution;
	int iGridResolution = iVertexResolution - 1; // resolution of grid (spaces between vertices, aka edges)
	int iIndexCount = iGridResolution * iGridResolution * 6;

	AllocateVertices( iVertexCount );

	TERRAINVERTEX *pBaseVertex = NULL;
	if (m_VertexBufferInterface) {
		m_VertexBufferInterface->Lock( m_VertexBufferAllocation.m_Offset, iVertexCount, (void**)&pBaseVertex );
	}
	// create vertices
	float fMinHeight=999999.9f, fMaxHeight=-999999.9f;
	if (pBaseVertex && m_pVertexData)
	{
		float fTexCoordOffsetX = (float)m_iSectorX;
		float fTexCoordOffsetY = (float)m_iSectorY;
		float fInvScale = 1.f / fSectorSize;
		float fVertexCountScaleX = 1.0f / (float)(iVertexResolution-1);
		float fVertexCountScaleY = 1.0f / (float)(iVertexResolution-1);
		TERRAINVERTEX *pVertex = pBaseVertex;
		int iVertex = 0;
		for (int y=0; y < iVertexResolution; y++)
		{
			// normalized coords within this sector
			float ny = (float)y * fVertexCountScaleY;
			// normalized coords within terrain
	//		float fy = ((ny * fSectorSize) + fOffsetY) / fTerrainSizeY;
			// world position
			float posy = ny * fSectorSize;

			for (int x=0; x < iVertexResolution; x++, pVertex++, iVertex++)
			{
				// normalized coords within this sector
				float nx = (float)x * fVertexCountScaleX;
				// normalized coords within terrain
	//			float fx = ((nx * fSectorSize) + fOffsetX) / fTerrainSizeX;
				// world position
				float posx = nx * fSectorSize;

				float posz = pHeightData[iVertex] * fZScale;

				pVertex->pos.Set( posx, posz, posy );
				pVertex->masktex[0] = nx;
				pVertex->masktex[1] = ny;
				pVertex->tiletex[0] = nx + fTexCoordOffsetX;
				pVertex->tiletex[1] = ny + fTexCoordOffsetY;
				pVertex->normal.Set( 0,1,0 );
				pVertex->tangent.Set( 1,0,0 );
				pVertex->lightmapuv[0] = nx;
				pVertex->lightmapuv[1] = ny;

				m_pVertexData[iVertex].Set( posx, posz, posy );

				// update mins/maxs
				if (posz < fMinHeight)
					fMinHeight = posz;
				if (posz > fMaxHeight)
					fMaxHeight = posz;
			}
		}
	}
	if (m_VertexBufferInterface) {
		m_VertexBufferInterface->Unlock();
	}

	// calculate bounding AABB and sphere radius
	m_fMinHeight = fMinHeight;
	m_fMaxHeight = fMaxHeight;
	UpdateBoundingData();

	m_bDirtyNormals = true;
}

int CTerrainSectorRenderObject::WriteIndicesLOD( int iLOD, int vLODEdges[4], unsigned short *pIndexData )
{
	int iSkip = 1 << iLOD;
	int iSectorResolution = m_pTerrainMgr->GetMetrics().iSectorResolution;
	int iVertexResolution = iSectorResolution + 1; // resolution of vertices (samples)
	int iGridResolution = iSectorResolution / iSkip; // resolution of grid (spaces between vertices, aka edges)
	int iIndexCount = iGridResolution * iGridResolution * 6;

	int nLODEdges = 0;
	for (int i=0; i < 4; i++) {
		if (vLODEdges[i])
			nLODEdges++;
	}

	// extra tris per edge is (half tesselation of a corner) + (half tesselation of the other corner) +
	// (full tesselation of all the grid spaces in between)
	int iExtraTriangles = (8 * iSectorResolution) * nLODEdges;
	int iExtraIndices = iExtraTriangles * 3;

	int RequiredSize = iIndexCount+iExtraIndices;

	if (pIndexData == NULL)
		return RequiredSize;

	int nFinalIndexCount = 0;

	int vVertices[9] = {
		0, 
		iSkip/2, 
		iSkip, 
		iVertexResolution*(iSkip/2), 
		iVertexResolution*(iSkip/2) + iSkip/2, 
		iVertexResolution*(iSkip/2) + iSkip, 
		iVertexResolution*iSkip, 
		iVertexResolution*iSkip + iSkip/2, 
		iVertexResolution*iSkip + iSkip
	};

	int vBase[4][4] = {
		{0,6,2,-1}, // quadtype0 tri0 |\|
		{8,2,6,-1}, // quadtype0 tri1 |\|
		{2,0,8,-1}, // quadtype1 tri0 |/|
		{6,8,0,-1}, // quadtype1 tri1 |/|
	};

	int vSplits[4][7] = {
		{0,4,2,0,6,4,-1}, // quadtype0 tri0 |\|
		{8,4,6,8,2,4,-1}, // quadtype0 tri1 |\|
		{2,4,8,2,0,4,-1}, // quadtype1 tri0 |/|
		{6,4,0,6,8,4,-1}, // quadtype1 tri1 |/|
	};

	int vTesselated[4][13] = {
		{6,4,3, 3,4,0, 0,4,1, 1,4,2,-1}, // quadtype0 tri0 |\|
		{2,4,5, 5,4,8, 8,4,7, 7,4,6,-1}, // quadtype0 tri1 |\|
		{0,4,1, 1,4,2, 2,4,5, 5,4,8,-1}, // quadtype1 tri0 |/|
		{8,4,7, 7,4,6, 6,4,3, 3,4,0,-1}, // quadtype1 tri1 |/|
	};

	int iQuadType = 0;
	int iRowQuadType = 0;
	for (int y=0; y < iGridResolution; y++)
	{
		iQuadType = iRowQuadType = !iRowQuadType;
		for (int x=0; x < iGridResolution; x++)
		{
			int iStart = y*iVertexResolution*iSkip + x*iSkip;

			int iNull = -1;
			int *pTrianglesA = &iNull;
			int *pTrianglesB = &iNull;

			if ((x > 0) && (x < (iGridResolution-1)) && (y == 0) && vLODEdges[3])
			{
				// bottom (no corners)
				pTrianglesA = &vTesselated[iQuadType*2+0][0];
				pTrianglesB = &vSplits[iQuadType*2+1][0];
			}
			else if ((x > 0) && (x < (iGridResolution-1)) && (y == (iGridResolution-1)) && vLODEdges[1])
			{
				// top (no corners)
				pTrianglesA = &vSplits[iQuadType*2+0][0];
				pTrianglesB = &vTesselated[iQuadType*2+1][0];
			}
			else if ((y > 0) && (y < (iGridResolution-1)) && (x == 0) && vLODEdges[2])
			{
				// right (no corners)
				if (iQuadType)
				{
					pTrianglesA = &vSplits[iQuadType*2+0][0];
					pTrianglesB = &vTesselated[iQuadType*2+1][0];
				}
				else
				{
					pTrianglesA = &vTesselated[iQuadType*2+0][0];
					pTrianglesB = &vSplits[iQuadType*2+1][0];
				}
			}
			else if ((y > 0) && (y < (iGridResolution-1)) && (x == (iGridResolution-1)) && vLODEdges[0])
			{
				// left (no corners)
				if (iQuadType)
				{
					pTrianglesA = &vTesselated[iQuadType*2+0][0];
					pTrianglesB = &vSplits[iQuadType*2+1][0];
				}
				else
				{
					pTrianglesA = &vSplits[iQuadType*2+0][0];
					pTrianglesB = &vTesselated[iQuadType*2+1][0];
				}
			}
			else if (x == 0 && y == 0 && (vLODEdges[2] || vLODEdges[3]))
			{
				// corner
				if (vLODEdges[2] && vLODEdges[3])
				{
					pTrianglesA = &vTesselated[0][0];
					pTrianglesB = &vTesselated[1][0];
				}
				else if (vLODEdges[2])
				{
					pTrianglesA = &vTesselated[3][0];
					pTrianglesB = &vSplits[2][0];
				}
				else if (vLODEdges[3])
				{
					pTrianglesA = &vSplits[3][0];
					pTrianglesB = &vTesselated[2][0];
				}
			}
			else if (x == (iGridResolution-1) && y == 0 && (vLODEdges[0] || vLODEdges[3]))
			{
				// corner
				if (vLODEdges[0] && vLODEdges[3])
				{
					pTrianglesA = &vTesselated[0][0];
					pTrianglesB = &vTesselated[1][0];
				}
				else if (vLODEdges[0])
				{
					pTrianglesA = &vSplits[0][0];
					pTrianglesB = &vTesselated[1][0];
				}
				else if (vLODEdges[3])
				{
					pTrianglesA = &vTesselated[0][0];
					pTrianglesB = &vSplits[1][0];
				}
			}
			else if (x == (iGridResolution-1) && y == (iGridResolution-1) && (vLODEdges[0] || vLODEdges[1]))
			{
				// corner
				if (vLODEdges[0] && vLODEdges[1])
				{
					pTrianglesA = &vTesselated[0][0];
					pTrianglesB = &vTesselated[1][0];
				}
				else if (vLODEdges[0])
				{
					pTrianglesA = &vSplits[3][0];
					pTrianglesB = &vTesselated[2][0];
				}
				else if (vLODEdges[1])
				{
					pTrianglesA = &vTesselated[3][0];
					pTrianglesB = &vSplits[2][0];
				}
			}
			else if (x == 0 && y == (iGridResolution-1) && (vLODEdges[1] || vLODEdges[2]))
			{
				// corner
				if (vLODEdges[1] && vLODEdges[2])
				{
					pTrianglesA = &vTesselated[0][0];
					pTrianglesB = &vTesselated[1][0];
				}
				else if (vLODEdges[1])
				{
					pTrianglesA = &vSplits[0][0];
					pTrianglesB = &vTesselated[1][0];
				}
				else if (vLODEdges[2])
				{
					pTrianglesA = &vTesselated[0][0];
					pTrianglesB = &vSplits[1][0];
				}
			}
			else
			{
				pTrianglesA = &vBase[iQuadType*2+0][0];
				pTrianglesB = &vBase[iQuadType*2+1][0];
			}

			for (int i=0; pTrianglesA[i] >= 0; i++)
			{
				pIndexData[nFinalIndexCount++] = iStart + vVertices[pTrianglesA[i]];
			}
			for (int i=0; pTrianglesB[i] >= 0; i++)
			{
				pIndexData[nFinalIndexCount++] = iStart + vVertices[pTrianglesB[i]];
			}

			iQuadType = !iQuadType;
		}
	}
	return nFinalIndexCount;
}

void CTerrainSectorRenderObject::SetLOD( int iLOD, bool bNeighborLODChange, bool bNotifyNeighbors )
{
	bool bLODChanged = (iLOD != m_iCurrentLOD);
	if (bLODChanged || bNeighborLODChange)
	{
		TERRAINSECTORNEIGHBORINFO neighborinfo;
		memset( &neighborinfo, 0, sizeof(neighborinfo) );
		neighborinfo.iSourceX = m_iSectorX;
		neighborinfo.iSourceY = m_iSectorY;

		static DWORD msgHash_GetTerrainSectorNeighborInfo = CHashString(_T("GetTerrainSectorNeighborInfo")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetTerrainSectorNeighborInfo, sizeof(neighborinfo), &neighborinfo ) != MSG_HANDLED)
			return;

		// count and list the edges we need to add triangles to
		// if an edge has a neighbor with a higher lod then we need to generate more triangles
		// for the current lod along that edge
		// Also signal if the edge lod's have changed so we can early out if they have not.
		bool bLODEdgesChanged = false;
		int vLODEdges[4];
		for (int i=0; i < 4; i++)
		{
			if (neighborinfo.vNeighborLODs[i] < iLOD && 
				neighborinfo.vNeighborLODs[i] >= 0)
			{
				vLODEdges[i] = 1;
			}
			else
				vLODEdges[i] = 0;

			if (vLODEdges[i] != m_vCurrentLODEdges[i])
			{
				bLODEdgesChanged = true;
				m_vCurrentLODEdges[i] = vLODEdges[i];
			}
		}

		if (iLOD == m_iCurrentLOD && !bLODEdgesChanged)
			return;

		int RequiredSize = WriteIndicesLOD( iLOD, vLODEdges, NULL );
		AllocateIndices( RequiredSize );

		int nFinalIndexCount = 0;
		if (m_IndexBufferInterface)
		{
			unsigned short *pLockedIndices = NULL;
			m_IndexBufferInterface->Lock( m_IndexBufferAllocation.m_Offset, RequiredSize, (void**)&pLockedIndices );

			if (pLockedIndices) {
				nFinalIndexCount = WriteIndicesLOD( iLOD, vLODEdges, pLockedIndices );
			}

			m_IndexBufferInterface->Unlock();
		}

		assert( nFinalIndexCount <= RequiredSize );

		m_IndexBufferAllocation.m_Size = nFinalIndexCount;
		m_pGeometry->SetIndexBufferAllocation( m_IndexBufferAllocation );
		m_pDistantGeometry->SetIndexBufferAllocation( m_IndexBufferAllocation );

		static DWORD msgHash_TerrainSectorResetVertexData = CHashString(_T("TerrainSectorResetVertexData")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TerrainSectorResetVertexData, 0, NULL, GetParentName() );

		m_CurrentLODIndexCount = nFinalIndexCount;
		m_iCurrentLOD = iLOD; // set our current lod

		// notify all neighbors that our lod has changed and they should check their
		// geometry and update it as necessary
		if (bLODChanged && bNotifyNeighbors)
		{
			for (int i=0; i < 4; i++)
			{
				if (neighborinfo.vNeighborNames[i] != NULL)
				{
					static DWORD msgHash_TerrainSectorUpdateNeighborLOD = CHashString(_T("TerrainSectorUpdateNeighborLOD")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_TerrainSectorUpdateNeighborLOD, 0, NULL, neighborinfo.vNeighborNames[i] );
				}
			}
		}
	}
}

int CTerrainSectorRenderObject::CalculateLOD()
{
	Vec3 CamPos;
	CONTEXTCAMVECTORPARAMS ccvp;
	ccvp.pRC = NULL;
	ccvp.vdata = &CamPos;
	static DWORD msgHash_GetActiveCameraPosition = CHashString(_T("GetActiveCameraPosition")).GetUniqueID();
	if( m_ToolBox->SendMessage(msgHash_GetActiveCameraPosition, sizeof(CONTEXTCAMVECTORPARAMS), &ccvp ) != MSG_NOT_HANDLED )
	{
		CamPos = *ccvp.vdata;
		Vec3 vecDist = m_CenterPosition - CamPos;
		float fDistSqrd = vecDist.Dot( vecDist );
		float ADist = sqrt( fDistSqrd ) - m_fBoundingSphereRadius;

		float fLODDistance = CalculateLODDistance();

		// set geometry lod
		float fLOD2 = fLODDistance * 2;
		float fLOD1 = fLODDistance;
		if( ADist > fLOD2 )
			return 2;
		else if (ADist > fLOD1)
			return 1;
		else
			return 0;
	}
	return -1;
}

float CTerrainSectorRenderObject::CalculateLODDistance()
{
	// make sure lod distance is at least the size of the sector or we can have an lod of 2 next to an lod of 0
	// which will cause seaming
	float fEpsilon = 0.5f; // safety margin
	return max( m_pTerrainMgr->GetMetrics().fLODDistance, m_pTerrainMgr->GetMetrics().fSectorSize+fEpsilon );
}

void CTerrainSectorRenderObject::DistantRender( UINT pass )
{
	if( m_pDistantGeometry == NULL)
	{
		return;
	}

	m_pDistantGeometry->Render( m_SceneSettings );
}

void CTerrainSectorRenderObject::DetailRender( UINT pass )
{
	if( m_VertexBufferInterface == NULL ||
		m_IndexBufferInterface == NULL ||
		m_CurrentLODIndexCount == 0 ||
		m_pTerrainMgr == NULL )
	{
		return;
	}

	float fLODDistance = CalculateLODDistance();

	static CHashString hszVertexContantParams( _T("Params") );
	static CHashString hszPixelContantBaseScales( _T("BaseScales") );
	static CHashString hszPixelContantLayerScales( _T("LayerScales") );
	static CHashString hszPixelContantMaskModifiers( _T("MaskModifiers") );

	m_pMaterial->SetVertexConstant( &hszVertexContantParams, fLODDistance, 0.0f, 0.0f, 0.0f );
	m_pMaterial->SetPixelConstant( &hszPixelContantBaseScales, m_fBaseTextureScaleX, 0.0f, 0.0f, 0.0f );
	float vLayerScales[4] = { 0, 0, 0, 0 };
	float vMaskModifiers[4] = { 0, 0, 0, 0 };
	for (unsigned int i=0; i < 4; i++)
	{
		if (i < m_Layers.size())
		{
			vLayerScales[i] = m_Layers[i].m_fSplatTextureScaleX;
			vMaskModifiers[i] = 1.0f;
		}
	}
	m_pMaterial->SetPixelConstant( &hszPixelContantLayerScales, vLayerScales, 4 );
	m_pMaterial->SetPixelConstant( &hszPixelContantMaskModifiers, vMaskModifiers, 4 );

	m_pMaterial->SetTexture( 0, m_pTerrainMgr->GetBaseTextureStage(), m_pBaseTexture );
	UINT baseMaskStage = m_pTerrainMgr->GetMaskTextureStageStart();
	m_pMaterial->SetTexture( 0, baseMaskStage, m_pMaskTexture0 );
	for (unsigned int i=0; i < 4; i++)
	{
		if (i < m_Layers.size())
			m_pMaterial->SetTexture( 0, (baseMaskStage+1)+i, m_Layers[i].m_pSplatTexture );
		else
			m_pMaterial->SetTexture( 0, (baseMaskStage+1)+i, NULL );
	}
	if (m_LightmapTexture)
		m_pMaterial->SetTexture( 0, m_pTerrainMgr->GetLightmapStage(), m_LightmapTexture );
//	m_pRenderer->RenderIndexBuffer( m_IndexBufferInterface, m_VertexBufferInterface, 0, m_CurrentLODIndexCount/3 );

	m_pGeometry->Render( m_SceneSettings );
}

void CTerrainSectorRenderObject::GenerateUVs( int iVertexResolution, float * outUVStream )
{
	float fSectorSize = m_pTerrainMgr->GetMetrics().fSectorSize;
	float fTexCoordOffsetX = (float)m_iSectorX;
	float fTexCoordOffsetY = (float)m_iSectorY;
	float fInvScale = 1.f / fSectorSize;
	float fVertexCountScaleX = 1.0f / (float)(iVertexResolution-1);
	float fVertexCountScaleY = 1.0f / (float)(iVertexResolution-1);
	int iVertex = 0;
	for (int y=0; y < iVertexResolution; y++)
	{
		// normalized coords within this sector
		float ny = (float)y * fVertexCountScaleY;
		for (int x=0; x < iVertexResolution; x++, outUVStream += 2)
		{
			// normalized coords within this sector
			float nx = (float)x * fVertexCountScaleX;
			outUVStream[0] = nx;
			outUVStream[1] = ny;
		}
	}
}

void CTerrainSectorRenderObject::GenerateNormals( int iVertexResolution, const Vec3 *pVertices, Vec3 *pNormals )
{
	int iInnerRowEnd = iVertexResolution-1;

	Vec3 a,b,c,d;
	int iVertex = 0;
	for (int j=0; j <= iInnerRowEnd; j++)
	{
		for (int i=0; i <= iInnerRowEnd; i++)
		{
			if (i == 0) // left bound
			{
				c = pVertices[iVertex+1] - pVertices[iVertex];
				a = -c;
			}
			else if (i == iInnerRowEnd) // right bound
			{
				a = pVertices[iVertex-1] - pVertices[iVertex];
				c = -a;
			}
			else
			{
				c = pVertices[iVertex+1] - pVertices[iVertex];
				a = pVertices[iVertex-1] - pVertices[iVertex];
			}

			if (j == 0) // top bound
			{
				d = pVertices[iVertex+iVertexResolution] - pVertices[iVertex];
				b = -d;
			}
			else if (j == iInnerRowEnd) // bottom bound
			{
				b = pVertices[iVertex-iVertexResolution] - pVertices[iVertex];
				d = -b;
			}
			else
			{
				d = pVertices[iVertex+iVertexResolution] - pVertices[iVertex];
				b = pVertices[iVertex-iVertexResolution] - pVertices[iVertex];
			}

			pNormals[iVertex] = b.Cross( a );
			pNormals[iVertex] += c.Cross( b );
			pNormals[iVertex] += d.Cross( c );
			pNormals[iVertex] += a.Cross( d );

			pNormals[iVertex].Normalize();

			iVertex++;
		}
	}
}

void CTerrainSectorRenderObject::GenerateNormals()
{
	if( !m_pVertexData )
		return;

	int vLODEdges[4] = { 0,0,0,0 };
	int iMaxIndexCount = WriteIndicesLOD( 0, vLODEdges, NULL );
	unsigned short *pIndexData = m_StaticBuffers.GetSectorBaseIndexData( iMaxIndexCount );
	int iIndexCount = WriteIndicesLOD( 0, vLODEdges, pIndexData );

	int nVertices = m_VertexBufferAllocation.m_Size;
	Vec3 *pNormalData = m_StaticBuffers.GetTempSectorNormalBuffer( nVertices );

	int iVertexResolution = m_pTerrainMgr->GetMetrics().iSectorResolution + 1;

	GenerateNormals( iVertexResolution, m_pVertexData, pNormalData );

	TERRAINVERTEX *pVertices = NULL;
	m_VertexBufferInterface->Lock( m_VertexBufferAllocation.m_Offset, m_VertexBufferAllocation.m_Size, (void**)&pVertices );
	if (pVertices)
	{
		for (int i=0; i < nVertices; i++)
		{
			pVertices[i].normal = pNormalData[i];
		}
	}
	GenerateTangents( pVertices, m_VertexBufferAllocation.m_Size, pIndexData, iIndexCount );
	m_VertexBufferInterface->Unlock();

	// set edge normals
	for (int i=0; i < 4; i++)
	{
		if (m_pEdgeNormals[i] == NULL)
			m_pEdgeNormals[i] = new Vec3[iVertexResolution];

		switch (i)
		{
		case 0: // right edge (x+)
			for (int v=0; v < iVertexResolution; v++)
			{
				int src = v * iVertexResolution + (iVertexResolution-1);
				m_pEdgeNormals[i][v] = pNormalData[src];
			}
			break;
		case 1: // top edge (y+)
			for (int v=0; v < iVertexResolution; v++)
			{
				int src = v  + (iVertexResolution-1) * iVertexResolution;
				m_pEdgeNormals[i][v] = pNormalData[src];
			}
			break;
		case 2: // left edge (x-)
			for (int v=0; v < iVertexResolution; v++)
			{
				int src = v * iVertexResolution;
				m_pEdgeNormals[i][v] = pNormalData[src];
			}
			break;
		case 3: // bottom edge (y-)
			for (int v=0; v < iVertexResolution; v++)
			{
				int src = v;
				m_pEdgeNormals[i][v] = pNormalData[src];
			}
			break;
		};
	}

	m_bDirtyNormals = false;
	m_bDirtyNormalEdges = true;
}

void CTerrainSectorRenderObject::StitchNormals()
{
	// Get Neighbor Info
	TERRAINSECTORNEIGHBORINFO neighborinfo;
	memset( &neighborinfo, 0, sizeof(neighborinfo) );
	neighborinfo.iSourceX = m_iSectorX;
	neighborinfo.iSourceY = m_iSectorY;
	static DWORD msgHash_GetTerrainSectorNeighborInfo = CHashString(_T("GetTerrainSectorNeighborInfo")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetTerrainSectorNeighborInfo, sizeof(neighborinfo), &neighborinfo ) != MSG_HANDLED)
		return;

	int iVertexResolution = m_pTerrainMgr->GetMetrics().iSectorResolution + 1;

	TERRAINVERTEX *pVertices = NULL;
	m_VertexBufferInterface->Lock( m_VertexBufferAllocation.m_Offset, m_VertexBufferAllocation.m_Size, (void**)&pVertices );
	for (int i=0; i < 4; i++)
	{
		if (!m_pEdgeNormals[i])
			continue;

		int iNeighborEdge = (i+2)%4;
		Vec3 *pNeighborEdgeNormals = neighborinfo.vPolyData[i].pEdgeNormals[iNeighborEdge];
		if (!pNeighborEdgeNormals)
			continue;

		int src = 0, dst = 0, soff = 0, doff = 0;
		switch (i)
		{
		case 0: // right neighbor (x+)
			src = 0;
			soff = iVertexResolution;
			dst = (iVertexResolution-1);
			doff = iVertexResolution;
			break;
		case 1: // top neighbor (y+)
			src = 0;
			soff = 1;
			dst = (iVertexResolution-1) * iVertexResolution;
			doff = 1;
			break;
		case 2: // left neighbor (x-)
			src = (iVertexResolution-1);
			soff = iVertexResolution;
			dst = iVertexResolution;
			doff = 1;
			break;
		case 3: // bottom neighbor (y-)
			src = (iVertexResolution-1) * iVertexResolution;
			soff = 1;
			dst = 0;
			doff = 1;
			break;
		};

		for (int v=0; v < iVertexResolution; v++, src+=soff, dst+=doff)
		{
			pVertices[dst].normal = (m_pEdgeNormals[i][v] + pNeighborEdgeNormals[v]) * 0.5f;
			pVertices[dst].tangent = pVertices[dst].normal.Cross( Vec3(0,0,1) );
			pVertices[dst].tangent.Normalize();
		}
	}
	m_VertexBufferInterface->Unlock();

	m_bDirtyNormalEdges = false;
}

bool CTerrainSectorRenderObject::Update()
{
	if (m_bTransformIsDirty)
 	{
		m_bTransformIsDirty = false;

		// set sector position
		Vec3 tempPosition;
		static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &tempPosition, GetParentName() ) == MSG_HANDLED)
			SetPosition( tempPosition );
		else
			SetPosition( Vec3(0,0,0) );
		
		if( !m_bGeneratedLightmap )
		{
			m_bGeneratedLightmap = true;
			static CHashString type(_T("CTerrainSectorRenderObject"));
			ADDLIGHTMAPPABLEOBJECT addLightmappableMsg;
			addLightmappableMsg.name = GetName();
			addLightmappableMsg.type = &type;
			addLightmappableMsg.bUseIncomingTexCoord = true;
			addLightmappableMsg.useTextureCoordIndex = 0; //use mask's which is stretched over the sector
			static DWORD msgHash_AddLightmappableObject = CHashString("AddLightmappableObject").GetUniqueID();
			//m_ToolBox->SendMessage(msgHash_AddLightmappableObject, sizeof( ADDLIGHTMAPPABLEOBJECT ),&addLightmappableMsg );
		}
 	}
	return true;
}

bool CTerrainSectorRenderObject::Render( UINT pass, IEffect * override )
{
	PERFORMANCE_PROFILER_AUTO(TerrainSectorRenderObject_Render);
	if (pass != 0)
		return false;

	if (m_iCurrentLOD == -1)
	{
		SetLOD( 0, false, false );
		
	}

	if( !m_VertexBufferInterface || !m_IndexBufferInterface )
	{
		return false;
	}

	if (m_bDirtyNormals && (m_iNormalGenerationTimer < GetTickCount()))
	{
		GenerateNormals();
		m_iNormalGenerationTimer = GetTickCount() + 400;
	}

	if (m_bDirtyNormalEdges)
	{
		StitchNormals();
	}

	Matrix4x4 worldTransform;
	worldTransform.SetTranslation( m_Position );
	m_pRenderer->SetMatrix( WORLD_MATRIX, worldTransform.GetMatrix() );

	// use distant or detailed render pass technique

	// find fog end
	FOGVALUEMESSAGE fvm;
	float fogEnd=0;
	static DWORD msgHash_GetFogValue = CHashString(_T("GetFogValue")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetFogValue, sizeof(fvm), &fvm ) == MSG_HANDLED)
	{
		fogEnd = fvm.m_Start + fvm.m_FogDistance;
	}

	// calc distance from camera to edge of terrain sector's bounding sphere and compare to fog end
	bool bDistantRender = false;
	Vec3 CamPos;
	CONTEXTCAMVECTORPARAMS ccvp;
	ccvp.pRC = NULL;
	ccvp.vdata = &CamPos;
	static DWORD msgHash_GetActiveCameraPosition = CHashString(_T("GetActiveCameraPosition")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetActiveCameraPosition, sizeof(CONTEXTCAMVECTORPARAMS), &ccvp ) == MSG_HANDLED)
	{
		float fDistance = (m_CenterPosition - CamPos).Length();
		fDistance = max( (fDistance - m_fBoundingSphereRadius), 0.0f );
		bDistantRender = (fDistance > fogEnd);
	}

	if (bDistantRender)
	{
		DistantRender( pass );
	}
	else
	{
		DetailRender( pass );
	}

//	RenderNormals();
	return true;
}

void CTerrainSectorRenderObject::RenderNormals()
{
	TERRAINVERTEX *pVertices = NULL;
	m_VertexBufferInterface->Lock( m_VertexBufferAllocation.m_Offset, m_VertexBufferAllocation.m_Size, (void**)&pVertices );

	// draw lines along normals
	int iVertexResolution = m_pTerrainMgr->GetMetrics().iSectorResolution + 1;
	int iVertexCount = iVertexResolution * iVertexResolution;
	TERRAINVERTEX *pVertex = pVertices;
	for (int i=0; i < iVertexCount; i++, pVertex++)
	{
		Vec3 p0, p1;
		p0 = m_Position + pVertex->pos;
		p1 = m_Position + pVertex->pos + pVertex->normal;
		m_pRenderer->Draw3DLine( p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, 
			0, 0, 255, 
			1.0f );
	}

	m_VertexBufferInterface->Unlock();
}

DWORD CTerrainSectorRenderObject::GetRenderPriority()
{
	return 0;
}

void CTerrainSectorRenderObject::UpdateBoundingData()
{
	float fSectorSize = m_pTerrainMgr->GetMetrics().fSectorSize;

	m_BoundingMins.Set( m_Position.x, m_Position.y + m_fMinHeight, m_Position.z );
	m_BoundingMaxs.Set( m_Position.x+fSectorSize, m_Position.y + m_fMaxHeight, m_Position.z+fSectorSize );
	m_fBoundingSphereRadius = m_BoundingMaxs.DistanceBetween(m_BoundingMins) * 0.5f;
	m_CenterPosition = (m_BoundingMaxs + m_BoundingMins) * 0.5f;
}

void CTerrainSectorRenderObject::SetPosition( const Vec3 &vec )
{
	m_Position = vec;

	UpdateBoundingData();

	UpdateBoundingObject();
}

Vec3 &CTerrainSectorRenderObject::GetPosition()
{
	return m_Position;
}

void CTerrainSectorRenderObject::GetBoundingSphere( Vec3 &position, float &radius )
{
	position = m_CenterPosition;
	radius = m_fBoundingSphereRadius;
}

CTerrainSectorRenderObject::TERRAINLAYERLIST::iterator CTerrainSectorRenderObject::FindTerrainLayerItr( IHashString *pSplatTextureName )
{
	if (!pSplatTextureName)
		return m_Layers.end();

	DWORD iTargetUID = pSplatTextureName->GetUniqueID();

	TERRAINLAYERLIST::iterator itrLayers;
	for (itrLayers = m_Layers.begin(); itrLayers != m_Layers.end(); itrLayers++)
	{
		if (itrLayers->m_szSplatTextureName.GetUniqueID() == iTargetUID)
			return itrLayers;
	}

	return m_Layers.end();
}

CTerrainSectorRenderObject::CTerrainLayer* CTerrainSectorRenderObject::FindTerrainLayer( IHashString *pSplatTextureName )
{
	TERRAINLAYERLIST::iterator itrLayer = FindTerrainLayerItr( pSplatTextureName );
	if (itrLayer != m_Layers.end())
		return &(*itrLayer);
	else
		return NULL;
}

DWORD CTerrainSectorRenderObject::OnTerrainLayerInfo(DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(TERRAINLAYERINFOPARAMS) );
	TERRAINLAYERINFOPARAMS *pParams = (TERRAINLAYERINFOPARAMS*)in_params;

	// write out sector information into the message
	pParams->pBaseTextureName = &m_szBaseTexture;
	pParams->fBaseTextureScaleX = m_fBaseTextureScaleX;
	pParams->fBaseTextureScaleY = m_fBaseTextureScaleY;
	if (pParams->pTerrainLayerList)
	{
		TERRAINLAYERLIST::iterator itrLayers;
		for (itrLayers = m_Layers.begin(); itrLayers != m_Layers.end(); itrLayers++)
		{
			TERRAINLAYERINFOPARAMS::TerrainLayer_t param;
			param.szDetailTexture = &itrLayers->m_szSplatTextureName;
			param.fDetailTextureScaleX = itrLayers->m_fSplatTextureScaleX;
			param.fDetailTextureScaleY = itrLayers->m_fSplatTextureScaleY;
			pParams->pTerrainLayerList->push_back( param );
		}
		TERRAINLAYERINFOPARAMS::TerrainLayerMask maskInfo;
		maskInfo.szMaskTexture = &m_szMaskTexture0;
		pParams->pTerrainLayerMaskList->push_back(maskInfo);
	}

	return MSG_HANDLED_PROCEED;
}


bool CTerrainSectorRenderObject::LoadLayer( CTerrainLayer *pLayer )
{
	pLayer->Unload();

	// get splat texture
	ITextureObject *pSplatTexture = dynamic_cast<ITextureObject*>( LoadTexture( pLayer->m_szSplatTextureName.GetString() ) );

	pLayer->m_pSplatTexture = pSplatTexture;
	if (pSplatTexture)
		pLayer->m_bLoaded = true;

	return true;
}

void CTerrainSectorRenderObject::UnloadAllLayers()
{
	TEXTUREOBJECTPARAMS tobj;

	// unload layer resources but don't destroy the list
	TERRAINLAYERLIST::iterator itrLayer;
	for (itrLayer = m_Layers.begin(); itrLayer != m_Layers.end(); itrLayer++)
	{
		itrLayer->Unload();
	}

	// Remove Base Texture
	if (m_pBaseTexture)
	{
		tobj.bLoad = false;
		tobj.Name = &m_szBaseTexture;
		tobj.TextureObjectInterface = NULL;
		static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
		m_pBaseTexture = NULL;
	}
}

void CTerrainSectorRenderObject::UpdateLayerMasksFromTextures()
{
	unsigned char *pSourceData = NULL;
	if (m_pMaskTexture0)
	{
		pSourceData = (unsigned char *)m_pMaskTexture0->Read();
	}

	for (unsigned int iLayer=0; iLayer < m_Layers.size(); iLayer++)
	{
		CTerrainLayer *pLayer = &m_Layers[iLayer];
		if (pSourceData)
		{
			int iLayerBufferSize = m_pMaskTexture0->GetWidth() * m_pMaskTexture0->GetWidth();

			// delete previous mask buffer
			if (!pLayer->m_pMaskBuffer || pLayer->m_iMaskSize != m_pMaskTexture0->GetWidth())
			{
				SAFE_DELETE_ARRAY( pLayer->m_pMaskBuffer );
				pLayer->m_pMaskBuffer = new unsigned char[iLayerBufferSize];
			}
			// copy data from mask texture into layer mask
			for (int i=0; i < iLayerBufferSize; i++)
			{
				pLayer->m_pMaskBuffer[i] = pSourceData[i*4+iLayer];
			}
			pLayer->m_iMaskSize = m_pMaskTexture0->GetWidth();
		}
		else
		{
			SAFE_DELETE_ARRAY( pLayer->m_pMaskBuffer );
			pLayer->m_iMaskSize = 0;
		}
	}
}

void CTerrainSectorRenderObject::UpdateTextureMasksFromLayers()
{
	// no layers, nothing to do except remove the useless mask texture
	if (m_Layers.size() == 0)
	{
		// Remove Mask Texture
		if (m_pMaskTexture0)
		{
			TEXTUREOBJECTPARAMS tobj;
			tobj.bLoad = false;
			tobj.Name = &m_szMaskTexture0;
			tobj.TextureObjectInterface = NULL;
			static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
			m_pMaskTexture0 = NULL;
			m_szMaskTexture0 = _T("");
		}
		return;
	}

	// create a default mask texture
	if (!m_pMaskTexture0)
	{
		int iMaskTextureSize = 64;
		if (m_Layers.size() > 0)
			iMaskTextureSize = m_Layers[0].m_iMaskSize;
		CreateDefaultMaskTexture( iMaskTextureSize );
	}

	// mask creation failed somewhere
	if (!m_pMaskTexture0)
		return;

	// get temp byte buffer big enough for mask texture
	int iMaskTextureBufferSize = m_pMaskTexture0->GetWidth() * m_pMaskTexture0->GetWidth() * 4;
	unsigned char *pByteData = m_StaticBuffers.GetTempSectorByteTextureBuffer( iMaskTextureBufferSize );
	memset( pByteData, 0, iMaskTextureBufferSize ); // zero the mask
	// write layer masks into mask texture buffer
	for (unsigned int iLayer=0; iLayer < m_Layers.size(); iLayer++)
	{
		CTerrainLayer *pLayer = &m_Layers[iLayer];
		// sanity check that the layer buffer is the same size as the mask texture
		if (pLayer->m_iMaskSize != m_pMaskTexture0->GetWidth())
			continue;

		int iLayerBufferSize = pLayer->m_iMaskSize * pLayer->m_iMaskSize;
		// write layer mask data into texture channel
		for (int i=0; i < iLayerBufferSize; i++)
		{
			pByteData[i*4+iLayer] = pLayer->m_pMaskBuffer[i];
		}
	}
	// write mask texture buffer to texture
	m_pMaskTexture0->Write( pByteData );
}

void CTerrainSectorRenderObject::CreateDefaultMaskTexture( int iMaskTextureSize )
{
	if (iMaskTextureSize == 0)
		iMaskTextureSize = 64;

	StdString szMaskName = GetParentName()->GetString();
	szMaskName += _T("_mask0.tga");
	szMaskName.MakeSafeFileName();
	m_szMaskTexture0 = szMaskName;

	CREATETEXTUREPARAMS createtextureparams;
	createtextureparams.bitDepth = 32;
	createtextureparams.Name = &m_szMaskTexture0;
	createtextureparams.sizeX = iMaskTextureSize;
	createtextureparams.sizeY = iMaskTextureSize;
	static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateTexture, sizeof(createtextureparams), &createtextureparams );
	m_pMaskTexture0 = dynamic_cast<ITextureObject*>( createtextureparams.TextureObjectInterface );
}

///	\brief	remove mask texture from texture manager and clear pointer to texture object
void CTerrainSectorRenderObject::RemoveMaskTexture()
{
	// Remove Mask Texture
	if (m_pMaskTexture0)
	{
		TEXTUREOBJECTPARAMS tobj;
		tobj.bLoad = false;
		tobj.Name = &m_szMaskTexture0;
		tobj.TextureObjectInterface = NULL;
		static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
		m_pMaskTexture0 = NULL;
	}
}

void CTerrainSectorRenderObject::InsertLayer( 
	IHashString *pSplatInsertLocation, 
	IHashString *pSplatTextureName, 
	float fSplatScaleX, 
	float fSplatScaleY )
{
	// max layer count of 4
	if (m_Layers.size() >= 4)
		return;

	// insert layer into list
	CTerrainLayer newLayer;
	newLayer.SetSplatTextureName( pSplatTextureName->GetString() );
	newLayer.m_fSplatTextureScaleX = fSplatScaleX;
	newLayer.m_fSplatTextureScaleY = fSplatScaleY;

	// find optional previous splat for insertion position
	TERRAINLAYERLIST::iterator itrLocation = FindTerrainLayerItr( pSplatInsertLocation );
	if (itrLocation == m_Layers.end())
		m_Layers.push_back( newLayer );
	else
		m_Layers.insert( itrLocation, newLayer );
}

DWORD CTerrainSectorRenderObject::OnTerrainSectorInsertLayer( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(TERRAINSECTORINSERTLAYER) );
	TERRAINSECTORINSERTLAYER *pParams = (TERRAINSECTORINSERTLAYER*)in_params;

	InsertLayer( pParams->pSplatInsertLocation, pParams->pSplatTextureName, pParams->fSplatScaleX, pParams->fSplatScaleY );

	// try to load layer data
	CTerrainLayer *pLayer = FindTerrainLayer( pParams->pSplatTextureName );
	if (pLayer)
	{
		LoadLayer( pLayer );
		UpdateTextureMasksFromLayers();

		return MSG_HANDLED_STOP;
	}
	else
		return MSG_ERROR;
}

DWORD CTerrainSectorRenderObject::OnTerrainSectorSwapLayers( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(TERRAINSECTORSWAPLAYERS) );
	TERRAINSECTORSWAPLAYERS *pParams = (TERRAINSECTORSWAPLAYERS*)in_params;

	TERRAINLAYERLIST::iterator itrLayerA = FindTerrainLayerItr( pParams->pSplatTextureNameA );
	TERRAINLAYERLIST::iterator itrLayerB = FindTerrainLayerItr( pParams->pSplatTextureNameB );
	if (itrLayerA == m_Layers.end() || itrLayerB == m_Layers.end())
		return MSG_ERROR;

	CTerrainLayer tempLayer = *itrLayerA;
	*itrLayerA = *itrLayerB;
	*itrLayerB = tempLayer;
	
	return MSG_HANDLED_STOP;
}

DWORD CTerrainSectorRenderObject::OnTerrainSectorRemoveLayer( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(IHashString*) );
	IHashString *pParams = (IHashString*)in_params;

	TERRAINLAYERLIST::iterator itrLayer = FindTerrainLayerItr( pParams );
	if (itrLayer != m_Layers.end())
	{
		itrLayer->Unload();
		m_Layers.erase( itrLayer );
		UpdateTextureMasksFromLayers();
	
		return MSG_HANDLED_STOP;
	}

	return MSG_NOT_HANDLED;
}

DWORD CTerrainSectorRenderObject::OnTerrainSectorSetData( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(HEIGHTMAPDATAPARAMS) );
	HEIGHTMAPDATAPARAMS *pParams = (HEIGHTMAPDATAPARAMS*)in_params;

	ResampleDataFromHeightMap( (float*)pParams->pData, pParams->iWidth );

	return MSG_HANDLED_PROCEED;
}

DWORD CTerrainSectorRenderObject::OnTerrainSectorSetState( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(TERRAINSECTORSETSTATEPARAMS) );
	TERRAINSECTORSETSTATEPARAMS *pParams = (TERRAINSECTORSETSTATEPARAMS*)in_params;

	if (pParams->m_iValidFields & SECTORSTATE_LAYERSCALE)
	{
		CTerrainLayer *pLayer = FindTerrainLayer( pParams->m_pAffectedLayer );
		if (pLayer)
		{
			pLayer->m_fSplatTextureScaleX = pParams->m_fLayerScaleX;
			pLayer->m_fSplatTextureScaleY = pParams->m_fLayerScaleY;
		}
	}

	return MSG_HANDLED_STOP;
}

void CTerrainSectorRenderObject::GetBounds( Vec3 &aabbMin, Vec3 &aabbMax )
{
	aabbMin = m_BoundingMins;
	aabbMax = m_BoundingMaxs;
}


DWORD CTerrainSectorRenderObject::OnTerrainSectorSetBaseTexture( DWORD size, void *in_params )
{
	TERRAINBASETEXTUREPARAMS * parm;
	VERIFY_MESSAGE_SIZE( size, sizeof( TERRAINBASETEXTUREPARAMS ) );
	parm = (TERRAINBASETEXTUREPARAMS*)in_params;
	
	if( _tcslen( parm->BaseTextureName ) > 0 &&
		parm->fBaseTextureScaleX != 0 &&
		parm->fBaseTextureScaleY != 0 )
	{
		StdString szBaseTexture(parm->BaseTextureName);
		szBaseTexture.MakeSafeFileName();
		m_szBaseTexture = szBaseTexture;
		m_fBaseTextureScaleX = parm->fBaseTextureScaleX;
		m_fBaseTextureScaleY = parm->fBaseTextureScaleY;

		//Reinitialize		
		Init();
		return MSG_HANDLED_PROCEED;
	}
	return MSG_NOT_HANDLED;

}

DWORD CTerrainSectorRenderObject::OnTerrainSectorGetBaseTexture( DWORD size, void *in_params )
{
	TERRAINBASETEXTUREPARAMS * parm;
	VERIFY_MESSAGE_SIZE( size, sizeof( TERRAINBASETEXTUREPARAMS ) );
	parm = (TERRAINBASETEXTUREPARAMS*)in_params;
	
	parm->BaseTextureName = m_szBaseTexture.GetString();
	parm->fBaseTextureScaleX = m_fBaseTextureScaleX;
	parm->fBaseTextureScaleY = m_fBaseTextureScaleY;
	return MSG_HANDLED_STOP;

}

DWORD CTerrainSectorRenderObject::OnTerrainSectorGetExtents( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( TERRAINSECTOREXTENTSPARAMS ) );
	TERRAINSECTOREXTENTSPARAMS *pParams = (TERRAINSECTOREXTENTSPARAMS*)in_params;
	if (pParams->pMins)
		*pParams->pMins = m_BoundingMins;
	if (pParams->pMaxs)
		*pParams->pMaxs = m_BoundingMaxs;

	return MSG_HANDLED_STOP;
}

DWORD CTerrainSectorRenderObject::OnGetTerrainSectorLOD( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( int ) );
	int *pParams = (int*)in_params;
	*pParams = CalculateLOD();

	return MSG_HANDLED_STOP;
}

DWORD CTerrainSectorRenderObject::OnTerrainSectorUpdateNeighborLOD( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, 0 );

	// force an lod update because one of our neighbor's lod's have changed
	// and our geometry may be different
	int iUpdatedLOD = CalculateLOD();
	if (iUpdatedLOD >= 0)
		SetLOD( iUpdatedLOD, true, true );
	return MSG_HANDLED_STOP;
}

DWORD CTerrainSectorRenderObject::OnTerrainSectorUpdateLOD( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE( size, 0 );

	int iUpdatedLOD = CalculateLOD();
	if (iUpdatedLOD >= 0)
		SetLOD( iUpdatedLOD, false, true );
	return MSG_HANDLED_STOP;
}

DWORD CTerrainSectorRenderObject::OnTerrainSectorGetPolyData( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE(sizeof(TERRAINSECTORGETPOLYDATA), size);
	TERRAINSECTORGETPOLYDATA *params = (TERRAINSECTORGETPOLYDATA*)in_params;

	params->pVertexData = m_pVertexData;
	params->iVertexCount = m_iVertexCount;

	params->pIndexData = m_StaticBuffers.GetSectorBaseIndexData();
	params->iIndexCount = m_iBaseIndexCount;

	params->pEdgeNormals[0] = m_pEdgeNormals[0];
	params->pEdgeNormals[1] = m_pEdgeNormals[1];
	params->pEdgeNormals[2] = m_pEdgeNormals[2];
	params->pEdgeNormals[3] = m_pEdgeNormals[3];

	if( params->in_bGenerateUVs )
	{
		int iVertexResolution = params->in_iGenerationResolution;
		params->pUVStream = new float[2*iVertexResolution*iVertexResolution];
		GenerateUVs( iVertexResolution, params->pUVStream );
	}
	if (params->in_bGenerateNormals)
	{
		int iVertexResolution = params->in_iGenerationResolution;
		if (iVertexResolution == 0)
			iVertexResolution = m_pTerrainMgr->GetMetrics().iSectorResolution + 1;

		params->pNormalData = new Vec3[iVertexResolution*iVertexResolution];

		GenerateNormals( iVertexResolution, m_pVertexData, params->pNormalData );
	}

	params->vertexAllocation = m_VertexBufferAllocation;
	params->vertexInterface = m_VertexBufferInterface;
	params->indexAllocation = m_IndexBufferAllocation;
	params->indexInterface = m_IndexBufferInterface;

	return MSG_HANDLED_STOP;
}

DWORD CTerrainSectorRenderObject::OnTerrainSectorFreePolyData( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE(sizeof(TERRAINSECTORGETPOLYDATA), size);
	TERRAINSECTORGETPOLYDATA *params = (TERRAINSECTORGETPOLYDATA*)in_params;

	params->in_bGenerateNormals = false;

	params->pVertexData = NULL;
	params->iVertexCount = 0;

	params->pIndexData = NULL;
	params->iIndexCount = 0;

	params->pEdgeNormals[0] = NULL;
	params->pEdgeNormals[1] = NULL;
	params->pEdgeNormals[2] = NULL;
	params->pEdgeNormals[3] = NULL;

	SAFE_DELETE_ARRAY( params->pNormalData );

	return MSG_HANDLED_STOP;
}

DWORD CTerrainSectorRenderObject::OnTerrainSectorModifyLayerMask( DWORD size, void *in_params )
{
	VERIFY_MESSAGE_SIZE(sizeof(TERRAINSECTORMODIFYLAYERMASK), size);
	TERRAINSECTORMODIFYLAYERMASK *params = (TERRAINSECTORMODIFYLAYERMASK*)in_params;

	// make sure layer index is within bounds
	if (params->iLayer < 0 || params->iLayer >= (int)m_Layers.size()-1)
		return MSG_ERROR;

	CTerrainLayer *pLayer = &m_Layers[params->iLayer];
	if (!pLayer->m_pMaskBuffer)
	{
		return MSG_ERROR;
	}

	float fMin = params->fMin;
	float fMax = params->fMax;

	int iVertexResolution = m_pTerrainMgr->GetMetrics().iSectorResolution + 1;
	int iNormalResolution = pLayer->m_iMaskSize;

	TERRAINSECTORGETPOLYDATA tsgpd;
	tsgpd.in_bGenerateNormals = true;
	tsgpd.in_iGenerationResolution = iNormalResolution;
	if (OnTerrainSectorGetPolyData(sizeof(tsgpd), &tsgpd) != MSG_HANDLED)
		return false;

	Vec3 *pVertices = tsgpd.pVertexData;
	Vec3 *pNormals = tsgpd.pNormalData;

	float fMaskToSector = (float)iVertexResolution / (float)pLayer->m_iMaskSize;

	// UNDONE: need to have a feather factor to allow slope to feather off edges.  if 
	// feather factor is 1 then slope = slope renormalized between min and max.
	// if feather factor is 0 then slope = 0 or 1 if it's in range or not
	// anything in between will cause something that looks in between.

	float fFeather = 1.0;
	float fHalfMinMaxRange = (fMax-fMin)*0.5f;
	float fAvgMinMax = (fMax+fMin)*0.5f;

	if (params->iLayerMaskGenMode == TERRAIN_LAYERMASK_GEN_SLOPE)
	{
		int iTexel = 0;
		for (int y=0; y < pLayer->m_iMaskSize; y++)
		{
			for (int x=0; x < pLayer->m_iMaskSize; x++)
			{
				int iVertex = (int)(y*fMaskToSector) * iNormalResolution + (int)(x*fMaskToSector);
				// find slope as the angle between normal and 'up' and then unify
				float fSlope = (float)(acos(pNormals[iVertex].z) / (M_PI / 2.0));
				if (fSlope > fMax)
					fSlope = 0.0f;
				else if (fSlope < fMin)
					fSlope = 0.0f;
				else if (fSlope >= fAvgMinMax)
					fSlope = 1.0f - (fSlope - fAvgMinMax) / fHalfMinMaxRange;
				else if (fSlope < fAvgMinMax)
					fSlope = (fSlope-fMin) / fHalfMinMaxRange;

//				if (fSlope >= fMin && fSlope <= fMax)
//				{
//					fSlope = 1.0f;
//				}
//				else
//				{
//					fSlope = 0.0f;
//				}

				int iColor = (unsigned char)(fSlope * 255.0f);
				if (params->bAccumMask)
					pLayer->m_pMaskBuffer[iTexel++] = (pLayer->m_pMaskBuffer[iTexel] + iColor) > 255 ? 255 : (pLayer->m_pMaskBuffer[iTexel] + iColor);
				else
					pLayer->m_pMaskBuffer[iTexel++] = iColor;
			}
		}
	}
	else if (params->iLayerMaskGenMode == TERRAIN_LAYERMASK_GEN_HEIGHT)
	{
		int iTexel = 0;
		for (int y=0; y < pLayer->m_iMaskSize; y++)
		{
			for (int x=0; x < pLayer->m_iMaskSize; x++)
			{
				int iVertex = (int)(y*fMaskToSector) * iVertexResolution + (int)(x*fMaskToSector);
				float fHeight = pVertices[iVertex].z;
				if (fHeight >= fMin && fHeight <= fMax)
				{
					fHeight = 1.0f;
				}
				else
				{
					fHeight = 0.0f;
				}

				int iColor = (unsigned char)(fHeight * 255.0f);
				if (params->bAccumMask)
					pLayer->m_pMaskBuffer[iTexel++] = (pLayer->m_pMaskBuffer[iTexel] + iColor) > 255 ? 255 : (pLayer->m_pMaskBuffer[iTexel] + iColor);
				else
					pLayer->m_pMaskBuffer[iTexel++] = iColor;
			}
		}
	}

	UpdateTextureMasksFromLayers();

	OnTerrainSectorFreePolyData( sizeof(tsgpd), &tsgpd );

	return MSG_HANDLED_STOP;
}

bool CTerrainSectorRenderObject::AllocateVertices( int iVertexCount )
{
	if (m_VertexBufferAllocation.m_Size >= iVertexCount)
		return true;

	CHANNELDESCRIPTORLIST VertexFormat;
	VertexFormat.push_back( MESH_STREAM_VERTEX_POSITION3 );
	VertexFormat.push_back( MESH_STREAM_VERTEX_TEXTURECOORD1 );
	VertexFormat.push_back( MESH_STREAM_VERTEX_TEXTURECOORD2 );
	VertexFormat.push_back( MESH_STREAM_VERTEX_NORMAL );
	VertexFormat.push_back( MESH_STREAM_VERTEX_TANGENT3 );
	VertexFormat.push_back( MESH_STREAM_VERTEX_TEXTURECOORD3 );

	m_VertexBufferAllocation.VertexFormat = VertexFormat;
	// remove previous allocation
	if (m_VertexBufferAllocation.m_Size != 0)
	{
		static DWORD msgHash_RemoveVertexAllocation = CHashString(_T("RemoveVertexAllocation")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveVertexAllocation, sizeof(m_VertexBufferAllocation), &m_VertexBufferAllocation );
		m_VertexBufferAllocation.m_Size = 0;
	}

	// create new internal buffers
	SAFE_DELETE_ARRAY( m_pVertexData );
	m_pVertexData = new Vec3[iVertexCount];
	m_iVertexCount = iVertexCount;

	//create new allocation
	ALLOCATERENDERBUFFERMSG msg;
	msg.ElementsToAllocate = iVertexCount;
	msg.SizeVertexFormat = sizeof( TERRAINVERTEX );
	msg.VertexFormat = VertexFormat;
	msg.m_AllowSharedBuffer = false;
	static DWORD msgHash_AllocateVertices = CHashString(_T("AllocateVertices")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_AllocateVertices, sizeof(msg), &msg );
	if (res != MSG_HANDLED)
	{
		m_VertexBufferInterface = NULL;
		return false;
	}

	m_VertexBufferAllocation.m_Offset = msg.m_ReturnOffset;
	m_VertexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
	m_VertexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
	m_VertexBufferAllocation.m_Size = iVertexCount;
	m_VertexBufferAllocation.m_Stride = msg.SizeVertexFormat;
	m_VertexBufferInterface = (IVertexBufferObject*)msg.m_InterfacePointer;

	m_pGeometry->SetVertexBufferAllocation( m_VertexBufferAllocation );
	m_pDistantGeometry->SetVertexBufferAllocation( m_VertexBufferAllocation );

	static DWORD msgHash_TerrainSectorResetVertexData = CHashString(_T("TerrainSectorResetVertexData")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_TerrainSectorResetVertexData, 0, NULL, GetParentName() );

	return true;
}

bool CTerrainSectorRenderObject::AllocateIndices( int iIndexCount )
{
	if (m_IndexBufferAllocation.m_Size >= iIndexCount)
		return true;

	// remove previous allocation
	if (m_IndexBufferAllocation.m_Size != 0)
	{
		static DWORD msgHash_RemoveIndexAllocation = CHashString(_T("RemoveIndexAllocation")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveIndexAllocation, sizeof(m_IndexBufferAllocation), &m_IndexBufferAllocation );
		m_IndexBufferAllocation.m_Size = 0;
	}

	//create new allocation
	ALLOCATERENDERBUFFERMSG msg;
	msg.m_AllowSharedBuffer = false;
	msg.ElementsToAllocate = iIndexCount;
	msg.SizeVertexFormat = sizeof(unsigned short);

	static DWORD msgHash_AllocateIndices = CHashString(_T("AllocateIndices")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_AllocateIndices, sizeof( msg ), &msg, NULL, NULL );
	if( res != MSG_HANDLED )
	{
		m_IndexBufferInterface = NULL;
		return false;
	}

	m_IndexBufferAllocation.m_Offset = msg.m_ReturnOffset;
	m_IndexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
	m_IndexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
	m_IndexBufferAllocation.m_Size = iIndexCount;
	m_IndexBufferAllocation.m_Stride = sizeof(unsigned short);
	m_IndexBufferInterface = (IIndexBuffer*)msg.m_InterfacePointer;

	return true;
}

DWORD CTerrainSectorRenderObject::OnSetLightmapData( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( SETLIGHTMAPDATAMSG ) );
	SETLIGHTMAPDATAMSG * parms = (SETLIGHTMAPDATAMSG*)params;
	if( parms &&
		parms->cache &&
		m_VertexBufferInterface )
	{
		//for all geometry
		LightmapCache * cache = parms->cache;
		if( cache->m_bRemapped == true )//not remapped
		{
			//means we need to remap verts, don't do this for now, we're using straight projective texturing for lightmap
			//onto the sector since the sector really won't be that stretched, if it is, it will already show up
			//on the base textures so it's no worse off in quality already.
		}
		//grab uvs
		int iVertexResolution = m_pTerrainMgr->GetMetrics().iSectorResolution + 1; // resolution of vertices (samples)
		TERRAINVERTEX *pBaseVertex = NULL;
		int iVertexCount = iVertexResolution * iVertexResolution;
	
		if( iVertexCount != cache->m_NumVertices )
		{
			m_ToolBox->Log( LOGERROR, _T("Lightmap data for terrain does not match vertex data\n") );
			return MSG_ERROR;
		}
		if (m_VertexBufferInterface) {
			m_VertexBufferInterface->Lock( m_VertexBufferAllocation.m_Offset, iVertexCount, (void**)&pBaseVertex );
		}
		if (pBaseVertex && m_pVertexData)
		{
			TERRAINVERTEX *pVertex = pBaseVertex;
			int iVertex = 0;
			for (int y=0; y < iVertexResolution; y++)
			{
				for (int x=0; x < iVertexResolution; x++, pVertex++, iVertex++)
				{				
					int index = (y*iVertexResolution + x )*2;
					pVertex->lightmapuv[0] = cache->m_UVData[ index ];
					pVertex->lightmapuv[1] = cache->m_UVData[ index + 1 ];
				}
			}
		}
		if (m_VertexBufferInterface) {
			m_VertexBufferInterface->Unlock();
		}
		//Lightmap texture
		m_LightmapTexture = dynamic_cast< ITextureObject* >(parms->pTexture);
		bool bLightmaps = (m_LightmapTexture != NULL);
		m_pMaterial->SetEffect( m_pTerrainMgr->GetDetailEffect( bLightmaps ) );
	}
	return MSG_HANDLED_STOP;
}

DWORD CTerrainSectorRenderObject::OnGetGeometryAllocations( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETRENDERBUFFERALLOCATIONMSG ) );
	GETRENDERBUFFERALLOCATIONMSG * parms = (GETRENDERBUFFERALLOCATIONMSG *)params;
	
	if( parms )
	{
		//Loop through geometry and add allocations:
		RENDERBUFFERALLOCATION curAlloc;
		curAlloc.m_VertexAllocation = m_VertexBufferAllocation;
		curAlloc.m_IndexAllocation = m_IndexBufferAllocation;
		curAlloc.m_IndexBufferInterface = m_IndexBufferInterface;
		curAlloc.m_VertexBufferInterface = m_VertexBufferInterface;
		curAlloc.m_Material = NULL;
		parms->m_GeometryAllocationsOut.push_back( curAlloc );
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

DWORD CTerrainSectorRenderObject::OnGetLocalMatrixTransform( DWORD size, void * params )
{
	Matrix4x4 * matrix;
	VERIFY_MESSAGE_SIZE( size, sizeof( Matrix4x4 ) );
	matrix = (Matrix4x4 *)params;
	if( matrix )
	{
		Matrix4x4 worldTransform;
		worldTransform.SetIdentity();
		worldTransform.SetTranslation( m_Position );
		*matrix = worldTransform;
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

DWORD CTerrainSectorRenderObject::OnTerrainSetPath( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( IHashString ) );
	IHashString *pNewPath = (IHashString *)params;

	if (m_szMaskTexture0.GetString()[0] != '\0')
	{
		CHashString oldMaskTexture0 = m_szMaskTexture0;

		TCHAR szTemp[MAX_PATH];
		_stprintf( szTemp, _T("%s%s_mask0.tga"), pNewPath->GetString(), GetParentName()->GetString() );
		StdString szMaskTexture0(szTemp);
		szMaskTexture0.MakeSafeFileName();

		// path already correct for mask texture
		if (_tcsicmp(m_szMaskTexture0.GetString(), szMaskTexture0) == 0)
			return MSG_HANDLED_PROCEED;

		m_szMaskTexture0 = szMaskTexture0;

		// get old mask texture
		TEXTUREOBJECTPARAMS top;
		top.bLoad = false;
		top.TextureObjectInterface = NULL;
		top.Name = &oldMaskTexture0;
		static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
		DWORD retVal = m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(top), &top);

		// clone the mask texture
		if (top.TextureObjectInterface)
		{
			top.Name = &m_szMaskTexture0;
			static DWORD msgHash_AddTexture = CHashString(_T("AddTexture")).GetUniqueID();
			retVal = m_ToolBox->SendMessage(msgHash_AddTexture, sizeof(top), &top);
		}

		m_pMaskTexture0 = dynamic_cast<ITextureObject*>(top.TextureObjectInterface);

		// set the new name in the texture object
		if (m_pMaskTexture0)
		{
			m_pMaskTexture0->SetTextureName( &m_szMaskTexture0 );
		}

		// remove the old reference by name
		top.Name = &oldMaskTexture0;
		top.TextureObjectInterface = NULL;
		static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
		retVal = m_ToolBox->SendMessage(msgHash_RemoveTexture, sizeof(top), &top);
	}

	return MSG_HANDLED_PROCEED;
}

IArchive* CTerrainSectorRenderObject::ConvertTerrainArchive( IArchive &ar )
{
	float fVersion = 0;
	ar.Read( fVersion );
	if (fVersion < m_fCurrentVersion)
	{
		if (fVersion == 1.0f)
		{
			CHashString streamType(_T("Memory"));
			CREATEARCHIVE ca;
			ca.mode = STREAM_MODE_READ | STREAM_MODE_WRITE | STREAM_MODE_BINARY;
			ca.streamData = NULL;
			ca.streamType = &streamType;
			ca.streamSize = 0;
			static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
			if( m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED )
			{
				return NULL;
			}

			IArchive *pArchive = ca.archive;
			pArchive->SetIsWriting( true );


			int iSectorX, iSectorY, iLayerCount = 0;
			float fBaseTextureScaleX, fBaseTextureScaleY;
			StdString szBaseTexture;

			// read first portion of old version
			ar.Read( iSectorX );
			ar.Read( iSectorY );
			ar.Read( szBaseTexture );
			ar.Read( fBaseTextureScaleX );
			ar.Read( fBaseTextureScaleY );
			ar.Read( iLayerCount );

			if (iLayerCount > 0)
				CreateDefaultMaskTexture(0);

			// write first portion of new version
			pArchive->Write( iSectorX );
			pArchive->Write( iSectorY );
			pArchive->Write( szBaseTexture );
			pArchive->Write( fBaseTextureScaleX );
			pArchive->Write( fBaseTextureScaleY );
			pArchive->Write( m_szMaskTexture0.GetString() );
			pArchive->Write( iLayerCount );

			if (iLayerCount > 0)
			{
				int nSamples = m_pMaskTexture0->GetWidth() * m_pMaskTexture0->GetWidth();
				unsigned char *pMaskTextureData = m_StaticBuffers.GetTempSectorByteTextureBuffer( nSamples * 4 );
				memset( pMaskTextureData, 0, nSamples * 4 );

				for (int iLayer=0; iLayer < iLayerCount; iLayer++)
				{
					StdString szMaskTextureName;
					StdString szSplatTextureName;
					float fSplatTextureScaleX;
					float fSplatTextureScaleY;

					// read a layer from old version
					ar.Read( szMaskTextureName );
					ar.Read( szSplatTextureName );
					ar.Read( fSplatTextureScaleX );
					ar.Read( fSplatTextureScaleY );

					if (iLayer < 4)
					{
						// write a layer to new version
						pArchive->Write( szSplatTextureName );
						pArchive->Write( fSplatTextureScaleX );
						pArchive->Write( fSplatTextureScaleY );

						// write layer mask data to new mask texture
						ITextureObject *pLayerMask = dynamic_cast<ITextureObject*>( LoadTexture( szMaskTextureName ) );
						if (pLayerMask)
						{
							unsigned char *pLayerMaskData = (unsigned char*)pLayerMask->Read();
							for (int i=0; i < nSamples; i++)
							{
								pMaskTextureData[i*4+iLayer] = pLayerMaskData[i];
							}
						}
					}
				}

				// write mask texture data
				m_pMaskTexture0->Write( pMaskTextureData );
			}

			// reset archive for reading
			pArchive->SeekTo( 0 );
			pArchive->SetIsWriting( false );
			return pArchive;
		}
	}
	return NULL;
}

void CTerrainSectorRenderObject::AttachLight( ILightObject * light )
{	
	if (light->GetLightType() == AMBIENT_LIGHT)
	{
		m_pGeometry->AttachLight( light );
	}
	else
	{
		float fPriority = 0.0f;
		bool bAffected = light->GetLightPriority( m_CenterPosition, m_fBoundingSphereRadius, fPriority );
		if( !bAffected )
			return;

		// find light slot to insert
		int i = 0;
		for( i = 0; i < m_NumLights; i++ )
		{
			// push back the rest of the lights
			if( (m_LightPriorities[ i ] < fPriority) )
			{
				for( int j = MAX_LIGHTS - 1 ; j > i ; j-- )
				{
					m_Lights[ j ] = m_Lights[ j-1 ];
					m_LightPriorities[ j ] = m_LightPriorities[ j-1 ];
				}
				break;
			}
		}

		// if no slot available
		if (i == MAX_LIGHTS)
			return;

		// insert light
		m_Lights[ i ] = light;
		m_LightPriorities[ i ] = fPriority;
		if (m_NumLights < MAX_LIGHTS)
			m_NumLights++;

		// set lights in geometry in priority correct order
		m_pGeometry->ResetLightCount();
		for (int a=0; a < m_NumLights; a++)
			m_pGeometry->AttachLight( m_Lights[a], a );

		// update scene settings
		if( m_SceneSettings )
		{
			if( m_SceneSettings->GetShaderFlags() & (SHADOW_RECEIVE) )
			{
				//get highest priority light that can cast shadows:
				for( int a = 0; a < m_NumLights; a++ )
				{
					ILightObject * curLight = m_Lights[ a ];
					if( curLight && curLight->CanCastShadows() )
					{
						//clear receiver flags
						m_SceneSettings->RemoveShaderFlag( SHADOW_RECEIVE_HEMI | SHADOW_RECEIVE_OMNI |
												SHADOW_RECEIVE_DIR );
						//check what type of light is affecting it:
						if( curLight->GetLightType() == OMNI_POINT_LIGHT )
						{
							m_SceneSettings->AddShaderFlag( SHADOW_RECEIVE_OMNI );
						}
						else if( curLight->GetLightType() == HEMI_POINT_LIGHT )
						{
							m_SceneSettings->AddShaderFlag( SHADOW_RECEIVE_HEMI );
						}
						else if( curLight->GetLightType() == DIRECTIONAL_LIGHT )
						{
							m_SceneSettings->AddShaderFlag( SHADOW_RECEIVE_DIR );
						}	
						break;
					}
				}
			}
		}
	}
}

void CTerrainSectorRenderObject::DetachAllLights()
{
	m_NumLights = 0;
	memset( m_Lights, 0, sizeof( m_Lights ) );
	memset( m_LightPriorities, 0, sizeof( m_LightPriorities ) );

	m_pGeometry->DetachAllLights();
}

void CTerrainSectorRenderObject::GenerateTangent( TERRAINVERTEX *pTerrainVertices, unsigned short v0, unsigned short v1, unsigned short v2 )
{
	// Step 1. Compute the approximate tangent vector.
	float du1 = pTerrainVertices[v1].masktex[0] - pTerrainVertices[v0].masktex[0];
	float dv1 = pTerrainVertices[v1].masktex[1] - pTerrainVertices[v0].masktex[1];
	float du2 = pTerrainVertices[v2].masktex[0] - pTerrainVertices[v0].masktex[0];
	float dv2 = pTerrainVertices[v2].masktex[1] - pTerrainVertices[v0].masktex[1];

	float prod1 = (du1*dv2-dv1*du2);
	float prod2 = (du2*dv1-dv2*du1);
	if ((fabs(prod1) < 0.000001)||(fabs(prod2) < 0.000001))
	{
		return;
	}

	float x = dv2/prod1;
	float y = dv1/prod2;

	Vec3 vec1 = pTerrainVertices[v1].pos - pTerrainVertices[v0].pos;
	Vec3 vec2 = pTerrainVertices[v2].pos - pTerrainVertices[v0].pos;
	Vec3 tangent = (vec1 * x) + (vec2 * y);

	// Step 2. Orthonormalize the tangent.
	float component = tangent.Dot(pTerrainVertices[v0].normal);
	tangent -= (pTerrainVertices[v0].normal * (component));
	tangent.Normalize();

	// Step 3: Add the estimated tangent to the overall estimate for the vertex.
	pTerrainVertices[v0].tangent += tangent;
}

void CTerrainSectorRenderObject::GenerateTangents( TERRAINVERTEX *pTerrainVertices, int iVertexCount, unsigned short *pTerrainIndices, int iIndexCount )
{
/*	for (int i=0; i < iIndexCount; i+=3)
	{
		GenerateTangent( pTerrainVertices, pTerrainIndices[i+0], pTerrainIndices[i+1], pTerrainIndices[i+2] );
		GenerateTangent( pTerrainVertices, pTerrainIndices[i+1], pTerrainIndices[i+2], pTerrainIndices[i+0] );
		GenerateTangent( pTerrainVertices, pTerrainIndices[i+2], pTerrainIndices[i+0], pTerrainIndices[i+1] );
	}

	for (int i=0; i < iVertexCount; i++)
	{
		pTerrainVertices[i].tangent.Normalize();
	}
*/

	for (int i=0; i < iVertexCount; i++)
	{
		pTerrainVertices[i].tangent = pTerrainVertices[i].normal.Cross( Vec3(0,0,1) );
		pTerrainVertices[i].tangent.Normalize();
	}
}

IMaterial* CTerrainSectorRenderObject::CreateMaterial( const TCHAR *pszSuffix )
{
	// material
	StdString szMaterialBaseName( GetName()->GetString() );
	szMaterialBaseName += pszSuffix;
	szMaterialBaseName.tolower();
	CHashString hszMaterialBaseName(szMaterialBaseName);
	CHashString hszMaterialName;
	GENERATEUNIQUEOBJECTNAMEPARAMS generateNameParams;
	generateNameParams.newname = &hszMaterialName;
	generateNameParams.name = &hszMaterialBaseName;
	static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(generateNameParams), &generateNameParams ) != MSG_HANDLED)
		return NULL;

	CHashString hszMaterialType( _T("CMaterial" ) );
	CREATEOBJECTPARAMS cop;
	cop.parentName = NULL;
	cop.typeName = &hszMaterialType;
	cop.name = &hszMaterialName;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop) != MSG_HANDLED)
		return NULL;

	CREATEMATERIALINTERFACE cmat;
	cmat.m_Name = &hszMaterialName;
	static DWORD msgHash_GetMaterialInterface = CHashString(_T("GetMaterialInterface")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetMaterialInterface, sizeof(cmat), &cmat) != MSG_HANDLED)
		return NULL;

	return cmat.m_MaterialInterface;
}
