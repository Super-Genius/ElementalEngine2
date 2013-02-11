///============================================================================
/// \file		LuaArchive.cpp
/// \brief		Implementation file for CLuaArchive
/// \date		07-29-2005
/// \author		Josh Lulewicz
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

#include "StdAfx.h"
#include "Vec3.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLuaArchive::CLuaArchive()
:m_ArchiveInterface(0)
{
	m_ToolBox = EngineGetToolBox();
	m_pMemchunk = NULL;
	m_ArchiveInterface = NULL;
}

CLuaArchive::CLuaArchive(IArchive *ar)
{
	//m_bIsReading = true;
	//m_bIsWriting = true;
	//m_pStream = NULL;
	m_ArchiveInterface = ar;
	m_pMemchunk = NULL;
}

CLuaArchive::~CLuaArchive( )
{
	Close();
}

bool CLuaArchive::Init( IEEStream *pStream )
{
	return true;
}

void CLuaArchive::Close()
{
	if( m_pMemchunk != NULL )
	{
		delete [] m_pMemchunk;
	}
	m_pMemchunk = NULL;

	if (m_ArchiveInterface)
	{
		m_ArchiveInterface->Close();
		m_ArchiveInterface = NULL;
	}
}

bool CLuaArchive::SetArchive( IArchive* ar)
{
	m_ArchiveInterface = ar;
	return true;
}

void CLuaArchive::InitArchive( int iMode, IHashString* iType )
{
	if( m_pMemchunk != NULL )
		delete [] m_pMemchunk;

	CREATEARCHIVE caOut;
	caOut.mode = iMode;
	caOut.streamData = NULL;
	caOut.streamSize = 0;
	caOut.streamType = iType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut) != MSG_HANDLED)
	{
		m_ToolBox->Log( LOGERROR, _T("CLuaArchive::InitArchive(): Archive Ceation Failed!\n") );
		return;
	}

	m_ArchiveInterface = caOut.archive;
}

void CLuaArchive::InitArchive( int iMode, int iSize, IHashString* iType )
{
	if( m_pMemchunk != NULL )
		delete [] m_pMemchunk;

	m_pMemchunk = new char[iSize];
	memset( m_pMemchunk, 0, iSize );

	CREATEARCHIVE caOut;
	caOut.mode = iMode;
	caOut.streamData = m_pMemchunk;
	caOut.streamSize = iSize;
	caOut.streamType = iType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut) != MSG_HANDLED)
	{
		m_ToolBox->Log( LOGERROR, _T("CLuaArchive::InitArchive(): Archive Ceation Failed!\n") );
		return;
	}
	m_ArchiveInterface = caOut.archive;

}

// Seek to a position in the file if possible
DWORD CLuaArchive::SeekTo(DWORD offset, int aSeek)
{
	return m_ArchiveInterface->SeekTo(offset, aSeek);
}

/// \brief	Test if the IArchive is being read
/// \return	True if the IArchive is being read, false if not
bool CLuaArchive::IsReading() const
{
	return (m_ArchiveInterface->IsReading());
}

/// \brief Starts a New Class Object
/// \param wszName - name of the class
/// \return True if Success, False otherwise
bool CLuaArchive::StartClass(const StdString &wszName ) 
{
	m_ArchiveInterface->StartClass(wszName);
	return true;
}

