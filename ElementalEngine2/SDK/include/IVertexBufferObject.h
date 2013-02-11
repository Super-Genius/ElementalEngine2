///============================================================================
/// \file	IVertexBufferObject.h
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

#ifndef IVERTEXBUFFEROBJECT_H
#define IVERTEXBUFFEROBJECT_H

namespace ElementalEngine
{
using namespace std;

struct VertexBufferDescriptor
{
	ElementalEngine::CHANNELDESCRIPTORLIST * outChannelDesc;
	UINT	outnumElements;
	VertexBufferDescriptor()
	{
		outChannelDesc = NULL;
	}
};

#define VBO_LOCK_DISCARD		1
#define VBO_LOCK_NOOVERWRITE	2

/// IVertexBufferObject
/// interface for vertex buffers
class IVertexBufferObject
{
public:
	/// Destructor
	virtual ~IVertexBufferObject() {}

	/// Initializes a vertex buffer
	/// \param channels - vertex channel descriptor
	/// \param numElements - number of elements
	/// \param bdynamic - bool indicating if this buffer is dynamic
	/// \param streammode - unsigned int indicated the type of stream mode (default = 0)
	/// \return - true on success
	virtual bool Initialize( CHANNELDESCRIPTORLIST &channels,  UINT numElements, bool bdynamic, UINT streammode = 0 ) = 0;

	/// Gets the Vertex Buffer Description
	/// \param Desc - vertex buffer description struct to be filled in
	virtual void GetDescription( VertexBufferDescriptor &Desc ) = 0;

	/// Locks the buffer
	/// \param offset - unsigned int specifying the offset
	/// \param size - unsigned int specifying the size
	/// \param pData - pointer to a void pointer of the locked index buffer
	/// \param flags - vertex buffer lock flags (default = 0)
	/// \return - true on success
	virtual bool Lock( UINT offset, UINT size, void** pData, DWORD flags = 0 ) = 0;

	/// Unlocks the buffer
	/// \return - true on success
	virtual bool Unlock( void ) = 0;	
	
	/// Get the stride of the vertex buffer
	/// \return - unsigned int of the stride
	virtual UINT GetStride( void ) = 0;

	/// Get the size (number of elements) of the vertex buffer
	/// \return - unsigned int of the number of elements
	virtual UINT GetSize( void ) = 0;
};

} //namespace ElementalEngine

#endif