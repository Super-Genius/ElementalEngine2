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

#include "Stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON( LightMapManager );
REGISTER_MESSAGE_HANDLER(BuildLightmapScene, OnBuildLightmap, LightMapManager);
REGISTER_MESSAGE_HANDLER(RegisterLightmapCache, OnRegisterLightmapCache, LightMapManager );
REGISTER_MESSAGE_HANDLER(UnRegisterLightmapCache, OnUnRegisterLightmapCache, LightMapManager );
REGISTER_MESSAGE_HANDLER(GetLightmapCache, OnGetLightmapCache, LightMapManager );
REGISTER_MESSAGE_HANDLER(AddLightmappableObject, OnAddLightmappableObject, LightMapManager );
REGISTER_MESSAGE_HANDLER(CreateMeshParameterization, OnCreateMeshParameterization, LightMapManager );
REGISTER_MESSAGE_HANDLER(SetGeneratorParams, OnSetGeneratorParams, LightMapManager);
REGISTER_MESSAGE_HANDLER(GetGeneratorParams, OnGetGeneratorParams, LightMapManager);
REGISTER_MESSAGE_HANDLER(SetLightmapOutputDirectory, OnSetLightmapOutputDirectory, LightMapManager);
REGISTER_MESSAGE_HANDLER( ResizeLightmapCache, OnResizeLightmapCache, LightMapManager );
/// \brief The Default Constructor for LightMapManager
/// \return void

LightMapManager::LightMapManager() : 
	CManager( _T("LightMapManager"), 0, LIGHTMAPPRIORITY ),
	m_LightTypeName(_T("LightMapObject")),
	m_worldLMapTypeName(_T("WorldLightmapObject")),
	m_worldTypeName(_T("World"))
{
}

/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *LightMapManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( LightMapManager );

}

/// \brief The Default Destructor for LightMapManager
/// \return void
LightMapManager::~LightMapManager( )
{
	DeInit();
}
// Initializes the manager
bool LightMapManager::Init()
{
	return true;
}

/// \brief	Update function for the LightMapManager
///			Renders immediately, the queued list of renderobjects
/// \return	void
void LightMapManager::Update( DWORD tickCount )
{	
	//send a dummy file so we get a callback for this first phase	
	IDTOOBJECTMAP::iterator objIter;
	IDTOOBJECTMAP *objMap = GetObjectMap( &m_LightTypeName);
	//update our objects
	if( objMap )
	{		
		IObject * destObject;
		for( objIter = objMap->begin();objIter != objMap->end(); objIter++ )
		{
			destObject = objIter->second;
			destObject->Update();
		}
	}
	
}

/// \brief	DeInitializes the LightMapManager
/// \return	bool: Success
bool LightMapManager::DeInit( void )
{
	map< DWORD, LightmapCache * >::iterator itrLightmapCache = m_LightmapCaches.begin();
	while (itrLightmapCache != m_LightmapCaches.end())
	{
		itrLightmapCache->second->Clean();
		delete itrLightmapCache->second;
		itrLightmapCache++;
	}
	m_LightmapCaches.clear(); 
	CManager::DeInit();
	return true;
}

DWORD LightMapManager::OnSetLightmapOutputDirectory( DWORD size, void * params )
{
	//
	//Go through all cal objects in scene

	static CHashString lObjectType(_T("LightMapObject"));
	static CHashString lMapObject(_T("WorldLightmapObject") );
	static DWORD msgHash_SetLightmapOutputDirectory = CHashString(_T("SetLightmapOutputDirectory")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetLightmapOutputDirectory, sizeof( IHashString ), params, &lMapObject, &lObjectType );
	
	return MSG_HANDLED_STOP;
}

DWORD LightMapManager::OnBuildLightmap( DWORD size, void * params )
{
	//
	//Go through all cal objects in scene

	static CHashString lObjectType(_T("LightMapObject"));
	static CHashString lMapObject(_T("WorldLightmapObject") );
	static DWORD msgHash_GenerateLightmaps = CHashString(_T("GenerateLightmaps")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GenerateLightmaps, sizeof( IHashString ), params, &lMapObject, &lObjectType );
	
	return MSG_HANDLED_STOP;
}

DWORD LightMapManager::OnRegisterLightmapCache( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( LightmapCache ) );
	LightmapCache * cache = (LightmapCache*)params;
	LightmapCache * internalCache;
	if( cache )
	{
		//link this data to map
		if (m_LightmapCaches.find( cache->m_Hash ) != m_LightmapCaches.end())
		{
			internalCache = m_LightmapCaches[ cache->m_Hash ];
		}
		else
		{
			internalCache = new LightmapCache;
			m_LightmapCaches[ cache->m_Hash ] = internalCache;
		}
		cache->CloneTo( *internalCache );
	}
	return MSG_HANDLED_STOP;
}

