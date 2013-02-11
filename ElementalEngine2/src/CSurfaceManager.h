#ifndef CVertexBufferManager_H
#define CVertexBufferManager_H

#include "CManager.h"
#include "IVertexBufferObject.h"
#include "IRenderer.h"

typedef DWORD	VBSIZE;
typedef DWORD	VBOFFSET;
typedef pair< VBSIZE, VBOFFSET > VBALLOCATIONDESCPAIR;
typedef multimap< VBSIZE, VBOFFSET > VBALLOCATIONDESC_MAP;
struct VBAllocationDescriptor
{
	/// map of size to allocation index
	VBALLOCATIONDESC_MAP	m_FreeSpace;
	IVertexBufferObject *			m_VB;
};

typedef std::vector< VBAllocationDescriptor > VERTEXBUFFERLIST;
typedef std::map<DWORD, VERTEXBUFFERLIST > VERTEXBUFFER_VERTEXFORMAT_MAP; //maps vertex buffer to vertex format
typedef std::map<DWORD, BufferAllocation > VERTEXBUFFERMAP;
typedef std::pair<DWORD, BufferAllocation > VERTEXBUFFERMAP_ELEMENT;

class CVertexBufferManager : public CManager
{
public:

	/// \brief The Default Destructor for CVertexBufferManager
	/// \return void
	~CVertexBufferManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( va_list argptr );

	/// \brief	Initializes the Particle Manager
	/// \param	void
	/// \return	bool: Success
	virtual bool Init( void );

	/// \brief	Update function for the Particle Manager
	/// \return	void
	virtual void Update( void );
	
	/// \brief	DeInitialization
	/// \return	bool: Success
	virtual bool DeInit( void );
	
	DWORD OnAllocateVertices(DWORD size, void *params);
	DWORD OnRemoveAllocation( DWORD size, void *params);
	DWORD OnGetAllocationBuffer( DWORD size, void *params);

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
};



typedef DWORD	IBSIZE;
typedef DWORD	IBOFFSET;
typedef pair< IBSIZE, IBOFFSET > IBALLOCATIONDESCPAIR;
typedef multimap< IBSIZE, IBOFFSET > IBALLOCATIONDESC_MAP;

struct IBAllocationDescriptor
{
	/// map of size to allocation index
	IBALLOCATIONDESC_MAP	m_FreeSpace;
	IIndexBuffer *			m_IB;
};

typedef std::vector< IBAllocationDescriptor > INDEXBUFFERLIST;
typedef std::map<DWORD, INDEXBUFFERLIST > INDEXBUFFER_MAP; //maps vertex buffer to vertex format
typedef std::map<DWORD, BufferAllocation > INDEXBUFFERALLOCATIONMAP;
typedef std::pair<DWORD, BufferAllocation > INDEXBUFFERMAP_ELEMENT;

class CIndexBufferManager : public CManager
{
public:

	/// \brief The Default Destructor for CVertexBufferManager
	/// \return void
	~CIndexBufferManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( va_list argptr );

	/// \brief	Initializes the Particle Manager
	/// \param	void
	/// \return	bool: Success
	virtual bool Init( void );

	/// \brief	Update function for the Particle Manager
	/// \return	void
	virtual void Update( void );
	
	/// \brief	DeInitialization
	/// \return	bool: Success
	virtual bool DeInit( void );
	
	DWORD OnAllocateIndices(DWORD size, void *params);
	DWORD OnRemoveAllocation( DWORD size, void *params);
	DWORD OnGetAllocationBuffer( DWORD size, void *params);

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
};


#endif