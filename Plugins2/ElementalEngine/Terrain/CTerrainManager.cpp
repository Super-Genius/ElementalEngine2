///==========================================================================
/// \file	CTerrainManager.cpp
/// \brief	Implementation of Terrain
/// \date	03-24-2005
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TERRAINPRIORITY     0x0580

#define DEFAULT_HEIGHTMAP_SIZE 64

TOLUA_API int tolua_TerrainScript_open (lua_State* tolua_S);

REGISTER_COMPONENT_SINGLETON( CTerrainManager );
REGISTER_MESSAGE_HANDLER(TerrainDeform, OnTerrainDeform, CTerrainManager);
REGISTER_MESSAGE_HANDLER(GetAffectedTerrainSectors, OnGetAffectedTerrainSectors, CTerrainManager);
REGISTER_MESSAGE_HANDLER(SetTerrainConfig, OnSetTerrainConfig, CTerrainManager);
REGISTER_MESSAGE_HANDLER(TerrainManagerCreateTerrain, OnCreateTerrain, CTerrainManager );
REGISTER_MESSAGE_HANDLER(TerrainManagerModifyTerrain, OnModifyConfig, CTerrainManager );
REGISTER_MESSAGE_HANDLER(GetTerrainDescriptor, OnGetTerrainDescriptor, CTerrainManager );
REGISTER_MESSAGE_HANDLER(GetLODDistance, OnGetLODDistance, CTerrainManager );
REGISTER_MESSAGE_HANDLER(GetTerrainSectorNeighborInfo, OnGetTerrainSectorNeighborInfo, CTerrainManager );
REGISTER_MESSAGE_HANDLER(Start, OnStart, CTerrainManager);
REGISTER_MESSAGE_HANDLER(Stop, OnStop, CTerrainManager);
REGISTER_MESSAGE_HANDLER(CreateTerrainDecal, OnCreateTerrainDecal, CTerrainManager);

IComponent *CTerrainManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CTerrainManager );
}

CTerrainManager::CTerrainManager() : 
	CManager( _T("CTerrainManager"), TERRAINPRIORITY, TERRAINPRIORITY ),
	m_HeightMapClass(_T("CHeightmapObject")),
	m_TerrainSectorClass(_T("CTerrainSector")), 
	m_TerrainSectorRenderObjectClass(_T("CTerrainSectorRenderObject")),
	m_TerrainDecalRenderObjectClass(_T("CTerrainDecalRenderObject"))
{
	m_bIsRunning = true;
	m_iSectorLODUpdateTimer = 0;
	m_pDetailEffect = NULL;
	m_pDetailEffectLightmap = NULL;
	m_pDistantEffect = NULL;
	
	m_BaseTextureStage = 0;
	m_MaskTextureStageStart = 0;
	m_LightmapTextureStage = 0;

	static DWORD msgHash_GetMasterScriptState = CHashString(_T( "GetMasterScriptState" )).GetUniqueID();
	DWORD amessageHandle = m_ToolBox->SendMessage(msgHash_GetMasterScriptState, sizeof( lua_State * ), &m_LuaState );
	if( amessageHandle != MSG_HANDLED)
	{
		StdString error;
		error = _T("Error missing Master Script State Data\n");
	
		// log error
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
		EngineGetToolBox()->Log(LOGWARNING, error);
	}
	else
	{
		// register our additional structures/handlers with LUA master
		tolua_TerrainScript_open(m_LuaState);
	}

	Init();
}

CTerrainManager::~CTerrainManager()
{
	FreeTerrain();
}

bool CTerrainManager::Init()
{
	m_iSectorResolution = 0;
	m_fSectorSize = 0;
	m_fHeightMapZScale = 0;
	m_fLODDistance = 0;

	m_bLightTerrain = false;

	return CManager::Init();
}

void CTerrainManager::Update(DWORD tickCount)
{
	if (m_bIsRunning)
	{
		if (m_iSectorLODUpdateTimer < (int)GetTickCount())
		{
			// Update LOD on all sectors
			IDTOOBJECTMAP *pSectorList = GetObjectMap(&m_TerrainSectorClass);
			if( pSectorList && !pSectorList->empty() )
			{
				for (IDTOOBJECTMAP::iterator iter = pSectorList->begin();
					iter != pSectorList->end(); iter++)
				{
					IObject *sectorObject = iter->second;
					IHashString *curname = sectorObject->GetName();
					static DWORD msgHash_TerrainSectorUpdateLOD = CHashString(_T("TerrainSectorUpdateLOD")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_TerrainSectorUpdateLOD, 0, NULL, curname );
				}
			}
			m_iSectorLODUpdateTimer = GetTickCount() + 200;
		}

		// run update on all managed types
		MAPGROUPLISTS *pGroups = GetGroupList();
		for (MAPGROUPLISTS::iterator itrGroup = pGroups->begin(); itrGroup != pGroups->end(); itrGroup++)
		{
			IDTOOBJECTMAP *pObjectList = itrGroup->second;
			if( pObjectList && !pObjectList->empty() )
			{
				for (IDTOOBJECTMAP::iterator iter = pObjectList->begin(); iter != pObjectList->end(); iter++)
				{
					IObject *object = iter->second;
					object->Update();
				}
			}
		}
	}
}

DWORD CTerrainManager::PumpHandler(DWORD size, void *data, IHashString *name, IHashString *compType, MSGFUNCTION msgFunction)
{
	// only pump messages for certain types
	static CHashString chmoTypeName(_T("CHeightmapObject"));
	static CHashString ctdroTypeName(_T("CTerrainDecalRenderObject"));
	if (compType)
	{
		if (*compType == chmoTypeName || 
			*compType == ctdroTypeName)
			return CManager::PumpHandler(size,data,name,compType,msgFunction);
	}
	return MSG_NOT_HANDLED;
}

DWORD CTerrainManager::OnSetTerrainConfig(DWORD size, void *in_params)
{	
	CTerrainConfig *pConfig = (CTerrainConfig*)in_params;
	m_iSectorResolution = pConfig->GetSectorResolution();
	m_fSectorSize       = pConfig->GetSectorSize();
	m_fHeightMapZScale  = pConfig->GetHeightMapZScale();
	m_fLODDistance		= pConfig->GetLODDistance();

	// enforce power of two resolution (required for fast lod)
	m_iSectorResolution = g_Math.NearestPowerOfTwo( m_iSectorResolution );

	// clamp a min resolution
	if (m_iSectorResolution < 8)
		m_iSectorResolution = 8;

	m_ConfigName.Init( pConfig->GetName()->GetString() );
	return MSG_HANDLED_PROCEED;
}

bool CTerrainManager::CalculateTerrainAABSquare( int &XStart, int &YStart, int &XEnd, int &YEnd )
{
	IDTOOBJECTMAP *pSectorList = GetObjectMap(&m_TerrainSectorClass);

	//Now that the config is modified, we have to go through our sectors and delete
	//as necessary, resize, and create new sectors as necessary
	if( pSectorList && !pSectorList->empty() )
	{
		XStart = 0x7fffffff;
		YStart = 0x7fffffff;
		XEnd = 0xffffffff;
		YEnd = 0xffffffff;
		TERRAINSECTORPOSITIONPARAMS cursectpos;
		IDTOOBJECTMAP::iterator iter = pSectorList->begin();
		for (;iter != pSectorList->end(); iter++)
		{
			IObject *sectorObject = iter->second;
			IHashString * curname = sectorObject->GetName();
			//find out if this sector is still needed given our sector dimensions
			static DWORD msgHash_TerrainSectorGetPosition = CHashString(_T("TerrainSectorGetPosition")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_TerrainSectorGetPosition, sizeof(TERRAINSECTORPOSITIONPARAMS ),
				&cursectpos, curname, &m_TerrainSectorClass);
			//set old positions so we know what to add
			XEnd = XEnd > cursectpos.SectorX ? XEnd : cursectpos.SectorX;
			YEnd = YEnd > cursectpos.SectorY ? YEnd : cursectpos.SectorY;
			//set old positions so we know what to add
			XStart = XStart < cursectpos.SectorX ? XStart : cursectpos.SectorX;
			YStart = YStart < cursectpos.SectorY ? YStart : cursectpos.SectorY;
		}
		return true;
	}
	else
	{
		return false;
	}
}

