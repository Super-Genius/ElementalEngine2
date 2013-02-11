///============================================================================
/// \file		DatabaseManager.cpp
/// \brief		Implmentation of CDatabaseManager Class
/// \date		07-26-2005
/// \author		D. Patrick Ghiocel
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

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DATABASEPRIORITY 0x280

TOLUA_API int tolua_DatabaseScript_open (lua_State* tolua_S);

REGISTER_COMPONENT_SINGLETON(CDatabaseManager);
REGISTER_MESSAGE_HANDLER(AddDatabase, OnAddDatabase, CDatabaseManager);
REGISTER_MESSAGE_HANDLER(GetDatabase, OnGetDatabase, CDatabaseManager);
REGISTER_MESSAGE_HANDLER(GetTypedDatabase, OnGetTypedDatabase, CDatabaseManager);
REGISTER_MESSAGE_HANDLER(SetDatabase, OnSetDatabase, CDatabaseManager);
REGISTER_MESSAGE_HANDLER(RemoveDatabase, OnRemoveDatabase, CDatabaseManager);
REGISTER_MESSAGE_HANDLER(FindDatabaseByFileName, OnFindDatabaseByFileName, CDatabaseManager);
REGISTER_MESSAGE_HANDLER(CreateGlobalDatabase, OnCreateGlobalDBObject, CDatabaseManager);

REGISTER_MESSAGE_HANDLER(LoadGlobalAttributes, OnLoadGlobalAttributes, CDatabaseManager);
REGISTER_MESSAGE_HANDLER(GetGlobalAttribute, OnGetGlobalAttribute, CDatabaseManager);
REGISTER_MESSAGE_HANDLER(GetGlobalAttributeType, OnGetGlobalAttributeType, CDatabaseManager);
REGISTER_MESSAGE_HANDLER(SetGlobalAttribute, OnSetGlobalAttribute, CDatabaseManager);


CDatabaseManager::CDatabaseManager( void ) : 
	CManager(_T("CDatabaseManager"), 0, DATABASEPRIORITY),
	m_definedDatabaseComponentType(_T("CDefinedDatabase")),
	m_databaseSchemaComponentType(_T("CDatabaseSchema"))
{
	// initialize global count
	m_iGlobalCount = 0;

	m_LuaState = NULL;
	m_GlobalAttributesDatabase = NULL;
	RegisterLuaDefinitions();
}

CDatabaseManager::~CDatabaseManager( void )
{
	DeInit();
}

void CDatabaseManager::RegisterLuaDefinitions()
{
	static DWORD msgHash_GetMasterScriptState = CHashString(_T( "GetMasterScriptState" )).GetUniqueID();
	DWORD amessageHandle = m_ToolBox->SendMessage(msgHash_GetMasterScriptState, sizeof( lua_State * ), &m_LuaState );
	if( amessageHandle != MSG_HANDLED)
	{
		StdString error( "Error missing Master Script State Data\n" );

		// log error
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
		EngineGetToolBox()->Log(LOGWARNING, error);
    }
	else
	{
		// register our additional structures/handlers with LUA master
		tolua_DatabaseScript_open(m_LuaState);
	}
}

void CDatabaseManager::MapObjectToFileName( IHashString *hszFileName, IObject *object )
{
	if( hszFileName != NULL && object != NULL )
	{
		m_FilepathToObject[hszFileName->GetUniqueID()] = object;
		m_ObjectToFilename[ object->GetName()->GetUniqueID() ] = hszFileName->GetUniqueID();
	}
}

IObject* CDatabaseManager::GetDatabaseByFile( IHashString *hszFile )
{
	DWORD objectID = hszFile->GetUniqueID();
	if( m_FilepathToObject.find( objectID ) != m_FilepathToObject.end() )
	{
		IObject *object = m_FilepathToObject[ objectID ];
		return object;
	}
	return NULL;
}

