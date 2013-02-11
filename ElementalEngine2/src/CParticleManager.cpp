///============================================================================
/// \file	CParticleManager.cpp
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


REGISTER_COMPONENT_SINGLETON( CParticleManager );

REGISTER_MESSAGE_HANDLER(GetParticleType, OnGetParticleType, CParticleManager );
REGISTER_MESSAGE_HANDLER(GetParticleTypeNoLoad, OnGetParticleTypeNoLoad, CParticleManager );
REGISTER_MESSAGE_HANDLER(GetParticleEmitterType, OnGetEmitterType, CParticleManager );
REGISTER_MESSAGE_HANDLER(ReloadParticle, OnReloadParticle, CParticleManager );
REGISTER_MESSAGE_HANDLER(ReloadAllParticles, OnReloadAllParticles, CParticleManager );
REGISTER_MESSAGE_HANDLER(RenameParticleBrush, OnRenameParticleBrush, CParticleManager);
REGISTER_MESSAGE_HANDLER(RenameParticleEmitter, OnRenameParticleEmitter, CParticleManager);
//REGISTER_MESSAGE_HANDLER(EditorAddNewBrush, OnEditorAddNewBrush, CParticleManager);
//REGISTER_MESSAGE_HANDLER(EditorAddNewEmitter, OnEditorAddNewEmitter, CParticleManager);
REGISTER_MESSAGE_HANDLER(EditorDisplay, OnEditorDisplay, CParticleManager);
REGISTER_MESSAGE_HANDLER(AddLoadedEmitter, OnAddLoadedEmitter, CParticleManager);
REGISTER_MESSAGE_HANDLER(AddLoadedBrush, OnAddLoadedBrush, CParticleManager);

void ExtractFileName(TCHAR * extractedfilename, const TCHAR * filepath)
{
//	TCHAR filepath[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	_tsplitpath( filepath, drive, dir, fname, ext );

	StdString szOutput;
	szOutput += fname;
	szOutput += ext;

	_tcscpy(extractedfilename, szOutput);
}

void CParticleManager::RandomBuildNormalVectorTable()
{
	for (int i = 0; i < MAX_RANDOM_NORMAL_VECTORS; i++)
	{
		float d = 0.0f;
		Vec3 &v = m_v3RandomNormalVectorTable[i];
		do {
			v.x = float(rand()) / RAND_MAX * 2.0f - 1.0f;
			v.y = float(rand()) / RAND_MAX * 2.0f - 1.0f;
			v.z = float(rand()) / RAND_MAX * 2.0f - 1.0f;
		d = v.x*v.x + v.y*v.y + v.z*v.z;
		} while (d < 0.9f*0.9f || d > 1.1f*1.1f);
		v.Normalize();
	}
}

void CParticleManager::RandomBuildNormalXZDiskTable()
{
	for (int i = 0; i < MAX_RANDOM_NORMAL_XZDISK; i++)
	{
		float d = 0.0f;
		Vec3 &v = m_v3RandomNormalXZDiskTable[i];
		do {
			v.x = float(rand()) / RAND_MAX * 2.0f - 1.0f;
			v.y = 0.0;
			v.z = float(rand()) / RAND_MAX * 2.0f - 1.0f;
		d = v.x*v.x + v.z*v.z;
		} while (d < 0.9f*0.9f || d > 1.1f*1.1f);
		v.Normalize();
	}
}
void CParticleManager::RandomBuildXZDiskTable()
{
	for (int i = 0; i < MAX_RANDOM_XZDISK; i++)
	{
		float d = 0.0f;
		Vec3 &v = m_v3RandomXZDiskTable[i];
		do {
			v.x = (float(rand()) / RAND_MAX) * 2.0f - 1.0f;
			v.y = 0.0;
			v.z = (float(rand()) / RAND_MAX) * 2.0f - 1.0f;
			d = v.x*v.x + v.z*v.z;
		} while (d > 1.0f);
	}
}


void CParticleManager::RandomBuildFloatTable()
{
	for (int i = 0; i < MAX_RANDOM_FLOATS; i++)
	{
		float &f = m_fRandomFloatTable[i];
		f = (rand()%10000)/10000.0f;
	}
}

/// \brief The Default Constructor for CParticleManager
/// \return void
CParticleManager::CParticleManager() : 
	CManager(_T("CParticleManager"), PARTICLEPRIORITY, PARTICLEPRIORITY),
	m_szParticleLineTypeName(_T("CParticleLineType")), 
	m_szParticleRibbonTypeName(_T("CParticleRibbonType")), 
	m_szParticleEmitterTypeName(_T("CParticleEmitterType")),
	m_szParticleLineRenderObjectName(_T("CParticleLineRenderObject")),
	m_szParticleRibbonRenderObjectName(_T("CParticleRibbonRenderObject")),
	m_szParticleRenderObjectName(_T("CParticleRenderObject")),
	m_szParticleEmitterObjectName(_T("CParticleEmitterObject")),
	m_szParticleSystemObjectName(_T("CParticleSystemObject")),
	m_ccamTypeName(_T("CCamera"))
{
	m_Dynamic2DVertexBuffer = NULL;
	m_bInitialized = false;
	m_bEditorDisplay = false;
	m_pEditorContext = NULL;
	m_BatchRenderObject = NULL;
	
	Init();
}


