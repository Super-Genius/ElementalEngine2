///============================================================================
/// \file	CVertexBufferManager.cpp
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
#include "CVertexBufferManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_VB_SIZE 65536

REGISTER_COMPONENT_SINGLETON( CVertexBufferManager );
REGISTER_MESSAGE_HANDLER( AllocateVertices, OnAllocateVertices, CVertexBufferManager);
REGISTER_MESSAGE_HANDLER( GetVertexAllocationInterface, OnGetAllocationBuffer, CVertexBufferManager );
REGISTER_MESSAGE_HANDLER( RemoveVertexAllocation, OnRemoveAllocation, CVertexBufferManager );
REGISTER_MESSAGE_HANDLER_PRIORITY( DestroyRenderer, OnDestroyRenderer, CVertexBufferManager, 0x70000000 );


/// \brief The Default Constructor for CVertexBufferManager
/// \return void
CVertexBufferManager::CVertexBufferManager() : 
	CManager(_T("CVertexBufferManager"))
{
	m_DefaultVBSize = DEFAULT_VB_SIZE;
	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;

	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &renderInterface);

	m_Renderer = renderInterface.m_RendererInterface;
	m_LastCreatedHandle = 0;
	m_LastCreatedInterfaceHandle = -1;
	m_InterfaceHandleCount = 0;
}


/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CVertexBufferManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CVertexBufferManager );
}

/// \brief The Default Destructor for CVertexBufferManager
/// \return void
CVertexBufferManager::~CVertexBufferManager( )
{
	DeInit();
}

/// \brief	DeInitializes the CVertexBufferManager
/// \return	bool: Success
bool CVertexBufferManager::DeInit( void )
{
	m_LastCreatedHandle = 0;
	m_LastCreatedInterfaceHandle = -1;
	m_VertexBufferAllocations.clear();

	// make sure this hasn't already been done
	if (m_Renderer)
	{
		VERTEXBUFFER_VERTEXFORMAT_MAP::iterator itrFormat = m_VertexBufferMap.begin();
		while (itrFormat != m_VertexBufferMap.end())
		{
			VERTEXBUFFERLIST &VBList = itrFormat->second;
			VERTEXBUFFERLIST::iterator itrVB = VBList.begin();
			while (itrVB != VBList.end())
			{
				m_Renderer->DestroyVertexBuffer( itrVB->second.m_VB );
				itrVB++;
			}
			itrFormat++;
		}
		m_VertexBufferMap.clear();
	}

	return CManager::DeInit();
}


