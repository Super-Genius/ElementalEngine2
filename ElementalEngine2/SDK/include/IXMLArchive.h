///==========================================================================
/// \file	IXMLArchive.h
/// \brief	Archiving using XML.
/// \date	5/30/2005
/// \author	Kenneth Hurley
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

#ifndef	IXMLARCHIVE_H
#define	IXMLARCHIVE_H

#include "IArchive.hpp"

namespace ElementalEngine
{

class Vec3;
class Matrix3x3;
class Matrix4x4;
class Quaternion;

///==========================================================================
/// \class	IXMLArchive
/// \brief	Archive using XML and schemas.
///			This class uses the SAX parser from LibXML++ to parse the XML
///			file. Since it is parsed using SAX only the current node is in
///			memory.
///==========================================================================
class IXMLArchive : public IArchive
{
public:
	/// \brief	Destructor
	virtual ~IXMLArchive() {};

	/// \brief	Initialize the archive
	/// \param	pStream - The stream to input xml
	virtual bool Init( IEEStream *pStream ) = 0;

	/// \brief	Deinitialize the archive
	virtual void DeInit( void ) = 0;

	/// Get the position of the current node
	/// \param void
	/// \return int - number of current node
	virtual int GetCurrentNodePosition() = 0;
	/// \brief	Get the number of attributes in the tag
	/// \return	The number of attributes in the node, returns 0 if there 
	///			are no attributes.
	virtual int GetNumAttributes() = 0;
	/// \brief	Get the currently selected attribute
	/// \return	The number of the currently selected attribute
	virtual int GetCurrentAttribute() = 0;
	/// \brief	Creates a map of attributes at the current node to their names
	/// \param &attributesMap map in which to populate the attributes
	virtual void GetAttributesMap( map<StdString, StdString> &attributesMap ) = 0;
	/// \brief Gets the current name of the node
	/// \return The name of the current node;
	virtual StdString &GetCurrentNodeName( void ) = 0;
	/// \brief	Load a schema and set it to validate
	/// \param	name - The name of the XML Schema to load
	virtual void LoadSchema( TCHAR *name ) = 0;
	/// \brief	Load a schema and set it to validate
	/// \param	name - The name of the XML Schema to load
	virtual void LoadSchema( StdString &name ) = 0;
	/// \brief	Validate a node with the loaded schema
	/// \return	true if the schema is valid, false if it is not
	virtual bool ValidateNode() = 0;
	/// \brief	Move to the Next Node
	/// \param &string stores the name of the node in the string
	/// \return true for Success, false for No Move or Error
	virtual bool GetNode( StdString &szName ) = 0;
	/// \brief	Move to the Parent Node
	/// \param &string stores the name of the node in the string
	/// \return true for Success, false for No Move or Error
	virtual bool GotoParentNode( StdString &szName ) = 0;
	/// \brief	Move to the Next Node
	/// \param &string stores the name of the node in the string
	/// \return true for Success, false for No Move or Error
	virtual bool ReturnToChildNode( StdString &szName ) = 0;
	/// \brief	Get the name of the schema for the current node
	/// \return	the name and path of the schema for the current node
	virtual StdString &GetSchema() = 0;
	/// \brief	Get the depth of the current node in the XML tree
	/// \return	the depth of the element in the XML tree
	virtual int GetDepth() = 0;
	/// \brief	Get the name of the parent element
	/// \return	pointer to string with the name of the parent element
	virtual StdString *GetParentName() = 0;

	/// \brief Set the archive to write or read
	///	\param bWrite - if true, archive is writing, else reading
	virtual void SetIsWriting( bool bWrite ) = 0;

	/// \brief	Test if the IArchive is being read
	/// \return	True if the IArchive is being read, false if not
	virtual bool IsReading() const = 0;
	/// \brief	Test if the IArchive is being written
	/// \return	True if the IArchive is being written, false if not
	virtual bool IsWriting() const = 0;
	/// \brief Starts a New Class Object
	/// \param wszName - name of the class
	/// \return True if Success, False otherwise
	virtual bool StartClass(const StdString &wszName ) = 0;
	/// \brienf Ends a New Class Object
	/// \return True if Success, False otherwise
	virtual bool EndClass( void ) = 0;
	