/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CParticleManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CParticleManager );
}

/// \brief The Default Destructor for CParticleManager
/// \return void

CParticleManager::~CParticleManager( )
{
	DeInit();
}
// Initializes the manager
bool CParticleManager::Init()
{
	if( !m_bInitialized )
	{
		//Initialize 2D vertex buffer
		//create new allocation
		CHANNELDESCRIPTORLIST VertexFormatParticles;
		VertexFormatParticles.push_back( MESH_STREAM_VERTEX_POSITION3 ); // position
		VertexFormatParticles.push_back( MESH_STREAM_VERTEX_TEXTURECOORD1 ); // texcoord
		VertexFormatParticles.push_back( MESH_STREAM_VERTEX_NORMAL ); // position offset
		VertexFormatParticles.push_back( MESH_STREAM_VERTEX_TANGENT4 ); // rotation
		VertexFormatParticles.push_back( MESH_STREAM_VERTEX_COLORCRGBA ); // color

		ALLOCATERENDERBUFFERMSG msg;	
		UINT Max2DAllocation = MAX_PARTICLE_BUFFER;
		msg.ElementsToAllocate = Max2DAllocation;
		msg.SizeVertexFormat = sizeof( PARTICLE_VERTEX );
		msg.VertexFormat = VertexFormatParticles;
		msg.Dynamic = true;
		msg.m_AllowSharedBuffer = false;

		static DWORD msgHash_AllocateVertices = CHashString(_T("AllocateVertices")).GetUniqueID();
		DWORD res = EngineGetToolBox()->SendMessage(msgHash_AllocateVertices, sizeof( msg ), &msg, NULL, NULL );

		if( res == MSG_HANDLED )
		{
			m_Dynamic2DVertexBufferAllocation.m_Offset = msg.m_ReturnOffset;
			m_Dynamic2DVertexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
			m_Dynamic2DVertexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
			m_Dynamic2DVertexBufferAllocation.m_Size = Max2DAllocation;
			m_Dynamic2DVertexBufferAllocation.m_Stride = msg.SizeVertexFormat;
			m_Dynamic2DVertexBuffer = (IVertexBufferObject*)msg.m_InterfacePointer;
		}			
		else
		{	
			//return false;
		}

		msg.ElementsToAllocate = MAX_PARTICLE_COUNT * 6;
		msg.Dynamic = false;
		msg.m_AllowSharedBuffer = false;
		static DWORD msgHash_AllocateIndices = CHashString(_T("AllocateIndices")).GetUniqueID();
		res = EngineGetToolBox()->SendMessage(msgHash_AllocateIndices, sizeof( msg ), &msg, NULL, NULL );
		if( res == MSG_HANDLED )
		{
			m_IndexBufferAllocation.m_Offset = msg.m_ReturnOffset;
			m_IndexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
			m_IndexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
			m_IndexBufferAllocation.m_Size = msg.ElementsToAllocate;
			m_IndexBufferAllocation.m_Stride = sizeof(short);
			m_IndexBuffer = (IIndexBuffer*)msg.m_InterfacePointer;
			if (m_IndexBuffer)
			{
				unsigned short *pIndices = NULL;
				m_IndexBuffer->Lock( 0, 0, (void**)&pIndices );
				if (pIndices)
				{
					int iParticle = 0;
					for (unsigned int i=0; i < msg.ElementsToAllocate; i+=6, iParticle+=4)
					{
						pIndices[i+0] = iParticle+0;
						pIndices[i+1] = iParticle+1;
						pIndices[i+2] = iParticle+2;
						pIndices[i+3] = iParticle+2;
						pIndices[i+4] = iParticle+1;
						pIndices[i+5] = iParticle+3;
					}
				}
				m_IndexBuffer->Unlock();
			}
		}			
		else
		{	
			//return false;
		}

		m_bInitialized = true;
	}

	static CHashString hszName(_T("CManagerParticleBatchRenderObject"));
	static CHashString hszComponentType(_T("CParticleBatchRenderObject"));
	m_BatchRenderObject = dynamic_cast<CParticleBatchRenderObject *>(
		m_ToolBox->CreateComponent(&hszComponentType, 3, &hszName, NULL, false));

	OFACTORYADDPARAMS ofap;
	ofap.name = &hszName;
	ofap.component = m_BatchRenderObject;
	static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
	DWORD result = m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(OFACTORYADDPARAMS), &ofap);
	if (result != MSG_HANDLED)
		m_ToolBox->Log(LOGERROR, _T("%s(%d): Failed to add Particle Batch Render Object  %s to object factory.\n"), __FILE__, __LINE__, hszName.GetString());

	m_BatchRenderObject->Init();
	

	m_iRandomNormalIndex = 0;
	m_iRandomFloatIndex = 0;
	m_iRandomNormalXZDiskIndex = 0;
	m_iRandomXZDiskIndex = 0;
	RandomBuildNormalVectorTable();
	RandomBuildFloatTable();
	RandomBuildNormalXZDiskTable();
	RandomBuildXZDiskTable();
	return true;
}

