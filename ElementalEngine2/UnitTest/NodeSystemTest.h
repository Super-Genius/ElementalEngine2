#ifndef NODESYSTEMTEST_H
#define NODESYSTEMTEST_H

class CNodeSystemTest;

typedef bool (CNodeSystemTest::*NODESYSTEMTESTCASE)();
typedef list<NODESYSTEMTESTCASE> NODESYSTEMTESTCASELIST;

class CNodeSystemTest : public CTestBase
{
public:
	CNodeSystemTest( CRichEditCtrl *dialogEditControl );
	~CNodeSystemTest();

	virtual void Initialize();
	virtual int RunTests();

private:
	NODESYSTEMTESTCASELIST m_NodeSystemTestCaseList;
	bool TestCreateNode();
	bool TestCreateNodeConnection();
	bool TestCreateNodeSystem();
};

#endif // #ifndef NODESYSTEMTEST_H