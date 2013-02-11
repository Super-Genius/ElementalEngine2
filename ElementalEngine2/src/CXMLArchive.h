///==========================================================================
/// \file	CXMLArchive.h
/// \brief	Archiving using XML.
/// \date	3/22/2005
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

#ifndef	CXMLARCHIVE_H
#define	CXMLARCHIVE_H

#include "IXMLArchive.h"
#include "LibXML++\LibXML++.h"
#include "LibXML++\io\istreamparserinputbuffer.h"

using namespace xmlpp;

namespace ElementalEngine
{

class Vec3;
class Matrix3x3;
class Matrix4x4;
class Quaternion;
class IEEStream;
class CEEStdStreamProxy;

///==========================================================================
/// \class	CXMLArchive
/// \brief	Archive using XML and schemas.
///			This class uses the SAX parser from LibXML++ to parse the XML
///			file. Since it is parsed using SAX only the current node is in
///			memory.
///==========================================================================
class CXMLArchive : public IXMLArchive
{
public:
	/// \brief	default constructor
	CXMLArchive();
	/// \brief	Destructor
	virtual ~CXMLArchive();

	/// \brief	Initialize the archive
	/// \param	The name of the XML file to load
	/// \param bInput = boolean if XML is input or output
	/// \return true if successful initialization
	bool Init( TCHAR *pFile, bool bInput = true );

	/// Seek to a position in the file if possible
	/// \param offset - offset to seek from
	/// \param aSeek = place to seek from
	DWORD SeekTo(DWORD offset, int aSeek = SEEK_SET);

	/// Close the archive, since we allocated it
	virtual void Close()
	{
		delete this;
	}

	/// \brief	Initialize the archive
	/// \param	pStream - The stream to input xml
	/// \return true if successful initialization
	bool Init( IEEStream *pStream );
	/// \brief	Deinitialize the archive
	void DeInit( void );

	/// Get the number of the current node
	/// \param void
	/// \return int - number of current node
	int GetCurrentNodePosition();
	/// \brief	Get the number of attributes in the tag
	/// \return	The number of attributes in the node, returns 0 if there 
	///			are no attributes.
	int GetNumAttributes();
	/// \brief	Get the currently selected attribute
	/// \return	The number of the currently selected attribute 
	///			or -1 if current attribute is not set
	int GetCurrentAttribute();
	/// \brief	Creates a map of attributes at the current node to their names
	/// \param &attributesMap map in which to populate the attributes
	void GetAttributesMap( map<StdString, StdString> &attributesMap );
	/// \brief Gets the current name of the node
	/// \return The name of the current node;
	StdString &GetCurrentNodeName( void );
	/// \brief	Load a schema and set it to validate
	/// \param	name - The name of the XML Schema to load
	void LoadSchema( TCHAR *name );
	/// \brief	Load a schema and set it to validate
	/// \param	name - The name of the XML Schema to load
	void LoadSchema( StdString &name );
	/// \brief	Validate a node with the loaded schema
	/// \return	true if the schema is valid, false if it is not
	bool ValidateNode();
	/// \brief	Move to the Next Node
	/// \param &StdString stores the name of the node in the string
	/// \return true for Success, false for No Move or Error
	bool GetNode( StdString &szName );
	/// \brief	Get the name of the schema for the current node
	/// \return	the name and path of the schema for the current node
	StdString &GetSchema();
	/// \brief	Get the depth of the current node in the XML tree
	/// \return	the depth of the element in the XML tree
	int GetDepth();
	/// \brief	Get the name of the parent element
	/// \return	The name of the parent element
	StdString *GetParentName();

	// Dealing with Parent/Child Manipulation
	/// \brief Moves the Reader to the Parent Node
	/// \param &StdString stores the name of the node in the string
	/// \return true for Success, false for No Move or Error
	virtual bool GotoParentNode( StdString &szName);

	/// \brief Moves the Reader back to the Child Node
	/// \param &StdString stores the name of the node in the string
	/// \return true for Success, false for No Move or Error
	virtual bool ReturnToChildNode( StdString &szName );

