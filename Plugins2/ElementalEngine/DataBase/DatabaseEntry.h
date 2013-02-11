///============================================================================
/// \file		DatabaseEntry.h
/// \brief		Declaration of CDatabaseEntry class. This class serves as 
///				an anchor to databases from a hierarchical object.
/// \date		04-18-2007
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

#ifndef __DATABASEENTRY_H__
#define __DATABASEENTRY_H__

class CDatabaseEntry : public CObjectTemplate<IObject>
{
public:
	// Default constructor
	CDatabaseEntry( IHashString *parentName, IHashString *name );

	// Default destructor
	virtual ~CDatabaseEntry();

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

	/// Loads the schema and creates a database item into the defined database corresponding to the entry
	void RegisterAsDatabaseItem();

	/// Fetches the entry's value of the specified attribute.
	DWORD OnGetAttribute( DWORD size, void *params );

	/// Fetches the entry's value type of the specified attribute.
	DWORD OnGetAttributeType( DWORD size, void *params );

	/// Sets the entry's value of the specified attribute.
	DWORD OnSetAttribute( DWORD size, void *params );

	/// Gets the properties schema for editor view
	DWORD OnGetSchema( DWORD size, void *params );

private:
	IObject *GetMapObject(  IHashString *objectName, IHashString *objectType  );
	CDefinedDatabase *GetDefinedDatabase();
	CDatabaseItem *GetDatabaseItem();
	CDatabaseSchema *GetSchema();

	IToolBox					*m_ToolBox;
	CHashString					m_hszSchemaFile;
	CHashString					m_hszSchema;
	CHashString					m_hszDatabase;
	CHashString					m_hszDatabaseItem;

	CDefinedDatabase			*m_Database;
	CDatabaseItem				*m_DatabaseItem;
	CDatabaseSchema				*m_Schema;

	CHashString					m_hszDefinedDatabaseType;
	CHashString					m_hszDatabaseItemType;
	CHashString					m_hszDatabaseSchemaType;
};

#endif //#ifndef __DATABASEENTRY_H__