DWORD CParticleManager::OnAddLoadedEmitter(DWORD size, void *params)
{
	// make sure the size is correct
	VERIFY_MESSAGE_SIZE(size, sizeof(PARTICLELOADINFO));

	// extract the SCRIPTPARAMS from the void pointer
	PARTICLELOADINFO *pli = (PARTICLELOADINFO *)params;

	DWORD result;
	
	CHashString hszFilepath = pli->filepath;
//	TCHAR czFilename[_MAX_PATH];
//	ExtractFileName(czFilename, hszFilepath.GetString());
//	CHashString hszFilename(czFilename);
	CHashString hszComponentType = _T("CParticleEmitter");

	// create object (global)
	CParticleEmitter *pParticleEmitter = dynamic_cast<CParticleEmitter *>(
		m_ToolBox->CreateComponent(&hszComponentType, 3, &hszFilepath, NULL, false));

	// add the emitter filepath and body to the map
	AddEmitterToLibrary(hszFilepath.GetUniqueID(), pParticleEmitter);

	// add object to object factory
	OFACTORYADDPARAMS ofap;
	ofap.name = &hszFilepath;
	ofap.component = pParticleEmitter;
	static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(OFACTORYADDPARAMS), &ofap);
	if (result != MSG_HANDLED)
		m_ToolBox->Log(LOGERROR, _T("%s(%d): Failed to add Emitter %s to object factory.\n"), __FILE__, __LINE__, hszFilepath.GetString());

	// serialize object
	pParticleEmitter->Serialize(*pli->data);

	// init object
	pParticleEmitter->Init();

	return MSG_HANDLED_PROCEED;
}
DWORD CParticleManager::OnAddLoadedBrush(DWORD size, void *params)
{
	// make sure the size is correct
	VERIFY_MESSAGE_SIZE(size, sizeof(PARTICLELOADINFO));

	// extract the SCRIPTPARAMS from the void pointer
	PARTICLELOADINFO *pli = (PARTICLELOADINFO *)params;

	DWORD result;

	CHashString hszFilepath = pli->filepath;
//	TCHAR czFilename[_MAX_PATH];
//	ExtractFileName(czFilename, hszFilepath.GetString());
//	CHashString hszFilename(czFilename);
	CHashString hszComponentType = _T("CParticleType");
	

	// create object (global)
	CParticleType *pParticleBrush = dynamic_cast<CParticleType *>(
		m_ToolBox->CreateComponent(&hszComponentType, 3, &hszFilepath, NULL, false));

	// add the brush filepath and body to the map
	AddBrushToLibrary(hszFilepath.GetUniqueID(), pParticleBrush);
	
	// add object to object factory
	OFACTORYADDPARAMS ofap;
	ofap.name = &hszFilepath;
	ofap.component = pParticleBrush;
	static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(OFACTORYADDPARAMS), &ofap);
	if (result != MSG_HANDLED)
		m_ToolBox->Log(LOGERROR, _T("%s(%d): Failed to add Brush %s to object factory.\n"), __FILE__, __LINE__, hszFilepath.GetString());

	// serialize object
	pParticleBrush->Serialize(*pli->data);

	// init object
	pParticleBrush->Init();


	return MSG_HANDLED_PROCEED;
}

DWORD CParticleManager::OnRenameParticleEmitter(DWORD size, void *param)
{
	PARTICLEFILEPARAMS * pfp;
	VERIFY_MESSAGE_SIZE(sizeof(PARTICLEFILEPARAMS), size);
	pfp = (PARTICLEFILEPARAMS *)param;
	if (pfp == NULL)
		return MSG_ERROR;

	CHashString hszComponentType( _T("CParticleEmitter") );
	
	if (!pfp->filename)
		return MSG_ERROR;
	CHashString hszOldName( pfp->filename );

	if (!pfp->newfilename)
		return MSG_ERROR;
	CHashString hszNewName( pfp->newfilename );

	StdString szNewName(pfp->newfilename->GetString());

	EMITTERTYPES::iterator iter;
	if( m_EmitterTypeLibrary.size() > 0 )
	{
		iter = m_EmitterTypeLibrary.find( pfp->filename->GetUniqueID() );
	}
	// Empty Emitter Library
	else
		return MSG_ERROR;

	// Emitter found
	if (m_EmitterTypeLibrary.size() > 0 && iter != m_EmitterTypeLibrary.end() )
	{
		IParticleEmitter * temp;
		
		// Save off pointer to emitter
		if (iter->second)
			temp = iter->second;
		else
			return MSG_ERROR;

		// Remove entity from library
		m_EmitterTypeLibrary.erase( iter );

		// Reinsert Particle Emitter in Library, replacing ID
		m_EmitterTypeLibrary.insert( EMITTERTYPESPAIR( hszNewName.GetUniqueID(),
				(IParticleEmitter*)temp ) );

		// Rename the Emitter in its own data.
		temp->SetFilename(&szNewName);

		// Rename the Emitter in the Heirarchy
		CHANGEOBJNAME con;
		con.compType = &hszComponentType;
		con.oldName = &hszOldName;
		con.newName = &hszNewName;
		static DWORD msgHash_RenameObject = CHashString(_T("RenameObject")).GetUniqueID();
		DWORD result = m_ToolBox->SendMessage(msgHash_RenameObject, sizeof(CHANGEOBJNAME), &con, NULL, NULL);
		if (result != MSG_HANDLED)
			return MSG_ERROR;

    	return MSG_HANDLED_STOP;
	}
	// You shouldn't be calling to rename Emitters that don't exist.
	else
	{
		m_ToolBox->Log(LOGERROR, _T("%s(%d): Rename attempted on non-existant emitter.\n"), __FILE__, __LINE__);
		return MSG_ERROR;
	}
}

