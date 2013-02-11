///============================================================================
/// \file	CGeometry.cpp
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

void CGeometry::SetMaterial( IMaterial * mat )
{
	m_Material = mat;
}


IMaterial * CGeometry::GetMaterial() 
{
	return m_Material;
}

// --vertex buffer associations
// allows you to lock the vertices (delegation) of the vertex buffer directly
bool CGeometry::LockVertices( UINT offset, UINT size, void ** pData ) 
{
	GETALLOCATIONVERTEXBUFFER vbinterface;
	if( size <= (UINT)m_VertexBufferAllocation.m_Size )
	{
		if( size == 0 )
		{
			size = m_VertexBufferAllocation.m_Size;
		}
		vbinterface.AllocationHandle = m_VertexBufferAllocation.m_AllocationHandle;
		//send a message to get the index buffer if it still exists
		//use the index buffer and lock it with the proper offsets
		static DWORD msgHash_GetVertexAllocationInterface = CHashString(_T("GetVertexAllocationInterface")).GetUniqueID();
		DWORD res = m_ToolBox->SendMessage(msgHash_GetVertexAllocationInterface,sizeof( vbinterface ), &vbinterface);
		if( res == MSG_HANDLED )
		{
			if( vbinterface.m_VertexBufferObject != NULL )
			{
				return vbinterface.m_VertexBufferObject->Lock( m_VertexBufferAllocation.m_Offset + offset,
														size, pData );		
			}			
		}
	}
	return false;
}

// allows you to resize the buffer
bool CGeometry::ResizeVertices( UINT size, UINT stridebytes, CHANNELDESCRIPTORLIST * VertexFormat )
{
	//resize the vert allocation
	if( m_VertexBufferAllocation.m_Size != 0 )
	{
		//previously allocated, remove
		static DWORD msgHash_RemoveVertexAllocation = CHashString(_T("RemoveVertexAllocation")).GetUniqueID();
		DWORD res = m_ToolBox->SendMessage(msgHash_RemoveVertexAllocation,sizeof( m_VertexBufferAllocation ), &m_VertexBufferAllocation);
	}
	//create new allocation
	ALLOCATERENDERBUFFERMSG msg;
	msg.ElementsToAllocate = size;
	msg.SizeVertexFormat = stridebytes;
		
	//just for cal 3d
	if( VertexFormat )
	{
		msg.VertexFormat = *VertexFormat;
	}			

	static DWORD msgHash_AllocateVertices = CHashString(_T("AllocateVertices")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_AllocateVertices, sizeof( msg ), &msg, NULL, NULL );
	if( res == MSG_HANDLED )
	{
		m_VertexBufferAllocation.m_Offset = msg.m_ReturnOffset;
		m_VertexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
		m_VertexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
		m_VertexBufferAllocation.m_Size = size;
		m_VertexBufferAllocation.m_Stride = stridebytes;
		m_VertexBufferInterface = (IVertexBufferObject*)msg.m_InterfacePointer;
		return true;
	}				
	return false;
}

// Index buffer associations
bool CGeometry::ResizeIndices( UINT size, UINT stridebytes )
{
	//resize index allocation
	if( m_IndexBufferAllocation.m_Size != 0 )
	{
		//previously allocated, remove
		static DWORD msgHash_RemoveIndexAllocation = CHashString(_T("RemoveIndexAllocation")).GetUniqueID();
		DWORD res = m_ToolBox->SendMessage(msgHash_RemoveIndexAllocation, sizeof( m_IndexBufferAllocation ), &m_IndexBufferAllocation );
	}
	//create new allocation
	ALLOCATERENDERBUFFERMSG msg;
	msg.ElementsToAllocate = size;

	static DWORD msgHash_AllocateIndices = CHashString(_T("AllocateIndices")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_AllocateIndices, sizeof( msg ), &msg, NULL, NULL );
	if( res == MSG_HANDLED )
	{
		m_IndexBufferAllocation.m_Offset = msg.m_ReturnOffset;
		m_IndexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
		m_IndexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
		m_IndexBufferAllocation.m_Size = size;
		m_IndexBufferAllocation.m_Stride = stridebytes;
		m_IndexBufferInterface = (IIndexBuffer*)msg.m_InterfacePointer;
		return true;
	}	
	return false;
}

