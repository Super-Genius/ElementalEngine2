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
#include "CDX9IndexBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDX9IndexBuffer::~CDX9IndexBuffer()
{
	DeInit();
}

CDX9IndexBuffer::CDX9IndexBuffer()
{
	m_Size = 0;
	m_IBInterface = NULL;
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

bool CDX9IndexBuffer::Resize( UINT numElements ){

	HRESULT hr;
	LPDIRECT3DDEVICE9 pDevice;
	if( !m_Renderer )
	{
		return false;
	}
	pDevice = (LPDIRECT3DDEVICE9)m_Renderer->GetAPIDevice();
	if( !pDevice )
	{
		return false;
	}
	DeInit();

	m_Size = numElements;
	DWORD flags = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;
	if( FAILED( hr = pDevice->CreateIndexBuffer( numElements*2, //TODO: make option for 32 or 16 bit indices
														flags, D3DFMT_INDEX16,
														D3DPOOL_DEFAULT, &m_IBInterface, NULL) ) )
	{
		return false;
	}
	return true;
};

UINT CDX9IndexBuffer::GetSize()
{
	return m_Size;
}

void CDX9IndexBuffer::DeInit()
{
	if( m_IBInterface )
	{
		m_IBInterface->Release();
		m_IBInterface = NULL;
		m_Size = 0;
	}
}

bool CDX9IndexBuffer::Lock( UINT offset, UINT size, void ** pData )
{
	if( m_IBInterface )
	{
		//TODO: Flags
		//TODO: make 32 and 16 bit formats available
		int stride = sizeof(short);
#ifdef DISABLE_RANGED_BUFFER_LOCKS
		// range locking not supported
		HRESULT hr =  m_IBInterface->Lock( 0, 0, pData,  D3DLOCK_DISCARD );
		if (hr == D3D_OK)
			*pData = (BYTE*)*pData + offset*stride;
#else
		HRESULT hr =  m_IBInterface->Lock( offset*stride, size*stride, pData,  D3DLOCK_DISCARD );
#endif
		return (hr == D3D_OK );
	}
	return false;
}

bool CDX9IndexBuffer::Unlock()
{
	if( m_IBInterface )
	{
		HRESULT hr = m_IBInterface->Unlock( );
		return (hr == D3D_OK );
	}
	return false;
}