void CalculateResizeGrowth( bool pGrowthX, bool pGrowthY, bool nGrowthX, bool nGrowthY,
						   int &PositiveGrowthX, int &PositiveGrowthY, int &NegativeGrowthX, 
						   int &NegativeGrowthY, int GrowthX, int GrowthY )
{
	//calculate which way to grow
	if( !pGrowthX && nGrowthX  )
	{
		PositiveGrowthX = 0;
		NegativeGrowthX = -GrowthX;
	}
	else if( pGrowthX && !nGrowthX )
	{
		NegativeGrowthX = 0;
		PositiveGrowthX = GrowthX;
	}else
	{
		NegativeGrowthX = -( GrowthX >> 1 );
		PositiveGrowthX = GrowthX + NegativeGrowthX;
	}
	//y dir now
	if( !pGrowthY && nGrowthY  )
	{
		PositiveGrowthY = 0;
		NegativeGrowthY = -GrowthY;
	}
	else if( pGrowthY && !nGrowthY )
	{
		NegativeGrowthY = 0;
		PositiveGrowthY = GrowthY;
	}else
	{
		NegativeGrowthY = -( GrowthY >> 1 );
		PositiveGrowthY = GrowthY + NegativeGrowthY;
	}
}

DWORD CTerrainManager::OnModifyConfig(DWORD size, void *in_params)
{
	static CHashString hszEntityName( _T("TerrainEntity") );
	int GrowthX;
	int GrowthY;
	int PositiveGrowthX;
	int PositiveGrowthY;
	int NegativeGrowthX;
	int NegativeGrowthY;
	RECT NewTerrainBounds;
	int PreviousTerrainStartX;
	int PreviousTerrainStartY;
	int PreviousTerrainEndX;
	int PreviousTerrainEndY;
	int OldTerrainWidth;
	int OldTerrainHeight;
	TERRAINCREATIONPARAMS * params; 
	VERIFY_MESSAGE_SIZE(size, sizeof( TERRAINCREATIONPARAMS ) );
	params = (TERRAINCREATIONPARAMS*)in_params;
	
	if( m_iSectorResolution == 0 )//means we don't have a terrain to begin with!
	{
		OnCreateTerrain( size, in_params );
	}
	else
	{	//modify terrain	
		//first thing we do is modify the config
		if( params->TerrainWidth == 0 
			|| params->TerrainHeight == 0 
			|| params->SectorSize == 0 
			|| params->SectorResolution == 0 )
		{
			return MSG_ERROR;
		}
        
		m_BaseTextureScaleX = params->BaseTextureScaleX;
		m_BaseTextureScaleY = params->BaseTextureScaleY;
		CHashString hszTypeName( _T("CTerrainConfig") );
		CHashString hszName( _T("TerrainConfig") );

		IArchive *MemArchive;	
		CHashString memType(_T("Memory"));

		CREATEARCHIVE ca;
		ca.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
		ca.streamData = NULL;
		ca.streamSize = 0;
		ca.streamType = &memType;
		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
		{
			return MSG_NOT_HANDLED;
		}

		MemArchive = ca.archive;
		MemArchive->Write( _T("") );
		MemArchive->Write( _T("") );
		MemArchive->Write( params->SectorResolution );
		MemArchive->Write( params->SectorSize );
		MemArchive->Write( params->ZScale );
		MemArchive->Write( params->LODDistance );

		SERIALIZEOBJECTPARAMS sop;
		sop.name = &hszName;
		sop.archive = MemArchive;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);

		INITOBJECTPARAMS iop;
		iop.name = &hszName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);

		//used for resizing to a new center
		if( !CalculateTerrainAABSquare( PreviousTerrainStartX, PreviousTerrainStartY, PreviousTerrainEndX, PreviousTerrainEndY ) )
		{ return MSG_NOT_HANDLED; }


		//plus one because coordinates are [ ] inclusive
		OldTerrainWidth = PreviousTerrainEndX - PreviousTerrainStartX + 1;
		OldTerrainHeight = PreviousTerrainEndY - PreviousTerrainStartY + 1;
		assert( OldTerrainWidth >= 0 && OldTerrainHeight >= 0 );
		GrowthX= ( params->TerrainWidth - OldTerrainWidth );
		GrowthY = ( params->TerrainHeight - OldTerrainHeight );	
		CalculateResizeGrowth( params->PositiveResizeX, params->PositiveResizeY, params->NegativeResizeX,
			params->NegativeResizeY, PositiveGrowthX, PositiveGrowthY, NegativeGrowthX, NegativeGrowthY ,
			GrowthX, GrowthY );
		//calculate new terrain bounds:
		NewTerrainBounds.left = PreviousTerrainStartX + NegativeGrowthX;
		NewTerrainBounds.right = PreviousTerrainEndX + PositiveGrowthX;
		NewTerrainBounds.top = PreviousTerrainStartY + NegativeGrowthY;
		NewTerrainBounds.bottom = PreviousTerrainEndY + PositiveGrowthY;
		//remember, we're adding one because these are inclusive endpoints
		//and +1 determines full width
		assert(  (NewTerrainBounds.right - NewTerrainBounds.left + 1 )== params->TerrainWidth  
			&&   (NewTerrainBounds.bottom - NewTerrainBounds.top + 1)== params->TerrainHeight  );
		
        //Now that the config is modified, we have to go through our sectors and delete
		//as necessary, resize, and create new sectors as necessary
		IDTOOBJECTMAP *pSectorList = GetObjectMap(&m_TerrainSectorClass);
		if( pSectorList && !pSectorList->empty() )
		{
			TERRAINSECTORPOSITIONPARAMS cursectpos;
			DELETEOBJECTPARAMS dop;
			IDTOOBJECTMAP::iterator iter = pSectorList->begin();
			std::set<IHashString * > CheckedSectors;
			for (;iter != pSectorList->end();)
			{
				IObject *sectorObject = iter->second;
				IHashString * curname = sectorObject->GetName();
				//find out if this sector is still needed given our sector dimensions
				static DWORD msgHash_TerrainSectorGetPosition = CHashString(_T("TerrainSectorGetPosition")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_TerrainSectorGetPosition, sizeof(TERRAINSECTORPOSITIONPARAMS ),
					&cursectpos, curname, &m_TerrainSectorClass );
				//test if this sector is still valid within new terrain bounds
				if( cursectpos.SectorX <= NewTerrainBounds.right
					&& cursectpos.SectorX >= NewTerrainBounds.left
					&& cursectpos.SectorY <= NewTerrainBounds.bottom
					&& cursectpos.SectorY >= NewTerrainBounds.top )
				{
					//valid sector
					if( CheckedSectors.find( curname ) == CheckedSectors.end() )
					{
						// set dirty transform on sector
						static DWORD msgHash_SetDirtyTransformFlag = CHashString("SetDirtyTransformFlag").GetUniqueID();
						m_ToolBox->SendMessage(msgHash_SetDirtyTransformFlag, 0, NULL, curname );
						//modify the properties of this terrain sector
						StdString ssCurName( curname->GetString());
						ModifyTerrainSectorRenderObject( ssCurName, params->TerrainBaseTexture );
						CheckedSectors.insert( curname );
					}
					iter++;
				}
				else
				{
					//sector no longer used, delete
					dop.name = curname;
					static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof( DELETEOBJECTPARAMS ), &dop, NULL, NULL );
					iter = pSectorList->begin();				
				}
			}
		}
		
		//looping through new sectors to build
		for( int i = NewTerrainBounds.left; i <= NewTerrainBounds.right ; i++ )
		{
			for( int j = NewTerrainBounds.top; j <= NewTerrainBounds.bottom; j ++ )
			{
				//create sectors that weren't created before
				if( i < PreviousTerrainStartX
					|| i > PreviousTerrainEndX
					|| j < PreviousTerrainStartY
					|| j > PreviousTerrainEndY )
				{
					BuildTerrainHeightmap( &hszEntityName, MemArchive, params->TerrainPath, params->TerrainName, i, j, params->SectorResolution );
					CHashString *sectorname = BuildTerrainSector( &hszEntityName, MemArchive, params->TerrainPath, params->TerrainName, params->TerrainBaseTexture, i, j, params->SectorSize );
					//find out if this sector is a boundary sector
					//find out what sector to stitch with
					int XStichIndex = i;
					int YStichIndex = j;
					if( i == (PreviousTerrainStartX - 1 ) ||
						i == (PreviousTerrainEndX + 1 ) )
					{
						XStichIndex = (PreviousTerrainStartX - i);
						if( XStichIndex != 0 ){
							XStichIndex = ( XStichIndex > 0 ) ? 1 : -1; }//normalize 
						XStichIndex += i;
					}

						if( j == (PreviousTerrainStartY - 1) || 
							j == (PreviousTerrainEndY + 1 ) )
					{
						YStichIndex = (PreviousTerrainStartY- j);
						if( YStichIndex != 0 ){
							YStichIndex = ( YStichIndex > 0 ) ? 1 : -1; }//normalize 
						YStichIndex += j;
					}
					if( !(YStichIndex == j && XStichIndex == i) )
					{
						IHashString * StitchSector =  GetSectorName( XStichIndex, YStichIndex );
						StitchSectors( sectorname, i, j, StitchSector, XStichIndex, YStichIndex );
					}
				}
			}
		}

		MemArchive->Close();
		return MSG_HANDLED_PROCEED;
	}
	return MSG_NOT_HANDLED;
}

