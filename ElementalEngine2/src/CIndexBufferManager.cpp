///============================================================================
/// \file	CIndexedBufferManager.cpp
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
#include "CIndexBufferManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_IB_SIZE 65536

REGISTER_COMPONENT_SINGLETON( CIndexBufferManager );
REGISTER_MESSAGE_HANDLER( AllocateIndices, OnAllocateIndices, CIndexBufferManager);
REGISTER_MESSAGE_HANDLER( GetIndexAllocationInterface, OnGetAllocationBuffer, CIndexBufferManager );
REGISTER_MESSAGE_HANDLER( RemoveIndexAllocation, OnRemoveAllocation, CIndexBufferManager );
REGISTER_MESSAGE_HANDLER_PRIORITY( DestroyRenderer, OnDestroyRenderer, CIndexBufferManager, 0x70000000 );

/// \brief The Default Constructor for CVertexBufferManager
/// \return void
CIndexBufferManager::CIndexBufferManager() : 
	CManager(_T("CIndexBufferManager"))
{
	m_DefaultIBSize = DEFAULT_IB_SIZE;
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
IComponent *CIndexBufferManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CIndexBufferManager );
}

/// \brief The Default Destructor for CIndexBufferManager
/// \return void
CIndexBufferManager::~CIndexBufferManager( )
{
	DeInit();
}

/// \brief	DeInitializes the CIndexBufferManager
/// \return	bool: Success
bool CIndexBufferManager::DeInit( void )
{	
	m_LastCreatedHandle = 0;
	m_LastCreatedInterfaceHandle = -1;
	m_IndexBufferAllocations.clear();

	// make sure the renderer hasn't already been cleared
	if (m_Renderer)
	{
		INDEXBUFFER_MAP::iterator itrFormat = m_IndexBufferMap.begin();
		while (itrFormat != m_IndexBufferMap.end())
		{
			INDEXBUFFERLIST &IBList = itrFormat->second;
			INDEXBUFFERLIST::iterator itrIB = IBList.begin();
			while (itrIB != IBList.end())
			{
				m_Renderer->DestroyIndexBuffer( itrIB->second.m_IB );			
				itrIB++;
			}
			itrFormat++;
		}
		m_IndexBufferMap.clear();
	}

	return CManager::DeInit();
}


