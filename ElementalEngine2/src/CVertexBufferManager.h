///============================================================================
/// \file	CVertexBufferManager.h
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
#include "IVertexBufferObject.h"
#include "IRenderer.h"
#include "IMeshObject.h"

typedef DWORD	VBSIZE;
typedef DWORD	VBOFFSET;
typedef pair< VBSIZE, VBOFFSET > VBALLOCATIONDESCPAIR;
typedef multimap< VBSIZE, VBOFFSET > VBALLOCATIONDESC_MAP;
struct VBAllocationDescriptor
{
	/// map of size to allocation index
	VBALLOCATIONDESC_MAP	m_FreeSpace;
	IVertexBufferObject *			m_VB;
	bool m_bSharedBuffer;
};

typedef std::map< UINT, VBAllocationDescriptor > VERTEXBUFFERLIST;
typedef std::map<CHANNELDESCRIPTORLIST, VERTEXBUFFERLIST, VERTEXDESCRIPTIONCOMPARE> VERTEXBUFFER_VERTEXFORMAT_MAP; //maps vertex buffer to vertex format
//typedef std::map<DWORD, VERTEXBUFFERLIST> VERTEXBUFFER_VERTEXFORMAT_MAP; //maps vertex buffer to vertex format
typedef std::map<DWORD, BUFFERALLOCATIONSTRUCT > VERTEXBUFFERMAP;
typedef std::pair<DWORD, BUFFERALLOCATIONSTRUCT > VERTEXBUFFERMAP_ELEMENT;

class CVertexBufferManager : public CManager
{
public:

	/// \brief The Default Destructor for CVertexBufferManager
	/// \return void
	~CVertexBufferManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief	DeInitialization
	/// \return	bool: Success
	virtual bool DeInit( void );
	
	DWORD OnAllocateVertices(DWORD size, void *params);
	DWORD OnRemoveAllocation( DWORD size, void *params);
	DWORD OnGetAllocationBuffer( DWORD size, void *params);
	DWORD OnDestroyRenderer( DWORD size, void *param );

private:
	SINGLETONCONSTRUCTOROVERRIDE( CVertexBufferManager );
	/// \brief The Default Constructor for CVertexBufferManager
	/// \return void
	CVertexBufferManager();

	/// mapping of vertex buffer list to vertex format
	VERTEXBUFFER_VERTEXFORMAT_MAP	m_VertexBufferMap; 
	VERTEXBUFFERMAP					m_VertexBufferAllocations;
	UINT			m_DefaultVBSize;
	/// renderer
	IRenderer * m_Renderer;

	DWORD		m_LastCreatedHandle;
	UINT		m_LastCreatedInterfaceHandle;
	UINT		m_InterfaceHandleCount;
};



#endif