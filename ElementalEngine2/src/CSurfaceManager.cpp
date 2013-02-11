#include "Stdafx.h"
#include "CSurfaceManager.h"
#include "CIndexBuffer.h"

#define DEFAULT_VB_SIZE 16384
#define DEFAULT_IB_SIZE 16384

REGISTER_COMPONENT_SINGLETON( CVertexBufferManager );
REGISTER_MESSAGE_HANDLER( AllocateVertices, OnAllocateVertices, CVertexBufferManager);
REGISTER_MESSAGE_HANDLER( GetVertexAllocationInterface, OnGetAllocationBuffer, CVertexBufferManager );
REGISTER_MESSAGE_HANDLER( RemoveVertexAllocation, OnRemoveAllocation, CVertexBufferManager );

REGISTER_COMPONENT_SINGLETON( CIndexBufferManager );
REGISTER_MESSAGE_HANDLER( AllocateIndices, OnAllocateIndices, CIndexBufferManager);
REGISTER_MESSAGE_HANDLER( GetIndexAllocationInterface, OnGetAllocationBuffer, CIndexBufferManager );
REGISTER_MESSAGE_HANDLER( RemoveIndexAllocation, OnRemoveAllocation, CIndexBufferManager );

/// \brief The Default Constructor for CVertexBufferManager
/// \return void
CVertexBufferManager::CVertexBufferManager() : CManager(0)
{
	m_ToolBox = EngineGetToolBox();	
	m_DefaultVBSize = DEFAULT_VB_SIZE;
	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;

	m_ToolBox->SendMessage(_T("GetRendererInterface"), 
						   sizeof(GETRENDERERINTERFACEPARAMS), 
						   &renderInterface, 
						   NULL, 
						   NULL);

	m_Renderer = renderInterface.m_RendererInterface;
	m_LastCreatedHandle = 0;
}


/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CVertexBufferManager::Create(va_list argptr)
{
	return SINGLETONINSTANCE( CVertexBufferManager );
}

/// \brief The Default Destructor for CVertexBufferManager
/// \return void
CVertexBufferManager::~CVertexBufferManager( )
{
	
}
// Initializes the manager
bool CVertexBufferManager::Init()
{
	return true;
}

/// \brief	Update function for the CVertexBufferManager
///			Renders immediately, the queued list of renderobjects
/// \return	void
void CVertexBufferManager::Update( void )
{	
}

/// \brief	DeInitializes the CVertexBufferManager
/// \return	bool: Success
bool CVertexBufferManager::DeInit( void )
{	
	return true;
}


DWORD CVertexBufferManager::OnAllocateVertices(DWORD size, void *params)
{
	ALLOCATERENDERBUFFERMSG * pmsg;
	VERIFY_MESSAGE_SIZE( size, sizeof( ALLOCATERENDERBUFFERMSG ) );
	pmsg = (ALLOCATERENDERBUFFERMSG*)params;
	//create the allocation
	//find out if we have a buffer to accomodate the vertex type
	//find the FVF in our array
	DWORD tempFVF = pmsg->SizeVertexFormat;//CRenderer::CreateVertexBuffer();
	DWORD VBIndex = -1;
	VERTEXBUFFER_VERTEXFORMAT_MAP::iterator iterf =  m_VertexBufferMap.find( tempFVF );
	if( iterf == m_VertexBufferMap.end() )
	{
		//format not found, create new entry
		IVertexBufferObject * newVB = m_Renderer->CreateVertexBuffer();
		newVB->Initialize( pmsg->SizeVertexFormat, m_DefaultVBSize, false );
		VBAllocationDescriptor desc;
		desc.m_FreeSpace.insert( VBALLOCATIONDESCPAIR( m_DefaultVBSize, 0 ) );
		desc.m_VB = newVB;
		m_VertexBufferMap[ tempFVF ].push_back( desc );
		VBIndex = 0;
	}
	else
	{
        //check full, not done
		VBIndex = 0;
	}
	//now get an allocation block
    //find a size that fits
	VBALLOCATIONDESC_MAP::iterator iter = m_VertexBufferMap[ tempFVF ][ VBIndex ].m_FreeSpace.end();
	do//kind of weird since we're iterating backward
	{
		iter--;
		DWORD AvailableSize = (*iter).first;
		if( AvailableSize > pmsg->ElementsToAllocate )
		{
			//use this iterator
			DWORD curOffset = (*iter).second;
			//create an allocation block:
			m_VertexBufferAllocations[ m_LastCreatedHandle ].m_AllocationHandle = m_LastCreatedHandle;
			m_VertexBufferAllocations[ m_LastCreatedHandle ].m_Offset = curOffset;
			m_VertexBufferAllocations[ m_LastCreatedHandle ].m_Size = pmsg->SizeVertexFormat;
			m_VertexBufferAllocations[ m_LastCreatedHandle ].m_Stride = tempFVF;
			m_VertexBufferAllocations[ m_LastCreatedHandle ].m_InterfaceHandle = VBIndex;
			pmsg->m_ReturnHandle = m_LastCreatedHandle;
			pmsg->m_ReturnOffset = curOffset;
			pmsg->m_ReturnInterfaceHandle = VBIndex;
			m_LastCreatedHandle++;
			m_VertexBufferMap[ tempFVF ][ VBIndex ].m_FreeSpace.erase( iter );
			m_VertexBufferMap[ tempFVF ][ VBIndex ].m_FreeSpace.insert( VBALLOCATIONDESCPAIR( AvailableSize - pmsg->ElementsToAllocate, curOffset + pmsg->ElementsToAllocate ) );
		}
	}
    while( iter != m_VertexBufferMap[ tempFVF ][ VBIndex ].m_FreeSpace.begin() );
	
	

	return MSG_HANDLED;
}