DWORD CVertexBufferManager::OnAllocateVertices(DWORD size, void *params)
{
	ALLOCATERENDERBUFFERMSG * pmsg;
	VERIFY_MESSAGE_SIZE( size, sizeof( ALLOCATERENDERBUFFERMSG ) );
	pmsg = (ALLOCATERENDERBUFFERMSG*)params;

	// buffers of type VertexFormat
	VERTEXBUFFERLIST &vertexBufferList = m_VertexBufferMap[pmsg->VertexFormat];
	// find a buffer with freespace
	if (pmsg->m_AllowSharedBuffer)
	{
		VERTEXBUFFERLIST::reverse_iterator itrBuffer;
		for (itrBuffer = vertexBufferList.rbegin(); itrBuffer != vertexBufferList.rend(); itrBuffer++)
		{
			VBALLOCATIONDESC_MAP::iterator itrFreeSpace;
			for (itrFreeSpace = itrBuffer->second.m_FreeSpace.begin(); itrFreeSpace != itrBuffer->second.m_FreeSpace.end(); itrFreeSpace++)
			{
				UINT iAvailableSize = itrFreeSpace->first;
				if (iAvailableSize >= pmsg->ElementsToAllocate)
				{
					// free space found
					UINT iOffset = itrFreeSpace->second;
					// create new buffer allocation description
					m_VertexBufferAllocations[ m_LastCreatedHandle ].m_AllocationHandle = m_LastCreatedHandle;
					m_VertexBufferAllocations[ m_LastCreatedHandle ].m_Offset = iOffset;
					m_VertexBufferAllocations[ m_LastCreatedHandle ].m_Size = pmsg->ElementsToAllocate;
					m_VertexBufferAllocations[ m_LastCreatedHandle ].m_Stride = pmsg->SizeVertexFormat;
					m_VertexBufferAllocations[ m_LastCreatedHandle ].m_InterfaceHandle = itrBuffer->first;
					m_VertexBufferAllocations[ m_LastCreatedHandle ].VertexFormat = pmsg->VertexFormat;
					// set return values
					pmsg->m_ReturnHandle = m_LastCreatedHandle;
					pmsg->m_ReturnOffset = iOffset;
					pmsg->m_ReturnInterfaceHandle = itrBuffer->first;			
					pmsg->m_InterfacePointer = (void*)itrBuffer->second.m_VB;

					m_LastCreatedHandle++;

					// reinsert new freespace
					itrBuffer->second.m_FreeSpace.erase( itrFreeSpace );
					itrBuffer->second.m_FreeSpace.insert( 
						VBALLOCATIONDESCPAIR( iAvailableSize - pmsg->ElementsToAllocate, 
						iOffset + pmsg->ElementsToAllocate )
						);

					return MSG_HANDLED_STOP;
				}
			}
		}
	}

	// create new vertex buffer
	UINT iInterfaceHandle = m_InterfaceHandleCount++;
	UINT VBSize = (pmsg->m_AllowSharedBuffer) ? m_DefaultVBSize : pmsg->ElementsToAllocate;
	VBAllocationDescriptor &allocDesc = vertexBufferList[iInterfaceHandle];
	allocDesc.m_bSharedBuffer = pmsg->m_AllowSharedBuffer;
	allocDesc.m_VB = m_Renderer->CreateVertexBuffer();
	if (!allocDesc.m_VB)
		return MSG_ERROR;
	if (!allocDesc.m_VB->Initialize( pmsg->VertexFormat, VBSize, pmsg->Dynamic, pmsg->m_StreamMode ))
		return MSG_ERROR;
	// insert default freespace
	if (VBSize > pmsg->ElementsToAllocate)
		allocDesc.m_FreeSpace.insert( VBALLOCATIONDESCPAIR( VBSize - pmsg->ElementsToAllocate, pmsg->ElementsToAllocate ) );

	// create new buffer allocation description
	m_VertexBufferAllocations[ m_LastCreatedHandle ].m_AllocationHandle = m_LastCreatedHandle;
	m_VertexBufferAllocations[ m_LastCreatedHandle ].m_Offset = 0;
	m_VertexBufferAllocations[ m_LastCreatedHandle ].m_Size = pmsg->ElementsToAllocate;
	m_VertexBufferAllocations[ m_LastCreatedHandle ].m_Stride = pmsg->SizeVertexFormat;
	m_VertexBufferAllocations[ m_LastCreatedHandle ].m_InterfaceHandle = iInterfaceHandle;
	m_VertexBufferAllocations[ m_LastCreatedHandle ].VertexFormat = pmsg->VertexFormat;
	// set return values
	pmsg->m_ReturnHandle = m_LastCreatedHandle;
	pmsg->m_ReturnOffset = 0;
	pmsg->m_ReturnInterfaceHandle = iInterfaceHandle;			
	pmsg->m_InterfacePointer = (void*)allocDesc.m_VB;

	m_LastCreatedHandle++;

	return MSG_HANDLED_STOP;
}


