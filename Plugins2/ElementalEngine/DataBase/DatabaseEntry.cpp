///============================================================================
/// \file		DatabaseEntry.cpp
/// \brief		Implentation of CDatabaseEntry Class
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

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CDatabaseEntry);
REGISTER_MESSAGE_HANDLER(GetAttribute, OnGetAttribute, CDatabaseEntry);
REGISTER_MESSAGE_HANDLER(GetAttributeType, OnGetAttributeType, CDatabaseEntry);
REGISTER_MESSAGE_HANDLER(SetAttribute, OnSetAttribute, CDatabaseEntry);

REGISTER_MESSAGE_HANDLER(GetSchema, OnGetSchema, CDatabaseEntry);

CDatabaseEntry::CDatabaseEntry( IHashString *parentName, IHashString *name ) :
	OBJECTTEMPLATE( CDatabaseManager, CDatabaseEntry, IObject, parentName, name ),
	m_hszDefinedDatabaseType(_T("CDefinedDatabase")),
	m_hszDatabaseItemType(_T("CDatabaseItem")),
	m_hszDatabaseSchemaType(_T("CDatabaseSchema"))
{
	m_ToolBox = EngineGetToolBox();
	m_hszDatabaseItem.Init(_T(""));
	AddToHierarchy();
}

CDatabaseEntry::~CDatabaseEntry()
{
	RemoveFromHierarchy();
}

void CDatabaseEntry::Init()
{
}

bool CDatabaseEntry::Update()
{
	// NO-OP
	return true;
}

void CDatabaseEntry::DeInit()
{
	DELETEOBJECTPARAMS dop;
	dop.name = &m_hszDatabaseItem;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop );
}

IComponent *CDatabaseEntry::Create( int nArgs, va_list argptr )
{
	IObject *self;

	IHashString *name = va_arg( argptr, IHashString * );
	IHashString *parentName = va_arg( argptr, IHashString * );
	try
	{
		self = new CDatabaseEntry( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CDatabaseEntry::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		StdString szTempString;
		ar.Read( szTempString, _T("SchemaFile") );
		m_hszSchemaFile.Init( szTempString );
		ar.Read( szTempString, _T("Schema") );
		m_hszSchema.Init( szTempString );
		ar.Read( szTempString, _T("Database") );
		m_hszDatabase.Init( szTempString );

		int iSpecifiedValues = 0;
		ar.Read( iSpecifiedValues, _T("SpecifiedValues") );

		RegisterAsDatabaseItem();

		for( int i = 0; i < iSpecifiedValues; i++ )
		{
			ar.Read( szTempString, _T("Name") );
			CHashString hszName( szTempString );

			DATABASEATTRIBUTEPARAMS setAttributeParams;
			setAttributeParams.m_AttributeName = &hszName;
			setAttributeParams.m_AttributeArchive = &ar;
			m_DatabaseItem->SetAttribute( &setAttributeParams );
		}
	}
	else
	{
		ar.Write( m_hszSchemaFile.GetString(), _T("SchemaFile") );
		ar.Write( m_hszSchema.GetString(), _T("Schema") );
		ar.Write( m_hszDatabase.GetString(), _T("Database") );

		ATTRIBUTEIDTODATAMAP *valuesMap = m_DatabaseItem->GetValuesMap();
		int iSpecifiedValues = (int)valuesMap->size();
		ar.Write( iSpecifiedValues, _T("SpecifiedValues") );
		ATTRIBUTEIDTODATAMAP::iterator valuesMapIterator;

		for( valuesMapIterator = valuesMap->begin(); valuesMapIterator != valuesMap->end(); valuesMapIterator++ )
		{
			ar.StartClass( _T("CDatabaseEntryValue") );

			CHashString hszAttributeName( m_ToolBox->GetHashString( valuesMapIterator->first ) );
			ar.Write( hszAttributeName.GetString(), _T("Name") );

			DATABASEATTRIBUTEPARAMS getAttributeParams;
			getAttributeParams.m_AttributeName = &hszAttributeName;
			getAttributeParams.m_AttributeArchive = &ar;
			m_DatabaseItem->GetAttribute( &getAttributeParams );

			ar.EndClass();
		}
	}
}

void CDatabaseEntry::RegisterAsDatabaseItem()
{
	m_Schema = GetSchema();

	// Create a database if it doesn't already exist.
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &m_hszDatabase;
	DWORD findObjectRetVal = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param);
	if( ( findObjectRetVal == MSG_HANDLED ) && (param.bFound == false) )
	{
		CREATEOBJECTPARAMS createObjectParams;

		createObjectParams.name = &m_hszDatabase;
		createObjectParams.parentName = NULL;
		createObjectParams.typeName = &m_hszDefinedDatabaseType;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &createObjectParams);

		CREATEARCHIVE ca;
		CHashString memType(_T("Memory"));

		ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
		ca.streamData = NULL;
		ca.streamSize = 0;
		ca.streamType = &memType;
		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);

		ca.archive->SetIsWriting( true );
		ca.archive->Write( 1.0f );
		ca.archive->Write( m_hszDatabase.GetString() );
		ca.archive->Write( m_hszSchemaFile.GetString() );
		ca.archive->Write( m_hszSchema.GetString() );
		ca.archive->Write( (int)0 );
		ca.archive->SetIsWriting( false );

		SERIALIZEOBJECTPARAMS serializeObjectParams;
		serializeObjectParams.name = &m_hszDatabase;
		serializeObjectParams.archive = ca.archive;

		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &serializeObjectParams);

		ca.archive->Close();
	}

	// Set a pointer to the database
	m_Database = GetDefinedDatabase();
	if( m_Database->GetSchema() != m_Schema )
	{
		m_ToolBox->Log(LOGWARNING, _T("Schema in database %s does not match specified schema %s.\n"), m_hszDatabase.GetString(), m_hszSchema.GetString() );
	}

	// Generate a unique name for the database item
	GENERATEUNIQUEOBJECTNAMEPARAMS generateNameParams;
	StdString baseNameString = StdString( GetName()->GetString() ) + _T("_DatabaseItem");
	CHashString baseName( baseNameString );
	generateNameParams.name = &baseName;
	generateNameParams.newname = &m_hszDatabaseItem;
	static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(generateNameParams), &generateNameParams );

	// Create a database item corresponding to the entry.
	CREATEOBJECTPARAMS createObjectParams;
	createObjectParams.name = &m_hszDatabaseItem;
	createObjectParams.parentName = &m_hszDatabase;
	createObjectParams.typeName = &m_hszDatabaseItemType;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &createObjectParams);

	// Set a pointer to the created item
	m_DatabaseItem = GetDatabaseItem();
}

