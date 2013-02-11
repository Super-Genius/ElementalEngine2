#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "stdafx.h"

CDatabaseTest::CDatabaseTest( CRichEditCtrl *dialogEditControl )
	: CTestBase( _T( "Database Test" ), dialogEditControl )
{
}

CDatabaseTest::~CDatabaseTest()
{
}

void CDatabaseTest::Initialize()
{
	CTestBase::Initialize();
	RegisterTestCase( _T( "Creation of Database Schema 1" ), (TESTCASE)&CDatabaseTest::TestCreateSchema );
	RegisterTestCase( _T( "Creation of Database Schema Item 1" ), (TESTCASE)&CDatabaseTest::TestCreateSchemaItem );
	RegisterTestCase( _T( "Creation of Database Schema 2" ), (TESTCASE)&CDatabaseTest::TestSerializeSchema );
	RegisterTestCase( _T( "Creation of Defined Database 1" ), (TESTCASE)&CDatabaseTest::TestCreateDefinedDatabase );
	RegisterTestCase( _T( "Creation of Database Item 1" ), (TESTCASE)&CDatabaseTest::TestCreateDatabaseItem );
	RegisterTestCase( _T( "Creation of Database Structure" ), (TESTCASE)&CDatabaseTest::TestDatabaseStructure );
	RegisterTestCase( _T( "Load schema from file" ), (TESTCASE)&CDatabaseTest::TestLoadSchemaFile );
	RegisterTestCase( _T( "Load Database from file" ), (TESTCASE)&CDatabaseTest::TestLoadDatabaseFile );
	RegisterTestCase( _T( "Creation of Database Entry" ), (TESTCASE)&CDatabaseTest::TestCreateDatabaseEntry );
	RegisterTestCase( _T( "Getting attribute type" ), (TESTCASE)&CDatabaseTest::TestGetAttributeType );
	RegisterTestCase( _T( "Database Edit Value" ), (TESTCASE)&CDatabaseTest::TestDatabaseValueEdit );
	RegisterTestCase( _T( "Loading global attributes" ), (TESTCASE)&CDatabaseTest::TestLoadGlobalAttibutes );
	RegisterTestCase( _T( "Indexing database by key value" ), (TESTCASE)&CDatabaseTest::TestIndexing );
}

void CDatabaseTest::TestCreateSchema()
{
	CHashString hszDatabaseSchemaName(_T("Schema1"));
	CHashString hszDatabaseSchemaType(_T("CDatabaseSchema"));

	CreateObject( &hszDatabaseSchemaName, NULL, &hszDatabaseSchemaType );
	InitializeObject( &hszDatabaseSchemaName );

	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &hszDatabaseSchemaName;
	DWORD retVal = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );

	TestAssert( _T("Created schema found in engine"), (retVal == MSG_HANDLED) && (param.bFound) );

	IArchive *archive = CreateMemoryArchive();
	SerializeObject( &hszDatabaseSchemaName, archive, true );

	float fVersion;
	StdString szName;
	int iNumItems;
	archive->SetIsWriting( false );
	archive->Read( fVersion );
	archive->Read( szName );
	archive->Read( iNumItems );
	archive->Close();

	TestAssertEquals( _T("Verify default version"), 1.0f, fVersion );
	TestAssertEquals( _T("Verify default name"), hszDatabaseSchemaName.GetString(), szName );
	TestAssertEquals( _T("Verify default count"), 0, iNumItems );

	DeleteObject( &hszDatabaseSchemaName );
}