DWORD LightMapManager::OnUnRegisterLightmapCache( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( LightmapCache ) );
	LightmapCache * cache = (LightmapCache*)params;
	map< DWORD, LightmapCache * >::iterator iter = m_LightmapCaches.find( cache->m_Hash );
	
	if( iter != m_LightmapCaches.end() )
	{
		iter->second->Clean();
		delete iter->second;
		m_LightmapCaches.erase( iter );
	}
	
	return MSG_HANDLED_STOP;
}

DWORD LightMapManager::OnGetLightmapCache( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( LightmapCache ) );
	LightmapCache * cache = (LightmapCache*)params;
	map< DWORD, LightmapCache * >::iterator iter = m_LightmapCaches.find( cache->m_Hash );
	if( iter != m_LightmapCaches.end() )
	{
		LightmapCache * internalCache = iter->second;
		if( internalCache )
		{
			internalCache->CloneTo( *cache );
			return MSG_HANDLED_STOP;
		}
	}
	return MSG_ERROR;
}

DWORD LightMapManager::OnCreateMeshParameterization( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( CREATEMESHPARAMETERIZATION ) );
	CREATEMESHPARAMETERIZATION * meshParam = (CREATEMESHPARAMETERIZATION*) params;
	if( meshParam != NULL )
	{
		if( meshParam->name )
		{
			IComponent *tmpComponent;
			tmpComponent = m_ToolBox->CreateComponent(&CHashString("MeshParameterization"), 2, meshParam->name, meshParam->name );
			MeshParameterization * pMeshParam = dynamic_cast<MeshParameterization*>(tmpComponent);
			if( !pMeshParam )
			{
				m_ToolBox->Log( LOGERROR, _T("Could not create Mesh parameterizatoin\n"));
				return MSG_ERROR;
			}
		}
	}
	//create new mesh param
	return MSG_HANDLED_STOP;
}

DWORD LightMapManager::OnAddLightmappableObject( DWORD size, void * params )
{
	ADDLIGHTMAPPABLEOBJECT * msg;
	IHashString * name;
	IHashString * type;
	VERIFY_MESSAGE_SIZE( size, sizeof( ADDLIGHTMAPPABLEOBJECT) );
	msg = (ADDLIGHTMAPPABLEOBJECT*)params;
	name = msg->name;
	type = msg->type;

	static CHashString lMapObject(_T("WorldLightmapObject") );
	static CHashString lObjectType(_T("LightMapObject"));
	static CHashString worldParent(_T("World"));
	FINDOBJECTPARAMS param;
	param.hszName = &lMapObject;
	//find it
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	DWORD result = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );
	
	if( result != MSG_HANDLED )
	{
		return MSG_ERROR;
	}

	if( param.bFound == false )
	{
		//couldn't find it, create one
		CREATEOBJECTPARAMS createObjectParams;
		createObjectParams.parentName = &worldParent;
		createObjectParams.typeName = &lObjectType;
		createObjectParams.name = &lMapObject;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &createObjectParams);
	}
	//now pass message to it:
	static DWORD msgHash_AddLightmappableObject = CHashString(_T("AddLightmappableObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddLightmappableObject, sizeof( ADDLIGHTMAPPABLEOBJECT ), msg, &lMapObject, &lObjectType );
	return MSG_HANDLED_STOP;
}

void AA()
{
	GETRENDERBUFFERALLOCATIONMSG alloc;
	for( int i = 0; i < (int)alloc.m_GeometryAllocationsOut.size(); i++ )
	{
		RENDERBUFFERALLOCATION &rb = alloc.m_GeometryAllocationsOut[ i ];
		//rb.m_VertexAllocation
	}
}


DWORD LightMapManager::OnSetGeneratorParams( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GENERATORPARAMSMSG ) );
	GENERATORPARAMSMSG * msg = (GENERATORPARAMSMSG*)params;
	if( msg )
	{
		LightMapGenerator::NUMPHOTONS = msg->NUMPHOTONS;
		LightMapGenerator::MAX_BOUNCE_TEST = msg->MAX_BOUNCE_TEST;
		LightMapGenerator::PHOTON_DIRECT_RANGE_MAX = msg->PHOTON_DIRECT_RANGE_MAX;
		LightMapGenerator::PHOTON_INDIRECT_RANGE_MAX = msg->PHOTON_INDIRECT_RANGE_MAX;
		LightMapGenerator::PHOTON_WEIGHT_RANGE = msg->PHOTON_WEIGHT_RANGE;
		LightMapGenerator::PHOTON_RANGE_EXTENSION = msg->PHOTON_RANGE_EXTENSION;
		LightMapGenerator::PHOTON_SAMPLES = msg->PHOTON_SAMPLES;
		LightMapGenerator::PHOTON_WEIGHT_RANGE_MAX = msg->PHOTON_WEIGHT_RANGE_MAX;
		LightMapGenerator::SHADOW_EPSILON = msg->SHADOW_EPSILON;
		LightMapGenerator::SPECULAR_CHANCE = msg->SPECULAR_CHANCE;
		LightMapGenerator::PHOTON_MAX_DISK_ITERATION = msg->PHOTON_MAX_DISK_ITERATION;
		LightMapGenerator::TRIANGLE_NORMAL_EPSILON = msg->TRIANGLE_NORMAL_EPSILON;	//epsilon for averaged normals, making sure we're still lighting if a vertex is somehow
																		//different from the averaged normal in a way that makes it lit when the average normal isn't lit
		LightMapGenerator::ENERGY_CUTOFF = msg->ENERGY_CUTOFF;
		LightMapGenerator::NUMBOUNCES = msg->NUMBOUNCES;
		LightMapGenerator::NORMAL_CUTOFF = msg->NORMAL_CUTOFF; //anything with dissimilar normals does not get affected, should be > 90 degrees
		LightMapGenerator::DOT_SHADOW_ANGLE_CUTOFF = msg->DOT_SHADOW_ANGLE_CUTOFF;
		LightMapGenerator::energy_loss = msg->energy_loss;
		LightMapGenerator::DIRECT_LIGHT_ONLY = msg->DIRECT_LIGHT_ONLY;

		MeshParameterization::MIN_TEXTURE_SIZE = msg->MIN_TEXTURE_SIZE;
		MeshParameterization::MAX_TEXTURE_SIZE = msg->MAX_TEXTURE_SIZE;
		MeshParameterization::TEXTURE_TO_WORLD_SPACE_RATIO = msg->TEXTURE_TO_WORLD_SPACE_RATIO;
	}
	return MSG_HANDLED_STOP;
}

