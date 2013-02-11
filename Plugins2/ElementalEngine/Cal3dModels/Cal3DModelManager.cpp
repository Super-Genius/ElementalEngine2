///============================================================================
/// \file	Cal3DModelManager.cpp
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

TOLUA_API int tolua_Cal3dModelsLua_open (lua_State* tolua_S);
REGISTER_COMPONENT_SINGLETON( Cal3DModelManager );

//these are internally used messages, nothing should have access to the message structures
REGISTER_MESSAGE_HANDLER( GetCal3dCallback, OnGetCallback, Cal3DModelManager);
REGISTER_MESSAGE_HANDLER( GetCal3DModelInterface, OnGetCal3DModelInterface, Cal3DModelManager);
REGISTER_MESSAGE_HANDLER( SaveAllCalCubes, OnSaveAllCalCubes, Cal3DModelManager);
REGISTER_MESSAGE_HANDLER(DestroyRenderer, OnDestroyRenderer, Cal3DModelManager);

/// \brief The Default Constructor for Cal3DModelManager
/// \return void
Cal3DModelManager::Cal3DModelManager() : 
	CManager(_T("Cal3DModelManager"), RENDEROBJECTPRIORITY, RENDEROBJECTPRIORITY )//prioirity?
{
	m_ToolBox = EngineGetToolBox();	

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
		tolua_Cal3dModelsLua_open(m_LuaState);
	}

	VertexFormat.clear();
	VertexFormat.push_back( MESH_STREAM_VERTEX_POSITION3 );
	VertexFormat.push_back( MESH_STREAM_WEIGHTS );
	VertexFormat.push_back( MESH_STREAM_MATRIX_INDICES );
	VertexFormat.push_back( MESH_STREAM_VERTEX_NORMAL );
	VertexFormat.push_back( MESH_STREAM_VERTEX_TEXTURECOORD1 );
	VertexFormat.push_back( MESH_STREAM_VERTEX_TANGENT4 );

	VertexFormatNoSkin.clear();
	VertexFormatNoSkin.push_back( MESH_STREAM_VERTEX_POSITION3 );
	VertexFormatNoSkin.push_back( MESH_STREAM_VERTEX_NORMAL );
	VertexFormatNoSkin.push_back( MESH_STREAM_VERTEX_TEXTURECOORD1 );
	VertexFormatNoSkin.push_back( MESH_STREAM_VERTEX_TANGENT4 );
	VertexFormatNoSkin.push_back( MESH_STREAM_VERTEX_TEXTURECOORD2 );
}

/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *Cal3DModelManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( Cal3DModelManager );
}

/// \brief The Default Destructor for Cal3DModelManager
/// \return void
Cal3DModelManager::~Cal3DModelManager( )
{
	DeInit();
}
// Initializes the manager
bool Cal3DModelManager::Init()
{
	return true;
}

/// \brief	Update function for the Cal3DModelManager
///			Renders immediately, the queued list of renderobjects
/// \return	void
void Cal3DModelManager::Update( DWORD tickCount )
{	
	//check for bad callbacks
	stack< CalCoreAnimation * > deadCallbacks;
	CAL3DANIMATIONTOCALLBACKMAP::iterator iter = m_AnimationCallbacks.begin();
	for(;iter != m_AnimationCallbacks.end(); ++iter)
	{
		Cal3dCallback * cb = (*iter).second;
		if( cb && cb->m_bDelete )
		{
			deadCallbacks.push( (*iter).first );
		}
	}
	//delete dead animation callbacks
	while( !deadCallbacks.empty() )
	{			
		CalCoreAnimation * ca = deadCallbacks.top();
		deadCallbacks.pop();
		iter = m_AnimationCallbacks.find( ca );
		if( iter != m_AnimationCallbacks.end())
		{
			Cal3dCallback * cb = (*iter).second;
			ca->removeCallback( cb );
			delete cb;
			m_AnimationCallbacks.erase( iter);
		}
	}

	OBJECTLIST	objList;
	static CHashString calTypeName( _T("Cal3DRenderObject"));
	//send a dummy file so we get a callback for this first phase	
	IDTOOBJECTMAP::iterator objIter;
	IDTOOBJECTMAP *objMap = GetObjectMap( &calTypeName );
	//update our objects
	if( objMap )
	{		
		IObject * destObject;
		for( objIter = objMap->begin();objIter != objMap->end(); ++objIter )
		{
			destObject = objIter->second;
			destObject->Update();
		}
	}
	
}