CDatabaseSchema *CDatabaseManager::GetSchemaByFile( IHashString *hszSchemaFile )
{
	CDatabaseSchema *retVal = NULL;

	DWORD objectID = hszSchemaFile->GetUniqueID();
	if( m_FilepathToObject.find( objectID ) != m_FilepathToObject.end() )
	{
		IObject *object = m_FilepathToObject[ objectID ];
		if( object->GetComponentType()->GetUniqueID() == m_databaseSchemaComponentType.GetUniqueID() )
		{
			retVal = (CDatabaseSchema*)object;
		}
	}

	return retVal;
}

IComponent* CDatabaseManager::Create(int nArgs, va_list argptr)
{
	// Return the singleton instance of this class
	return SINGLETONINSTANCE( CDatabaseManager );
}

//

bool CDatabaseManager::DeInit( void )
{
	// free up allocated hash strings
	FILENAMETODATABASEMAP::iterator ftdbmIter;
	DATABASEINFO *dbInfo;

	for (ftdbmIter = m_FilepathToDBInfo.begin(); ftdbmIter != m_FilepathToDBInfo.end();
		++ftdbmIter)
	{
		dbInfo = &ftdbmIter->second;
		ReleaseDatabase(dbInfo);
	}
	m_FilepathToDBInfo.clear();

	return CManager::DeInit();
}

DWORD CDatabaseManager::OnGetTypedDatabase(DWORD size, void *params)
{
	TYPEDATABASEINFO *pDBTypeInfo = NULL;
	LOADFILEEXTPARAMS lfep;
	FILENAMETODATABASEMAP::iterator itrDBInfo;
	IDTOOBJECTMAP::iterator itrDBObject;
	DWORD result = 0;

	// Verify and Process incomming data.
	VERIFY_MESSAGE_SIZE(size, sizeof(TYPEDATABASEINFO));
	pDBTypeInfo = (TYPEDATABASEINFO *)params;

	IHashString *tempHash = pDBTypeInfo->phszFilepath;
	if (tempHash == NULL)
	{
		m_ToolBox->Log(LOGWARNING, _T("DB: Invalid database filename specifed, cannot be NULL\n"));
		return MSG_ERROR;
	}
	if (!pDBTypeInfo->pArchive)
	{
		m_ToolBox->Log(LOGWARNING, _T("DB: Invalid database archive specifed, cannot be NULL\n"));
		return MSG_ERROR;
	}
	// Check manager maps to see if database is already loaded.
	itrDBInfo = m_FilepathToDBInfo.find( tempHash->GetUniqueID() );
	itrDBObject = m_FilepathToObject.find( tempHash->GetUniqueID() );

	// Name is not found and database does not exist in manager map. Load DB.
	if( itrDBInfo == m_FilepathToDBInfo.end() && itrDBObject == m_FilepathToObject.end() )
	{
		lfep.fileName = (TCHAR *)tempHash->GetString();
		lfep.bInternalLoad = true;
		DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		result = m_ToolBox->SendMessage(msgHash_LoadFileByExtension,
								   sizeof(LOADFILEEXTPARAMS), 
								   &lfep);
		if (result != MSG_HANDLED)
		{
			m_ToolBox->SetErrorValue(WARN_INVALID_FILE);
			m_ToolBox->Log(LOGWARNING, _T("Invalid database file %s\n"), lfep.fileName);
			return MSG_NOT_HANDLED;
		}

		// Because databases are added to the manager automatically and we cannot get
		// an iterator to the object just loaded by extension, we have to search for the
		// object again in manager map.
		itrDBInfo = m_FilepathToDBInfo.find( tempHash->GetUniqueID() );
		itrDBObject = m_FilepathToObject.find( tempHash->GetUniqueID() );
	}
	

	// There are two types of Databases, an old type that does not use schemas and a newer type that does.
	// New Type:
	CDefinedDatabase *pDefinedDB = NULL;
	CDatabaseSchema *pDBSchema = NULL;
	if ((itrDBInfo == m_FilepathToDBInfo.end()) &&  (itrDBObject != m_FilepathToObject.end()))
	{
		pDefinedDB = (CDefinedDatabase*)(itrDBObject->second);
		pDBSchema = (CDatabaseSchema*)(pDefinedDB->GetSchema());

		// Retrieve Schema Table.
		CREATEARCHIVE ca;
		CHashString memType(_T("Memory"));
		ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
		ca.streamData = NULL;
		ca.streamSize = 0;
		ca.streamType = &memType;
		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
		IArchive * archive = ca.archive;
		pDBSchema->GetSchemaArchive( archive );
		
		IArchive * archiveOut = pDBTypeInfo->pArchive;

		StdString szTemp;
		bool bTemp;
		int nSchemaValues = 0;
		StdString szName;
		CHashString hszName;
		StdString szType;
		archive->Read(szTemp);
		archive->Read(nSchemaValues);
		int nDBItems = pDefinedDB->GetNumItems();

		// Write Total Number of Elements
		archiveOut->SetIsWriting(true);
		archiveOut->Write(nDBItems);
		archiveOut->Write(nSchemaValues);
		for (int i = 0; i < nDBItems; i++)
		{
			archive->SeekTo(0);	
			archive->Read(szTemp);
			archive->Read(nSchemaValues);
			for (int j = 0; j < nSchemaValues; j++)
			{
				archive->Read(szName);
				archive->Read(szType);
				archive->Read(bTemp);

				hszName.Init( szType.c_str() );
				archiveOut->Write(szName.c_str());
				archiveOut->Write(szType.c_str());
				DATABASEDATA * pValue = NULL;
				pValue = pDefinedDB->GetDatabaseItem(i)->GetDataByName( &hszName );
				pDBSchema->GetSchemaItem(&hszName)->DBWrite(pValue, archiveOut, _T(""));
			}
		}
		archiveOut->SetIsWriting(false);
		archive->Close();
		return MSG_HANDLED_STOP;
	}
	// Old Type DB or Missing DB:
	else
	{
		m_ToolBox->Log(LOGERROR, _T("DB: Attempt to call GetTypedDatabase on a non-existant DB or a CDatabase DB that does not support types.\n"));
		return MSG_ERROR;
	}
}

