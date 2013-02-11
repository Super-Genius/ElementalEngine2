#ifndef CINDEXBUFFER_H
#define CINDEXBUFFER_H

#include "IIndexBuffer.h"
#include "CVertexBufferObject.h"

//kind of hard to assocaite with vision since we have to hold a whole mesh buffer. hrmm
class CIndexBuffer : public IIndexBuffer
{
public:	
	~CIndexBuffer()
	{

	}

	CIndexBuffer()
	{
		m_pMeshBuffer = NULL;
		m_pMeshBufferObj = NULL;
	}
	virtual bool Resize( UINT numElements ){
		if( m_pMeshBuffer )
		{
			m_pMeshBuffer->AllocateIndexList( numElements );
			return true;
		}
		return false;
	};
	virtual UINT GetSize()
	{
		if( m_pMeshBuffer )
		{
			return m_pMeshBuffer->GetIndexCount();
		}
		return 0;
	}
	virtual bool Lock( UINT offset, UINT size, void ** pData )
	{
		if( m_pMeshBuffer )
		{
			*pData = m_pMeshBuffer->LockIndices( 0 );
			return true;
		}
		return false;
	}
	virtual bool Unlock()
	{
		if( m_pMeshBuffer )
		{
			m_pMeshBuffer->UnLockIndices( );
			return true;
		}
		return false;
	}

	//Vision dependent for now, only one IB per VB right now because of vision(unless we want to rewrite vision's hooks into OGL)
	virtual void SetVertexBuffer( IVertexBufferObject * vb )
	{
		if( vb )
		{
			CVertexBufferObject * cvb = dynamic_cast< CVertexBufferObject * >( vb );
			if( cvb )
			{
				m_pMeshBuffer = cvb->GetVisMeshBuffer();
				m_pMeshBufferObj = cvb->GetVisMeshBufferObj();
			}
		}
	}
	VisMeshBufferPtr m_pMeshBuffer;
	VisMeshBufferObject_cl *m_pMeshBufferObj;
};

#endif //#ifndef CINDEXBUFFER_H