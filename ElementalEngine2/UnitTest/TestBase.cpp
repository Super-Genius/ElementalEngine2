#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "stdafx.h"

CTestBase::CTestBase()
{
	m_dialogEditControl = NULL;
}

CTestBase::CTestBase( CRichEditCtrl *dialogEditControl )
{
	m_dialogEditControl = dialogEditControl;
}

CTestBase::CTestBase( const StdString &szTestFixture, CRichEditCtrl *dialogEditControl )
{
	m_dialogEditControl = dialogEditControl;
	m_szTestFixture = szTestFixture;
}

CTestBase::~CTestBase()
{
}

void CTestBase::Initialize()
{
	m_ToolBox = EngineGetToolBox();
}

void CTestBase::DeInitialize()
{
}

int CTestBase::RunTests()
{
	m_iFailedTests = 0;
	StartTestFixture();

	TESTCASELIST::iterator testCaseIterator;
	for( testCaseIterator = m_TestCaseList.begin(); testCaseIterator != m_TestCaseList.end(); testCaseIterator++ )
	{
		TESTPAIR testPair = *testCaseIterator;
		StdString *szTest = &testPair.first;
		TESTCASE functionPointer = testPair.second;

		StartTestGroup( *szTest );
		(this->*functionPointer)();
		EndTestGroup();
	}

	EndTestFixture();
	return m_iFailedTests;
}

void CTestBase::StartTestFixture()
{
	StdString szOutput = _T("Start test fixture - ") + m_szTestFixture + _T("\n");
	m_dialogEditControl->ReplaceSel( szOutput );
}

void CTestBase::EndTestFixture()
{
	StdString szOutput = _T("End test fixture - ") + m_szTestFixture + _T(": ");
	if( m_iFailedTests == 0 )
	{
		szOutput += _T( "PASSED" );
	}
	else
	{
		szOutput += _T( "FAILED" );
	}
	szOutput += _T("\n");
	m_dialogEditControl->ReplaceSel( szOutput );
}

void CTestBase::StartTestGroup( const StdString &szTestGroup )
{
	m_TestStack.push_back( TESTGROUPPAIR( szTestGroup, true ) );
	int iStackSize = (int)m_TestStack.size();
	StdString szSpacing = _T("");
	for( int i = 0; i < iStackSize; i++ )
	{
		szSpacing += _T("  ");
	}
	StdString szOutput = szSpacing + _T("Start test group - ") + szTestGroup + _T("\n");
	m_dialogEditControl->ReplaceSel( szOutput );
}

void CTestBase::EndTestGroup()
{
	int iStackSize = (int)m_TestStack.size();
	if( iStackSize > 0 )
	{
		TESTGROUPPAIR *lastPair = &m_TestStack.back();
		StdString szTestGroup = lastPair->first;
		bool bTestResult = lastPair->second;

		StdString szSpacing = _T("");
		for( int i = 0; i < iStackSize; i++ )
		{
			szSpacing += _T("  ");
		}
		StdString szOutput = szSpacing + _T("End test group - ") + szTestGroup + _T(": ");
		if( bTestResult == true )
		{
			szOutput += _T( "PASSED" );
		}
		else
		{
			szOutput += _T( "FAILED" );
		}
		szOutput += _T("\n");
		m_TestStack.pop_back();
		m_dialogEditControl->ReplaceSel( szOutput );
	}
}

void CTestBase::RegisterTestCase( const StdString &szTestCase, TESTCASE testCase )
{
	m_TestCaseList.push_back( TESTPAIR( szTestCase, testCase ) );
}

void CTestBase::TestAssert( const StdString &szTest, bool bResult )
{
	TestAssertEquals( szTest, true, bResult );
}

void CTestBase::TestAssertEquals( const StdString &szTest, bool bExpected, bool bActual )
{
	int iStackSize = (int)m_TestStack.size();

	StdString szSpacing = _T("");
	for( int i = 0; i < iStackSize + 1; i++ )
	{
		szSpacing += _T("  ");
	}
	StdString szOutput = szSpacing + szTest + _T(": ");
	if( bExpected == bActual )
	{
		szOutput += _T( "PASSED" );
	}
	else
	{
		szOutput += _T( "FAILED" );
		StdString szExpected = bExpected ? _T("true") : _T("false");
		StdString szActual = bActual ? _T("true") : _T("false");
		FailedAssertEquals( &szOutput, szSpacing, szExpected, szActual );
	}
	szOutput += _T("\n");
	m_dialogEditControl->ReplaceSel( szOutput );
}