// locks the index buffer
bool CGeometry::LockIndices( UINT offset, UINT size, void ** pData ) 
{
	GETALLOCATIONINDEXBUFFER ibinterface;
	if( size <= (UINT)m_IndexBufferAllocation.m_Size )
	{
		if( size == 0 )
		{
			size = m_IndexBufferAllocation.m_Size;
		}
		ibinterface.AllocationHandle = m_IndexBufferAllocation.m_AllocationHandle;
		//send a message to get the index buffer if it still exists
		//use the index buffer and lock it with the proper offsets
		static DWORD msgHash_GetIndexAllocationInterface = CHashString(_T("GetIndexAllocationInterface")).GetUniqueID();
		DWORD res = m_ToolBox->SendMessage(msgHash_GetIndexAllocationInterface, sizeof( ibinterface ), &ibinterface);
		if ((res == MSG_HANDLED) && (ibinterface.m_IndexBufferInterface != NULL))
		{
			return ibinterface.m_IndexBufferInterface->Lock( m_IndexBufferAllocation.m_Offset + offset,
															size, pData );		
		}
	}
	return false;		
}

bool CGeometry::UnlockVertices( ) 
{
	GETALLOCATIONVERTEXBUFFER vbinterface;
	{
		
		vbinterface.AllocationHandle = m_VertexBufferAllocation.m_AllocationHandle;
		//send a message to get the index buffer if it still exists
		//use the index buffer and lock it with the proper offsets
		static DWORD msgHash_GetVertexAllocationInterface = CHashString(_T("GetVertexAllocationInterface")).GetUniqueID();
		DWORD res = m_ToolBox->SendMessage(msgHash_GetVertexAllocationInterface, sizeof( vbinterface ), &vbinterface, NULL, NULL );
		if( res == MSG_HANDLED )
		{
			if( vbinterface.m_VertexBufferObject != NULL )
			{
				return vbinterface.m_VertexBufferObject->Unlock();
			}			
		}
	}		
	return false;
}

bool CGeometry::UnlockIndices( ) 
{
	GETALLOCATIONINDEXBUFFER ibinterface;
	
	ibinterface.AllocationHandle = m_IndexBufferAllocation.m_AllocationHandle;
	//send a message to get the index buffer if it still exists
	//use the index buffer and lock it with the proper offsets
	static DWORD msgHash_GetIndexAllocationInterface = CHashString(_T("GetIndexAllocationInterface")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_GetIndexAllocationInterface, sizeof( ibinterface ), &ibinterface);
	if (( res == MSG_HANDLED ) && (ibinterface.m_IndexBufferInterface != NULL ))
	{
		return ibinterface.m_IndexBufferInterface->Unlock();	
	}			
	return false;		
}