DWORD CIndexBufferManager::OnAllocateIndices(DWORD size, void *params)
{
	ALLOCATERENDERBUFFERMSG * pmsg;
	VERIFY_MESSAGE_SIZE( size, sizeof( ALLOCATERENDERBUFFERMSG ) );
	pmsg = (ALLOCATERENDERBUFFERMSG*)params;

	//create the allocation
	//find out if we have a buffer to accomodate the vertex type
	//find the FVF in our array
	DWORD VBIndex = -1;
	INDEXBUFFER_MAP::iterator iterf =  m_IndexBufferMap.find( pmsg->SizeVertexFormat );
	if( !pmsg->m_AllowSharedBuffer || iterf == m_IndexBufferMap.end() )
	{
		//format not found, create new entry
		int IBSize = (pmsg->m_AllowSharedBuffer) ? m_DefaultIBSize : pmsg->ElementsToAllocate;
		IIndexBuffer * newIB = m_Renderer->CreateIndexBuffer();
		newIB->Resize( IBSize );
		IBAllocationDescriptor desc;
		desc.m_FreeSpace.insert( IBALLOCATIONDESCPAIR( IBSize, 0 ) );
		desc.m_IB = newIB;
		desc.m_bSharedBuffer = pmsg->m_AllowSharedBuffer;
		VBIndex = m_InterfaceHandleCount++;
		if (desc.m_bSharedBuffer)
			m_LastCreatedInterfaceHandle = VBIndex;
		m_IndexBufferMap[ pmsg->SizeVertexFormat ][VBIndex] = desc;
	}
	else
	{
        //check full, not done
		VBIndex = m_LastCreatedInterfaceHandle;
	}
	//now get an allocation block
    //find a size that fits
	IBALLOCATIONDESC_MAP::iterator iter;
	bool sizefound = false;
	INDEXBUFFERLIST::iterator itrMap = m_IndexBufferMap[ pmsg->SizeVertexFormat ].find(VBIndex);
	if (itrMap != m_IndexBufferMap[ pmsg->SizeVertexFormat ].end())
	{
		iter = itrMap->second.m_FreeSpace.end();
		do//kind of weird since we're iterating backward
		{
			if (iter == itrMap->second.m_FreeSpace.begin())
				break;
			iter--;
			DWORD AvailableSize = (*iter).first;
			if( AvailableSize >= pmsg->ElementsToAllocate )
			{
				sizefound = true;
				break;
			}
		}
		while( iter != itrMap->second.m_FreeSpace.begin() );
	}

	if( !sizefound &&
		pmsg->m_AllowSharedBuffer )
	{
		//format not found, create new entry
		int IBSize = m_DefaultIBSize;
		IIndexBuffer * newIB = m_Renderer->CreateIndexBuffer();
		newIB->Resize( IBSize );
		IBAllocationDescriptor desc;
		desc.m_FreeSpace.insert( IBALLOCATIONDESCPAIR( IBSize, 0 ) );
		desc.m_IB = newIB;
		desc.m_bSharedBuffer = true;
		VBIndex = m_InterfaceHandleCount++;
		if (desc.m_bSharedBuffer)
			m_LastCreatedInterfaceHandle = VBIndex;
		m_IndexBufferMap[ pmsg->SizeVertexFormat ][VBIndex] = desc;
		iter = m_IndexBufferMap[ pmsg->SizeVertexFormat ][ VBIndex ].m_FreeSpace.end();
		iter--;
	}

	if( iter != m_IndexBufferMap[ pmsg->SizeVertexFormat ][ VBIndex ].m_FreeSpace.end() )
	{
		//use this iterator
		DWORD AvailableSize = (*iter).first;
		DWORD curOffset = (*iter).second;
		//create an allocation block:
		if( AvailableSize >= pmsg->ElementsToAllocate )
		{
			m_IndexBufferAllocations[ m_LastCreatedHandle ].m_AllocationHandle = m_LastCreatedHandle;
			m_IndexBufferAllocations[ m_LastCreatedHandle ].m_Offset = curOffset;
			m_IndexBufferAllocations[ m_LastCreatedHandle ].m_Size = pmsg->ElementsToAllocate;
			m_IndexBufferAllocations[ m_LastCreatedHandle ].m_Stride = pmsg->SizeVertexFormat;
			m_IndexBufferAllocations[ m_LastCreatedHandle ].m_InterfaceHandle = VBIndex;
			pmsg->m_ReturnHandle = m_LastCreatedHandle;
			pmsg->m_ReturnOffset = curOffset;
			pmsg->m_ReturnInterfaceHandle = VBIndex;
			pmsg->m_InterfacePointer = (void*)m_IndexBufferMap[ pmsg->SizeVertexFormat ][ VBIndex ].m_IB;
			m_LastCreatedHandle++;
			m_IndexBufferMap[ pmsg->SizeVertexFormat ][ VBIndex ].m_FreeSpace.erase( iter );
			m_IndexBufferMap[ pmsg->SizeVertexFormat ][ VBIndex ].m_FreeSpace.insert( IBALLOCATIONDESCPAIR( AvailableSize - pmsg->ElementsToAllocate, curOffset + pmsg->ElementsToAllocate ) );
			return MSG_HANDLED_STOP;
		}
	}
	return MSG_HANDLED_STOP;
}


