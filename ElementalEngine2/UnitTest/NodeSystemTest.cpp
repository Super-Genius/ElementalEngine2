#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "stdafx.h"

CNodeSystemTest::CNodeSystemTest( CRichEditCtrl *dialogEditControl )
	: CTestBase( dialogEditControl )
{
}

CNodeSystemTest::~CNodeSystemTest()
{
}

void CNodeSystemTest::Initialize()
{
	CTestBase::Initialize();

	m_NodeSystemTestCaseList.push_back( &CNodeSystemTest::TestCreateNode );
	m_NodeSystemTestCaseList.push_back( &CNodeSystemTest::TestCreateNodeConnection );
	m_NodeSystemTestCaseList.push_back( &CNodeSystemTest::TestCreateNodeSystem );
}

int CNodeSystemTest::RunTests()
{
	int failedTests = 0;
	NODESYSTEMTESTCASELIST::iterator testCaseIterator;
	for( testCaseIterator = m_NodeSystemTestCaseList.begin(); testCaseIterator != m_NodeSystemTestCaseList.end(); testCaseIterator++ )
	{
		NODESYSTEMTESTCASE functionPointer = *testCaseIterator;
		bool testResult = (this->*functionPointer)();
		SUBREPORT( testResult );
		if( !testResult )
		{
			failedTests++;
		}
	}
	return failedTests;
}

bool CNodeSystemTest::TestCreateNode()
{
	bool result = true;
	bool subResult = false;

	CHashString hszNodeName(_T("Node1"));
	CHashString hszNodeType(_T("CNode"));

	m_dialogEditControl->ReplaceSel(_T("  Testing Creation of Node 1 - \n"));
	CreateObject( &hszNodeName, NULL, &hszNodeType );

	InitializeObject( &hszNodeName );

	m_dialogEditControl->ReplaceSel(_T("    Created node found in engine: "));

	subResult = FindObject( &hszNodeName );
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("    Verify default values: "));
	IArchive *archive = CreateMemoryArchive();
	SerializeObject( &hszNodeName, archive, true );

	float fVersion;
	float fX;
	float fY;
	float fZ;
	archive->SetIsWriting( false );
	archive->Read( fVersion );
	archive->Read( fX );
	archive->Read( fY );
	archive->Read( fZ );
	archive->Close();

	subResult = fVersion == 1.0f;
	subResult = subResult && fX == 0.0f;
	subResult = subResult && fY == 0.0f;
	subResult = subResult && fZ == 0.0f;
	SUBREPORT( subResult );

	result = result && subResult;

	DeleteObject( &hszNodeName );

	m_dialogEditControl->ReplaceSel(_T("  TestCreateNode overall: "));
	return result;
}

bool CNodeSystemTest::TestCreateNodeConnection()
{
	bool result = true;
	bool subResult = false;

	CHashString hszNodeConnectionName(_T("NodeConnection1"));
	CHashString hszNodeConnectionType(_T("CNodeConnection"));

	m_dialogEditControl->ReplaceSel(_T("  Testing Creation of Node Connection 1 - \n"));
	CreateObject( &hszNodeConnectionName, NULL, &hszNodeConnectionType );

	InitializeObject( &hszNodeConnectionName );

	m_dialogEditControl->ReplaceSel(_T("    Created node found in engine: "));

	subResult = FindObject( &hszNodeConnectionName );
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("    Verify default values: "));
	IArchive *archive = CreateMemoryArchive();
	SerializeObject( &hszNodeConnectionName, archive, true );

	float fVersion;
	StdString szNode1;
	StdString szNode2;
	archive->SetIsWriting( false );
	archive->Read( fVersion );
	archive->Read( szNode1 );
	archive->Read( szNode2 );
	archive->Close();

	subResult = fVersion == 1.0f;
	subResult = subResult && szNode1 == "";
	subResult = subResult && szNode2 == "";
	SUBREPORT( subResult );

	result = result && subResult;

	DeleteObject( &hszNodeConnectionName );
	m_dialogEditControl->ReplaceSel(_T("  TestCreateNodeConnection overall: "));
	return result;
}

bool CNodeSystemTest::TestCreateNodeSystem()
{
	bool result = true;
	bool subResult = false;

	CHashString hszNodeSystemName(_T("NodeSystem1"));
	CHashString hszNodeSystemType(_T("CNodeSystem"));

	m_dialogEditControl->ReplaceSel(_T("  Testing Creation of Node System 1 - \n"));
	CreateObject( &hszNodeSystemName, NULL, &hszNodeSystemType );

	InitializeObject( &hszNodeSystemName );

	m_dialogEditControl->ReplaceSel(_T("    Created node system found in engine: "));

	subResult = FindObject( &hszNodeSystemName );
	SUBREPORT( subResult );
	result = result && subResult;

	CHashString hszNodeName(_T("Node1"));
	CHashString hszNodeType(_T("CNode"));

	CreateObject( &hszNodeName, &hszNodeSystemName, &hszNodeType );

	InitializeObject( &hszNodeName );
	IArchive *archive = CreateMemoryArchive();
	archive->SetIsWriting( true );
	archive->Write( 1.0f, _T("Version") );
	archive->Write( 0.0f, _T("X") );
	archive->Write( 0.0f, _T("Y") );
	archive->Write( 0.0f, _T("Z") );
	archive->SetIsWriting( false );

	SerializeObject( &hszNodeName, archive, false );
	archive->Close();

	CHashString hszNodeConnectionName(_T("NodeConnection1"));
	CHashString hszNodeConnectionType(_T("CNodeConnection"));

	CreateObject( &hszNodeConnectionName, &hszNodeSystemName, &hszNodeConnectionType );

	InitializeObject( &hszNodeConnectionName );
	archive = CreateMemoryArchive();
	archive->SetIsWriting( true );
	archive->Write( 1.0f, _T("Version") );
	archive->Write( "", _T("Node1") );
	archive->Write( "", _T("Node2") );
	archive->SetIsWriting( false );

	SerializeObject( &hszNodeConnectionName, archive, false );
	archive->Close();

	archive = CreateMemoryArchive();
	archive->SetIsWriting( true );
	SerializeObject( &hszNodeSystemName, archive, true );
	float fVersion;
	int iNumNodes;
	int iNumConnections;

	archive->SetIsWriting( false );
	archive->Read( fVersion );
	archive->Read( iNumNodes );
	archive->Read( iNumConnections );
	archive->Close();

	m_dialogEditControl->ReplaceSel(_T("    Verify values: "));
	subResult = fVersion == 1.0f;
	subResult = subResult && iNumNodes == 1;
	subResult = subResult && iNumConnections == 1;
	SUBREPORT( subResult );
	result = result && subResult;

	DeleteObject( &hszNodeSystemName );

	m_dialogEditControl->ReplaceSel(_T("    Objects properly deleted: "));
	subResult = !FindObject( &hszNodeSystemName );
	subResult = subResult && !FindObject( &hszNodeName );
	subResult = subResult && !FindObject( &hszNodeConnectionName );
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestCreateNodeSystem overall: "));
	return result;
}