DWORD LightMapManager::OnResizeLightmapCache( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( ResizeCacheMessage  ) );
	ResizeCacheMessage * msg = (ResizeCacheMessage*)params;
	if( msg 
		&&
		msg->cache )
	{
		if( msg->numRemappedData > 0 )
		{
			msg->cache->ResizeRemappedData( msg->numRemappedData );
		}
		if( msg->numRemappedIndices > 0 )
		{
			msg->cache->ResizeRemappedIndices( msg->numRemappedIndices );
		}
		if( msg->numUVData > 0 )
		{
			msg->cache->ResizeUVData( msg->numUVData );
		}
	}
	return MSG_HANDLED_STOP;
}

DWORD LightMapManager::OnGetGeneratorParams( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GENERATORPARAMSMSG ) );
	GENERATORPARAMSMSG * msg = (GENERATORPARAMSMSG*)params;
	if( msg )
	{
		msg->DIRECT_LIGHT_ONLY = LightMapGenerator::DIRECT_LIGHT_ONLY;
		msg->NUMPHOTONS = LightMapGenerator::NUMPHOTONS;
		msg->MAX_BOUNCE_TEST = LightMapGenerator::MAX_BOUNCE_TEST;
		msg->PHOTON_DIRECT_RANGE_MAX = LightMapGenerator::PHOTON_DIRECT_RANGE_MAX;
		msg->PHOTON_INDIRECT_RANGE_MAX = LightMapGenerator::PHOTON_INDIRECT_RANGE_MAX;
		msg->PHOTON_WEIGHT_RANGE = LightMapGenerator::PHOTON_WEIGHT_RANGE;
		msg->PHOTON_RANGE_EXTENSION = LightMapGenerator::PHOTON_RANGE_EXTENSION;
		msg->PHOTON_SAMPLES = LightMapGenerator::PHOTON_SAMPLES;
		msg->PHOTON_WEIGHT_RANGE_MAX = LightMapGenerator::PHOTON_WEIGHT_RANGE_MAX;
		msg->SHADOW_EPSILON = LightMapGenerator::SHADOW_EPSILON;
		msg->SPECULAR_CHANCE = LightMapGenerator::SPECULAR_CHANCE;
		msg->PHOTON_MAX_DISK_ITERATION = LightMapGenerator::PHOTON_MAX_DISK_ITERATION;
		msg->TRIANGLE_NORMAL_EPSILON = LightMapGenerator::TRIANGLE_NORMAL_EPSILON;	//epsilon for averaged normals, making sure we're still lighting if a vertex is somehow
																		//different from the averaged normal in a way that makes it lit when the average normal isn't lit
		msg->ENERGY_CUTOFF = LightMapGenerator::ENERGY_CUTOFF;
		msg->NUMBOUNCES = LightMapGenerator::NUMBOUNCES;
		msg->NORMAL_CUTOFF = LightMapGenerator::NORMAL_CUTOFF; //anything with dissimilar normals does not get affected, should be > 90 degrees
		msg->DOT_SHADOW_ANGLE_CUTOFF = LightMapGenerator::DOT_SHADOW_ANGLE_CUTOFF;
		msg->energy_loss = LightMapGenerator::energy_loss;

		msg->MIN_TEXTURE_SIZE =	MeshParameterization::MIN_TEXTURE_SIZE;
		msg->MAX_TEXTURE_SIZE = MeshParameterization::MAX_TEXTURE_SIZE;
		msg->TEXTURE_TO_WORLD_SPACE_RATIO = MeshParameterization::TEXTURE_TO_WORLD_SPACE_RATIO;
	}
	return MSG_HANDLED_STOP;
}