void CDatabaseTest::TestCreateSchemaItem()
{
	CHashString hszDatabaseSchemaName(_T("SchemaItem1"));
	CHashString hszDatabaseSchemaType(_T("CSchemaItem"));

	CreateObject( &hszDatabaseSchemaName, NULL, &hszDatabaseSchemaType );

	InitializeObject( &hszDatabaseSchemaName );

	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &hszDatabaseSchemaName;
	DWORD retVal = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );

	TestAssert( _T("Created schema found in engine"), retVal == MSG_HANDLED && param.bFound );

	IArchive *archive = CreateMemoryArchive();
	SerializeObject( &hszDatabaseSchemaName, archive, true );

	float fVersion;
	StdString szName;
	StdString szType;
	StdString szClass;
	StdString szDefault;
	archive->SetIsWriting( false );
	archive->Read( fVersion );
	archive->Read( szName );
	archive->Read( szType );
	archive->Read( szClass );
	archive->Read( szDefault );
	archive->Close();

	TestAssertEquals( _T("Verify default version"), 1.0f, fVersion );
	TestAssertEquals( _T("Verify default name"), _T(""), szName );
	TestAssertEquals( _T("Verify default type"), _T("char"), szType );
	TestAssertEquals( _T("Verify default class"), _T("edit"), szClass );
	TestAssertEquals( _T("Verify default value"), _T(""), szDefault );

	DeleteObject( &hszDatabaseSchemaName );
}

void CDatabaseTest::TestSerializeSchema()
{
	CHashString hszDatabaseSchemaName(_T("SchemaItem2"));
	CHashString hszDatabaseSchemaType(_T("CSchemaItem"));

	CreateObject( &hszDatabaseSchemaName, NULL, &hszDatabaseSchemaType );
	InitializeObject( &hszDatabaseSchemaName );

	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &hszDatabaseSchemaName;
	DWORD retVal = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );

	TestAssert( _T("Created schema found in engine"), retVal == MSG_HANDLED && param.bFound );

	IArchive *archive = CreateMemoryArchive();
	archive->SetIsWriting( true );
	archive->Write( 1.0f );
	archive->Write( _T("TestAttribute") );
	archive->Write( _T("float") );
	archive->Write( _T("key") );
	archive->Write( 1.1111f );
	SerializeObject( &hszDatabaseSchemaName, archive, false );
	archive->Close();

	archive = CreateMemoryArchive();
	SerializeObject( &hszDatabaseSchemaName, archive, true );

	float fVersion;
	StdString szName;
	StdString szType;
	StdString szClass;
	float fDefault;
	archive->SetIsWriting( false );
	archive->Read( fVersion );
	archive->Read( szName );
	archive->Read( szType );
	archive->Read( szClass );
	archive->Read( fDefault );
	archive->Close();

	TestAssertEquals( _T("Verify serialized version"), 1.0f, fVersion );
	TestAssertEquals( _T("Verify serialized name"), _T("TestAttribute"), szName );
	TestAssertEquals( _T("Verify serialized type"), _T("float"), szType );
	TestAssertEquals( _T("Verify serialized class"), _T("key"), szClass );
	TestAssertEquals( _T("Verify serialized value"), 1.1111f, fDefault );

	DeleteObject( &hszDatabaseSchemaName );
}

void CDatabaseTest::TestCreateDefinedDatabase()
{
	CHashString hszDefinedDatabaseName(_T("DefinedDatabase 1"));
	CHashString hszDefinedDatabaseType(_T("CDefinedDatabase"));

	CreateObject( &hszDefinedDatabaseName, NULL, &hszDefinedDatabaseType );
	InitializeObject( &hszDefinedDatabaseName );

	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &hszDefinedDatabaseName;
	DWORD retVal = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );

	TestAssert( _T("Created Defined Database found in engine"), retVal == MSG_HANDLED && param.bFound );

	IArchive *archive = CreateMemoryArchive();
	SerializeObject( &hszDefinedDatabaseName, archive, true );

	float fVersion;
	StdString szName;
	StdString szSchema;
	int iNumItems;
	archive->SetIsWriting( false );
	archive->Read( fVersion );
	archive->Read( szName );
	archive->Read( szSchema );
	archive->Read( iNumItems );
	archive->Close();

	TestAssertEquals( _T("Verify default version"), 1.0f, fVersion );
	TestAssertEquals( _T("Verify default name"), hszDefinedDatabaseName.GetString(), szName );
	TestAssertEquals( _T("Verify default schema"), _T(""), szSchema );
	TestAssertEquals( _T("Verify default count"), 0, iNumItems );

	DeleteObject( &hszDefinedDatabaseName );
}