__forceinline float localexp2( float a )
{
	return a*a;
}

__forceinline float Falloff( float SourceHeight, float DestHeight, float Distance )
{
	if( Distance < .03f )
		return SourceHeight;
	return (SourceHeight*(1-Distance) + DestHeight*Distance);
}

void CTerrainManager::StitchSectors( IHashString * toStitch, int sourcex, int sourcey,
									IHashString * withStitch, int destx, int desty )
{
	HEIGHTMAPDATAPARAMS referencestitchdata;
	HEIGHTMAPDATAPARAMS stitchdata;
	static DWORD msgHash_TerrainSectorGetData = CHashString(_T("TerrainSectorGetData")).GetUniqueID();
	if( m_ToolBox->SendMessage(msgHash_TerrainSectorGetData, sizeof( HEIGHTMAPDATAPARAMS ), &referencestitchdata, withStitch, NULL ) != MSG_HANDLED
		||	m_ToolBox->SendMessage(msgHash_TerrainSectorGetData, sizeof( HEIGHTMAPDATAPARAMS ), &stitchdata, toStitch, NULL ) != MSG_HANDLED )
	{
		//no stitch
		return;
	}

	int EdgeSize = ( sourcex != destx 
					&& sourcey != desty ) ? 1 : m_iSectorResolution;
	Vec3 * Edge = new Vec3[ EdgeSize ];
	int sourceHmapX, sourceHmapY; // [0, m_iSectorResolution]
	int xloc = Clamp( sourcex - destx, 0, 1 );	
	int yloc = Clamp( sourcey - desty, 0, 1 );
	sourceHmapX = xloc*(m_iSectorResolution-1);
	sourceHmapY = yloc*(m_iSectorResolution-1);
	float * pRefStitch =(float*) referencestitchdata.pData;
	if( EdgeSize > 1 )
	{
		//find axis, copy along y
        if( sourcey == desty )
		{
            for( int k = 0; k < m_iSectorResolution; k++ )
			{
				Edge[ k ] = Vec3(
					(float)(sourceHmapX + destx*m_iSectorResolution),
					(float)(sourceHmapY + k + desty*m_iSectorResolution),
					pRefStitch[ sourceHmapX + k*m_iSectorResolution  ] );
			}
		}//copy along x
		else
		{
			for( int k = 0; k < m_iSectorResolution; k++ )
			{
				Edge[ k ] = Vec3(
					(float)(sourceHmapX + k + destx*m_iSectorResolution),
					(float)(sourceHmapY + desty*m_iSectorResolution),
					pRefStitch[ sourceHmapY*m_iSectorResolution + k  ] );
			}
		}
		
	}
	else//copy heightmap data at hampx, y to Edge
	{
		Edge[0] = Vec3( (float)(sourceHmapX + destx*m_iSectorResolution),
						(float)(sourceHmapY + desty*m_iSectorResolution),
						pRefStitch[ ( sourceHmapX) + ( sourceHmapY)*m_iSectorResolution ] );		
	}
	float * pdestStitchData = (float*) stitchdata.pData;
	//loop through all values of to be modified stich map

	for( int i = 0; i < m_iSectorResolution; i++ )
	{
		for( int j = 0; j < m_iSectorResolution; j++ )
		{
			
			//i and j are current pixels to modify
			Vec3 CurPos( (float)(i + sourcex*m_iSectorResolution),(float)( j + sourcey*m_iSectorResolution), 0 );
			Vec3 RefPos;
			if( EdgeSize > 1)
			{
				RefPos = ( sourcex == destx ) ? Edge[ i ] : Edge[ j ];
			}else
			{
				RefPos = Edge[ 0 ];
			}	
			float distance =  sqrtf( localexp2( (CurPos.x - RefPos.x) ) + localexp2( (CurPos.y - RefPos.y) ) );
			distance /= m_iSectorResolution;
			float curval = pdestStitchData[ i + j*m_iSectorResolution ];
			
			pdestStitchData[ i + j*m_iSectorResolution ] = Falloff( RefPos.z, curval, distance );
			//clamp:
			if( RefPos.z > curval )
			{
				if( pdestStitchData[ i + j*m_iSectorResolution ] < curval )
				{
					pdestStitchData[ i + j*m_iSectorResolution ] = curval;
				}
			}
			else if( RefPos.z < curval )
			{
				if( pdestStitchData[ i + j*m_iSectorResolution ] > curval )
				{
					pdestStitchData[ i + j*m_iSectorResolution ] = curval;
				}
			}
		}
	}
	static DWORD msgHash_TerrainSectorSetData = CHashString(_T("TerrainSectorSetData")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_TerrainSectorSetData, sizeof( HEIGHTMAPDATAPARAMS ), &stitchdata, toStitch, NULL );

	delete[] Edge;
}

//Need to optmize this
IHashString* CTerrainManager::GetSectorName( int X, int Y )
{
	IDTOOBJECTMAP *pSectorList = GetObjectMap(&m_TerrainSectorClass);

	if( pSectorList && !pSectorList->empty() )
	{
		TERRAINSECTORPOSITIONPARAMS cursectpos;
		IDTOOBJECTMAP::iterator iter = pSectorList->begin();
		for (;iter != pSectorList->end(); iter++)
		{
			IObject *sectorObject = iter->second;
			IHashString * curname = sectorObject->GetName();
			//find out if this sector is still needed given our sector dimensions
			static DWORD msgHash_TerrainSectorGetPosition = CHashString(_T("TerrainSectorGetPosition")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_TerrainSectorGetPosition, sizeof(TERRAINSECTORPOSITIONPARAMS ),
				&cursectpos, curname, &m_TerrainSectorClass );
			//test if this sector is still valid within new terrain bounds
			if( cursectpos.SectorX == X
				&& cursectpos.SectorY == Y )
			{
				return curname;
			}
		}
	}
	return NULL;
}