bool CGeometry::Render( ISceneSettings * settings, IEffect * override )
{
	static CLightManager *pLightManager = SINGLETONINSTANCE( CLightManager );

	if( m_Renderer == NULL )
	{		
		// grab the render interface
		GETRENDERERINTERFACEPARAMS renderInterface;

		static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface);
		m_Renderer = renderInterface.m_RendererInterface;
	}
	if( m_Renderer && 
		m_Material)
	{		
		m_NumPrimitives = m_IndexBufferAllocation.m_Size/3;

		UINT pShadowCastingTexPass[MAXSHADOWMAPS] = { 0, 0 };
		UINT pShadowCastingTexStage[MAXSHADOWMAPS] = { 0, 0 };
		ILightObject *pShadowCastingLights[MAXSHADOWMAPS] = { NULL, NULL };
		Light_Info *pShadowCastingLightInfos[MAXSHADOWMAPS] = { NULL, NULL };
		int iShadowCastingLightCount = 0;

		// store shader flags
		SHADER_RENDER_FLAG settingsShaderFlags;
		if (settings)
			settingsShaderFlags = settings->GetShaderFlags();

		// update settings
		if( settings )
		{
			settings->SetDetailLightCount( m_NumDetailLights );
			settings->SetActiveLightCount( m_NumLights );

			// check that there is a valid shadow map attached to at least one of the lights
			if( settings->GetShaderFlags() & (SHADOW_RECEIVE_OMNI|SHADOW_RECEIVE_HEMI) )
			{
				for (int i=0; i < MAX_COMPLEX_LIGHTS && i < m_NumLights && iShadowCastingLightCount < MAXSHADOWMAPS; i++)
				{
					static CHashString ShadowHash(_T("ShadowMap"));
					static CHashString ShadowHash2(_T("ShadowMapSecondary"));
					CHashString *pShadowHash = &ShadowHash;
					if (iShadowCastingLightCount > 0)
						pShadowHash = &ShadowHash2;

					if (m_Lights[i] && m_Lights[i]->CanCastShadows())
					{
						Light_Info * lInfo = pLightManager->GetLightShadowInfo( m_Lights[i] );
						if( lInfo && lInfo->ShadowMapIndex != INVALID_SHADOW_MAP_INDEX )
						{
							UINT lPass=0, lStage=0;
							bool bHasTextureChannel = false;
							if( override && override->GetTextureChannels( pShadowHash, lPass, lStage ) )
								bHasTextureChannel = true;
							else if( m_Material && m_Material->GetTextureChannels( pShadowHash, lPass, lStage ) )
								bHasTextureChannel = true;

							if (bHasTextureChannel)
							{
								pShadowCastingTexPass[iShadowCastingLightCount] = lPass;
								pShadowCastingTexStage[iShadowCastingLightCount] = lStage;
								pShadowCastingLights[iShadowCastingLightCount] = m_Lights[i];
								pShadowCastingLightInfos[iShadowCastingLightCount] = lInfo;
								iShadowCastingLightCount++;
							}
						}
					}
				}
				if (iShadowCastingLightCount == 0)
				{
					//clear receiver flags
					settings->RemoveShaderFlag( SHADOW_RECEIVE_HEMI | SHADOW_RECEIVE_OMNI |
											SHADOW_RECEIVE_DIR );
				}
			}
			settings->SetShadowMapCount( iShadowCastingLightCount );
		}

		// apply material
		if( override )
		{
			m_Renderer->SetEffect( 0, override );
		}
		else
		{
			m_Material->ApplyMaterial( 0, settings );
		}

		if ((m_MatOverride) && (m_Material))
		{
			IEffect *effect = m_Material->GetEffect();
			if (effect)
			{
				m_MatOverride->Apply(m_Renderer, effect);
			}
		}

		// apply shadow map data
		for (int i=0; i < iShadowCastingLightCount; i++)
		{
			ILightObject *pShadowCastingLight = pShadowCastingLights[i];
			Light_Info *lInfo = pShadowCastingLightInfos[i];

			//Grab the shadow texture
			float p[4] = {0, 0, 0, 0};
			p[0] = lInfo->lastLightPosition.x;
			p[1] = lInfo->lastLightPosition.y;
			p[2] = lInfo->lastLightPosition.z;
			p[3] = pShadowCastingLight->GetAttenuationDistance();

			UINT lPass = pShadowCastingTexPass[i];
			UINT lStage = pShadowCastingTexStage[i];
			m_Renderer->SetTexture( lStage, lInfo->LightPerspectiveMap );
			m_Renderer->SetSamplerState( lStage, SAMPLERSTATE_MIPFILTER, TEXTURESTAGE_TEXF_POINT );

			m_Material->SetVertexConstant( C_SHADOWCOORD0+i, p, 1 );
		}

		//Setup lights:
		m_Renderer->SetLightArray( m_Lights, m_NumLights, m_DetailLights, m_NumDetailLights,  m_AmbientLightTerm );	

		//Set up the lightmap texture
		if(	m_Lightmap )
		{
			UINT lPass = 0;
			UINT lStage = 0;
			static CHashString LightmapHash(_T("LightMap"));
			if( override &&
				override->GetTextureChannels( &LightmapHash, lPass, lStage ) )
			{
				m_Renderer->SetTexture( lStage, m_Lightmap );
			}
			else 
				if( m_Material &&
				m_Material->GetTextureChannels( &LightmapHash, lPass, lStage ) )
			{
				m_Renderer->SetTexture( lStage, m_Lightmap );
			}
		}

		//Do the rendering
		m_Renderer->RenderIndexBuffer( m_IndexBufferInterface, m_VertexBufferInterface, 
			m_VertexBufferAllocation.m_Offset, m_VertexBufferAllocation.m_Size, m_IndexBufferAllocation.m_Offset,
										m_NumPrimitives, m_PrimType);

		// restore settings shader flags
		if (settings)
			settings->SetShaderFlags( settingsShaderFlags );
	}
	return true;
}