void CDatabaseTest::TestCreateDatabaseItem()
{
	CHashString hszDatabaseItemName(_T("DatabaseItem 1"));
	CHashString hszDatabaseItemType(_T("CDatabaseItem"));

	CreateObject( &hszDatabaseItemName, NULL, &hszDatabaseItemType );

	InitializeObject( &hszDatabaseItemName );

	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &hszDatabaseItemName;
	DWORD retVal = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );

	TestAssert( _T("Created Database Item found in engine"), retVal == MSG_HANDLED && param.bFound );

	DeleteObject( &hszDatabaseItemName );
}

void CDatabaseTest::TestDatabaseStructure()
{
	CHashString hszSchemaFile(_T(""));
	CHashString hszSchemaName(_T("Schema2"));
	CHashString hszSchemaItemName1(_T("SchemaItem1"));
	CHashString hszSchemaItemName2(_T("SchemaItem2"));
	CHashString hszSchemaItemName3(_T("SchemaItem3"));

	CreateTestSchema1( &hszSchemaName, &hszSchemaItemName1, &hszSchemaItemName2, &hszSchemaItemName3 );


	CHashString hszDefinedDatabaseName( _T("Database2") );
	CHashString hszDatabaseItemName1( _T("DatabaseItem1") );
	CHashString hszDatabaseItemName2( _T("DatabaseItem2") );
	CHashString hszDatabaseItemName3( _T("DatabaseItem3") );

	CreateTestDefinedDatabase( &hszDefinedDatabaseName, &hszDatabaseItemName1, &hszDatabaseItemName2, &hszDatabaseItemName3, &hszSchemaFile, &hszSchemaName );

	short id;
	StdString name;
	float health;

	IArchive *archive = CreateMemoryArchive();
	SerializeObject( &hszDatabaseItemName1, archive, true );
	archive->SetIsWriting( false );
	archive->Read( id );
	archive->Read( name );
	archive->Read( health );
	archive->Close();

	TestAssertEquals( _T("Verify serialized Database Item 1 id"), 1, id );
	TestAssertEquals( _T("Verify serialized Database Item 1 name"), _T("Test Item 1"), name );
	TestAssertEquals( _T("Verify serialized Database Item 1 health"), 50.0, health );

	archive = CreateMemoryArchive();
	SerializeObject( &hszDatabaseItemName2, archive, true );
	archive->SetIsWriting( false );
	archive->Read( id );
	archive->Read( name );
	archive->Read( health );
	archive->Close();

	TestAssertEquals( _T("Verify serialized Database Item 2 id"), 2, id );
	TestAssertEquals( _T("Verify serialized Database Item 2 name"), _T("Test Item 2"), name );
	TestAssertEquals( _T("Verify serialized Database Item 2 health"), 60.0, health );

	archive = CreateMemoryArchive();
	SerializeObject( &hszDatabaseItemName3, archive, true );
	archive->SetIsWriting( false );
	archive->Read( id );
	archive->Read( name );
	archive->Read( health );
	archive->Close();

	TestAssertEquals( _T("Verify serialized Database Item 3 id"), 3, id );
	TestAssertEquals( _T("Verify serialized Database Item 3 name"), _T("Test Item 3"), name );
	TestAssertEquals( _T("Verify serialized Database Item 3 health"), 70.0, health );

	DeleteObject( &hszDatabaseItemName1 );
	DeleteObject( &hszDatabaseItemName2 );
	DeleteObject( &hszDatabaseItemName3 );
	DeleteObject( &hszDefinedDatabaseName );

	DeleteObject( &hszSchemaItemName1 );
	DeleteObject( &hszSchemaItemName2 );
	DeleteObject( &hszSchemaItemName3 );
	DeleteObject( &hszSchemaName );
}

