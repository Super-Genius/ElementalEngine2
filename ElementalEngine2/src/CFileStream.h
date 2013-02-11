///==========================================================================
/// \file	CFileStream.h
/// \brief	Implementation of IEEStream interface for reading/writing from a file
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

#ifndef	CFILESTREAM_H
#define	CFILESTREAM_H

#include "IEEStream.h"

namespace ElementalEngine 
{

///==========================================================================
/// \class	CFileStream
/// \brief	Implementation of IEEStream interface for reading/writing to a file
///==========================================================================
class CFileStream : public IEEStream
{
private:
	/// \brief	Can't use default constructor
	CFileStream();
	/// \brief	Can't use copy constructor
	CFileStream( const CFileStream& ar );
	/// \brief	Can't use assignment operator
	void operator=( const CFileStream& ar );
	
	/// \brief File ptr for the output file.
	FILE *m_fpFile;
	int m_iFlags;
	bool m_IsReading;
	bool m_IsWriting;
	char* m_Filename;

public:
	// room for flags for compression, encryption, etc.
	static const int kFileAppend = (1<<0);

	/// \brief	Constructor
	/// \param	fileName - The name of the file to open
	CFileStream( TCHAR *fileName, int mode, int iFlags=0 );

	/// \brief Destructor
	~CFileStream();
	
	/// Seek to a position in the file if possible
	/// \param offset - offset to seek from
	/// \param aSeek = place to seek from
	DWORD SeekTo(DWORD offset, int aSeek = SEEK_SET);

	/// \brief	Read data from the input stream
	/// \param	buffer - The buffer containing the data read from the stream
	/// \param	len - The amount of data to read
	/// \return	The size of the data read from the stream
	size_t Read( void* buffer, size_t len );

	/// \brief	Write to the stream
	/// \param	buffer p The buffer containing the data to write to the stream
	/// \param	len - The amount of data to write to the stream
	virtual size_t Write( const void* buffer, size_t len );

	virtual bool IsReading();

	virtual bool IsWriting();

	/// \brief Check to see if stream was successfully opened
	virtual bool IsOpen() const;

	/// \brief	Close the stream
	virtual void Close();

	/// \brief	returns the size of the file
	virtual int GetSize();

	/// \brief	returns true if it's the end of the file
	virtual bool IsEndOfFile();

	/// Returns the current offset of the read/write pointer
	/// NOTE: use SeekTo(0, SEEK_END) and this to get filesize
	/// \return current read/write offset
	virtual long Tell();

};

}	//namespace ElementalEngine 

#endif	// CFileStream_H