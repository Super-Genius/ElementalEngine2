///============================================================================
/// \file		LuaArchive.h
/// \brief		Header file for CLuaArchive (wrapper for EE-Archive)
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

#ifndef	CLUAARCHIVE_H
#define	CLUAARCHIVE_H

// tolua_begin
class CLuaArchive : public ILuaArchive
{
    // tolua_end

public:
 
//tolua will generate a destructor for us, and our
//wrapper is not a child of IArchive, so should not mimic the CArchive
//archive so perfectly anyway, so...
#if 0
	/// Close the archive, since we allocated it
	virtual void Close()
	{
		delete this;
	}
#endif

	// tolua_begin
	/// \brief Constructor used by "CreateLuaArchive" requests
    CLuaArchive();

	/// \break create archive from IArchiv
	CLuaArchive(IArchive *ar);
	/// \brief	Destructor
	virtual ~CLuaArchive();

	/// \brief	Initialize the archive
	/// \param	pStream - The stream to input xml
	virtual bool Init( IEEStream *pStream );

	/// Close the archive, since we allocated it
	virtual void Close();

    /// Used by "EE to lua script" callback system to wrap parameters going to lua
    virtual bool SetArchive( IArchive* ar);

	/// Used by Lua Scripts to create an Archive in EE with dynamically allocated memory
	void InitArchive( int iMode, IHashString* iType );

	/// Used by Lua Scripts to create an Archive in EE
	void InitArchive( int iMode, int iSize, IHashString* iType );

    /// \brief	Test if the IArchive is being read
	/// \return	True if the IArchive is being read, false if not
	virtual bool IsReading() const;

	/// Seek to a position in the file if possible
	/// \param offset - offset to seek from
	/// \param aSeek = place to seek from
	virtual DWORD SeekTo(DWORD offset, int aSeek = SEEK_SET);

	/// \brief Starts a New Class Object
	/// \param wszName - name of the class
	/// \return True if Success, False otherwise
	virtual bool StartClass(const StdString &wszName );
	/// \brienf Ends a New Class Object
	/// \return True if Success, False otherwise
	virtual bool EndClass( void );
	
	/// \brief	Read a float from the archive
	/// \param tag - Tag for particular attribute
	/// \return float that was read
	virtual float ReadFloat(const TCHAR *tag = _T("") );
	/// \brief	Read a double from the archive
	/// \param tag - Tag for particular attribute
	/// \return double that was read
	virtual double ReadDouble(const TCHAR *tag = _T("") );
	/// \brief	Read an int from the archive
	/// \param tag - Tag for particular attribute
    /// \return int that was read
	virtual int ReadInt(const TCHAR *tag = _T("") );
	/// \brief	Read a short from the archive
	/// \param tag - Tag for particular attribute
	/// \return short that was read
	virtual short ReadShort(const TCHAR *tag = _T("") );
	/// \brief	Read a TCHAR from the archive
	/// \param tag - Tag for particular attribute
	/// \return TCHAR that was read
	virtual char ReadChar(const TCHAR *tag = _T("") );
	/// \brief	Read an unsigned int from the archive
	/// \param tag - Tag for particular attribute
	/// \return unsigned that was read
	virtual unsigned ReadUInt(const TCHAR *tag = _T("") );
	/// \brief	Read a bool from the archive
	/// \param tag - Tag for particular attribute
	/// \return bool that was read
	virtual bool ReadBool(const TCHAR *tag = _T("") );
	/// \brief	Read a string from the archive
	/// \param value - String to Read to
	/// \param tag - Tag for particular attribute
	virtual void ReadString(StdString &value, const TCHAR *tag = _T("") );

	virtual void ReadHashString(IHashString *value, const TCHAR *tag = _T("") );
	/// \brief	Read a Vec3 from the archive
	/// \param	value - Vec3 to Read to
	/// \param	tag - Tag for particular attribute
	virtual void ReadVec3(CLuaVec3 &value, const TCHAR *tag = _T("") );

	/// \brief	Read a Vec3 from the archive
	/// \param	value - Vec3 to Read to
	/// \param	tag - Tag for particular attribute
	virtual void ReadEulerAngle(CLuaEulerAngle &value, const TCHAR *tag = _T("") );