DWORD CVertexBufferManager::OnRemoveAllocation( DWORD size, void *params)
{
	BufferAllocation * pmsg;
	VERIFY_MESSAGE_SIZE( size, sizeof( BufferAllocation ) );
	pmsg = (BufferAllocation*)params;
	
	//remove from actual allocation
	//find it in list
	int tempFVF = pmsg->m_Stride;
	VERTEXBUFFER_VERTEXFORMAT_MAP::iterator iterf =  m_VertexBufferMap.find( tempFVF );
	if( iterf != m_VertexBufferMap.end() )
	{
		//found a mapping for this vertex size
		if(  pmsg->m_InterfaceHandle >= 0 &&
			 pmsg->m_InterfaceHandle < (int)m_VertexBufferMap[ tempFVF ].size() )
		{			
			//should merge free space together later
			m_VertexBufferMap[ tempFVF ][ pmsg->m_InterfaceHandle ].m_FreeSpace.insert( VBALLOCATIONDESCPAIR( pmsg->m_Size, pmsg->m_Offset ) );
			//remove the allocation block
			VERTEXBUFFERMAP::iterator iteralloc = m_VertexBufferAllocations.find( pmsg->m_AllocationHandle );
			if( iteralloc != m_VertexBufferAllocations.end() )
			{
				m_VertexBufferAllocations.erase( iteralloc ); 
			}
		}
	}
	return MSG_HANDLED;	
}

DWORD CVertexBufferManager::OnGetAllocationBuffer( DWORD size, void *params)
{
	GETALLOCATIONVERTEXBUFFER * pmsg;
	VERIFY_MESSAGE_SIZE( size, sizeof( GETALLOCATIONVERTEXBUFFER ) );
	pmsg = (GETALLOCATIONVERTEXBUFFER*)params;
	VERTEXBUFFERMAP::iterator iteralloc = m_VertexBufferAllocations.find( pmsg->AllocationHandle );
	if( iteralloc != m_VertexBufferAllocations.end() )
	{
		int VertexSize = m_VertexBufferAllocations[ pmsg->AllocationHandle ].m_Stride;
		int VBIndex =  m_VertexBufferAllocations[ pmsg->AllocationHandle ].m_InterfaceHandle;
        pmsg->m_VertexBufferObject = m_VertexBufferMap[ VertexSize ][ VBIndex ].m_VB;
		return MSG_HANDLED;
	}
	else
	{
		pmsg->m_VertexBufferObject = NULL;
		return MSG_NOT_HANDLED;
	}
}


//-------------------------------------------------------------------------------------------------



/// \brief The Default Constructor for CVertexBufferManager
/// \return void
CIndexBufferManager::CIndexBufferManager() : CManager(0)
{
	m_ToolBox = EngineGetToolBox();	
	m_DefaultIBSize = DEFAULT_IB_SIZE;
	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;

	m_ToolBox->SendMessage(_T("GetRendererInterface"), 
						   sizeof(GETRENDERERINTERFACEPARAMS), 
						   &renderInterface, 
						   NULL, 
						   NULL);

	m_Renderer = renderInterface.m_RendererInterface;
	m_LastCreatedHandle = 0;
}


/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CIndexBufferManager::Create(va_list argptr)
{
	return SINGLETONINSTANCE( CIndexBufferManager );
}

/// \brief The Default Destructor for CIndexBufferManager
/// \return void
CIndexBufferManager::~CIndexBufferManager( )
{
	
}
// Initializes the manager
bool CIndexBufferManager::Init()
{
	return true;
}

/// \brief	Update function for the CIndexBufferManager
///			Renders immediately, the queued list of renderobjects
/// \return	void
void CIndexBufferManager::Update( void )
{	
}

/// \brief	DeInitializes the CIndexBufferManager
/// \return	bool: Success
bool CIndexBufferManager::DeInit( void )
{	
	return true;
}


