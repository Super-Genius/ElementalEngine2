///============================================================================
/// \file	IMeshObject.h
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

#ifndef IMESHOBJECT_H
#define IMESHOBJECT_H

namespace ElementalEngine
{
using namespace std;
/// ChannelDesc - Vertex Stream Descriptor
typedef struct ChannelDesc
{
	DWORD NameHash;
	DWORD ChannelHash;
	int			Stride;
	ChannelDesc()
	{
		NameHash = 0;
		ChannelHash = 0;
		Stride = 0;
	};
	ChannelDesc( IHashString *channel, int stride, IHashString *name=NULL )
	{
		ChannelHash = channel->GetUniqueID();
		if (name)
			NameHash = name->GetUniqueID();
		Stride = stride;
	}
} ChannelDesc;

/// MESHSTREAMDESC - Mesh Stream Descriptor
typedef struct MESHSTREAMDESC
{
	int		Stride;
	int		NumElements;
}StreamDesc;

/// CHANNELDESCRIPTORLIST - list of Vertex Channel Descriptors
typedef vector< ChannelDesc > CHANNELDESCRIPTORLIST;

/// VERTEXDESCRIPTIONCOMPARE - allows different descriptors to be compared
struct VERTEXDESCRIPTIONCOMPARE
{
	__forceinline bool operator()(CHANNELDESCRIPTORLIST LeftObj, CHANNELDESCRIPTORLIST Right) const
	{		
		if( LeftObj.size() != Right.size() )
		{
			return ( LeftObj.size() > Right.size() );
		}else
		{
			size_t size = LeftObj.size();
			int scmp = 0;
			for( size_t i = 0; i < size; i++ )
			{
				if (LeftObj[ i ].ChannelHash != Right[ i ].ChannelHash)
					return (LeftObj[ i ].ChannelHash < Right[ i ].ChannelHash);
			}
		}
		return false;
	}
};

/// IMeshObject
/// Interface for mesh objects (inherits from IObject)
class IMeshObject : public IObject
{
public:
	/// Destructor
	virtual ~IMeshObject() {};

	/// Sets the stream data
	/// \param pIn - void pointer to the stream data
	/// \param numElements - int number of elements
	/// \param flagArray - Channel descriptor list
	/// \return - true on success
	virtual bool SetStreamData( void *pIn, int numElements,  CHANNELDESCRIPTORLIST &flagArray ) = 0;

	/// Gets the stream data
	/// \param pOut - void pointer to the stream data (to be filed out)
	/// \param flagArray - Channel descriptor list (to be filled out)
	/// \param numElements - int number of elements (default = 0)
	/// \return - a mesh stream descriptor (with size and stride info)
	virtual MESHSTREAMDESC GetStreamData( void * pOut, CHANNELDESCRIPTORLIST &flagArray, int numElements = 0 ) = 0;

	/// Gets the channel descriptor
	/// \param ChannelDescsOut - Channel descriptor list (to be filled out)
	virtual void GetMeshDescription( CHANNELDESCRIPTORLIST &ChannelDescsOut ) = 0;
};

} //namespace ElementalEngine

#endif