DWORD CTerrainManager::OnGetLODDistance(DWORD size, void *in_params)
{
	float* tmp = (float*)(in_params);
    *tmp = m_fLODDistance;

	return MSG_HANDLED_STOP;
}

TerrainMetrics_t CTerrainManager::GetMetrics()
{
	// calculate terrain metrics
	TerrainMetrics_t metrics;
	metrics.fSectorSize = m_fSectorSize;
	metrics.iSectorResolution = m_iSectorResolution;
	metrics.fZScale = m_fHeightMapZScale;
	metrics.fLODDistance = m_fLODDistance;
	return metrics;
}

void CTerrainManager::SetSectorExistance( IHashString *pSectorName, int iSectorX, int iSectorY, bool bExists )
{
	if (bExists)
	{
		DWORD sectorName = pSectorName->GetUniqueID();
		int iSectorIndex = GetTerrainSectorIndex( iSectorX, iSectorY );
		SECTORINDEXTONAMEMAP::iterator oldSectorNameItr = m_SectorIndexToNameMap.find(iSectorIndex);
		SECTORNAMETOINDEXMAP::iterator oldSectorIndexItr = m_SectorNameToIndexMap.find(sectorName);
		// remove old mapping from name to index
		if (oldSectorIndexItr != m_SectorNameToIndexMap.end())
			m_SectorNameToIndexMap.erase( oldSectorIndexItr );
		// remove old mapping from index to name
		if (oldSectorNameItr != m_SectorIndexToNameMap.end())
			m_SectorIndexToNameMap.erase( oldSectorNameItr );
		// add to sector map
		m_SectorIndexToNameMap[iSectorIndex] = pSectorName;
		m_SectorNameToIndexMap[sectorName] = iSectorIndex;
	}
	else
	{
		DWORD sectorName = pSectorName->GetUniqueID();
		int iSectorIndex = GetTerrainSectorIndex( iSectorX, iSectorY );
		SECTORINDEXTONAMEMAP::iterator oldSectorNameItr = m_SectorIndexToNameMap.find(iSectorIndex);
		SECTORNAMETOINDEXMAP::iterator oldSectorIndexItr = m_SectorNameToIndexMap.find(sectorName);
		// remove old mapping from name to index
		if (oldSectorIndexItr != m_SectorNameToIndexMap.end())
			m_SectorNameToIndexMap.erase( oldSectorIndexItr );
		// remove old mapping from index to name
		if (oldSectorNameItr != m_SectorIndexToNameMap.end())
			m_SectorIndexToNameMap.erase( oldSectorNameItr );
	}
}

bool CTerrainManager::TerrainDeform( BRUSHPARAMS &brush )
{
	IDTOOBJECTMAP *pObjMap = GetObjectMap(&m_TerrainSectorClass);
	if (!pObjMap)
		return false;

	list<IHashString*> sectorList;
	if (!GetAffectedTerrainSectors( sectorList, brush.m_fSize*0.5f, *brush.m_pPosition ))
		return false;

	float fAvgHeight = 0;
	if (brush.m_eBrushOp == BRUSH_OP_SMOOTH)
	{
		TERRAINBRUSHAREAINFO info;
		info.m_Brush = brush;
		TerrainGetBrushAreaInfo( info );
		fAvgHeight = info.fAverageValue;
	}

	for (list<IHashString*>::iterator itr=sectorList.begin(); itr != sectorList.end(); itr++)
	{
		IDTOOBJECTMAP::iterator itrMap = pObjMap->find( (*itr)->GetUniqueID() );
		if (itrMap != pObjMap->end())
		{
			CTerrainSector *pSector = (CTerrainSector*)itrMap->second;
			pSector->Deform( brush, fAvgHeight );
		}
	}
	return true;
}

bool CTerrainManager::TerrainGetBrushAreaInfo( TERRAINBRUSHAREAINFO &info )
{
	list<IHashString*> sectorList;
	if (!GetAffectedTerrainSectors( sectorList, info.m_Brush.m_fSize*0.5f, *info.m_Brush.m_pPosition ))
		return false;

	int iValidSectorCount = 0;
	info.fAverageValue = 0;
	for (list<IHashString*>::iterator itr=sectorList.begin(); itr != sectorList.end(); itr++)
	{
		IHashString *pSectorName = *itr;
		TERRAINBRUSHAREAINFO sectorInfo;
		sectorInfo.fAverageValue = 0;
		sectorInfo.m_Brush = info.m_Brush;
		static DWORD msgHash_TerrainSectorGetBrushAreaInfo = CHashString(_T("TerrainSectorGetBrushAreaInfo")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_TerrainSectorGetBrushAreaInfo, sizeof(sectorInfo), &sectorInfo, pSectorName ) == MSG_HANDLED)
		{
			info.fAverageValue += sectorInfo.fAverageValue;
			iValidSectorCount++;
		}
	}

	info.fAverageValue /= (float)iValidSectorCount;

	return true;
}

bool CTerrainManager::GetAffectedTerrainSectors( std::list<IHashString*> &sectorList, float fRadius, Vec3 pos )
{
	// An epsilon is required because of rounding errors in the functions that use the returned
	// list of sectors.  These errors can cause the functions using this list to affect some sectors
	// that the physics collision checks would otherwise ignore. The epsilon can't be calculated 
	// easily because it depends on who is calling the collision check.  For example, if this 
	// is being called to find a list of sectors to paint then the precise epsilon required 
	// would be the sector_size / paint_mask_resolution and a good epsilon would probably add 1.0f
	// to that just for good measure (floating point precision errors).
	// For now we will use an epsilon of sector_size / 32.0f + 1.0
	// This will work for any sector size (the largest factor in the equation) but will assume that
	// the datamap resolution is greater or equal to 32 (a likely assumption).
	float fEpsilon = m_fSectorSize / 32.0f + 1.0f;
	fRadius += fEpsilon;
	Vec3 vecMin = pos - Vec3( fRadius, 4096.0f, fRadius );
	Vec3 vecMax = pos + Vec3( fRadius, 4096.0f, fRadius );

	AABBINTERSECTPARAMS aabbMsg;
	std::list<IObject*> intersectionList;
	aabbMsg.listIntersections = &intersectionList;
	aabbMsg.min = &vecMin;
	aabbMsg.max = &vecMax;
	aabbMsg.collisionGroup = TERRAIN;

	static DWORD msgHash_IntersectAABB = CHashString(_T("IntersectAABB")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_IntersectAABB, sizeof(aabbMsg), &aabbMsg ) != MSG_HANDLED)
		return false;

	std::list<IObject*>::iterator itr = intersectionList.begin();
	while (itr != intersectionList.end())
	{
		IObject *pObject = (*itr);
		IHashString *pParent = pObject->GetParentName();
		sectorList.push_back( pParent );
		itr++;
	}

	return true;
}

DWORD CTerrainManager::OnTerrainDeform(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(BRUSHPARAMS));
	BRUSHPARAMS *pParams = (BRUSHPARAMS*)in_params;

	if (!TerrainDeform( *pParams ))
		return MSG_ERROR;

	return MSG_HANDLED_STOP;
}

DWORD CTerrainManager::OnGetAffectedTerrainSectors(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETAFFECTEDOBJECTSPARAMS));
	GETAFFECTEDOBJECTSPARAMS *pParams = (GETAFFECTEDOBJECTSPARAMS*)in_params;

	if (!GetAffectedTerrainSectors( *pParams->pAffectedObjectsList, pParams->fRadius, *pParams->pPosition ))
		return MSG_ERROR;

	return MSG_HANDLED_STOP;
}

