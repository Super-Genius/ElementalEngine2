///============================================================================
/// \file	IVertexFormat
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

#ifndef IVERTEXFORMAT_H
#define IVERTEXFORMAT_H

namespace ElementalEngine
{

/// Vertex Format Interface
class IVertexFormat : public IObject
{
public:
	/// Destructor
	virtual ~IVertexFormat(){};
	
	/// Set the descriptor of this format
	/// \param channelList - reference to the channel descriptor list to set
	virtual void SetChannelDescriptor( CHANNELDESCRIPTORLIST &channelList ) = 0;

	/// Gets the channel descriptor directl
	/// \return - channel descriptor
	virtual CHANNELDESCRIPTORLIST & GetChannelDescriptor() = 0;

	/// Gets the offset in bytes to the channel from the vertex start
	/// \param channelName - channel name, i.e. MESH_STREAM_VERTEX_TEXTURECOORD1, see definition for ChannelDesc
	/// \return - dword to the number of bytes of offset for a channel
	virtual DWORD GetOffsetByChannel( IHashString * channelName ) = 0;

	/// Gets the offset in bytes to the channel referenced by name, from the vertex start
	/// \param Name - custom name identifier given to channel, i.e. "Diffuse"
	/// \return - dword to the number of bytes of offset
	virtual DWORD GetOffsetByName( IHashString * Name ) = 0;

	/// Gets the total stride of the channel
	/// \return - dword to the number of bytes of total stride
	virtual DWORD GetTotalStride() = 0;
};

} // namespace EE

#endif
