///============================================================================
/// \file		DatabaseItem.h
/// \brief		Declaration of CDatabaseItem class which describes the 
///				attributes of a database item object
/// \date		04-13-2007
/// \author		Richard Nguyen
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

#ifndef __DATABASEITEM_H__
#define __DATABASEITEM_H__

class CDatabaseItem : public CObjectTemplate<IObject>
{
public:
	// Default constructor
	CDatabaseItem( IHashString *parentName, IHashString *name );

	// Default destructor
	virtual ~CDatabaseItem();

	/// \fn		virtual void Init()
	/// \brief	Initialize the object
	void Init();

	/// \fn		virtual void Update()
	/// \brief	Update the object
	bool Update();

	/// \fn		virtual void DeInit()
	/// \brief	Deinitialize the object
	void DeInit();

	static IComponent *Create( int nArgs, va_list argptr );

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	void Serialize( IArchive &ar );

	ATTRIBUTEIDTODATAMAP *GetValuesMap();

	/// \brief Returns the DB value of a item line with a certain name.
	/// \return Returns the value struct for the item value.
	DATABASEDATA * GetDataByName( IHashString * phszName );

	/// Writes the value of the specified attribute name into the params archive.
	DWORD GetAttribute( DATABASEATTRIBUTEPARAMS *databaseAttributeParams );

	/// Writes the class type of the specified attribute name into the hash string.
	void GetClassType( IHashString *attributeName, IHashString *classType );

	/// Writes the value type of the specified attribute name into the params archive.
	DWORD GetAttributeType( DATABASEATTRIBUTEPARAMS *databaseAttributeParams );

	/// Writes the value type of the specified attribute name into the hash string.
	void GetAttributeType( IHashString *attributeName, IHashString *attributeType );

	/// Reads the value of the specified attribute name from the params archive.
	DWORD SetAttribute( DATABASEATTRIBUTEPARAMS *databaseAttributeParams );

	bool AttributeEquals( IHashString *attributeName, DATABASEDATA *attributeValue );

private:
	/// \fn		CDatabaseSchema GetSchema()
	/// \brief	Returns a pointer to the schema specified by the parent CDefinedDatabase
	CDatabaseSchema *GetSchema();

	/// \fn		CDatabaseSchema GetSchemaItem()
	/// \brief	Returns a pointer to the specified schema item
	CSchemaItem *GetSchemaItem( IHashString *name );

	IToolBox					*m_ToolBox;
	ATTRIBUTEIDTODATAMAP		m_Values;
	CDefinedDatabase			*m_Database;
};

#endif //#ifndef __DATABASEITEM_H__