// iX, iY are [0,heightmapsize]
float SampleHeightMapRAW( int iX, int iY, const float *pHeightMap, int iHeightMapSize )
{
	// range check
	iX = Clamp( iX, 0, iHeightMapSize-1 );
	iY = Clamp( iY, 0, iHeightMapSize-1 );

	// sample heightmap
	return pHeightMap[iY * iHeightMapSize + iX];
}

// fSamplePointX, fSamplePointY are [0,heightmapsize]
float SampleHeightMapBilinearly2( float fSamplePointX, float fSamplePointY, const float *pHeightMap, int iHeightMapSize )
{
	// convert coords to ints
	int iSamplePointX = (int)fSamplePointX;
	int iSamplePointY = (int)fSamplePointY;

	// get 2x2 samples
	float fSample00 = SampleHeightMapRAW( iSamplePointX, iSamplePointY, pHeightMap, iHeightMapSize );
	float fSample10 = SampleHeightMapRAW( iSamplePointX+1, iSamplePointY, pHeightMap, iHeightMapSize );
	float fSample01 = SampleHeightMapRAW( iSamplePointX, iSamplePointY+1, pHeightMap, iHeightMapSize );
	float fSample11 = SampleHeightMapRAW( iSamplePointX+1, iSamplePointY+1, pHeightMap, iHeightMapSize );

	// calculate sampling biases
	float fX1 = fmodf( fSamplePointX, 1.f );
	float fY1 = fmodf( fSamplePointY, 1.f );
	float fX0 = 1.f - fX1;
	float fY0 = 1.f - fY1;

	// bilinear interpolation of samples
	return ((fSample00*fX0 + fSample10*fX1) * fY0 + (fSample01*fX0 + fSample11*fX1) * fY1);
}

// fX, fY are [0,1]
float SampleHeightMapBilinearly( float fX, float fY, const float *pHeightMap, int iHeightMapSize )
{
	// convert coords from [0,1] to [0,heightmapsize]
	float fSamplePointX = fX * (float)(iHeightMapSize-1);
	float fSamplePointY = fY * (float)(iHeightMapSize-1);

	return SampleHeightMapBilinearly2( fSamplePointX, fSamplePointY, pHeightMap, iHeightMapSize );
}

// fSamplePointX, fSamplePointY are [0,heightmapsize]
Vec3 SampleHeightMapNormal( float fSamplePointX, float fSamplePointY, const float *pHeightMap, int iHeightMapSize, int iSampleResolution )
{
	static const float fDelta = 1.0f;
	static const float fDirScale = 2.0f;
	static Vec3 vecDX, vecDY;

/*	if (fSamplePointX > (float)(iSampleResolution / 2))
		vecDX.Set(fDirScale, 0.f,  SampleHeightMapBilinearly2(fSamplePointX,fSamplePointY, pHeightMap,iHeightMapSize) - SampleHeightMapBilinearly2(fSamplePointX-fDelta,fSamplePointY, pHeightMap,iHeightMapSize));
	else
		vecDX.Set(fDirScale, 0.f,  SampleHeightMapBilinearly2(fSamplePointX+fDelta,fSamplePointY, pHeightMap,iHeightMapSize) - SampleHeightMapBilinearly2(fSamplePointX,fSamplePointY, pHeightMap,iHeightMapSize));

	if (fSamplePointY > (float)(iSampleResolution / 2))
		vecDY.Set(0.f, fDirScale, SampleHeightMapBilinearly2(fSamplePointX,fSamplePointY, pHeightMap,iHeightMapSize) - SampleHeightMapBilinearly2(fSamplePointX,fSamplePointY-fDelta, pHeightMap,iHeightMapSize));
	else
		vecDY.Set(0.f, fDirScale, SampleHeightMapBilinearly2(fSamplePointX,fSamplePointY+fDelta, pHeightMap,iHeightMapSize) - SampleHeightMapBilinearly2(fSamplePointX,fSamplePointY, pHeightMap,iHeightMapSize));
*/
	

	vecDX.Set(fDirScale, 0.f,  SampleHeightMapBilinearly2(fSamplePointX+fDelta,fSamplePointY, pHeightMap,iHeightMapSize) - SampleHeightMapBilinearly2(fSamplePointX-fDelta,fSamplePointY, pHeightMap,iHeightMapSize));
	vecDY.Set(0.f, fDirScale, SampleHeightMapBilinearly2(fSamplePointX,fSamplePointY+fDelta, pHeightMap,iHeightMapSize) - SampleHeightMapBilinearly2(fSamplePointX,fSamplePointY-fDelta, pHeightMap,iHeightMapSize));
	//EE2FIX_HACK cross product:
	Vec3 vecNormal;
	vecNormal.x	=	vecDX.y*vecDY.z-vecDX.z*vecDY.y;
	vecNormal.y	=	vecDX.z*vecDY.x-vecDX.x*vecDY.z;
	vecNormal.z	=	vecDX.x*vecDY.y-vecDX.y*vecDY.x;	
	//Vec3 vecNormal = vecDX.GetCrossProduct( vecDY );
	vecNormal.Normalize();
	return Vec3(vecNormal.x, vecNormal.y, vecNormal.z );
}

void SampleHeightMapData( float in_fSectorSize, int in_iSampleResolution, const float *in_pHeightMap, int in_iHeightMapSize, float *out_pHeightMapSamples )
{
//	float fStep = 1.0f / (float)(in_iSampleResolution-1);
	float fSampleStep = (float)(in_iHeightMapSize-1) / (float)(in_iSampleResolution-1);

	int iSample = 0;
	float fSampleX =0.0f, fSampleY = 0.0f;
	for (int y=0; y < in_iSampleResolution; y++)
	{
		fSampleX = 0.0f;
		for (int x=0; x < in_iSampleResolution; x++)
		{
			if (out_pHeightMapSamples)
			{
				out_pHeightMapSamples[iSample] = SampleHeightMapBilinearly2( fSampleX, fSampleY, in_pHeightMap, in_iHeightMapSize );
//				out_pHeightMapSamples[iSample] = SampleHeightMapRAW( x, y, in_pHeightMap, in_iHeightMapSize);
			}
			fSampleX += fSampleStep;
			iSample++;
		}
		fSampleY += fSampleStep;
	}
}

DWORD CTerrainManager::OnCreateTerrain( DWORD size, void * in_params )
{
	TERRAINCREATIONPARAMS * tercreation;
	VERIFY_MESSAGE_SIZE( size, sizeof(TERRAINCREATIONPARAMS) );
	tercreation = ( TERRAINCREATIONPARAMS*)in_params;
	if( tercreation->SectorSize != 0
		&& tercreation->TerrainHeight != 0
		&& tercreation->TerrainWidth != 0 )
	{
		//if there was an old terrain free it first
		FreeTerrain();		
		//create the terrain
		m_BaseTextureScaleX = tercreation->BaseTextureScaleX;
		m_BaseTextureScaleY = tercreation->BaseTextureScaleY;
		CreateTerrain( tercreation->TerrainWidth, tercreation->TerrainHeight, tercreation->SectorResolution,
			tercreation->SectorSize , tercreation->ZScale, 150.0f, 
			tercreation->TerrainPath, tercreation->TerrainName, tercreation->TerrainBaseTexture );
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
	
}

void CTerrainManager::FreeTerrain()
{
	//iterate through terrain and remove it from hierarchy
	DELETEOBJECTPARAMS dop;

	//Delete all the sectors
	IDTOOBJECTMAP *pSectorList = GetObjectMap(&m_TerrainSectorClass);
	if (pSectorList)
	{
		IDTOOBJECTMAP::iterator itr = pSectorList->begin();
		while (pSectorList->begin() != pSectorList->end())
		{
			itr = pSectorList->begin();
			IObject *sectorObject = itr->second;
			dop.name = sectorObject->GetName();
			static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof( DELETEOBJECTPARAMS ), &dop, NULL, NULL );
		}
	}

	//delete the terrain config
	dop.name = &m_ConfigName;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof( DELETEOBJECTPARAMS ), &dop, NULL, NULL );

	m_iSectorResolution = 0;
}