DWORD CDatabaseManager::OnGetDatabase(DWORD size, void *params)
{
	DATABASEINFO *dbInfo;
	LOADFILEEXTPARAMS lfep;
	FILENAMETODATABASEMAP::iterator ftdbmIter;
	IDTOOBJECTMAP::iterator objectMapIterator;

	// verify the message we have received is the the right size
	VERIFY_MESSAGE_SIZE(size, sizeof(DATABASEINFO));

	// cast params to a DATABASEINFO
	dbInfo = (DATABASEINFO *)params;

	// find the name in our map
	IHashString *tempHash = dbInfo->m_FileName;
	if (tempHash == NULL)
	{
		m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
		m_ToolBox->Log(LOGWARNING, _T("Invalid database filename specifed, cannot be NULL\n"));
		return MSG_NOT_HANDLED;
	}

	ftdbmIter = m_FilepathToDBInfo.find(tempHash->GetUniqueID());
	objectMapIterator = m_FilepathToObject.find( tempHash->GetUniqueID() );

	// if the name isn't found
	if( ftdbmIter == m_FilepathToDBInfo.end() && objectMapIterator == m_FilepathToObject.end() )
	{
		lfep.fileName = (TCHAR *)tempHash->GetString();
		lfep.bInternalLoad = true;
		DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_LoadFileByExtension,
								   sizeof(LOADFILEEXTPARAMS), 
								   &lfep) != MSG_HANDLED)
		{
			// log warning
			m_ToolBox->SetErrorValue(WARN_INVALID_FILE);
			m_ToolBox->Log(LOGWARNING, _T("Invalid database file %s\n"), lfep.fileName);
			return MSG_NOT_HANDLED;
		}

		ftdbmIter = m_FilepathToDBInfo.find(tempHash->GetUniqueID());
		objectMapIterator = m_FilepathToObject.find( tempHash->GetUniqueID() );
	}

	if( ftdbmIter != m_FilepathToDBInfo.end() )
	{
		DATABASEINFO *newDBInfo = &ftdbmIter->second;
		// increment reference count
		++newDBInfo->m_RefCount;
		dbInfo->m_DatabaseData = newDBInfo->m_DatabaseData;
		dbInfo->m_nItems = newDBInfo->m_nItems;
		dbInfo->m_nRowSize = newDBInfo->m_nRowSize;
		dbInfo->m_SchemaName = newDBInfo->m_SchemaName;
		dbInfo->m_SchemaFile = newDBInfo->m_SchemaFile;
	}
	else if( objectMapIterator != m_FilepathToObject.end() )
	{
		CDefinedDatabase *iteratedDatabase = (CDefinedDatabase*)objectMapIterator->second;
		dbInfo->m_DatabaseName = iteratedDatabase->GetName();
		dbInfo->m_nItems = iteratedDatabase->GetNumItems();
		dbInfo->m_nRowSize = iteratedDatabase->GetSchema()->GetNumItems();
		dbInfo->m_SchemaName = iteratedDatabase->GetSchema()->GetName();
		dbInfo->m_SchemaFile = iteratedDatabase->GetSchemaFile();
	}
	else
	{
		return MSG_ERROR;
	}

	return MSG_HANDLED_PROCEED;
}