DWORD CParticleManager::OnRenameParticleBrush(DWORD size, void *param)
{
	PARTICLEFILEPARAMS * pfp;
	VERIFY_MESSAGE_SIZE(sizeof(PARTICLEFILEPARAMS), size);
	pfp = (PARTICLEFILEPARAMS *)param;
	if (pfp == NULL)
		return MSG_ERROR;

	CHashString hszComponentType( _T("CParticleType") );
	
	if (!pfp->newfilename)
		return MSG_ERROR;
	CHashString hszNewName( pfp->newfilename );

	if (!pfp->filename)
		return MSG_ERROR;
	CHashString hszOldName( pfp->filename );

	StdString szNewName(pfp->newfilename->GetString());

	PARTICLETYPES::iterator iter;
	if( m_ParticleTypeLibrary.size() > 0 )
	{
		iter = m_ParticleTypeLibrary.find( pfp->filename->GetUniqueID() );
	}
	// Empty Particle Library
	else
		return MSG_ERROR;

	// Particle found
	if (m_ParticleTypeLibrary.size() > 0 && iter != m_ParticleTypeLibrary.end() )
	{
		IParticleType * temp;
		
		// Save off pointer to Particle
		if (iter->second)
			temp = iter->second;
		else
			return MSG_ERROR;

		// Remove entity from library
		m_ParticleTypeLibrary.erase( iter );

		// Reinsert Particle Brush in Library, replacing ID
		m_ParticleTypeLibrary.insert( PARTICLETYPESPAIR( hszNewName.GetUniqueID(),
				(IParticleType*)temp ) );

		// Rename the Brush in its own data.
		temp->SetFilename(&szNewName);

		// Rename the Brush in the Heirarchy
		CHANGEOBJNAME con;
		con.compType = &hszComponentType;
		con.oldName = &hszOldName;
		con.newName = &hszNewName;
		static DWORD msgHash_RenameObject = CHashString(_T("RenameObject")).GetUniqueID();
		DWORD result = m_ToolBox->SendMessage(msgHash_RenameObject, sizeof(CHANGEOBJNAME), &con, NULL, NULL);
		if (result != MSG_HANDLED)
			return MSG_ERROR;

    	return MSG_HANDLED_STOP;
	}
	// You shouldn't be calling to rename Brushs that don't exist.
	else
	{
		return MSG_ERROR;
	}
}
/// \brief	Update function for the CParticleManager
///			Renders immediately, the queued list of renderobjects
/// \return	void
void CParticleManager::Update( DWORD tickCount  )
{	
	if( m_bInitialized )
	{
		// PARTICLES THAT RELY ON MANAGER TO DO BATCHING
		// Manual Particle Buffer Management
		// CParticleRenderObjects & CParticleEmitterObjects
		IDTOOBJECTMAP::iterator objIter;
		IDTOOBJECTMAP * CParticleSystemObjectMap = NULL;
		IDTOOBJECTMAP * CParticleRenderObjectMap = NULL;
		IDTOOBJECTMAP * CParticleEmitterObjectMap = NULL;
		stack< IObject * >			deadParticles;
		IObject *					pObject	= NULL;
		int curoffset = m_Dynamic2DVertexBufferAllocation.m_Offset;
       
		CParticleSystemObjectMap = GetObjectMap( &m_szParticleSystemObjectName );
		CParticleRenderObjectMap = GetObjectMap( &m_szParticleRenderObjectName );
		CParticleEmitterObjectMap = GetObjectMap( &m_szParticleEmitterObjectName );
		if( CParticleRenderObjectMap || CParticleEmitterObjectMap)
		{		
			// Run Update on all Particle Systems
			if ( CParticleSystemObjectMap )
			{
				for( objIter = CParticleSystemObjectMap->begin(); objIter != CParticleSystemObjectMap->end(); ++objIter )
				{
					pObject = objIter->second;
					if( !pObject->Update( ) )
					{
						deadParticles.push( pObject );
					}
				}
			}
			// Run Update (a.k.a particle keyframe iteration) on all Particle Emitters 
			// (CParticleRenderObject)
			if ( CParticleRenderObjectMap )
			{
				for( objIter = CParticleRenderObjectMap->begin(); objIter != CParticleRenderObjectMap->end(); ++objIter )
				{
					pObject = objIter->second;
					if( !pObject->Update( ) )
					{
						deadParticles.push( pObject );
					}			
				}
			}
			// (CParticleEmitterObject)
			if ( CParticleEmitterObjectMap )
			{
				for( objIter = CParticleEmitterObjectMap->begin(); objIter != CParticleEmitterObjectMap->end(); ++objIter )
				{
					pObject = objIter->second;
					if( !pObject->Update( ) )
					{
						deadParticles.push( pObject );
					}			
				}
			}
			// Lock and Fill VertexBuffer for all Particle Emitters
			if( m_Dynamic2DVertexBuffer )
			{
				void * pDest = NULL;
				
				m_Dynamic2DVertexBuffer->Lock( m_Dynamic2DVertexBufferAllocation.m_Offset,
												m_Dynamic2DVertexBufferAllocation.m_Size,
												(void**)&pDest,
												VBO_LOCK_DISCARD
												);
				// (CParticleRenderObjects)
				if ( CParticleRenderObjectMap )
				{
					CParticleRenderObject * pParticleRenderObject;
					for( objIter = CParticleRenderObjectMap->begin();objIter != CParticleRenderObjectMap->end(); ++objIter )
					{
						pParticleRenderObject = (CParticleRenderObject *)(objIter->second);
						curoffset += pParticleRenderObject->UpdateBuffer( (char**)&pDest, curoffset, m_Dynamic2DVertexBuffer );
					}
				}
				// (CParticleEmitterObjects)
				if ( CParticleEmitterObjectMap )
				{
					CParticleEmitterObject * pParticleEmitterObject;
					for( objIter = CParticleEmitterObjectMap->begin();objIter != CParticleEmitterObjectMap->end(); ++objIter )
					{
						pParticleEmitterObject = (CParticleEmitterObject *)(objIter->second);
						curoffset += pParticleEmitterObject->UpdateBuffer( (char**)&pDest, curoffset, m_Dynamic2DVertexBuffer, m_IndexBuffer );
					}
				}
				m_Dynamic2DVertexBuffer->Unlock();	//TODO: MARV COMMENT, SUSPECT: Don't really have to unlock I don't think since lock is full featured
			}
		}


		// PARTICLES THAT USE BATCHING SYSTEM
		// CParticleLineRenderObject - Run Update Functions & Push all dead particles for Delete:
		// For these Objects Rendering is done in CParticleBatchRenderObject.
		IDTOOBJECTMAP *				BatchedObjectMap = NULL;
		BatchedObjectMap = GetObjectMap( &m_szParticleLineRenderObjectName );
		if( BatchedObjectMap )
		{		
			for( objIter = BatchedObjectMap->begin();objIter != BatchedObjectMap->end(); ++objIter )
			{
				pObject = objIter->second;
				if( !pObject->Update() )
				{
					deadParticles.push( pObject );
				}			
			}
		}
		// CParticleRibbonRenderObject - Run Update Functions & Push all dead particles for Delete:
		BatchedObjectMap = GetObjectMap( &m_szParticleRibbonRenderObjectName );
		if( BatchedObjectMap )
		{		
			for( objIter = BatchedObjectMap->begin();objIter != BatchedObjectMap->end(); ++objIter )
			{
				pObject = objIter->second;
				if( !pObject->Update() )
				{
					deadParticles.push( pObject );
				}			
			}
		}
		// DELETE ALL DEAD PARTICLES
		// CParticle RenderObjects EmitterObjects LineRenderObjects & RibbonRenderObjects
		// Delete Dead Particle Emitters (Both CParticleEmitterObjects and CParticleRenderObjects use this buffer)
		DELETEOBJECTPARAMS			dop;						
		while( !deadParticles.empty() )
		{			
			pObject = deadParticles.top();
			deadParticles.pop();
			dop.name = pObject->GetName();
			static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof( DELETEOBJECTPARAMS ), &dop, NULL, NULL );
		}
	} // if( m_bInitialized )
}
/// \brief	DeInitializes the CParticleManager
/// \return	bool: Success
bool CParticleManager::DeInit( void )
{	

	m_ParticleTypeLibrary.clear();
	m_EmitterTypeLibrary.clear();

//	DELETEOBJECTPARAMS dop;
//	CHashString hszName = _T("CManagerParticleBatchRenderObject");
//	dop.name = &hszName;
//	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
//	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof( DELETEOBJECTPARAMS ), &dop, NULL, NULL );


	return CManager::DeInit();
}

