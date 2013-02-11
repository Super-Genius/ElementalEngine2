///============================================================================
/// \file		DatabaseManager.h
/// \brief		Declaration of CDatabaseManager Class
/// \date		08-02-2005
/// \author		Kenneth Hurley
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

#ifndef __DATABASEMANAGER_H__
#define __DATABASEMANAGER_H__

typedef map<DWORD, IObject *> DATABASETOOBJECTMAP;
/// file name, database info
typedef map<DWORD, DATABASEINFO> FILENAMETODATABASEMAP;

class CDatabaseSchema;
class CDefinedDatabase;

class CDatabaseManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CDatabaseManager );

	/// \brief	Default Destructor
	///			Since CDatabaseManager is implemented as a singleton the
	///			constructor is private.
	CDatabaseManager();

public:

	/// Default Destructor
	virtual ~CDatabaseManager( );

	void RegisterLuaDefinitions();

	void MapObjectToFileName( IHashString *hszFileName, IObject *object );

	CDatabaseSchema *GetSchemaByFile( IHashString *hszSchemaFile );
	IObject* GetDatabaseByFile( IHashString *hszFile );

	/// Function to get this component.
	///			Returns the singleton instance in this case
	static IComponent *Create(int nArgs, va_list argptr);

	///	DeInitializes the CDatabaseManager
	/// \return	True on success, False on failure
	virtual bool DeInit();

	/// message function handlers

	/// adds a database to the manager
	/// \param size = sizeof DATABASEINFO structure
	/// \param params = pointer to DATABASEINFO structure
	/// \return MSG_XXX return value
	DWORD OnAddDatabase(DWORD size, void *params);

	/// removes a database from the manager
	/// \param size = sizeof DATABASEINFO structure
	/// \param params = pointer to DATABASEINFO structure
	/// \return MSG_XXX return value
	DWORD OnRemoveDatabase(DWORD size, void *params);

	/// fills in DATABASEINFO.nItems and DATABASEINFO.data
	/// for a given filename
	/// \param size = sizeof DATABASEINFO structure
	/// \param params = pointer to DATABASEINFO structure
	/// \return MSG_XXX return value
	DWORD OnGetDatabase(DWORD size, void *params);

	/// Takes in a TYPEDATABASEINFO struct that contains
	/// a filename to a CDefinedDatabase struct. If you send
	/// it another DB type it exits with a MSG_ERROR.
	/// If no error, fills in a passed archive with name, type, data
	/// for that archive.
	/// \param size = sizeof TYPEDATABASEINFO structure
	/// \param params = pointer to TYPEDATABASEINFO structure
	/// \return MSG_XXX return value
	DWORD OnGetTypedDatabase(DWORD size, void *params);

	/// gets  DATABASEINFO.nItems and DATABASEINFO.data
	/// given a specific filename
	/// \param size = sizeof DATABASEINFO structure
	/// \param params = pointer to DATABASEINFO structure
	/// \return MSG_XXX return value
	DWORD OnSetDatabase(DWORD size, void *params);

	/// Find a Database that was previous created  by filename
	/// \param size = sizeof DATABASEINFO structure
	/// \param params = pointer to DATABASEINFO structure
	/// \return MSG_XXX return value
	DWORD OnFindDatabaseByFileName(DWORD size, void *params);

	/// Create a global database object
	/// \param size = sizeof(DBCREATEPARAMS)
	/// \param params = DBCREATEPARAMS point to create database parameters
	DWORD OnCreateGlobalDBObject(DWORD size, void *params);

	/// Create a global data set of attributes
	/// \param size = sizeof(LOADGLOBALATTRIBUTESPARAMS)
	/// \param params = LOADGLOBALATTRIBUTESPARAMS point to create attributes parameters
	DWORD OnLoadGlobalAttributes( DWORD size, void *params );

	/// Gets the specified global attribute and writes it into the archive
	/// \param size = sizeof(DATABASEATTRIBUTEPARAMS)
	/// \param params = DATABASEATTRIBUTEPARAMS point to create attributes parameters
	DWORD OnGetGlobalAttribute( DWORD size, void *params );

	/// Reads the value from the archive and set it to the specified global attribute
	/// \param size = sizeof(DATABASEATTRIBUTEPARAMS)
	/// \param params = DATABASEATTRIBUTEPARAMS point to create attributes parameters
	DWORD OnSetGlobalAttribute( DWORD size, void *params );

	/// Gets the specified attribute's type
	/// \param size = sizeof(DATABASEATTRIBUTEPARAMS)
	/// \param params = DATABASEATTRIBUTEPARAMS point to create attributes parameters
	DWORD OnGetGlobalAttributeType( DWORD size, void *params );

	/// Performs extra steps on the manager side before a defined database is removed
	void OnDatabaseDeInit( CDefinedDatabase *definedDatabase );

	virtual bool DeleteObject( IObject* Object );

private:
	///	free all memory allocations for database
	///	\param	dbInfo - pointer to database information
	void ReleaseDatabase(DATABASEINFO *dbInfo);

private:
	// Map of Archive File Paths to Archive Names.
	FILENAMETODATABASEMAP m_FilepathToDBInfo;
	// Map of Archive File Paths to Archive Object Pointers.
	IDTOOBJECTMAP m_FilepathToObject;
	// Map of Archive Object Pointers to File Paths.
	std::map< DWORD, DWORD > m_ObjectToFilename;

	/// global database count
	int m_iGlobalCount;
	lua_State *m_LuaState;

	CDefinedDatabase *m_GlobalAttributesDatabase;
	CHashString m_definedDatabaseComponentType;
	CHashString m_databaseSchemaComponentType;
};

#endif