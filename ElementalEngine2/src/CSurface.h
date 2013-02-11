#ifndef CSURFACE_H
#define CSURFACE_H

#include "ISurface.h"
#include "IIndexBuffer.h"
#include "IVertexBufferObject.h"

typedef map< DWORD, IBaseTextureObject * > TEXTURESTAGEMAP;

class CSurface : public ISurface
{
	/// Sets the texture of the surface to a stage name. Stage names are things like "BaseTexture", "NormalMap",
	/// and that are used by the shader
	virtual void SetTexture( IHashString * stagename, IBaseTextureObject * texture )
	{
		if( stagename )
		{
			m_TextureStages[ stagename->GetUniqueID() ] = texture;
		}
	}
	/// Gets the texture of the surface from a stage name. Stage names are things like "BaseTexture", "NormalMap",
	/// and that are used by the shader. Returns NULL if surface does not have a listing of the stage name
	virtual IBaseTextureObject * GetTextureFromStageName( IHashString * stagename ) 
	{
		if( stagename )
		{
			TEXTURESTAGEMAP::iterator iter =m_TextureStages.find( stagename->GetUniqueID() );
			if( iter != m_TextureStages.end() )
			{
				return m_TextureStages[ stagename->GetUniqueID() ];
			}
		}
        return NULL;	
	}

	/// responsible for loading the mesh into internal buffers and then allocating vertex buffer space for the mesh
	virtual bool SetMesh( IMeshObject * mesh ) = 0;

	virtual void Serialize( IArchive &ar )
	{
		
	}
	/// --vertex buffer associations
	/// allows you to lock the vertices (delegation) of the vertex buffer directly
	virtual bool LockVertices( UINT offset, UINT size, void ** pData ) 
	{
		GETALLOCATIONVERTEXBUFFER vbinterface;
		if( size <= (UINT)m_VertexBufferAllocation.m_Size )
		{
			if( size == 0 )
			{
				size = m_IndexBufferAllocation.m_Size;
			}
			vbinterface.AllocationHandle = m_VertexBufferAllocation.m_AllocationHandle;
			//send a message to get the index buffer if it still exists
			//use the index buffer and lock it with the proper offsets
			DWORD res = m_ToolBox->SendMessage( _T("GetVertexAllocationInterface"),
									sizeof( vbinterface ), &vbinterface, NULL, NULL );
			if( res == MSG_HANDLED )
			{
				if( vbinterface.m_VertexBufferObject != NULL )
				{
					vbinterface.m_VertexBufferObject->Lock( m_VertexBufferAllocation.m_Offset + offset,
															size, pData );				
				}			
			}
		}
		return false;
	}
	/// get number of vertices
	virtual UINT GetNumVertices()
	{
		return m_SizeVerts;
	}
	/// allows you to resize the buffer
	virtual bool ResizeVertices( UINT size, UINT stridebytes )
	{
		//resize the vert allocation
		if( m_VertexBufferAllocation.m_Size != 0 )
		{
			//previously allocated, remove
			DWORD res = m_ToolBox->SendMessage( _T("RemoveVertexAllocation"),
								sizeof( m_VertexBufferAllocation ), &m_VertexBufferAllocation, NULL, NULL );
		}
		//create new allocation
		ALLOCATERENDERBUFFERMSG msg;
		msg.ElementsToAllocate = size;
		msg.SizeVertexFormat = stridebytes;

		DWORD res = m_ToolBox->SendMessage( _T("AllocateVertices"), sizeof( msg ), &msg, NULL, NULL );
		if( res == MSG_HANDLED )
		{
			m_VertexBufferAllocation.m_Offset = msg.m_ReturnOffset;
			m_VertexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
			m_VertexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
			m_VertexBufferAllocation.m_Size = size;
			m_VertexBufferAllocation.m_Stride = stridebytes;
		}				
	}
	/// Index buffer associations
	virtual bool ResizeIndices( UINT size, UINT stridebytes )
	{
		//resize index allocation
		//resize the vert allocation
		if( m_VertexBufferAllocation.m_Size != 0 )
		{
			//previously allocated, remove
			DWORD res = m_ToolBox->SendMessage( _T("RemoveVertexAllocation"),
								sizeof( m_VertexBufferAllocation ), &m_VertexBufferAllocation, NULL, NULL );
		}
		//create new allocation
		ALLOCATERENDERBUFFERMSG msg;
		msg.ElementsToAllocate = size;
		msg.SizeVertexFormat = stridebytes;

		DWORD res = m_ToolBox->SendMessage( _T("AllocateVertices"), sizeof( msg ), &msg, NULL, NULL );
		if( res == MSG_HANDLED )
		{
			m_VertexBufferAllocation.m_Offset = msg.m_ReturnOffset;
			m_VertexBufferAllocation.m_AllocationHandle = msg.m_ReturnHandle;
			m_VertexBufferAllocation.m_InterfaceHandle = msg.m_ReturnInterfaceHandle;
			m_VertexBufferAllocation.m_Size = size;
			m_VertexBufferAllocation.m_Stride = stridebytes;
		}	
	}
	/// locks the index buffer
	virtual bool LockIndices( UINT offset, UINT size, void ** pData ) 
	{
		GETALLOCATIONINDEXBUFFER ibinterface;
		if( size <= (UINT)m_IndexBufferAllocation.m_Size )
		{
			if( size == 0 )
			{
				size = m_IndexBufferAllocation.m_Size;
			}
			ibinterface.AllocationHandle = m_VertexBufferAllocation.m_AllocationHandle;
			//send a message to get the index buffer if it still exists
			//use the index buffer and lock it with the proper offsets
			DWORD res = m_ToolBox->SendMessage( _T("GetIndexAllocationInterface"),
									sizeof( ibinterface ), &ibinterface, NULL, NULL );
			if( res == MSG_HANDLED )
			{
				if( ibinterface.m_IndexBufferInterface != NULL )
				{
					ibinterface.m_IndexBufferInterface->Lock( m_IndexBufferAllocation.m_Offset + offset,
																size, pData );				
				}			
			}
		}
		return false;		
	}
	/// Index buffer associations
	virtual UINT GetNumIndices()
	{
		return m_SizeIndices;
	}
	/// number of primitives
	virtual UINT GetNumPrimitives()
	{
		return m_NumPrimitives;
	}

