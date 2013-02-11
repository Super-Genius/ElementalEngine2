///==========================================================================
/// \file	CFileInputStream.cpp
/// \brief	Implementation of IInputStream interface for reading from a file
/// \date	3/24/2005
/// \author	Kyle Swaim
///==========================================================================

#include "StdAfx.h"

#include "CFileInputStream.h"

/// \brief	Can't use default constructor
CFileInputStream::CFileInputStream()
{
}

/// \brief	Can't use copy constructor
CFileInputStream::CFileInputStream( const CFileInputStream& ar )
{
}

/// \brief	Can't use assignment operator
void CFileInputStream::operator=( const CFileInputStream& ar )
{
}

/// \brief	Constructor
/// \param	fileName - The name of the file to open
CFileInputStream::CFileInputStream( TCHAR *fileName )
{
	m_fpFile = NULL;

	if (fileName == NULL)
		return;

	m_fpFile = fopen( fileName, _T("rb") );
}

CFileInputStream::~CFileInputStream()
{
	assert( m_fpFile == NULL );
	if (m_fpFile)
		fclose( m_fpFile );
}

/// \brief	Read data from the input stream
/// \param	buffer - The buffer containing the data read from the stream
/// \param	len - The amount of data to read
/// \return	The size of the data read from the stream
size_t CFileInputStream::Read( void* buffer, int len )
{
	if (m_fpFile)
		return fread( buffer, 1, len, m_fpFile);
	else
		return 0;
}

bool CFileInputStream::IsOpen() const
{
	return (m_fpFile == NULL);
}

/// \brief	Close the stream
void CFileInputStream::Close()
{
	if (m_fpFile)
	{
		fclose( m_fpFile );
		m_fpFile = NULL;
	}
}
