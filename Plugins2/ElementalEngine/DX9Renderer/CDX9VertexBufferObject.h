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

#ifndef CDX9VertexBufferObject_H
#define CDX9VertexBufferObject_H

#include "IVertexBufferObject.h"

namespace ElementalEngine
{
class CDX9VertexBufferObject : public IVertexBufferObject
{
public:
	CDX9VertexBufferObject();
	~CDX9VertexBufferObject();
	virtual bool Initialize( CHANNELDESCRIPTORLIST &channels, UINT numElements, bool bdynamic, UINT streammode );
	virtual void GetDescription( VertexBufferDescriptor &Desc );
	virtual bool Lock( UINT offset, UINT size, void ** pData, DWORD flags );
	virtual bool Unlock();	

	//DX9 renderer specific
	LPDIRECT3DVERTEXBUFFER9 GetDX9Interface(){
		return m_VBInterface;
	}
	virtual UINT GetStride()
	{
		return m_Stride;
	}

	virtual UINT GetSize()
	{
		return m_NumVertices;
	}

	//DX9 renderer specific:
	LPDIRECT3DVERTEXDECLARATION9 GetVertexDeclaration()
	{
		return m_VertexDeclaration;
	}
	//DX9 renderer specific:
	DWORD GetFVF()
	{
		return m_FVF;
	}
private:
	//Sets the FVF and vdecl
	void SetVertexDescription( CHANNELDESCRIPTORLIST &channels, LPDIRECT3DDEVICE9 pDevice, UINT streamode );
	void DeInit();

	UINT					m_NumVertices;
	UINT					m_Stride;
	ElementalEngine::CHANNELDESCRIPTORLIST	m_VertexDescription;
	LPDIRECT3DVERTEXBUFFER9 m_VBInterface;
	bool					m_bDynamic;
	IRenderer *				m_Renderer;

	//For DX9
	DWORD					m_FVF;//if necessary
	LPDIRECT3DVERTEXDECLARATION9	m_VertexDeclaration;

};
}

#endif