	//Renders this surface with a shader(1 pass )
	virtual bool Render( IShader * )
	{
		if( m_Renderer == NULL )
		{		
			// grab the render interface
			GETRENDERERINTERFACEPARAMS renderInterface;

			m_ToolBox->SendMessage(_T("GetRendererInterface"), 
								sizeof(GETRENDERERINTERFACEPARAMS), 
								&renderInterface, 
								NULL, 
								NULL);
			m_Renderer = renderInterface.m_RendererInterface;
		}
		if( m_Renderer )
		{
			//m_Renderer->RenderIndexBuffer( ib, vb, m_IndexBufferAllocation.m_Offset,
			//								m_NumPrimitives, shader, texture, numtextures );
		}
		return true;
	};

	virtual IController * GetController() = 0;

	CSurface()
	{
		//m_IndexBuffer = NULL;
		//m_VertexBuffer = NULL;
		m_NumPrimitives = 0;
		//m_IndexBufferOffset = 0;
		//m_VertexBufferOffset = 0;
		m_SizeVerts = 0;
		m_SizeIndices = 0;
		m_ToolBox = EngineGetToolBox();
		//so we don't always have to check whether its available or not
		//it should always exist
		assert( m_ToolBox != NULL );
		m_Renderer = NULL;
	}
private:

	
	UINT	m_NumPrimitives;

	UINT	m_SizeVerts;
	UINT	m_SizeIndices;

	BufferAllocation m_IndexBufferAllocation;
	BufferAllocation m_VertexBufferAllocation;

	IToolBox * m_ToolBox;

	TEXTURESTAGEMAP m_TextureStages;
	IRenderer * m_Renderer;

};

#endif