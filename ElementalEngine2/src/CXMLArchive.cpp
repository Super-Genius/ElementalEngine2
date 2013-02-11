///==========================================================================
/// \file	CXMLArchive.cpp
/// \brief	Archiving using XML
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

#include "StdAfx.h"

using namespace ElementalEngine;

#include "CXMLArchive.h"
#include "EEStdStreamProxy.h"

#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <atlenc.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef _LPCBYTE_DEFINED
#define _LPCBYTE_DEFINED
typedef const BYTE *LPCBYTE;
#endif

using namespace xmlpp;
using namespace uniStr;

/// \brief	Default constructor
CXMLArchive::CXMLArchive()
{
	m_XMLReader = NULL;
	m_Schema = NULL;
	m_Validator = NULL;
	m_Node = NULL;
	m_pDocOut = NULL;
	m_IsValidated = false;
	m_bReading = true;

	m_NumAttributes = 0;
	m_EndNodeNum = 0;
	m_CurrNodeNum = 0;
	m_CurrNodePos = 0;

	m_BeginningNode = NULL;
	m_EndNode = NULL;
	m_CurParent = NULL;

	m_EEStdStreamProxy = NULL;
	m_XMLParseInputBuffer = NULL;
	m_bIsStreaming = false;
	m_pStream = NULL;
}

/// \brief	Destructor
CXMLArchive::~CXMLArchive()
{
	DeInit();
}

/// \brief	Initialize the archive
/// \param	The name of the XML file to load
bool CXMLArchive::Init( TCHAR *pFile, bool bInput )
{
	IToolBox *toolbox = EngineGetToolBox();
	// If Input comes from the file
	m_bReading = bInput;

	CREATESTREAM cs;
	cs.mode = m_bReading ? STREAM_MODE_READ : STREAM_MODE_WRITE;
	cs.streamData = pFile;
	cs.streamSize = 0;
	static DWORD msgHash_CreateStream_File = CHashString(_T("CreateStream_File")).GetUniqueID();
	DWORD retval = toolbox->SendMessage(msgHash_CreateStream_File, sizeof(CREATESTREAM), &cs);
	if ((retval != MSG_HANDLED) || (cs.openStream == NULL))
	{
		toolbox->Log( LOGERROR, _T("Error Loading XML File %s\n"), pFile );
		return false;
	}

	return Init( cs.openStream );
}

bool CXMLArchive::Init( IEEStream *pStream )
{
	m_pStream = pStream;
	m_bIsStreaming = true;
	try
	{
		delete m_EEStdStreamProxy;
		m_EEStdStreamProxy = new CEEStdStreamProxy(pStream);
	}
	catch(...)
	{
		IToolBox *toolbox = EngineGetToolBox();

		toolbox->Log(LOGERROR, _T("Unable to allocate CEESTDStreamProxy ")
			_T("in file %s.\n"), __FILE__);
		toolbox->SetErrorValue(ERR_OUT_OF_MEMORY);
		return false;
	}

	if (IsReading())
	{
		try
		{
			m_XMLParseInputBuffer = new xmlpp::IStreamParserInputBuffer(*m_EEStdStreamProxy, true);
			// Load the XML file
			m_XMLReader = new TextReader( m_XMLParseInputBuffer );
			// Read the contents of the XML file into memory
			m_XMLReader->read();
			m_XMLReader->preserve_node();
			//m_XMLReader->move_to_element();
			// Set m_Node to the root node
			//m_Node = m_XMLReader->get_current_node();


			//m_Validator = new SchemaValidator();
			//LoadSchema( _T("C:\\RedPhoenix\\Maps\\Maps_Countryside_Fast\\maps_countryside_fast.xsh") );
			//bool temp = ValidateNode();
		}
		catch ( xmlpp::internal_error err )
		{
			EngineGetToolBox()->Log( LOGERROR, _T("%s: %s\n"), _T("XML Internal Error"), err.what() );
			return false;
		}
		catch ( xmlpp::validity_error err )
		{
			EngineGetToolBox()->Log( LOGERROR, _T("%s: %s\n"), _T("XML Validity Error"), err.what() );
			return false;
		}
		catch( xmlpp::parse_error err )
		{
			EngineGetToolBox()->Log( LOGERROR, _T("%s: %s\n"), _T("XML Parse Error"), err.what() );
			return false;
		}
		catch( xmlpp::exception err )
		{
			EngineGetToolBox()->Log( LOGERROR, _T("%s: %s\n"), _T("XML Parse Error"), err.what() );
			return false;
		}
		catch (...)
		{
			EngineGetToolBox()->Log( LOGERROR, _T("%s: %s\n"), _T("XML Error"), _T("Unknown") );
			return false;
		}

		//if( m_IsValidated )
		//	MessageBox( NULL, "True!", "Schema Validator Return Value", MB_OK );
		//else
		//	MessageBox( NULL, "False!", "Schema Validator Return Value", MB_OK );
		//MessageBox( NULL, "Schema Validator Disabled", "Schema Validator Return Value", MB_OK );

		// initialize the current node number
		m_CurrNodeNum = 0;
		// initialize the end node number
		m_EndNodeNum = CountNodes();
		// initialize the number of the current node
		m_CurrNodePos = 0;
		
		return true;
	}
	// Else Output to the file
	else
	{
		m_bReading = false;
		m_pDocOut = new Document();
		m_pRootNode = NULL;
		m_pNode = NULL;
		m_szOffset = "";
		m_szElementName = "";

		return true;
	}
}