/// \brief Searches through library for Emitter and Returns it.
IParticleEmitter * CParticleManager::GetEmitterFromLibrary( DWORD id )
{
//	assert(id);

	EMITTERTYPES::iterator iter;
	if( m_EmitterTypeLibrary.size() > 0 )
	{
		iter = m_EmitterTypeLibrary.find( id );
	}

	if( m_EmitterTypeLibrary.size() > 0 && iter != m_EmitterTypeLibrary.end() )
	{
		// Found, return;
		return (*iter).second;
	}
	else
		return NULL;
}
/// \brief Searches through library for Brush and Returns it
IParticleType * CParticleManager::GetBrushFromLibrary( DWORD id )
{
//	assert(id);

	PARTICLETYPES::iterator iter;
	if( m_ParticleTypeLibrary.size() > 0 )
	{
		iter = m_ParticleTypeLibrary.find( id );
	}

	if( m_ParticleTypeLibrary.size() > 0 && iter != m_ParticleTypeLibrary.end() )
	{
		// Found, return;
		return (*iter).second;
	}
	else
		return NULL;
}



// Searches through the library to see if Brush Exists, if not, adds it.
void CParticleManager::AddEmitterToLibrary( DWORD id, IParticleEmitter * pEmitter)
{
	assert(id);
	assert(pEmitter);

	EMITTERTYPES::iterator iter;

	if( m_EmitterTypeLibrary.size() > 0 )
	{
		iter = m_EmitterTypeLibrary.find( id );
	}
	// If not found, add it:
	if( !( m_EmitterTypeLibrary.size() > 0 && iter != m_EmitterTypeLibrary.end() ) )
	{
		m_EmitterTypeLibrary.insert( EMITTERTYPESPAIR( id, pEmitter ) );
	}

	return;
}


