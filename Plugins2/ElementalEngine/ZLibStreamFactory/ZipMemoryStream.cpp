///============================================================================
/// \file		CZipMemoryStream.cpp
/// \brief		Implementation of CZipMemoryStream. This stream zips memory content 
/// \date		10/18/2007
/// \author		Andrey Ivanov
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
#include "StdAfx.h"
#include "ZipMemoryStream.h"

namespace ElementalEngine
{

/// \brief	Constructor
///	\param	mode - stream interaction mode	
CZipMemoryStream::CZipMemoryStream(DWORD mode)
	: m_dwMode(mode)
	, m_iReadLength(0)
	, m_iOffset(0)
	, m_pZippedData(NULL)
	, m_iZippedDataLength(0)
{
}

CZipMemoryStream::~CZipMemoryStream()
{
	Close();
}

DWORD CZipMemoryStream::SeekTo(DWORD offset, int aSeek /*= SEEK_SET*/)
{
	DWORD dwNewOffset = 0;
	// determine new absolute offset in the stream
	switch (aSeek)
	{
		case SEEK_SET:
			dwNewOffset = offset;
			break;
		case SEEK_CUR:
			dwNewOffset = (DWORD)m_iOffset + offset;
			break;
		case SEEK_END:
			dwNewOffset = (DWORD)m_iReadLength + offset;
			break;
		default:
			assert(!"Unsupported direction");
	}

	// check offset against stream size
	if (dwNewOffset > m_iReadLength)
	{
		dwNewOffset = (DWORD)m_iReadLength;
	}

	m_iOffset = dwNewOffset;

	return dwNewOffset;
}

bool CZipMemoryStream::IsReading()
{
	return ((m_dwMode & STREAM_MODE_READ) == STREAM_MODE_READ);
}

bool CZipMemoryStream::IsWriting()
{
	return ((m_dwMode & STREAM_MODE_WRITE) == STREAM_MODE_WRITE);
}

size_t CZipMemoryStream::Read(void* buffer, size_t len)
{
	assert(IsReading());
	// calculate new offset in the buffer
	size_t iNewOffset = m_iOffset + len;
	if (iNewOffset > m_iReadLength)
	{
		// this read attempts to get data behind of stream end 
		// so, return the remained part only
		len = m_iReadLength - m_iOffset;
		iNewOffset = m_iReadLength;
	}

	if (0 == len)
	{
		return 0;
	}

	// allocate buffer for unpacked data
	LPBYTE pUnpackedData = new BYTE[iNewOffset];
	uLongf iLength = (uLongf)iNewOffset;
	// then uncompress data from buffer with zipped data
	uncompress(pUnpackedData, &iLength, m_pZippedData, (uLongf)m_iZippedDataLength);
	assert(iNewOffset == iLength);
	memcpy(buffer, pUnpackedData + m_iOffset, len);

	delete [] pUnpackedData;

	m_iOffset = iNewOffset;

	return len;
}

size_t CZipMemoryStream::Write(const void* buffer, size_t len)
{
	assert(IsWriting());
	// calculate result data length
	size_t iNewOffset = m_iOffset + len;

	// calculate unpacked data length
	size_t iUnpackedLength = max(m_iReadLength, iNewOffset);
	// allocate buffer for unpacked data
	LPBYTE pUnpackedData = new BYTE[iUnpackedLength];
	// then uncompress data from buffer with zipped data
	uLongf iLength = (uLongf)iUnpackedLength;
	uncompress(pUnpackedData, &iLength, m_pZippedData, (uLongf)m_iZippedDataLength);

	// copying changed parts from internal buffer
	memcpy(pUnpackedData + m_iOffset, buffer, len);

	// then pack data
	SetData(pUnpackedData, iUnpackedLength);
	
	delete []pUnpackedData;

	m_iReadLength = iUnpackedLength;
	m_iOffset = iNewOffset;


	return len;
}

void CZipMemoryStream::Close()
{
	delete []m_pZippedData;
	m_pZippedData = NULL;
}

bool CZipMemoryStream::IsEndOfFile()
{
	return m_iOffset == m_iReadLength;
}

long CZipMemoryStream::Tell()
{
	return (long)m_iOffset;
}

void CZipMemoryStream::SetData(LPCBYTE pUnpackedData, size_t iUnpackedDataLength)
{
	// then trying to pack data
	uLongf iBufferLength = (uLongf)compressBound((uLongf)iUnpackedDataLength);
	while (true)
	{
		LPBYTE pPackedData = new BYTE[iBufferLength];
		uLongf iLength = (uLongf)iBufferLength;
		if (Z_BUF_ERROR != compress2(pPackedData, &iLength, pUnpackedData, (uLongf)iUnpackedDataLength, Z_BEST_COMPRESSION))
		{
			// TODO: improve these memory interactions
			// allocate buffer for compressed data with exact buffer size
			delete []m_pZippedData;
			m_pZippedData = new BYTE[iLength];
			memcpy(m_pZippedData, pPackedData, iLength);
			m_iZippedDataLength = iLength;
			delete []pPackedData;
			break;
		}
		delete []pPackedData;
		iBufferLength *= 2;
	}
}


} //namespace ElementalEngine