// Seek to a position in the file if possible
DWORD CXMLArchive::SeekTo(DWORD offset, int aSeek)
{
	DWORD newNode;
	xmlpp::Node *start;

	switch (aSeek)
	{
		case SEEK_SET:
			start = m_BeginningNode;
			m_CurrNodeNum = offset;
			if (offset == 0)
			{
				SetNode(start);
				return m_CurrNodeNum;
			}
			else
			{
				newNode = offset;
				if (newNode < 0 || newNode > m_EndNodeNum)
				{
					return 0;
				}	
			}
			break;
		case SEEK_CUR:
			start = m_XMLReader->get_current_element();
			m_CurrNodeNum += offset;
			newNode = offset;
			if (newNode < 0 || newNode > m_EndNodeNum)
			{
				return 0;
			}
			break;
		case SEEK_END:
			if (offset != 0)
			{
				newNode = m_EndNodeNum + offset;
				start = m_BeginningNode;
				m_CurrNodeNum = newNode;
				if (newNode < 0 || newNode > m_EndNodeNum)
				{
					return 0;
				}
			}
			else 
			{
				SetNode(m_EndNode);
				m_CurrNodeNum = m_EndNodeNum;
				return m_CurrNodeNum;
			}
			break;
		default:
			break;
	}

	IterateToNode(start, newNode);

	return m_CurrNodeNum;
}

/// \brief Iterate to node specified by offset
void CXMLArchive::IterateToNode(xmlpp::Node *start, DWORD offset)
{
	StdString szTemp;
	DWORD i;

	// set start
	m_XMLReader->set_current_element(start);

	// iterate to new node
	for (i = 0; i <= offset; ++i)
	{
		// advance node
		GetNode(szTemp);
	}
}

/// \brief	Deinitialize the archive
void CXMLArchive::DeInit()
{
	if( m_XMLReader != NULL )
	{
		delete m_XMLReader;
	}
	m_XMLReader = NULL;

	if( m_Schema != NULL )
	{
		delete m_Schema;
	}
	m_Schema = NULL;

	if( m_Validator != NULL )
	{
        delete m_Validator;
	}
	m_Validator = NULL;

	if( m_Node != NULL )
	{
        delete m_Node;
	}
	m_Node = NULL;

	if( m_pDocOut != NULL )
	{
		delete m_pDocOut;
	}
	m_pDocOut = NULL;

	delete m_XMLParseInputBuffer;
	m_XMLParseInputBuffer = NULL;

	// ORDER: m_XMLParseInputBuffer holds reference to m_EEStdStreamProxy
	// so stream should be deleted later
	delete m_EEStdStreamProxy;
	m_EEStdStreamProxy = NULL;
	m_pStream = NULL; // deleted internally by m_EEStdStreamProxy

	m_IsValidated = false;
	m_szElementName = "";
	m_bIsStreaming = false;

	m_NumAttributes = 0;
}

/// \brief	Test if the IArchive is being read
/// \return	True if the IArchive is being read, false if not
bool CXMLArchive::IsReading() const
{
	if( m_bReading )
	{
		return true;
	}
	else
	{
		return false;
	}
}

/// \brief	Test if the IArchive is being written
/// \return	True if the IArchive is being written, false if not
bool CXMLArchive::IsWriting() const
{
	if( m_bReading )
	{
		return false;
	}
	else
	{
		return true;
	}
}

/// \brief	Get the number of attributes in the tag
/// \return	The number of attributes in the node, returns 0 if there are no 
///			attributes.
int CXMLArchive::GetNumAttributes()
{
	if( m_XMLReader->get_node_type() != TextReader::Element )
	{
		// xml reader returns number if attributes for current non-NULL node 
		// instead of current element
		// so, this code sets current node value to NULL, saves counter
		// then set attribute node back as current node
		// TODO: this does not set back non-Attribute nodes
		int numAttributes = 0;
		int attributeNo = GetCurrentAttribute();
		Node *curElement = m_XMLReader->get_current_element();
		m_XMLReader->set_current_element( curElement );
		numAttributes = m_XMLReader->get_attribute_count();
		m_XMLReader->move_to_attribute( attributeNo );
		return numAttributes;
	}

	return m_XMLReader->get_attribute_count();
}

/// \brief	Get the currently selected attribute
/// \return	The number of the currently selected attribute
int CXMLArchive::GetCurrentAttribute()
{
	int index = 0;
	Node *curAttribute = m_XMLReader->get_current_node();
	Node *curElement = m_XMLReader->get_current_element();
	m_XMLReader->set_current_element( curElement );
	while( m_XMLReader->move_to_next_attribute() )
	{
		if( curAttribute == m_XMLReader->get_current_node() )
		{
			return index;
		}
		++index;
	}

	return -1;
}