/// \brief	DeInitializes the Cal3DModelManager
/// \return	bool: Success
bool Cal3DModelManager::DeInit( void )
{
	// delete animation callbacks
	CAL3DANIMATIONTOCALLBACKMAP::iterator iter = m_AnimationCallbacks.begin();
	for(;iter != m_AnimationCallbacks.end(); iter++)
	{
		Cal3dCallback * cb = (*iter).second;
		delete cb;
	}
	m_AnimationCallbacks.clear();

	// clean up any hardware allocations
	HARDWAREALLOCATIONMAP::iterator itr;
	for (itr = m_HardwareAllocationMap.begin(); itr != m_HardwareAllocationMap.end(); itr++)
	{
		static DWORD msgHash_RemoveVertexAllocation = CHashString(_T("RemoveVertexAllocation")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveVertexAllocation, sizeof(BUFFERALLOCATIONSTRUCT), &itr->second.hardwareAllocation.vertexBufferAllocation );

		static DWORD msgHash_RemoveIndexAllocation = CHashString(_T("RemoveIndexAllocation")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveIndexAllocation, sizeof(BUFFERALLOCATIONSTRUCT), &itr->second.hardwareAllocation.indexBufferAllocation );

		SAFE_DELETE( itr->second.hardwareAllocation.pCalHardwareModel );
	}
	m_HardwareAllocationMap.clear();

	CManager::DeInit();
	return true;
}

/*
bool Cal3DModelManager::LoadCacheDataFromFile( const TCHAR *file, CalCoreMesh *pCoreMesh )
{
	LOADFILEEXTPARAMS lfep;
	lfep.fileName = (TCHAR*)file;
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
	if( retval != MSG_HANDLED )
	{
		m_ToolBox->Log( LOGINFORMATION, _T("Cal3d Load: Missing model cache file data %s.\n"), file );
		return false;
	}

	CHashString hszFile(file);
	CAL3DATASTREAMMAP::iterator it = m_mDataStreams.find( hszFile.GetUniqueID() );
	if( it == m_mDataStreams.end() )
	{
		m_ToolBox->Log( LOGINFORMATION, _T("Cal3d Load: Missing model cache file data %s.\n"), file );
		return false;
	}

	Cal3DDataStream &cal3DDataStream = it->second;
	cal3DDataStream.GetArchive()->SeekTo(0);
	int result = CalLoader::loadCoreMeshCache(cal3DDataStream, pCoreMesh);
	if (result)
	{
		return true;
	}
	else
	{
		m_ToolBox->Log( LOGWARNING, _T("Cal3d Load: Bad model cache file data %s.\n"), file );
		return false;
	}
}
*/