void CTerrainManager::CreateTerrain( int width, int height, int sectorresolution, float sectorsize, float zscale, float LODDistance, 
									StdString &TerrainPath, StdString &TerrainName, StdString &TerrainBaseTexture )
{
	static CHashString archiveType(_T("Memory"));
	static CHashString hszEntityType( _T("CEntity") );
	static CHashString hszEntityName( _T("TerrainEntity") );

	// set file version
	static CHashString hszFileVersion( _T("2.5") );
	static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &hszFileVersion);

	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &archiveType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return;
	}
	IArchive *MemArchive = ca.archive;

	CREATEOBJECTPARAMS cop;
	SERIALIZEOBJECTPARAMS sop;
	INITOBJECTPARAMS iop;
	FINDOBJECTPARAMS fop;

	// create the encompassing CEntity
	{
		Vec3 zeroVec(0,0,0);
		Vec3 oneVec(1,1,1);
		MemArchive->SetIsWriting( true );
		MemArchive->SeekTo(0);
		MemArchive->Write( _T(""), "EntityTypes" );
		MemArchive->Write( zeroVec, "Position" );
		MemArchive->Write( zeroVec, "Rotation" );
		MemArchive->Write( oneVec, "Scale" );
		MemArchive->SetIsWriting( false );
		MemArchive->SeekTo(0);

		// check for duplicate terrain entry
		fop.hszName = &hszEntityName;
		static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
		if (MSG_HANDLED != m_ToolBox->SendMessage(msgHash_FindObject, sizeof(fop), &fop) ||
			!fop.bFound)
		{
			static CHashString hszEntityParentName( _T("World") );
			cop.typeName = &hszEntityType;
			cop.name = &hszEntityName;
			cop.parentName = &hszEntityParentName;
			static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
		}

		sop.name = &hszEntityName;
		sop.archive = MemArchive;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

		iop.name = &hszEntityName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
	}

	// create the terrain config data
	{
		static CHashString hszConfigType( _T("CTerrainConfig") );
		static CHashString hszConfigName( _T("TerrainConfig") );
		cop.typeName = &hszConfigType;
		cop.name = &hszConfigName;
		cop.parentName = &hszEntityName;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

		MemArchive->SetIsWriting( true );
		MemArchive->SeekTo(0);
		MemArchive->Write( _T("") );
		MemArchive->Write( _T("") );
		MemArchive->Write( sectorresolution );
		MemArchive->Write( sectorsize );
		MemArchive->Write( zscale );
		MemArchive->Write( LODDistance );
		MemArchive->SetIsWriting( false );
		MemArchive->SeekTo(0);

		sop.name = &hszConfigName;
		sop.archive = MemArchive;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

		iop.name = &hszConfigName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
	}
	// create the sectors
	for( int i = 0; i < width; i++ )
	{
		for( int j = 0; j < height; j++ )
		{
			BuildTerrainHeightmap( &hszEntityName, MemArchive, TerrainPath, TerrainName, i, j, sectorresolution );
			BuildTerrainSector( &hszEntityName, MemArchive, TerrainPath, TerrainName, TerrainBaseTexture, i, j, sectorsize );
		}
	}
	MemArchive->Close();
}

void CTerrainManager::BuildTerrainHeightmap( IHashString *pParentName, IArchive *MemArchive, StdString &TerrainPath, StdString &TerrainName, int iSectorX, int iSectorY, int iSectorResolution )
{
	char temp[64];
	StdString wszHeightMapName = TerrainPath;
	wszHeightMapName += TerrainName;
	wszHeightMapName += _T("_");
	_itot( iSectorX, temp, 10);
	wszHeightMapName += temp;
	wszHeightMapName += _T("_");
	_itot( iSectorY, temp, 10);
	wszHeightMapName += temp;
	wszHeightMapName += _T(".hmp");
	CHashString hszName((const char*)wszHeightMapName);

	CREATEOBJECTPARAMS cop;
	SERIALIZEOBJECTPARAMS sop;
	INITOBJECTPARAMS iop;

	MemArchive->SetIsWriting( true );
	MemArchive->SeekTo(0);
	MemArchive->Write( iSectorResolution );
	for (int i=0; i < iSectorResolution*iSectorResolution; i++)
	{
		MemArchive->Write( 0.0f );
	}
	MemArchive->SetIsWriting( false );
	MemArchive->SeekTo(0);

	cop.typeName = &m_HeightMapClass;
	cop.name = &hszName;
	cop.parentName = pParentName;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);

	sop.name = &hszName;
	sop.archive = MemArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);

	iop.name = &hszName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);
}

CHashString *CTerrainManager::BuildTerrainSector( IHashString *pParentName, IArchive *MemArchive, StdString &TerrainPath, StdString &TerrainName, StdString &TerrainBaseTexture, int SectorX, int SectorY, float sectorsize )
{
	char temp[64];
	StdString TerrainSectorName = _T("TerrainSector");
	_itot( SectorX, temp, 10);
	TerrainSectorName += temp;
	TerrainSectorName += "_";
	_itot( SectorY , temp, 10);
	TerrainSectorName += temp;
	static CHashString hszName;
	
	hszName = TerrainSectorName.c_str();

	CREATEOBJECTPARAMS cop;
	SERIALIZEOBJECTPARAMS sop;
	INITOBJECTPARAMS iop;

	StdString wszHeightMapName = TerrainPath;
	wszHeightMapName += TerrainName;
	wszHeightMapName += _T("_");
	_itot( SectorX, temp, 10);
	wszHeightMapName += temp;
	wszHeightMapName += _T("_");
	_itot( SectorY, temp, 10);
	wszHeightMapName += temp;
	wszHeightMapName += _T(".hmp");

	MemArchive->SetIsWriting( true );
	MemArchive->SeekTo(0);
	MemArchive->Write( SectorX );
	MemArchive->Write( SectorY );
	MemArchive->Write( wszHeightMapName );
	MemArchive->SetIsWriting( false );
	MemArchive->SeekTo(0);

	cop.typeName = &m_TerrainSectorClass;
	cop.name = &hszName;
	cop.parentName = pParentName;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);

	sop.name = &hszName;
	sop.archive = MemArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);

	iop.name = &hszName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);

	BuildTerrainSectorRenderObject( TerrainSectorName, TerrainBaseTexture, SectorX, SectorY, sectorsize );
	BuildTerrainSectorPhysicsObject( TerrainSectorName, SectorX, SectorY );
	BuildTerrainSectorWaterObject( TerrainSectorName, SectorX, SectorY, sectorsize );

	return &hszName;
}


