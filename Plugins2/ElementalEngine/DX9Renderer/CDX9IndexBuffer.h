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

#ifndef CDX9IndexBuffer_H
#define CDX9IndexBuffer_H

#include "IIndexBuffer.h"

namespace ElementalEngine
{


class CDX9IndexBuffer : public IIndexBuffer
{
public:	
	~CDX9IndexBuffer();
	CDX9IndexBuffer();
	virtual bool Resize( UINT numElements );
	virtual UINT GetSize();
	virtual bool Lock( UINT offset, UINT size, void ** pData );
	virtual bool Unlock();
	
	LPDIRECT3DINDEXBUFFER9 GetDX9Interface(){
		return m_IBInterface;
	}
private:
	void DeInit();

	UINT m_Size;
	LPDIRECT3DINDEXBUFFER9 m_IBInterface;
	IRenderer * m_Renderer;
};

}//namespace EE
#endif //#ifndef CDX9IndexBuffer_H