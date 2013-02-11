///============================================================================
/// \file	CIndexedBufferManager.h
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

#ifndef CVertexBufferManager_H
#define CVertexBufferManager_H

#include "CManager.h"
#include "IRenderer.h"

typedef DWORD	IBSIZE;
typedef DWORD	IBOFFSET;
typedef pair< IBSIZE, IBOFFSET > IBALLOCATIONDESCPAIR;
typedef multimap< IBSIZE, IBOFFSET > IBALLOCATIONDESC_MAP;

struct IBAllocationDescriptor
{
	/// map of size to allocation index
	IBALLOCATIONDESC_MAP	m_FreeSpace;
	IIndexBuffer *			m_IB;
	bool m_bSharedBuffer;
};

typedef std::map< UINT, IBAllocationDescriptor > INDEXBUFFERLIST;
typedef std::map<DWORD, INDEXBUFFERLIST > INDEXBUFFER_MAP; //maps vertex buffer to vertex format
typedef std::map<DWORD, BUFFERALLOCATIONSTRUCT > INDEXBUFFERALLOCATIONMAP;
typedef std::pair<DWORD, BUFFERALLOCATIONSTRUCT > INDEXBUFFERMAP_ELEMENT;

class CIndexBufferManager : public CManager
{
public:

	/// \brief The Default Destructor for CVertexBufferManager
	/// \return void
	~CIndexBufferManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief	DeInitialization
	/// \return	bool: Success
	virtual bool DeInit( void );
	
	DWORD OnAllocateIndices(DWORD size, void *params);
	DWORD OnRemoveAllocation( DWORD size, void *params);
	DWORD OnGetAllocationBuffer( DWORD size, void *params);
	DWORD OnDestroyRenderer( DWORD size, void *param );

private:
	SINGLETONCONSTRUCTOROVERRIDE( CIndexBufferManager );
	/// \brief The Default Constructor for CVertexBufferManager
	/// \return void
	CIndexBufferManager();

	/// mapping of vertex buffer list to vertex format
	INDEXBUFFER_MAP	m_IndexBufferMap; 
	INDEXBUFFERALLOCATIONMAP					m_IndexBufferAllocations;
	UINT			m_DefaultIBSize;
	/// renderer
	IRenderer * m_Renderer;

	DWORD		m_LastCreatedHandle;
	UINT		m_LastCreatedInterfaceHandle;
	UINT		m_InterfaceHandleCount;
};


#endif