void CDatabaseTest::TestLoadSchemaFile()
{
	CHashString hszDatabaseSchemaName(_T("TestSchema1"));
	LoadFile( _T("TestSchema1.fxl") );

	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &hszDatabaseSchemaName;
	DWORD retVal = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );

	TestAssert( _T("Loaded schema found in engine"), retVal == MSG_HANDLED && param.bFound );

	IArchive *archive = CreateMemoryArchive();
	SerializeObject( &hszDatabaseSchemaName, archive, true );
	archive->SetIsWriting( false );

	float fVersion;
	StdString szName;
	int iNumItems;

	archive->Read( fVersion );
	archive->Read( szName );
	archive->Read( iNumItems );

	TestAssertEquals( _T("Verify serialized schema file version"), 1.0f, fVersion );
	TestAssertEquals( _T("Verify serialized schema file name"), _T("TestSchema1"), szName );
	TestAssertEquals( _T("Verify serialized schema file count"), 3, iNumItems );

	StdString szType;
	StdString szClass;
	StdString szDefault;

	archive->Read( fVersion );
	archive->Read( szName );
	archive->Read( szType );
	archive->Read( szClass );
	archive->Read( szDefault );

	TestAssertEquals( _T("Verify serialized schema item 1 version"), 1.0f, fVersion );
	TestAssertEquals( _T("Verify serialized schema item 1 name"), _T("Monster"), szName );
	TestAssertEquals( _T("Verify serialized schema item 1 type"), _T("string"), szType );
	TestAssertEquals( _T("Verify serialized schema item 1 class"), _T("edit"), szClass );
	TestAssertEquals( _T("Verify serialized schema item 1 default"), _T(""), szDefault );

	short siDefault = 0;
	archive->Read( fVersion );
	archive->Read( szName );
	archive->Read( szType );
	archive->Read( szClass );
	archive->Read( siDefault );

	TestAssertEquals( _T("Verify serialized schema item 2 version"), 1.0f, fVersion );
	TestAssertEquals( _T("Verify serialized schema item 2 name"), _T("Id"), szName );
	TestAssertEquals( _T("Verify serialized schema item 2 type"), _T("short int"), szType );
	TestAssertEquals( _T("Verify serialized schema item 2 class"), _T("key"), szClass );
	TestAssertEquals( _T("Verify serialized schema item 2 default"), -1, siDefault );

	int iDefault = 0;
	archive->Read( fVersion );
	archive->Read( szName );
	archive->Read( szType );
	archive->Read( szClass );
	archive->Read( iDefault );

	TestAssertEquals( _T("Verify serialized schema item 3 version"), 1.0f, fVersion );
	TestAssertEquals( _T("Verify serialized schema item 3 name"), _T("Health"), szName );
	TestAssertEquals( _T("Verify serialized schema item 3 type"), _T("long int"), szType );
	TestAssertEquals( _T("Verify serialized schema item 3 class"), _T("edit"), szClass );
	TestAssertEquals( _T("Verify serialized schema item 3 default"), 1000, iDefault );

	archive->Close();
	DeleteObject( &hszDatabaseSchemaName );
}

void CDatabaseTest::TestLoadDatabaseFile()
{
	CHashString hszDatabaseSchemaName(_T("TestSchema1"));

	CHashString hszDatabaseName(_T("TestDatabase1"));
	LoadFile( _T("TestDatabase1.dbx") );

	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &hszDatabaseName;
	DWORD retVal = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );

	TestAssert( _T("Loaded database found in engine"), retVal == MSG_HANDLED && param.bFound );

	IArchive *archive = CreateMemoryArchive();
	SerializeObject( &hszDatabaseName, archive, true );
	archive->SetIsWriting( false );

	float fVersion;
	StdString szName;
	StdString szSchemaFile;
	StdString szSchema;
	int iNumItems;
	archive->Read( fVersion );
	archive->Read( szName );
	archive->Read( szSchemaFile );
	archive->Read( szSchema );
	archive->Read( iNumItems );

	TestAssertEquals( _T("Verify serialized Defined Database version"), 1.0f, fVersion );
	TestAssertEquals( _T("Verify serialized Defined Database name"), _T("TestDatabase1"), szName );
	TestAssertEquals( _T("Verify serialized Defined Database schema file"), _T("TestSchema1.fxl"), szSchemaFile );
	TestAssertEquals( _T("Verify serialized Defined Database schema"), _T("TestSchema1"), szSchema );
	TestAssertEquals( _T("Verify serialized Defined Database count"), 3, iNumItems );

	StdString szMonster;
	short iId;
	int iHealth;

	archive->Read( szMonster );
	archive->Read( iId );
	archive->Read( iHealth );

	TestAssertEquals( _T("Verify serialized Database Item 1 name"), _T("TestMonster1"), szMonster );
	TestAssertEquals( _T("Verify serialized Database Item 1 ID"), 100, iId );
	TestAssertEquals( _T("Verify serialized Database Item 1 health"), 500, iHealth );

	archive->Read( szMonster );
	archive->Read( iId );
	archive->Read( iHealth );

	TestAssertEquals( _T("Verify serialized Database Item 2 name"), _T("TestMonster2"), szMonster );
	TestAssertEquals( _T("Verify serialized Database Item 2 ID"), 101, iId );
	TestAssertEquals( _T("Verify serialized Database Item 2 health"), 400, iHealth );

	archive->Read( szMonster );
	archive->Read( iId );
	archive->Read( iHealth );

	TestAssertEquals( _T("Verify serialized Database Item 3 name"), _T("TestMonster3"), szMonster );
	TestAssertEquals( _T("Verify serialized Database Item 3 ID"), 102, iId );
	TestAssertEquals( _T("Verify serialized Database Item 3 health"), 300, iHealth );

	DeleteObject( &hszDatabaseName );
	DeleteObject( &hszDatabaseSchemaName );
}