DWORD CIndexBufferManager::OnRemoveAllocation( DWORD size, void *params)
{
	BUFFERALLOCATIONSTRUCT * pmsg;
	VERIFY_MESSAGE_SIZE( size, sizeof( BUFFERALLOCATIONSTRUCT ) );
	pmsg = (BUFFERALLOCATIONSTRUCT*)params;
	
	//remove from actual allocation
	//find it in list
	int tempFVF = pmsg->m_Stride;
	INDEXBUFFER_MAP::iterator iterf =  m_IndexBufferMap.find( tempFVF );
	if( iterf != m_IndexBufferMap.end() )
	{
		//found a mapping for this vertex size
		if (m_IndexBufferMap[ tempFVF ].find( pmsg->m_InterfaceHandle ) != m_IndexBufferMap[ tempFVF ].end())
		{			
			if (m_IndexBufferMap[ tempFVF ][ pmsg->m_InterfaceHandle ].m_bSharedBuffer)
			{
				//should merge free space together later
				m_IndexBufferMap[ tempFVF ][ pmsg->m_InterfaceHandle ].m_FreeSpace.insert( IBALLOCATIONDESCPAIR( pmsg->m_Size, pmsg->m_Offset ) );
			}
			else
			{
				// destroy the nonshared buffer
				INDEXBUFFERLIST::iterator itrMap = m_IndexBufferMap[ tempFVF ].find( pmsg->m_InterfaceHandle );
				if (itrMap != m_IndexBufferMap[ tempFVF ].end())
				{
					m_Renderer->DestroyIndexBuffer( itrMap->second.m_IB );
					m_IndexBufferMap[ tempFVF ].erase( pmsg->m_InterfaceHandle );
				}
			}

			//remove the allocation block
			INDEXBUFFERALLOCATIONMAP::iterator iteralloc = m_IndexBufferAllocations.find( pmsg->m_AllocationHandle );
			if( iteralloc != m_IndexBufferAllocations.end() )
			{
				m_IndexBufferAllocations.erase( iteralloc ); 
			}
		}
	}
	return MSG_HANDLED_STOP;	
}

DWORD CIndexBufferManager::OnGetAllocationBuffer( DWORD size, void *params)
{
	GETALLOCATIONINDEXBUFFER * pmsg;
	VERIFY_MESSAGE_SIZE( size, sizeof( GETALLOCATIONINDEXBUFFER ) );
	pmsg = (GETALLOCATIONINDEXBUFFER*)params;
	INDEXBUFFERALLOCATIONMAP::iterator iteralloc = m_IndexBufferAllocations.find( pmsg->AllocationHandle );
	if( iteralloc != m_IndexBufferAllocations.end() )
	{
		int VertexSize = m_IndexBufferAllocations[ pmsg->AllocationHandle ].m_Stride;
		int VBIndex =  m_IndexBufferAllocations[ pmsg->AllocationHandle ].m_InterfaceHandle;
        pmsg->m_IndexBufferInterface = m_IndexBufferMap[ VertexSize ][ VBIndex ].m_IB;
		return MSG_HANDLED_STOP;
	}

	// none found, but we did handle the message
	pmsg->m_IndexBufferInterface = NULL;
	return MSG_HANDLED_STOP;
}

DWORD CIndexBufferManager::OnDestroyRenderer( DWORD size, void *param )
{
	// if the dll is detaching, we have to free up resources
	if (m_Renderer)
	{
		INDEXBUFFER_MAP::iterator itrFormat = m_IndexBufferMap.begin();
		while (itrFormat != m_IndexBufferMap.end())
		{
			INDEXBUFFERLIST &IBList = itrFormat->second;
			INDEXBUFFERLIST::iterator itrIB = IBList.begin();
			while (itrIB != IBList.end())
			{
				m_Renderer->DestroyIndexBuffer( itrIB->second.m_IB );			
				itrIB++;
			}
			itrFormat++;
		}
		m_IndexBufferMap.clear();
	}
	m_Renderer = NULL;
	return MSG_HANDLED_PROCEED;
}