void CTestBase::TestAssertEquals( const StdString &szTest, int iExpected, int iActual )
{
	int iStackSize = (int)m_TestStack.size();

	StdString szSpacing = _T("");
	for( int i = 0; i < iStackSize + 1; i++ )
	{
		szSpacing += _T("  ");
	}
	StdString szOutput = szSpacing + szTest + _T(": ");
	if( iExpected == iActual )
	{
		szOutput += _T( "PASSED" );
	}
	else
	{
		szOutput += _T( "FAILED" );
		static char szBuffer[64];
		StdString szExpected = _itoa( iExpected, szBuffer, 10 );
		StdString szActual = _itoa( iActual, szBuffer, 10 );
		FailedAssertEquals( &szOutput, szSpacing, szExpected, szActual );
	}
	szOutput += _T("\n");
	m_dialogEditControl->ReplaceSel( szOutput );
}

void CTestBase::TestAssertEquals( const StdString &szTest, float fExpected, float fActual )
{
	int iStackSize = (int)m_TestStack.size();

	StdString szSpacing = _T("");
	for( int i = 0; i < iStackSize + 1; i++ )
	{
		szSpacing += _T("  ");
	}
	StdString szOutput = szSpacing + szTest + _T(": ");
	if( fExpected == fActual )
	{
		szOutput += _T( "PASSED" );
	}
	else
	{
		szOutput += _T( "FAILED" );
		static char szBuffer[64];
		sprintf( szBuffer, "%f", fExpected );
		StdString szExpected = szBuffer;
		sprintf( szBuffer, "%f", fActual );
		StdString szActual = szBuffer;
		FailedAssertEquals( &szOutput, szSpacing, szExpected, szActual );
	}
	szOutput += _T("\n");
	m_dialogEditControl->ReplaceSel( szOutput );
}

void CTestBase::TestAssertEquals( const StdString &szTest, const StdString &szExpected, const StdString &szActual )
{
	int iStackSize = (int)m_TestStack.size();

	StdString szSpacing = _T("");
	for( int i = 0; i < iStackSize + 1; i++ )
	{
		szSpacing += _T("  ");
	}
	StdString szOutput = szSpacing + szTest + _T(": ");
	if( szExpected == szActual )
	{
		szOutput += _T( "PASSED" );
	}
	else
	{
		szOutput += _T( "FAILED\n" );
		FailedAssertEquals( &szOutput, szSpacing, szExpected, szActual );
	}
	szOutput += _T("\n");
	m_dialogEditControl->ReplaceSel( szOutput );
}

void CTestBase::FailedAssertEquals( StdString *szOutput, const StdString &szSpacing, const StdString &szExpected, const StdString &szActual )
{
	int iStackSize = (int)m_TestStack.size();
	if( iStackSize > 0 )
	{
		m_iFailedTests++;
		(*szOutput) += szSpacing + _T( "  Expected: " ) + szExpected + _T("\n");
		(*szOutput) += szSpacing + _T( "  Actual: " ) + szActual + _T("\n");

		TESTSTACK::iterator testStackIterator = m_TestStack.begin();
		for( testStackIterator; testStackIterator != m_TestStack.end(); testStackIterator++ )
		{
			testStackIterator->second = false;
		}
	}
}

IArchive *CTestBase::CreateMemoryArchive()
{
	CREATEARCHIVE ca;
	CHashString memType(_T("Memory"));

	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &memType;

	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
	ASSERT( retVal == MSG_HANDLED );
	
	return ca.archive;
}

DWORD CTestBase::CreateObject( IHashString *objectName, IHashString *objectParentName, IHashString *objectTypeName )
{
	CREATEOBJECTPARAMS cop;

	cop.name = objectName;
	cop.parentName = objectParentName;
	cop.typeName = objectTypeName;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);
//	ASSERT( retVal == MSG_HANDLED );
	return retVal;
}

DWORD CTestBase::InitializeObject( IHashString *objectName )
{
	INITOBJECTPARAMS initObjectParams;
	initObjectParams.name = objectName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &initObjectParams );

	return retVal;
}

DWORD CTestBase::SerializeObject( IHashString *objectName, IArchive *archive, bool isWriting )
{
	archive->SetIsWriting( isWriting );

	SERIALIZEOBJECTPARAMS sop;
	sop.name = objectName;
	sop.archive = archive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop );
//	ASSERT( retVal == MSG_HANDLED );
	return retVal;
}

DWORD CTestBase::DeleteObject( IHashString *objectName )
{
	DELETEOBJECTPARAMS dop;
	dop.name = objectName;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop );
//	ASSERT( retVal == MSG_HANDLED );
	return retVal;
}

bool CTestBase::FindObject( IHashString *objectName )
{
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = objectName;
	DWORD retVal = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );

	return retVal == MSG_HANDLED && param.bFound;
}

DWORD CTestBase::LoadFile( TCHAR* fileName )
{
	DWORD retval = MSG_NOT_HANDLED;
	LOADFILEEXTPARAMS lfep;
	lfep.fileName = fileName;
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
	return retval;
}

////////////////////////////////////////////////////////////////////////////////////
