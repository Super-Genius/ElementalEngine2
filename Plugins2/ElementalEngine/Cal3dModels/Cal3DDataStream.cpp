///==========================================================================
/// \file	Cal3DDataStream.hpp
/// \brief	Implementation of Cal3DDataStream
/// \date	3/06/2007
/// \author Patrick Ghiocel
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

///==========================================================================

#ifndef CAL_16BIT_INDICES
#define CAL_16BIT_INDICES //make sure we use 16 bit indices!!!
#endif

#include "StdAfx.h"
#include "Cal3DDataStream.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Cal3DDataStream::Cal3DDataStream()
{
	m_pArchive = NULL;
}

Cal3DDataStream::Cal3DDataStream(IArchive * pArchive)
{
	if( pArchive != NULL )
	{
		m_pArchive = pArchive;
	}
	else
	{
		m_pArchive = NULL;
	}
}

Cal3DDataStream::~Cal3DDataStream()
{
    
}

void Cal3DDataStream::CloseArchive()
{
	if( m_pArchive != NULL )
	{
		m_pArchive->Close();
	}
	m_pArchive = NULL;
}

bool Cal3DDataStream::ok() const
{
	if( m_pArchive != NULL )
	{
		if( m_pArchive->IsReading() )
			return true;
	}
	return false;
}

void Cal3DDataStream::setError() const
{

}

bool Cal3DDataStream::readBytes(void* pBuffer, int length)
{
	// Todo: This is slow, we need to fix this.
	return (m_pArchive->Read( pBuffer, length ) == length);
}

bool Cal3DDataStream::readFloat(float& value)
{
	return (m_pArchive->Read(value) == sizeof(float));
}

bool Cal3DDataStream::readInteger(int& value)
{
	return (m_pArchive->Read(value) == sizeof(int));
}

bool Cal3DDataStream::readString(std::string& value)
{
	int length;
	m_pArchive->Read( length );
	if( length < 0 )
	{
		return false;
	}

	StdString tmp;
 	m_pArchive->Read( tmp );
	value = tmp;

	return true;
}

IArchive* Cal3DDataStream::GetArchive(void)
{
	return m_pArchive;
}

