///============================================================================
/// \file	CSpriteManager.cpp
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
#include "CSpriteManager.h"
#include "IMeshObject.h"
#include "IVertexBufferObject.h"

REGISTER_COMPONENT_SINGLETON( CSpriteManager );

REGISTER_MESSAGE_HANDLER( AddSprite, OnAddSprite, CSpriteManager);

/// \brief The Default Constructor for CSpriteManager
/// \return void
CSpriteManager::CSpriteManager() : 
	CManager(_T("CSpriteManager"), SPRITEPRIORITY )
{
}


/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CSpriteManager::Create(  int nArgs, va_list argptr )
{
	return SINGLETONINSTANCE( CSpriteManager );
}

/// \brief The Default Destructor for CSpriteManager
/// \return void
CSpriteManager::~CSpriteManager( )
{
	
}

/// \brief	Update function for the CSpriteManager
/// adds the callback for rendering to the list
/// \return	void
void CSpriteManager::Update(  DWORD tickCount  )
{	
	ADDRENDERLISTPARAMS		addRenderListMessage;
	OBJECTLIST					tempObjList;
	addRenderListMessage.dwListPriorityKey = RENDERPRIORITY_ALPHA-1;
	addRenderListMessage.dwSortMethod = 0;
	addRenderListMessage.objList = &tempObjList;	
	addRenderListMessage.dwFuncCallback = CSpriteManager::RenderCallBack;
	static DWORD msgHash_AddRenderList = CHashString(_T("AddRenderList")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddRenderList, sizeof(addRenderListMessage), &addRenderListMessage, NULL, NULL );
	//clear this before adding sprites
	m_bLastFrameRendered = true;
}

/// \brief	DeInitializes the CSpriteManager
/// \return	bool: Success
bool CSpriteManager::DeInit( void )
{	
	//loop through VBs delete them
	VBVERTTYPEMAPPING::iterator vter = m_VBs.begin();
	for(;vter != m_VBs.end(); vter++ )
	{
		VBFILLINFO &r = (*vter).second;
		r.offset = 0;
		delete r.vb;
		r.vb = NULL;
	}
	m_VBs.clear();
	m_bLastFrameRendered = false;
	return CManager::DeInit();
}

void CSpriteManager::RenderCallBack()
{
	CSpriteManager * spManager = SINGLETONINSTANCE( CSpriteManager );
	GETRENDERERINTERFACEPARAMS renderInterface;
	//go through draw entries and render
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	spManager->m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface, NULL, NULL);
	IRenderer * Renderer = renderInterface.m_RendererInterface;
	if( !Renderer )
	{
		return;
	}
	Matrix4x4 worldmat;
	worldmat.SetIdentity();
	Renderer->SetMatrix( WORLD_MATRIX, (const float*)worldmat.m );

	UINT size = (UINT)spManager->m_DrawEntries.size();
	for(UINT i = 0; i < size; i++ )
	{
		SPRITEDRAWENTRY *spriteEntry;
		spriteEntry = &spManager->m_DrawEntries[i];

		if (spManager->m_VBs.find( spriteEntry->vbindex )!= spManager->m_VBs.end() )
		{
			Renderer->SetEffect( 0, spriteEntry->effect );
			Renderer->SetTexture( 0, spriteEntry->texture );
			Renderer->SetMatrix( WORLD_MATRIX, (const float*)worldmat.m );
			VBFILLINFO &r= spManager->m_VBs[ spriteEntry->vbindex ];
			IVertexBufferObject * vb  = r.vb;
			Renderer->SetRenderState( RENDERSTATE_CULLMODE, RENDERSTATEPARAM_CULLNONE );
			Renderer->SetRenderState( RENDERSTATE_ALPHABLENDENABLE, RENDERSTATEPARAM_TRUE );
			Renderer->SetRenderState( RENDERSTATE_ALPHATESTENABLE, RENDERSTATEPARAM_FALSE );
			Renderer->SetRenderState( RENDERSTATE_ZENABLE, RENDERSTATEPARAM_ZTRUE );
			Renderer->SetRenderState( RENDERSTATE_ZWRITEENABLE, RENDERSTATEPARAM_ZFALSE );
			Renderer->SetPolygonZBias(0.0f, -1.0f / 30000.0f);
			Renderer->RenderVertexBuffer( vb, spriteEntry->offset, spriteEntry->size );
			Renderer->SetPolygonZBias(0.0f, 0.0f);
		}
	}
}
	
