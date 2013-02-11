///==========================================================================
/// \file	CMemArchive.cpp
/// \brief	Archiving using Memory
/// \date	4/22/2005
/// \author	Brian The Brainiac Bazyk
///==========================================================================

#include "StdAfx.h"

#include "CMemArchive.h"
#include "IVec3.h"
#include "IMatrix3x3.h"
#include "IMatrix4x4.h"
#include "IQuaternion.h"

#include <stdlib.h>

CMemArchive::CMemArchive( )
{
	m_bIsWriting = false;
	m_pStream = NULL;
	m_pStream = NULL;
}

CMemArchive::~CMemArchive( )
{
	m_bIsWriting = false;
	m_pStream = NULL;
	m_pStream = NULL;
}

bool CMemArchive::Init( IEEStream *pStream )
{
	assert( pStream != NULL );
	if( pStream == NULL )
		return false;
    // if data is used as input then we are not writing, we are reading
	m_bIsWriting = pStream->IsReading();
	m_pStream = pStream;

	return true;
}

/// \brief	Test if the IArchive is being read
/// \return	True if the IArchive is being read, false if not
bool CMemArchive::IsReading() const
{
	return (!m_bIsWriting);
}

/// \brief	Test if the IArchive is being written
/// \return	True if the IArchive is being written, false if not
bool CMemArchive::IsWriting() const
{
	return m_bIsWriting;
}

/// \brief Starts a New Class Object
/// \param wszName - name of the class
/// \return True if Success, False otherwise
bool CMemArchive::StartClass( StdString &wszName ) 
{

	return true;
}

