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
#include "IMeshObject.h"
#include "CDX9VertexBufferObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDX9VertexBufferObject::CDX9VertexBufferObject()
{
	m_NumVertices = 0;
	m_VBInterface = NULL;
	m_VertexDeclaration= NULL;
	m_Stride = 0;
	m_FVF = 0;

	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;

	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, 
						sizeof(GETRENDERERINTERFACEPARAMS), 
						&renderInterface, 
						NULL, 
						NULL);
	m_Renderer = renderInterface.m_RendererInterface;

}

CDX9VertexBufferObject::~CDX9VertexBufferObject()
{
	DeInit();
}

void CDX9VertexBufferObject::DeInit()
{
	//check if vertex buffer was created before
	m_NumVertices = 0;	
	if (m_VBInterface)
	{
		m_VBInterface->Release();
		m_VBInterface = NULL;
	}
	if( m_VertexDeclaration )
	{
		m_VertexDeclaration->Release();
		m_VertexDeclaration = NULL;
	}
	m_FVF = 0;
	m_VertexDescription.clear();
	
}
/// \brief Initializees a Vertex buffer
/// \param channels = the vertex description
/// \param numElements = number of vertices to allocate
/// \param bdynamic = dynamic vertex buffer or not? 
/// \return bool = true if successful, false if creation failed. 
bool CDX9VertexBufferObject::Initialize( CHANNELDESCRIPTORLIST &channels, UINT numElements, bool bdynamic, UINT streammode )
{
	HRESULT hr;
	LPDIRECT3DDEVICE9 pDevice;
	int totalstride;
	if( !m_Renderer )
	{
		return false;
	}
	pDevice = (LPDIRECT3DDEVICE9)m_Renderer->GetAPIDevice();
	if( !pDevice )
	{
		return false;
	}
	//deinit if it was previously initialized
	DeInit();

	//Calculate requested stride:
	totalstride =0;
	//calculate strides
	for( int i = 0; i < (int)channels.size(); i++ )
	{
		ChannelDesc * curStream = &channels[ i ];
		m_VertexDescription.push_back( *curStream );//add to internal format descriptor
		totalstride += curStream->Stride;
	}

	DWORD flags;
	if( bdynamic )
	{
		flags = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;
	}else
	{
		flags = D3DUSAGE_WRITEONLY;
	}
	if( FAILED( hr = pDevice->CreateVertexBuffer( numElements*totalstride,
														flags, 0,
														D3DPOOL_DEFAULT, &m_VBInterface, NULL) ) )
	{
		m_VBInterface = NULL;
		m_NumVertices = 0;
		m_VertexDescription.clear();
		m_Stride = 0;
		return false;
	}
	//Set the FVF and vertex declaration formats
	SetVertexDescription( channels, pDevice, streammode );
	m_Stride = totalstride;
	m_NumVertices = numElements;
	m_bDynamic = bdynamic;
	return true;
}

void CDX9VertexBufferObject::GetDescription( VertexBufferDescriptor &Desc )
{
	if( Desc.outChannelDesc )
	{
		//fill with channel desc
		int size = m_VertexDescription.size();
		for( int i = 0; i < size;i++ )
		{
			Desc.outChannelDesc->push_back( m_VertexDescription[ i ] );
		}
	}
	Desc.outnumElements = m_NumVertices;
}

bool CDX9VertexBufferObject::Lock( UINT offset, UINT size, void ** pData, DWORD flags )
{
	if( m_VBInterface )
	{
		//TODO: Flags
		DWORD d3dflags = 0;
		if( m_bDynamic && flags & VBO_LOCK_DISCARD)
		{
			d3dflags |= D3DLOCK_DISCARD;
		}
		if( flags & VBO_LOCK_NOOVERWRITE )
		{
			d3dflags |= D3DLOCK_NOOVERWRITE;
		}
			

#ifdef DISABLE_RANGED_BUFFER_LOCKS
		// range locking not supported
		HRESULT hr = m_VBInterface->Lock( 0, 0, pData,  d3dflags );
		if (hr == D3D_OK)
			*pData = (BYTE*)*pData + offset*m_Stride;
#else
		HRESULT hr = m_VBInterface->Lock( offset*m_Stride, size*m_Stride, pData, d3dflags);
#endif
		return (hr == D3D_OK);
	}
	return false;
}

bool CDX9VertexBufferObject::Unlock()
{
	if( m_VBInterface )
	{
		HRESULT hr = m_VBInterface->Unlock();
		return (hr == D3D_OK);
	}
	return false;
}


void CDX9VertexBufferObject::SetVertexDescription( CHANNELDESCRIPTORLIST &channels, LPDIRECT3DDEVICE9 pDevice, UINT streamode )
{
	
	m_FVF = 0;
	if( m_VertexDeclaration )
	{
		m_VertexDeclaration->Release();
		m_VertexDeclaration = NULL;
	}
	CDX9Renderer * DX9Renderer = (CDX9Renderer*)m_Renderer;
	m_VertexDeclaration = DX9Renderer->GetVertexDescription( channels, streamode );
};
