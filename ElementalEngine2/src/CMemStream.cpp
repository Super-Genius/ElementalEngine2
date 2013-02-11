///==========================================================================
/// \file	CMemStream.cpp
/// \brief	Implementation of streaming using Memory
/// \date	4/22/2005
/// \author	Brian Bazyk
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
///==========================================================================

#include "StdAfx.h"
#include "CMemStream.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMemStream::CMemStream( const CMemStream& ar )
{
}

void CMemStream::operator=( const CMemStream& ar )
{
}

CMemStream::CMemStream()
{
	m_pBuffer = NULL;
	m_iMaxBuffLength = 0;
    m_iBufferSize = 0;
	m_iReadIndex = 0;
	m_iWriteIndex = 0;
	m_Mode = 0;
	m_bUseInternal = false;
	m_pInternalBuffer = NULL;
}

CMemStream::~CMemStream()
{
	Close();
}

// Seek to a position in the file if possible
DWORD CMemStream::SeekTo(DWORD offset, int aSeek)
{
	size_t newOffset;
	
	if (m_Mode & STREAM_MODE_READ)
	{
		newOffset = m_iReadIndex;
	}
	else 
	{
		newOffset = m_iWriteIndex;
	}

	switch (aSeek)
	{
		case SEEK_SET:
			newOffset = (size_t)offset;
			break;
		case SEEK_CUR:
			newOffset = newOffset + (size_t)offset;
			break;
		case SEEK_END:
            newOffset = m_iBufferSize + (size_t)offset;
			break;
	}
	
	// this is a bad assert, this is UNSIGNED int
	assert(newOffset >= 0);

	// seek_end will put the offset at the very end of the buffer + 1, so this should be <=	
	assert(newOffset <= m_iMaxBuffLength); 

	// stream was truncated, make sure we aren't past the 
	// written buffer size.
	if (m_Mode & STREAM_MODE_TRUNCATE)
	{
		assert(newOffset < m_iBufferSize);
	}
	m_iReadIndex = m_iWriteIndex = newOffset;
	return 0;
}

void CMemStream::Init( size_t len, const void *buffer, DWORD mode )
{
	m_iReadIndex = 0;
	m_iWriteIndex = 0;
	m_Mode = mode;

	if (buffer != NULL)
	{
		m_pBuffer = static_cast<const unsigned char*>(buffer);
		m_iMaxBuffLength = len;
		m_iBufferSize = len;
		m_bUseInternal = false;
	}
	else
	{
		m_pInternalBuffer = new char[INITBLOCKSIZE];
		m_iMaxBuffLength = INITBLOCKSIZE;
		m_iBufferSize = 0;
		m_bUseInternal = true;
	}
}

size_t CMemStream::Read( void* buffer, size_t len )
{
	if (!m_bUseInternal) // reading from external buffer
	{
		assert( m_Mode & STREAM_MODE_READ);
		assert( buffer != NULL );
		assert( m_pBuffer != NULL );

		// this should ensure the buffer won't be overread
		if ((m_iBufferSize - m_iReadIndex) < len)
		{
			len = m_iBufferSize - m_iReadIndex;
		}
		if (len > 0)
		{
			memcpy( buffer, &m_pBuffer[m_iReadIndex], len );
			m_iReadIndex += len;
			return len;
		}
		else
		{
			return 0;
		}
	}
	else // reading from internal buffer
	{
		assert( m_Mode & STREAM_MODE_READ);
		assert( buffer != NULL );
		assert(m_pInternalBuffer);

		if ((m_iBufferSize - m_iReadIndex) < len)
		{
			len = m_iBufferSize - m_iReadIndex;
		}
		if (len > 0)
		{
			memcpy( buffer, &m_pInternalBuffer[m_iReadIndex], len );
			m_iReadIndex += len;
			return len;
		}		
		else
		{
			return 0;
		}
	}
}

size_t CMemStream::Write( const void* buffer, size_t len )
{
	if (!m_bUseInternal) // using external buffer
	{
		assert( m_Mode & STREAM_MODE_WRITE);
		assert( buffer != NULL );
		assert( m_pBuffer != NULL );

		if (len == 0)
			return 0;

		assert( (m_iWriteIndex + len) <= m_iMaxBuffLength );
		if ((m_iWriteIndex + len) <= m_iMaxBuffLength)
		{
			memcpy( (void *)&m_pBuffer[m_iWriteIndex], buffer, len );
			m_iWriteIndex += len;
			if (m_iWriteIndex > m_iBufferSize){
				m_iBufferSize = m_iWriteIndex;
			}
			return len;
		}
		else
		{
			return 0;
		}
	}
	else // using internal buffer
	{
		assert( m_Mode & STREAM_MODE_WRITE);
		assert( buffer != NULL );
		assert( m_pInternalBuffer != NULL );

		if (len == 0)
			return 0;

		if ((m_iWriteIndex + len) <= m_iMaxBuffLength) // data fits
		{
			memcpy( (void *)&m_pInternalBuffer[m_iWriteIndex], buffer, len );
			m_iWriteIndex += len;
			if (m_iWriteIndex > m_iBufferSize){
				m_iBufferSize = m_iWriteIndex;
			}
			return len;
		}
		else // resize internal buffer
		{
			if ((m_iWriteIndex + len) > m_iMaxBuffLength)
			{
				while ((m_iWriteIndex + len) > m_iMaxBuffLength)
				{
					m_iMaxBuffLength *= 2;
				}
				// allocate new buffer
				char *tempBuf;
				tempBuf = new char [m_iMaxBuffLength*2];
				memcpy((void*) tempBuf, m_pInternalBuffer, m_iWriteIndex);
				// delete old buffer
				delete []m_pInternalBuffer;
				// redirect pointer
				m_pInternalBuffer = tempBuf;
			}
			memcpy( (void *)&m_pInternalBuffer[m_iWriteIndex], buffer, len );
			m_iWriteIndex += len;
			if (m_iWriteIndex > m_iBufferSize){
				m_iBufferSize = m_iWriteIndex;
			}
			return len;
		}
	}
}

void CMemStream::Close()
{
	m_pBuffer = NULL;
	m_iMaxBuffLength = 0;
    m_iBufferSize = 0;
	m_iReadIndex = 0;
	m_iWriteIndex = 0;
	if (m_bUseInternal)
	{
		delete []m_pInternalBuffer;
		m_pInternalBuffer = NULL;
	}
}

// return if stream is reading or writing
bool CMemStream::IsReading()
{
	return ((m_Mode & STREAM_MODE_READ) == STREAM_MODE_READ);
}

bool CMemStream::IsWriting()
{
	return ((m_Mode & STREAM_MODE_WRITE) == STREAM_MODE_WRITE);
}

bool CMemStream::IsEndOfFile()
{
	if (IsReading())
	{
		if (m_iReadIndex >= m_iBufferSize)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (m_iWriteIndex >= m_iBufferSize)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

long CMemStream::Tell()
{
	if (IsReading())
	{
		return (long)m_iReadIndex;
	}
	else
	{
		return (long)m_iWriteIndex;
	}
}
