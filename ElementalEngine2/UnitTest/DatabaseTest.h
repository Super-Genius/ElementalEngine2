#ifndef DATABASETEST_H
#define DATABASETEST_H

class CDatabaseTest : public CTestBase
{
public:
	CDatabaseTest( CRichEditCtrl *dialogEditControl );
	~CDatabaseTest();

	void Initialize();

	void TestCreateSchema();
	void TestCreateSchemaItem();
	void TestSerializeSchema();
	void TestCreateDefinedDatabase();
	void TestCreateDatabaseItem();
	void TestDatabaseStructure();
	void TestLoadSchemaFile();
	void TestLoadDatabaseFile();
	void TestCreateDatabaseEntry();
	void TestGetAttributeType();
	void TestDatabaseValueEdit();
	void TestLoadGlobalAttibutes();
	void TestIndexing();

private:
	void CreateTestSchema1( IHashString *name, IHashString *schemaItem1, IHashString *schemaItem2, IHashString *schemaItem3 );
	void CreateTestSchemaItem1( IHashString *name, IHashString *parentName );
	void CreateTestSchemaItem2( IHashString *name, IHashString *parentName );
	void CreateTestSchemaItem3( IHashString *name, IHashString *parentName );

	void CreateTestDefinedDatabase( IHashString *name, IHashString *databaseItem1, IHashString *databaseItem2, IHashString *databaseItem3, IHashString *schemaFile, IHashString *schemaName );
	void CreateTestDatabaseItem1( IHashString *name, IHashString *parentName );
	void CreateTestDatabaseItem2( IHashString *name, IHashString *parentName );
	void CreateTestDatabaseItem3( IHashString *name, IHashString *parentName );

	void CreateTestDatabaseEntry1( IHashString *name, IHashString *schemaFile, IHashString *schemaName, IHashString *databaseName );
};

#endif // #ifndef DATABASETEST_H