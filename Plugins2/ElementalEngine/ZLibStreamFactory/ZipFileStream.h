///============================================================================
/// \file		CZipFileStream.h
/// \brief		Header file for Zip File Streams
/// \date		10/28/2007
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
#ifndef __ZIPFILESTREAM_HPP_
#define __ZIPFILESTREAM_HPP_

#include "IEEStream.h"
#include "physfs.h"

namespace ElementalEngine
{

/// \class	CZipFileStream
/// \brief	Implementation of IEEStream interface for compressed memory block
class CZipFileStream : public IEEStream
{
public:
	/// \brief	Constructor
	CZipFileStream();

	virtual ~CZipFileStream();

	/// \brief	Initialize memory input stream
	/// \param	path - path in physfs
	/// \param mode = STREAM_MODE_READING or STREAM_MODE_WRITING
	/// \return	true if stream is initialized
	bool Init(const char *filename, DWORD mode);

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

	///	\brief	add search path to physfs. This affects streams opened for reading
	///	\param	path - path to file system object. It may refer to folder, zip file
	///				   or other types of archives (grp, hog, mvl, qpak, wad)
	///	\return	true if path was added successfully
	static bool AddSearchPath(const char *path);


	/// checks if a file exists
	/// \param path - path to file
	/// \return bool true if found, else returns false
	static bool CheckFileExist(const char *path);

private:
	/// \brief	Can't use default constructor
	//CZipFileStream();
	/// \brief	Can't use copy constructor
	CZipFileStream(const CZipFileStream& ar);
	/// \brief	Can't use assignment operator
	void operator=(const CZipFileStream& ar);

	///	\breif	report PhysFS error to EE
	void ReportPhysFSError();

private:
	/// flags for if the stream is reading or writing, etc.
	DWORD		m_dwMode;
	///	this value limits size of the stream,
	size_t		m_iReadLength;
	///	buffer offset in unzipped data
	size_t		m_iOffset;

	StdString	m_filename;
	PHYSFS_file *m_file;
};

}; //namespace ElementalEngine
#endif // __ZIPFILESTREAM_HPP_
