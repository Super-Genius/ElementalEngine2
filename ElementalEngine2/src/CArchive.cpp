///==========================================================================
/// \file	CArchive.cpp
/// \brief	Archiving using Memory
/// \date	4/22/2005
/// \author	Brian The Brainiac Bazyk
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
#include "CArchive.h"
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CArchive::CArchive( )
{
	m_bIsReading = true;
	m_bIsWriting = false;
	m_pStream = NULL;
}

CArchive::~CArchive( )
{
	m_bIsReading = true;
	m_bIsWriting = false;
	if (m_pStream != NULL)
	{
		m_pStream->Close();
		delete m_pStream;
		m_pStream = NULL;
	}
}

bool CArchive::Init( IEEStream *pStream )
{
	assert( pStream != NULL );
	if( pStream == NULL )
		return false;
	// close the old stream
	if (m_pStream != NULL)
	{
		m_pStream->Close();
		delete m_pStream;
	}
	// if data is used as input then we are not writing, we are reading
  	m_bIsReading = pStream->IsReading();
  	m_bIsWriting = pStream->IsWriting();
	m_pStream = pStream;

	return true;
}

// Seek to a position in the file if possible
DWORD CArchive::SeekTo(DWORD offset, int aSeek)
{
	m_pStream->SeekTo(offset, aSeek);
	return (DWORD)m_pStream->Tell();
}

/// \brief	Test if the IArchive is being read
/// \return	True if the IArchive is being read, false if not
bool CArchive::IsReading() const
{
	return (m_bIsReading);
}

/// \brief	Test if the IArchive is being written to
/// \return	True if the IArchive is being written to, false if not
bool CArchive::IsWriting() const
{
	return (m_bIsWriting);
}

/// \brief Starts a New Class Object
/// \param wszName - name of the class
/// \return True if Success, False otherwise
bool CArchive::StartClass(const StdString &wszName ) 
{
	return true;
}