// Add a database information struct to the manager
// This message is usually sent by the Database Loader
DWORD CDatabaseManager::OnAddDatabase(DWORD size, void *params)
{
	// add the database filename and database info to the map
	IHashString *tempHash;
	// make sure the size is correct
	VERIFY_MESSAGE_SIZE(size, sizeof(DATABASEINFO));

	// extract the DATABASEINFO from the void pointer
	DATABASEINFO *dbInfo = (DATABASEINFO *)params;

	tempHash = dbInfo->m_FileName;
	DATABASEINFO *newDBInfo = &(m_FilepathToDBInfo[tempHash->GetUniqueID()]);

	newDBInfo->m_FileName = new CHashString(dbInfo->m_FileName);
	newDBInfo->m_SchemaName = new CHashString(dbInfo->m_SchemaName);
	newDBInfo->m_nItems = dbInfo->m_nItems;
	newDBInfo->m_nRowSize = dbInfo->m_nRowSize;
	newDBInfo->m_DatabaseData = dbInfo->m_DatabaseData;
	newDBInfo->m_RefCount = 1;

	return MSG_HANDLED_PROCEED;
}

DWORD CDatabaseManager::OnRemoveDatabase(DWORD size, void *params)
{
	FILENAMETODATABASEMAP::iterator ftdbmIter;
	DATABASEINFO *dbInfo;
	// verify the message we have received is the the right size
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));

	// cast to a IHashString
	IHashString *name = (IHashString *)params;

	// find it
	ftdbmIter = m_FilepathToDBInfo.find(name->GetUniqueID());
	if (ftdbmIter == m_FilepathToDBInfo.end())
	{
		// log warning
		m_ToolBox->SetErrorValue(WARN_INVALID_FILE);
		m_ToolBox->Log(LOGWARNING, _T("Invalid database file %s\n"), name->GetString());
		return MSG_NOT_HANDLED;
	}

	dbInfo = &ftdbmIter->second;
	// decrement reference count here
	dbInfo->m_RefCount--;
	if (dbInfo->m_RefCount == 0)
	{
		// really free the data
		ReleaseDatabase(dbInfo);
		m_FilepathToDBInfo.erase(ftdbmIter);
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CDatabaseManager::OnSetDatabase(DWORD size, void *params)
{
	DATABASEINFO *dbInfo;
	FILENAMETODATABASEMAP::iterator ftdbmIter;

	// verify the message we have received is the the right size
	VERIFY_MESSAGE_SIZE(size, sizeof(DATABASEINFO));

	// cast params to a DATABASEINFO
	dbInfo = (DATABASEINFO *)params;

	// find the name in our map
	IHashString *tempHash = dbInfo->m_FileName;
	ftdbmIter = m_FilepathToDBInfo.find(tempHash->GetUniqueID());

	// if the name isn't found
	if (ftdbmIter == m_FilepathToDBInfo.end())
	{
		// log warning
		m_ToolBox->SetErrorValue(WARN_INVALID_FILE);
		m_ToolBox->Log(LOGWARNING, _T("Invalid database file %s\n"), tempHash->GetString());
		return MSG_NOT_HANDLED;
	}

	DATABASEINFO *newDBInfo = &ftdbmIter->second;
	newDBInfo->m_DatabaseData = dbInfo->m_DatabaseData;
	newDBInfo->m_nItems = dbInfo->m_nItems;
	newDBInfo->m_nRowSize = dbInfo->m_nRowSize;

	return MSG_HANDLED_PROCEED;
}

DWORD CDatabaseManager::OnFindDatabaseByFileName(DWORD size, void *params)
{
	DBFINDDATABASE *dbInfo;
	
	// verify the message we have received is the the right size
	VERIFY_MESSAGE_SIZE(size, sizeof(DBFINDDATABASE));

	// cast params to a DATABASEINFO
	dbInfo = (DBFINDDATABASE *)params;
	if( dbInfo->m_DatabaseFileName == NULL )
	{
		return MSG_ERROR;
	}

	IObject* tmpobj = GetDatabaseByFile( dbInfo->m_DatabaseFileName );

	if( tmpobj == NULL )
	{
		dbInfo->m_bIsFound = false;
		dbInfo->m_DatabaseObjName = NULL;
		return MSG_HANDLED_PROCEED;
	}
	
	dbInfo->m_bIsFound = true;
	dbInfo->m_DatabaseObjName = tmpobj->GetName();

	return MSG_HANDLED_PROCEED;	
}

DWORD CDatabaseManager::OnCreateGlobalDBObject(DWORD size, void *params)
{
	DBCREATEPARAMS *db;
	CHashString memType(_T("Memory"));
	IArchive *MemArchive;
	StdString str;
	TCHAR strNum[50];
	int memsize;
	VERIFY_MESSAGE_SIZE(size, sizeof(DBCREATEPARAMS));

	db = (DBCREATEPARAMS*)params;

	// begin name
	str = _T("GlobalDatabase");

	// append number to each name
	_ltot(m_iGlobalCount, strNum, 10);
	str = str + strNum;
	
	// increment global count
	++m_iGlobalCount;

	// get name, parent and type
	CHashString name(str.c_str());
	static CHashString typeName(_T("CDatabase"));
	
	// create object
	CDatabase *pDatabase = dynamic_cast<CDatabase *>(
		m_ToolBox->CreateComponent(&typeName, 3, &name, NULL, false));

	// add object to object factory by hand since we need a pointer to the database object
	OFACTORYADDPARAMS aotf;
	aotf.component = pDatabase;
	aotf.name = &name;
	static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(aotf), &aotf );

	// get size of name
	memsize = _tcslen(db->fileName)+1;

	// create memory chunk
	char* memBuffer = new char[memsize];

	// create archive
	CREATEARCHIVE ca;
	ca.mode =  STREAM_MODE_READ | STREAM_MODE_WRITE;
	ca.streamData = memBuffer;
	ca.streamSize = memsize;
	ca.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		StdString error;
		error = _T(__FILE__ " OnCreateGlobalDBObject(): cannot create archive\n");
		EngineGetToolBox()->Log( LOGWARNING, error );
		if( memBuffer != NULL )
		{
			delete [] memBuffer;
		}
		memBuffer = NULL;
		return MSG_ERROR;
	}

	// write name to archive 
	MemArchive = ca.archive;
	MemArchive->Write(db->fileName);

	// serialize database object
	pDatabase->Serialize(*MemArchive);
	// init database object
	pDatabase->Init();

	// close the archive
	MemArchive->Close();
	// clean up
	if( memBuffer != NULL )
	{
		delete [] memBuffer;
	}
	memBuffer = NULL;
	
	*static_cast<IDatabase**>(db->dbObj) = pDatabase;

	return MSG_HANDLED_STOP;
}