/// \brief	Creates a map of attributes at the current node to their names
/// \param &attributesMap map in which to populate the attributes
void CXMLArchive::GetAttributesMap( map<StdString, StdString> &attributesMap )
{
	StdString szName;
	StdString szValue;
	bool readAttribute = true;

	while( readAttribute )
	{
		if( m_XMLReader->get_node_type() == TextReader::Attribute )
		{
			szName = m_XMLReader->get_name().c_str();
			szValue = m_XMLReader->get_value().c_str();
			attributesMap[szName] = szValue;
		}
		readAttribute = m_XMLReader->move_to_next_attribute();
	}
	m_XMLReader->move_to_first_attribute();
}

/// \brief Gets the current name of the node
/// \return The name of the current node;
StdString &CXMLArchive::GetCurrentNodeName( void )
{
	static StdString tempString;
	tempString = m_XMLReader->get_current_node()->get_name().c_str();
	return tempString;
}

/// \brief	Load a schema and set it to validate
/// \param	name - The name of the XML Schema to load
void CXMLArchive::LoadSchema( TCHAR *name )
{
	StdString filename = name;
	m_Schema = new Schema( filename );
	m_Validator->set_schema( m_Schema );
}

/// \brief	Load a schema and set it to validate
/// \param	name - The name of the XML Schema to load
void CXMLArchive::LoadSchema( StdString &name )
{
	m_Schema = new Schema( name );
	m_Validator->set_schema( m_Schema );
}

/// \brief	Validate a node with the loaded schema
/// \return	true if the schema is valid, false if it is not
bool CXMLArchive::ValidateNode()
{
	if( m_Schema != NULL )
	{
		m_IsValidated = m_Validator->validate( m_XMLReader->get_current_node() );
	}
	return m_IsValidated;
}

/// \brief	Get the name of the schema for the current node
/// \return	the name and path of the schema for the current node
StdString &CXMLArchive::GetSchema()
{
	// this assumes that the name of the schema will always be the first
	// attribute in the tag.

	int currentAttributeNo = GetCurrentAttribute();
	// using static variable for reference return value
	static StdString attributeValue;

	// move to the first attribute containing the schema data
	if (m_XMLReader->move_to_first_attribute())
	{
		attributeValue = m_XMLReader->get_value().c_str();
	}
	else
	{
		attributeValue.clear();
	}

	// return to the original attribute
	m_XMLReader->move_to_attribute( currentAttributeNo );

	return attributeValue;
}

/// \brief	Get the depth of the current node in the XML tree
/// \return	the depth of the element in the XML tree
int CXMLArchive::GetDepth()
{
	return m_XMLReader->get_depth();
}

/// \brief	Get the name of the parent element
/// \return	The name of the parent element
StdString *CXMLArchive::GetParentName()
{
	return m_CurParent;
}

/// \brief	Get the name of the parent element
/// \return	The name of the parent element
void CXMLArchive::SaveParentName()
{
	Element *parentNode;
	Attribute *attr;

	// first get the node
	parentNode = m_XMLReader->get_current_node()->get_parent_element();
	// then get attribute of name
	if (parentNode != NULL)
	{
		attr = parentNode->get_attribute(_T("Name"));
		if (attr != NULL)
		{
			m_CurParentSave = attr->get_value().c_str();
			m_CurParent = &m_CurParentSave;
		}
	}
	else
	{
		m_CurParent = NULL;
	}
}

bool CXMLArchive::GotoParentNode( StdString &szName)
{
	bool retval;
	TextReader::xmlNodeType NodeType = m_XMLReader->get_node_type();
	StdString wszTemp;
	if( (NodeType != TextReader::Element) && (NodeType != TextReader::Attribute) )
	{
		GetNode( wszTemp );
		--m_CurrNodePos;
	}	

	m_pChildNode.push( m_XMLReader->get_current_element() );
	m_XMLReader->get_parent_node();
	retval = GetNode( szName );
	--m_CurrNodePos;
	return retval;
}

bool CXMLArchive::ReturnToChildNode( StdString &szName )
{
	bool retval;
    m_XMLReader->set_current_element( m_pChildNode.front() );
	m_pChildNode.pop();
	retval = GetNode( szName );
	--m_CurrNodePos;
	return retval;
}

DWORD CXMLArchive::CountNodes()
{
	DWORD nodeCount = 0;
	StdString wszTemp;

	TextReader::xmlNodeType NodeType = m_XMLReader->get_node_type();
	if( (NodeType != TextReader::Element) && (NodeType != TextReader::Attribute) )
	{
		GetNode( wszTemp );
		++nodeCount;
	}	
	m_BeginningNode = m_XMLReader->get_current_element();
	
	GetNode( wszTemp );
	NodeType = m_XMLReader->get_node_type();

	while ((NodeType != TextReader::EndElement) && (NodeType != TextReader::None))
	{
		++nodeCount;
		m_EndNode = m_XMLReader->get_current_element();
		GetNode( wszTemp );
		NodeType = m_XMLReader->get_node_type();
	}

	m_XMLReader->set_current_element( m_BeginningNode );
	m_XMLReader->get_parent_node();

	NodeType = m_XMLReader->get_node_type();

	return nodeCount;
}