void CTerrainManager::BuildTerrainSectorRenderObject( StdString &ParentName, StdString &TerrainBaseTexture, int SectorX, int SectorY, float sectorsize )
{
	CHashString hszTypeName(_T("CTerrainSectorRenderObject"));
	char temp[64];
	StdString TerrainSectorName = _T("TerrainRenderObj");
	_itot( SectorX, temp, 10);
	TerrainSectorName += temp;
	TerrainSectorName += "_";
	_itot( SectorY , temp, 10);
	TerrainSectorName += temp;
	CHashString hszName((const char*)TerrainSectorName);
	CHashString hszParent((const char*)ParentName);

    // Create the Terrain Sector Object
	CREATEOBJECTPARAMS cop;
	cop.typeName = &hszTypeName;
	cop.name = &hszName;
	cop.parentName = &hszParent;	// aka CWorld
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);

	IArchive *MemArchive;	
	CHashString memType(_T("Memory"));

	// Build the Terrain Sector Object Parameters
	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return;
	}

	MemArchive = ca.archive;
	MemArchive->Write( 1.1f ); // version
	MemArchive->Write( SectorX );
	MemArchive->Write( SectorY );
	MemArchive->Write( TerrainBaseTexture );
	MemArchive->Write( (float)m_BaseTextureScaleX );
	MemArchive->Write( (float)m_BaseTextureScaleY );
	MemArchive->Write( _T(""), _T("MaskTexture") );
	MemArchive->Write( int(0) );

	MemArchive->SetIsWriting( false );
	MemArchive->SeekTo(0);

	// Serialize the Terrain Sector Object Parameters
	SERIALIZEOBJECTPARAMS sop;
	sop.name = &hszName;
	sop.archive = MemArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);

	MemArchive->Close();

	INITOBJECTPARAMS iop;
	iop.name = &hszName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);
}

// we should move this to a Spawn Script.  The editors should
// be doing this, not Elemental Engine. - Ken
void CTerrainManager::BuildTerrainSectorPhysicsObject( StdString ParentName, int SectorX, int SectorY )
{
	CHashString hszTypeName(_T("CHeightfieldPhysicsObject"));
	char temp[64];
	_itot( SectorX , temp, 10);
	StdString TerrainSectorName = _T("TerrainPhysicsObj");
	TerrainSectorName += temp;
	TerrainSectorName += "_";
	_itot( SectorY , temp, 10);
	TerrainSectorName += temp;
	CHashString hszName((const char*)TerrainSectorName);
	CHashString hszParent((const char*)ParentName);

	CREATEOBJECTPARAMS cop;
	cop.typeName = &hszTypeName;
	cop.name = &hszName;
	cop.parentName = &hszParent;	// aka CWorld
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);

	// Create Archive
	IArchive *MemArchive;	
	CHashString memType(_T("Memory"));
	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return;
	}
	MemArchive = ca.archive;

	MemArchive->Write( TERRAIN, _T("CollisionGroup") );
	MemArchive->Write( 0, _T("ContactGroup") );

	MemArchive->SetIsWriting( false );
	MemArchive->SeekTo(0);

	// Serialize the Terrain Sector Object Parameters
	SERIALIZEOBJECTPARAMS sop;
	sop.name = &hszName;
	sop.archive = MemArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);

	MemArchive->Close();

	INITOBJECTPARAMS iop;
	iop.name = &hszName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);
}

void CTerrainManager::BuildTerrainSectorWaterObject( StdString ParentName, int SectorX, int SectorY, float sectorsize )
{
	CHashString hszTypeName(_T("CTerrainWaterRenderObject"));
	char temp[64];
	_itot( SectorX , temp, 10);
	StdString TerrainSectorName = _T("TerrainWaterObj");
	TerrainSectorName += temp;
	TerrainSectorName += "_";
	_itot( SectorY , temp, 10);
	TerrainSectorName += temp;
	CHashString hszName((const char*)TerrainSectorName);
	CHashString hszParent((const char*)ParentName);

	// Create Object
	CREATEOBJECTPARAMS cop;
	cop.typeName = &hszTypeName;
	cop.name = &hszName;
	cop.parentName = &hszParent;	// aka CWorld
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop, NULL, NULL);

	// Create Archive
	IArchive *MemArchive;	
	CHashString memType(_T("Memory"));
	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return;
	}
	MemArchive = ca.archive;

	float posX = sectorsize * (float)SectorX;
	float posY = sectorsize * (float)SectorY;

	MemArchive->Write( -m_fHeightMapZScale * 0.5f, _T("WaterLevel") );
	MemArchive->Write( 32, _T("Tesselation") );
	MemArchive->Write( 0.0f, _T("TransparencyDepth") );
	MemArchive->Write( _T("Materials\\TerrainWater.xrf"), _T("Shader") );

	MemArchive->SetIsWriting( false );
	MemArchive->SeekTo(0);

	// Serialize the Terrain Sector Object Parameters
	SERIALIZEOBJECTPARAMS sop;
	sop.name = &hszName;
	sop.archive = MemArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop, NULL, NULL);

	MemArchive->Close();

   INITOBJECTPARAMS iop;
	iop.name = &hszName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop, NULL, NULL);
}

void CTerrainManager::ModifyTerrainSectorRenderObject( StdString &ParentName, StdString &TerrainBaseTexture )
{
	//There are not many parameters that you can ask a renderobject to modify
	//it takes care of the rest by itself
	TERRAINBASETEXTUREPARAMS basetex;
	basetex.BaseTextureName = TerrainBaseTexture;
	basetex.fBaseTextureScaleX = m_BaseTextureScaleX;//these will be modifiable soon
	basetex.fBaseTextureScaleY = m_BaseTextureScaleY;
	static DWORD msgHash_TerrainSectorSetBaseTexture = CHashString(_T("TerrainSectorSetBaseTexture")).GetUniqueID();
	CHashString hsParentName(ParentName);
	if( m_ToolBox->SendMessage(msgHash_TerrainSectorSetBaseTexture, sizeof( TERRAINBASETEXTUREPARAMS ), &basetex, &hsParentName, NULL ) != MSG_HANDLED )
	{
		return;
	}
}

DWORD CTerrainManager::OnGetTerrainDescriptor( DWORD size, void * in_params )
{
	TERRAINDESCRIPTOR * params;
	VERIFY_MESSAGE_SIZE( size, sizeof( TERRAINDESCRIPTOR ) );
	params = (TERRAINDESCRIPTOR*)in_params;
	int StartX, StartY, EndX, EndY;

	IDTOOBJECTMAP *pSectorList = GetObjectMap(&m_TerrainSectorClass);
	if( pSectorList && !pSectorList->empty() )
	{
		//there's a terrain to be read	
		CalculateTerrainAABSquare( StartX, StartY, EndX, EndY );
		params->TerrainHeight = EndY - StartY + 1;
		params->TerrainWidth = EndX - StartX + 1;

		params->SectorResolution = m_iSectorResolution;
		params->SectorSize = m_fSectorSize;
		params->ZScale = m_fHeightMapZScale;

		TERRAINBASETEXTUREPARAMS basetex;
		IDTOOBJECTMAP::iterator iter = pSectorList->begin();
		IObject *sectorObject = iter->second;
		static DWORD msgHash_TerrainSectorGetBaseTexture = CHashString(_T("TerrainSectorGetBaseTexture")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TerrainSectorGetBaseTexture, sizeof(basetex), &basetex, sectorObject->GetName() );
		params->BaseTextureScaleX = basetex.fBaseTextureScaleX;
		params->BaseTextureScaleY = basetex.fBaseTextureScaleY;
		params->TerrainBaseTexture = basetex.BaseTextureName;
		return MSG_HANDLED_STOP;
	}

	return MSG_ERROR;//No terrain
}

