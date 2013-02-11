///==========================================================================
/// \file	IArchive.h
/// \brief	Archive class for serializing objects and data
/// \date	3/1/2005
/// \author	Kyle Swaim
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

#ifndef	IARCHIVE_H
#define	IARCHIVE_H

namespace ElementalEngine
{

class IObject;
class IEEStream;
class Vec3;
class Vec4;
class Matrix3x3;
class Matrix4x4;
class Quaternion;
class EulerAngle;
class StdString;

///==========================================================================
/// \class	IArchive
/// \brief	Archive class for serializing objects and data
///==========================================================================
class IArchive
{
public:
	/// Default destructor so RTTI and deletes will happen
	virtual ~IArchive() {};
	/// \brief	Initialize the archive
	/// \param	pStream - The stream to input xml
	virtual bool Init( IEEStream *pStream ) = 0;

	/// Seek to a position in the file if possible
	/// \param offset - offset to seek from
	/// \param aSeek = place to seek from
	/// \returns new offset in the file
	virtual DWORD SeekTo(DWORD offset, int aSeek = SEEK_SET) = 0;

	/// Close the archive, since we allocated it
	virtual void Close() = 0;

	/// \brief	Test if the IArchive is being read
	/// \return	True if the IArchive is being read, false if not
	virtual bool IsReading() const = 0;

	/// \brief Starts a New Class Object
	/// \param wszName - name of the class
	/// \return True if Success, False otherwise
	virtual bool StartClass( const StdString &wszName ) = 0;

	/// \brief Set the archive to write or read
	///	\param bWrite - if true, archive is writing, else reading
	virtual void SetIsWriting( bool bWrite ) = 0;

	/// \brienf Ends a New Class Object
	/// \return True if Success, False otherwise
	virtual bool EndClass( void ) = 0;

	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	/// \return number of bytes read
	virtual UINT Read( float &value, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a double from the archive
	/// \param	value - the float read from the archive
	/// \return number of bytes read
	virtual UINT Read( double &value, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read an int from the archive
	/// \param	value - the float read from the archive
	/// \return number of bytes read
	virtual UINT Read( int &value, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a short from the archive
	/// \param	value - the float read from the archive
	/// \return number of bytes read
	virtual UINT Read( short &value, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a char from the archive
	/// \param	value - the float read from the archive
	/// \return number of bytes read
	virtual UINT Read( char &value, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read an unsigned from the archive
	/// \param	value - the float read from the archive
	/// \return number of bytes read
	virtual UINT Read( unsigned &value, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a bool from the archive
	/// \param	value - the float read from the archive
	/// \return number of bytes read
	virtual UINT Read( bool &value, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a StdString from the archive
	/// \param	value - the float read from the archive
	/// \return number of bytes read
	virtual UINT Read( StdString &value, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a Vec3 from the archive
	/// \param	vec - the vector to store the values read from the archive
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( Vec3 &vec, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a Vec4 from the archive
	/// \param	vec - the vector to store the values read from the archive
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( Vec4 &vec, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a Matrix3x3 from the archive
	/// \param	mat - THe matrix 3x3 to store the values read from the archive
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( Matrix3x3 &mat, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a Matrix4x4 from the archive
	/// \param	mat - THe matrix 4x4 to store the values read from the archive
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( Matrix4x4 &mat, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a quaternion from the archive
	/// \param	quat - The quaternion to store the values read from the archive
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( Quaternion &quat, const TCHAR *tag = _T("")) = 0;	
	/// \brief	Read a EulerAngle from the archive
	/// \param	euler - The EulerAngle to store the values read from the archive
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( EulerAngle &euler, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a binary data to the archive
	/// \param	buffer - pointer to buffer with binary data
	/// \param	size - size of data to read
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( void *buffer, size_t size, const TCHAR *tag = _T("") ) = 0;
	/// \brief	Write a float to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( float value, const TCHAR *tag = _T("FloatAttribute")) = 0;
	/// \brief	Write a double to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( double value, const TCHAR *tag = _T("DoubleAttribute")) = 0;
	/// \brief	Write an int to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( int value, const TCHAR *tag = _T("IntAttribute")) = 0;
	/// \brief	Write a short to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( short value, const TCHAR *tag = _T("ShortAttribute")) = 0;
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( char value, const TCHAR *tag = _T("CharAttribute")) = 0;
	/// \brief	Write an unsigned to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( unsigned value, const TCHAR *tag = _T("UnsignedAttribute")) = 0;
	/// \brief	Write a bool to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( bool value, const TCHAR *tag = _T("BoolAttribute")) = 0;
	/// \brief	Write a StdString to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( const StdString & value, const TCHAR *tag = _T("StringAttribute")) = 0;
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( const TCHAR * value, const TCHAR *tag = _T("StringAttribute")) = 0;
	/// \brief	Write a Vec3 to an archive.
	///			Writes three successive float attributes to the current element
	///			representing a vector 3.
	/// \param	vec - The vector to write
	/// \param	tag - the tag for the data
	virtual void Write( Vec3 &vec, const TCHAR *tag = _T("Vec3Atribute") ) = 0;
	/// \brief	Write a Vec4 to an archive.
	///			Writes three successive float attributes to the current element
	///			representing a vector 4.
	/// \param	vec - The vector to write
	/// \param	tag - the tag for the data
	virtual void Write( Vec4 &vec, const TCHAR *tag = _T("Vec4Atribute") ) = 0;
	/// \brief	Write a 3x3 matrix to the archive
	/// \param	mat - The matrix to write
	/// \param	tag - The tag for the data
	virtual void Write( Matrix3x3 &mat, const TCHAR *tag = _T("Matrix3x3Attribute")) = 0;
	/// \brief	Write a 4x4 matrix to the archive
	/// \param	mat - The matrix to write
	/// \param	tag - The tag for the data
	virtual void Write( Matrix4x4 &mat, const TCHAR *tag = _T("Matrix4x4Attribute")) = 0;
	/// \brief	Write a quaternion to the archive
	/// \param	quat - The quaternion to write
	/// \param	tag - The tag for the data
	virtual void Write( Quaternion &quat, const TCHAR *tag = _T("QuaternionAttribute")) = 0;
	/// \brief	Write a EulerAngle to the archive
	/// \param	euler - The EulerAngle to write to the archive
	/// \param	tag - the tag for the data
	virtual void Write( EulerAngle &euler, const TCHAR *tag = _T("EulerAttribute")) = 0;
	/// \brief	Write a binary data to the archive
	/// \param	buffer - pointer to buffer with binary data
	/// \param	size - size of data in the buffer
	/// \param	tag - the tag for the data
	virtual void Write( const void *buffer, size_t size, const TCHAR *tag = _T("BinaryAttribute") ) = 0;
	/// \brief  Check to see if we've reached the end of the stream
	/// \return true if eof
	virtual bool IsEndOfFile() = 0;
};
}	// namespace ElementalEngine

#endif	// IARCHIVE_HPP
