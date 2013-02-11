///==========================================================================
/// \file	CFileOutputStream.h
/// \brief	Implementation of IOutputStream interface for reading from a file
/// \date	3/24/2005
/// \author	Kyle Swaim
///==========================================================================

#ifndef	CFILEOUTPUTSTREAM_H
#define	CFILEOUTPUTSTREAM_H

#include "IOutputStream.h"

///==========================================================================
/// \class	CFileOutputStream
/// \brief	Implementation of IOutputStream interface for writing to a file
///==========================================================================
class CFileOutputStream : public IOutputStream
{
private:
	/// \brief	Can't use default constructor
	CFileOutputStream();
	/// \brief	Can't use copy constructor
	CFileOutputStream( const CFileOutputStream& ar );
	/// \brief	Can't use assignment operator
	void operator=( const CFileOutputStream& ar );

	/// \brief File ptr for the output file.
	FILE *m_fpFile;
	int m_iFlags;

public:
	// room for flags for compression, encryption, etc.
	static const int kFileAppend = (1<<0);

	/// \brief	Constructor
	/// \param	fileName - The name of the file to open
	CFileOutputStream( TCHAR *fileName, int iFlags=0 );

	/// \brief	Write to the stream
	/// \param	buffer p The buffer containing the data to write to the stream
	/// \param	len - The amount of data to write to the stream
	virtual size_t Write( const void* buffer, size_t len );

	/// \brief Check to see if stream was successfully opened
	virtual bool IsOpen() const;

	/// \brief	Close the stream
	virtual void Close();
};

#endif	// CFILEOUTPUTSTREAM_H