// Renders this surface with a shader(1 pass )
bool CGeometry::Render( IEffect * override )
{
	return Render( m_SceneSettings, override );
};

bool CGeometry::DrawGeometry()
{
	if( m_Renderer == NULL )
	{		
	}

	if( m_Renderer )
	{
		m_NumPrimitives = m_IndexBufferAllocation.m_Size/3;
		m_Renderer->RenderIndexBuffer( m_IndexBufferInterface, m_VertexBufferInterface, m_IndexBufferAllocation.m_Offset,
										m_NumPrimitives, m_PrimType );
	}
	return true;

}

CGeometry::CGeometry()
{
	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;

	m_IndexBufferInterface = NULL;
	m_VertexBufferInterface = NULL;
	m_NumPrimitives = 0;
	m_Material = NULL;

	// default to at least one texture per pass
	m_MaxTextures = 1;
	//m_IndexBufferOffset = 0;
	//m_VertexBufferOffset = 0;
	m_SizeVerts = 0;
	m_SizeIndices = 0;
	m_ToolBox = EngineGetToolBox();
	//so we don't always have to check whether its available or not
	//it should always exist
	assert( m_ToolBox != NULL );
	m_Renderer = NULL;

	renderInterface.m_RendererInterface = NULL;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface);
	m_Renderer = renderInterface.m_RendererInterface;
	if (m_Renderer != NULL)
	{
		IConfig *maxStagesConfig;
		CHashString nameMaxStagesName(_T("MAX_TEXTURE_STAGES"));
		maxStagesConfig = m_Renderer->GetConfiguration(&nameMaxStagesName);
		if (maxStagesConfig == NULL)
		{
			assert(0);
		}
		else
		{
			m_MaxTextures = (UINT)(*maxStagesConfig);
		}
	}

	m_Controller = NULL;
	m_SceneSettings = NULL;
	m_PrimType = DP_TRIANGLELIST;
    DetachAllLights();
	m_LightmapVB = NULL;
	m_Lightmap = NULL;
	m_MatOverride = NULL;

	static DWORD msgCreateMaterialOverride = CHashString(_T("CreateMaterialOverride")).GetUniqueID();
	m_ToolBox->SendMessage(msgCreateMaterialOverride, sizeof(IMaterialOverride**), &m_MatOverride);
}

CGeometry::~CGeometry()
{
	if (m_MatOverride)
	{
		// created with a factory... should be safe to delete here, right?
		SAFE_DELETE(m_MatOverride);
	}
}

// for pre allocated buffers, for example, we want to share a vertex pool
bool CGeometry::SetVertexBufferAllocation( BUFFERALLOCATIONSTRUCT &alloc )
{
	m_VertexBufferAllocation = alloc;
	GETALLOCATIONVERTEXBUFFER vbinterface;
	
	vbinterface.AllocationHandle = m_VertexBufferAllocation.m_AllocationHandle;
	//send a message to get the index buffer if it still exists
	//use the index buffer and lock it with the proper offsets
	static DWORD msgHash_GetVertexAllocationInterface = CHashString(_T("GetVertexAllocationInterface")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_GetVertexAllocationInterface,sizeof( vbinterface ), &vbinterface, NULL, NULL );
	if( res == MSG_HANDLED )
	{
		if( vbinterface.m_VertexBufferObject != NULL )
		{
			m_VertexBufferInterface = vbinterface.m_VertexBufferObject;
			return true;
		}			
	}
	//invalid interface
	return false;		

}