/// \brienf Ends a New Class Object
/// \return True if Success, False otherwise
bool CArchive::EndClass( void )
{
	return true;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
UINT CArchive::Read( float &value, const TCHAR *tag )
{
	assert(m_bIsReading);
	UINT bytesRead;
	bytesRead = (UINT) m_pStream->Read( &value, sizeof(float) );
	return bytesRead;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
UINT CArchive::Read( double &value, const TCHAR *tag )
{
	assert(m_bIsReading);
	UINT bytesRead;
	bytesRead = (UINT) m_pStream->Read( &value, sizeof(double) );
    return bytesRead;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
UINT CArchive::Read( int &value, const TCHAR *tag )
{
	assert(m_bIsReading);
	UINT bytesRead;
	bytesRead = (UINT) m_pStream->Read( &value, sizeof(int) );
	return bytesRead;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
UINT CArchive::Read( short &value, const TCHAR *tag )
{
	assert(m_bIsReading);
	UINT bytesRead;
	bytesRead = (UINT)m_pStream->Read( &value, sizeof(short) );
	return bytesRead;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
UINT CArchive::Read( char &value, const TCHAR *tag )
{
	assert(m_bIsReading);
	UINT bytesRead;
	bytesRead = (UINT)m_pStream->Read( &value, sizeof(char) );
	return bytesRead;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
UINT CArchive::Read( unsigned &value, const TCHAR *tag )
{
	assert(m_bIsReading);
	UINT bytesRead;
	bytesRead = (UINT)m_pStream->Read( &value, sizeof(unsigned) );
	return bytesRead;	
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
UINT CArchive::Read( bool &value, const TCHAR *tag )
{
	assert(m_bIsReading);
	UINT bytesRead;
	bytesRead = (UINT)m_pStream->Read( &value, sizeof(bool) );
	return bytesRead;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
UINT CArchive::Read( StdString &value, const TCHAR *tag )
{
	assert(m_bIsReading);
	TCHAR tempChar;
	//clear the contents of value;
	value.clear();
	UINT bytesRead = 0;
	while (m_pStream->Read( &tempChar, sizeof(TCHAR) ) == sizeof(TCHAR))
	{
		if (tempChar == '\0')
			break;
		value += tempChar;
		bytesRead += sizeof(TCHAR);
	}
	return bytesRead;
}

/// \brief	Read a Vec3 from the archive
/// \param	vec - the vector to store the values read from the arcihve
/// \param	tag - the tag for the data
UINT CArchive::Read( Vec3 &vec, const TCHAR *tag)
{
	assert(m_bIsReading);
	float value[3];
	UINT bytesRead;
	bytesRead = (UINT)m_pStream->Read( value, sizeof(float)*3 );
	vec.x = value[0];
	vec.y = value[1];
	vec.z = value[2];
	return bytesRead;
}

/// \brief	Read a Vec4 from the archive
/// \param	vec - the vector to store the values read from the arcihve
/// \param	tag - the tag for the data
UINT CArchive::Read( Vec4 &vec, const TCHAR *tag)
{
	assert(m_bIsReading);
	float value[4];
	UINT bytesRead;
	bytesRead = (UINT)m_pStream->Read( value, sizeof(float)*4 );
	vec.x = value[0];
	vec.y = value[1];
	vec.z = value[2];
	vec.w = value[3];
	return bytesRead;
}

/// \brief	Read a Matrix3x3 from the archive
/// \param	mat - THe matrix 3x3 to store the values read from the archive
/// \param	tag - the tag for the data
UINT CArchive::Read( Matrix3x3 &mat, const TCHAR *tag)
{
	assert(m_bIsReading);
	UINT bytesRead;
	bytesRead = (UINT)m_pStream->Read( mat.m, sizeof(float)*9 );
	return bytesRead;
}

/// \brief	Read a Matrix4x4 from the archive
/// \param	mat - THe matrix 4x4 to store the values read from the archive
/// \param	tag - the tag for the data
UINT CArchive::Read( Matrix4x4 &mat, const TCHAR *tag)
{
	assert(m_bIsReading);
	UINT bytesRead;
	bytesRead = (UINT)m_pStream->Read( mat.m, sizeof(float)*16 );
	return bytesRead;
}

/// \brief	Read a quaternion from the archive
/// \param	quat - The quaternion to store the vaules read from the archive
/// \param	tag - the tag for the data
UINT CArchive::Read(Quaternion &quat, const TCHAR *tag)
{
	assert(m_bIsReading);
	float value[4];
	UINT bytesRead;
	bytesRead = (UINT)m_pStream->Read( value, sizeof(float)*4 );
	quat.x = value[0];
	quat.y = value[1];
	quat.z = value[2];
	quat.w = value[3];
	return bytesRead;
}

/// \brief	Read a EulerAngle from the archive
/// \param	euler - The EulerAngle to store the values read from the archive
/// \param	tag - the tag for the data
/// \return number of bytes read
UINT CArchive::Read(EulerAngle &euler, const TCHAR *tag)
{
	assert(m_bIsReading);
	float value[3];
	int ival;
	UINT bytesRead;
	bytesRead = (UINT)m_pStream->Read( value, sizeof(float)*3 );
	euler.roll = value[0];
	euler.pitch = value[1];
	euler.yaw = value[2];
	bytesRead += (UINT)m_pStream->Read( &ival, sizeof(int) );
	euler.order = (EULER_ORDER)ival;
	return bytesRead;
}

/// \brief	Read a binary data to the archive
/// \param	buffer - pointer to buffer with binary data
/// \param	size - size of data to read
/// \param	tag - the tag for the data
/// \return number of bytes read
UINT CArchive::Read( void *buffer, size_t size, const TCHAR *tag)
{
	assert(m_bIsReading);
	assert(m_pStream != NULL);
	return (UINT)m_pStream->Read( buffer, size );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CArchive::Write( float value, const TCHAR *tag )
{
	assert(m_bIsWriting);
	m_pStream->Write( &value, sizeof(float) );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CArchive::Write( double value, const TCHAR *tag )
{
	assert(m_bIsWriting);
	m_pStream->Write( &value, sizeof(double) );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CArchive::Write( int value, const TCHAR *tag )
{
	assert(m_bIsWriting);
	m_pStream->Write( &value, sizeof(int) );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CArchive::Write( short value, const TCHAR *tag )
{
	assert(m_bIsWriting);
	m_pStream->Write( &value, sizeof(short) );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CArchive::Write( TCHAR value, const TCHAR *tag )
{
	assert(m_bIsWriting);
	m_pStream->Write( &value, sizeof(TCHAR) );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CArchive::Write( unsigned value, const TCHAR *tag )
{
	assert(m_bIsWriting);
	m_pStream->Write( &value, sizeof(unsigned) );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CArchive::Write( bool value, const TCHAR *tag )
{
	assert(m_bIsWriting);
	m_pStream->Write( &value, sizeof(bool) );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CArchive::Write( const StdString &value, const TCHAR *tag )
{
	assert(m_bIsWriting);
	size_t iCharCount = value.length();
	m_pStream->Write( (const TCHAR*)value, sizeof(TCHAR)*(iCharCount+1) );
}
/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CArchive::Write( const TCHAR * value, const TCHAR *tag )
{
	assert(m_bIsWriting);
	size_t iCharCount = _tcslen(value);
	m_pStream->Write( value, sizeof(TCHAR)*(iCharCount+1) );
}

/// \brief	Write a Vec3 to an archive.
///			Writes three succesive float attibutes to the current element
///			representing a vector 3.
/// \param	vec - The vector to write
/// \param	tag - the tag for the data
void CArchive::Write( Vec3 &vec, const TCHAR *tag )
{
	assert(m_bIsWriting);
	float value[3] = { vec.x, vec.y, vec.z };
	m_pStream->Write( value, sizeof(float)*3 );
}
/// \brief	Write a Vec4 to an archive.
///			Writes three succesive float attibutes to the current element
///			representing a vector 4.
/// \param	vec - The vector to write
/// \param	tag - the tag for the data
void CArchive::Write( Vec4 &vec, const TCHAR *tag )
{
	assert(m_bIsWriting);
	float value[4] = { vec.x, vec.y, vec.z, vec.w };
	m_pStream->Write( value, sizeof(float)*4 );
}
/// \brief	Write a 3x3 matrix to the archive
/// \param	mat - The matrix to write
/// \param	tag - The tag for the data
void CArchive::Write( Matrix3x3 &mat, const TCHAR *tag )
{
	assert(m_bIsWriting);
	m_pStream->Write( mat.m, sizeof(float)*9 );
}

/// \brief	Write a 4x4 matrix to the archive
/// \param	mat - The matrix to write
/// \param	tag - The tag for the data
void CArchive::Write( Matrix4x4 &mat, const TCHAR *tag )
{
	assert(m_bIsWriting);
	m_pStream->Write( mat.m, sizeof(float)*16 );
}

/// \brief	Write a quaternion to the archive
/// \param	quat - The quaternion to write
/// \param	tag - The tag for the data
void CArchive::Write( Quaternion &quat, const TCHAR *tag )
{
	assert(m_bIsWriting);
	float value[4] = { quat.x, quat.y, quat.z, quat.w };
	m_pStream->Write( value, sizeof(float)*4 );
}

/// \brief	Write a quaternion to the archive
/// \param	quat - The quaternion to write
/// \param	tag - The tag for the data
void CArchive::Write( EulerAngle &euler, const TCHAR *tag )
{
	assert(m_bIsWriting);
	float value[3] = { euler.roll, euler.pitch, euler.yaw };
	m_pStream->Write( value, sizeof(float)*3 );
	int ival = (int)euler.order;
	m_pStream->Write( &ival, sizeof(int) );
}

/// \brief	Write a binary data to the archive
/// \param	buffer - pointer to buffer with binary data
/// \param	size - size of data in the buffer
/// \param	tag - the tag for the data
void CArchive::Write( const void *buffer, size_t size, const TCHAR *tag )
{
	assert(m_bIsWriting);
	assert(m_pStream != NULL);
	m_pStream->Write( buffer, size );
}

void CArchive::SetIsWriting( bool bWrite )
{
	if(bWrite)
	{
		m_bIsWriting = true;
		m_bIsReading = false;
	}
	else
	{
		m_bIsWriting = false;
		m_bIsReading = true;
	}
}

bool CArchive::IsEndOfFile()
{
	if (m_pStream)
	{
		return m_pStream->IsEndOfFile();
	}
	else
	{
		return true;
	}
}