	/// \brief	Test if the IArchive is being read
	/// \return	True if the IArchive is being read, false if not
	virtual bool IsReading() const;
	/// \brief	Test if the IArchive is being written
	/// \return	True if the IArchive is being written, false if not
	virtual bool IsWriting() const;
	/// \brief Starts a New Class Object
	/// \param wszName - name of the class
	/// \return True if Success, False otherwise
	virtual bool StartClass(const StdString &wszName ) ;
	/// \brienf Ends a New Class Object
	/// \return True if Success, False otherwise
	virtual bool EndClass( void );
	
	/// \brief	Write the header for the XML document
	/// \param	wVersion - The XML version. Defaults to "<?xml version="1.0"?>"
	/// \param	wEncode - The character encoding of the XML document
	/// \return	true on successfully writing the header, false on failure
	bool CreateHeader(const StdString &wVersion, const StdString &wEncode);

	/// \brief Set the archive to write or read
	/// Not implemented in xmlarchive?
	///	\param bWrite - if true, archive is writing, else reading
	virtual void SetIsWriting( bool bWrite );

	/// \brief	Read a float from the archive
	/// \param	value - the float read from the archive
	/// \param	tag - the attribute tag for the data
	virtual UINT Read( float &value, const TCHAR *tag);
	/// \brief	Read a double from the archive
	/// \param	value - the float read from the archive
	/// \param	tag - the attribute tag for the data
	virtual UINT Read( double &value, const TCHAR *tag);
	/// \brief	Read a int from the archive
	/// \param	value - the float read from the archive
	/// \param	tag - the attribute tag for the data
	virtual UINT Read( int &value, const TCHAR *tag);
	/// \brief	Read a short from the archive
	/// \param	value - the float read from the archive
	/// \param	tag - the attribute tag for the data
	virtual UINT Read( short &value, const TCHAR *tag);
	/// \brief	Read a char from the archive
	/// \param	value - the float read from the archive
	/// \param	tag - the attribute tag for the data
	virtual UINT Read( char &value, const TCHAR *tag );
	/// \brief	Read a unsigned from the archive
	/// \param	value - the float read from the archive
	/// \param	tag - the attribute tag for the data
	virtual UINT Read( unsigned &value, const TCHAR *tag);
	/// \brief	Read a bool from the archive
	/// \param	value - the float read from the archive
	/// \param	tag - the attribute tag for the data
	virtual UINT Read( bool &value, const TCHAR *tag);
	/// \brief	Read a StdString from the archive
	/// \param	value - the float read from the archive
	/// \param	tag - the attribute tag for the data
	virtual UINT Read( StdString &value, const TCHAR *tag );
	/// \brief	Read a Vec3 from the archive
	/// \param	vec - the vector to store the values read from the archive
	/// \param	tag - the attribute tag for the data
	virtual UINT Read( Vec3 &vec, const TCHAR *tag );
	/// \brief	Read a Vec4 from the archive
	/// \param	vec - the vector to store the values read from the archive
	/// \param	tag - the tag for the data
	virtual UINT Read( Vec4 &vec, const TCHAR *tag = _T(""));
	/// \brief	Read a Matrix3x3 from the archive
	/// \param	mat - THe matrix 3x3 to store the values read from the archive
	/// \param	tag - the attribute tag for the data
	virtual UINT Read( Matrix3x3 &mat, const TCHAR *tag);
	/// \brief	Read a Matrix4x4 from the archive
	/// \param	mat - THe matrix 4x4 to store the values read from the archive
	/// \param	tag - the attribute tag for the data
	virtual UINT Read( Matrix4x4 &mat, const TCHAR *tag);
	/// \brief	Read a quaternion from the archive
	/// \param	quat - The quaternion to store the values read from the archive
	/// \param	tag - the attribute tag for the data
	virtual UINT Read( Quaternion &quat, const TCHAR *tag);
	/// \brief	Read a EulerAngle from the archive
	/// \param	euler - The EulerAngle to store the values read from the archive
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( EulerAngle &euler, const TCHAR *tag );
	/// \brief	Read a binary data to the archive
	/// \param	buffer - pointer to buffer with binary data
	/// \param	size - size of data to read
	/// \param	tag - the tag for the data
	/// \return number of bytes read
	virtual UINT Read( void *buffer, size_t size, const TCHAR *tag );