DWORD CDatabaseManager::OnLoadGlobalAttributes( DWORD size, void *params )
{
	if( m_GlobalAttributesDatabase == NULL )
	{
		VERIFY_MESSAGE_SIZE( size, sizeof(LOADGLOBALATTRIBUTESPARAMS) );
		LOADGLOBALATTRIBUTESPARAMS *loadGlobalAttributesParams = (LOADGLOBALATTRIBUTESPARAMS*)params;
		CHashString definedDatabaseType( _T("CDefinedDatabase") );
		CHashString databaseItemType( _T("CDatabaseItem") );

		// Creating a CDefinedDatabase for the global attributes.
		// It is simply a database with a single entry.
		CREATEOBJECTPARAMS cop;

		cop.name = loadGlobalAttributesParams->Name;
		cop.parentName = NULL;
		cop.typeName = &definedDatabaseType;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

		// Set a pointer to the global attributes database
		IDTOOBJECTMAP *objectMap = GetObjectMap( &definedDatabaseType );
		if (objectMap)
		{
			IDTOOBJECTMAP::iterator objectsIterator = objectMap->find( loadGlobalAttributesParams->Name->GetUniqueID() );
			if( objectsIterator != objectMap->end() )
			{
				m_GlobalAttributesDatabase = (CDefinedDatabase*)objectsIterator->second;
			}
		}
		else
			return MSG_ERROR;
		// Creating memory archive
		CREATEARCHIVE ca;
		CHashString memType(_T("Memory"));

		ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
		ca.streamData = NULL;
		ca.streamSize = 0;
		ca.streamType = &memType;
		ca.archive = NULL;

		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
		if (!ca.archive)
			return MSG_ERROR;
		// Writing serialization values to archive
		float fVersion = 1.0f;
		const TCHAR *name = loadGlobalAttributesParams->Name->GetString();
		const TCHAR *schemaFile = loadGlobalAttributesParams->SchemaFile->GetString();
		const TCHAR *schemaName = loadGlobalAttributesParams->SchemaName->GetString();
		int numItems = 1;
		IArchive *archive = ca.archive;
		archive->SetIsWriting( true );
		archive->Write( fVersion );
		archive->Write( name );
		archive->Write( schemaFile );
		archive->Write( schemaName );
		archive->Write( numItems );
		archive->SetIsWriting( false );

		// Serialize values to defined database object
		SERIALIZEOBJECTPARAMS sop;
		sop.name = loadGlobalAttributesParams->Name;
		sop.archive = archive;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop );

		archive->Close();

		// Creating a CDatabaseItem single entry.
		StdString szDatabaseItemName = loadGlobalAttributesParams->Name->GetString();
		szDatabaseItemName += _T("_SingleEntry");
		CHashString hszDatabaseItemName( szDatabaseItemName );

		cop.name = &hszDatabaseItemName;
		cop.parentName = loadGlobalAttributesParams->Name;
		cop.typeName = &databaseItemType;
		m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
	}
	else
	{
		EngineGetToolBox()->Log( LOGWARNING, _T("A global attributes set has already loaded.") );
	}

	return MSG_HANDLED_STOP;
}