DWORD  CSpriteManager::OnAddSprite(DWORD size, void *params)
{
	if( m_bLastFrameRendered )
	{
		m_bLastFrameRendered = false;
		m_DrawEntries.clear();
		VBVERTTYPEMAPPING::iterator vter = m_VBs.begin();
		for(;vter != m_VBs.end(); vter++ )
		{
			VBFILLINFO &r = (*vter).second;
			r.offset = 0;
		}
	}
	ADDSPRITELISTMESSAGE * ap;
	CHANNELDESCRIPTORLIST	Descriptor;
	GETRENDERERINTERFACEPARAMS renderInterface;
	VERIFY_MESSAGE_SIZE( size, sizeof( ADDSPRITELISTMESSAGE) );
	ap = (ADDSPRITELISTMESSAGE*)params;
	if( ap->m_ChannelDescriptor == NULL ||
		ap->m_Data == NULL || 
		ap->m_NumChannels == NULL ||
		ap->m_numVertices == NULL )
	{
		return MSG_ERROR;
	}

	int totalstride = 0;
	for( int i = 0; i < ap->m_NumChannels; i++ )
	{
		Descriptor.push_back( ap->m_ChannelDescriptor[ i ] );
		Descriptor[ i ].NameHash = Descriptor[ i ].ChannelHash;
		totalstride += ap->m_ChannelDescriptor[ i ].Stride;
	}	
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface, NULL, NULL);
	IRenderer * Renderer = renderInterface.m_RendererInterface;
	if( !Renderer )
	{
		return MSG_ERROR;
	}
	if( m_VBs.find( totalstride ) == m_VBs.end() )
	{
        //vertex list not found, create new one
		IVertexBufferObject * vb = Renderer->CreateVertexBuffer();
		if( !vb->Initialize( Descriptor, MAX_SPRITE_ELEMENTS, true ) )
		{
			delete vb;
			return MSG_ERROR;
		}
		//insert VB into mapping
		VBFILLINFO vinfo;
		vinfo.size = MAX_SPRITE_ELEMENTS;
		vinfo.vb = vb;
		//for now this will be the FVF:/ very very bad, but we're not using this elswewhere yet
		m_VBs.insert( pair< DWORD, VBFILLINFO >( totalstride, vinfo ) );
	}
	VBVERTTYPEMAPPING::iterator vter = m_VBs.find( totalstride );
	if( vter == m_VBs.end() )
	{
		//something went wrong, 
		return MSG_ERROR;
	}
	//we have a valid vb
	VBFILLINFO &curvb = (*vter).second;
	
	//don't allow rendering over max sprites for now, since this is a queued system it's hard to just
	//render once the buffer is full. 
	if( curvb.offset + ap->m_numVertices >=  MAX_SPRITE_ELEMENTS )
	{
		return MSG_ERROR;
	}

	void * pData = 0;
	//try to lock
	if( !curvb.vb->Lock( curvb.offset, ap->m_numVertices, &pData, VBO_LOCK_NOOVERWRITE ) )
	{
		return MSG_ERROR;
	}
	//copy vertices
	memcpy( pData, ap->m_Data, totalstride* ap->m_numVertices );
	curvb.vb->Unlock();
	//add draw entry
	SPRITEDRAWENTRY newentry;
	newentry.vbindex = totalstride;//for now this will be the FVF:/ very very bad, but we're not using this elswewhere yet
	newentry.offset = curvb.offset;
	newentry.texture = ap->m_Texture;
	newentry.effect = ap->m_Effect;
	newentry.size = ap->m_numVertices/3;
	m_DrawEntries.push_back( newentry );
	//increment Vb offset
	curvb.offset += ap->m_numVertices;

	return MSG_HANDLED_STOP;	
}