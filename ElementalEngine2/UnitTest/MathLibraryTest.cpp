#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "stdafx.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

CMathLibraryTest::CMathLibraryTest( CRichEditCtrl *dialogEditControl )
	: CTestBase( dialogEditControl )
{
}

CMathLibraryTest::~CMathLibraryTest()
{
}

void CMathLibraryTest::Initialize()
{
	CTestBase::Initialize();

	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestLog2 );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestLogBase );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestIsPowerOfTwo );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestNearestPowerOfTwo );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestRadToDeg );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestDegToRad );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestGetATan );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestGetACos );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestGetASin );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestGetTan );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestGetCos );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestGetSin );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestMatrixLookAt );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestMatrixToEuler );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestEulerToMatrix );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestEulerToQuaternion );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestQuaternionToEuler );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestCrossProduct );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestMatrixToQuaternionToEulerToMatrix );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestMatrixToEulerToQuaternionToMatrix );
	m_MathLibraryTestCaseList.push_back( &CMathLibraryTest::TestVectorRotations );
}

int CMathLibraryTest::RunTests()
{
	int failedTests = 0;
	MATHLIBRARYTESTCASELIST::iterator testCaseIterator;
	for( testCaseIterator = m_MathLibraryTestCaseList.begin(); testCaseIterator != m_MathLibraryTestCaseList.end(); testCaseIterator++ )
	{
		MATHLIBRARYTESTCASE functionPointer = *testCaseIterator;
		bool testResult = (this->*functionPointer)();
		SUBREPORT( testResult );
		if( !testResult )
		{
			failedTests++;
		}
	}
	return failedTests;
}

bool CMathLibraryTest::TestLog2()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing Log2 - \n"));
	MathUtil math;

	float log2one = math.Log2( 1 );
	subResult = log2one == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    Log2(1): "));
	SUBREPORT( subResult );
	result = result && subResult;

	float log2two = math.Log2( 2 );
	subResult = log2two == 1.0f;
	m_dialogEditControl->ReplaceSel(_T("    Log2(2): "));
	SUBREPORT( subResult );
	result = result && subResult;

	float log2three = math.Log2( 3 );
	subResult = log2three > 1.584f && log2three < 1.585f;
	m_dialogEditControl->ReplaceSel(_T("    Log2(3): "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestLog2 overall: "));
	return result;
}

bool CMathLibraryTest::TestLogBase()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing LogBase - \n"));
	MathUtil math;

	float logBase;
	logBase = math.LogBase( 1, 0 );
	subResult = logBase == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    LogBase(1,0): "));
	SUBREPORT( subResult );
	result = result && subResult;

	logBase = math.LogBase( 1, 10 );
	subResult = logBase == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    LogBase(1, 10): "));
	SUBREPORT( subResult );
	result = result && subResult;

	logBase = math.LogBase( 10, 10 );
	subResult = logBase == 1.0f;
	m_dialogEditControl->ReplaceSel(_T("    LogBase(10, 10): "));
	SUBREPORT( subResult );
	result = result && subResult;

	logBase = math.LogBase( 81, 3 );
	subResult = logBase == 4.0f;
	m_dialogEditControl->ReplaceSel(_T("    LogBase(81,3): "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestLogBase overall: "));
	return result;
}

bool CMathLibraryTest::TestIsPowerOfTwo()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing IsPowerOfTwo - \n"));
	MathUtil math;

	subResult = !math.IsPowerOfTwo( -4 );
	result = result && subResult;

	subResult = !math.IsPowerOfTwo( 1 );
	result = result && subResult;

	subResult = math.IsPowerOfTwo( 2 );
	result = result && subResult;

	subResult = !math.IsPowerOfTwo( 3 );
	result = result && subResult;

	subResult = math.IsPowerOfTwo( 4 );
	result = result && subResult;

	subResult = !math.IsPowerOfTwo( 255 );
	result = result && subResult;

	subResult = math.IsPowerOfTwo( 256 );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestIsPowerOfTwo overall: "));
	return result;
}

bool CMathLibraryTest::TestNearestPowerOfTwo()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing NearestPowerOfTwo - \n"));
	MathUtil math;

	subResult = math.NearestPowerOfTwo( -4 ) == 2;
	result = result && subResult;

	subResult = math.NearestPowerOfTwo( 1 ) == 2;
	result = result && subResult;

	subResult = math.NearestPowerOfTwo( 2 ) == 2;
	result = result && subResult;

	subResult = math.NearestPowerOfTwo( 3 ) == 4;
	result = result && subResult;

	subResult = math.NearestPowerOfTwo( 4 ) == 4;
	result = result && subResult;

	subResult = math.NearestPowerOfTwo( 255 ) == 256;
	result = result && subResult;

	subResult = math.NearestPowerOfTwo( 256 ) == 256;
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestNearestPowerOfTwo overall: "));
	return result;
}

