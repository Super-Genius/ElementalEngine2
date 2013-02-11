///==========================================================================
/// \file	CFileOutputStream.cpp
/// \brief	Implementation of IOutputStream interface for writing to a file
/// \date	3/24/2005
/// \author	Kyle Swaim
///==========================================================================

#include "StdAfx.h"

#include "CFileOutputStream.h"

/// \brief	Can't use default constructor
CFileOutputStream::CFileOutputStream()
{
}

/// \brief	Can't use copy constructor
CFileOutputStream::CFileOutputStream( const CFileOutputStream& ar )
{
}

/// \brief	Can't use assignment operator
void CFileOutputStream::operator=( const CFileOutputStream& ar )
{
}

/// \brief	Constructor
/// \param	fileName - The name of the file to open
CFileOutputStream::CFileOutputStream( TCHAR *fileName, int iFlags )
{
	m_fpFile = NULL;

	if (fileName == NULL)
		return;

	m_iFlags = iFlags;

	if (m_iFlags & kFileAppend)
		m_fpFile = fopen( fileName, _T("a+b") );
	else
		m_fpFile = fopen( fileName, _T("wb") );
}

/// \brief	Write to the stream
/// \param	buffer p The buffer containing the data to write to the stream
/// \param	len - The amount of data to write to the stream
size_t CFileOutputStream::Write( const void* buffer, size_t len )
{
	if (m_fpFile)
		return fwrite( buffer, 1, len, m_fpFile );
	else
		return 0;
}

bool CFileOutputStream::IsOpen() const
{
	return (m_fpFile == NULL);
}

/// \brief	Close the stream
void CFileOutputStream::Close()
{
	if (m_fpFile)
	{
		fclose( m_fpFile );
		m_fpFile = NULL;
	}
}