	/// \brief	Read a Matrix4x4 from the archive
	/// \param	value - Matrix4x4 to Read to
	/// \param	tag - Tag for particular attribute
	virtual void ReadMatrix4x4(CLuaMatrix4x4 &value, const TCHAR *tag = _T("") );

	//virtual void Read( TCHARk *value, const TCHAR *tag = _T("") );

	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	/// \return number of bytes read
	virtual UINT Read( float &value, const TCHAR *tag = _T(""));
	/// \brief	Read a double from the archive
	/// \param	value - the float read from the archive
	/// \return number of bytes read
	virtual UINT Read( double &value, const TCHAR *tag = _T(""));
	/// \brief	Read an int from the archive
	/// \param	value - the float read from the archive
	/// \return number of bytes read
	virtual UINT Read( int &value, const TCHAR *tag = _T(""));
	/// \brief	Read a short from the archive
	/// \param	value - the float read from the archive
	/// \return number of bytes read
	virtual UINT Read( short &value, const TCHAR *tag = _T(""));
	/// \brief	Read a char from the archive
	/// \param	value - the float read from the archive
	/// \return number of bytes read
	virtual UINT Read( char &value, const TCHAR *tag = _T(""));
	/// \brief	Read an unsigned from the archive
	/// \param	value - the float read from the archive
	/// \return number of bytes read
	virtual UINT Read( unsigned &value, const TCHAR *tag = _T(""));
	/// \brief	Read a bool from the archive
	/// \param	value - the float read from the archive
	/// \return number of bytes read
	virtual UINT Read( bool &value, const TCHAR *tag = _T(""));
	/// \brief	Read a StdString from the archive
	/// \param	value - the float read from the archive
	/// \return number of bytes read
	virtual UINT Read( StdString &value, const TCHAR *tag = _T(""));
	/// \brief	Read a Vec3 from the archive
	/// \param	vec - the vector to store the values read from the archive
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( Vec3 &vec, const TCHAR *tag = _T(""));
	/// \brief	Read a Vec4 from the archive
	/// \param	vec - the vector to store the values read from the archive
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( Vec4 &vec, const TCHAR *tag = _T(""));
	/// \brief	Read a Matrix3x3 from the archive
	/// \param	mat - THe matrix 3x3 to store the values read from the archive
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( Matrix3x3 &mat, const TCHAR *tag = _T(""));
	/// \brief	Read a Matrix4x4 from the archive
	/// \param	mat - THe matrix 4x4 to store the values read from the archive
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( Matrix4x4 &mat, const TCHAR *tag = _T(""));
	/// \brief	Read a quaternion from the archive
	/// \param	quat - The quaternion to store the values read from the archive
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( Quaternion &quat, const TCHAR *tag = _T(""));
	/// \brief	Read a EulerAngle from the archive
	/// \param	euler - The EulerAngle to store the values read from the archive
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( EulerAngle &euler, const TCHAR *tag = _T("") );
	/// \brief	Read a binary data to the archive
	/// \param	buffer - pointer to buffer with binary data
	/// \param	size - size of data to read
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( void *buffer, size_t size, const TCHAR *tag = _T("") );
	
