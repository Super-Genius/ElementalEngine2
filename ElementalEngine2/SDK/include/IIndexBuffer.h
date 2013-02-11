///============================================================================
/// \file	IIndexBuffer.h
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

#ifndef IINDEXBUFFER_H
#define IINDEXBUFFER_H

namespace ElementalEngine
{

/// IIndexBuffer
/// interface for indexbuffers
class IIndexBuffer
{
public:	
	/// Destructor
	virtual ~IIndexBuffer() {}

	/// Resizes the index buffer's number of elements
	/// \param numElements - unsigned int of the number of elements
	/// \return - true on success
	virtual bool Resize( UINT numElements ) = 0;

	/// Gets the number of elements of this index buffer
	/// \return - unsigned int of the number of elements
	virtual UINT GetSize( void ) = 0;

	/// Locks the buffer
	/// \param offset - unsigned int specifying the offset
	/// \param size - unsigned int specifying the size
	/// \param pData - pointer to a void pointer of the locked index buffer
	/// \return - true on success
	virtual bool Lock( UINT offset, UINT size, void** pData ) = 0;

	// Unlocks the buffer
	// \return - true on success
	virtual bool Unlock( void ) = 0;
	
};

}
#endif