DWORD CDatabaseManager::OnGetGlobalAttribute( DWORD size, void *params )
{
	DWORD retVal = MSG_HANDLED_STOP;
	if( m_GlobalAttributesDatabase != NULL )
	{
		VERIFY_MESSAGE_SIZE( size, sizeof(DATABASEATTRIBUTEPARAMS) );
		DATABASEATTRIBUTEPARAMS *databaseAttributesParams = (DATABASEATTRIBUTEPARAMS*)params;

		// Global attributes database should only have one entry.
		CDatabaseItem *databaseItem = m_GlobalAttributesDatabase->GetDatabaseItem( 0 );
		retVal = databaseItem->GetAttribute( databaseAttributesParams );
	}
	else
	{
		EngineGetToolBox()->Log( LOGWARNING, _T("No global attributes set has been loaded.") );
	}

	return retVal;
}

DWORD CDatabaseManager::OnSetGlobalAttribute( DWORD size, void *params )
{
	DWORD retVal = MSG_HANDLED_STOP;
	if( m_GlobalAttributesDatabase != NULL )
	{
		VERIFY_MESSAGE_SIZE( size, sizeof(DATABASEATTRIBUTEPARAMS) );
		DATABASEATTRIBUTEPARAMS *databaseAttributesParams = (DATABASEATTRIBUTEPARAMS*)params;

		// Global attributes database should only have one entry.
		CDatabaseItem *databaseItem = m_GlobalAttributesDatabase->GetDatabaseItem( 0 );
		retVal = databaseItem->SetAttribute( databaseAttributesParams );
	}
	else
	{
		EngineGetToolBox()->Log( LOGWARNING, _T("No global attributes set has been loaded.") );
	}

	return retVal;
}