DWORD CTerrainManager::OnGetTerrainSectorNeighborInfo( DWORD size, void * in_params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(TERRAINSECTORNEIGHBORINFO) );
	TERRAINSECTORNEIGHBORINFO *pParams = (TERRAINSECTORNEIGHBORINFO*)in_params;

	int vNeighborIndices[4][2] = { 
		pParams->iSourceX+1, pParams->iSourceY+0, 
		pParams->iSourceX+0, pParams->iSourceY+1, 
		pParams->iSourceX-1, pParams->iSourceY+0, 
		pParams->iSourceX+0, pParams->iSourceY-1 };

	for (int i=0; i < 4; i++)
	{
		pParams->vNeighborNames[i] = NULL;
		pParams->vNeighborLODs[i] = -1;

		int iLOD = -1;
		int iSectorIndex = GetTerrainSectorIndex( vNeighborIndices[i][0], vNeighborIndices[i][1] );
		SECTORINDEXTONAMEMAP::iterator itrSector = m_SectorIndexToNameMap.find(iSectorIndex);
		if (itrSector != m_SectorIndexToNameMap.end())
		{
			IHashString *pNeighbor = itrSector->second;
			pParams->vNeighborNames[i] = pNeighbor;

			static DWORD msgHash_GetTerrainSectorLOD = CHashString(_T("GetTerrainSectorLOD")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_GetTerrainSectorLOD, sizeof(int), &iLOD, pNeighbor ) == MSG_HANDLED)
				pParams->vNeighborLODs[i] = iLOD;

			memset( &pParams->vPolyData[i], 0, sizeof(TERRAINSECTORGETPOLYDATA) );
			TERRAINSECTORGETPOLYDATA polydata;
			static DWORD msgHash_TerrainSectorGetPolyData = CHashString(_T("TerrainSectorGetPolyData")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_TerrainSectorGetPolyData, sizeof(TERRAINSECTORGETPOLYDATA), &polydata, pNeighbor ) == MSG_HANDLED)
				pParams->vPolyData[i] = polydata;

//			IMeshObject *pMeshObject = NULL;
//			static DWORD msgHash_GetMeshObject = CHashString(_T("GetMeshObject")).GetUniqueID();
//			if (m_ToolBox->SendMessage(msgHash_GetMeshObject, sizeof(IMeshObject*), &pMeshObject, pNeighbor ) == MSG_HANDLED)
//				pParams->vMeshObjects[i] = pMeshObject;
		}
	}

	return MSG_HANDLED_STOP;
}

DWORD CTerrainManager::OnStart(DWORD size, void *data)
{
	m_bIsRunning = true;
	
	return MSG_HANDLED_PROCEED;
}

DWORD CTerrainManager::OnStop(DWORD size, void *data)
{
	m_bIsRunning = false;
	
	return MSG_HANDLED_PROCEED;
}

IEffect* CTerrainManager::GetDetailEffect( bool bLightmap )
{
	if (bLightmap)
	{
		if (!m_pDetailEffectLightmap)		
		{
			CHashString hszDetailEffectLightmap(_T("Shaders\\TerrainDetail_Lightmap.efx"));
			m_pDetailEffectLightmap = CRenderObject<>::LoadEffect( &hszDetailEffectLightmap );		
			if( m_pDetailEffectLightmap )
			{
				UINT pass;
				CHashString baseChannel(_T("Base"));
				CHashString detailChannel(_T("Mask0"));
				CHashString lightmapChannel(_T("LightMap"));
				m_pDetailEffectLightmap->GetTextureChannels( &baseChannel, pass, m_BaseTextureStage );
				m_pDetailEffectLightmap->GetTextureChannels( &detailChannel, pass, m_MaskTextureStageStart );
				m_pDetailEffectLightmap->GetTextureChannels( &lightmapChannel, pass, m_LightmapTextureStage );
			}
		}
		return m_pDetailEffectLightmap;
	}
	else
	{
		if (!m_pDetailEffect)		
		{
			CHashString hszDetailEffect(_T("Shaders\\TerrainDetail.efx"));
			m_pDetailEffect = CRenderObject<>::LoadEffect( &hszDetailEffect );		
			if( m_pDetailEffect )
			{
				UINT pass;
				CHashString baseChannel(_T("Base"));
				CHashString detailChannel(_T("Mask0"));
				m_pDetailEffect->GetTextureChannels( &baseChannel, pass, m_BaseTextureStage );
				m_pDetailEffect->GetTextureChannels( &detailChannel, pass, m_MaskTextureStageStart );
			}
		}
		return m_pDetailEffect;
	}
}
IEffect* CTerrainManager::GetDistantEffect()
{
	if (!m_pDistantEffect)
	{
		CHashString hszDistantEffect(_T("Shaders\\TerrainDistant.efx"));
		m_pDistantEffect = CRenderObject<>::LoadEffect( &hszDistantEffect );
	}
	return m_pDistantEffect;
}

DWORD CTerrainManager::OnCreateTerrainDecal(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TERRAINDECALPARAMS));
	TERRAINDECALPARAMS tdp = *(TERRAINDECALPARAMS*) param;

	// nick: should we create a decal with no texture? I don't think so...
	if ((tdp.hsTextureName != NULL) && (tdp.vPosition != NULL))
	{
		static CHashString defaultDecalName(_T("TerrainDecal"));
		CHashString newName(_T(""));
		GENERATEUNIQUEOBJECTNAMEPARAMS guonp;
		guonp.name = &defaultDecalName;
		guonp.newname = &newName;

		static DWORD msgGenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgGenerateUniqueObjectName, sizeof(GENERATEUNIQUEOBJECTNAMEPARAMS), &guonp) == MSG_HANDLED)
		{
			static CHashString archiveType(_T("Memory"));
			CREATEARCHIVE ca;
			ca.mode = STREAM_MODE_READ | STREAM_MODE_WRITE;
			ca.streamData = NULL;
			ca.streamSize = 0;
			ca.streamType = &archiveType;

			static DWORD msgCreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
			if ((m_ToolBox->SendMessage(msgCreateArchive, sizeof(CREATEARCHIVE), &ca) == MSG_HANDLED) &&
				(ca.archive != NULL))
			{
				IArchive *ar = ca.archive;

				static CHashString decalType(_T("CTerrainDecalRenderObject"));

				// create
				CREATEOBJECTPARAMS cop;
				cop.name = &newName;
				cop.typeName = &decalType;
				cop.parentName = NULL;

				static DWORD msgCreateObject = CHashString(_T("CreateObject")).GetUniqueID();
				if (m_ToolBox->SendMessage(msgCreateObject, sizeof(CREATEOBJECTPARAMS), &cop) == MSG_HANDLED)
				{				
					Vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
					if (tdp.v4Color)
					{
						color = *tdp.v4Color;
					}
					// serialize
					ar->SetIsWriting(true);
					ar->Write(tdp.hsTextureName->GetString());
					ar->Write(tdp.fSize);
					ar->Write(color);
					ar->Write(tdp.fLingerTime);
					ar->Write(tdp.fFadeTime);
					ar->SetIsWriting(false);

					SERIALIZEOBJECTPARAMS sop;
					sop.archive = ar;
					sop.name = &newName;

					static DWORD msgSerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
					m_ToolBox->SendMessage(msgSerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

					ar->Close();

					// init
					INITOBJECTPARAMS iop;
					iop.name = &newName;
					
					static DWORD msgInitObject = CHashString(_T("InitObject")).GetUniqueID();
					m_ToolBox->SendMessage(msgInitObject, sizeof(INITOBJECTPARAMS), &iop);

					// remove from hierarchy
					static DWORD msgRemoveObject = CHashString(_T("RemoveObject")).GetUniqueID();
					m_ToolBox->SendMessage(msgRemoveObject, sizeof(IHashString), &newName);

					// now set position of decal
					static DWORD msgSetPosition = CHashString(_T("SetPosition")).GetUniqueID();
					m_ToolBox->SendMessage(msgSetPosition, sizeof(Vec3), tdp.vPosition, &newName);

					return MSG_HANDLED_STOP;
				}
				else
				{
					m_ToolBox->Log(LOGWARNING, _T("%s create terrain decal: could not create object\n"), __FILE__);	
					ar->Close();
				}
			}
			else
			{
				m_ToolBox->Log(LOGWARNING, _T("%s create terrain decal: could not create archive\n"), __FILE__);				
			}
		}
		else
		{
			m_ToolBox->Log(LOGWARNING, _T("%s create terrain decal: could not get unique object name\n"), __FILE__);			
		}
	}

	return MSG_NOT_HANDLED;
}