DWORD Cal3DModelManager::OnGetCallback( DWORD size, void * params )
{
	GETCAL3DCALLBACK * msg;
	VERIFY_MESSAGE_SIZE( size, sizeof( GETCAL3DCALLBACK ) );
	msg = (GETCAL3DCALLBACK*)params;
	if( msg && msg->anim )
	{		
		CAL3DANIMATIONTOCALLBACKMAP::iterator iter = m_AnimationCallbacks.find( msg->anim );
		if( iter != m_AnimationCallbacks.end() )
		{
			msg->instance = (*iter).second;
		}else
		{
			//create new instance
			Cal3dCallback * cb = new Cal3dCallback();
			m_AnimationCallbacks.insert( pair< CalCoreAnimation *, Cal3dCallback * >( msg->anim, cb ) );
			CalCoreAnimation * anim = msg->anim;
			anim->registerCallback( cb,0);
			msg->instance = cb;
		}		
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

DWORD Cal3DModelManager::OnSaveAllCalCubes(DWORD size, void *param)
{
	static CHashString modelType = _T("Cal3DRenderObject");
	IDTOOBJECTMAP *calMap = GetObjectMap(&modelType);
	if (calMap)
	{
		IDTOOBJECTMAP::iterator objIter = calMap->begin();
		while (objIter != calMap->end())
		{
			IObject *calObj = objIter->second;
			IHashString *objName = calObj->GetName();
			static DWORD msgHash_SaveCalCubes = CHashString(_T("SaveCalCubes")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SaveCalCubes, 0, 0, objName, &modelType );
			objIter++;
		}
	}
	return MSG_HANDLED_STOP;
}
DWORD Cal3DModelManager::OnGetCal3DModelInterface(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETCALMODELINTERFACE));
	GETCALMODELINTERFACE *gcmi = (GETCALMODELINTERFACE*) param;
	gcmi->modelRO = NULL;

	CHashString modelType = _T("Cal3DRenderObject");
	IDTOOBJECTMAP *calMap = GetObjectMap(&modelType);
	if (calMap)
	{
		IHashString *modelName = gcmi->modelName;
		IDTOOBJECTMAP::iterator objIter = calMap->find(modelName->GetUniqueID());
		if (objIter != calMap->end())
		{
			gcmi->modelRO = dynamic_cast<IRenderObject*>(objIter->second);
		}
	}
	return MSG_HANDLED_STOP;
}

CalCoreSkeleton* Cal3DModelManager::GetCoreSkeleton( IHashString *pName )
{
	// look for object
	CHashString hszComponent(_T("CCal3DCoreSkeletonCache"));
	CCal3DCoreSkeletonCache *pCache = (CCal3DCoreSkeletonCache*)GetObjectByName( pName, &hszComponent );
	if (pCache)
		return pCache->m_pCoreSkeleton;

	// not found, try to load from file
	LOADFILEEXTPARAMS lfep;
	lfep.fileName = const_cast<TCHAR*>(pName->GetString());
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(lfep), &lfep);

	// look again for object
	pCache = (CCal3DCoreSkeletonCache*)GetObjectByName( pName, &hszComponent );
	if (pCache)
		return pCache->m_pCoreSkeleton;

	// failed to load
	return NULL;
}

CalCoreAnimation* Cal3DModelManager::GetCoreAnimation( IHashString *pName )
{
	// look for object
	CHashString hszComponent(_T("CCal3DCoreAnimationCache"));
	CCal3DCoreAnimationCache *pCache = (CCal3DCoreAnimationCache*)GetObjectByName( pName, &hszComponent );
	if (pCache)
		return pCache->m_pCoreAnimation;

	// not found, try to load from file
	LOADFILEEXTPARAMS lfep;
	lfep.fileName = const_cast<TCHAR*>(pName->GetString());
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(lfep), &lfep);

	// look again for object
	pCache = (CCal3DCoreAnimationCache*)GetObjectByName( pName, &hszComponent );
	if (pCache)
		return pCache->m_pCoreAnimation;

	// failed to load
	return NULL;
}

CalCoreMesh* Cal3DModelManager::GetCoreMesh( IHashString *pName )
{
	// look for object
	CHashString hszComponent(_T("CCal3DCoreMeshCache"));
	CCal3DCoreMeshCache *pCache = (CCal3DCoreMeshCache*)GetObjectByName( pName, &hszComponent );
	if (pCache)
		return pCache->m_pCoreMesh;

	// not found, try to load from file
	LOADFILEEXTPARAMS lfep;
	lfep.fileName = const_cast<TCHAR*>(pName->GetString());
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(lfep), &lfep);

	// look again for object
	pCache = (CCal3DCoreMeshCache*)GetObjectByName( pName, &hszComponent );
	if (pCache)
		return pCache->m_pCoreMesh;

	// failed to load
	return NULL;
}

CalCoreModel* Cal3DModelManager::GetCoreModel( IHashString *pName )
{
	CCal3DCoreModelCache *pCache = (CCal3DCoreModelCache*)GetCoreModelCache(pName);
	if (pCache)
		return pCache->m_pCoreModel;
	return NULL;
}