/// \brief Starts a New Class Object
/// \param wszName - name of the class
/// \return True if Success, False otherwise
bool CXMLArchive::StartClass(const StdString &wszName ) 
{
	if( m_pRootNode == NULL )
	{
		OpenRootElement( wszName );
	}
	else
	{
		OpenElement( wszName );
	}
	
	return true;
}

/// \brienf Ends a New Class Object
/// \return True if Success, False otherwise
bool CXMLArchive::EndClass( void )
{
	if( m_pNode == m_pRootNode )
	{
		CloseRootElement();
	}
	else
	{
		CloseElement();
	}
    
	return true;
}

// Move to the Next Node
bool CXMLArchive::GetNode( StdString &szName )
{
	while( 1 )
	{
        TextReader::xmlNodeType NodeType = m_XMLReader->get_node_type();
		switch( NodeType )
		{
		case TextReader::EndElement:
	        m_XMLReader->read();
			break;

		case TextReader::Element:
			SaveParentName();
			m_XMLReader->preserve_node();
			if( m_XMLReader->has_attributes() )
			{
				// Gets the Element Name and assign it to szName
				szName = m_XMLReader->get_name().c_str();
				m_XMLReader->move_to_first_attribute();
			}
			else
			{
				szName = m_XMLReader->get_name().c_str();
				m_XMLReader->read();
			}
			// increment node position 
			++m_CurrNodePos;
			return true;
			break;

		case TextReader::Attribute:
		case TextReader::Text:
			m_XMLReader->read();
			break;

		case TextReader::None:
			return false;
			break;

		case TextReader::SignificantWhitespace:
			m_XMLReader->read();
			break;

		default:
			break;
		}
	}

	return false;
}