/// \brienf Ends a New Class Object
/// \return True if Success, False otherwise
bool CMemArchive::EndClass( void )
{
	return true;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
void CMemArchive::Read( float &value, StdString *tag )
{
	assert(!m_bIsWriting);
	m_pStream->Read( &value, sizeof(float) );
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
void CMemArchive::Read( double &value, StdString *tag )
{
	assert(!m_bIsWriting);
	m_pStream->Read( &value, sizeof(double) );

}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
void CMemArchive::Read( int &value, StdString *tag )
{
	assert(!m_bIsWriting);
	m_pStream->Read( &value, sizeof(int) );
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
void CMemArchive::Read( short &value, StdString *tag )
{
	assert(!m_bIsWriting);
	m_pStream->Read( &value, sizeof(short) );
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
void CMemArchive::Read( char &value, StdString *tag )
{
	assert(!m_bIsWriting);
	m_pStream->Read( &value, sizeof(char) );
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
void CMemArchive::Read( unsigned &value, StdString *tag )
{
	assert(!m_bIsWriting);
	m_pStream->Read( &value, sizeof(unsigned) );
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
void CMemArchive::Read( bool &value, StdString *tag )
{
	assert(!m_bIsWriting);
	m_pStream->Read( &value, sizeof(bool) );
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
void CMemArchive::Read( StdString &value, StdString *tag )
{
	assert(!m_bIsWriting);
	TCHAR tempChar;
	//clear the contents of value;
	value.clear();
	while (m_pStream->Read( &tempChar, sizeof(TCHAR) ) == sizeof(TCHAR))
	{
		if (tempChar == '\0')
			break;
		value += tempChar;
	}
}

/// \brief	Read a Vec3 from the archive
/// \param	vec - the vector to store the values read from the arcihve
/// \param	tag - the tag for the data
void CMemArchive::Read( IVec3 &vec, StdString *tag)
{
	assert(!m_bIsWriting);
	float value;
	m_pInputStream->Read( &value, sizeof(float) );
	vec.SetX(value);
	m_pInputStream->Read( &value, sizeof(float) );
	vec.SetY(value);
	m_pInputStream->Read( &value, sizeof(float) );
	vec.SetZ(value);
}

/// \brief	Read a Matrix3x3 from the archive
/// \param	mat - THe matrix 3x3 to store the values read from the archive
/// \param	tag - the tag for the data
void CMemArchive::Read( IMatrix3x3 &mat, StdString *tag)
{
	assert(!m_bIsWriting);
	float* matptr = mat.GetFloatPtr();
	m_pInputStream->Read( &matptr[ 0], sizeof(float) );
	m_pInputStream->Read( &matptr[ 1], sizeof(float) );
	m_pInputStream->Read( &matptr[ 2], sizeof(float) );
	m_pInputStream->Read( &matptr[ 3], sizeof(float) );
	m_pInputStream->Read( &matptr[ 4], sizeof(float) );
	m_pInputStream->Read( &matptr[ 5], sizeof(float) );
	m_pInputStream->Read( &matptr[ 6], sizeof(float) );
	m_pInputStream->Read( &matptr[ 7], sizeof(float) );
	m_pInputStream->Read( &matptr[ 8], sizeof(float) );
}

/// \brief	Read a Matrix4x4 from the archive
/// \param	mat - THe matrix 4x4 to store the values read from the archive
/// \param	tag - the tag for the data
void CMemArchive::Read( IMatrix4x4 &mat, StdString *tag)
{
	assert(!m_bIsWriting);
	float* matptr = mat.GetFloatPtr();
	m_pInputStream->Read( &matptr[ 0], sizeof(float) );
	m_pInputStream->Read( &matptr[ 1], sizeof(float) );
	m_pInputStream->Read( &matptr[ 2], sizeof(float) );
	m_pInputStream->Read( &matptr[ 3], sizeof(float) );
	m_pInputStream->Read( &matptr[ 4], sizeof(float) );
	m_pInputStream->Read( &matptr[ 5], sizeof(float) );
	m_pInputStream->Read( &matptr[ 6], sizeof(float) );
	m_pInputStream->Read( &matptr[ 7], sizeof(float) );
	m_pInputStream->Read( &matptr[ 8], sizeof(float) );
	m_pInputStream->Read( &matptr[ 9], sizeof(float) );
	m_pInputStream->Read( &matptr[10], sizeof(float) );
	m_pInputStream->Read( &matptr[11], sizeof(float) );
}

/// \brief	Read a quaternion from the archive
/// \param	quat - The quaternion to store the vaules read from the archive
/// \param	tag - the tag for the data
void CMemArchive::Read( IQuaternion &quat, StdString *tag)
{
	assert(!m_bIsWriting);
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CMemArchive::Write( float value, StdString *tag )
{
	assert(m_bIsWriting);
	m_pStream->Write( &value, sizeof(float) );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CMemArchive::Write( double value, StdString *tag )
{
	assert(m_bIsWriting);
	m_pStream->Write( &value, sizeof(double) );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CMemArchive::Write( int value, StdString *tag )
{
	assert(m_bIsWriting);
	m_pStream->Write( &value, sizeof(int) );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CMemArchive::Write( short value, StdString *tag )
{
	assert(m_bIsWriting);
	m_pStream->Write( &value, sizeof(short) );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CMemArchive::Write( TCHAR value, StdString *tag )
{
	assert(m_bIsWriting);
	m_pStream->Write( &value, sizeof(TCHAR) );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CMemArchive::Write( unsigned value, StdString *tag )
{
	assert(m_bIsWriting);
	m_pStream->Write( &value, sizeof(unsigned) );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CMemArchive::Write( bool value, StdString *tag )
{
	assert(m_bIsWriting);
	m_pStream->Write( &value, sizeof(bool) );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CMemArchive::Write( const StdString &value, StdString *tag )
{
	assert(m_bIsWriting);
	TCHAR tempChar;
	for( int i = 0; i <= value.length(); i++ )
	{
		tempChar = ((TCHAR *)value)[i];
        m_pStream->Write( &tempChar, sizeof(TCHAR) );
	}
	//write the cr
	//m_pStream->Write( '\0', sizeof(wchar_t) );
}
/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CMemArchive::Write( const TCHAR * value, StdString *tag )
{
	assert(m_bIsWriting);
	TCHAR tempChar;
	for( int i = 0; value[i] != '\0'; i++ )
	{
		tempChar = value[i];
        m_pStream->Write( &tempChar, sizeof(TCHAR) );
	}
	tempChar = '\0';
	m_pStream->Write( &tempChar, sizeof(TCHAR) );
}

/// \brief	Write a Vec3 to an archive.
///			Writes three succesive float attibutes to the current element
///			representing a vector 3.
/// \param	vec - The vector to write
/// \param	tag - the tag for the data
void CMemArchive::Write( IVec3 &vec, StdString *tag )
{
	assert(m_bIsWriting);
	float value;
	value = vec.GetX();
	m_pOutputStream->Write( &value, sizeof(float) );
	value = vec.GetY();
    m_pOutputStream->Write( &value, sizeof(float) );
	value = vec.GetZ();
	m_pOutputStream->Write( &value, sizeof(float) );
}
/// \brief	Write a 3x3 matrix to the archive
/// \param	mat - The matrix to write
/// \param	tag - The tag for the data
void CMemArchive::Write( IMatrix3x3 &mat, StdString *tag )
{
	assert(m_bIsWriting);
	float* matptr = mat.GetFloatPtr();
	m_pOutputStream->Write( &matptr[0], sizeof(float) );
	m_pOutputStream->Write( &matptr[1], sizeof(float) );
	m_pOutputStream->Write( &matptr[2], sizeof(float) );
	m_pOutputStream->Write( &matptr[3], sizeof(float) );
	m_pOutputStream->Write( &matptr[4], sizeof(float) );
	m_pOutputStream->Write( &matptr[5], sizeof(float) );
	m_pOutputStream->Write( &matptr[6], sizeof(float) );
	m_pOutputStream->Write( &matptr[7], sizeof(float) );
	m_pOutputStream->Write( &matptr[8], sizeof(float) );
}

/// \brief	Write a 4x4 matrix to the archive
/// \param	mat - The matrix to write
/// \param	tag - The tag for the data
void CMemArchive::Write( IMatrix4x4 &mat, StdString *tag )
{
	assert(m_bIsWriting);
	float* matptr = mat.GetFloatPtr();
	m_pOutputStream->Write( &matptr[ 0], sizeof(float) );
	m_pOutputStream->Write( &matptr[ 1], sizeof(float) );
	m_pOutputStream->Write( &matptr[ 2], sizeof(float) );
	m_pOutputStream->Write( &matptr[ 3], sizeof(float) );
	m_pOutputStream->Write( &matptr[ 4], sizeof(float) );
	m_pOutputStream->Write( &matptr[ 5], sizeof(float) );
	m_pOutputStream->Write( &matptr[ 6], sizeof(float) );
	m_pOutputStream->Write( &matptr[ 7], sizeof(float) );
	m_pOutputStream->Write( &matptr[ 8], sizeof(float) );
	m_pOutputStream->Write( &matptr[ 9], sizeof(float) );
	m_pOutputStream->Write( &matptr[10], sizeof(float) );
	m_pOutputStream->Write( &matptr[11], sizeof(float) );
}

/// \brief	Write a quaternion to the archive
/// \param	quat - The quaternion to write
/// \param	tag - The tag for the data
void CMemArchive::Write( IQuaternion &quat, StdString *tag )
{
	assert(m_bIsWriting);
}
