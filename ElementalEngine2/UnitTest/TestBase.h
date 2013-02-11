#ifndef TESTBASE_H
#define TESTBASE_H

class CTestBase;

typedef void (CTestBase::*TESTCASE)();
typedef pair< StdString, TESTCASE > TESTPAIR;
typedef list<TESTPAIR> TESTCASELIST;

// first = Name of the test group, second = test group result
typedef pair< StdString, bool > TESTGROUPPAIR;
typedef list<TESTGROUPPAIR> TESTSTACK;

class CTestBase
{
public:
	CTestBase();
	CTestBase( CRichEditCtrl *dialogEditControl );
	CTestBase( const StdString &szTestFixture, CRichEditCtrl *dialogEditControl );
	virtual ~CTestBase();
	virtual void Initialize();
	virtual void DeInitialize();

	// Performs all test cases and returns the number of failed tests
	virtual int RunTests();

protected:
	void StartTestFixture();
	void EndTestFixture();
	void StartTestGroup( const StdString &szTestGroup );
	void EndTestGroup();

	void RegisterTestCase( const StdString &szTestCase, TESTCASE testCase );

	void TestAssert( const StdString &szTest, bool bResult );
	void TestAssertEquals( const StdString &szTest, bool bExpected, bool bActual );
	void TestAssertEquals( const StdString &szTest, int iExpected, int iActual );
	void TestAssertEquals( const StdString &szTest, float fExpected, float fActual );
	void TestAssertEquals( const StdString &szTest, const StdString &szExpected, const StdString &szActual );

	void FailedAssertEquals( StdString *szOutput, const StdString &szSpacing, const StdString &szExpected, const StdString &szActual );

	virtual IArchive *CreateMemoryArchive();
	virtual DWORD CreateObject( IHashString *objectName, IHashString *objectParentName, IHashString *objectTypeName );
	virtual DWORD InitializeObject( IHashString *objectName );
	virtual DWORD SerializeObject( IHashString *objectName, IArchive *archive, bool isWriting );
	virtual DWORD DeleteObject( IHashString *objectName );
	virtual bool FindObject( IHashString *objectName );
	virtual DWORD LoadFile( TCHAR* fileName );

	void SUBREPORT(bool success)
	{
		if (m_dialogEditControl != NULL)
		{
			if (success)
			{
				LogMessage(_T("Success!\n"));
			}
			else
			{
				LogMessage(_T("Failed!\n"));
			}
		}
	}

	void LogMessage(LPCTSTR szMessage)
	{
		m_dialogEditControl->ReplaceSel(szMessage);
		TRACE(szMessage);
	}

	IToolBox *m_ToolBox;
	CRichEditCtrl *m_dialogEditControl;

private:
	TESTCASELIST m_TestCaseList;
	TESTSTACK m_TestStack;
	int m_iFailedTests;
	StdString m_szTestFixture;
};

#endif // #ifndef TESTBASE_H
