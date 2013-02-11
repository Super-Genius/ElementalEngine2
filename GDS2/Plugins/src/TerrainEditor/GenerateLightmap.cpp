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

#include "stdafx.h"

void GenerateLightmap::Generate()
{
	static CHashString TerrainType( _T("CTerrainSector"));
	static CHashString MeshTypeName( _T("MeshParameterization"));
	static CHashString LightMapGeneratorTypeName( _T("LightMapGenerator"));
	static CHashString LightMapGeneratorName( _T("tempLightmapGenerator"));
	static TCHAR buffer[1024];
	IToolBox * m_ToolBox = EngineGetToolBox();	
	
	IComponent *tmpComponent;
	tmpComponent = m_ToolBox->CreateComponent( &LightMapGeneratorTypeName, 2, NULL, &LightMapGeneratorName );
	//just a container pointer, we don't actually use interface functions
	LightMapGenerator * m_LightGen = (LightMapGenerator*)(tmpComponent);

	TERRAINSECTORGETPOLYDATA	terrainData;
	terrainData.in_bGenerateUVs = true;
	IToolBox * toolBox = EngineGetToolBox();
	IHashString * terrainSectorName = NULL;
	static DWORD msgHash_TerrainSectorGetPolyData = CHashString(_T("TerrainSectorGetPolyData")).GetUniqueID();
	if( toolBox->SendMessage(msgHash_TerrainSectorGetPolyData, sizeof(TERRAINSECTORGETPOLYDATA), &terrainData, terrainSectorName, &TerrainType
		) != MSG_HANDLED )
	{
		toolBox->Log( LOGWARNING, _T("Warning, Could not get Terrain Sector Data") );		
	}
	//terrainData.
	//add to mesh param
	//add mesh param to lightmap generator
	
	_stprintf( buffer, "%s_lightMap", terrainSectorName->GetString() );
	CHashString lightMapName( buffer );
	_stprintf( buffer, "%s_meshParam", terrainSectorName->GetString() );
	CHashString meshParamName( buffer );
	tmpComponent = m_ToolBox->CreateComponent(&MeshTypeName, 2, NULL, &meshParamName );
	//just a container pointer, we don't actually use interface functions
	MeshParameterization * curMesh = (MeshParameterization*)(tmpComponent);

	//send message to add vertices for mesh		
	GETRENDERBUFFERALLOCATIONMSG msg;
	static DWORD msgHash_GetGeometryAllocations = CHashString(_T("GetGeometryAllocations")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGeometryAllocations, sizeof( msg ), &msg, terrainSectorName, &TerrainType );
	//MeshParam->Initialize();	
	for( int i = 0; i < (int)msg.m_GeometryAllocationsOut.size(); i++ )
	{
		RENDERBUFFERALLOCATION * pAlloc = &msg.m_GeometryAllocationsOut[ i ];
		static DWORD msgHash_AddVertexIndexSoup = CHashString(_T("AddVertexIndexSoup")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_AddVertexIndexSoup, sizeof(RENDERBUFFERALLOCATION), pAlloc, terrainSectorName, &TerrainType );
	}
	//create texture for mesh
	CREATETEXTUREPARAMS createtextureparams;
	createtextureparams.bitDepth = 32;
	createtextureparams.Name = &lightMapName;
	DWORD texSize = 128; //default temp, maybe shouldbe 0
	static DWORD msgHash_GetTextureSize = CHashString(_T("GetTextureSize")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetTextureSize, sizeof(texSize), &texSize, &meshParamName, &MeshTypeName );
	createtextureparams.sizeX = texSize;
	createtextureparams.sizeY = texSize;
	static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateTexture, sizeof(createtextureparams), &createtextureparams );
	ITextureObject * texture = dynamic_cast<ITextureObject*>( createtextureparams.TextureObjectInterface );
	
	//send message to add mesh to lightmap generator
	//MESHTEXTUREPAIRMSG meshTextureMsg;
	//TODO:
	//meshTextureMsg.mesh = curMesh;
	//meshTextureMsg.texture = texture;

	//m_ToolBox=>SendMessage( _T("AddMesh"), sizeof( meshTextureMsg ), &meshTextureMsg, &meshParamName, &MeshTypeName );
	
	//send message to generate the whole thing



	
}