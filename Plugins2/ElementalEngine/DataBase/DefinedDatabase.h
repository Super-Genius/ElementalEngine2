///============================================================================
/// \file		DefinedDatabase.h
/// \brief		Declaration of CDefinedDatabase class, which is a database 
///				whose serialization depends on the schema
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

#ifndef __DEFINEDDATABASE_H__
#define __DEFINEDDATABASE_H__

class CDatabaseItem;
typedef vector<CDatabaseItem*> DATABASEENTRYVEC;

class CDefinedDatabase : public CObjectTemplate<IObject>
{
public:
	// Default constructor
	CDefinedDatabase( IHashString *parentName, IHashString *name );

	// Default destructor
	virtual ~CDefinedDatabase();

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

	///	\return A pointer to the schema correlating to the database
	CDatabaseSchema *GetSchema();

	/// \return pointer to the schema filename hash string
	IHashString *GetSchemaFile();

	/// Returns the database item at the given index. If index is out of bounds, return NULL.
	CDatabaseItem *GetDatabaseItem( int index );

	int GetNumItems();

	/// Returns the database item index given the value of a key parameter.
	int GetDatabaseItemIndex( IHashString *keyAttributeName, DATABASEDATA *keyAttributeValue );

	/// \brief Adds an entry to the order list
	void RegisterDatabaseItem( CDatabaseItem *databaseItem );

	/// \brief Removes an entry from the ordered list
	void DeregisterDatabaseItem( CDatabaseItem *databaseItem );

	/// Message handler for mapping object to file name
	DWORD OnMapToFileName( DWORD size, void *params );

	/// Gets the specified attribute of an item at index
	DWORD OnGetAttribute( DWORD size, void *params );

	/// Gets the specified attribute's type of an item at index
	DWORD OnGetAttributeType( DWORD size, void *params );

	/// Gets general information about the database
	DWORD OnGetDatabaseInfo( DWORD size, void *params );

	/// Sets the specified attribute of an item at index
	DWORD OnSetAttribute( DWORD size, void *params );

	/// Gets the index of the key value
	DWORD OnGetIndex( DWORD size, void *params );

private:
	/// \brief	Sets the m_Schema to its corresponding pointer.
	void InitializeSchema();

	IToolBox					*m_ToolBox;
	float						m_fVersion;
	CHashString					m_hszSchemaFile;
	CHashString					m_hszSchema;
	int							m_iNumItems;
	CDatabaseSchema				*m_Schema;
	DATABASEENTRYVEC			m_DatabaseEntryVec;
};

#endif //#ifndef __DEFINEDDATABASE_H__