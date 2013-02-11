///============================================================================
/// \file		DatabaseSchema.h
/// \brief		Declaration of CDatabaseSchema class which describes the
///				attributes of a database item object
/// \date		04-12-2007
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

#ifndef __DATABASESCHEMA_H__
#define __DATABASESCHEMA_H__

class CSchemaItem;
typedef list<CSchemaItem*> SCHEMAATTRIBUTELIST;

class CDatabaseSchema : public CObjectTemplate<IObject>
{
public:
	// Default constructor
	CDatabaseSchema( IHashString *parentName, IHashString *name );

	// Default destructor
	virtual ~CDatabaseSchema();

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

	/// Serialize a database item's values to the archive.
	void SerializeDatabaseItem( ATTRIBUTEIDTODATAMAP &valuesMap, IArchive &ar );

	/// Adds a schema item to the ordered list of attributes.
	void RegisterSchemaItem( CSchemaItem *schemaItem );

	/// Removes a schema item to the ordered list of attributes.
	void DeregisterSchemaItem( CSchemaItem *schemaItem );

	/// Returns the number of attributes specified by the schema.
	int GetNumItems();

	/// \fn		CDatabaseSchema GetSchemaItem()
	/// \brief	Returns a pointer to the specified schema item
	CSchemaItem *GetSchemaItem( IHashString *name );

	/// Initializes values to their defaults if they have not already been assigned
	void InitializeValues( ATTRIBUTEIDTODATAMAP *valuesMap );

	/// Returns true of the attribute name is a key attribute of the schema.
	bool IsKey( IHashString *name );

	DWORD OnMapToFileName( DWORD size, void *params );

	DWORD OnGetSchema( DWORD size, void *params );

	/// \brief Gets a Schema Archive.
	/// \return bool - true success, false failure.
	bool GetSchemaArchive( IArchive * archive );

private:
	IToolBox					*m_ToolBox;
	float						m_fVersion;
	int							m_iNumItems;
	SCHEMAATTRIBUTELIST			m_SchemaAttributeList;
	IDTOOBJECTMAP				m_SchemaAttributeMap;
};

#endif //#ifndef __DATABASESCHEMA_H__