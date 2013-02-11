///============================================================================
/// \file	CVertexFormat.cpp
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

#include "stdafx.h"
#include "CVertexFormat.h"

REGISTER_COMPONENT(CVertexFormat);


CVertexFormat::CVertexFormat(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE( CShaderManager, CVertexFormat, IVertexFormat, parentName, name )
{
	DeInit();
}


/// IObject  functions
/// \brief	Create the a instance of IObject
/// \return	IComponent pointer
IComponent * CVertexFormat::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = (IObject*)new CVertexFormat(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

/// \brief	The Initialization for the Object
/// \param	IManager* pManager: Parent Manager
void  CVertexFormat::Init()
{
}

/// \brief	The Update Function for the Object
/// \return	True on sucess, False on faulure
bool  CVertexFormat::Update()
{
	return true;
}

/// \brief	The DeInit Function for the Object
void CVertexFormat::DeInit()
{

}

/// \brief either reads or writes the entity information to a file
void  CVertexFormat::Serialize(IArchive &ar)
{
	if( ar.IsReading() )
	{
		int numChannels;
		ar.Read( numChannels, _T("numChannels") );
		m_ChannelDescriptorList.resize( numChannels );
		for( int i = 0; i < numChannels; i++ )
		{
			StdString szName, szChannel;
			ChannelDesc channelDesc;
			ar.Read( szName, _T("Name") );
			ar.Read( szChannel, _T("Channel") );
			ar.Read( channelDesc.Stride, _T("Stride") );
			channelDesc.NameHash = CHashString( szName ).GetUniqueID();
			channelDesc.ChannelHash = CHashString( szName ).GetUniqueID();
			m_ChannelDescriptorList[ i ] = channelDesc;
		}
	}
}

void CVertexFormat::SetChannelDescriptor( CHANNELDESCRIPTORLIST &channelList )
{
	m_ChannelDescriptorList = channelList;
}

CHANNELDESCRIPTORLIST & CVertexFormat::GetChannelDescriptor()
{ 
	return m_ChannelDescriptorList;
}

DWORD CVertexFormat::GetOffsetByChannel( IHashString * channelName )
{
	if( channelName != NULL )
	{
		DWORD offset = 0;
		int vertexFormatSize = (int)m_ChannelDescriptorList.size();
		for( int i = 0; i < vertexFormatSize; i++ )
		{
			ChannelDesc &desc = m_ChannelDescriptorList[i];
			if( desc.ChannelHash == channelName->GetUniqueID() )
			{
				break;
			}
			offset += desc.Stride;
		}
		return offset;
	}
	return 0;
}

DWORD CVertexFormat::GetTotalStride()
{
	DWORD offset = 0;
	int vertexFormatSize = (int)m_ChannelDescriptorList.size();
	for( int i = 0; i < vertexFormatSize; i++ )
	{
		ChannelDesc &desc = m_ChannelDescriptorList[i];
		offset += desc.Stride;
	}
	return offset;
}

DWORD CVertexFormat::GetOffsetByName( IHashString * channelName )
{
	if( channelName != NULL )
	{
		DWORD offset = 0;
		int vertexFormatSize = (int)m_ChannelDescriptorList.size();
		for( int i = 0; i < vertexFormatSize; i++ )
		{
			ChannelDesc &desc = m_ChannelDescriptorList[i];
			if( desc.NameHash == channelName->GetUniqueID() )
			{
				break;
			}
			offset += desc.Stride;
		}
		return offset;
	}
	return 0;
}