// for pre allocated buffers, for example, we want to share a vertex pool
bool CGeometry::SetIndexBufferAllocation( BUFFERALLOCATIONSTRUCT &alloc )
{
	m_IndexBufferAllocation = alloc;
	GETALLOCATIONINDEXBUFFER ibinterface;
	
	ibinterface.AllocationHandle = m_IndexBufferAllocation.m_AllocationHandle;
	//send a message to get the index buffer if it still exists
	//use the index buffer and lock it with the proper offsets
	static DWORD msgHash_GetIndexAllocationInterface = CHashString(_T("GetIndexAllocationInterface")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_GetIndexAllocationInterface,sizeof( ibinterface ), &ibinterface);
	if ((res == MSG_HANDLED) && (ibinterface.m_IndexBufferInterface != NULL))
	{
		m_IndexBufferInterface = ibinterface.m_IndexBufferInterface;
		return true;
	}
	//invalid interface
	return false;		
}

void CGeometry::SetLightmapTexture( IBaseTextureObject* texture )
{
	m_Lightmap = texture;
}

void CGeometry::SetLightmapUVStream( float * uvstream, int numelements  )
{
	if( numelements == (m_VertexBufferAllocation.m_Size*2) )
	{
		//create the new VB allocation
		ALLOCATERENDERBUFFERMSG msg;
		msg.ElementsToAllocate = m_VertexBufferAllocation.m_Size;
		msg.SizeVertexFormat = sizeof(float)*2;
		msg.m_StreamMode = 1;//stream 1
		CHANNELDESCRIPTORLIST uvVert;
		uvVert.push_back( MESH_STREAM_VERTEX_TEXTURECOORD2 ); //second set of UVs
		//just for cal 3d
		msg.VertexFormat = uvVert;

		static DWORD msgHash_AllocateVertices = CHashString(_T("AllocateVertices")).GetUniqueID();
		DWORD res = m_ToolBox->SendMessage(msgHash_AllocateVertices, sizeof( msg ), &msg, NULL, NULL );
		if( res == MSG_HANDLED )
		{
			m_LightmapUVs.m_Offset = msg.m_ReturnOffset;
			m_LightmapUVs.m_AllocationHandle = msg.m_ReturnHandle;
			m_LightmapUVs.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
			m_LightmapUVs.m_Size = m_VertexBufferAllocation.m_Size;
			m_LightmapUVs.m_Stride = msg.SizeVertexFormat;
			m_LightmapVB = (IVertexBufferObject*)msg.m_InterfacePointer;
			if( m_LightmapVB )
			{
				BYTE * pData = NULL;
				if( m_LightmapVB->Lock( m_LightmapUVs.m_Offset, m_LightmapUVs.m_Size, (void**)&pData ) )
				{
					memcpy( pData, uvstream, m_LightmapUVs.m_Size*m_LightmapUVs.m_Stride );
					m_LightmapVB->Unlock();
				}else
				{
					m_ToolBox->Log( LOGERROR, _T("Failed to Lock UV Stream for writing lightmap uvs\n" ) );
				}
			}
		}				
		//copy the stream

	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Could not set up UV stream for lightmap, number of vertices does not match\n" ) );
	}
}

inline bool CGeometry::HasAlpha()
{
	if (m_Material)
	{
		if (m_Material->GetShaderFlags() & ALPHA)
		{
			return true;
		}

		IMaterialTemplate *pTemplate = m_Material->GetMaterialTemplate();
		if (pTemplate && pTemplate->HasAlpha())
			return true;
	}

	if (m_MatOverride)
	{
		if (m_MatOverride->HasAlpha())
		{
			return true;
		}
	}

	return false;
}