DWORD CDatabaseEntry::OnGetAttribute( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(DATABASEATTRIBUTEPARAMS) );
	DATABASEATTRIBUTEPARAMS *databaseAttributeParams = (DATABASEATTRIBUTEPARAMS*)params;

	DWORD retVal = m_DatabaseItem->GetAttribute( databaseAttributeParams );

	return retVal;
}

DWORD CDatabaseEntry::OnGetAttributeType( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(DATABASEATTRIBUTEPARAMS) );
	DATABASEATTRIBUTEPARAMS *databaseAttributeParams = (DATABASEATTRIBUTEPARAMS*)params;

	DWORD retVal = m_DatabaseItem->GetAttributeType( databaseAttributeParams );

	return retVal;
}

DWORD CDatabaseEntry::OnSetAttribute( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(DATABASEATTRIBUTEPARAMS) );
	DATABASEATTRIBUTEPARAMS *databaseAttributeParams = (DATABASEATTRIBUTEPARAMS*)params;

	DWORD retVal = m_DatabaseItem->SetAttribute( databaseAttributeParams );

	return retVal;
}

DWORD CDatabaseEntry::OnGetSchema( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETSCHEMAPARAMS));
	GETSCHEMAPARAMS *getSchemaParams = (GETSCHEMAPARAMS*)params;
	IXMLArchive *archive = getSchemaParams->ar;
	if( archive != NULL )
	{
		int numItems = m_Schema->GetNumItems();
		archive->StartClass( GetComponentType()->GetString() );
		archive->Write( GetName()->GetString(), _T("Name") );
		archive->Write( 2 * numItems + 4, _T("Num") );

		archive->StartClass(_T("hidden") );
		archive->Write( _T("string"), _T("Type") );
		archive->EndClass();

		archive->StartClass(_T("hidden") );
		archive->Write( _T("string"), _T("Type") );
		archive->EndClass();

		archive->StartClass(_T("hidden") );
		archive->Write( _T("string"), _T("Type") );
		archive->EndClass();

		archive->StartClass(_T("hidden") );
		archive->Write( _T("int"), _T("Type") );
		archive->EndClass();

		ATTRIBUTEIDTODATAMAP *valuesMap = m_DatabaseItem->GetValuesMap();
		ATTRIBUTEIDTODATAMAP::iterator valuesMapIterator;

		for( valuesMapIterator = valuesMap->begin(); valuesMapIterator != valuesMap->end(); valuesMapIterator++ )
		{
			CHashString hszAttributeName( m_ToolBox->GetHashString( valuesMapIterator->first ) );
			CHashString hszClassType( _T("") );
			CHashString hszAttributeType( _T("") );
			archive->StartClass( _T("hidden") );
			archive->Write( _T("string"), _T("Type") );
			archive->EndClass();

			m_DatabaseItem->GetClassType( &hszAttributeName, &hszClassType );
			m_DatabaseItem->GetAttributeType( &hszAttributeName, &hszAttributeType );
			archive->StartClass( hszClassType.GetString() );
			archive->Write( hszAttributeType.GetString(), _T("Type") );
			archive->Write( hszAttributeName.GetString(), _T("Name") );
			archive->Write( m_Schema->IsKey( &hszAttributeName ), _T("Key") );
			archive->EndClass();
		}

		archive->EndClass();
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Attempt to get CDatabaseEntry schema. No archive specified.\n" ) );
	}

	return MSG_HANDLED_STOP;
}

