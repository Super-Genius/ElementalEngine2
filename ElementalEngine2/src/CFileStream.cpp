///==========================================================================
/// \file	CFileStream.cpp
/// \brief	Implementation of IOutputStream interface for reading/writing to a file
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

#include "StdAfx.h"

#include "CFileStream.h"
#include <sys\stat.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// read/write mode table
TCHAR* modes[] = {
	NULL, 
	_T("r"),
	_T("w"),
	_T("r+"),
	NULL,
	NULL,
	_T("w+"),
	_T("w+"),
	NULL,
	_T("rb"),
	_T("wb"),
	_T("w+b"),
	_T("w+b"),
	NULL,
	_T("w+b"),
	_T("w+b"),
};

/// \brief	Can't use default constructor
CFileStream::CFileStream()
{
	m_IsReading = true;
	m_Filename = NULL;
}

/// \brief	Can't use copy constructor
CFileStream::CFileStream( const CFileStream& ar )
{
	m_IsReading = true;
}

CFileStream::~CFileStream( void )
{
	Close();
}

/// \brief	Can't use assignment operator
void CFileStream::operator=( const CFileStream& ar )
{
}

/// \brief	Constructor
/// \param	fileName - The name of the file to open
CFileStream::CFileStream( TCHAR *fileName, int mode, int iFlags )
{
	m_fpFile = NULL;
	m_Filename = NULL;

	if (fileName == NULL || *fileName == '\0')
		return;

	m_iFlags = iFlags;	

//	mode = mode & 0xf; 

	// make sure our mode is valid
	if (modes[mode] != NULL)
	{
		// open file with appropriate mode
		StdString szPath(fileName);
		szPath.MakeSafeFileName();
#ifdef XBOX
		szPath = SetPathDrive( szPath, EngineGetToolBox()->GetDrive() );
#endif

		m_fpFile = fopen( szPath, modes[mode] );
		if( m_fpFile == NULL )
		{
			TCHAR *error = strerror(errno);
			EngineGetToolBox()->Log(LOGERROR, _T("%s(%d): file load error: %s\n"), __FILE__, __LINE__, error);
			return;
		}
	}
	else 
	{
		// log error
		return;
	}

	m_IsReading = false;
	m_IsWriting = false;

	if (mode & STREAM_MODE_READ)
	{
		m_IsReading = true;
	}
	if (mode & STREAM_MODE_WRITE)
	{
		m_IsWriting = true;
	}
	m_Filename = new char[strlen(fileName) + 1];
	strcpy(m_Filename, fileName);
}

// Seek to a position in the file if possible
DWORD CFileStream::SeekTo(DWORD offset, int aSeek)
{
	return fseek(m_fpFile, offset, aSeek);
}

/// \brief	Read data from the input stream
/// \param	buffer - The buffer containing the data read from the stream
/// \param	len - The amount of data to read
/// \return	The size of the data read from the stream
size_t CFileStream::Read( void* buffer, size_t len )
{
	assert(m_IsReading);
	if (m_fpFile)
		return fread( buffer, 1, len, m_fpFile);
	else
		return 0;
}

/// \brief	Write to the stream
/// \param	buffer p The buffer containing the data to write to the stream
/// \param	len - The amount of data to write to the stream
size_t CFileStream::Write( const void* buffer, size_t len )
{
	assert(m_IsWriting);
	if (m_fpFile)
		return fwrite( buffer, 1, len, m_fpFile );
	else
		return 0;
}

bool CFileStream::IsOpen() const
{
	return (m_fpFile != NULL);
}

/// \brief	Close the stream
void CFileStream::Close()
{
	if (m_fpFile)
	{
		fclose( m_fpFile );
		m_fpFile = NULL;
	}

	if( m_Filename != NULL )
	{
		delete [] m_Filename;
		m_Filename = NULL;
	}
}

bool CFileStream::IsReading()
{
	return m_IsReading;
}

bool CFileStream::IsWriting()
{
	return m_IsWriting;
}

int CFileStream::GetSize()
{
	if (m_Filename == NULL)
	{
		return 0;
	}	
	struct stat fileStat; 
	int err = stat( m_Filename, &fileStat ); 
	if (0 != err) return 0; 
	return fileStat.st_size;
}

bool CFileStream::IsEndOfFile()
{
	// if feof returns false (0) we are not yet at the end of file
	// in other words, if it returns false, we return false
	return (0==feof(m_fpFile)?false:true);
}

long CFileStream::Tell()
{
	return ftell(m_fpFile);
}
