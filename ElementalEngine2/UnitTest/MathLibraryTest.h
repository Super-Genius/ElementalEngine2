#ifndef MathLibraryTEST_H
#define MathLibraryTEST_H

class CMathLibraryTest;

typedef bool (CMathLibraryTest::*MATHLIBRARYTESTCASE)();
typedef list<MATHLIBRARYTESTCASE> MATHLIBRARYTESTCASELIST;

class CMathLibraryTest : public CTestBase
{
public:
	CMathLibraryTest( CRichEditCtrl *dialogEditControl );
	~CMathLibraryTest();

	virtual void Initialize();
	virtual int RunTests();

private:
	bool TestLog2();
	bool TestLogBase();
	bool TestIsPowerOfTwo();
	bool TestNearestPowerOfTwo();
	bool TestRadToDeg();
	bool TestDegToRad();
	bool TestGetATan();
	bool TestGetACos();
	bool TestGetASin();
	bool TestGetTan();
	bool TestGetCos();
	bool TestGetSin();
	bool TestMatrixLookAt();
	bool TestMatrixToEuler();
	bool TestEulerToMatrix();
	bool TestEulerToQuaternion();
	bool TestQuaternionToEuler();
	bool TestCrossProduct();
	bool TestMatrixToQuaternionToEulerToMatrix();
	bool TestMatrixToEulerToQuaternionToMatrix();
	bool TestVectorRotations();

	MATHLIBRARYTESTCASELIST m_MathLibraryTestCaseList;
};

#endif // #ifndef MathLibraryTEST_H