void CDatabaseTest::TestCreateDatabaseEntry()
{
	CHashString hszDatabaseEntryName(_T("TestDatabaseEntry1"));

	CHashString hszSchemaFile( _T("TestSchema1.fxl") );
	CHashString hszSchemaName( _T("TestSchema1") );
	CHashString hszDatabaseName( _T("TestEntryDatabase") );

	CreateTestDatabaseEntry1( &hszDatabaseEntryName, &hszSchemaFile, &hszSchemaName, &hszDatabaseName );

	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &hszDatabaseEntryName;
	DWORD retVal = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );

	TestAssert( _T("Created entry found in engine"), retVal == MSG_HANDLED && param.bFound );

	CHashString hszAttributeName( _T("Id") );
	IArchive *attributeArchive = CreateMemoryArchive();

	DATABASEATTRIBUTEPARAMS databaseAttributeParams;
	databaseAttributeParams.m_AttributeName = &hszAttributeName;
	databaseAttributeParams.m_AttributeArchive = attributeArchive;
	static DWORD msgHash_GetAttribute = CHashString(_T("GetAttribute")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetAttribute, sizeof( DATABASEATTRIBUTEPARAMS ), &databaseAttributeParams, &hszDatabaseEntryName, &CHashString(_T("CDatabaseEntry")) );

	short iId;
	attributeArchive->SetIsWriting( false );
	attributeArchive->SeekTo( 0 );
	attributeArchive->Read( iId );

	TestAssertEquals( _T("Verify default ID"), -1, iId );

	iId = 101;
	attributeArchive->SetIsWriting( true );
	attributeArchive->SeekTo( 0 );
	attributeArchive->Write( iId );

	attributeArchive->SetIsWriting( false );
	attributeArchive->SeekTo( 0 );
	static DWORD msgHash_SetAttribute = CHashString(_T("SetAttribute")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetAttribute, sizeof( DATABASEATTRIBUTEPARAMS ), &databaseAttributeParams, &hszDatabaseEntryName, &CHashString(_T("CDatabaseEntry")) );

	iId = 0;
	attributeArchive->SeekTo( 0 );
	m_ToolBox->SendMessage(msgHash_GetAttribute, sizeof( DATABASEATTRIBUTEPARAMS ), &databaseAttributeParams, &hszDatabaseEntryName, &CHashString(_T("CDatabaseEntry")) );

	attributeArchive->SetIsWriting( false );
	attributeArchive->SeekTo( 0 );
	attributeArchive->Read( iId );

	TestAssertEquals( _T("Verify ID value set"), 101, iId );

	attributeArchive->Close();

	DeleteObject( &hszDatabaseEntryName );
	DeleteObject( &hszDatabaseName );
	DeleteObject( &hszSchemaName );
}