DWORD CIndexBufferManager::OnAllocateIndices(DWORD size, void *params)
{
	ALLOCATERENDERBUFFERMSG * pmsg;
	VERIFY_MESSAGE_SIZE( size, sizeof( ALLOCATERENDERBUFFERMSG ) );
	pmsg = (ALLOCATERENDERBUFFERMSG*)params;
	//create the allocation
	//find out if we have a buffer to accomodate the vertex type
	//find the FVF in our array
	DWORD tempFVF = pmsg->SizeVertexFormat;//CRenderer::CreateVertexBuffer();
	DWORD VBIndex = -1;
	INDEXBUFFER_MAP::iterator iterf =  m_IndexBufferMap.find( tempFVF );
	if( iterf == m_IndexBufferMap.end() )
	{
		//format not found, create new entry
		IIndexBuffer * newIB = m_Renderer->CreateIndexBuffer();
		newIB->Resize( m_DefaultIBSize );
		IBAllocationDescriptor desc;
		desc.m_FreeSpace.insert( IBALLOCATIONDESCPAIR( m_DefaultIBSize, 0 ) );
		desc.m_IB = newIB;
		m_IndexBufferMap[ tempFVF ].push_back( desc );
		VBIndex = 0;
	}
	else
	{
        //check full, not done
		VBIndex = 0;
	}
	//now get an allocation block
    //find a size that fits
	VBALLOCATIONDESC_MAP::iterator iter = m_IndexBufferMap[ tempFVF ][ VBIndex ].m_FreeSpace.end();
	do//kind of weird since we're iterating backward
	{
		iter--;
		DWORD AvailableSize = (*iter).first;
		if( AvailableSize > pmsg->ElementsToAllocate )
		{
			//use this iterator
			DWORD curOffset = (*iter).second;
			//create an allocation block:
			m_IndexBufferAllocations[ m_LastCreatedHandle ].m_AllocationHandle = m_LastCreatedHandle;
			m_IndexBufferAllocations[ m_LastCreatedHandle ].m_Offset = curOffset;
			m_IndexBufferAllocations[ m_LastCreatedHandle ].m_Size = pmsg->SizeVertexFormat;
			m_IndexBufferAllocations[ m_LastCreatedHandle ].m_Stride = tempFVF;
			m_IndexBufferAllocations[ m_LastCreatedHandle ].m_InterfaceHandle = VBIndex;
			pmsg->m_ReturnHandle = m_LastCreatedHandle;
			pmsg->m_ReturnOffset = curOffset;
			pmsg->m_ReturnInterfaceHandle = VBIndex;
			m_LastCreatedHandle++;
			m_IndexBufferMap[ tempFVF ][ VBIndex ].m_FreeSpace.erase( iter );
			m_IndexBufferMap[ tempFVF ][ VBIndex ].m_FreeSpace.insert( VBALLOCATIONDESCPAIR( AvailableSize - pmsg->ElementsToAllocate, curOffset + pmsg->ElementsToAllocate ) );
		}
	}
    while( iter != m_IndexBufferMap[ tempFVF ][ VBIndex ].m_FreeSpace.begin() );
	
	return MSG_HANDLED;
}


DWORD CIndexBufferManager::OnRemoveAllocation( DWORD size, void *params)
{
	BufferAllocation * pmsg;
	VERIFY_MESSAGE_SIZE( size, sizeof( BufferAllocation ) );
	pmsg = (BufferAllocation*)params;
	
	//remove from actual allocation
	//find it in list
	int tempFVF = pmsg->m_Stride;
	INDEXBUFFER_MAP::iterator iterf =  m_IndexBufferMap.find( tempFVF );
	if( iterf != m_IndexBufferMap.end() )
	{
		//found a mapping for this vertex size
		if(  pmsg->m_InterfaceHandle >= 0 &&
			 pmsg->m_InterfaceHandle < (int)m_IndexBufferMap[ tempFVF ].size() )
		{			
			//should merge free space together later
			m_IndexBufferMap[ tempFVF ][ pmsg->m_InterfaceHandle ].m_FreeSpace.insert( VBALLOCATIONDESCPAIR( pmsg->m_Size, pmsg->m_Offset ) );
			//remove the allocation block
			VERTEXBUFFERMAP::iterator iteralloc = m_IndexBufferAllocations.find( pmsg->m_AllocationHandle );
			if( iteralloc != m_IndexBufferAllocations.end() )
			{
				m_IndexBufferAllocations.erase( iteralloc ); 
			}
		}
	}
	return MSG_HANDLED;	
}

DWORD CIndexBufferManager::OnGetAllocationBuffer( DWORD size, void *params)
{
	GETALLOCATIONINDEXBUFFER * pmsg;
	VERIFY_MESSAGE_SIZE( size, sizeof( GETALLOCATIONINDEXBUFFER ) );
	pmsg = (GETALLOCATIONINDEXBUFFER*)params;
	VERTEXBUFFERMAP::iterator iteralloc = m_IndexBufferAllocations.find( pmsg->AllocationHandle );
	if( iteralloc != m_IndexBufferAllocations.end() )
	{
		int VertexSize = m_IndexBufferAllocations[ pmsg->AllocationHandle ].m_Stride;
		int VBIndex =  m_IndexBufferAllocations[ pmsg->AllocationHandle ].m_InterfaceHandle;
        pmsg->m_IndexBufferInterface = m_IndexBufferMap[ VertexSize ][ VBIndex ].m_IB;
		return MSG_HANDLED;
	}
	else
	{
		pmsg->m_IndexBufferInterface = NULL;
		return MSG_NOT_HANDLED;
	}
}