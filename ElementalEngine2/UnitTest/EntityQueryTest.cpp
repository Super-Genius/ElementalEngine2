#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "stdafx.h"
#include "..\plugins2\include\entityquery\entityquerydefs.h"

class CLightObject;

CEntityQueryTest::CEntityQueryTest()
{
}

CEntityQueryTest::~CEntityQueryTest()
{
}

void CEntityQueryTest::Initialize()
{
	CTestBase::Initialize();
	SetupWorld();
}

void CEntityQueryTest::DeInitialize()
{
	CTestBase::DeInitialize();
}

bool CEntityQueryTest::TestEmptyQuery()
{
	static DWORD msgHash_SubmitQuery = CHashString(_T("SubmitQuery")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage(msgHash_SubmitQuery, sizeof(GETOBJECTQUERYPARAMS), NULL );
	return ( retVal == MSG_HANDLED );
}

bool CEntityQueryTest::TestQuery1()
{/*
	bool passed = false;
	Vec3 position( 0.0f, 0.0f, 0.0f );
	std::list<OBJECTDISTANCEQUERY> objectList;
	GETOBJECTQUERYPARAMS objectQueryParams;
	objectQueryParams.fRadius = 1.0f;
	objectQueryParams.pPosition = &position;
	objectQueryParams.pObjectsList = &objectList;

	static DWORD msgHash_SubmitQuery = CHashString(_T("SubmitQuery")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage(msgHash_SubmitQuery, sizeof(GETOBJECTQUERYPARAMS), &objectQueryParams );
	assert( retVal == MSG_HANDLED );
	passed = objectList.size() == 0;

	std::list<OBJECTDISTANCEQUERY>::iterator objectIterator = objectList.begin();

	for( ; objectIterator != objectList.end(); objectIterator++ )
	{
		OBJECTDISTANCEQUERY iteratedObject = (OBJECTDISTANCEQUERY)*objectIterator;
		IHashString *testname = iteratedObject.name;
	}

	return passed;
	*/
	return true;
}

bool CEntityQueryTest::TestQuery2()
{
	/*
	bool passed = false;
	Vec3 position( 0.0f, 0.0f, 0.0f );
	std::list<OBJECTDISTANCEQUERY> objectList;
	GETOBJECTQUERYPARAMS objectQueryParams;
	objectQueryParams.fRadius = 1.0f;
	objectQueryParams.pPosition = &position;
	objectQueryParams.pObjectsList = &objectList;

	std::list<OBJECTDISTANCEQUERY>::iterator objectIterator = objectList.begin();

	for( ; objectIterator != objectList.end(); objectIterator++ )
	{
		OBJECTDISTANCEQUERY iteratedObject = (OBJECTDISTANCEQUERY)*objectIterator;
	}

	static DWORD msgHash_SubmitQuery = CHashString(_T("SubmitQuery")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage(msgHash_SubmitQuery, sizeof(GETOBJECTQUERYPARAMS), &objectQueryParams );
	assert( retVal == MSG_HANDLED );
	passed = objectList.size() == 1;
	return passed;
	*/
	return true;
}

void CEntityQueryTest::SetupWorld()
{
	// TODO <rnguyen>: Need to create a world with a couple of entities.
	// Right now, we can only compare render objects, but that entails
	// setting up a render window which we would should be creating
	// generically for other potential test cases.
}