void CDatabaseTest::TestGetAttributeType()
{
	CHashString hszDatabaseEntryName(_T("TestDatabaseEntry1"));
	CHashString hszSchemaFile( _T("TestSchema1.fxl") );
	CHashString hszSchemaName( _T("TestSchema1") );
	CHashString hszDatabaseName( _T("TestEntryDatabase") );

	CreateTestDatabaseEntry1( &hszDatabaseEntryName, &hszSchemaFile, &hszSchemaName, &hszDatabaseName );

	CHashString hszAttributeName( _T("Id") );
	IArchive *attributeTypeArchive = CreateMemoryArchive();

	DATABASEATTRIBUTEPARAMS databaseAttributeParams;
	databaseAttributeParams.m_AttributeName = &hszAttributeName;
	databaseAttributeParams.m_AttributeTypeArchive = attributeTypeArchive;
	static DWORD msgHash_GetAttribute = CHashString(_T("GetAttribute")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetAttribute, sizeof( DATABASEATTRIBUTEPARAMS ), &databaseAttributeParams, &hszDatabaseEntryName, &CHashString(_T("CDatabaseEntry")) );

	StdString szAttributeType;
	attributeTypeArchive->SeekTo( 0 );
	attributeTypeArchive->SetIsWriting( false );
	attributeTypeArchive->Read( szAttributeType );

	TestAssertEquals( _T( "Verify attribute type for value in entry" ), _T("short int"), szAttributeType );

	DeleteObject( &hszDatabaseEntryName );
	DeleteObject( &hszDatabaseName );
	DeleteObject( &hszSchemaName );
}

void CDatabaseTest::TestDatabaseValueEdit()
{
	CHashString hszSchemaName( _T("TestSchema1") );
	CHashString hszDatabaseFile( _T("TestDatabase1.dbx") );
	CHashString hszDatabaseName( _T("TestDatabase1") );

	LoadFile( (TCHAR*)hszDatabaseFile.GetString() );

	CHashString hszAttributeName( _T("Health") );
	IArchive *attributeArchive = CreateMemoryArchive();

//	m_dialogEditControl->ReplaceSel(_T("    Verifying get value for database item: "));
	DATABASEATTRIBUTEPARAMS databaseAttributeParams;
	databaseAttributeParams.m_Index = 1;
	databaseAttributeParams.m_AttributeName = &hszAttributeName;
	databaseAttributeParams.m_AttributeArchive = attributeArchive;
	static DWORD msgHash_GetAttribute = CHashString(_T("GetAttribute")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_GetAttribute, sizeof( DATABASEATTRIBUTEPARAMS ), &databaseAttributeParams, &hszDatabaseName, &CHashString(_T("CDefinedDatabase")) );

	int iHealth = 0;
	attributeArchive->SetIsWriting( false );
	attributeArchive->Read( iHealth );

	TestAssertEquals( _T("Verify get value"), 400, iHealth );

	DeleteObject( &hszDatabaseName );
	DeleteObject( &hszSchemaName );
}
void CDatabaseTest::TestLoadGlobalAttibutes()
{
	CHashString globalAttributesGroupName( _T("TestGlobalAttributes") );
	CHashString globalAttributesSchemaFile( _T("TestSchema1.fxl") );
	CHashString globalAttributesSchemaName( _T("TestSchema1") );

	LOADGLOBALATTRIBUTESPARAMS loadGlobalAttributesParams;
	loadGlobalAttributesParams.Name = &globalAttributesGroupName;
	loadGlobalAttributesParams.SchemaFile = &globalAttributesSchemaFile;
	loadGlobalAttributesParams.SchemaName = &globalAttributesSchemaName;

	static DWORD msgLoadGlobalAttributes = CHashString( _T("LoadGlobalAttributes") ).GetUniqueID();
	m_ToolBox->SendMessage(msgLoadGlobalAttributes, sizeof(LOADGLOBALATTRIBUTESPARAMS), &loadGlobalAttributesParams);

	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &globalAttributesGroupName;
	DWORD retVal = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );

	TestAssert( _T("Verify database for global attributes loaded"), retVal == MSG_HANDLED && param.bFound );

	StdString szDatabaseItemName = globalAttributesGroupName.GetString();
	szDatabaseItemName += _T("_SingleEntry");
	CHashString hszDatabaseItemName( szDatabaseItemName );
	param.hszName = &hszDatabaseItemName;
	param.bFound = false;
	retVal = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );

	TestAssert( _T("Verify single database entry for attributes loaded"), retVal == MSG_HANDLED && param.bFound );

	CHashString attributeName( _T("Health") );
	IArchive *attributeArchive = CreateMemoryArchive();

	DATABASEATTRIBUTEPARAMS databaseAttributeParams;
	databaseAttributeParams.m_AttributeName = &attributeName;
	databaseAttributeParams.m_AttributeArchive = attributeArchive;
	static DWORD msgHash_GetGlobalAttribute = CHashString(_T("GetGlobalAttribute")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_GetGlobalAttribute, sizeof( DATABASEATTRIBUTEPARAMS ), &databaseAttributeParams );

	int iHealth;
	attributeArchive->SetIsWriting( false );
	attributeArchive->SeekTo( 0 );
	attributeArchive->Read( iHealth );

	TestAssertEquals( _T("Verify single database entry default values"), 1000, iHealth);

	DeleteObject( &globalAttributesGroupName );
	DeleteObject( &globalAttributesSchemaName );
}
void CDatabaseTest::TestIndexing()
{
	CHashString hszSchemaName( _T("TestSchema1") );
	CHashString hszDatabaseFile( _T("TestDatabase1.dbx") );
	CHashString hszDatabaseName( _T("TestDatabase1") );

	CHashString hszKeyAttributeName( _T("Id") );

	LoadFile( (TCHAR*)hszDatabaseFile.GetString() );

	short int iId = 102;
	IArchive *attributeArchive = CreateMemoryArchive();
	attributeArchive->SetIsWriting( true );
	attributeArchive->Write( iId );
	attributeArchive->SetIsWriting( false );

	DATABASEATTRIBUTEPARAMS databaseAttributeParams;
	databaseAttributeParams.m_Index = -1;
	databaseAttributeParams.m_AttributeName = &hszKeyAttributeName;
	databaseAttributeParams.m_AttributeArchive = attributeArchive;

	static DWORD msgHash_GetAttribute = CHashString(_T("GetIndex")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_GetAttribute, sizeof( DATABASEATTRIBUTEPARAMS ), &databaseAttributeParams, &hszDatabaseName, &CHashString(_T("CDefinedDatabase")) );

	TestAssertEquals( _T("Verify get index of key value"), 2, databaseAttributeParams.m_Index );

	DeleteObject( &hszDatabaseName );
	DeleteObject( &hszSchemaName );
}