vector<IMaterial*>* Cal3DModelManager::GetModelMaterials( IHashString *pName )
{
	CCal3DCoreModelCache *pCache = (CCal3DCoreModelCache*)GetCoreModelCache(pName);
	if (pCache)
		return &pCache->m_Materials;
	return NULL;
}

CCal3DCoreModelCache* Cal3DModelManager::GetCoreModelCache( IHashString *pName )
{
	// look for object
	CHashString hszComponent(_T("CCal3DCoreModelCache"));
	CCal3DCoreModelCache *pCache = (CCal3DCoreModelCache*)GetObjectByName( pName, &hszComponent );
	if (pCache)
		return pCache;

	// not found, try to load from file
	LOADFILEEXTPARAMS lfep;
	lfep.fileName = const_cast<TCHAR*>(pName->GetString());
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(lfep), &lfep);

	// look again for object
	pCache = (CCal3DCoreModelCache*)GetObjectByName( pName, &hszComponent );
	if (pCache)
		return pCache;

	// failed to load
	return NULL;
}

bool Cal3DModelManager::LoadHardwareAllocation( CalCoreModel *pCoreModel, IHashString *pMeshName, IHashString *pSkeletonName, HARDWARE_ALLOCATION &out_hardwareAlloc )
{
	assert( pMeshName );

	// see if has already been loaded
	HARDWARE_ALLOCATION_ID key;
	key.meshNameUniqueID = pMeshName->GetUniqueID();
	key.skeletonNameUniqueID = pSkeletonName ? pSkeletonName->GetUniqueID() : 0;
	HARDWAREALLOCATIONMAP::iterator itr = m_HardwareAllocationMap.find( key );
	if (itr != m_HardwareAllocationMap.end())
	{
		out_hardwareAlloc = itr->second.hardwareAllocation;
		itr->second.refCount++;
		return true;
	}

	assert( pCoreModel );

	REFHARDWARE_ALLOCATION refHardwareAlloc;
	HARDWARE_ALLOCATION &hardwareAllocation = refHardwareAlloc.hardwareAllocation;

	CalHardwareModel *pCalHardwareModel = new CalHardwareModel( pCoreModel );

	static VERTEX g_FastSkinBuffer[ 60000 ];
	static VERTEX_NOSKIN g_FastVertexBuffer[ 60000 ];
	static CalIndex g_FastIndexBuffer[ 60000*3 ];
	void* pVertex;
	CalIndex *pFace;

	//Put all this in a temporary buffer so we can run Mesh Mender on all the Sub Geometry
	unsigned char * pTempVBuffer = NULL;
	CalIndex * pTempIBuffer = g_FastIndexBuffer;
	int PosOffset = 0;
	int NorOffset = 0;
	int SOffset = 0;
	int TOffset = 0;
	int TanOffset = 0;
	int	sizeVertex = 0;
	pCalHardwareModel->setTextureCoordNum(1); 
	if (pCoreModel->getCoreSkeleton())
	{
		pTempVBuffer = (unsigned char*)g_FastSkinBuffer;
		sizeVertex = sizeof( VERTEX );
		pCalHardwareModel->setVertexBuffer((char*)pTempVBuffer,sizeVertex);
		pCalHardwareModel->setWeightBuffer(((char*)pTempVBuffer) + 12 ,sizeVertex);
		pCalHardwareModel->setMatrixIndexBuffer(((char*)pTempVBuffer) + 28 ,sizeVertex);  
		pCalHardwareModel->setNormalBuffer(((char*)pTempVBuffer)+44,sizeVertex);
		pCalHardwareModel->setTextureCoordBuffer(0,((char*)pTempVBuffer)+56,sizeVertex);
		pCalHardwareModel->setTangentSpaceBuffer(0, ((char*)pTempVBuffer)+64,sizeVertex );
		PosOffset = 0;
		NorOffset = 44;
		SOffset = 56;
		TOffset = 60;
		TanOffset = 64;
	}
	else
	{
		pTempVBuffer = (unsigned char*)g_FastVertexBuffer;
		sizeVertex = sizeof( VERTEX_NOSKIN );
		pCalHardwareModel->setVertexBuffer((char*)pTempVBuffer,sizeVertex);
		pCalHardwareModel->setNormalBuffer(((char*)pTempVBuffer) + 12 ,sizeVertex);
		pCalHardwareModel->setTextureCoordBuffer(0, ((char*)pTempVBuffer) + 24 ,sizeVertex);  
		pCalHardwareModel->setTangentSpaceBuffer(0, ((char*)pTempVBuffer)+32,sizeVertex );
		PosOffset = 0;
		NorOffset = 12;
		SOffset = 24;
		TOffset = 28;
		TanOffset = 32;
	}
	unsigned char * pTempVBufferOriginal = pTempVBuffer;
	CalIndex * pTempIBufferOriginal = pTempIBuffer;

	//Number of texture coords used
	pCalHardwareModel->setIndexBuffer(pTempIBuffer);
	pCalHardwareModel->load( 0, 0, MAXBONESPERMESH );  

	//Ok now that we have the vertex data loaded into a temp buffer, loop through the meshes
	int curVertexSize = 0;
	int curIndexSize = 0;
	
	std::vector< CalHardwareModel::CalHardwareMesh > &hmeshes = pCalHardwareModel->getVectorHardwareMesh();

	int vertexoffset = 0;
	int IndexOffset = 0;
	int hardwareMeshId;		
	for(hardwareMeshId = 0; 
		hardwareMeshId < pCalHardwareModel->getHardwareMeshCount();
		hardwareMeshId++ )
	{
		pCalHardwareModel->selectHardwareMesh(hardwareMeshId);

		{
			//OutputDebugString( "Child Geom Indices: " );
			int VBsize = hmeshes[ hardwareMeshId ].vertexCount;
			int IBsize = hmeshes[ hardwareMeshId ].faceCount * 3;		
			//fill verts, add to accumulating buffer
			//fill indices
			for( int i = 0; i < IBsize; i++ )
			{
				//here, vertexOffset is the base vertex index that the index starts refering to
				//so to make a valid global list of indices, we have to add to this offset
				pTempIBuffer[ IndexOffset + i ] = pTempIBuffer[ IndexOffset + i ] + vertexoffset;//copy back new indices with offsets embedded
				//char debugstr[ 1024 ];
				//sprintf( debugstr, "[%d]\t", indices[ i ] );
				//OutputDebugString( debugstr );
			}			
			//OutputDebugString( "\n" );-> 
			vertexoffset += VBsize;
 			IndexOffset += IBsize;
		}
	}
	int totalVertices = vertexoffset;
	int totalIndices = IndexOffset;
	
	//now we have to figure
	unsigned char * gBufferSpace = NULL;
	//now really create our buffer space:
	gBufferSpace = (unsigned char * )pTempVBufferOriginal;
	
	//Allocate hardware vertex buffer space

	//create new allocation
	ALLOCATERENDERBUFFERMSG msg;
	//Skinned and nonskinned versions
	if( pCoreModel->getCoreSkeleton() )
	{
		msg.ElementsToAllocate = totalVertices;
		msg.SizeVertexFormat = sizeof( VERTEX );
		msg.VertexFormat = VertexFormat;
		sizeVertex = sizeof( VERTEX );
	}else
	{
		msg.ElementsToAllocate = totalVertices;
		msg.SizeVertexFormat = sizeof( VERTEX_NOSKIN );
		msg.VertexFormat = VertexFormatNoSkin;
		sizeVertex = sizeof( VERTEX_NOSKIN );
	}

	static DWORD msgHash_AllocateVertices = CHashString(_T("AllocateVertices")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_AllocateVertices, sizeof( msg ), &msg, NULL, NULL );

	if( res == MSG_HANDLED )
	{
		hardwareAllocation.vertexBufferAllocation.VertexFormat = msg.VertexFormat;
		hardwareAllocation.vertexBufferAllocation.m_Offset = msg.m_ReturnOffset;
		hardwareAllocation.vertexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
		hardwareAllocation.vertexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
		hardwareAllocation.vertexBufferAllocation.m_Size = totalVertices;
		hardwareAllocation.vertexBufferAllocation.m_Stride = msg.SizeVertexFormat;
		hardwareAllocation.pVertexBuffer = (IVertexBufferObject*)msg.m_InterfacePointer;
	}
	else
	{
		return false;
	}
	//create new allocation
	msg.ElementsToAllocate = totalIndices;
	msg.SizeVertexFormat = sizeof( CalIndex );

	static DWORD msgHash_AllocateIndices = CHashString(_T("AllocateIndices")).GetUniqueID();
	res = m_ToolBox->SendMessage(msgHash_AllocateIndices, sizeof( msg ), &msg, NULL, NULL );
	if( res == MSG_HANDLED )
	{
		hardwareAllocation.indexBufferAllocation.m_Offset = msg.m_ReturnOffset;
		hardwareAllocation.indexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
		hardwareAllocation.indexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
		hardwareAllocation.indexBufferAllocation.m_Size = totalIndices;
		hardwareAllocation.indexBufferAllocation.m_Stride = sizeof( CalIndex );
		hardwareAllocation.pIndexBuffer = (IIndexBuffer*)msg.m_InterfacePointer;
	}
	else
	{
		return false;
	}

	//Lock the vertex buffer and index buffer
	if( !hardwareAllocation.pVertexBuffer->Lock( hardwareAllocation.vertexBufferAllocation.m_Offset,
		totalVertices, (void**)&pVertex ) )
	{
		return false;
	}
	memcpy( pVertex, gBufferSpace, hardwareAllocation.vertexBufferAllocation.m_Size*hardwareAllocation.vertexBufferAllocation.m_Stride );
	hardwareAllocation.pVertexBuffer->Unlock();

	if( !hardwareAllocation.pIndexBuffer->Lock( hardwareAllocation.indexBufferAllocation.m_Offset,
		totalIndices, (void**)&pFace ) )
	{
		return false;
	}
	memcpy( pFace, pTempIBufferOriginal, hardwareAllocation.indexBufferAllocation.m_Size*hardwareAllocation.indexBufferAllocation.m_Stride );
	hardwareAllocation.pIndexBuffer->Unlock();

	hardwareAllocation.pCalHardwareModel = pCalHardwareModel;

	// set output param
	out_hardwareAlloc = hardwareAllocation;

	// add to map
	refHardwareAlloc.refCount = 1;
	m_HardwareAllocationMap[key] = refHardwareAlloc;
	return true;
}

