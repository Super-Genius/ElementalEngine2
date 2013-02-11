///============================================================================
/// \file	CDecalManager.cpp
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
#include "CRenderObject.h"

REGISTER_COMPONENT_SINGLETON( CDecalManager );

REGISTER_MESSAGE_HANDLER( AddDecalSprite, OnAddDecalSprite, CDecalManager);
REGISTER_MESSAGE_HANDLER( AddDecalTemp, OnAddDecalTemp, CDecalManager);

#define INVALID_TIME -1

TOLUA_API int tolua_DecalScript_open (lua_State* tolua_S);

float CDecalManager::m_PrecentageUpdateIteration = .2f;

/// \brief The Default Constructor for CDecalManager
/// \return void
CDecalManager::CDecalManager() : 
	CManager(_T("CDecalManager"), DECALPRIORITY )
{
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
		tolua_DecalScript_open(m_LuaState);
	}
}


/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CDecalManager::Create(  int nArgs, va_list argptr )
{
	return SINGLETONINSTANCE( CDecalManager );
}

/// \brief The Default Destructor for CDecalManager
/// \return void
CDecalManager::~CDecalManager( )
{
	
}

/// \brief	Update function for the CDecalManager
/// adds the callback for rendering to the list
/// \return	void
void CDecalManager::Update(  DWORD tickCount  )
{	
	//Empty out the queue during the update
	ProcessDecalQueue();
}

void CDecalManager::ProcessDecalQueue()
{
	if( m_DecalQueue.size() > 0 )
	{
		CHANNELDESCRIPTORLIST Descriptor;
		//descriptor
		Descriptor.push_back( MESH_STREAM_VERTEX_POSITION3 );
		Descriptor.push_back( MESH_STREAM_VERTEX_NORMAL );
		Descriptor.push_back( MESH_STREAM_VERTEX_TEXTURECOORD1 );
		Descriptor.push_back( MESH_STREAM_VERTEX_COLORCRGBA );
		Descriptor.push_back( MESH_STREAM_VERTEX_TEXTURECOORD2 );
		
		ADDSPRITELISTMESSAGE msg;
		msg.m_ChannelDescriptor = &( Descriptor[ 0 ]);
		msg.m_Effect = m_DecalEffect;
		msg.m_NumChannels = 5;
		static DWORD msgHash_AddSprite = CHashString(_T("AddSprite")).GetUniqueID();

		for( size_t textures = 0; textures < m_DecalQueue.size(); textures++ )
		{
			TEXTUREVECTORPAIR &texvec = m_DecalQueue[ textures ];
			msg.m_Texture = texvec.first;
			int NumVerts = 6*(int)texvec.second.size();
			msg.m_Data = &( texvec.second[ 0 ]);
			msg.m_numVertices = NumVerts;
			EngineGetToolBox()->SendMessage( msgHash_AddSprite, sizeof(msg ), &msg );
			
		}
		float curTime = m_Timer->GetTime();
		/// Cleans up the queue of any old data keeping the memory
		/// contiguous for fast vertex buffer insertion
		/// and does only a few sections at a time since regardless if we have
		/// old sprites in here, if they are old, they won't actually be rendered by the
		/// vertex shader
		for( size_t textures = 0; textures < m_DecalQueue.size(); textures++ )
		{
			TEXTUREVECTORPAIR &texvec = m_DecalQueue[ textures ];
			size_t & iter = curIteration[ textures ];
			int numIterate = (int)( m_PrecentageUpdateIteration*(float)texvec.second.size() );	
			for( int i = 0; i < numIterate; i++ )
			{
				if( iter >= texvec.second.size() )
				{
					iter = 0;
				}
				//
				DECALQUEUEENTRY &refEntry = texvec.second[ iter ];
				if(	
					( //means this is already on the empty list
						refEntry.m_Verts[0].time[0] !=  INVALID_TIME
					)
					&& 
					curTime > ( refEntry.m_Verts[0].time[0] + refEntry.m_Verts[0].time[1] )  )
				{
					if( iter ==  texvec.second.size() - 1 )
					{
						//last element, just pop
						texvec.second.pop_back();
						//EngineGetToolBox()->Log( LOGINFORMATION, _T("Popping back\n"));
					}else//element in the middle, we're going to have to reuse this
					{
						//push this back on the stack
						m_EmptyIndices[ textures ].push( iter );
						refEntry.m_Verts[0].time[0] = INVALID_TIME;
						//EngineGetToolBox()->Log( LOGINFORMATION, _T("Deleted %d\n"), iter );
					}
				}
				iter++;
			}
		}

	}

}

