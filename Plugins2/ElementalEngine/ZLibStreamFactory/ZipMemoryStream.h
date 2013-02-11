///============================================================================
/// \file		CZipMemoryStream.h
/// \brief		Header file for CZipMemoryStream
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
#ifndef __ZIPMEMORYSTREAM_HPP_
#define __ZIPMEMORYSTREAM_HPP_

#include "IEEStream.h"

#ifndef _LPCBYTE_DEFINED
#define _LPCBYTE_DEFINED
typedef const BYTE *LPCBYTE;
#endif

namespace ElementalEngine
{

/// \class	CZipMemoryStream
/// \brief	Implementation of IEEStream interface for compressed memory block
class CZipMemoryStream : public IEEStream
{
public:
	/// \brief	Constructor
	///	\param	mode - stream interaction mode	
	CZipMemoryStream(DWORD mode);

	virtual ~CZipMemoryStream();

	/// \brief	Initialize memory input stream
	/// \param	len - the size of the buffer
	/// \param	buffer - If not NULL then stream will use given buffer (assumes buffer
	///                   is at least iBufferSize in size).  Does not try to delete given buffer.
	///                   If NULL stream will manage its own internal buffer.
	/// \param mode = STREAM_MODE_READING, STREAM_MODE_WRITING
	void Init(size_t len, const void *buffer, DWORD mode);

	/// Seek to a position in the file if possible
	/// \param offset - offset to seek from
	/// \param aSeek = place to seek from
	virtual DWORD SeekTo(DWORD offset, int aSeek = SEEK_SET);

	/// return if the memory stream is Reading
	/// \return bool indicating in read mode = true, write mode = false
	virtual bool IsReading();

	/// return if the memory stream is Writing
	/// \return bool indicating in write mode = true, read mode = false
	virtual bool IsWriting();

	/// \brief	Read data from the input stream
	/// \param	buffer - The buffer containing the data read from the stream
	/// \param	len - The amount of data to read
	/// \return	The size of the data read from the stream
	virtual size_t Read(void* buffer, size_t len);

	/// \brief	Write to the stream
	/// \param	buffer p The buffer containing the data to write to the stream
	/// \param	len - The amount of data to write to the stream
	virtual size_t Write(const void* buffer, size_t len);

	/// \brief	Close the stream
	virtual void Close();

	/// Checks to see if we're at the end of stream
	/// \return true if eof
	virtual bool IsEndOfFile();

	/// Returns the current offset of the read/write pointer
	/// NOTE: use SeekTo(0, SEEK_END) and this to get filesize
	/// \return current read/write offset
	virtual long Tell();

private:
	/// \brief	Can't use default constructor
	//CZipMemoryStream();
	/// \brief	Can't use copy constructor
	CZipMemoryStream(const CZipMemoryStream& ar);
	/// \brief	Can't use assignment operator
	void operator=(const CZipMemoryStream& ar);

	void SetData(LPCBYTE pUnpackedData, size_t iUnpackedDataLength);

private:
	/// flags for if the stream is reading or writing, etc.
	DWORD		m_dwMode;
	///	this value limits size of the stream,
	size_t		m_iReadLength;
	///	buffer offset in unzipped data
	size_t		m_iOffset;

	/// flag for using internal or external buffer
	/// the internal dynamic buffer
	LPBYTE		m_pZippedData;
	/// The max buffer size declared by the client who supplied the buffer
	size_t		m_iZippedDataLength;
};

}; //namespace ElementalEngine
#endif // __ZIPMEMORYSTREAM_HPP_
