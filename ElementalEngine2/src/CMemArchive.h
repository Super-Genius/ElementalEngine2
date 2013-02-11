///==========================================================================
/// \file	CMemArchive.h
/// \brief	Archiving using Memory
/// \date	4/22/2005
/// \author	Brian Bazyk
///==========================================================================

#ifndef	__CMEMARCHIVE_H_
#define	__CMEMARCHIVE_H_

#include "IMemArchive.h"
class IVec;
class IMatrix3x3;
class IMatrix4x4;
class IQuaternion;

class CMemArchive : public IMemArchive
{
private:
	/// \brief	Can't use copy constructor
	CMemArchive( const CMemArchive& ar );
	/// \brief	Can't use assignment operator
	void operator=( const CMemArchive& ar );

public:
	/// \brief	default constructor
	CMemArchive();
	/// \brief	Destructor
	virtual ~CMemArchive();

	/// \brief	Initialize the archive
	/// \param	pStream - The stream to input
	virtual bool Init( IEEStream *pStream );

	/// \brief	Test if the IArchive is being read
	/// \return	True if the IArchive is being read, false if not
	virtual bool IsReading() const;
	/// \brief	Test if the IArchive is being written
	/// \return	True if the IArchive is being written, false if not
	virtual bool IsWriting() const;

	/// \brief Starts a New Class Object
	/// \param wszName - name of the class
	/// \return True if Success, False otherwise
	virtual bool StartClass( StdString &wszName );
	/// \brienf Ends a New Class Object
	/// \return True if Success, False otherwise
	virtual bool EndClass( void );
	
	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual void Read( float &value, StdString *tag );
	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual void Read( double &value, StdString *tag );
	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual void Read( int &value, StdString *tag );
	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual void Read( short &value, StdString *tag );
	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual void Read( char &value, StdString *tag );
	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual void Read( unsigned &value, StdString *tag );
	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual void Read( bool &value, StdString *tag );
	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual void Read( StdString &value, StdString *tag );

	/// \brief	Read a Vec3 from the archive
	/// \param	vec - the vector to store the values read from the arcihve
	/// \param	tag - the tag for the data
	virtual void Read( IVec3 &vec, StdString *tag );
	/// \brief	Read a Matrix3x3 from the archive
	/// \param	mat - THe matrix 3x3 to store the values read from the archive
	/// \param	tag - the tag for the data
	virtual void Read( IMatrix3x3 &mat, StdString *tag );
	/// \brief	Read a Matrix4x4 from the archive
	/// \param	mat - THe matrix 4x4 to store the values read from the archive
	/// \param	tag - the tag for the data
	virtual void Read( IMatrix4x4 &mat, StdString *tag );
	/// \brief	Read a quaternion from the archive
	/// \param	quat - The quaternion to store the vaules read from the archive
	/// \param	tag - the tag for the data
	virtual void Read( IQuaternion &quat, StdString *tag );
	
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( float value, StdString *tag );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( double value, StdString *tag );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( int value, StdString *tag );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( short value, StdString *tag );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( char value, StdString *tag );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( unsigned value, StdString *tag );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( bool value, StdString *tag );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( const StdString & value, StdString *tag );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( const TCHAR * value, StdString *tag );
	/// \brief	Write a Vec3 to an archive.
	///			Writes three succesive float attibutes to the current element
	///			representing a vector 3.
	/// \param	vec - The vector to write
	/// \param	tag - the tag for the data
	virtual void Write( IVec3 &vec, StdString *tag );
	/// \brief	Write a 3x3 matrix to the archive
	/// \param	mat - The matrix to write
	/// \param	tag - The tag for the data
	virtual void Write( IMatrix3x3 &mat, StdString *tag );
	/// \brief	Write a 4x4 matrix to the archive
	/// \param	mat - The matrix to write
	/// \param	tag - The tag for the data
	virtual void Write( IMatrix4x4 &mat, StdString *tag );
	/// \brief	Write a quaternion to the archive
	/// \param	quat - The quaternion to write
	/// \param	tag - The tag for the data
	virtual void Write( IQuaternion &quat, StdString *tag );

private:
	bool m_bIsWriting;
	IEEStream* m_pStream;

};

#endif //__CMEMARCHIVE_H_