/// \brief	DeInitializes the CDecalManager
/// \return	bool: Success
bool CDecalManager::DeInit( void )
{	
	return CManager::DeInit();
}

	
void CDecalManager::LoadDecalEffect()
{
	CHashString hszDecalEffect(_T("shaders\\DecalEffect.efx"));
	m_DecalEffect = CRenderObject<>::LoadEffect( &hszDecalEffect );	
	m_DecalTextureTest = CRenderObject<>::LoadTexture(_T("Textures\\waves2.tga"));
}

inline int GetBiasComponent( float vec[3] )
{
	if( vec[0] > vec[1] &&
		vec[0] > vec[2] )
	{
		return 0;
	}
	else if( vec[1] > vec[2] &&
		vec[1] > vec[0] )
	{
		return 1;
	}
	return 2;
}

inline int GetSwapComponent( int i )
{
	if( i == 2 )
	{
		return 0;
	}
	return i + 1;
}

DWORD CDecalManager::OnAddDecalSprite(DWORD size, void * params)
{
	if( m_DecalEffect == NULL )
	{
		LoadDecalEffect();
	}
	VERIFY_MESSAGE_SIZE( size, sizeof( ADDDECALSPRITEMESSAGE ) );
	ADDDECALSPRITEMESSAGE * parms = (ADDDECALSPRITEMESSAGE*)params;
	if( parms )
	{
		IBaseTextureObject *texture = parms->m_Texture;
		if (texture == NULL)
		{
			if (parms->m_hsTextureName == NULL)
			{
				m_ToolBox->Log(LOGWARNING, _T("%s add decal sprite: no texture or texture name specified\n"), __FILE__);
				return MSG_NOT_HANDLED;
			}
			else
			{
				// texture name supplied; try to get texture interface
				TEXTUREOBJECTPARAMS top;
				top.Name = parms->m_hsTextureName;
				top.TextureObjectInterface = NULL;
				top.bLoad = true;
				static DWORD msgAddTexture = CHashString(_T("AddTexture")).GetUniqueID();
				m_ToolBox->SendMessage(msgAddTexture, sizeof(TEXTUREOBJECTPARAMS), &top);
				
				if (top.TextureObjectInterface == NULL)
				{
					m_ToolBox->Log(LOGWARNING, _T("%s add decal sprite: texture failed to load: %s\n"), __FILE__, parms->m_hsTextureName->GetString());
					return MSG_NOT_HANDLED;
				}

				texture = top.TextureObjectInterface;
			}
		}

		float pos[3] = {0.0f};
		float norm[3] = {0.0f};

		if ((parms->m_Normal) && (parms->m_Position))
		{
			pos[0] = parms->m_Position->x;
			pos[1] = parms->m_Position->y;
			pos[2] = parms->m_Position->z;

			norm[0] = parms->m_Normal->x;
			norm[1] = parms->m_Normal->y;
			norm[2] = parms->m_Normal->z;
		}

		//static decal:
		DECALQUEUEENTRY entry;

		int Bias = GetBiasComponent( norm );
		int Tan1 = GetSwapComponent( Bias );
		int Tan2 = GetSwapComponent( Tan1 );
		float curTime = m_Timer->GetTime();
		float StartFadeAt = curTime + parms->m_TimeLinger;
		float StopFadeAt = parms->m_FadeTime;
		
		//swap x and y
		entry.m_Verts[0].pos[Tan1] = pos[Tan1] + norm[ Bias ]*parms->m_Size;
		entry.m_Verts[0].pos[Bias] = pos[Bias] + norm[ Tan1 ]*parms->m_Size;
		entry.m_Verts[0].pos[Tan2] = pos[Tan2] + norm[ Tan2 ]*parms->m_Size;
		entry.m_Verts[0].uv[0] = 0;
		entry.m_Verts[0].uv[1] = 0;
		entry.m_Verts[0].time[0] = StartFadeAt;
		entry.m_Verts[0].time[1] = StopFadeAt;
		//swap z and y
		entry.m_Verts[1].pos[Tan1] = pos[Tan1]  + norm[ Tan1 ]*parms->m_Size;
		entry.m_Verts[1].pos[Bias] = pos[Bias] + norm[ Tan2 ]*parms->m_Size;
		entry.m_Verts[1].pos[Tan2] = pos[Tan2]  + norm[Bias]*parms->m_Size;
		entry.m_Verts[1].uv[0] = 0;
		entry.m_Verts[1].uv[1] = 1.f;
		entry.m_Verts[1].time[0] = StartFadeAt;
		entry.m_Verts[1].time[1] = StopFadeAt;
		//swap -z with y
		entry.m_Verts[2].pos[Tan1] = pos[Tan1]  + norm[ Tan1 ]*parms->m_Size;
		entry.m_Verts[2].pos[Bias] = pos[Bias] + norm[ Tan2 ]*parms->m_Size;
		entry.m_Verts[2].pos[Tan2] = pos[Tan2]  - norm[ Bias ]*parms->m_Size;
		entry.m_Verts[2].uv[0] = 1.f;
		entry.m_Verts[2].uv[1] = 0;
		entry.m_Verts[2].time[0] = StartFadeAt;
		entry.m_Verts[2].time[1] = StopFadeAt;
		entry.m_Verts[3] = entry.m_Verts[2];
		entry.m_Verts[4] = entry.m_Verts[1];
		//swap -x with y
		entry.m_Verts[5].pos[Tan1] = pos[Tan1] - norm[ Bias ]*parms->m_Size;
		entry.m_Verts[5].pos[Bias] = pos[Bias] + norm[ Tan1 ]*parms->m_Size;
		entry.m_Verts[5].pos[Tan2] = pos[Tan2] + norm[ Tan2 ]*parms->m_Size;
		entry.m_Verts[5].uv[0] = 1.f;
		entry.m_Verts[5].uv[1] = 1.f;
		entry.m_Verts[5].time[0] = StartFadeAt;
		entry.m_Verts[5].time[1] = StopFadeAt;		

		//find texture:
		bool found = false;
		for( size_t i = 0; i < m_DecalQueue.size(); i++ )
		{
			//Found existing entry
			TEXTUREVECTORPAIR &texvec = m_DecalQueue[ i ];
			if( texvec.first == texture )
			{
				found = true;
				//push back or reuse an empty element:
				if( m_EmptyIndices[ i ].size() > 0 )
				{
					int index = m_EmptyIndices[ i ].top();
					m_EmptyIndices[ i ].pop();
					texvec.second[ index ] = entry;
					//EngineGetToolBox()->Log( LOGINFORMATION, _T("Using empty %d\n"), index );
				}
				else
				{					
					//EngineGetToolBox()->Log( LOGINFORMATION, _T("New Decal\n") );
					texvec.second.push_back( entry );
				}
				break;
			}
		}
		//no existing entry
		if( !found ) 
		{
			vector< DECALQUEUEENTRY > newvec;
			newvec.push_back( entry );
			DELETEDQUEUE newdeleteQueue;
			m_DecalQueue.push_back( TEXTUREVECTORPAIR( texture, newvec ) );
			m_EmptyIndices.push_back( newdeleteQueue );
			curIteration.push_back( 0 );		

		}		
	}
	return MSG_HANDLED_STOP;
}

DWORD CDecalManager::OnAddDecalTemp( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( Vec3 ) );
	Vec3 * p = (Vec3*)params;
	if( p )
	{
		ADDDECALSPRITEMESSAGE msg;
		msg.m_Position = p;
		Vec3 normVec(1.0f, 0.0f, 0.0f);
		msg.m_Normal = &normVec;
		msg.m_Size = 20;
		msg.m_TimeLinger = 1.f;
		msg.m_FadeTime = 1.f;
		OnAddDecalSprite( sizeof( msg ), (void*)&msg );
	}
	return MSG_HANDLED_STOP;
}