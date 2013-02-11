///==========================================================================
/// \file	CMemStream.h
/// \brief	Streaming using Memory
/// \date	05/30/2005
/// \author	Kenneth Hurley
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

#ifndef	__CMEMSTREAM_H_
#define	__CMEMSTREAM_H_

#include "BaseMessages.h"

#define INITBLOCKSIZE 256 /// initial memory allocation

namespace ElementalEngine 
{
class  CMemStream : public IEEStream
{
private:
	/// \brief	Can't use copy constructor
	CMemStream( const CMemStream& ar );
	/// \brief	Can't use assignment operator
	void operator=( const CMemStream& ar );

public:
	CMemStream();
	~CMemStream();

	/// \brief	Initialize memory input stream
	/// \param	len - the size of the buffer
	/// \param	buffer - If not NULL then stream will use given buffer (assumes buffer
	///                   is at least iBufferSize in size).  Does not try to delete given buffer.
	///                   If NULL stream will manage its own internal buffer.
	/// \param mode = STREAM_MODE_READING, STREAM_MODE_WRITING
	void Init( size_t len, const void *buffer, DWORD mode );

	/// Seek to a position in the file if possible
	/// \param offset - offset to seek from
	/// \param aSeek = place to seek from
	DWORD SeekTo(DWORD offset, int aSeek = SEEK_SET);

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
	virtual size_t Read( void* buffer, size_t len );

	/// \brief	Write to the stream
	/// \param	buffer p The buffer containing the data to write to the stream
	/// \param	len - The amount of data to write to the stream
	virtual size_t Write( const void* buffer, size_t len );

	/// \brief	Close the stream
	virtual void Close();

	/// Checks to see if we're at the end of stream
	/// \return true if eof
	virtual bool IsEndOfFile();

	/// Returns the current offset of the read/write pointer
	/// NOTE: use SeekTo(0, SEEK_END) and this to get filesize
	/// NOTE: if reading is enabled, returns read offset; otherwise, returns write offset
	/// \return current read/write offset
	virtual long Tell();

private:
	const unsigned char *m_pBuffer;
    ///The max buffer size declared by the client who supplied the buffer
	size_t m_iMaxBuffLength;
    ///The maximum amount of buffer that has thus far been consumed
    size_t m_iBufferSize;
	size_t m_iReadIndex;
	size_t m_iWriteIndex;
	
	/// flags for if the stream is reading or writing, etc.
	DWORD m_Mode;

	/// flag for using internal or external buffer
	bool m_bUseInternal;

	/// the internal dynamic buffer
	char *m_pInternalBuffer;
};

}	//namespace ElementalEngine 

#endif //__CMemStream_H_