bool CXMLArchive::Read( StdString &szName, StdString &szValue )
{
	
	if( m_XMLReader->get_node_type() != TextReader::Attribute )
	{
		StdString szTemp;
		do
		{
			if (!GetNode( szTemp ))
				break;
		} while( (m_XMLReader->get_node_type() != TextReader::Attribute) );	
	}

	// make sure we have an attribute now
	if (m_XMLReader->get_node_type() != TextReader::Attribute)
	{
		szValue = "";
		return false;
	}

	szName = m_XMLReader->get_name().c_str();
	szValue = m_XMLReader->get_value().c_str();
	if( !m_XMLReader->move_to_next_attribute() )
	{
		m_XMLReader->move_to_element();
		m_XMLReader->read();
		m_szElementName = "";
	}
	return true;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
UINT CXMLArchive::Read( float &value, const TCHAR *tag )
{
	StdString attributeName;
	StdString attributeValue;
	UINT bytesRead;

	bytesRead = Read( attributeName, attributeValue );
	value = (float)atof(attributeValue.c_str());
	return bytesRead;
}

/// \brief	Read a float from the archive 
/// \param	value - the float read from the archive
UINT CXMLArchive::Read( double &value, const TCHAR *tag )
{
	StdString attributeName;
	StdString attributeValue;
	UINT bytesRead;

	bytesRead = Read( attributeName, attributeValue );	
	value = atof(attributeValue.c_str());
	return bytesRead;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
UINT CXMLArchive::Read( int &value, const TCHAR *tag )
{
    StdString attributeName;
	StdString attributeValue;
	UINT bytesRead;

	bytesRead = Read( attributeName, attributeValue );
	value = atoi(attributeValue.c_str());
	return bytesRead;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
UINT CXMLArchive::Read( short &value, const TCHAR *tag )
{
    StdString attributeName;
	StdString attributeValue;
	UINT bytesRead;

	bytesRead = Read( attributeName, attributeValue );
	value = atoi(attributeValue.c_str());
	return bytesRead;
}

/// \brief	Read a float from the archive
/// \param	value - the float read from the archive
UINT CXMLArchive::Read( char &value, const TCHAR *tag )
{
    StdString attributeName;
	StdString attributeValue;
	UINT bytesRead;

	bytesRead = Read( attributeName, attributeValue );
	value = *attributeValue.c_str();
	return bytesRead;
}

/// \brief	Read a unsigned from the archive
/// \param	value - the unsigned read from the archive
UINT CXMLArchive::Read( unsigned &value, const TCHAR *tag )
{
	StdString attributeName;
	StdString attributeValue;
	UINT bytesRead;

	bytesRead = Read( attributeName, attributeValue );
	value = atoi(attributeValue.c_str());
	return bytesRead;
}

/// \brief	Read a bool from the archive
/// \param	value - the bool read from the archive
UINT CXMLArchive::Read( bool &value, const TCHAR *tag )
{
	StdString attributeName;
	StdString attributeValue;
	UINT bytesRead;

	bytesRead = Read( attributeName, attributeValue );
	value = false;
	if( _tcsicmp(attributeValue.c_str(), _T("true"))  == 0 )
	{
		value = true;
	}
	return bytesRead;
}

/// \brief	Read a StdString from the archive
/// \param	value - the StdString read from the archive
UINT CXMLArchive::Read( StdString &value, const TCHAR *tag )
{
	StdString attributeName;
	StdString attributeValue;
	UINT bytesRead;

	bytesRead = Read( attributeName, attributeValue );
	value = attributeValue;
	return bytesRead;
}

/// \brief	Read a Vec3 from the archive
/// \param	vec - the vector to store the values read from the arcihve
/// \param	tag - the tag for the data
UINT CXMLArchive::Read( Vec3 &vec, const TCHAR *tag )
{
	StdString attributeValue;
	StdString attributeName;
	UINT bytesRead;

	bytesRead = Read( attributeName, attributeValue );
	vec.x = (float)atof( attributeValue.c_str() );
	
    bytesRead += Read( attributeName, attributeValue );
	vec.y = (float)atof( attributeValue.c_str() );
	
	bytesRead += Read( attributeName, attributeValue );
	vec.z = (float)atof( attributeValue.c_str() );
	return bytesRead;
}

/// \brief	Read a Vec4 from the archive
/// \param	vec - the vector to store the values read from the archive
/// \param	tag - the tag for the data
UINT CXMLArchive::Read( Vec4 &vec, const TCHAR *tag )
{
	StdString attributeValue;
	StdString attributeName;
	UINT bytesRead;

	bytesRead = Read( attributeName, attributeValue );
	vec.x = (float)atof( attributeValue.c_str() );
	
    bytesRead += Read( attributeName, attributeValue );
	vec.y = (float)atof( attributeValue.c_str() );
	
	bytesRead += Read( attributeName, attributeValue );
	vec.z = (float)atof( attributeValue.c_str() );

	bytesRead += Read( attributeName, attributeValue );
	vec.w = (float)atof( attributeValue.c_str() );
	return bytesRead;
}

/// \brief	Read a Matrix3x3 from the archive
/// \param	mat - The matrix 3x3 to store the values read from the archive
/// \param	tag - the tag for the data
UINT CXMLArchive::Read( Matrix3x3 &mat, const TCHAR *tag )
{
	StdString attributeValue;
	StdString attributeName;
	UINT bytesRead = 0;

	for( int i=0; i<9; i++ )
	{
		bytesRead += Read( attributeName, attributeValue );
		mat.m[i] = ( (float)atof(attributeValue.c_str()) );
	}
	return bytesRead;
}

/// \brief	Read a Matrix4x4 from the archive
/// \param	mat - The matrix 4x4 to store the values read from the archive
/// \param	tag - the tag for the data
UINT CXMLArchive::Read( Matrix4x4 &mat, const TCHAR *tag )
{
	StdString attributeValue;
	StdString attributeName;
	UINT bytesRead=0;

	for( int i=0; i<16; i++ )
	{
		bytesRead += Read( attributeName, attributeValue );
		mat.m[i] = ( (float)atof(attributeValue.c_str()) );
	}
	return bytesRead;
}

/// \brief	Read a quaternion from the archive
/// \param	quat - The quaternion to store the values read from the archive
/// \param	tag - the tag for the data
UINT CXMLArchive::Read( Quaternion &quat, const TCHAR *tag )
{
	StdString attributeValue;
	StdString attributeName;
	UINT bytesRead;

	bytesRead = Read( attributeName, attributeValue );
	quat.x = (float)atof(attributeValue.c_str());
	bytesRead += Read( attributeName, attributeValue );
	quat.y = (float)atof(attributeValue.c_str());
	bytesRead += Read( attributeName, attributeValue );
	quat.z = (float)atof(attributeValue.c_str());
	bytesRead += Read( attributeName, attributeValue );
	quat.w = (float)atof(attributeValue.c_str());
	// bnb - renormalize the quat after reading from an xml because writing quat as a string
	// rounds too much.
	quat.Normalize();
	return bytesRead;
}

/// \brief	Read a quaternion from the archive
/// \param	quat - The quaternion to store the values read from the archive
/// \param	tag - the tag for the data
UINT CXMLArchive::Read( EulerAngle &euler, const TCHAR *tag )
{
	StdString attributeValue;
	StdString attributeName;
	UINT bytesRead;

	bytesRead = Read( attributeName, attributeValue );
	euler.roll = (float)atof(attributeValue.c_str());
	bytesRead += Read( attributeName, attributeValue );
	euler.pitch = (float)atof(attributeValue.c_str());
	bytesRead += Read( attributeName, attributeValue );
	euler.yaw = (float)atof(attributeValue.c_str());
	bytesRead += Read( attributeName, attributeValue );
	euler.order = (EULER_ORDER)atoi(attributeValue.c_str());
	return bytesRead;
}

/// \brief	Read a binary data to the archive
/// \param	buffer - pointer to buffer with binary data
/// \param	size - size of data to read
/// \param	tag - the tag for the data
/// \return number of bytes read
UINT CXMLArchive::Read( void *buffer, size_t size, const TCHAR *tag )
{
#ifdef WIN32
	StdString attributeValue;
	StdString attributeName;
	UINT bytesRead = Read( attributeName, attributeValue );
	int nLen = ATL::AtlHexDecodeGetRequiredLength((int)attributeValue.size());
	if (nLen > (int)size)
	{
		nLen = (int)size;
	}

	ATL::AtlHexDecode(attributeValue.data(), (int)attributeValue.size(), static_cast<LPBYTE>(buffer), &nLen);

	return nLen;
#else
	EngineGetToolBox()->Log( LOGFATALERROR, _T("Not yet implemented!") );
	return 0;
#endif
}

/// \brief	Write the header for the XML document
/// \param	wVersion - The XML version. Defaults to "<?xml version="1.0"?>"
/// \param	wEncode - The character encoding of the XML document
/// \return	true on successfully writing the header, false on failure
bool CXMLArchive::CreateHeader(const StdString &szVersion, const StdString &szEncode )
{
	return true;
}

void CXMLArchive::SetIsWriting( bool bWrite )
{
	if(bWrite && m_bReading)
	{
		m_bReading = false;
//		if (m_pStream)
//			m_pStream->SeekTo( 0 );
	}
	else if (!bWrite && !m_bReading)
	{
		m_bReading = true;
		try
		{
			if (!m_EEStdStreamProxy || !m_pStream)
			{
				EngineGetToolBox()->Log(LOGERROR, _T("XMLArchive set to writing without a stream."));
				return;
			}
			m_pStream->SeekTo( 0 );
			if (!m_XMLParseInputBuffer)
			{
				m_XMLParseInputBuffer = new xmlpp::IStreamParserInputBuffer(*m_EEStdStreamProxy, true);
			}
			// Load the XML file
			if (!m_XMLReader)
			{
				m_XMLReader = new TextReader( m_XMLParseInputBuffer );
				// Read the contents of the XML file into memory
				m_XMLReader->read();
				m_XMLReader->preserve_node();
				//m_XMLReader->move_to_element();
				// Set m_Node to the root node
				//m_Node = m_XMLReader->get_current_node();


				//m_Validator = new SchemaValidator();
				//LoadSchema( _T("C:\\RedPhoenix\\Maps\\Maps_Countryside_Fast\\maps_countryside_fast.xsh") );
				//bool temp = ValidateNode();

				//if( m_IsValidated )
				//	MessageBox( NULL, "True!", "Schema Validator Return Value", MB_OK );
				//else
				//	MessageBox( NULL, "False!", "Schema Validator Return Value", MB_OK );
				//MessageBox( NULL, "Schema Validator Disabled", "Schema Validator Return Value", MB_OK );

				// initialize the current node number
				m_CurrNodeNum = 0;
				// initialize the end node number
				m_EndNodeNum = CountNodes();
				// initialize the number of the current node
				m_CurrNodePos = 0;
			}
		}
		catch ( xmlpp::internal_error err )
		{
			EngineGetToolBox()->Log( LOGERROR, _T("%s: %s\n"), _T("XML Internal Error"), err.what() );
			return;
		}
		catch ( xmlpp::validity_error err )
		{
			EngineGetToolBox()->Log( LOGERROR, _T("%s: %s\n"), _T("XML Validity Error"), err.what() );
			return;
		}
		catch( xmlpp::parse_error err )
		{
			EngineGetToolBox()->Log( LOGERROR, _T("%s: %s\n"), _T("XML Parse Error"), err.what() );
			return;
		}
		catch( xmlpp::exception err )
		{
			EngineGetToolBox()->Log( LOGERROR, _T("%s: %s\n"), _T("XML Parse Error"), err.what() );
			return;
		}
		catch (...)
		{
			EngineGetToolBox()->Log( LOGERROR, _T("%s: %s\n"), _T("XML Internal Error"), _T("Unknown Error.") );
			return;
		}
	}
}

/// \brief	Open and write the root element
/// \param	wTag - The name of the root element
/// \param	wPrefix - The prefix for the xmlns attribute. (I'm not sure if this is correct)
/// \param	wUri - The URI for any further XML documents (used to store the schema location)
/// \return	true on successfully writing the root element, false on failure
bool CXMLArchive::OpenRootElement(const StdString &wTag, const StdString &wPrefix, const StdString &wUri )
{

	m_pRootNode = m_pDocOut->create_root_node( wTag, wUri, wPrefix );
	m_pNode = m_pRootNode;
	m_szOffset += "\n";

	if( m_pRootNode == NULL )
		return false;

	return true;
}

/// \brief	Write the close element tag for the root element and finalize the XML file
void CXMLArchive::CloseRootElement( void )
{
	m_pNode->add_child_text(m_szOffset);
	m_szOffset = "";
	if (m_bIsStreaming)
	{
		m_pDocOut->write_to_stream(*m_EEStdStreamProxy );
		m_EEStdStreamProxy->sync();
		delete m_XMLParseInputBuffer;
		m_XMLParseInputBuffer = NULL;
	}
	else
	{
		m_pDocOut->write_to_file(m_wFileOut);
	}

	delete m_pDocOut;
	m_pDocOut = NULL;
	m_pRootNode = NULL;
	m_pNode = NULL;	
}

/// \brief	Open a new element tag
/// \param	wTag - the name of the element
/// \return true on successfully writing the element, false on failure
bool CXMLArchive::OpenElement(const StdString &wTag )
{
	if( m_pNode == NULL )
	{
		return false;
	}
	m_szOffset += "\t";
	m_pNode->add_child_text(m_szOffset);

    m_pNode = m_pNode->add_child(wTag);
	if( m_pNode == NULL )
	{
		m_pNode->add_child_text(m_szOffset);
		m_szOffset.resize( m_szOffset.length() - 1 );
		return false;
	}   
	return true;
}

/// \brief	Write the close element tag for the element
void CXMLArchive::CloseElement( void )
{
	if( m_pNode != m_pRootNode )
	{
		m_pNode->add_child_text(m_szOffset);
		m_szOffset.resize( m_szOffset.length() - 1 );
		m_pNode = m_pNode->get_parent_element();
	}
}

/// \brief	Write a float to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CXMLArchive::Write( float value, const TCHAR *tag )
{
	if( m_pNode == NULL )
	{
		return;
	}

	char szValue[32];
	sprintf( szValue, "%f", value );
	m_pNode->set_attribute( tag, szValue );
}

/// \brief	Write a double to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CXMLArchive::Write( double value, const TCHAR *tag )
{
	if( m_pNode == NULL )
	{
		return;
	}
	char szValue[32];
	sprintf( szValue, "%f", value );
	m_pNode->set_attribute( tag, szValue );
}

/// \brief	Write a int to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CXMLArchive::Write( int value, const TCHAR *tag )
{
	if( m_pNode == NULL )
	{
		return;
	}
	char szValue[32];
	sprintf( szValue, "%d", value );
	m_pNode->set_attribute( tag, szValue );
}

/// \brief	Write a short to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CXMLArchive::Write( short value, const TCHAR *tag )
{
	if( m_pNode == NULL )
	{
		return;
	}
	char szValue[32];
	sprintf( szValue, "%d", value );
	m_pNode->set_attribute( tag, szValue );
}

/// \brief	Write a char to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CXMLArchive::Write( char value, const TCHAR *tag )
{
	if( m_pNode == NULL )
	{
		return;
	}
	StdString szValue(_T(""));
	szValue += value;
	m_pNode->set_attribute( tag, szValue );
}

/// \brief	Write a unsigned to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CXMLArchive::Write( unsigned value, const TCHAR *tag )
{
	if( m_pNode == NULL )
	{
		return;
	}
	char szValue[32];
	sprintf( szValue, "%d", value );
	m_pNode->set_attribute( tag, szValue );
}

/// \brief	Write a boolean to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CXMLArchive::Write( bool value, const TCHAR *tag )
{
	if( m_pNode == NULL )
	{
		return;
	}
	StdString szValue;
	if( value )
	{
        szValue = _T("true");
	}
	else
	{
		szValue = _T("false");
	}
	m_pNode->set_attribute( tag, szValue );
}

/// \brief	Write a const StdString to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CXMLArchive::Write( const StdString & value, const TCHAR *tag )
{
	if( m_pNode == NULL )
	{
		return;
	}

	m_pNode->set_attribute( tag, value );
}

/// \brief	Write a TCHAR pointer to the archive
/// \param	tag - the tag to for the data
/// \param	value - the char to write to the archive
void CXMLArchive::Write( const TCHAR * value, const TCHAR *tag )
{
	if( m_pNode == NULL )
	{
		return;
	}


	// this library should be compiled as UNICODE also
	m_pNode->set_attribute( tag, value );
}

/// \brief	Write a Vec3 to an archive.
///			Writes three succesive float attibutes to the current element
///			representing a vector 3.
/// \param	vec - The vector to write
/// \param	tag - the tag for the data
void CXMLArchive::Write( Vec3 &vec, const TCHAR *tag )
{
	StdString xName = _T("X");
	StdString yName = _T("Y");
	StdString zName = _T("Z");
	StdString tagName = tag + xName;

	// write x
	Write( vec.x, tagName );

	// write y
	tagName = tag + yName;
	Write( vec.y, tagName );

	// write z
	tagName = tag + zName;
	Write( vec.z, tagName );	
}

/// \brief	Write a Vec4 to an archive.
///			Writes three succesive float attibutes to the current element
///			representing a vector 4.
/// \param	vec - The vector to write
/// \param	tag - the tag for the data
void CXMLArchive::Write( Vec4 &vec, const TCHAR *tag )
{
	StdString xName = _T("X");
	StdString yName = _T("Y");
	StdString zName = _T("Z");
	StdString wName = _T("W");
	StdString tagName = tag + xName;

	// write x
	Write( vec.x, tagName );

	// write y
	tagName = tag + yName;
	Write( vec.y, tagName );

	// write z
	tagName = tag + zName;
	Write( vec.z, tagName );	

	// write w
	tagName = tag + wName;
	Write( vec.w, tagName );	
}

/// \brief	Write a 3x3 matrix to the archive
/// \param	mat - The matrix to write
/// \param	tag - The tag for the data
void CXMLArchive::Write( Matrix3x3 &mat, const TCHAR *tag )
{
	float val;

	// write the values
	val = mat.m[0];
	Write( val, _T("m0") );
	val = mat.m[1];
	Write( val, _T("m1") );
	val = mat.m[2];
	Write( val, _T("m2") );
	val = mat.m[3];
	Write( val, _T("m3") );
	val = mat.m[4];
	Write( val, _T("m4") );
	val = mat.m[5];
	Write( val, _T("m5") );
	val = mat.m[6];
	Write( val, _T("m6") );
	val = mat.m[7];
	Write( val, _T("m7") );
	val = mat.m[8];
	Write( val, _T("m8") );
}

/// \brief	Write a 4x4 matrix to the archive
/// \param	mat - The matrix to write
/// \param	tag - The tag for the data
void CXMLArchive::Write( Matrix4x4 &mat, const TCHAR *tag )
{
	float val;

	// write the values
	val = mat.m[0];
	Write( val, _T("m0") );
	val = mat.m[1];
	Write( val, _T("m1") );
	val = mat.m[2];
	Write( val, _T("m2") );
	val = mat.m[3];
	Write( val, _T("m3") );
	val = mat.m[4];
	Write( val, _T("m4") );
	val = mat.m[5];
	Write( val, _T("m5") );
	val = mat.m[6];
	Write( val, _T("m6") );
	val = mat.m[7];
	Write( val, _T("m7") );
	val = mat.m[8];
	Write( val, _T("m8") );
	val = mat.m[9];
	Write( val, _T("m9") );
	val = mat.m[10];
	Write( val, _T("m10") );
	val = mat.m[11];
	Write( val, _T("m11") );
	val = mat.m[12];
	Write( val, _T("m12") );
	val = mat.m[13];
	Write( val, _T("m13") );
	val = mat.m[14];
	Write( val, _T("m14") );
	val = mat.m[15];
	Write( val, _T("m15") );
}

/// \brief	Write a quaternion to the archive
/// \param	quat - The quaternion to write
/// \param	tag - The tag for the data
void CXMLArchive::Write( Quaternion &quat, const TCHAR *tag )
{
	StdString xName = _T("X");
	StdString yName = _T("Y");
	StdString zName = _T("Z");
	StdString wName = _T("W");
	StdString tagName = tag + xName;

	// write x
	Write( quat.x, tagName );

	// write y
	tagName = tag + yName;
	Write( quat.y, tagName );

	// write z
	tagName = tag + zName;
	Write( quat.z, tagName );

	// write w
	tagName = tag + wName;
	Write( quat.w, tagName );
}

/// \brief	Write a EulerAngle to the archive
/// \param	quat - The EulerAngle to write
/// \param	tag - The tag for the data
void CXMLArchive::Write( EulerAngle &euler, const TCHAR *tag )
{
	StdString xName = _T("Roll");
	StdString yName = _T("Pitch");
	StdString zName = _T("Yaw");
	StdString wName = _T("Order");
	StdString tagName = tag + xName;

	// write x
	Write( euler.roll, tagName );

	// write y
	tagName = tag + yName;
	Write( euler.pitch, tagName );

	// write z
	tagName = tag + zName;
	Write( euler.yaw, tagName );

	// write w
	tagName = tag + wName;
	Write( (int)euler.order, tagName );
}

/// \brief	Write a binary data to the archive
/// \param	buffer - pointer to buffer with binary data
/// \param	size - size of data in the buffer
/// \param	tag - the tag for the data
void CXMLArchive::Write( const void *buffer, size_t size, const TCHAR *tag )
{
#ifdef WIN32
	int nLen = ATL::AtlHexEncodeGetRequiredLength((int)size);
	string value(nLen, '\0');
	ATL::AtlHexEncode(static_cast<LPCBYTE>(buffer), (int)size, const_cast<LPTSTR>(value.data()), &nLen);
	Write( value, tag );
#else
	EngineGetToolBox()->Log( LOGFATALERROR, _T("Not yet implemented!") );
#endif
}

int CXMLArchive::GetCurrentNodePosition()
{
	// return the number of the current node
	return m_CurrNodePos;
}

bool CXMLArchive::IsEndOfFile()
{
	if (m_CurrNodePos == m_EndNodeNum)
	{
		return true;
	}
	return false;
}