void CParticleManager::AddBrushToLibrary( DWORD id, IParticleType * pBrush)
{
	assert(id);
	assert(pBrush);

	PARTICLETYPES::iterator iter;
	if( m_ParticleTypeLibrary.size() > 0 )
	{
		iter = m_ParticleTypeLibrary.find( id );
	}
	// If not found, add it:
	if( !( m_ParticleTypeLibrary.size() > 0 && iter != m_ParticleTypeLibrary.end() ) )
	{
		m_ParticleTypeLibrary.insert( PARTICLETYPESPAIR( id, pBrush ) );
	}

	return;
}

IParticleType * CParticleManager::LoadParticleTypeFromLibrary( IHashString * filepath )
{
	DWORD result;
	IParticleType*	pReturnBrush = NULL;

	if( filepath && m_bInitialized )
	{
		// Check if file is in manager library, the library prevents
		// extensive message calls to the loader saver.
		pReturnBrush = GetBrushFromLibrary(filepath->GetUniqueID());
		if (pReturnBrush)
			return pReturnBrush;
		// File is not in the library, send message to loadersaver to 
		// load the file.
		else
		{
			//not found, try loading
			CHashString hszCompType(_T("CParticleLoader"));
			static DWORD msgHash_LoadBrushFile = CHashString(_T("LoadBrushFile")).GetUniqueID();
			result = m_ToolBox->SendMessage(msgHash_LoadBrushFile, sizeof(TCHAR *), (void *)filepath->GetString(), 0, &hszCompType);
			// Fail out if unable to load file.
			if (result != MSG_HANDLED)
			{
				m_ToolBox->Log(LOGERROR, _T("%s(%d): Failed to load Brush \"%s\" from file.\n"), __FILE__, __LINE__, filepath->GetString());
				return NULL;
			}

			pReturnBrush = GetBrushFromLibrary(filepath->GetUniqueID());
			if (pReturnBrush)
				return pReturnBrush;
			else
			{
				m_ToolBox->Log(LOGERROR, _T("%s(%d): Manager failed to find Brush \"%s\" in Internal Map\n"), __FILE__, __LINE__, filepath->GetString());
				return NULL;
			}
		}
	}
	return NULL;
}

IParticleEmitter * CParticleManager::LoadEmitterType( IHashString * filepath )
{
	DWORD result;
	IParticleEmitter * pReturnEmitter = NULL;

	if( filepath && m_bInitialized )
	{
		// Check if file is in manager library, the library prevents
		// extensive message calls to the loader saver.
		pReturnEmitter = GetEmitterFromLibrary (filepath->GetUniqueID());
		if (pReturnEmitter)
			return pReturnEmitter;
		else
		{
			//not found, try loading
			TCHAR czTempFilepath[_MAX_PATH];
			_tcscpy(czTempFilepath, filepath->GetString());
			LOADFILEEXTPARAMS lfep;
			lfep.fileName = czTempFilepath;
			lfep.bInternalLoad = true;
			static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
			result = m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep, 0, 0);
			// Fail out if unable to load file.
			if (result != MSG_HANDLED)
			{
				m_ToolBox->Log(LOGERROR, _T("%s(%d): Failed to load Emitter \"%s\" from file.\n"), __FILE__, __LINE__, filepath->GetString());
				return NULL;
			}

			pReturnEmitter = GetEmitterFromLibrary (filepath->GetUniqueID());
			if (pReturnEmitter)
				return pReturnEmitter;
			else
			{
				m_ToolBox->Log(LOGERROR, _T("%s(%d): Failed to find Emitter in Internal Map\n"), __FILE__, __LINE__);
				return NULL;
			}
		}
	}
	if (!m_bInitialized)
		m_ToolBox->Log(LOGERROR, _T("%s(%d): ParticleManager attempted to load a particle while not Initailized.\n"), __FILE__, __LINE__);
	if (!filepath)
		m_ToolBox->Log(LOGERROR, _T("%s(%d): LoadEmitterFile passed a NULL filepath.\n"), __FILE__, __LINE__);
	return NULL;
}

