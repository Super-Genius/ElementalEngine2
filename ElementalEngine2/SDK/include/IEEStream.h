///==========================================================================
/// \file	IEEStream.h
/// \brief	Interface for streams
/// \date	5/30/2005
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

#ifndef IEEStream_H
#define	IEEStream_H 

namespace ElementalEngine
{
///==========================================================================
/// \class	IEEStream
/// \brief	Interface for buffered streams
///==========================================================================
class IEEStream
{
public:
	virtual ~IEEStream() {};

	/// Moves read/write pointer to position specified, if possible
	/// \param offset = value to set offset or to offset from
	/// \param aSeek = seek mode (SEEK_SET, SEEK_CUR, SEEK_END, etc)
	/// \return 0 if succesful, non-0 otherwise
	virtual DWORD SeekTo(DWORD offset, int aSeek = SEEK_SET) = 0;

	/// \brief	Write to the stream
	/// \param	buffer p The buffer containing the data to write to the stream
	/// \param	len - The amount of data to write to the stream
	virtual size_t Write( const void* buffer, size_t len ) = 0;

	/// \brief	Read data from the input stream
	/// \param	buffer - The buffer containing the data read from the stream
	/// \param	len - The amount of data to read
	/// \return	The size of the data read from the stream
	virtual size_t Read( void* buffer, size_t len ) = 0;

	/// return if the stream is in reading mode or not
	/// \return bool if it is reading
	virtual bool IsReading() = 0;

	/// return if the stream is in write mode or not
	/// \return bool if it is writing
	virtual bool IsWriting() = 0;

	/// \brief	Close the stream
	virtual void Close() = 0;

	/// Checks to see if we're at the end of stream
	/// \return true if eof
	virtual bool IsEndOfFile() = 0;

	/// Returns the current offset of the read/write pointer
	/// NOTE: use SeekTo(0, SEEK_END) and this to get filesize
	/// \return current read/write offset
	virtual long Tell() = 0;
};

}	//namespace ElementalEngine

#endif	// IEEStream_H