DWORD CDatabaseManager::OnGetGlobalAttributeType( DWORD size, void *params )
{
	DWORD retVal = MSG_HANDLED_STOP;
	if( m_GlobalAttributesDatabase != NULL )
	{
		VERIFY_MESSAGE_SIZE( size, sizeof(DATABASEATTRIBUTEPARAMS) );
		DATABASEATTRIBUTEPARAMS *databaseAttributesParams = (DATABASEATTRIBUTEPARAMS*)params;

		// Global attributes database should only have one entry.
		CDatabaseItem *databaseItem = m_GlobalAttributesDatabase->GetDatabaseItem( 0 );
		retVal = databaseItem->GetAttributeType( databaseAttributesParams );
	}
	else
	{
		EngineGetToolBox()->Log( LOGWARNING, _T("No global attributes set has been loaded.") );
	}

	return retVal;
}

void CDatabaseManager::OnDatabaseDeInit( CDefinedDatabase *definedDatabase )
{
	if( definedDatabase == m_GlobalAttributesDatabase )
	{
		m_GlobalAttributesDatabase= NULL;
	}
}

bool CDatabaseManager::DeleteObject( IObject* Object )
{
	if( Object != NULL )
	{
		DWORD objNameID = Object->GetName()->GetUniqueID();
		std::map< DWORD, DWORD >::iterator fileIDIterator;
		fileIDIterator = m_ObjectToFilename.find( objNameID );
		if( fileIDIterator != m_ObjectToFilename.end() )
		{
			DWORD fileNameID = fileIDIterator->second;
			IDTOOBJECTMAP::iterator olIter;
			
			// see if in our map
			olIter = m_FilepathToObject.find( fileNameID );
			// check to make sure it was found before deleting it
			if (olIter != m_FilepathToObject.end())
			{
				m_FilepathToObject.erase(olIter);
			}
		}
	}

	return CManager::DeleteObject(Object);
}

///	free all memory allocations for database
///	\param	dbInfo - pointer to database information
void CDatabaseManager::ReleaseDatabase(DATABASEINFO *dbInfo)
{
	DATABASEDATA *data = dbInfo->m_DatabaseData;
	if (data != NULL)
	{
		for_each(data, data + dbInfo->m_nItems, mem_fun_ref(&DATABASEDATA::Release));
		GLOBALFREE(data);
	}
	// free up the alloc CHashString*
	delete dbInfo->m_SchemaName;
	delete dbInfo->m_FileName;
	// TODO: Check that name is owned by DATABASEINFO structure also
	//delete dbInfo->m_DatabaseName;
	//delete dbInfo->m_SchemaFile;
}