bool CMathLibraryTest::TestRadToDeg()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing RadToDeg - \n"));
	MathUtil math;

	float pi = 3.14159265f;
	float radToDeg;

	radToDeg = math.RadToDeg( 0.0 );
	subResult = radToDeg == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    RadToDeg(0.0): "));
	SUBREPORT( subResult );
	result = result && subResult;

	radToDeg = math.RadToDeg( -pi );
	subResult = radToDeg < -179.0f && radToDeg > -181.0f;
	m_dialogEditControl->ReplaceSel(_T("    radToDeg(-pi): "));
	SUBREPORT( subResult );
	result = result && subResult;

	radToDeg = math.RadToDeg( pi );
	subResult = radToDeg > 179.0f && radToDeg < 181.0f;
	m_dialogEditControl->ReplaceSel(_T("    RadToDeg(pi): "));
	SUBREPORT( subResult );
	result = result && subResult;

	radToDeg = math.RadToDeg( 2 * pi );
	subResult = radToDeg > 359.0f && radToDeg < 361.0f;
	m_dialogEditControl->ReplaceSel(_T("    RadToDeg(2pi): "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestRadToDeg overall: "));
	return result;
}


bool CMathLibraryTest::TestDegToRad()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing DegToRad - \n"));
	MathUtil math;

	float pi = 3.14159265f;
	float piLower = pi - .0001f;
	float piUpper = pi + .0001f;
	float radToDeg;

	radToDeg = math.DegToRad( 0.0 );
	subResult = radToDeg == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    DegToRad(0.0): "));
	SUBREPORT( subResult );
	result = result && subResult;

	radToDeg = math.DegToRad( -180 );
	subResult = radToDeg < -piLower && radToDeg > -piUpper;
	m_dialogEditControl->ReplaceSel(_T("    DegToRad(-180): "));
	SUBREPORT( subResult );
	result = result && subResult;

	radToDeg = math.DegToRad( 180 );
	subResult = radToDeg > piLower && radToDeg < piUpper;
	m_dialogEditControl->ReplaceSel(_T("    DegToRad(180): "));
	SUBREPORT( subResult );
	result = result && subResult;

	radToDeg = math.DegToRad( 360 );
	subResult = radToDeg > 2 * piLower && 2 * piUpper;
	m_dialogEditControl->ReplaceSel(_T("    DegToRad(360): "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestDegToRad overall: "));
	return result;
}

bool CMathLibraryTest::TestGetATan()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing GetATan - \n"));
	MathUtil math;

	float pi = 3.14159265f;
	float piLower = pi - .0001f;
	float piUpper = pi + .0001f;
	float tanValue;

	tanValue = math.GetATan( 0.0f, 0.0f );
	subResult = tanValue == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    GetATan(0.0): "));
	SUBREPORT( subResult );
	result = result && subResult;

	// 30°, 60°, 90° Triangle test
	float rootThree = 1.7320508f; //sqrt(3)
	tanValue = math.GetATan( rootThree, 1.0f );
	subResult = tanValue > piLower / 3.0f && tanValue < piUpper / 3.0f;
	m_dialogEditControl->ReplaceSel(_T("    GetATan(sqrt3): "));
	SUBREPORT( subResult );
	result = result && subResult;

	tanValue = math.GetATan( 1.0f, rootThree );
	subResult = tanValue > piLower / 6.0f && tanValue < piUpper / 6.0f;
	m_dialogEditControl->ReplaceSel(_T("    GetATan(1/sqrt3): "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestGetATan overall: "));
	return result;
}

bool CMathLibraryTest::TestGetACos()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing GetACos - \n"));
	MathUtil math;

	float pi = 3.14159265f;
	float piLower = pi - .0001f;
	float piUpper = pi + .0001f;
	float cosValue;

	cosValue = math.GetACos( -1.0f );
	subResult = cosValue > piLower && cosValue < piUpper;
	m_dialogEditControl->ReplaceSel(_T("    GetACos(-1.0): "));
	SUBREPORT( subResult );
	result = result && subResult;

	cosValue = math.GetACos( 0.0f );
	subResult = cosValue > piLower / 2.0f && cosValue < piUpper / 2.0f;
	m_dialogEditControl->ReplaceSel(_T("    GetACos(0.0): "));
	SUBREPORT( subResult );
	result = result && subResult;

	cosValue = math.GetACos( 1.0f );
	subResult = cosValue == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    GetACos(1.0): "));
	SUBREPORT( subResult );
	result = result && subResult;

	// 30°, 60°, 90° Triangle test
	float rootThree = 1.7320508f; //sqrt(3)
	float pi6thLower = pi / 6.0f - .01f;
	float pi6thUpper = pi / 6.0f + .01f;

	cosValue = math.GetACos( rootThree / 2.0f );
	subResult = cosValue > pi6thLower && cosValue < pi6thUpper;
	m_dialogEditControl->ReplaceSel(_T("    GetACos(sqrt3/2): "));
	SUBREPORT( subResult );
	result = result && subResult;

	cosValue = math.GetACos( 0.5f );
	subResult = cosValue > piLower / 3.0f && cosValue < piUpper / 3.0f;
	m_dialogEditControl->ReplaceSel(_T("    GetACos(1/2): "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestGetACos overall: "));
	return result;
}

bool CMathLibraryTest::TestGetASin()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing GetASin - \n"));
	MathUtil math;

	float pi = 3.14159265f;
	float piLower = pi - .0001f;
	float piUpper = pi + .0001f;
	float sinValue;

	sinValue = math.GetASin( -1.0f );
	subResult = sinValue < -piLower / 2.0f && sinValue > -piUpper / 2.0f;
	m_dialogEditControl->ReplaceSel(_T("    GetASin(-1.0): "));
	SUBREPORT( subResult );
	result = result && subResult;

	sinValue = math.GetASin( 0.0f );
	subResult = sinValue == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    GetASin(0.0): "));
	SUBREPORT( subResult );
	result = result && subResult;

	sinValue = math.GetASin( 1.0f );
	subResult = sinValue > piLower / 2.0f && sinValue < piUpper / 2.0f;
	m_dialogEditControl->ReplaceSel(_T("    GetASin(1.0): "));
	SUBREPORT( subResult );
	result = result && subResult;

	// 30°, 60°, 90° Triangle test
	float rootThree = 1.7320508f; //sqrt(3)
	float pi3rdLower = pi / 3.0f - .01f;
	float pi3rdUpper = pi / 3.0f + .01f;
	float pi6thLower = pi / 6.0f - .01f;
	float pi6thUpper = pi / 6.0f + .01f;

	sinValue = math.GetASin( rootThree / 2.0f );
	subResult = sinValue > pi3rdLower && sinValue < pi3rdUpper;
	m_dialogEditControl->ReplaceSel(_T("    GetASin(sqrt3/2): "));
	SUBREPORT( subResult );
	result = result && subResult;

	sinValue = math.GetASin( 0.5f );
	subResult = sinValue > pi6thLower && sinValue < pi6thUpper;
	m_dialogEditControl->ReplaceSel(_T("    GetASin(1/2): "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestGetASin overall: "));
	return result;
}

bool CMathLibraryTest::TestGetTan()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing GetTan - \n"));
	MathUtil math;

	float pi = 3.14159265f;
	float tanValue;

	tanValue = math.GetTan( 0.0f );
	subResult = tanValue == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    GetTan(0.0): "));
	SUBREPORT( subResult );
	result = result && subResult;

	// 30°, 60°, 90° Triangle test
	float rootThree = 1.7320508f; //sqrt(3)
	float rootThreeLower = rootThree - .0001f;
	float rootThreeUpper = rootThree + .0001f;
	float rootThreeRecipLower = 1.0f / rootThree - .0001f;
	float rootThreeRecipUpper = 1.0f / rootThree + .0001f;
	
	tanValue = math.GetTan( pi / 3.0f );
	subResult = tanValue > rootThreeLower && tanValue < rootThreeUpper;
	m_dialogEditControl->ReplaceSel(_T("    GetTan(pi/3): "));
	SUBREPORT( subResult );
	result = result && subResult;

	tanValue = math.GetTan( pi / 6.0f );
	subResult = tanValue > rootThreeRecipLower && tanValue < rootThreeRecipUpper;
	m_dialogEditControl->ReplaceSel(_T("    GetTan(pi/6): "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestGetTan overall: "));
	return result;
}

bool CMathLibraryTest::TestGetCos()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing GetCos - \n"));
	MathUtil math;

	float pi = 3.14159265358979323846264338327950288f;
	float cosValue;

	cosValue = math.GetCos( 0.0f );
	subResult = cosValue == 1.0f;
	m_dialogEditControl->ReplaceSel(_T("    GetCos(0): "));
	SUBREPORT( subResult );
	result = result && subResult;

	cosValue = math.GetCos( pi / 2.0f );
	subResult = cosValue > -0.01f && cosValue < 0.01f;
	m_dialogEditControl->ReplaceSel(_T("    GetCos(pi/2): "));
	SUBREPORT( subResult );
	result = result && subResult;

	cosValue = math.GetCos( pi );
	subResult = cosValue > -1.01f && cosValue < -0.99f;
	m_dialogEditControl->ReplaceSel(_T("    GetCos(pi): "));
	SUBREPORT( subResult );
	result = result && subResult;

	// 30°, 60°, 90° Triangle test
	float rootThree = 1.7320508f; //sqrt(3)
	float rootThreeHalvesLower = rootThree / 2.0f - 0.01f;
	float rootThreeHalvesUpper = rootThree / 2.0f + 0.01f;
	
	cosValue = math.GetCos( pi / 3.0f );
	subResult = cosValue > .49 && cosValue < .51;
	m_dialogEditControl->ReplaceSel(_T("    GetCos(pi/3): "));
	SUBREPORT( subResult );
	result = result && subResult;

	cosValue = math.GetCos( pi / 6.0f );
	subResult = cosValue > rootThreeHalvesLower && cosValue < rootThreeHalvesUpper;
	m_dialogEditControl->ReplaceSel(_T("    GetCos(pi/6): "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestGetCos overall: "));
	return result;
}

bool CMathLibraryTest::TestGetSin()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing GetSin - \n"));
	MathUtil math;

	float pi = 3.14159265358979323846264338327950288f;
	float sinValue;

	sinValue = math.GetSin( 0.0f );
	subResult = sinValue == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    GetSin(0): "));
	SUBREPORT( subResult );
	result = result && subResult;

	sinValue = math.GetSin( pi / 2.0f );
	subResult = sinValue > 0.99f && sinValue < 1.01f;
	m_dialogEditControl->ReplaceSel(_T("    GetSin(pi/2): "));
	SUBREPORT( subResult );
	result = result && subResult;

	sinValue = math.GetSin( pi );
	subResult = sinValue > -0.01f && sinValue < 0.01f;
	m_dialogEditControl->ReplaceSel(_T("    GetSin(pi): "));
	SUBREPORT( subResult );
	result = result && subResult;

	// 30°, 60°, 90° Triangle test
	float rootThree = 1.7320508f; //sqrt(3)
	float rootThreeHalvesLower = rootThree / 2.0f - 0.01f;
	float rootThreeHalvesUpper = rootThree / 2.0f + 0.01f;
	
	sinValue = math.GetSin( pi / 3.0f );
	subResult = sinValue > rootThreeHalvesLower && sinValue < rootThreeHalvesUpper;
	m_dialogEditControl->ReplaceSel(_T("    GetSin(pi/3): "));
	SUBREPORT( subResult );
	result = result && subResult;

	sinValue = math.GetSin( pi / 6.0f );
	subResult = sinValue > .49 && sinValue < .51;
	m_dialogEditControl->ReplaceSel(_T("    GetSin(pi/6): "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestGetSin overall: "));
	return result;
}

bool CMathLibraryTest::TestMatrixLookAt()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing MatrixLookAt - \n"));
	Vec3 vOrigin( 0, 0, 0 );
	Vec3 vTarget( 0, 0, 1 );
	Vec3 vUp( 0, 1, 0 );
	Matrix3x3 matrix;
	MathUtil math;

	math.MatrixLookAt( vOrigin, vTarget, vUp, matrix );

	subResult =
		matrix.m[0] == 1.0f &&
		matrix.m[1] == 0.0f &&
		matrix.m[2] == 0.0f &&
		matrix.m[3] == 0.0f &&
		matrix.m[4] == 1.0f &&
		matrix.m[5] == 0.0f &&
		matrix.m[6] == 0.0f &&
		matrix.m[7] == 0.0f &&
		matrix.m[8] == 1.0f;

	math.MatrixLookAt( vOrigin, vTarget, matrix );

	subResult = subResult &&
		matrix.m[0] == 1.0f &&
		matrix.m[1] == 0.0f &&
		matrix.m[2] == 0.0f &&
		matrix.m[3] == 0.0f &&
		matrix.m[4] == 1.0f &&
		matrix.m[5] == 0.0f &&
		matrix.m[6] == 0.0f &&
		matrix.m[7] == 0.0f &&
		matrix.m[8] == 1.0f;

	m_dialogEditControl->ReplaceSel(_T("    MatrixLookAt test 1: "));
	SUBREPORT( subResult );
	result = result && subResult;

	vUp.Set( 0, -1, 0 );

	math.MatrixLookAt( vOrigin, vTarget, vUp, matrix );

	subResult =
		matrix.m[0] == -1.0f &&
		matrix.m[1] == 0.0f &&
		matrix.m[2] == 0.0f &&
		matrix.m[3] == 0.0f &&
		matrix.m[4] == -1.0f &&
		matrix.m[5] == 0.0f &&
		matrix.m[6] == 0.0f &&
		matrix.m[7] == 0.0f &&
		matrix.m[8] == 1.0f;
	m_dialogEditControl->ReplaceSel(_T("    MatrixLookAt test 2: "));
	SUBREPORT( subResult );
	result = result && subResult;


	vOrigin.Set( 50, 50, 50 );
	vTarget.Set( 50, 50, 75 );
	vUp.Set( 0, 1, 0 );

	math.MatrixLookAt( vOrigin, vTarget, vUp, matrix );

	subResult =
		matrix.m[0] == 1.0f &&
		matrix.m[1] == 0.0f &&
		matrix.m[2] == 0.0f &&
		matrix.m[3] == 0.0f &&
		matrix.m[4] == 1.0f &&
		matrix.m[5] == 0.0f &&
		matrix.m[6] == 0.0f &&
		matrix.m[7] == 0.0f &&
		matrix.m[8] == 1.0f;
	m_dialogEditControl->ReplaceSel(_T("    MatrixLookAt test 3: "));
	SUBREPORT( subResult );
	result = result && subResult;


	vOrigin.Set( 50, 50, 50 );
	vTarget.Set( 75, 50, 75 );
	vUp.Set( 0, 1, 0 );

	math.MatrixLookAt( vOrigin, vTarget, vUp, matrix );

	subResult =
		matrix.m[0] > 0.70710f && matrix.m[0] < 0.70711f &&
		matrix.m[1] == 0.0f &&
		matrix.m[2] < -0.70710f && matrix.m[2] > -0.70711f &&
		matrix.m[3] == 0.0f &&
		matrix.m[4] == 1.0f &&
		matrix.m[5] == 0.0f &&
		matrix.m[6] > 0.70710f && matrix.m[6] < 0.70711f &&
		matrix.m[7] == 0.0f &&
		matrix.m[8] > 0.70710f &&  matrix.m[8] < 0.70711f;
	m_dialogEditControl->ReplaceSel(_T("    MatrixLookAt test 4: "));
	SUBREPORT( subResult );
	result = result && subResult;

	vOrigin.Set( 0, 0, 0 );
	vTarget.Set( 1, 0, 0 );
	vUp.Set( 0, 1, 0 );

	math.MatrixLookAt( vOrigin, vTarget, matrix );

	subResult =
		matrix.m[0] == 0.0f &&
		matrix.m[1] == 0.0f &&
		matrix.m[2] == -1.0f &&
		matrix.m[3] == 0.0f &&
		matrix.m[4] == 1.0f &&
		matrix.m[5] == 0.0f &&
		matrix.m[6] == 1.0f &&
		matrix.m[7] == 0.0f &&
		matrix.m[8] == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    MatrixLookAt test 5: "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestMatrixLookAt overall: "));
	return result;
}

bool CMathLibraryTest::TestMatrixToEuler()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing MatrixToEuler - \n"));

	Matrix3x3 matrix(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f );
	EulerAngle angles;
	MathUtil math;

	math.MatrixToEuler( matrix, angles );
	subResult =
		angles.roll == 0.0f &&
		angles.pitch == 0.0f &&
		angles.yaw == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    MatrixToEuler no rotation: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		0.0f, 0.0f, -1.0f, //x basis
		0.0f, 1.0f, 0.0f, //y basis
		1.0f, 0.0f, 0.0f ); //z basis
	math.MatrixToEuler( matrix, angles );
	subResult =
		angles.roll == 0.0f &&
		angles.pitch == 0.0f &&
		angles.yaw == 90.0f;
	m_dialogEditControl->ReplaceSel(_T("    MatrixToEuler yaw 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f );
	math.MatrixToEuler( matrix, angles );
	subResult =
		angles.roll == 0.0f &&
		angles.pitch == 0.0f &&
		angles.yaw == -90.0f;
	m_dialogEditControl->ReplaceSel(_T("    MatrixToEuler yaw -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f );
	math.MatrixToEuler( matrix, angles );
	subResult =
		angles.roll == 0.0f &&
		angles.pitch == 90.0f &&
		angles.yaw == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    MatrixToEuler pitch 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, -1.0f, 0.0f );
	math.MatrixToEuler( matrix, angles );
	subResult =
		angles.roll == 0.0f &&
		angles.pitch == -90.0f &&
		angles.yaw == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    MatrixToEuler pitch -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f );
	math.MatrixToEuler( matrix, angles );
	subResult =
		angles.roll == 90.0f &&
		angles.pitch == 0.0f &&
		angles.yaw == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    MatrixToEuler roll 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f );
	math.MatrixToEuler( matrix, angles );
	subResult =
		angles.roll == -90.0f &&
		angles.pitch == 0.0f &&
		angles.yaw == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    MatrixToEuler roll -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestMatrixToEuler overall: "));
	return result;
}

bool CMathLibraryTest::TestEulerToMatrix()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing EulerToMatrix - \n"));
	
	Matrix3x3 matrix;
	EulerAngle angles( 0.0f, 0.0f, 0.0f );
	MathUtil math;

	math.EulerToMatrix( angles, matrix );
	subResult =
		matrix.m[0] == 1.0f &&
		matrix.m[1] == 0.0f &&
		matrix.m[2] == 0.0f &&
		matrix.m[3] == 0.0f &&
		matrix.m[4] == 1.0f &&
		matrix.m[5] == 0.0f &&
		matrix.m[6] == 0.0f &&	
		matrix.m[7] == 0.0f &&
		matrix.m[8] == 1.0f;
	m_dialogEditControl->ReplaceSel(_T("    EulerToMatrix no rotation: "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("    Matrix Parity: "));
	subResult = matrix.Parity();
	SUBREPORT( subResult );
	result = result && subResult;

	angles.Set( 0.0f, 0.0f, 90.0f );
	math.EulerToMatrix( angles, matrix );
	subResult =
		matrix.m[0] > -0.000001f && matrix.m[0] < 0.000001f &&
		matrix.m[1] > -0.000001f && matrix.m[1] < 0.000001f &&
		matrix.m[2] == -1.0f &&
		matrix.m[3] > -0.000001f && matrix.m[3] < 0.000001f &&
		matrix.m[4] == 1.0f &&
		matrix.m[5] > -0.000001f && matrix.m[5] < 0.000001f &&
		matrix.m[6] == 1.0f &&
		matrix.m[7] > -0.000001f && matrix.m[7] < 0.000001f &&
		matrix.m[8] > -0.000001f && matrix.m[8] < 0.000001f;
	m_dialogEditControl->ReplaceSel(_T("    EulerToMatrix yaw 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("    Matrix Parity: "));
	subResult = matrix.Parity();
	SUBREPORT( subResult );
	result = result && subResult;

	angles.Set( 0.0f, 0.0f, -90.0f );
	math.EulerToMatrix( angles, matrix );
	subResult =
		matrix.m[0] > -0.000001f && matrix.m[0] < 0.000001f &&
		matrix.m[1] > -0.000001f && matrix.m[1] < 0.000001f &&
		matrix.m[2] == 1.0f &&
		matrix.m[3] > -0.000001f && matrix.m[3] < 0.000001f &&
		matrix.m[4] == 1.0f &&
		matrix.m[5] > -0.000001f && matrix.m[5] < 0.000001f &&
		matrix.m[6] == -1.0f &&
		matrix.m[7] > -0.000001f && matrix.m[7] < 0.000001f &&
		matrix.m[8] > -0.000001f && matrix.m[8] < 0.000001f;
	m_dialogEditControl->ReplaceSel(_T("    EulerToMatrix yaw -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("    Matrix Parity: "));
	subResult = matrix.Parity();
	SUBREPORT( subResult );
	result = result && subResult;

	angles.Set( 0.0f, 90.0f, 0.0f );
	math.EulerToMatrix( angles, matrix );
	subResult =
		matrix.m[0] == 1.0f &&
		matrix.m[1] > -0.000001f && matrix.m[1] < 0.000001f &&
		matrix.m[2] > -0.000001f && matrix.m[2] < 0.000001f &&
		matrix.m[3] > -0.000001f && matrix.m[3] < 0.000001f &&
		matrix.m[4] > -0.000001f && matrix.m[4] < 0.000001f &&
		matrix.m[5] == -1.0f &&
		matrix.m[6] > -0.000001f && matrix.m[6] < 0.000001f &&
		matrix.m[7] == 1.0f &&
		matrix.m[8] > -0.000001f && matrix.m[8] < 0.000001f;
	m_dialogEditControl->ReplaceSel(_T("    EulerToMatrix pitch 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;
	
	m_dialogEditControl->ReplaceSel(_T("    Matrix Parity: "));
	subResult = matrix.Parity();
	SUBREPORT( subResult );
	result = result && subResult;

	angles.Set( 0.0f, -90.0f, 0.0f );
	math.EulerToMatrix( angles, matrix );
	subResult =
		matrix.m[0] == 1.0f &&
		matrix.m[1] > -0.000001f && matrix.m[1] < 0.000001f &&
		matrix.m[2] > -0.000001f && matrix.m[2] < 0.000001f &&
		matrix.m[3] > -0.000001f && matrix.m[3] < 0.000001f &&
		matrix.m[4] > -0.000001f && matrix.m[4] < 0.000001f &&
		matrix.m[5] == 1.0f &&
		matrix.m[6] > -0.000001f && matrix.m[6] < 0.000001f &&
		matrix.m[7] == -1.0f &&
		matrix.m[8] > -0.000001f && matrix.m[8] < 0.000001f;
	m_dialogEditControl->ReplaceSel(_T("    EulerToMatrix pitch -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("    Matrix Parity: "));
	subResult = matrix.Parity();
	SUBREPORT( subResult );
	result = result && subResult;

	angles.Set( 90.0f, 0.0f, 0.0f );
	math.EulerToMatrix( angles, matrix );
	subResult =
		matrix.m[0] > -0.000001f && matrix.m[0] < 0.000001f &&
		matrix.m[1] == -1.0f &&
		matrix.m[2] > -0.000001f && matrix.m[2] < 0.000001f &&
		matrix.m[3] == 1.0f &&
		matrix.m[4] > -0.000001f && matrix.m[4] < 0.000001f &&
		matrix.m[5] > -0.000001f && matrix.m[5] < 0.000001f &&
		matrix.m[6] > -0.000001f && matrix.m[6] < 0.000001f &&
		matrix.m[7] > -0.000001f && matrix.m[7] < 0.000001f &&
		matrix.m[8] == 1.0f;
	m_dialogEditControl->ReplaceSel(_T("    EulerToMatrix roll 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("    Matrix Parity: "));
	subResult = matrix.Parity();
	SUBREPORT( subResult );
	result = result && subResult;

	angles.Set( -90.0f, 0.0f, 0.0f );
	math.EulerToMatrix( angles, matrix );
	subResult =
		matrix.m[0] > -0.000001f && matrix.m[0] < 0.000001f &&
		matrix.m[1] == 1.0f &&
		matrix.m[2] > -0.000001f && matrix.m[2] < 0.000001f &&
		matrix.m[3] == -1.0f &&
		matrix.m[4] > -0.000001f && matrix.m[4] < 0.000001f &&
		matrix.m[5] > -0.000001f && matrix.m[5] < 0.000001f &&
		matrix.m[6] > -0.000001f && matrix.m[6] < 0.000001f &&
		matrix.m[7] > -0.000001f && matrix.m[7] < 0.000001f &&
		matrix.m[8] == 1.0f;
	m_dialogEditControl->ReplaceSel(_T("    EulerToMatrix roll -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("    Matrix Parity: "));
	subResult = matrix.Parity();
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestEulerToMatrix overall: "));
	return result;
}

bool CMathLibraryTest::TestEulerToQuaternion()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing EulerToQuaternion - \n"));
	
	MathUtil math;
	EulerAngle euler;
	Quaternion quaternion;

	euler.Set( 0.0f, 0.0f, 0.0f );
	math.EulerToQuaternion( euler, quaternion );
	subResult =
		quaternion.x == 0.0f &&
		quaternion.y == 0.0f &&
		quaternion.z == 0.0f &&
		quaternion.w == 1.0f;
	m_dialogEditControl->ReplaceSel(_T("    EulerToQuaternion no rotation: "));
	SUBREPORT( subResult );
	result = result && subResult;

	euler.Set( 0.0f, 0.0f, 90.0f );
	math.EulerToQuaternion( euler, quaternion );
	subResult =
		quaternion.x == 0.0f &&
		quaternion.y > 0.7070f && quaternion.y < 0.7072f &&
		quaternion.z == 0.0f &&
		quaternion.w > 0.7070f && quaternion.w < 0.7072f;
	m_dialogEditControl->ReplaceSel(_T("    EulerToQuaternion yaw 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	euler.Set( 0.0f, 0.0f, -90.0f );
	math.EulerToQuaternion( euler, quaternion );
	subResult =
		quaternion.x == 0.0f &&
		quaternion.y < -0.7070f && quaternion.y > -0.7072f &&
		quaternion.z == 0.0f &&
		quaternion.w > 0.7070f && quaternion.w < 0.7072f;
	m_dialogEditControl->ReplaceSel(_T("    EulerToQuaternion yaw -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	euler.Set( 0.0f, 90.0f, 0.0f );
	math.EulerToQuaternion( euler, quaternion );
	subResult =
		quaternion.x > 0.7070f && quaternion.x < 0.7072f &&
		quaternion.y == 0.0f &&
		quaternion.z == 0.0f &&
		quaternion.w > 0.7070f && quaternion.w < 0.7072f;
	m_dialogEditControl->ReplaceSel(_T("    EulerToQuaternion pitch 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	euler.Set( 0.0f, -90.0f, 0.0f );
	math.EulerToQuaternion( euler, quaternion );
	subResult =
		quaternion.x < -0.7070f && quaternion.x > -0.7072f &&
		quaternion.y == 0.0f &&
		quaternion.z == 0.0f &&
		quaternion.w > 0.7070f && quaternion.w < 0.7072f;
	m_dialogEditControl->ReplaceSel(_T("    EulerToQuaternion pitch -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	euler.Set( 90.0f, 0.0f, 0.0f );
	math.EulerToQuaternion( euler, quaternion );
	subResult =
		quaternion.x == 0.0f &&
		quaternion.y == 0.0f &&
		quaternion.z > 0.7070f && quaternion.x < 0.7072f &&
		quaternion.w > 0.7070f && quaternion.w < 0.7072f;
	m_dialogEditControl->ReplaceSel(_T("    EulerToQuaternion roll 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	euler.Set( -90.0f, 0.0f, 0.0f );
	math.EulerToQuaternion( euler, quaternion );
	subResult =
		quaternion.x == 0.0f &&
		quaternion.y == 0.0f &&
		quaternion.z < -0.7070f && quaternion.x > -0.7072f &&
		quaternion.w > 0.7070f && quaternion.w < 0.7072f;
	m_dialogEditControl->ReplaceSel(_T("    EulerToQuaternion roll -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestEulerToQuaternion overall: "));
	return result;
}

bool CMathLibraryTest::TestQuaternionToEuler()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing QuaternionToEuler - \n"));
	
	MathUtil math;
	EulerAngle euler;
	Quaternion quaternion;
	
	quaternion.Set( 0.0f, 0.0f, 0.0f, 1.0f );
	quaternion.ToEuler( euler );
	subResult =
		euler.yaw == 0.0f &&
		euler.pitch == 0.0f &&
		euler.roll == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    QuaternionToEuler no rotation: "));
	SUBREPORT( subResult );
	result = result && subResult;

	quaternion.Set( 0.0f, 0.7071f, 0.0f, 0.7071f );
	quaternion.ToEuler( euler );
	subResult =
		euler.yaw == 90.0f &&
		euler.pitch == 0.0f &&
		euler.roll == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    QuaternionToEuler yaw 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	quaternion.Set( 0.0f, -0.7071f, 0.0f, 0.7071f );
	quaternion.ToEuler( euler );
	subResult =
		euler.yaw == -90.0f &&
		euler.pitch == 0.0f &&
		euler.roll == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    QuaternionToEuler yaw -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	quaternion.Set( 0.7071f, 0.0f, 0.0f, 0.7071f );
	quaternion.ToEuler( euler );
	subResult =
		euler.yaw == 0.0f &&
		euler.pitch == 90.0f &&
		euler.roll == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    QuaternionToEuler pitch 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	quaternion.Set( -0.7071f, 0.0f, 0.0f, 0.7071f );
	quaternion.ToEuler( euler );
	subResult =
		euler.yaw == 0.0f &&
		euler.pitch == -90.0f &&
		euler.roll == 0.0f;
	m_dialogEditControl->ReplaceSel(_T("    QuaternionToEuler pitch -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	quaternion.Set( 0.0f, 0.0f, 0.7071f, 0.7071f );
	quaternion.ToEuler( euler );
	subResult =
		euler.yaw == 0.0f &&
		euler.pitch == 0.0f &&
		euler.roll == 90.0f;
	m_dialogEditControl->ReplaceSel(_T("    QuaternionToEuler roll 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	quaternion.Set( 0.0f, 0.0f, -0.7071f, 0.7071f );
	quaternion.ToEuler( euler );
	subResult =
		euler.yaw == 0.0f &&
		euler.pitch == 0.0f &&
		euler.roll == -90.0f;
	m_dialogEditControl->ReplaceSel(_T("    QuaternionToEuler roll -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestQuaternionToEuler overall: "));
	return result;
}

bool CMathLibraryTest::TestMatrixToQuaternionToEulerToMatrix()
{bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing MatrixToQuaternionToEulerToMatrix - \n"));

	Matrix3x3 matrix(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f );
	EulerAngle angles;
	MathUtil math;
	Quaternion quat;
	Matrix4x4 matResult4;
	Matrix3x3 matResult;

	quat.SetFromMatrix( matrix );
	quat.ToEuler( angles );
	matResult.SetRotation( angles );
	subResult =
		matrix.m[0] > matResult.m[0]-0.000001f && matrix.m[0] < matResult.m[0]+0.000001f &&
		matrix.m[1] > matResult.m[1]-0.000001f && matrix.m[1] < matResult.m[1]+0.000001f &&
		matrix.m[2] > matResult.m[2]-0.000001f && matrix.m[2] < matResult.m[2]+0.000001f &&
		matrix.m[3] > matResult.m[3]-0.000001f && matrix.m[3] < matResult.m[3]+0.000001f &&
		matrix.m[4] > matResult.m[4]-0.000001f && matrix.m[4] < matResult.m[4]+0.000001f &&
		matrix.m[5] > matResult.m[5]-0.000001f && matrix.m[5] < matResult.m[5]+0.000001f &&
		matrix.m[6] > matResult.m[6]-0.000001f && matrix.m[6] < matResult.m[6]+0.000001f &&
		matrix.m[7] > matResult.m[7]-0.000001f && matrix.m[7] < matResult.m[7]+0.000001f &&
		matrix.m[8] > matResult.m[8]-0.000001f && matrix.m[8] < matResult.m[8]+0.000001f;

	m_dialogEditControl->ReplaceSel(_T("    MatrixToQuaternionToEulerToMatrix no rotation: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		0.0f, 0.0f, -1.0f, //x basis
		0.0f, 1.0f, 0.0f, //y basis
		1.0f, 0.0f, 0.0f ); //z basis
	
	
	quat.SetFromMatrix( matrix );
	quat.ToEuler( angles );
	matResult.SetRotation( angles );
	subResult =
		matrix.m[0] > matResult.m[0]-0.000001f && matrix.m[0] < matResult.m[0]+0.000001f &&
		matrix.m[1] > matResult.m[1]-0.000001f && matrix.m[1] < matResult.m[1]+0.000001f &&
		matrix.m[2] > matResult.m[2]-0.000001f && matrix.m[2] < matResult.m[2]+0.000001f &&
		matrix.m[3] > matResult.m[3]-0.000001f && matrix.m[3] < matResult.m[3]+0.000001f &&
		matrix.m[4] > matResult.m[4]-0.000001f && matrix.m[4] < matResult.m[4]+0.000001f &&
		matrix.m[5] > matResult.m[5]-0.000001f && matrix.m[5] < matResult.m[5]+0.000001f &&
		matrix.m[6] > matResult.m[6]-0.000001f && matrix.m[6] < matResult.m[6]+0.000001f &&
		matrix.m[7] > matResult.m[7]-0.000001f && matrix.m[7] < matResult.m[7]+0.000001f &&
		matrix.m[8] > matResult.m[8]-0.000001f && matrix.m[8] < matResult.m[8]+0.000001f;

	m_dialogEditControl->ReplaceSel(_T("    MatrixToQuaternionToEulerToMatrix yaw 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f );
	
	
	quat.SetFromMatrix( matrix );
	quat.ToEuler( angles );
	matResult.SetRotation( angles );
	subResult =
		matrix.m[0] > matResult.m[0]-0.000001f && matrix.m[0] < matResult.m[0]+0.000001f &&
		matrix.m[1] > matResult.m[1]-0.000001f && matrix.m[1] < matResult.m[1]+0.000001f &&
		matrix.m[2] > matResult.m[2]-0.000001f && matrix.m[2] < matResult.m[2]+0.000001f &&
		matrix.m[3] > matResult.m[3]-0.000001f && matrix.m[3] < matResult.m[3]+0.000001f &&
		matrix.m[4] > matResult.m[4]-0.000001f && matrix.m[4] < matResult.m[4]+0.000001f &&
		matrix.m[5] > matResult.m[5]-0.000001f && matrix.m[5] < matResult.m[5]+0.000001f &&
		matrix.m[6] > matResult.m[6]-0.000001f && matrix.m[6] < matResult.m[6]+0.000001f &&
		matrix.m[7] > matResult.m[7]-0.000001f && matrix.m[7] < matResult.m[7]+0.000001f &&
		matrix.m[8] > matResult.m[8]-0.000001f && matrix.m[8] < matResult.m[8]+0.000001f;

	m_dialogEditControl->ReplaceSel(_T("    MatrixToQuaternionToEulerToMatrix yaw -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f );
	
	
	quat.SetFromMatrix( matrix );
	quat.ToEuler( angles );
	matResult.SetRotation( angles );
	subResult =
		matrix.m[0] > matResult.m[0]-0.000001f && matrix.m[0] < matResult.m[0]+0.000001f &&
		matrix.m[1] > matResult.m[1]-0.000001f && matrix.m[1] < matResult.m[1]+0.000001f &&
		matrix.m[2] > matResult.m[2]-0.000001f && matrix.m[2] < matResult.m[2]+0.000001f &&
		matrix.m[3] > matResult.m[3]-0.000001f && matrix.m[3] < matResult.m[3]+0.000001f &&
		matrix.m[4] > matResult.m[4]-0.000001f && matrix.m[4] < matResult.m[4]+0.000001f &&
		matrix.m[5] > matResult.m[5]-0.000001f && matrix.m[5] < matResult.m[5]+0.000001f &&
		matrix.m[6] > matResult.m[6]-0.000001f && matrix.m[6] < matResult.m[6]+0.000001f &&
		matrix.m[7] > matResult.m[7]-0.000001f && matrix.m[7] < matResult.m[7]+0.000001f &&
		matrix.m[8] > matResult.m[8]-0.000001f && matrix.m[8] < matResult.m[8]+0.000001f;

	m_dialogEditControl->ReplaceSel(_T("    MatrixToQuaternionToEulerToMatrix pitch 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, -1.0f, 0.0f );
	
	
	quat.SetFromMatrix( matrix );
	quat.ToEuler( angles );
	matResult.SetRotation( angles );
	subResult =
		matrix.m[0] > matResult.m[0]-0.000001f && matrix.m[0] < matResult.m[0]+0.000001f &&
		matrix.m[1] > matResult.m[1]-0.000001f && matrix.m[1] < matResult.m[1]+0.000001f &&
		matrix.m[2] > matResult.m[2]-0.000001f && matrix.m[2] < matResult.m[2]+0.000001f &&
		matrix.m[3] > matResult.m[3]-0.000001f && matrix.m[3] < matResult.m[3]+0.000001f &&
		matrix.m[4] > matResult.m[4]-0.000001f && matrix.m[4] < matResult.m[4]+0.000001f &&
		matrix.m[5] > matResult.m[5]-0.000001f && matrix.m[5] < matResult.m[5]+0.000001f &&
		matrix.m[6] > matResult.m[6]-0.000001f && matrix.m[6] < matResult.m[6]+0.000001f &&
		matrix.m[7] > matResult.m[7]-0.000001f && matrix.m[7] < matResult.m[7]+0.000001f &&
		matrix.m[8] > matResult.m[8]-0.000001f && matrix.m[8] < matResult.m[8]+0.000001f;

	m_dialogEditControl->ReplaceSel(_T("    MatrixToQuaternionToEulerToMatrix pitch -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f );
	
	
	quat.SetFromMatrix( matrix );
	quat.ToEuler( angles );
	matResult.SetRotation( angles );
	subResult =
		matrix.m[0] > matResult.m[0]-0.000001f && matrix.m[0] < matResult.m[0]+0.000001f &&
		matrix.m[1] > matResult.m[1]-0.000001f && matrix.m[1] < matResult.m[1]+0.000001f &&
		matrix.m[2] > matResult.m[2]-0.000001f && matrix.m[2] < matResult.m[2]+0.000001f &&
		matrix.m[3] > matResult.m[3]-0.000001f && matrix.m[3] < matResult.m[3]+0.000001f &&
		matrix.m[4] > matResult.m[4]-0.000001f && matrix.m[4] < matResult.m[4]+0.000001f &&
		matrix.m[5] > matResult.m[5]-0.000001f && matrix.m[5] < matResult.m[5]+0.000001f &&
		matrix.m[6] > matResult.m[6]-0.000001f && matrix.m[6] < matResult.m[6]+0.000001f &&
		matrix.m[7] > matResult.m[7]-0.000001f && matrix.m[7] < matResult.m[7]+0.000001f &&
		matrix.m[8] > matResult.m[8]-0.000001f && matrix.m[8] < matResult.m[8]+0.000001f;

	m_dialogEditControl->ReplaceSel(_T("    MatrixToQuaternionToEulerToMatrix roll 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f );
	
	
	quat.SetFromMatrix( matrix );
	quat.ToEuler( angles );
	matResult.SetRotation( angles );
	subResult =
		matrix.m[0] > matResult.m[0]-0.000001f && matrix.m[0] < matResult.m[0]+0.000001f &&
		matrix.m[1] > matResult.m[1]-0.000001f && matrix.m[1] < matResult.m[1]+0.000001f &&
		matrix.m[2] > matResult.m[2]-0.000001f && matrix.m[2] < matResult.m[2]+0.000001f &&
		matrix.m[3] > matResult.m[3]-0.000001f && matrix.m[3] < matResult.m[3]+0.000001f &&
		matrix.m[4] > matResult.m[4]-0.000001f && matrix.m[4] < matResult.m[4]+0.000001f &&
		matrix.m[5] > matResult.m[5]-0.000001f && matrix.m[5] < matResult.m[5]+0.000001f &&
		matrix.m[6] > matResult.m[6]-0.000001f && matrix.m[6] < matResult.m[6]+0.000001f &&
		matrix.m[7] > matResult.m[7]-0.000001f && matrix.m[7] < matResult.m[7]+0.000001f &&
		matrix.m[8] > matResult.m[8]-0.000001f && matrix.m[8] < matResult.m[8]+0.000001f;

	m_dialogEditControl->ReplaceSel(_T("    MatrixToQuaternionToEulerToMatrix roll -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestMatrixToQuaternionToEulerToMatrix overall: "));
	return result;
}

bool CMathLibraryTest::TestMatrixToEulerToQuaternionToMatrix()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing MatrixToEulerToQuaternionToMatrix - \n"));

	Matrix3x3 matrix(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f );
	EulerAngle angles;
	MathUtil math;
	Quaternion quat;
	Matrix4x4 matResult4;
	Matrix3x3 matResult;

	math.MatrixToEuler( matrix, angles );
	quat.FromEuler( angles );
	quat.GetMatrix( matResult4 );
	matResult.SetFrom4x4( matResult4.GetMatrix() );
	subResult =
		matrix.m[0] > matResult.m[0]-0.000001f && matrix.m[0] < matResult.m[0]+0.000001f &&
		matrix.m[1] > matResult.m[1]-0.000001f && matrix.m[1] < matResult.m[1]+0.000001f &&
		matrix.m[2] > matResult.m[2]-0.000001f && matrix.m[2] < matResult.m[2]+0.000001f &&
		matrix.m[3] > matResult.m[3]-0.000001f && matrix.m[3] < matResult.m[3]+0.000001f &&
		matrix.m[4] > matResult.m[4]-0.000001f && matrix.m[4] < matResult.m[4]+0.000001f &&
		matrix.m[5] > matResult.m[5]-0.000001f && matrix.m[5] < matResult.m[5]+0.000001f &&
		matrix.m[6] > matResult.m[6]-0.000001f && matrix.m[6] < matResult.m[6]+0.000001f &&
		matrix.m[7] > matResult.m[7]-0.000001f && matrix.m[7] < matResult.m[7]+0.000001f &&
		matrix.m[8] > matResult.m[8]-0.000001f && matrix.m[8] < matResult.m[8]+0.000001f;

	m_dialogEditControl->ReplaceSel(_T("    MatrixToEulerToQuaternionToMatrix no rotation: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		0.0f, 0.0f, -1.0f, //x basis
		0.0f, 1.0f, 0.0f, //y basis
		1.0f, 0.0f, 0.0f ); //z basis
	
	math.MatrixToEuler( matrix, angles );
	quat.FromEuler( angles );
	quat.GetMatrix( matResult4 );
	matResult.SetFrom4x4( matResult4.GetMatrix() );
	subResult =
		matrix.m[0] > matResult.m[0]-0.000001f && matrix.m[0] < matResult.m[0]+0.000001f &&
		matrix.m[1] > matResult.m[1]-0.000001f && matrix.m[1] < matResult.m[1]+0.000001f &&
		matrix.m[2] > matResult.m[2]-0.000001f && matrix.m[2] < matResult.m[2]+0.000001f &&
		matrix.m[3] > matResult.m[3]-0.000001f && matrix.m[3] < matResult.m[3]+0.000001f &&
		matrix.m[4] > matResult.m[4]-0.000001f && matrix.m[4] < matResult.m[4]+0.000001f &&
		matrix.m[5] > matResult.m[5]-0.000001f && matrix.m[5] < matResult.m[5]+0.000001f &&
		matrix.m[6] > matResult.m[6]-0.000001f && matrix.m[6] < matResult.m[6]+0.000001f &&
		matrix.m[7] > matResult.m[7]-0.000001f && matrix.m[7] < matResult.m[7]+0.000001f &&
		matrix.m[8] > matResult.m[8]-0.000001f && matrix.m[8] < matResult.m[8]+0.000001f;

	m_dialogEditControl->ReplaceSel(_T("    MatrixToEulerToQuaternionToMatrix yaw 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f );
	
	math.MatrixToEuler( matrix, angles );
	quat.FromEuler( angles );
	quat.GetMatrix( matResult4 );
	matResult.SetFrom4x4( matResult4.GetMatrix() );
	subResult =
		matrix.m[0] > matResult.m[0]-0.000001f && matrix.m[0] < matResult.m[0]+0.000001f &&
		matrix.m[1] > matResult.m[1]-0.000001f && matrix.m[1] < matResult.m[1]+0.000001f &&
		matrix.m[2] > matResult.m[2]-0.000001f && matrix.m[2] < matResult.m[2]+0.000001f &&
		matrix.m[3] > matResult.m[3]-0.000001f && matrix.m[3] < matResult.m[3]+0.000001f &&
		matrix.m[4] > matResult.m[4]-0.000001f && matrix.m[4] < matResult.m[4]+0.000001f &&
		matrix.m[5] > matResult.m[5]-0.000001f && matrix.m[5] < matResult.m[5]+0.000001f &&
		matrix.m[6] > matResult.m[6]-0.000001f && matrix.m[6] < matResult.m[6]+0.000001f &&
		matrix.m[7] > matResult.m[7]-0.000001f && matrix.m[7] < matResult.m[7]+0.000001f &&
		matrix.m[8] > matResult.m[8]-0.000001f && matrix.m[8] < matResult.m[8]+0.000001f;

	m_dialogEditControl->ReplaceSel(_T("    MatrixToEulerToQuaternionToMatrix yaw -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f );
	
	math.MatrixToEuler( matrix, angles );
	quat.FromEuler( angles );
	quat.GetMatrix( matResult4 );
	matResult.SetFrom4x4( matResult4.GetMatrix() );
	subResult =
		matrix.m[0] > matResult.m[0]-0.000001f && matrix.m[0] < matResult.m[0]+0.000001f &&
		matrix.m[1] > matResult.m[1]-0.000001f && matrix.m[1] < matResult.m[1]+0.000001f &&
		matrix.m[2] > matResult.m[2]-0.000001f && matrix.m[2] < matResult.m[2]+0.000001f &&
		matrix.m[3] > matResult.m[3]-0.000001f && matrix.m[3] < matResult.m[3]+0.000001f &&
		matrix.m[4] > matResult.m[4]-0.000001f && matrix.m[4] < matResult.m[4]+0.000001f &&
		matrix.m[5] > matResult.m[5]-0.000001f && matrix.m[5] < matResult.m[5]+0.000001f &&
		matrix.m[6] > matResult.m[6]-0.000001f && matrix.m[6] < matResult.m[6]+0.000001f &&
		matrix.m[7] > matResult.m[7]-0.000001f && matrix.m[7] < matResult.m[7]+0.000001f &&
		matrix.m[8] > matResult.m[8]-0.000001f && matrix.m[8] < matResult.m[8]+0.000001f;

	m_dialogEditControl->ReplaceSel(_T("    MatrixToEulerToQuaternionToMatrix pitch 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, -1.0f, 0.0f );
	
	math.MatrixToEuler( matrix, angles );
	quat.FromEuler( angles );
	quat.GetMatrix( matResult4 );
	matResult.SetFrom4x4( matResult4.GetMatrix() );
	subResult =
		matrix.m[0] > matResult.m[0]-0.000001f && matrix.m[0] < matResult.m[0]+0.000001f &&
		matrix.m[1] > matResult.m[1]-0.000001f && matrix.m[1] < matResult.m[1]+0.000001f &&
		matrix.m[2] > matResult.m[2]-0.000001f && matrix.m[2] < matResult.m[2]+0.000001f &&
		matrix.m[3] > matResult.m[3]-0.000001f && matrix.m[3] < matResult.m[3]+0.000001f &&
		matrix.m[4] > matResult.m[4]-0.000001f && matrix.m[4] < matResult.m[4]+0.000001f &&
		matrix.m[5] > matResult.m[5]-0.000001f && matrix.m[5] < matResult.m[5]+0.000001f &&
		matrix.m[6] > matResult.m[6]-0.000001f && matrix.m[6] < matResult.m[6]+0.000001f &&
		matrix.m[7] > matResult.m[7]-0.000001f && matrix.m[7] < matResult.m[7]+0.000001f &&
		matrix.m[8] > matResult.m[8]-0.000001f && matrix.m[8] < matResult.m[8]+0.000001f;

	m_dialogEditControl->ReplaceSel(_T("    MatrixToEulerToQuaternionToMatrix pitch -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f );
	
	math.MatrixToEuler( matrix, angles );
	quat.FromEuler( angles );
	quat.GetMatrix( matResult4 );
	matResult.SetFrom4x4( matResult4.GetMatrix() );
	subResult =
		matrix.m[0] > matResult.m[0]-0.000001f && matrix.m[0] < matResult.m[0]+0.000001f &&
		matrix.m[1] > matResult.m[1]-0.000001f && matrix.m[1] < matResult.m[1]+0.000001f &&
		matrix.m[2] > matResult.m[2]-0.000001f && matrix.m[2] < matResult.m[2]+0.000001f &&
		matrix.m[3] > matResult.m[3]-0.000001f && matrix.m[3] < matResult.m[3]+0.000001f &&
		matrix.m[4] > matResult.m[4]-0.000001f && matrix.m[4] < matResult.m[4]+0.000001f &&
		matrix.m[5] > matResult.m[5]-0.000001f && matrix.m[5] < matResult.m[5]+0.000001f &&
		matrix.m[6] > matResult.m[6]-0.000001f && matrix.m[6] < matResult.m[6]+0.000001f &&
		matrix.m[7] > matResult.m[7]-0.000001f && matrix.m[7] < matResult.m[7]+0.000001f &&
		matrix.m[8] > matResult.m[8]-0.000001f && matrix.m[8] < matResult.m[8]+0.000001f;

	m_dialogEditControl->ReplaceSel(_T("    MatrixToEulerToQuaternionToMatrix roll 90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	matrix.Set(
		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f );
	
	math.MatrixToEuler( matrix, angles );
	quat.FromEuler( angles );
	quat.GetMatrix( matResult4 );
	matResult.SetFrom4x4( matResult4.GetMatrix() );
	subResult =
		matrix.m[0] > matResult.m[0]-0.000001f && matrix.m[0] < matResult.m[0]+0.000001f &&
		matrix.m[1] > matResult.m[1]-0.000001f && matrix.m[1] < matResult.m[1]+0.000001f &&
		matrix.m[2] > matResult.m[2]-0.000001f && matrix.m[2] < matResult.m[2]+0.000001f &&
		matrix.m[3] > matResult.m[3]-0.000001f && matrix.m[3] < matResult.m[3]+0.000001f &&
		matrix.m[4] > matResult.m[4]-0.000001f && matrix.m[4] < matResult.m[4]+0.000001f &&
		matrix.m[5] > matResult.m[5]-0.000001f && matrix.m[5] < matResult.m[5]+0.000001f &&
		matrix.m[6] > matResult.m[6]-0.000001f && matrix.m[6] < matResult.m[6]+0.000001f &&
		matrix.m[7] > matResult.m[7]-0.000001f && matrix.m[7] < matResult.m[7]+0.000001f &&
		matrix.m[8] > matResult.m[8]-0.000001f && matrix.m[8] < matResult.m[8]+0.000001f;

	m_dialogEditControl->ReplaceSel(_T("    MatrixToEulerToQuaternionToMatrix roll -90°: "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestMatrixToEulerToQuaternionToMatrix overall: "));
	return result;
}

bool CMathLibraryTest::TestCrossProduct()
{
	m_dialogEditControl->ReplaceSel(_T("  Testing Cross Product - \n"));
	Vec3 crossTest1( 0,0,1);
	Vec3 crossTest2( 0,1,0);
	Vec3 crossAssert(1,0,0);
	Vec3 crossResult = crossTest2.Cross( crossTest1 );
	m_dialogEditControl->ReplaceSel(_T("  TestCrossProduct overall: "));
	if( crossAssert == crossResult )
	{
		return true;
	}
	return false;
}

bool CMathLibraryTest::TestVectorRotations()
{
	bool result = true;
	bool subResult = false;

	m_dialogEditControl->ReplaceSel(_T("  Testing Vector Rotations - \n"));
	MathUtil math;
	Vec3 testVec;

	testVec.Set( 0.0f, 0.0f, 0.0f );
	testVec.RotateAboutAxisX( 90 );
	subResult = testVec == Vec3( 0.0f, 0.0f, 0.0f );
	m_dialogEditControl->ReplaceSel(_T("    RotateAboutAxisX 90 from 0, 0, 0: "));
	SUBREPORT( subResult );
	result = result && subResult;

	testVec.Set( 0.0f, 0.0f, 1.0f );
	testVec.RotateAboutAxisX( 90 );
	subResult =
		testVec.x > -0.000001f && testVec.x < 0.000001f &&
		testVec.y == 1.0f &&
		testVec.z > -0.000001f && testVec.z < 0.000001f;
	m_dialogEditControl->ReplaceSel(_T("    RotateAboutAxisX 90° from 0, 0, 1: "));
	SUBREPORT( subResult );
	result = result && subResult;

	testVec.Set( 0.0f, 0.0f, 1.0f );
	testVec.RotateAboutAxisX( -90 );
	subResult =
		testVec.x > -0.000001f && testVec.x < 0.000001f &&
		testVec.y == -1.0f &&
		testVec.z > -0.000001f && testVec.z < 0.000001f;
	m_dialogEditControl->ReplaceSel(_T("    RotateAboutAxisX -90° from 0, 0, 1: "));
	SUBREPORT( subResult );
	result = result && subResult;

	testVec.Set( 0.0f, 1.0f, 0.0f );
	testVec.RotateAboutAxisX( 90 );
	subResult =
		testVec.x > -0.000001f && testVec.x < 0.000001f &&
		testVec.y > -0.000001f && testVec.y < 0.000001f &&
		testVec.z == -1.0f;
	m_dialogEditControl->ReplaceSel(_T("    RotateAboutAxisX 90° from 0, 1, 0: "));
	SUBREPORT( subResult );
	result = result && subResult;

	testVec.Set( 0.0f, 1.0f, 0.0f );
	testVec.RotateAboutAxisX( -90 );
	subResult =
		testVec.x > -0.000001f && testVec.x < 0.000001f &&
		testVec.y > -0.000001f && testVec.y < 0.000001f &&
		testVec.z == 1.0f;
	m_dialogEditControl->ReplaceSel(_T("    RotateAboutAxisX -90° from 0, 1, 0: "));
	SUBREPORT( subResult );
	result = result && subResult;

	testVec.Set( 0.0f, 0.0f, 1.0f );
	testVec.RotateAboutAxisY( 90 );
	subResult =
		testVec.x == 1.0f &&
		testVec.y > -0.000001f && testVec.y < 0.000001f &&
		testVec.z > -0.000001f && testVec.z < 0.000001f;
	m_dialogEditControl->ReplaceSel(_T("    RotateAboutAxisY 90° from 0, 0, 1: "));
	SUBREPORT( subResult );
	result = result && subResult;

	testVec.Set( 0.0f, 0.0f, 1.0f );
	testVec.RotateAboutAxisY( -90 );
	subResult =
		testVec.x == -1.0f &&
		testVec.y > -0.000001f && testVec.y < 0.000001f &&
		testVec.z > -0.000001f && testVec.z < 0.000001f;
	m_dialogEditControl->ReplaceSel(_T("    RotateAboutAxisY -90° from 0, 0, 1: "));
	SUBREPORT( subResult );
	result = result && subResult;

	testVec.Set( 1.0f, 0.0f, 0.0f );
	testVec.RotateAboutAxisY( 90 );
	subResult =
		testVec.x > -0.000001f && testVec.x < 0.000001f &&
		testVec.y > -0.000001f && testVec.y < 0.000001f &&
		testVec.z == -1.0f;
	m_dialogEditControl->ReplaceSel(_T("    RotateAboutAxisY 90° from 1, 0, 0: "));
	SUBREPORT( subResult );
	result = result && subResult;

	testVec.Set( 1.0f, 0.0f, 0.0f );
	testVec.RotateAboutAxisY( -90 );
	subResult =
		testVec.x > -0.000001f && testVec.x < 0.000001f &&
		testVec.y > -0.000001f && testVec.y < 0.000001f &&
		testVec.z == 1.0f;
	m_dialogEditControl->ReplaceSel(_T("    RotateAboutAxisY -90° from 1, 0, 0: "));
	SUBREPORT( subResult );
	result = result && subResult;

	testVec.Set( 0.0f, 1.0f, 0.0f );
	testVec.RotateAboutAxisZ( 90 );
	subResult =
		testVec.x == 1.0f &&
		testVec.y > -0.000001f && testVec.y < 0.000001f &&
		testVec.z > -0.000001f && testVec.z < 0.000001f;
	m_dialogEditControl->ReplaceSel(_T("    RotateAboutAxisZ 90° from 0, 1, 0: "));
	SUBREPORT( subResult );
	result = result && subResult;

	testVec.Set( 0.0f, 1.0f, 0.0f );
	testVec.RotateAboutAxisZ( -90 );
	subResult =
		testVec.x == -1.0f &&
		testVec.y > -0.000001f && testVec.y < 0.000001f &&
		testVec.z > -0.000001f && testVec.z < 0.000001f;
	m_dialogEditControl->ReplaceSel(_T("    RotateAboutAxisZ -90° from 0, 1, 0: "));
	SUBREPORT( subResult );
	result = result && subResult;

	testVec.Set( 1.0f, 0.0f, 0.0f );
	testVec.RotateAboutAxisZ( 90 );
	subResult =
		testVec.x > -0.000001f && testVec.x < 0.000001f &&
		testVec.y == -1.0f;
		testVec.z > -0.000001f && testVec.z < 0.000001f;
	m_dialogEditControl->ReplaceSel(_T("    RotateAboutAxisZ 90° from 1, 0, 0: "));
	SUBREPORT( subResult );
	result = result && subResult;

	testVec.Set( 1.0f, 0.0f, 0.0f );
	testVec.RotateAboutAxisZ( -90 );
	subResult =
		testVec.x > -0.000001f && testVec.x < 0.000001f &&
		testVec.y == 1.0f;
		testVec.z > -0.000001f && testVec.z < 0.000001f;
	m_dialogEditControl->ReplaceSel(_T("    RotateAboutAxisZ -90° from 1, 0, 0: "));
	SUBREPORT( subResult );
	result = result && subResult;

	m_dialogEditControl->ReplaceSel(_T("  TestVectorRotations overall: "));
	return result;
}