DWORD CVertexBufferManager::OnRemoveAllocation( DWORD size, void *params)
{
	BUFFERALLOCATIONSTRUCT * pmsg;
	VERIFY_MESSAGE_SIZE( size, sizeof( BUFFERALLOCATIONSTRUCT ) );
	pmsg = (BUFFERALLOCATIONSTRUCT*)params;
	
	//remove from actual allocation
	//find it in list
	VERTEXBUFFER_VERTEXFORMAT_MAP::iterator iterf =  m_VertexBufferMap.find( pmsg->VertexFormat );
	if( iterf != m_VertexBufferMap.end() )
	{
		//found a mapping for this vertex size
		if (m_VertexBufferMap[ pmsg->VertexFormat ].find( pmsg->m_InterfaceHandle ) != m_VertexBufferMap[ pmsg->VertexFormat ].end())
		{			
			if (m_VertexBufferMap[ pmsg->VertexFormat ][ pmsg->m_InterfaceHandle ].m_bSharedBuffer)
			{
				//should merge shared free space together later
				m_VertexBufferMap[ pmsg->VertexFormat ][ pmsg->m_InterfaceHandle ].m_FreeSpace.insert( VBALLOCATIONDESCPAIR( pmsg->m_Size, pmsg->m_Offset ) );
			}
			else
			{
				// destroy the nonshared buffer
				VERTEXBUFFERLIST::iterator itrMap = m_VertexBufferMap[ pmsg->VertexFormat ].find( pmsg->m_InterfaceHandle );
				if (itrMap != m_VertexBufferMap[ pmsg->VertexFormat ].end())
				{
					m_Renderer->DestroyVertexBuffer( itrMap->second.m_VB );
					m_VertexBufferMap[ pmsg->VertexFormat ].erase( pmsg->m_InterfaceHandle );
				}
			}

			//remove the allocation block
			VERTEXBUFFERMAP::iterator iteralloc = m_VertexBufferAllocations.find( pmsg->m_AllocationHandle );
			if( iteralloc != m_VertexBufferAllocations.end() )
			{
				m_VertexBufferAllocations.erase( iteralloc ); 
			}
		}
	}
	return MSG_HANDLED_STOP;	
}

DWORD CVertexBufferManager::OnGetAllocationBuffer( DWORD size, void *params)
{
	GETALLOCATIONVERTEXBUFFER * pmsg;
	VERIFY_MESSAGE_SIZE( size, sizeof( GETALLOCATIONVERTEXBUFFER ) );
	pmsg = (GETALLOCATIONVERTEXBUFFER*)params;
	VERTEXBUFFERMAP::iterator iteralloc = m_VertexBufferAllocations.find( pmsg->AllocationHandle );
	if( iteralloc != m_VertexBufferAllocations.end() )
	{
		CHANNELDESCRIPTORLIST VertexFormat = m_VertexBufferAllocations[ pmsg->AllocationHandle ].VertexFormat;
		//int VertexSize = m_VertexBufferAllocations[ pmsg->AllocationHandle ].m_Stride;		
		int VBIndex =  m_VertexBufferAllocations[ pmsg->AllocationHandle ].m_InterfaceHandle;
        pmsg->m_VertexBufferObject = m_VertexBufferMap[ VertexFormat ][ VBIndex ].m_VB;
		return MSG_HANDLED_STOP;
	}
	else
	{
		pmsg->m_VertexBufferObject = NULL;
		return MSG_NOT_HANDLED;
	}
}

DWORD CVertexBufferManager::OnDestroyRenderer( DWORD size, void *param )
{
	// if the renderer dll is detaching, we have to clear up some stuff
	if (m_Renderer)
	{
		VERTEXBUFFER_VERTEXFORMAT_MAP::iterator itrFormat = m_VertexBufferMap.begin();
		while (itrFormat != m_VertexBufferMap.end())
		{
			VERTEXBUFFERLIST &VBList = itrFormat->second;
			VERTEXBUFFERLIST::iterator itrVB = VBList.begin();
			while (itrVB != VBList.end())
			{
				m_Renderer->DestroyVertexBuffer( itrVB->second.m_VB );
				itrVB++;
			}
			itrFormat++;
		}
		m_VertexBufferMap.clear();
	}
	m_Renderer = NULL;
	return MSG_HANDLED_PROCEED;
}