///////////////// Helper functions for creating prototype databases /////////////////////////////////////////////////

void CDatabaseTest::CreateTestSchema1( IHashString *name, IHashString *schemaItem1, IHashString *schemaItem2, IHashString *schemaItem3 )
{
	CHashString hszDatabaseSchemaType(_T("CDatabaseSchema"));
	CreateObject( name, NULL, &hszDatabaseSchemaType );

	IArchive *archive = CreateMemoryArchive();
	archive->SetIsWriting( true );
	archive->Write( 1.0f );
	archive->Write( _T("") );
	archive->Write( (int)3 );
	SerializeObject( name, archive, false );
	archive->Close();

	CreateTestSchemaItem1( schemaItem1, name );
	CreateTestSchemaItem2( schemaItem2, name );
	CreateTestSchemaItem3( schemaItem3, name );
}

void CDatabaseTest::CreateTestSchemaItem1( IHashString *name, IHashString *parentName )
{
	CHashString hszItemSchemaType(_T("CSchemaItem"));
	CreateObject( name, parentName, &hszItemSchemaType );

	IArchive *archive = CreateMemoryArchive();
	archive->SetIsWriting( true );
	archive->Write( 1.0f );
	archive->Write( _T("Id") );
	archive->Write( _T("short int") );
	archive->Write( _T("key") );
	archive->Write( (int)(-1) );
	SerializeObject( name, archive, false );
	archive->Close();
}