IObject *CDatabaseEntry::GetMapObject( IHashString *objectName, IHashString *objectType )
{
	IObject *object = NULL;

	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CDatabaseManager *amanager = (CDatabaseManager*)( amanagerComponent );
	IDTOOBJECTMAP *objectMap = amanager->GetObjectMap( objectType );
	if( objectMap != NULL )
	{
		IDTOOBJECTMAP::iterator objectsIterator = objectMap->find(objectName->GetUniqueID());
		if( objectsIterator != objectMap->end() )
		{
			object = objectsIterator->second;
		}
	}

	return object;
}

CDefinedDatabase *CDatabaseEntry::GetDefinedDatabase()
{
	CDefinedDatabase *database = (CDefinedDatabase*)GetMapObject( &m_hszDatabase, &m_hszDefinedDatabaseType );

	if( database == NULL )
	{
		m_ToolBox->Log(LOGWARNING, _T("Failed to create/locate database %s.\n"), m_hszDatabase.GetString());
	}

	return database;
}

CDatabaseItem *CDatabaseEntry::GetDatabaseItem()
{
	CDatabaseItem *databaseItem = (CDatabaseItem*)GetMapObject( &m_hszDatabaseItem, &m_hszDatabaseItemType );

	if( databaseItem == NULL )
	{
		m_ToolBox->Log(LOGWARNING, _T("Failed to create/locate database item %s.\n"), m_hszDatabase.GetString());
	}

	return databaseItem;
}

CDatabaseSchema *CDatabaseEntry::GetSchema()
{
	CDatabaseSchema *schema = (CDatabaseSchema*)GetMapObject( &m_hszSchema, &m_hszDatabaseSchemaType );

	if( schema == NULL )
	{
		IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
		CDatabaseManager *amanager = (CDatabaseManager*)( amanagerComponent );
		schema = amanager->GetSchemaByFile( &m_hszSchemaFile );

		// Schema could not be found, so we try loading the file and grabbing the schema by filename one more time
		if( schema == NULL )
		{
			LOADFILEEXTPARAMS loadFileParams;
			loadFileParams.fileName = (TCHAR*)m_hszSchemaFile.GetString();
			loadFileParams.bInternalLoad = true;
			static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(loadFileParams), &loadFileParams);

			schema = amanager->GetSchemaByFile( &m_hszSchemaFile );
		}

	}

	if( schema == NULL )
	{
		m_ToolBox->Log(LOGWARNING, _T("Failed to create/locate schema %s.\n"), m_hszSchema.GetString());
	}

	return schema;
}