DWORD CParticleManager::OnGetEmitterType( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETPARTICLEEMITTERMESSAGE ) );
	GETPARTICLEEMITTERMESSAGE * msg = (GETPARTICLEEMITTERMESSAGE*)params;

	assert(msg);
	assert(msg->filename);

	msg->emittertype = LoadEmitterType( msg->filename );
	if( msg->emittertype )
			return MSG_HANDLED_STOP;
	return MSG_ERROR;
}

DWORD CParticleManager::OnGetParticleType( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETPARTICLETYPEMESSAGE ) );
	GETPARTICLETYPEMESSAGE * msg = (GETPARTICLETYPEMESSAGE*)params;
	
	assert(msg);
	assert(msg->filename);

	msg->particletype = LoadParticleTypeFromLibrary( msg->filename );
		if( msg->particletype )
			return MSG_HANDLED_STOP;
	return MSG_ERROR;
}

DWORD CParticleManager::OnGetParticleTypeNoLoad( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETPARTICLETYPEMESSAGE ) );
	GETPARTICLETYPEMESSAGE * msg = (GETPARTICLETYPEMESSAGE*)params;
	
	assert(msg);
	assert(msg->filename);

	msg->particletype = GetBrushFromLibrary( msg->filename->GetUniqueID() );
		if( msg->particletype )
			return MSG_HANDLED_STOP;
	return MSG_ERROR;
}

DWORD CParticleManager::OnEditorDisplay( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( PARTICLEEDITORRENDERMESSAGE ) );
	PARTICLEEDITORRENDERMESSAGE * msg = ( PARTICLEEDITORRENDERMESSAGE * )params;
	m_bEditorDisplay = msg->render;
	m_pEditorContext = msg->rendercontext;

	return MSG_HANDLED_STOP;
}

DWORD CParticleManager::OnReloadParticle( DWORD size, void *params)
{
	return MSG_HANDLED_STOP;
}

DWORD CParticleManager::OnReloadAllParticles( DWORD size, void *params)
{
	DELETEOBJECTPARAMS dop;

	static CHashString peName(_T("CParticleEmitter"));
	static CHashString peType(_T("CParticleType"));
	// Object Type Names that this manager manages that we will clean up
	static IHashString *vManagedObjectNames[] = {
		&peName,
		&peType,
		NULL
	};

	// Call DeleteObject on every object we manage
	IDTOOBJECTMAP *pNAMETOOBJECTMAP = NULL;
	for (int i=0; vManagedObjectNames[i] != NULL; i++) 
	{ 
		IDTOOBJECTMAP *pObjMap = GetObjectMap( vManagedObjectNames[i] ); 
		if (!pObjMap) 
			continue; 

		IDTOOBJECTMAP::iterator itr = pObjMap->begin();
		while (pObjMap->begin() != pObjMap->end())
		{
			IObject *delObj = itr->second;
			dop.name = delObj->GetName();
			static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof( DELETEOBJECTPARAMS ), &dop, NULL, NULL );
			itr = pObjMap->begin();
		}
	}
	m_ParticleTypeLibrary.clear();
	m_EmitterTypeLibrary.clear();


	// Reinit all CParticleRenderObjects to restore their internal Emitter
	// Pointers and reload their emitter files.
	static CHashString hszParticleRenderObject(_T("CParticleRenderObject"));
	IDTOOBJECTMAP * objMap = GetObjectMap(&hszParticleRenderObject);
	IDTOOBJECTMAP::iterator ntomIter;
	for (ntomIter = objMap->begin(); ntomIter != objMap->end(); ++ntomIter);
	{
		IObject *particleObj;
		particleObj = ntomIter->second;
		particleObj->Init();
	}

	return true;
}

const IObject* CParticleManager::FindParticleType( IHashString *pTypeName, IHashString *pTypeTypeName )
{
	IDTOOBJECTMAP *objMap = GetObjectMap( pTypeTypeName );
	if (objMap)
	{
		IDTOOBJECTMAP::iterator itr = objMap->find( pTypeName->GetUniqueID() );
		if (itr != objMap->end())
		{
			return itr->second;
		}
	}
	return NULL;
}

const IObject* CParticleManager::LoadParticleType( IHashString *pTypeName, IHashString *pTypeTypeName )
{
	const IObject *pObject = FindParticleType(pTypeName, pTypeTypeName);
	if (pObject)
		return pObject;

	// not found, try loading
	LOADFILEEXTPARAMS lfep;
	lfep.fileName = (TCHAR*)pTypeName->GetString();
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep) != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGERROR, _T("%s(%d): Failed to load particle type(%s) \"%s\" from file.\n"), __FILE__, __LINE__, pTypeTypeName->GetString(), pTypeName->GetString());
		return NULL;
	}

	return FindParticleType(pTypeName, pTypeTypeName);
}

//
// LINE FUNCTIONS
//
PARTICLETYPETOPARTICLELINE* CParticleManager::GetParticleLineBatches( void )
{
	return &m_BatchDrawParticleLineMap;
}