	/// \brief	Write a float to the archive
	/// \param	value - the char to write to the archive
	/// \param	tag - the attribute tag for the data
	virtual void Write( float value, const TCHAR *tag);
	/// \brief	Write a double to the archive
	/// \param	value - the char to write to the archive
	/// \param	tag - the attribute tag for the data
	virtual void Write( double value, const TCHAR *tag);
	/// \brief	Write a int to the archive
	/// \param	value - the char to write to the archive
	/// \param	tag - the attribute tag for the data
	virtual void Write( int value, const TCHAR *tag );
	/// \brief	Write a short to the archive
	/// \param	value - the char to write to the archive
	/// \param	tag - the attribute tag for the data
	virtual void Write( short value, const TCHAR *tag);
	/// \brief	Write a char to the archive
	/// \param	value - the char to write to the archive
	/// \param	tag - the attribute tag for the data
	virtual void Write( char value, const TCHAR *tag);
	/// \brief	Write a unsigned to the archive
	/// \param	value - the char to write to the archive
	/// \param	tag - the attribute tag for the data
	virtual void Write( unsigned value, const TCHAR *tag );
	/// \brief	Write a bool to the archive
	/// \param	value - the char to write to the archive
	/// \param	tag - the attribute tag for the data
	virtual void Write( bool value, const TCHAR *tag );
	/// \brief	Write a const StdString to the archive
	/// \param	value - the char to write to the archive
	/// \param	tag - the attribute tag for the data
	virtual void Write( const StdString & value, const TCHAR *tag);
	/// \brief	Write a const TCHAR * to the archive
	/// \param	value - the char to write to the archive
	/// \param	tag - the attribute tag for the data
	virtual void Write( const TCHAR * value, const TCHAR *tag);
	/// \brief	Write a Vec3 to an archive.
	///			Writes three successive float attributes to the current element
	///			representing a vector 3.
	/// \param	vec - The vector to write
	/// \param	tag - the attribute tag for the data
	virtual void Write( Vec3 &vec, const TCHAR *tag);
	/// \brief	Write a Vec4 to an archive.
	///			Writes three successive float attributes to the current element
	///			representing a vector 4.
	/// \param	vec - The vector to write
	/// \param	tag - the tag for the data
	virtual void Write( Vec4 &vec, const TCHAR *tag = _T("Vec4Atribute") );
	/// \brief	Write a 3x3 matrix to the archive
	/// \param	mat - The matrix to write
	/// \param	tag - the attribute tag for the data
	virtual void Write( Matrix3x3 &mat, const TCHAR *tag);
	/// \brief	Write a 4x4 matrix to the archive
	/// \param	mat - The matrix to write
	/// \param	tag - the attribute tag for the data
	virtual void Write( Matrix4x4 &mat, const TCHAR *tag);
	/// \brief	Write a quaternion to the archive
	/// \param	quat - The quaternion to write
	/// \param	tag - the attribute tag for the data
	virtual void Write( Quaternion &quat, const TCHAR *tag);
	/// \brief	Write a EulerAngle to the archive
	/// \param	euler - The EulerAngle to write to the archive
	/// \param	tag - the tag for the data
	virtual void Write( EulerAngle &euler, const TCHAR *tag = _T("EulerAttribute"));
	/// \brief	Write a binary data to the archive
	/// \param	buffer - pointer to buffer with binary data
	/// \param	size - size of data in the buffer
	/// \param	tag - the tag for the data
	virtual void Write( const void *buffer, size_t size, const TCHAR *tag );

	/// Get number of nodes in XML file
	/// \return DWORD - number of nodes
	virtual DWORD GetNumberOfNodes() 
	{ 
		return m_EndNodeNum; 
		//return CountNodes();
	}
	/// Checks to see if we're at the end of stream
	/// \return true if eof
	virtual bool IsEndOfFile();

private:
	// FOR INPUT
	/// \brief	Pointer to the XML Text Reader
	TextReader *m_XMLReader;
	
