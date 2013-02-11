///==========================================================================
/// \file	CFileInputStream.h
/// \brief	Implementation of IInputStream interface for reading from a file
/// \date	3/24/2005
/// \author	Kyle Swaim
///==========================================================================

#ifndef	CFILEINPUTSTREAM_H
#define	CFILEINPUTSTREAM_H

#include "IInputStream.h"

///==========================================================================
/// \class	CFileInputStream
/// \brief	Implementation of IInputStream interface for writing to a file
///==========================================================================
class CFileInputStream : public IInputStream
{
private:
	/// \brief	Can't use default constructor
	CFileInputStream();
	/// \brief	Can't use copy constructor
	CFileInputStream( const CFileInputStream& ar );
	/// \brief	Can't use assignment operator
	void operator=( const CFileInputStream& ar );

	/// \brief File ptr for the input file.
	FILE *m_fpFile;

public:
	/// \brief	Constructor
	/// \param	fileName - The name of the file to open
	CFileInputStream( TCHAR *fileName );

	/// \brief Destructor
	~CFileInputStream();

	/// \brief	Read data from the input stream
	/// \param	buffer - The buffer containing the data read from the stream
	/// \param	len - The amount of data to read
	/// \return	The size of the data read from the stream
	virtual size_t Read( void* buffer, int len );

	/// \brief Check to see if stream was successfully opened
	virtual bool IsOpen() const;

	/// \brief	Close the stream
	virtual void Close();
};

#endif	// CFILEINPUTSTREAM_H