void Cal3DModelManager::UnloadHardwareAllocation( IHashString *pMeshName, IHashString *pSkeletonName )
{
	assert( pMeshName );

	HARDWARE_ALLOCATION_ID key;
	key.meshNameUniqueID = pMeshName->GetUniqueID();
	key.skeletonNameUniqueID = pSkeletonName ? pSkeletonName->GetUniqueID() : 0;
	HARDWAREALLOCATIONMAP::iterator itr = m_HardwareAllocationMap.find( key );
	if (itr != m_HardwareAllocationMap.end())
	{
		// unload if no longer ref'd
		if (--itr->second.refCount == 0)
		{
			static DWORD msgHash_RemoveVertexAllocation = CHashString(_T("RemoveVertexAllocation")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_RemoveVertexAllocation, sizeof(BUFFERALLOCATIONSTRUCT), &itr->second.hardwareAllocation.vertexBufferAllocation );

			static DWORD msgHash_RemoveIndexAllocation = CHashString(_T("RemoveIndexAllocation")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_RemoveIndexAllocation, sizeof(BUFFERALLOCATIONSTRUCT), &itr->second.hardwareAllocation.indexBufferAllocation );

			SAFE_DELETE( itr->second.hardwareAllocation.pCalHardwareModel );

			m_HardwareAllocationMap.erase( itr );
		}
	}
}

DWORD Cal3DModelManager::OnDestroyRenderer(DWORD size, void *param)
{
	DeInit();
	return MSG_HANDLED_PROCEED;
}