///==========================================================================
/// \file	CArchive.h
/// \brief	Archiving using streams
/// \date	4/22/2005
/// \author	Brian Bazyk
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

#ifndef	__CARCHIVE_H_
#define	__CARCHIVE_H_

#include "IArchive.hpp"
namespace ElementalEngine
{
class Vec3;
class Matrix3x3;
class Matrix4x4;
class Quaternion;
class IEEStream;

class CArchive : public IArchive
{
private:
	/// \brief	Can't use copy constructor
	CArchive( const CArchive& ar );
	/// \brief	Can't use assignment operator
	void operator=( const CArchive& ar );

public:
	/// \brief	default constructor
	CArchive();
	/// \brief	Destructor
	virtual ~CArchive();

	/// \brief	Initialize the archive
	/// \param	pStream - The stream to input
	virtual bool Init( IEEStream *pStream );

	/// \brief	Test if the IArchive is being read
	/// \return	True if the IArchive is being read, false if not
	virtual bool IsReading() const;

	/// \brief	Test if the IArchive is being written to
	/// \return	True if the IArchive is being written, false if not
	virtual bool IsWriting() const;

	/// Seek to a position in the file if possible
	/// \param offset - offset to seek from
	/// \param aSeek = place to seek from
	virtual DWORD SeekTo(DWORD offset, int aSeek = SEEK_SET);

	/// Close the archive, since we allocated it
	virtual void Close()
	{
		delete this;
	}

	/// \brief Starts a New Class Object
	/// \param wszName - name of the class
	/// \return True if Success, False otherwise
	virtual bool StartClass(const StdString &wszName );
	/// \brienf Ends a New Class Object
	/// \return True if Success, False otherwise
	virtual bool EndClass( void );

	/// \brief Set the archive to write or read
	///	\param bWrite - if true, archive is writing, else reading
	virtual void SetIsWriting( bool bWrite );

	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual UINT Read( float &value, const TCHAR *tag );
	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual UINT Read( double &value, const TCHAR *tag );
	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual UINT Read( int &value, const TCHAR *tag );
	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual UINT Read( short &value, const TCHAR *tag );
	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual UINT Read( char &value, const TCHAR *tag );
	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual UINT Read( unsigned &value, const TCHAR *tag );
	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual UINT Read( bool &value, const TCHAR *tag );
	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual UINT Read( StdString &value, const TCHAR *tag );

	/// \brief	Read a Vec3 from the archive
	/// \param	vec - the vector to store the values read from the archive
	/// \param	tag - the tag for the data
	virtual UINT Read( Vec3 &vec, const TCHAR *tag );
	/// \brief	Read a Vec4 from the archive
	/// \param	vec - the vector to store the values read from the archive
	/// \param	tag - the tag for the data
	virtual UINT Read( Vec4 &vec, const TCHAR *tag );
	/// \brief	Read a Matrix3x3 from the archive
	/// \param	mat - THe matrix 3x3 to store the values read from the archive
	/// \param	tag - the tag for the data
	virtual UINT Read( Matrix3x3 &mat, const TCHAR *tag );
	/// \brief	Read a Matrix4x4 from the archive
	/// \param	mat - THe matrix 4x4 to store the values read from the archive
	/// \param	tag - the tag for the data
	virtual UINT Read( Matrix4x4 &mat, const TCHAR *tag );
	/// \brief	Read a quaternion from the archive
	/// \param	quat - The quaternion to store the values read from the archive
	/// \param	tag - the tag for the data
	virtual UINT Read( Quaternion &quat, const TCHAR *tag );
	/// \brief	Read a EulerAngle from the archive
	/// \param	euler - The EulerAngle to store the values read from the archive
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( EulerAngle &euler, const TCHAR *tag);
	/// \brief	Read a binary data to the archive
	/// \param	buffer - pointer to buffer with binary data
	/// \param	size - size of data to read
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( void *buffer, size_t size, const TCHAR *tag );
	
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( float value, const TCHAR *tag );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( double value, const TCHAR *tag );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( int value, const TCHAR *tag );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( short value, const TCHAR *tag );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( char value, const TCHAR *tag );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( unsigned value, const TCHAR *tag );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( bool value, const TCHAR *tag );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( const StdString & value, const TCHAR *tag );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( const TCHAR * value, const TCHAR *tag );
	/// \brief	Write a Vec3 to an archive.
	///			Writes three successive float attributes to the current element
	///			representing a vector 3.
	/// \param	vec - The vector to write
	/// \param	tag - the tag for the data
	virtual void Write( Vec3 &vec, const TCHAR *tag );
	/// \brief	Write a Vec4 to an archive.
	///			Writes three successive float attributes to the current element
	///			representing a vector 3.
	/// \param	vec - The vector to write
	/// \param	tag - the tag for the data
	virtual void Write( Vec4 &vec, const TCHAR *tag );
	/// \brief	Write a 3x3 matrix to the archive
	/// \param	mat - The matrix to write
	/// \param	tag - The tag for the data
	virtual void Write( Matrix3x3 &mat, const TCHAR *tag );
	/// \brief	Write a 4x4 matrix to the archive
	/// \param	mat - The matrix to write
	/// \param	tag - The tag for the data
	virtual void Write( Matrix4x4 &mat, const TCHAR *tag );
	/// \brief	Write a quaternion to the archive
	/// \param	quat - The quaternion to write
	/// \param	tag - The tag for the data
	virtual void Write( Quaternion &quat, const TCHAR *tag );
	/// \brief	Write a EulerAngle to the archive
	/// \param	quat - The EulerAngle to write to the archive
	/// \param	tag - the tag for the data
	virtual void Write( EulerAngle &euler, const TCHAR *tag );
	/// \brief	Write a binary data to the archive
	/// \param	buffer - pointer to buffer with binary data
	/// \param	size - size of data in the buffer
	/// \param	tag - the tag for the data
	virtual void Write( const void *buffer, size_t size, const TCHAR *tag );
	/// \brief  Check to see if we've reached the end of the stream
	/// \return true if eof
	virtual bool IsEndOfFile();

private:
	bool m_bIsReading;
	bool m_bIsWriting;
	IEEStream* m_pStream;

};

}	// namespace ElementalEngine
#endif //__CARCHIVE_H_