	/// \brief	Write a float to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( float value, const TCHAR *tag = _T("FloatAttribute"));
	/// \brief	Write a double to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( double value, const TCHAR *tag = _T("DoubleAttribute"));
	/// \brief	Write an int to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( int value, const TCHAR *tag = _T("IntAttribute"));
	/// \brief	Write a short to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( short value, const TCHAR *tag = _T("ShortAttribute"));
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( char value, const TCHAR *tag = _T("CharAttribute"));
	/// \brief	Write an unsigned to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( unsigned value, const TCHAR *tag = _T("UnsignedAttribute"));
	/// \brief	Write a bool to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( bool value, const TCHAR *tag = _T("BoolAttribute"));
	/// \brief	Write a StdString to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( const StdString & value, const TCHAR *tag = _T("StringAttribute"));
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( const TCHAR * value, const TCHAR *tag = _T("StringAttribute"));
	/// \brief	Write a Vec3 to an archive.
	///			Writes three succesive float attibutes to the current element
	///			representing a vector 3.
	/// \param	vec - The vector to write
	/// \param	tag - the tag for the data
	virtual void Write( Vec3 &vec, const TCHAR *tag = _T("Vec3Atribute") );
	/// \brief	Write a Vec3 to an archive.
	///			Writes three succesive float attibutes to the current element
	///			representing a vector 3.
	/// \param	vec - The vector to write
	/// \param	tag - the tag for the data
	virtual void Write( Vec4 &vec, const TCHAR *tag = _T("Vec4Atribute") );
	/// \brief	Write a 3x3 matrix to the archive
	/// \param	mat - The matrix to write
	/// \param	tag - The tag for the data
	virtual void Write( Matrix3x3 &mat, const TCHAR *tag = _T("Matrix3x3Attribute"));
	/// \brief	Write a 4x4 matrix to the archive
	/// \param	mat - The matrix to write
	/// \param	tag - The tag for the data
	virtual void Write( Matrix4x4 &mat, const TCHAR *tag = _T("Matrix4x4Attribute"));
	/// \brief	Write a quaternion to the archive
	/// \param	quat - The quaternion to write
	/// \param	tag - The tag for the data
	virtual void Write( Quaternion &quat, const TCHAR *tag = _T("QuaternionAttribute"));
	/// \brief	Write a EulerAngle to the archive
	/// \param	quat - The EulerAngle to write
	/// \param	tag - The tag for the data
	virtual void Write( EulerAngle &euler, const TCHAR *tag = _T("EulerAttribute"));
	/// \brief	Write a binary data to the archive
	/// \param	buffer - pointer to buffer with binary data
	/// \param	size - size of data in the buffer
	/// \param	tag - the tag for the data
	virtual void Write( const void *buffer, size_t size, const TCHAR *tag = _T("BinaryAttribute") );

	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void WriteFloat( float value, const TCHAR *tag = _T("") );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void WriteDouble( double value, const TCHAR *tag = _T("") );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void WriteInt( int value, const TCHAR *tag = _T("") );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void WriteShort( short value, const TCHAR *tag = _T("") );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void WriteTCHAR(double value, const TCHAR *tag = _T("") );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void WriteUInt( unsigned value, const TCHAR *tag = _T("") );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void WriteString( const StdString &value, const TCHAR *tag = _T("") );
	/// \brief	Write a char to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void WriteTCHARStr( const TCHAR * value, const TCHAR *tag = _T("") );
	/// \brief	Write a vec3 to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the vec3 value to write to the archive
	virtual void WriteVec3( CLuaVec3 &value, const TCHAR *tag = _T("") );
	/// \brief	Write a vec3 to the archive
	/// \param	tag - the tag to for the data
	/// \param	valuex - the x value to write to the archive
	/// \param	valuey - the y value to write to the archive
	/// \param	valuez - the z value to write to the archive
	virtual void WriteVec3( float valuex, float valuey, float valuez, const TCHAR *tag = _T("") );
	/// \brief	Write a vec3 to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the vec3 value to write to the archive
	virtual void WriteEulerAngle( CLuaEulerAngle &value, const TCHAR *tag = _T("") );
	/// \brief	Write a vec3 to the archive
	/// \param	tag - the tag to for the data
	/// \param	valuex - the x value to write to the archive
	/// \param	valuey - the y value to write to the archive
	/// \param	valuez - the z value to write to the archive
	virtual void WriteEulerAngle( float valueroll, float valuepitch, float valueyaw, const TCHAR *tag = _T("") );
	/// Write a bool value
	/// \param value = bool value to write
	/// \param tag = attribute name, if you choose to specify it
	virtual void WriteBool( bool value, const TCHAR *tag = _T("") );
	/// Write a matrix value
	/// \param value = matrix value to write
	/// \param tag = attribute name, if you choose to specify it
	virtual void WriteMatrix4x4( Matrix4x4 &value, const TCHAR *tag = _T("") );

	/// \biref  Set the archive to write or read
	///	\param	bWrite - if true, the archive is in write mode, else in read mode
	virtual void SetIsWriting(bool bWrite);

	/// \brief  Check to see if we've reached the end of the stream
	/// \return true if eof
	virtual bool IsEndOfFile();

	// tolua_end
private:
//	bool m_bIsReading;
//	bool m_bIsWriting;
//	IEEStream* m_pStream;
	IArchive *m_ArchiveInterface;

	char* m_pMemchunk;

	IToolBox* m_ToolBox;
// tolua_begin
};
// tolua_end
#endif // CLUAARCHIVE_H 