void CDatabaseTest::CreateTestSchemaItem2( IHashString *name, IHashString *parentName )
{
	CHashString hszItemSchemaType(_T("CSchemaItem"));
	CreateObject( name, parentName, &hszItemSchemaType );

	IArchive *archive = CreateMemoryArchive();
	archive->SetIsWriting( true );
	archive->Write( 1.0f );
	archive->Write( _T("Name") );
	archive->Write( _T("string") );
	archive->Write( _T("edit") );
	archive->Write( _T("") );
	SerializeObject( name, archive, false );
	archive->Close();
}

void CDatabaseTest::CreateTestSchemaItem3( IHashString *name, IHashString *parentName )
{
	CHashString hszItemSchemaType(_T("CSchemaItem"));
	CreateObject( name, parentName, &hszItemSchemaType );

	IArchive *archive = CreateMemoryArchive();
	archive->SetIsWriting( true );
	archive->Write( 1.0f );
	archive->Write( _T("Health") );
	archive->Write( _T("float") );
	archive->Write( _T("edit") );
	archive->Write( 100.0f );
	SerializeObject( name, archive, false );
	archive->Close();
}


void CDatabaseTest::CreateTestDefinedDatabase( IHashString *name, IHashString *databaseItem1, IHashString *databaseItem2, IHashString *databaseItem3, IHashString *schemaFile, IHashString *schemaName )
{
	CHashString hszDefinedDatabaseType(_T("CDefinedDatabase"));
	CreateObject( name, NULL, &hszDefinedDatabaseType );

	IArchive *archive = CreateMemoryArchive();
	archive->SetIsWriting( true );
	archive->Write( 1.0f );
	archive->Write( _T("") );
	archive->Write( schemaFile->GetString() );
	archive->Write( schemaName->GetString() );
	archive->Write( (int)3 );
	SerializeObject( name, archive, false );
	archive->Close();

	CreateTestDatabaseItem1( databaseItem1, name );
	CreateTestDatabaseItem2( databaseItem2, name );
	CreateTestDatabaseItem3( databaseItem3, name );
}

void CDatabaseTest::CreateTestDatabaseItem1( IHashString *name, IHashString *parentName )
{
	CHashString hszDatabaseItemType(_T("CDatabaseItem"));
	CreateObject( name, parentName, &hszDatabaseItemType );

	IArchive *archive = CreateMemoryArchive();
	archive->SetIsWriting( true );
	archive->Write( (short)1 );
	archive->Write( _T("Test Item 1") );
	archive->Write( 50.0f );
	SerializeObject( name, archive, false );
	archive->Close();
}

void CDatabaseTest::CreateTestDatabaseItem2( IHashString *name, IHashString *parentName )
{
	CHashString hszDatabaseItemType(_T("CDatabaseItem"));
	CreateObject( name, parentName, &hszDatabaseItemType );

	IArchive *archive = CreateMemoryArchive();
	archive->SetIsWriting( true );
	archive->Write( (short)2 );
	archive->Write( _T("Test Item 2") );
	archive->Write( 60.0f );
	SerializeObject( name, archive, false );
	archive->Close();
}

void CDatabaseTest::CreateTestDatabaseItem3( IHashString *name, IHashString *parentName )
{
	CHashString hszDatabaseItemType(_T("CDatabaseItem"));
	CreateObject( name, parentName, &hszDatabaseItemType );

	IArchive *archive = CreateMemoryArchive();
	archive->SetIsWriting( true );
	archive->Write( (short)3 );
	archive->Write( _T("Test Item 3") );
	archive->Write( 70.0f );
	SerializeObject( name, archive, false );
	archive->Close();
}

void CDatabaseTest::CreateTestDatabaseEntry1( IHashString *name, IHashString *schemaFile, IHashString *schemaName, IHashString *databaseName )
{
	CHashString hszDatabaseEntryType(_T("CDatabaseEntry"));
	CreateObject( name, NULL, &hszDatabaseEntryType );

	IArchive *archive = CreateMemoryArchive();
	archive->SetIsWriting( true );
	archive->Write( schemaFile->GetString() );
	archive->Write( schemaName->GetString() );
	archive->Write( databaseName->GetString() );
	SerializeObject( name, archive, false );
	InitializeObject( name );
	archive->Close();
}