void CParticleManager::AddParticleLine( CParticleLineRenderObject* particleObject )
{
	PARTICLETYPETOPARTICLELINE::iterator it;
	it = m_BatchDrawParticleLineMap.find( particleObject->GetParticleLineType() );
	if( it == m_BatchDrawParticleLineMap.end() )
	{
		const CParticleLineType *particleType = particleObject->GetParticleLineType();
		if (particleType)
			m_BatchDrawParticleLineMap[particleType][particleObject->GetName()->GetUniqueID()] = particleObject;
	}
	else
	{
		it->second[particleObject->GetName()->GetUniqueID()] =  particleObject;
	}
}

void CParticleManager::RemoveParticleLine( CParticleLineRenderObject* particleObject )
{
	PARTICLETYPETOPARTICLELINE::iterator mapit;
	mapit = m_BatchDrawParticleLineMap.find( particleObject->GetParticleLineType() );
	if( mapit != m_BatchDrawParticleLineMap.end() )
	{
		PARTICLELINESMAP::iterator it;
		it = mapit->second.find( particleObject->GetName()->GetUniqueID() );
		if( it != mapit->second.end() )
		{
			mapit->second.erase( it );
		}

		if( mapit->second.size() == 0 )
		{
			m_BatchDrawParticleLineMap.erase( mapit );
		}
	}
}

//
// EMITTER FUNCTIONS
//
PARTICLETYPETOPARTICLEEMITTER* CParticleManager::GetParticleEmitterBatches( void )
{
	return &m_BatchDrawParticleEmitterMap;
}

void CParticleManager::AddParticleEmitter( CParticleEmitterObject* particleObject )
{
	PARTICLETYPETOPARTICLEEMITTER::iterator iter;
	iter = m_BatchDrawParticleEmitterMap.find( particleObject->GetParticleEmitterData() );
	if( iter == m_BatchDrawParticleEmitterMap.end() )
	{
		const CParticleEmitterType *particleType = particleObject->GetParticleEmitterData();
		if (particleType)
			m_BatchDrawParticleEmitterMap[particleType][particleObject->GetName()->GetUniqueID()] = particleObject;
	}
	else
	{
		iter->second[particleObject->GetName()->GetUniqueID()] =  particleObject;
	}
}

void CParticleManager::RemoveParticleEmitter( CParticleEmitterObject* particleObject )
{
	PARTICLETYPETOPARTICLEEMITTER::iterator mapiter;
	mapiter = m_BatchDrawParticleEmitterMap.find( particleObject->GetParticleEmitterData() );
	if( mapiter != m_BatchDrawParticleEmitterMap.end() )
	{
		PARTICLEEMITTERSMAP::iterator iter;
		iter = mapiter->second.find( particleObject->GetName()->GetUniqueID() );
		if( iter != mapiter->second.end() )
		{
			mapiter->second.erase( iter );
		}

		if( mapiter->second.size() == 0 )
		{
			m_BatchDrawParticleEmitterMap.erase( mapiter );
		}
	}
}

//
// RIBBON FUNCTIONS
//
PARTICLETYPETOPARTICLERIBBON* CParticleManager::GetParticleRibbonBatches( void )
{
	return &m_BatchDrawParticleRibbonMap;
}

void CParticleManager::AddParticleRibbon( CParticleRibbonRenderObject* particleObject )
{
	PARTICLETYPETOPARTICLERIBBON::iterator it;
	it = m_BatchDrawParticleRibbonMap.find( particleObject->GetParticleRibbonType() );
	if( it == m_BatchDrawParticleRibbonMap.end() )
	{
		m_BatchDrawParticleRibbonMap[particleObject->GetParticleRibbonType()][particleObject->GetName()->GetUniqueID()] = particleObject;
	}
	else
	{
		it->second[particleObject->GetName()->GetUniqueID()] =  particleObject;
	}
}

void CParticleManager::RemoveParticleRibbon( CParticleRibbonRenderObject* particleObject )
{
	PARTICLETYPETOPARTICLERIBBON::iterator mapit;
	mapit = m_BatchDrawParticleRibbonMap.find( particleObject->GetParticleRibbonType() );
	if( mapit != m_BatchDrawParticleRibbonMap.end() )
	{
		PARTICLERIBBONSMAP::iterator it;
		it = mapit->second.find( particleObject->GetName()->GetUniqueID() );
		if( it != mapit->second.end() )
		{
			mapit->second.erase( it );
		}

		if( mapit->second.size() == 0 )
		{
			m_BatchDrawParticleRibbonMap.erase( mapit );
		}
	}
}

ITextureObject* CParticleManager::LoadTexture( const TCHAR *str )
{
	if (!str || !str[0])
		return NULL;

	CHashString szName(str);
	TEXTUREOBJECTPARAMS tobj;
	tobj.bLoad = true;
	tobj.Name = &szName;
	tobj.TextureObjectInterface = NULL;
	static DWORD msgHash_AddTexture = CHashString(_T("AddTexture")).GetUniqueID();
	if( EngineGetToolBox()->SendMessage(msgHash_AddTexture, sizeof(tobj), &tobj ) == MSG_HANDLED)
	{
		return (ITextureObject*)tobj.TextureObjectInterface;
	}

	return NULL;
}