	/// \brief	Write the header for the XML document
	/// \param	wVersion - The XML version. Defaults to "<?xml version="1.0"?>"
	/// \param	wEncode - The character encoding of the XML document
	/// \return	true on successfully writing the header, false on failure
	virtual bool CreateHeader(const StdString &wVersion = _T(""), const StdString &wEncode = _T("")) = 0;

	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	virtual UINT Read( float &value, const TCHAR *tag = _T("") ) = 0;
	/// \brief	Read a double from the archive
	/// \param	value - the float read from the archive
	virtual UINT Read( double &value, const TCHAR *tag = _T("") ) = 0;
	/// \brief	Read a int from the archive
	/// \param	value - the float read from the archive
	virtual UINT Read( int &value, const TCHAR *tag = _T("") ) = 0;
	/// \brief	Read a short from the archive
	/// \param	value - the float read from the archive
	virtual UINT Read( short &value, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a char from the archive
	/// \param	value - the float read from the archive
	virtual UINT Read( char &value, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a unsigned from the archive
	/// \param	value - the float read from the archive
	virtual UINT Read( unsigned &value, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a bool from the archive
	/// \param	value - the float read from the archive
	virtual UINT Read( bool &value, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a StdString from the archive
	/// \param	value - the float read from the archive
	virtual UINT Read( StdString &value, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a Vec3 from the archive
	/// \param	vec - the vector to store the values read from the arcihve
	/// \param	tag - the tag for the data
	virtual UINT Read( Vec3 &vec, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a Vec4 from the archive
	/// \param	vec - the vector to store the values read from the arcihve
	/// \param	tag - the tag for the data
	virtual UINT Read( Vec4 &vec, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a Matrix3x3 from the archive
	/// \param	mat - THe matrix 3x3 to store the values read from the archive
	/// \param	tag - the tag for the data
	virtual UINT Read( Matrix3x3 &mat, const TCHAR *tag = _T("")) = 0;
	/// \brief	Read a Matrix4x4 from the archive
	/// \param	mat - THe matrix 4x4 to store the values read from the archive
	/// \param	tag - the tag for the data
	virtual UINT Read( Matrix4x4 &mat, const TCHAR *tag = _T("") ) = 0;
	/// \brief	Read a quaternion from the archive
	/// \param	quat - The quaternion to store the values read from the archive
	/// \param	tag - the tag for the data
	virtual UINT Read( Quaternion &quat, const TCHAR *tag = _T("") ) = 0;

	/// \brief	Write a float to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( float value, const TCHAR *tag = _T("FloatAttribute")) = 0;
	/// \brief	Write a double to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( double value, const TCHAR *tag = _T("DoubleAttribute")) = 0;
	/// \brief	Write a int to the archive
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
	/// \brief	Write a unsigned to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( unsigned value, const TCHAR *tag = _T("UnsignedAttribute")) = 0;
	/// \brief	Write a bool to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( bool value, const TCHAR *tag = _T("BoolAttribute")) = 0;
	/// \brief	Write a const StdString to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( const StdString &value, const TCHAR *tag = _T("StringAttribute")) = 0;
	/// \brief	Write a const TCHAR * to the archive
	/// \param	tag - the tag to for the data
	/// \param	value - the char to write to the archive
	virtual void Write( const TCHAR *value, const TCHAR *tag = _T("StringAtribute")) = 0;
	/// \brief	Write a Vec3 to an archive.
	///			Writes three successive float attributes to the current element
	///			representing a vector 3.
	/// \param	vec - The vector to write
	/// \param	tag - the tag for the data
	virtual void Write( Vec3 &vec, const TCHAR *tag = _T("Vec3Atribute")) = 0;
	/// \brief	Write a Vec4 to an archive.
	///			Writes three successive float attributes to the current element
	///			representing a vector 4.
	/// \param	vec - The vector to write
	/// \param	tag - the tag for the data
	virtual void Write( Vec4 &vec, const TCHAR *tag = _T("Vec4Atribute") ) = 0;
	/// \brief	Write a 3x3 matrix to the archive
	/// \param	mat - The matrix to write
	/// \param	tag - The tag for the data
	virtual void Write( Matrix3x3 &mat, const TCHAR *tag = _T("StringAttribute")) = 0;
	/// \brief	Write a 4x4 matrix to the archive
	/// \param	mat - The matrix to write
	/// \param	tag - The tag for the data
	virtual void Write( Matrix4x4 &mat, const TCHAR *tag = _T("StringAttribute")) = 0;
	/// \brief	Write a quaternion to the archive
	/// \param	quat - The quaternion to write
	/// \param	tag - The tag for the data
	virtual void Write( Quaternion &quat, const TCHAR *tag = _T("StringAttribute")) = 0;

	/// Get number of nodes in XML file
	/// \return DWORD - number of nodes
	virtual DWORD GetNumberOfNodes() = 0;
	/// Checks to see if we're at the end of stream
	/// \return true if eof
	virtual bool IsEndOfFile() = 0;
};

}	//namespace ElementalEngine

#endif	// IXMLARCHIVE_H