	/// boolean for using streaming or not
	bool m_bIsStreaming;
	/// ptr to stream we were initialized with
	IEEStream *m_pStream;

	/// proxy for std:iostreams to xml parser
	CEEStdStreamProxy *m_EEStdStreamProxy;

	/// to map CEEStdStreamProxy to xmlpp::ParseInputbuffer
	xmlpp::IStreamParserInputBuffer *m_XMLParseInputBuffer;

	// FOR OUTPUT
	/// \brief	Pointer to the XML Document For Output
	Document	*m_pDocOut;
	/// \brief	Pointer to root node of XML Document For Output
	xmlpp::Element* m_pRootNode;
	/// \brief	Filename of XML Document For Output
	StdString		m_wFileOut;

	/// 
	/// \brief	Pointer to Node of XML Document
	xmlpp::Element* m_pNode;
	/// \brief	String to offset Elements
	StdString	m_szOffset;

	/// current parent name of the current element
	StdString	*m_CurParent;

	/// Current Child Node (saved off on a getparent call)
	std::queue<xmlpp::Node*> m_pChildNode;

	/// temporary storage for parent name
	StdString	m_CurParentSave;

	/// \brief	Bool Indicating Reading or Writing
	///			true = reading, false = writing
	bool		m_bReading;
	
	// FOR SCHEMA
	/// \brief	Pointer to the schema
	Schema *m_Schema;
	/// \brief	Pointer to the schema validator
	SchemaValidator *m_Validator;
	/// \brief	Pointer to the current node
	Node *m_Node;
	/// \brief	Pointer to the beginning node
	xmlpp::Node *m_BeginningNode;
	/// \brief	Pointer to the end node
	xmlpp::Node *m_EndNode;

	/// \brief	offset of current node
	DWORD m_CurrNodeNum;
	/// \brief	offset of the end node
	DWORD m_EndNodeNum;
	/// position of the current node
	int m_CurrNodePos;

	/// \brief	True if the XML file has been validated, false if not.
	///			Defaults to false
	bool m_IsValidated;
	/// \brief	The number of attributes in the current tag
	int m_NumAttributes;

	/// \brief	Holds the name of the current Element
	StdString m_szElementName;

	/// \brief	Can't use copy constructor
	CXMLArchive( const CXMLArchive& ar );
	/// \brief	Can't use assignment operator
	void operator=( const CXMLArchive& ar );

	/// \brief	Open and write the root element
	/// \param	wTag - The name of the root element
	/// \param	wPrefix - The prefix for the xmlns attribute. (I'm not sure if this is correct)
	/// \param	wUri - The URI for any further XML documents (used to store the schema location)
	/// \return	true on successfully writing the root element, false on failure
	bool OpenRootElement(const StdString &wTag, const StdString &wPrefix = _T(""), const StdString &wUri = _T(""));
	/// \brief	Write the close element tag for the root element and finalize the XML file
	void CloseRootElement( void );

	/// save off the parent name once inside an element
	void SaveParentName();

	/// \brief	Open a new element tag
	/// \param	wTag - the name of the element
	/// \return true on successfully writing the element, false on failure
	bool OpenElement(const StdString &wTag );
	/// \brief	Write the close element tag for the element
	void CloseElement( void );

	/// \brief	Reads the XML Data to a String
	/// \param  szName - the tag for the data
	/// \param  szValue - the value for the data
	/// \return TRUE if Value & Name was writte, FALSE otherwise
	bool Read( StdString &szName, StdString &szValue );

	/// Set the current Node
	/// \param node - node you want current node set to
	/// \return void
	virtual void SetNode(Node *node) 
	{ 
		m_XMLReader->set_current_element(node); 
	}

	/// Iterate to node using a starting point and an offset
	/// \param start - starting node
	/// \param offset - offset from starting node of new node
	/// \return void
	void IterateToNode(xmlpp::Node *start, DWORD offset);

	/// Count Number of nodes in archive
	/// Also sets the beginning and ending nodes
	/// \return DWORD - number of nodes
	virtual DWORD CountNodes();
	
};

}	//namespace ElementalEngine

#endif	// CXMLARCHIVE_H