/// \brienf Ends a New Class Object
/// \return True if Success, False otherwise
bool CLuaArchive::EndClass( void )
{
	m_ArchiveInterface->EndClass();
	return true;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
float CLuaArchive::ReadFloat(const TCHAR *tag )
{
    float value;
	m_ArchiveInterface->Read(value, tag);
    return value;
}

/// \brief	Read a double from the archive
/// \param	value - the float read from the archive
double CLuaArchive::ReadDouble(const TCHAR *tag )
{
	double temp;
	m_ArchiveInterface->Read(temp, tag);
    return temp;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
int CLuaArchive::ReadInt(const TCHAR *tag )
{
	int temp;
	m_ArchiveInterface->Read(temp, tag);
    return temp;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
short CLuaArchive::ReadShort(const TCHAR *tag )
{
	short temp;
	m_ArchiveInterface->Read(temp, tag);
    return temp;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
char CLuaArchive::ReadChar(const TCHAR *tag )
{
    char value;
	m_ArchiveInterface->Read(value, tag);
    return value;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
unsigned CLuaArchive::ReadUInt(const TCHAR *tag )
{
	unsigned temp;
	m_ArchiveInterface->Read(temp, tag);
    return temp;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
bool CLuaArchive::ReadBool(const TCHAR *tag )
{
    bool temp;
	m_ArchiveInterface->Read(temp, tag);
    return temp;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
void CLuaArchive::ReadString( StdString &value, const TCHAR *tag )
{
 	m_ArchiveInterface->Read(value, tag);
}

void CLuaArchive::ReadHashString(IHashString *value, const TCHAR *tag )
{
	StdString tempStr;
	m_ArchiveInterface->Read(tempStr, tag);
	*value = tempStr;
}

/// \brief	Read a Vec3 from the archive
/// \param	tag - Tag for particular attribute
/// \return	Vec3 that was read
void CLuaArchive::ReadVec3(CLuaVec3 &value, const TCHAR *tag )
{
	m_ArchiveInterface->Read(dynamic_cast<Vec3&>( value ), tag);
}

/// \brief	Read a Vec3 from the archive
/// \param	tag - Tag for particular attribute
/// \return	Vec3 that was read
void CLuaArchive::ReadEulerAngle(CLuaEulerAngle &value, const TCHAR *tag )
{
	m_ArchiveInterface->Read(dynamic_cast<EulerAngle&>( value ), tag);
}

void CLuaArchive::ReadMatrix4x4(CLuaMatrix4x4 &value, const TCHAR *tag )
{
	m_ArchiveInterface->Read(dynamic_cast<Matrix4x4&>(value), tag);
}


/// \brief	Read a Matrix3x3 from the archive
/// \param	mat - THe matrix 3x3 to store the values read from the archive
/// \param	tag - the tag for the data
UINT CLuaArchive::Read( Matrix3x3 &mat, const TCHAR *tag)
{
	return m_ArchiveInterface->Read(mat, tag);
}

/// \brief	Read a Matrix4x4 from the archive
/// \param	mat - THe matrix 4x4 to store the values read from the archive
/// \param	tag - the tag for the data
UINT CLuaArchive::Read( Matrix4x4 &mat, const TCHAR *tag)
{
	return m_ArchiveInterface->Read(mat, tag);
}

/// \brief	Read a quaternion from the archive
/// \param	quat - The quaternion to store the values read from the archive
/// \param	tag - the tag for the data
UINT CLuaArchive::Read( Quaternion &quat, const TCHAR *tag)
{
	return m_ArchiveInterface->Read(quat, tag);
}

/// \brief	Read a EulerAngle from the archive
/// \param	euler - The EulerAngle to store the values read from the archive
/// \param	tag - the tag for the data
/// \return number of bytes read
UINT CLuaArchive::Read( EulerAngle &euler, const TCHAR *tag )
{
	return m_ArchiveInterface->Read(euler, tag);
}

/// \brief	Read a binary data to the archive
/// \param	buffer - pointer to buffer with binary data
/// \param	size - size of data to read
/// \param	tag - the tag for the data
/// \return number of bytes read
UINT CLuaArchive::Read( void *buffer, size_t size, const TCHAR *tag )
{
	return m_ArchiveInterface->Read(buffer, size, tag);
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
/// \return number of bytes read
UINT CLuaArchive::Read( float &value, const TCHAR *tag )
{
	return m_ArchiveInterface->Read( value, tag );
}

/// \brief	Read a double from the archive
/// \param	value - the float read from the archive
/// \return number of bytes read
UINT CLuaArchive::Read( double &value, const TCHAR *tag )
{
	return m_ArchiveInterface->Read( value, tag );
}

/// \brief	Read an int from the archive
/// \param	value - the float read from the archive
/// \return number of bytes read
UINT CLuaArchive::Read( int &value, const TCHAR *tag )
{
	return m_ArchiveInterface->Read( value, tag );
}

/// \brief	Read a short from the archive
/// \param	value - the float read from the archive
/// \return number of bytes read
UINT CLuaArchive::Read( short &value, const TCHAR *tag )
{
	return m_ArchiveInterface->Read( value, tag );
}

/// \brief	Read a char from the archive
/// \param	value - the float read from the archive
/// \return number of bytes read
UINT CLuaArchive::Read( char &value, const TCHAR *tag )
{
	return m_ArchiveInterface->Read( value, tag );
}

/// \brief	Read an unsigned from the archive
/// \param	value - the float read from the archive
/// \return number of bytes read
UINT CLuaArchive::Read( unsigned &value, const TCHAR *tag )
{
	return m_ArchiveInterface->Read( value, tag );
}

/// \brief	Read a bool from the archive
/// \param	value - the float read from the archive
/// \return number of bytes read
UINT CLuaArchive::Read( bool &value, const TCHAR *tag )
{
	return m_ArchiveInterface->Read( value, tag );
}

/// \brief	Read a StdString from the archive
/// \param	value - the float read from the archive
/// \return number of bytes read
UINT CLuaArchive::Read( StdString &value, const TCHAR *tag )
{
	return m_ArchiveInterface->Read( value, tag );
}

/// \brief	Read a Vec3 from the archive
/// \param	vec - the vector to store the values read from the archive
/// \param	tag - the tag for the data
/// \return number of bytes read
UINT CLuaArchive::Read( Vec3 &vec, const TCHAR *tag )
{
	return m_ArchiveInterface->Read( vec, tag );
}

/// \brief	Read a Vec4 from the archive
/// \param	vec - the vector to store the values read from the archive
/// \param	tag - the tag for the data
/// \return number of bytes read
UINT CLuaArchive::Read( Vec4 &vec, const TCHAR *tag )
{
	return m_ArchiveInterface->Read( vec, tag );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::WriteFloat( float value, const TCHAR *tag )
{
	m_ArchiveInterface->Write(value, tag);
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::WriteDouble( double value, const TCHAR *tag )
{
	m_ArchiveInterface->Write(value, tag);
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::WriteInt( int value, const TCHAR *tag )
{
	m_ArchiveInterface->Write(value, tag);
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::WriteShort( short value, const TCHAR *tag )
{
	m_ArchiveInterface->Write(value, tag);
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::WriteTCHAR(double value, const TCHAR *tag )
{
    TCHAR temp = (TCHAR)value;
	m_ArchiveInterface->Write(temp, tag);
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::WriteUInt( unsigned value, const TCHAR *tag )
{
	m_ArchiveInterface->Write(value, tag);
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::Write( bool value, const TCHAR *tag )
{
	m_ArchiveInterface->Write(value, tag);
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::WriteString( const StdString &value, const TCHAR *tag )
{
	m_ArchiveInterface->Write(value, tag);
}
/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::WriteTCHARStr( const TCHAR * value, const TCHAR *tag )
{
	m_ArchiveInterface->Write(value, tag);
}

/// \brief	Write a vec3 to the archive
/// \param	tag - the tag to for the data
/// \param	valuex - the x value to write to the archive
/// \param	valuey - the y value to write to the archive
/// \param	valuez - the z value to write to the archive
void CLuaArchive::WriteVec3( CLuaVec3 &value, const TCHAR *tag )
{	
	m_ArchiveInterface->Write( dynamic_cast<Vec3&>( value ) );
}

/// \brief	Write a vec3 to the archive
/// \param	tag - the tag to for the data
/// \param	valuex - the x value to write to the archive
/// \param	valuey - the y value to write to the archive
/// \param	valuez - the z value to write to the archive
void CLuaArchive::WriteVec3( float valuex, float valuey, float valuez, const TCHAR *tag )
{
	CLuaVec3 avec3(valuex, valuey, valuez);
	m_ArchiveInterface->Write( dynamic_cast<Vec3&>( avec3 ) );
}


/// \brief	Write a vec3 to the archive
/// \param	tag - the tag to for the data
/// \param	valuex - the x value to write to the archive
/// \param	valuey - the y value to write to the archive
/// \param	valuez - the z value to write to the archive
void CLuaArchive::WriteEulerAngle( CLuaEulerAngle &value, const TCHAR *tag )
{	
	m_ArchiveInterface->Write( dynamic_cast<EulerAngle&>( value ) );
}

/// \brief	Write a vec3 to the archive
/// \param	tag - the tag to for the data
/// \param	valuex - the x value to write to the archive
/// \param	valuey - the y value to write to the archive
/// \param	valuez - the z value to write to the archive
void CLuaArchive::WriteEulerAngle( float valueroll, float valuepitch, float valueyaw, const TCHAR *tag )
{
	CLuaEulerAngle aeuler(valueroll, valuepitch, valueyaw);
	m_ArchiveInterface->Write( dynamic_cast<EulerAngle&>( aeuler ) );
}

void CLuaArchive::WriteBool( bool value, const TCHAR *tag )
{
	m_ArchiveInterface->Write( value, tag );
}

void CLuaArchive::WriteMatrix4x4( Matrix4x4 &value, const TCHAR *tag )
{
	m_ArchiveInterface->Write(value, tag);
}

/// \brief	Write a Vec3 to an archive.
///			Writes three successive float attributes to the current element
///			representing a vector 3.
/// \param	vec - The vector to write
/// \param	tag - the tag for the data
void CLuaArchive::Write( Vec3 &vec, const TCHAR *tag )
{
	m_ArchiveInterface->Write(vec, tag);
}

/// \brief	Write a Vec4 to an archive.
///			Writes three successive float attributes to the current element
///			representing a vector 4.
/// \param	vec - The vector to write
/// \param	tag - the tag for the data
void CLuaArchive::Write( Vec4 &vec, const TCHAR *tag )
{
	m_ArchiveInterface->Write(vec, tag);
}

/// \brief	Write a 3x3 matrix to the archive
/// \param	mat - The matrix to write
/// \param	tag - The tag for the data
void CLuaArchive::Write( Matrix3x3 &mat, const TCHAR *tag )
{
	m_ArchiveInterface->Write(mat, tag);
}

/// \brief	Write a 4x4 matrix to the archive
/// \param	mat - The matrix to write
/// \param	tag - The tag for the data
void CLuaArchive::Write( Matrix4x4 &mat, const TCHAR *tag )
{
	m_ArchiveInterface->Write(mat, tag);
}

/// \brief	Write a quaternion to the archive
/// \param	quat - The quaternion to write
/// \param	tag - The tag for the data
void CLuaArchive::Write( Quaternion &quat, const TCHAR *tag )
{
	m_ArchiveInterface->Write(quat, tag);
}

/// \brief	Write a EulerAngle to the archive
/// \param	quat - The EulerAngle to write
/// \param	tag - The tag for the data
void CLuaArchive::Write( EulerAngle &euler, const TCHAR *tag )
{
	m_ArchiveInterface->Write(euler, tag);
}

/// \brief	Write a binary data to the archive
/// \param	buffer - pointer to buffer with binary data
/// \param	size - size of data in the buffer
/// \param	tag - the tag for the data
void CLuaArchive::Write( const void *buffer, size_t size, const TCHAR *tag )
{
	m_ArchiveInterface->Write(buffer, size, tag);
}

/// \brief	Write a float to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::Write( float value, const TCHAR *tag )
{
	m_ArchiveInterface->Write( value, tag );
}

/// \brief	Write a double to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::Write( double value, const TCHAR *tag )
{
	m_ArchiveInterface->Write( value, tag );
}

/// \brief	Write an int to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::Write( int value, const TCHAR *tag )
{
	m_ArchiveInterface->Write( value, tag );
}

/// \brief	Write a short to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::Write( short value, const TCHAR *tag )
{
	m_ArchiveInterface->Write( value, tag );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::Write( char value, const TCHAR *tag )
{
	m_ArchiveInterface->Write( value, tag );
}

/// \brief	Write an unsigned to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::Write( unsigned value, const TCHAR *tag )
{
	m_ArchiveInterface->Write( value, tag );
}

/// \brief	Write a StdString to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::Write( const StdString & value, const TCHAR *tag )
{
	m_ArchiveInterface->Write( value, tag );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CLuaArchive::Write( const TCHAR * value, const TCHAR *tag )
{
	m_ArchiveInterface->Write( value, tag );
}

/// \brief  Check to see if we've reached the end of the stream
/// \return true if eof

void CLuaArchive::SetIsWriting(bool bWrite)
{
	m_ArchiveInterface->SetIsWriting(bWrite);
}

bool CLuaArchive::IsEndOfFile()
{
	return m_ArchiveInterface->IsEndOfFile();
}