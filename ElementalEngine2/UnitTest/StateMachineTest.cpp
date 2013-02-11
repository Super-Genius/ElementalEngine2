#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "stdafx.h"
#include "QHStateMachine/QHStateMachineDefines.h"

// Construction ////////////////////////////////////////////////////////////////////

CStateMachineTest::CStateMachineTest( CRichEditCtrl *dialogEditControl )
	: CTestBase( dialogEditControl )
	, m_CQHStateMachineType( _T("CQHStateMachine") )
	, m_CQHStateType( _T("CQHState") )
	, m_CQHStateMachineEventType( _T("CQHStateMachineEvent") )
	, m_CQHStateMachineTransitionType( _T("CQHStateMachineTransition") )
	, m_CQHStateMachineActionHandlerType( _T("CQHStateMachineActionHandler") )
	, m_archive( NULL )
{
}

CStateMachineTest::~CStateMachineTest()
{
}

////////////////////////////////////////////////////////////////////////////////////


// Init / DeInit ///////////////////////////////////////////////////////////////////

void CStateMachineTest::Initialize()
{
	CTestBase::Initialize();

	m_archive = CreateMemoryArchive();

	m_StateObjectComponentType.Init( _T("CQHStateObject") );
	m_TestStateObject1.Init( _T("StateObject1") );
	m_TestStateObject3.Init( _T("StateObject3") );
	m_TestStateMachineFile1.Init( _T("TestStateMachine1.xms") );
	m_TestStateMachineFile2.Init( _T("TestStateMachine2.xms") );
	m_TestStateMachineFile3.Init( _T("TestStateMachine3.xms") );
	m_TestStateMachineFile4.Init( _T("TestStateMachine4.xms") );
	m_TestStateMachine1.Init( _T("Test_StateMachine") );
	m_TestStateMachine3.Init( _T("Test_DerivedStateMachine") );
	m_TestStateMachine4.Init( _T("Test_DerivedStateMachine2") );
}

void CStateMachineTest::DeInitialize()
{
	m_archive->Close();
	m_archive = NULL;
}

////////////////////////////////////////////////////////////////////////////////////


// Test functions //////////////////////////////////////////////////////////////////

bool CStateMachineTest::TestQueryStateMachineEvents1()
{
	bool testCasePassed = true;
	bool subResult = true;
	LogMessage(_T("\n"));

	LoadTestStateMachineFile1();

	QUERYEVENTSPARAMS queryEventsParams;
	queryEventsParams.StateMachineName = NULL;
	static DWORD msgHash_QueryStateMachineEvents = CHashString(_T("QueryStateMachineEvents")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_QueryStateMachineEvents, sizeof(QUERYEVENTSPARAMS), &queryEventsParams );
	assert( retval == MSG_HANDLED );
	subResult = queryEventsParams.QueryEventsList.size() == 0;
	LogMessage(_T("    Zero events found for NULL state machine: "));
	SUBREPORT( subResult );
	testCasePassed = testCasePassed && subResult;

	CHashString hszGarbageStateMachine(_T("GarbageStateMachine"));
	queryEventsParams.StateMachineName = &hszGarbageStateMachine;
	retval = EngineGetToolBox()->SendMessage(msgHash_QueryStateMachineEvents, sizeof(QUERYEVENTSPARAMS), &queryEventsParams );
	assert( retval == MSG_HANDLED );
	subResult = queryEventsParams.QueryEventsList.size() == 0;
	LogMessage(_T("    Zero events found for garbage state machine: "));
	SUBREPORT( subResult );
	testCasePassed = testCasePassed && subResult;

	queryEventsParams.StateMachineName = &m_TestStateMachine1;
	retval = EngineGetToolBox()->SendMessage(msgHash_QueryStateMachineEvents, sizeof(QUERYEVENTSPARAMS), &queryEventsParams );
	assert( retval == MSG_HANDLED );
	subResult = queryEventsParams.QueryEventsList.size() == 11;
	LogMessage(_T("    Events found for test state machine: "));
	SUBREPORT( subResult );
	testCasePassed = testCasePassed && subResult;

	subResult = true;
	LogMessage(_T("    Compare queried events in test state machine: "));
	QUERYEVENTSLIST::iterator queryEventsIterator = queryEventsParams.QueryEventsList.begin();

	assert( queryEventsIterator != queryEventsParams.QueryEventsList.end() );
	QUERYEVENTITEM eventItem = (QUERYEVENTITEM)*queryEventsIterator;
	subResult = subResult && ( *eventItem.StateName == CHashString("Test_Start_State") );
	subResult = subResult && ( *eventItem.EventName == CHashString("Test_Start_Entry_Event") );

	queryEventsIterator++;
	assert( queryEventsIterator != queryEventsParams.QueryEventsList.end() );
	eventItem = (QUERYEVENTITEM)*queryEventsIterator;
	subResult = subResult && ( *eventItem.StateName == CHashString("Test_Main_State") );
	subResult = subResult && ( *eventItem.EventName == CHashString("Test_Update_Event") );

	queryEventsIterator++;
	assert( queryEventsIterator != queryEventsParams.QueryEventsList.end() );
	eventItem = (QUERYEVENTITEM)*queryEventsIterator;
	subResult = subResult && ( *eventItem.StateName == CHashString("Test_Main_State") );
	subResult = subResult && ( *eventItem.EventName == CHashString("Test_Main_Attack_Event") );

	queryEventsIterator++;
	assert( queryEventsIterator != queryEventsParams.QueryEventsList.end() );
	eventItem = (QUERYEVENTITEM)*queryEventsIterator;
	subResult = subResult && ( *eventItem.StateName == CHashString("Test_Main_State") );
	subResult = subResult && ( *eventItem.EventName == CHashString("Test_Main_Idle_Event") );

	queryEventsIterator++;
	assert( queryEventsIterator != queryEventsParams.QueryEventsList.end() );
	eventItem = (QUERYEVENTITEM)*queryEventsIterator;
	subResult = subResult && ( *eventItem.StateName == CHashString("Test_Main_Idle_State") );
	subResult = subResult && ( *eventItem.EventName == CHashString("Test_Update_Event") );

	queryEventsIterator++;
	assert( queryEventsIterator != queryEventsParams.QueryEventsList.end() );
	eventItem = (QUERYEVENTITEM)*queryEventsIterator;
	subResult = subResult && ( *eventItem.StateName == CHashString("Test_Main_Idle_State") );
	subResult = subResult && ( *eventItem.EventName == CHashString("Test_Main_Attack_Event") );

	queryEventsIterator++;
	assert( queryEventsIterator != queryEventsParams.QueryEventsList.end() );
	eventItem = (QUERYEVENTITEM)*queryEventsIterator;
	subResult = subResult && ( *eventItem.StateName == CHashString("Test_Main_Idle_State") );
	subResult = subResult && ( *eventItem.EventName == CHashString("Test_Main_Idle_Event") );

	queryEventsIterator++;
	assert( queryEventsIterator != queryEventsParams.QueryEventsList.end() );
	eventItem = (QUERYEVENTITEM)*queryEventsIterator;
	subResult = subResult && ( *eventItem.StateName == CHashString("Test_Main_Attack_State") );
	subResult = subResult && ( *eventItem.EventName == CHashString("Test_Update_Event") );

	queryEventsIterator++;
	assert( queryEventsIterator != queryEventsParams.QueryEventsList.end() );
	eventItem = (QUERYEVENTITEM)*queryEventsIterator;
	subResult = subResult && ( *eventItem.StateName == CHashString("Test_Main_Attack_State") );
	subResult = subResult && ( *eventItem.EventName == CHashString("Test_Main_Attack_Event") );

	queryEventsIterator++;
	assert( queryEventsIterator != queryEventsParams.QueryEventsList.end() );
	eventItem = (QUERYEVENTITEM)*queryEventsIterator;
	subResult = subResult && ( *eventItem.StateName == CHashString("Test_Main_Attack_State") );
	subResult = subResult && ( *eventItem.EventName == CHashString("Test_Main_Idle_Event") );

	queryEventsIterator++;
	assert( queryEventsIterator != queryEventsParams.QueryEventsList.end() );
	eventItem = (QUERYEVENTITEM)*queryEventsIterator;
	subResult = subResult && ( *eventItem.StateName == CHashString("Test_Main_Attack_State") );
	subResult = subResult && ( *eventItem.EventName == CHashString("Test_Attack_Update_Event") );

	queryEventsIterator++;
	subResult = subResult && queryEventsIterator == queryEventsParams.QueryEventsList.end();

	SUBREPORT( subResult );
	testCasePassed = testCasePassed && subResult;

	DeleteObject( &m_TestStateMachine1 );

	LogMessage(_T("  TestQueryStateMachineEvents1 overall: "));
	return testCasePassed;
}

bool CStateMachineTest::TestLoadStateMachine1()
{
	bool result = true;
	bool subResult = true;
	LogMessage(_T("\n"));
	LoadTestStateMachineFile1();

	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &m_TestStateMachine1;
	DWORD retVal = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );
	LogMessage(_T("    Found loaded state machine: "));
	subResult = (retVal == MSG_HANDLED) && (param.bFound);
	SUBREPORT( subResult );
	result = result && subResult;

	StateMachineRootNode *expectedRoot = new StateMachineRootNode;
	GenerateExpectedFile1Structure( expectedRoot );
	subResult = CheckFileStructure( &m_TestStateMachine1, expectedRoot );
	delete expectedRoot;
	result = result && subResult;

	LogMessage(_T("  TestLoadStateMachine1 overall: "));

	DeleteObject( &m_TestStateMachine1 );
	return result;
}

bool CStateMachineTest::TestCreateStateObject1()
{
	bool result = true;
	bool subResult = true;

	LogMessage(_T("\n"));

	CreateStateObject( &m_TestStateObject1, &m_TestStateMachineFile1, &m_TestStateMachine1, 0 );

	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &m_TestStateObject1;
	DWORD retVal = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );
	LogMessage(_T("    Found loaded state object: "));
	subResult = (retVal == MSG_HANDLED) && (param.bFound);
	result = result && subResult;
	SUBREPORT( subResult );

	CHashString hszStateName( _T("") );
	static DWORD msgHash_GetCurrentState = CHashString("GetCurrentState").GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetCurrentState, sizeof(CHashString), &hszStateName, &m_TestStateObject1, &m_StateObjectComponentType );

	LogMessage(_T("    State object entered into correct state: "));
	subResult = hszStateName == _T("Test_Main_Idle_State");
	result = result && subResult;
	SUBREPORT( subResult );

	TRIGGEREVENTPARAMS eventParams;
	CHashString hszAttackEvent( _T("Test_Main_Attack_Event") );
	eventParams.EventName = &hszAttackEvent;
	eventParams.EventParamsArchive = NULL;

	static DWORD msgHash_TriggerEvent = CHashString("TriggerEvent").GetUniqueID();
	m_ToolBox->SendMessage(msgHash_TriggerEvent, sizeof(TRIGGEREVENTPARAMS), &eventParams, &m_TestStateObject1, &m_StateObjectComponentType );
	m_ToolBox->SendMessage(msgHash_GetCurrentState, sizeof(CHashString), &hszStateName, &m_TestStateObject1, &m_StateObjectComponentType );

	LogMessage(_T("    State event transitioned into correct state: "));
	subResult = hszStateName == _T("Test_Main_Attack_State");
	result = result && subResult;
	SUBREPORT( subResult );

	// Try triggering an event that does not exist.
	hszAttackEvent.Init(_T("GARBAGE_EVENT"));
	m_ToolBox->SendMessage(msgHash_TriggerEvent, sizeof(TRIGGEREVENTPARAMS), &eventParams, &m_TestStateObject1, &m_StateObjectComponentType );
	m_ToolBox->SendMessage(msgHash_GetCurrentState, sizeof(CHashString), &hszStateName, &m_TestStateObject1, &m_StateObjectComponentType );

	LogMessage(_T("    State event transitioned into correct state: "));
	subResult = hszStateName == _T("Test_Main_Attack_State");
	result = result && subResult;
	SUBREPORT( subResult );

	DeleteObject( &m_TestStateObject1 );
	DeleteObject( &m_TestStateMachine1 );

	LogMessage(_T("  TestCreateStateObject1 overall: "));

	return result;
}

bool CStateMachineTest::TestStateMachineEvents1()
{
	bool result = true;
	bool subResult = true;

	CreateStateObject( &m_TestStateObject1, &m_TestStateMachineFile2, &m_TestStateMachine1, 0 );

	LogMessage(_T("\n"));

	CHashString hszStateName(_T(""));

	LogMessage(_T("    Initial state transition to idle: "));
	static DWORD msgHash_GetCurrentState = CHashString("GetCurrentState").GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetCurrentState, sizeof(CHashString), &hszStateName, &m_TestStateObject1, &m_StateObjectComponentType );
	subResult = hszStateName == _T("Test_Main_Idle_State");
	result = result && subResult;
	SUBREPORT( subResult );

	TRIGGEREVENTPARAMS eventParams;
	CHashString hszEvent( _T("Test_Main_Idle_Event") );
	eventParams.EventName = &hszEvent;
	eventParams.EventParamsArchive = NULL;

	LogMessage(_T("    Event to guarded state: "));
	static DWORD msgHash_TriggerEvent = CHashString("TriggerEvent").GetUniqueID();
	m_ToolBox->SendMessage(msgHash_TriggerEvent, sizeof(TRIGGEREVENTPARAMS), &eventParams, &m_TestStateObject1, &m_StateObjectComponentType );
	m_ToolBox->SendMessage(msgHash_GetCurrentState, sizeof(CHashString), &hszStateName, &m_TestStateObject1, &m_StateObjectComponentType );
	subResult = hszStateName == _T("Test_Main_Guard_State");
	result = result && subResult;
	SUBREPORT( subResult );

	hszEvent.Init( _T("Test_Main_Attack_Event") );
	LogMessage(_T("    Trigger event again unguarded: "));
	m_ToolBox->SendMessage(msgHash_TriggerEvent, sizeof(TRIGGEREVENTPARAMS), &eventParams, &m_TestStateObject1, &m_StateObjectComponentType );
	m_ToolBox->SendMessage(msgHash_GetCurrentState, sizeof(CHashString), &hszStateName, &m_TestStateObject1, &m_StateObjectComponentType );
	subResult = hszStateName == _T("Test_Main_Attack_State");
	result = result && subResult;
	SUBREPORT( subResult );

	DeleteObject( &m_TestStateObject1 );
	DeleteObject( &m_TestStateMachine1 );

	LogMessage(_T("  TestStateMachineEvents1 overall: "));
	return result;
}

bool CStateMachineTest::TestDerivedStateMachine()
{
	bool result = true;
	bool subResult = true;
	LogMessage(_T("\n"));
	LoadTestStateMachineFile3();

	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &m_TestStateMachine3;
	DWORD retVal = m_ToolBox->SendMessage( msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );
	LogMessage(_T("    Found loaded state machine 3: "));
	subResult = (retVal == MSG_HANDLED) && (param.bFound);
	SUBREPORT( subResult );
	result = result && subResult;

	StateMachineRootNode *expectedRoot = new StateMachineRootNode;
	GenerateExpectedFile3Structure( expectedRoot );
	subResult = CheckFileStructure( &m_TestStateMachine3, expectedRoot );
	delete expectedRoot;
	result = result && subResult;

	LoadTestStateMachineFile4();

	param.hszName = &m_TestStateMachine4;
	retVal = m_ToolBox->SendMessage( msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );
	LogMessage(_T("    Found loaded state machine 4: "));
	subResult = (retVal == MSG_HANDLED) && (param.bFound);
	SUBREPORT( subResult );
	result = result && subResult;

	expectedRoot = new StateMachineRootNode;
	GenerateExpectedFile4Structure( expectedRoot );
	subResult = CheckFileStructure( &m_TestStateMachine4, expectedRoot );
	delete expectedRoot;
	result = result && subResult;

	LogMessage(_T("  TestDerivedStateMachine overall: "));

	DeleteObject( &m_TestStateMachine1 );
	DeleteObject( &m_TestStateMachine3 );
	DeleteObject( &m_TestStateMachine4 );
	return result;
}

bool CStateMachineTest::TestDerivedStateMachineEvents()
{
	bool result = true;
	bool subResult = true;

	CreateStateObject( &m_TestStateObject3, &m_TestStateMachineFile3, &m_TestStateMachine3, 0 );

	LogMessage(_T("\n"));

	CHashString hszStateName(_T(""));

	LogMessage(_T("    Transition to initial state: "));
	static DWORD msgHash_GetCurrentState = CHashString("GetCurrentState").GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetCurrentState, sizeof(CHashString), &hszStateName, &m_TestStateObject3, &m_StateObjectComponentType );
	subResult = hszStateName == _T("Test_Derived_State");
	result = result && subResult;
	SUBREPORT( subResult );

	TRIGGEREVENTPARAMS eventParams;
	CHashString hszEvent( _T("Test_Derived_Event") );
	eventParams.EventName = &hszEvent;
	eventParams.EventParamsArchive = NULL;

	LogMessage(_T("    Triggering derived event: "));
	static DWORD msgHash_TriggerEvent = CHashString("TriggerEvent").GetUniqueID();
	m_ToolBox->SendMessage(msgHash_TriggerEvent, sizeof(TRIGGEREVENTPARAMS), &eventParams, &m_TestStateObject3, &m_StateObjectComponentType );
	m_ToolBox->SendMessage(msgHash_GetCurrentState, sizeof(CHashString), &hszStateName, &m_TestStateObject3, &m_StateObjectComponentType );
	subResult = hszStateName == _T("Test_Main_Attack_State");
	result = result && subResult;
	SUBREPORT( subResult );

	DeleteObject( &m_TestStateObject3 );
	DeleteObject( &m_TestStateMachine1 );
	DeleteObject( &m_TestStateMachine3 );

	LogMessage(_T("  TestDerivedStateMachineEvents overall: "));
	return result;
}

////////////////////////////////////////////////////////////////////////////////////


// Recyclable test objects /////////////////////////////////////////////////////////

void CStateMachineTest::CreateStateObject( IHashString *stateObjectName, IHashString *stateMachineFile, IHashString *stateMachineName, int updatePriority )
{
	CreateObject( stateObjectName, NULL, &m_StateObjectComponentType );

	IArchive *archive = CreateMemoryArchive();

	archive->SetIsWriting( true );
	archive->Write( stateMachineFile->GetString(), _T("FileName") );
	archive->Write( stateMachineName->GetString(), _T("StateMachine") );
	archive->Write( _T(""), _T("StartState") );
	archive->Write( updatePriority, _T("UpdatePriority") );

	archive->SetIsWriting( false );
	archive->SeekTo( 0 );

	SerializeObject( stateObjectName, archive, false );

	archive->Close();
}

void CStateMachineTest::LoadTestStateMachineFile1()
{
	LOADFILEEXTPARAMS lfep;

	lfep.bInternalLoad = true;
	lfep.fileName = (TCHAR*)m_TestStateMachineFile1.GetString();

	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
}

void CStateMachineTest::LoadTestStateMachineFile3()
{
	LOADFILEEXTPARAMS lfep;

	lfep.bInternalLoad = true;
	lfep.fileName = (TCHAR*)m_TestStateMachineFile3.GetString();

	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
}

void CStateMachineTest::LoadTestStateMachineFile4()
{
	LOADFILEEXTPARAMS lfep;

	lfep.bInternalLoad = true;
	lfep.fileName = (TCHAR*)m_TestStateMachineFile4.GetString();

	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
}

bool CStateMachineTest::CheckFileStructure( IHashString *stateMachineName, StateMachineRootNode *expectedRoot )
{
	StateMachineRootNode *actualRoot = new StateMachineRootNode;
	bool checkResult = FillStateMachineStruct( stateMachineName, actualRoot );
	if( checkResult )
	{
		checkResult = CompareRootStructs( expectedRoot, actualRoot );
	}

	delete actualRoot;

	return checkResult;
}

void CStateMachineTest::GenerateExpectedFile1Structure( StateMachineRootNode *expectedRoot )
{
	expectedRoot->name = _T("Test_StateMachine");
	expectedRoot->numStates = 3;
	expectedRoot->startState = _T("Test_Start_State");
	expectedRoot->endState = _T("Test_End_State");

	StateMachineStateNode *statePointer;
	StateMachineEventNode *eventPointer;
	StateMachineActionHandlerNode *actionPointer;
	StateMachineTransitionNode *transitionPointer;

	statePointer = new StateMachineStateNode;
	statePointer->name = _T("Test_Start_State");
	statePointer->numSubStates = 0;
	statePointer->numEvents = 1;
	statePointer->entryEvent = _T("Test_Start_Entry_Event");
	statePointer->updateEvent = _T("");
	statePointer->exitEvent = _T("");
	expectedRoot->subStates[CHashString( statePointer->name ).GetUniqueID()] = statePointer;

	eventPointer = new StateMachineEventNode;
	eventPointer->name = _T("Test_Start_Entry_Event");
	eventPointer->numActions = 1;
	eventPointer->numTransitions = 1;
	statePointer->events[CHashString( eventPointer->name ).GetUniqueID()] = eventPointer;

	actionPointer = new StateMachineActionHandlerNode;
	actionPointer->name = _T("Test_Start_Entry_ActionHandler");
	actionPointer->actionName = _T("Test_ReadyLaunch");
	eventPointer->actions[CHashString( actionPointer->name ).GetUniqueID()] = actionPointer;

	transitionPointer = new StateMachineTransitionNode;
	transitionPointer->name = _T("Test_Start_Entry_Transition");
	transitionPointer->weight = 0;
	transitionPointer->targetState = _T("Test_Main_Idle_State");
	eventPointer->transitions[CHashString( transitionPointer->name ).GetUniqueID()] = transitionPointer;

	statePointer = new StateMachineStateNode;
	statePointer->name = _T("Test_Main_State");
	statePointer->numSubStates = 2;
	statePointer->numEvents = 3;
	statePointer->entryEvent = _T("");
	statePointer->updateEvent = _T("Test_Update_Event");
	statePointer->exitEvent = _T("");
	expectedRoot->subStates[CHashString( statePointer->name ).GetUniqueID()] = statePointer;

	eventPointer = new StateMachineEventNode;
	eventPointer->name = _T("Test_Update_Event");
	eventPointer->numActions = 0;
	eventPointer->numTransitions = 0;
	statePointer->events[CHashString( eventPointer->name ).GetUniqueID()] = eventPointer;

	eventPointer = new StateMachineEventNode;
	eventPointer->name = _T("Test_Main_Attack_Event");
	eventPointer->numActions = 0;
	eventPointer->numTransitions = 1;
	statePointer->events[CHashString( eventPointer->name ).GetUniqueID()] = eventPointer;

	transitionPointer = new StateMachineTransitionNode;
	transitionPointer->name = _T("Test_Main_Attack_Transition");
	transitionPointer->weight = 0;
	transitionPointer->targetState = _T("Test_Main_Attack_State");
	eventPointer->transitions[CHashString( transitionPointer->name ).GetUniqueID()] = transitionPointer;

	eventPointer = new StateMachineEventNode;
	eventPointer->name = _T("Test_Main_Idle_Event");
	eventPointer->numActions = 0;
	eventPointer->numTransitions = 1;
	statePointer->events[CHashString( eventPointer->name ).GetUniqueID()] = eventPointer;

	transitionPointer = new StateMachineTransitionNode;
	transitionPointer->name = _T("Test_Main_Idle_Transition");
	transitionPointer->weight = 0;
	transitionPointer->targetState = _T("Test_Main_Idle_State");
	eventPointer->transitions[CHashString( transitionPointer->name ).GetUniqueID()] = transitionPointer;

	StateMachineStateNode *subStatePointer;
	subStatePointer = new StateMachineStateNode;
	subStatePointer->name = _T("Test_Main_Idle_State");
	subStatePointer->numSubStates = 0;
	subStatePointer->numEvents = 1;
	subStatePointer->entryEvent = _T("");
	subStatePointer->updateEvent = _T("Test_Update_Event");
	subStatePointer->exitEvent = _T("");
	statePointer->subStates[CHashString( subStatePointer->name ).GetUniqueID()] = subStatePointer;

	eventPointer = new StateMachineEventNode;
	eventPointer->name = _T("Test_Update_Event");
	eventPointer->numActions = 0;
	eventPointer->numTransitions = 0;
	subStatePointer->events[CHashString( eventPointer->name ).GetUniqueID()] = eventPointer;

	subStatePointer = new StateMachineStateNode;
	subStatePointer->name = _T("Test_Main_Attack_State");
	subStatePointer->numSubStates = 0;
	subStatePointer->numEvents = 1;
	subStatePointer->entryEvent = _T("");
	subStatePointer->updateEvent = _T("Test_Attack_Update_Event");
	subStatePointer->exitEvent = _T("");
	statePointer->subStates[CHashString( subStatePointer->name ).GetUniqueID()] = subStatePointer;

	eventPointer = new StateMachineEventNode;
	eventPointer->name = _T("Test_Attack_Update_Event");
	eventPointer->numActions = 0;
	eventPointer->numTransitions = 0;
	subStatePointer->events[CHashString( eventPointer->name ).GetUniqueID()] = eventPointer;

	statePointer = new StateMachineStateNode;
	statePointer->name = _T("Test_End_State");
	statePointer->numSubStates = 0;
	statePointer->numEvents = 0;
	statePointer->entryEvent = _T("");
	statePointer->updateEvent = _T("");
	statePointer->exitEvent = _T("");
	expectedRoot->subStates[CHashString( statePointer->name ).GetUniqueID()] = statePointer;
}

void CStateMachineTest::GenerateExpectedFile3Structure( StateMachineRootNode *expectedRoot )
{
	StateMachineStateNode *statePointer;
	StateMachineStateNode *parentState;
	StateMachineEventNode *eventPointer;
	StateMachineTransitionNode *transitionPointer;

	// fills derived events
	GenerateExpectedFile1Structure( expectedRoot );

	expectedRoot->name = _T("Test_DerivedStateMachine");
	expectedRoot->numStates = 4;
	expectedRoot->startState = _T("Test_Derived_State");
	expectedRoot->endState = _T("Test_End_State");
	expectedRoot->parentStateMachinefile = _T("TestStateMachine1.xms");
	expectedRoot->parentStateMachinefile.MakeSafeFileName();
	expectedRoot->parentStateMachine = _T("Test_StateMachine");

	statePointer = new StateMachineStateNode;
	statePointer->name = _T("Test_Derived_State");
	statePointer->numSubStates = 0;
	statePointer->numEvents = 0;
	statePointer->entryEvent = _T("");
	statePointer->updateEvent = _T("");
	statePointer->exitEvent = _T("");
	parentState = expectedRoot->subStates[CHashString( _T("Test_Main_State") ).GetUniqueID()];
	parentState->subStates[CHashString( statePointer->name ).GetUniqueID()] = statePointer;
	++parentState->numSubStates;
	
	eventPointer = new StateMachineEventNode;
	eventPointer->name = _T("Test_Derived_Event");
	eventPointer->numActions = 0;
	eventPointer->numTransitions = 1;
	parentState->events[CHashString( eventPointer->name ).GetUniqueID()] = eventPointer;
	++parentState->numEvents;

	transitionPointer = new StateMachineTransitionNode;
	transitionPointer->name = _T("Test_Derived_Transition");
	transitionPointer->weight = 0;
	transitionPointer->targetState = _T("Test_Main_Attack_State");
	eventPointer->transitions[CHashString( transitionPointer->name ).GetUniqueID()] = transitionPointer;

	statePointer = new StateMachineStateNode;
	statePointer->name = _T("Test_Main2_State");
	statePointer->numSubStates = 0;
	statePointer->numEvents = 0;
	statePointer->entryEvent = _T("");
	statePointer->updateEvent = _T("");
	statePointer->exitEvent = _T("");
	expectedRoot->subStates[CHashString( statePointer->name ).GetUniqueID()] = statePointer;
}

void CStateMachineTest::GenerateExpectedFile4Structure( StateMachineRootNode *expectedRoot )
{
	StateMachineStateNode *statePointer;

	// fills derived events
	GenerateExpectedFile3Structure( expectedRoot );

	expectedRoot->name = _T("Test_DerivedStateMachine2");
	expectedRoot->numStates = 5;
	expectedRoot->startState = _T("Test_Start_State");
	expectedRoot->endState = _T("Test_End_State");
	expectedRoot->parentStateMachinefile = _T("TestStateMachine3.xms");
	expectedRoot->parentStateMachinefile.MakeSafeFileName();
	expectedRoot->parentStateMachine = _T("Test_DerivedStateMachine");

	statePointer = new StateMachineStateNode;
	statePointer->name = _T("Test_Main3_State");
	statePointer->numSubStates = 0;
	statePointer->numEvents = 0;
	statePointer->entryEvent = _T("");
	statePointer->updateEvent = _T("");
	statePointer->exitEvent = _T("");
	expectedRoot->subStates[CHashString( statePointer->name ).GetUniqueID()] = statePointer;
}

bool CStateMachineTest::FillStateMachineStruct( IHashString *objectName, StateMachineRootNode *rootStruct )
{
	float fVersion;
	ReadObjectState( objectName );
	m_archive->Read( fVersion, _T("version") );
	TestAssertEquals( _T("State machine archive version"), 1.3f, fVersion );

	m_archive->Read( rootStruct->name, _T("name") );
	TestAssertEquals( _T("Name of state machine"), objectName->GetString(), rootStruct->name );
	m_archive->Read( rootStruct->numStates, _T("numStates") );
	m_archive->Read( rootStruct->startState, _T("startState") );
	m_archive->Read( rootStruct->endState, _T("endState") );
	m_archive->Read( rootStruct->parentStateMachinefile, _T("parentStateMachinefile") );
	m_archive->Read( rootStruct->parentStateMachine, _T("parentStateMachine") );

	GETSETSTATEMACHINENAMESPARAMS gssmn;
	static DWORD msgHash_GetChildStateNames = CHashString(_T("GetChildStateNames")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage( msgHash_GetChildStateNames, sizeof(gssmn), &gssmn, objectName, &m_CQHStateMachineType );
	assert(retval == MSG_HANDLED);
	rootStruct->numStates = (int)gssmn.Names.size();

	if (!rootStruct->parentStateMachinefile.empty())
	{
		CHashString hszFilepath = rootStruct->parentStateMachinefile.c_str();
		GETSTATEMACHINEPARAMS gsmp;
		gsmp.StateMachineFilePath = &hszFilepath;
		static DWORD msgHash_GetStateMachineName = CHashString(_T("GetStateMachineName")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_GetStateMachineName, sizeof(gsmp), &gsmp);
		assert(retval == MSG_HANDLED);
		rootStruct->parentStateMachine = gsmp.StateMachineName->GetString();
	}

	StateMachineStateNode *stateStruct;
	list<IHashString *>::iterator it = gssmn.Names.begin();
	for( ; it != gssmn.Names.end(); ++it )
	{
		stateStruct = new StateMachineStateNode;
		FillStateStruct( *it, stateStruct );
		rootStruct->subStates[CHashString( stateStruct->name ).GetUniqueID()] = stateStruct;
	}
	return true;
}

bool CStateMachineTest::FillStateStruct( IHashString *objectName, StateMachineStateNode *stateStruct )
{
	float fVersion;
	ReadObjectState( objectName );
	m_archive->Read( fVersion, _T("version") );
	TestAssertEquals( _T("State archive version"), 1.2f, fVersion );

	m_archive->Read( stateStruct->name, _T("name") );
	m_archive->Read( stateStruct->entryEvent, _T("entryEvent") );
	m_archive->Read( stateStruct->updateEvent, _T("updateEvent") );
	m_archive->Read( stateStruct->exitEvent, _T("exitEvent") );

	GETSETSTATEMACHINENAMESPARAMS gssmn;
	static DWORD msgHash_GetStateEventLabels = CHashString(_T("GetStateEventLabels")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage( msgHash_GetStateEventLabels, sizeof(gssmn), &gssmn, objectName, &m_CQHStateType );
	assert(retval == MSG_HANDLED);
	stateStruct->numEvents = (int)gssmn.Names.size();

	StateMachineEventNode *eventStruct;
	list<IHashString *>::iterator it = gssmn.Names.begin();
	for( ; it != gssmn.Names.end(); ++it )
	{
		eventStruct = new StateMachineEventNode;
		CHashString hszName = *it;
		static DWORD msgHash_GetEventNameFromLabel = CHashString( _T("GetEventNameFromLabel") ).GetUniqueID();
		retval = m_ToolBox->SendMessage( msgHash_GetEventNameFromLabel, sizeof(IHashString), &hszName, objectName, &m_CQHStateType );
		assert(retval == MSG_HANDLED);
		FillEventStruct( &hszName, eventStruct );
		stateStruct->events[CHashString( eventStruct->name ).GetUniqueID()] = eventStruct;
	}

	gssmn.Names.clear();
	static DWORD msgHash_GetChildStateNames = CHashString(_T("GetChildStateNames")).GetUniqueID();
	retval = m_ToolBox->SendMessage( msgHash_GetChildStateNames, sizeof(gssmn), &gssmn, objectName, &m_CQHStateType );
	assert(retval == MSG_HANDLED);
	stateStruct->numSubStates = (int)gssmn.Names.size();

	StateMachineStateNode *subStateStruct;
	for( it = gssmn.Names.begin(); it != gssmn.Names.end(); ++it )
	{
		subStateStruct = new StateMachineStateNode;
		FillStateStruct( *it, subStateStruct );
		stateStruct->subStates[CHashString( subStateStruct->name ).GetUniqueID()] = subStateStruct;
	}
	stateStruct->objectName = objectName;
	return true;
}

bool CStateMachineTest::FillEventStruct( IHashString *objectName, StateMachineEventNode *eventStruct )
{
	float fVersion;
	ReadObjectState( objectName );
	m_archive->Read( fVersion, _T("version") );
	TestAssertEquals( _T("Event archive version"), 1.2f, fVersion );
	m_archive->Read( eventStruct->name, _T("name") );

	GETSETSTATEMACHINENAMESPARAMS gssmn;
	static DWORD msgHash_GetEventActionHandlers = CHashString(_T("GetEventActionHandlers")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage( msgHash_GetEventActionHandlers, sizeof(gssmn), &gssmn, objectName, &m_CQHStateMachineEventType );
	assert(retval == MSG_HANDLED);
	eventStruct->numActions = (int)gssmn.Names.size();

	StateMachineActionHandlerNode *actionStruct;
	list<IHashString *>::iterator it = gssmn.Names.begin();
	for( ; it != gssmn.Names.end(); ++it )
	{
		actionStruct = new StateMachineActionHandlerNode;
		FillActionHandlerStruct( *it, actionStruct );
		eventStruct->actions[CHashString( actionStruct->name ).GetUniqueID()] = actionStruct;
	}

	gssmn.Names.clear();
	static DWORD msgHash_GetEventTransitions = CHashString(_T("GetEventTransitions")).GetUniqueID();
	retval = m_ToolBox->SendMessage( msgHash_GetEventTransitions, sizeof(gssmn), &gssmn, objectName, &m_CQHStateMachineEventType );
	assert(retval == MSG_HANDLED);
	eventStruct->numTransitions = (int)gssmn.Names.size();
	StateMachineTransitionNode *transitionStruct;
	for( it = gssmn.Names.begin(); it != gssmn.Names.end(); ++it )
	{
		transitionStruct = new StateMachineTransitionNode;
		FillTransitionStruct( *it, transitionStruct );
		eventStruct->transitions[CHashString( transitionStruct->name ).GetUniqueID()] = transitionStruct;
	}
	eventStruct->objectName = objectName;
	return true;
}

bool CStateMachineTest::FillActionHandlerStruct( IHashString *objectName, StateMachineActionHandlerNode *actionStruct )
{
	float fVersion;
	ReadObjectState( objectName );
	m_archive->Read( fVersion, _T("version") );
	TestAssertEquals( _T("Action handler archive version"), 1.0f, fVersion );
	m_archive->Read( actionStruct->name, _T("name") );
	m_archive->Read( actionStruct->actionName, _T("actionName") );
	actionStruct->objectName = objectName;
	return true;
}

bool CStateMachineTest::FillTransitionStruct( IHashString *objectName, StateMachineTransitionNode *transitionStruct )
{
	float fVersion;
	ReadObjectState( objectName );
	m_archive->Read( fVersion, _T("version") );
	TestAssertEquals( _T("Transition archive version"), 1.0f, fVersion );
	m_archive->Read( transitionStruct->name, _T("name") );
	m_archive->Read( transitionStruct->weight, _T("weight") );
	m_archive->Read( transitionStruct->targetState, _T("targetState") );
	transitionStruct->objectName = objectName;
	return true;
}

bool CStateMachineTest::CompareRootStructs( StateMachineRootNode *root1, StateMachineRootNode *root2 )
{
	bool checkResult = true;

	checkResult = checkResult && root1->name == root2->name;
	checkResult = checkResult && root1->numStates == root2->numStates;
	checkResult = checkResult && root1->startState == root2->startState;
	checkResult = checkResult && root1->endState == root2->endState;
	checkResult = checkResult && root1->parentStateMachinefile == root2->parentStateMachinefile;
	checkResult = checkResult && root1->parentStateMachine == root2->parentStateMachine;

	assert( root1->numStates == root1->subStates.size() );
	assert( root2->numStates == root2->subStates.size() );

	LogMessage(_T("    Root node "));
	LogMessage( root1->name );
	LogMessage(_T(" compare values: "));
	SUBREPORT( checkResult );

	LogMessage(_T("    Root node "));
	LogMessage( root1->name );
	LogMessage(_T(" states count: "));
	if( root1->subStates.size() == root2->subStates.size() )
	{
		SUBREPORT( true );
		checkResult = CompareSubStates( root1->subStates, root2->subStates ) && checkResult;
	}
	else
	{
		SUBREPORT( false );
		checkResult = false;
	}

	return checkResult;
}

bool CStateMachineTest::CompareStateStructs( StateMachineStateNode *stateStruct1, StateMachineStateNode *stateStruct2 )
{
	bool checkResult = true;

	checkResult = checkResult && stateStruct1->name == stateStruct2->name;
	checkResult = checkResult && stateStruct1->numSubStates == stateStruct2->numSubStates;
	checkResult = checkResult && stateStruct1->numEvents == stateStruct2->numEvents;
	checkResult = checkResult && stateStruct1->entryEvent == stateStruct2->entryEvent;
	checkResult = checkResult && stateStruct1->updateEvent == stateStruct2->updateEvent;
	checkResult = checkResult && stateStruct1->exitEvent == stateStruct2->exitEvent;

	assert( stateStruct1->numSubStates == stateStruct1->subStates.size() );
	assert( stateStruct1->numEvents == stateStruct1->events.size() );
	assert( stateStruct2->numSubStates == stateStruct2->subStates.size() );
	assert( stateStruct2->numEvents == stateStruct2->events.size() );

	LogMessage(_T("    State node "));
	LogMessage( stateStruct1->name );
	LogMessage(_T(" compare values: "));
	SUBREPORT( checkResult );

	LogMessage(_T("    State node "));
	LogMessage( stateStruct1->name );
	LogMessage(_T(" read events count: "));
	if( stateStruct1->events.size() == stateStruct2->events.size() )
	{
		SUBREPORT( true );
		checkResult = CompareEvents( stateStruct1->events, stateStruct2->events) && checkResult;
	}
	else
	{
		SUBREPORT( false );
		checkResult = false;
	}

	LogMessage(_T("    State node "));
	LogMessage( stateStruct1->name );
	LogMessage(_T(" read sub states count: "));
	if( stateStruct1->subStates.size() == stateStruct2->subStates.size() )
	{
		SUBREPORT( true );
		checkResult = CompareSubStates( stateStruct1->subStates, stateStruct2->subStates ) && checkResult;
	}
	else
	{
		SUBREPORT( false );
		checkResult = false;
	}

	return checkResult;
}

bool CStateMachineTest::CompareEventStructs( StateMachineEventNode *eventStruct1, StateMachineEventNode *eventStruct2 )
{
	bool checkResult = true;

	checkResult = checkResult && eventStruct1->name == eventStruct2->name;
	checkResult = checkResult && eventStruct1->numActions == eventStruct2->numActions;
	checkResult = checkResult && eventStruct1->numTransitions == eventStruct2->numTransitions;

	assert( eventStruct1->numActions == eventStruct1->actions.size() );
	assert( eventStruct1->numTransitions == eventStruct1->transitions.size() );
	assert( eventStruct2->numActions == eventStruct2->actions.size() );
	assert( eventStruct2->numTransitions == eventStruct2->transitions.size() );

	LogMessage(_T("    Event node "));
	LogMessage( eventStruct1->name );
	LogMessage(_T("  compare values: "));
	SUBREPORT( checkResult );

	LogMessage(_T("    Event node "));
	LogMessage( eventStruct1->name );
	LogMessage(_T(" read action handlers count: "));
	if( eventStruct1->actions.size() == eventStruct1->actions.size() )
	{
		SUBREPORT( true );
		checkResult = CompareActionHandlers( eventStruct1->actions, eventStruct2->actions ) && checkResult;
	}
	else
	{
		SUBREPORT( false );
		checkResult = false;
	}

	LogMessage(_T("    Event node "));
	LogMessage( eventStruct1->name );
	LogMessage(_T(" read transitions count: "));
	if( eventStruct1->transitions.size() == eventStruct1->transitions.size() )
	{
		SUBREPORT( true );
		checkResult = CompareTransitions( eventStruct1->transitions, eventStruct2->transitions ) && checkResult;
	}
	else
	{
		SUBREPORT( false );
		checkResult = false;
	}

	return checkResult;
}

bool CStateMachineTest::CompareActionHandlerStructs( StateMachineActionHandlerNode *actionStruct1, StateMachineActionHandlerNode *actionStruct2 )
{
	bool checkResult = true;

	checkResult = checkResult && actionStruct1->name == actionStruct2->name;
	checkResult = checkResult && actionStruct1->actionName == actionStruct2->actionName;

	LogMessage(_T("    Action handler node "));
	LogMessage( actionStruct1->name );
	LogMessage(_T("  compare values: "));
	SUBREPORT( checkResult );

	return checkResult;
}

bool CStateMachineTest::CompareTransitionStructs( StateMachineTransitionNode *transitionStruct1, StateMachineTransitionNode *transitionStruct2 )
{
	bool checkResult = true;

	checkResult = checkResult && transitionStruct1->name == transitionStruct2->name;
	checkResult = checkResult && transitionStruct1->weight == transitionStruct2->weight;
	checkResult = checkResult && transitionStruct1->targetState == transitionStruct2->targetState;

	LogMessage(_T("    Transition node "));
	LogMessage( transitionStruct1->name );
	LogMessage(_T("  compare values: "));
	SUBREPORT( checkResult );

	return checkResult;
}

bool CStateMachineTest::CompareSubStates( STATESLIST &statesList1, STATESLIST &statesList2 )
{
	bool checkResult = true;
	STATESLIST::iterator statesIterator1 = statesList1.begin();

	for( statesIterator1 = statesList1.begin(); statesIterator1 != statesList1.end(); statesIterator1++ )
	{
		if( statesList2.find( statesIterator1->first ) != statesList2.end() )
		{
			checkResult = CompareStateStructs( statesIterator1->second, statesList2[statesIterator1->first] ) && checkResult;
		}
		else
		{
			checkResult = false;
		}
	}

	return checkResult;
}

bool CStateMachineTest::CompareEvents( EVENTSLIST &eventsList1, EVENTSLIST &eventsList2 )
{
	bool checkResult = true;
	EVENTSLIST::iterator eventsIterator1 = eventsList1.begin();

	for( eventsIterator1 = eventsList1.begin(); eventsIterator1 != eventsList1.end(); eventsIterator1++ )
	{
		if( eventsList2.find( eventsIterator1->first ) != eventsList2.end() )
		{
			checkResult = CompareEventStructs( eventsIterator1->second, eventsList2[eventsIterator1->first] ) && checkResult;
		}
		else
		{
			checkResult = false;
		}
	}

	return checkResult;
}

bool CStateMachineTest::CompareActionHandlers( ACTIONSLIST &actionsList1, ACTIONSLIST &actionsList2 )
{
	bool checkResult = true;
	ACTIONSLIST::iterator actionsIterator1 = actionsList1.begin();

	for( actionsIterator1 = actionsList1.begin(); actionsIterator1 != actionsList1.end(); actionsIterator1++ )
	{
		if( actionsList2.find( actionsIterator1->first ) != actionsList2.end() )
		{
			checkResult = CompareActionHandlerStructs( actionsIterator1->second, actionsList2[actionsIterator1->first] ) && checkResult;
		}
		else
		{
			checkResult = false;
		}
	}

	return checkResult;
}

bool CStateMachineTest::CompareTransitions( TRANSITIONSLIST &transitionsList1, TRANSITIONSLIST &transitionsList2 )
{
	bool checkResult = true;
	TRANSITIONSLIST::iterator transitionsIterator1 = transitionsList1.begin();

	for( transitionsIterator1 = transitionsList1.begin(); transitionsIterator1 != transitionsList1.end(); transitionsIterator1++ )
	{
		if( transitionsList2.find( transitionsIterator1->first ) != transitionsList2.end() )
		{
			checkResult = CompareTransitionStructs( transitionsIterator1->second, transitionsList2[transitionsIterator1->first] ) && checkResult;
		}
		else
		{
			checkResult = false;
		}
	}

	return checkResult;
}

DWORD CStateMachineTest::ReadObjectState( IHashString *objectName )
{
	m_archive->SetIsWriting( true );
	m_archive->SeekTo( 0 );
	DWORD res = SerializeObject( objectName, m_archive, true );
	m_archive->SetIsWriting( false );
	m_archive->SeekTo( 0 );
	return res;
}

DWORD CStateMachineTest::CreateAction( IHashString *parentEvent, IHashString *objectName, LPCTSTR actionName )
{
	DWORD retval = CreateObject( objectName, parentEvent, &m_CQHStateMachineActionHandlerType );
	assert(retval == MSG_HANDLED);

	m_archive->SetIsWriting( true );
	m_archive->SeekTo( 0 );
	m_archive->Write( 1.0f, _T("version") );
	m_archive->Write( actionName, _T("name") );
	m_archive->Write( actionName, _T("actionName") );
	m_archive->SetIsWriting( false );
	m_archive->SeekTo( 0 );

	retval = SerializeObject( objectName, m_archive, false );
	assert(retval == MSG_HANDLED);
	retval = InitializeObject( objectName );
	assert(retval == MSG_HANDLED);

	return retval;
}

bool CStateMachineTest::TestActionsReordering()
{
	bool success = true;

	LoadTestStateMachineFile1();

	GETSETSTATEMACHINENAMESPARAMS gssmn;
	static DWORD msgHash_GetChildStateNames = CHashString(_T("GetChildStateNames")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage( msgHash_GetChildStateNames, sizeof(gssmn), &gssmn, &m_TestStateMachine1, &m_CQHStateMachineType );
	assert(retval == MSG_HANDLED);

	assert(!gssmn.Names.empty());
	IHashString *stateName = gssmn.Names.front();
	gssmn.Names.clear();
	static DWORD msgHash_GetStateEventLabels = CHashString(_T("GetStateEventLabels")).GetUniqueID();
	retval = m_ToolBox->SendMessage( msgHash_GetStateEventLabels, sizeof(gssmn), &gssmn, stateName, &m_CQHStateType );
	assert(retval == MSG_HANDLED);

	assert(!gssmn.Names.empty());
	CHashString hszEventName = gssmn.Names.front();
	static DWORD msgHash_GetEventNameFromLabel = CHashString( _T("GetEventNameFromLabel") ).GetUniqueID();
	retval = m_ToolBox->SendMessage( msgHash_GetEventNameFromLabel, sizeof(IHashString), &hszEventName, stateName, &m_CQHStateType );
	assert(retval == MSG_HANDLED);

	gssmn.Names.clear();
	static DWORD msgHash_GetEventActionHandlers = CHashString( _T("GetEventActionHandlers") ).GetUniqueID();
	retval = m_ToolBox->SendMessage( msgHash_GetEventActionHandlers, sizeof(gssmn), &gssmn, &hszEventName, &m_CQHStateMachineEventType );
	assert(retval == MSG_HANDLED);

	assert(gssmn.Names.size() == 1);
	IHashString *startActionName = gssmn.Names.front();

	CHashString hszAction1Name = _T("Action_1");
	retval = CreateAction( &hszEventName, &hszAction1Name, _T("DoAction1") );
	assert(retval == MSG_HANDLED);
	
	CHashString hszAction2Name = _T("Action_2");
	retval = CreateAction( &hszEventName, &hszAction2Name, _T("DoAction2") );
	assert(retval == MSG_HANDLED);

	gssmn.Names.clear();
	retval = m_ToolBox->SendMessage( msgHash_GetEventActionHandlers, sizeof(gssmn), &gssmn, &hszEventName, &m_CQHStateMachineEventType );
	assert(retval == MSG_HANDLED);

	list<IHashString *>::iterator it = gssmn.Names.begin();
	if( gssmn.Names.size() != 3 ||
		*startActionName != **it ||
		hszAction1Name != **(++it) ||
		hszAction2Name != **(++it) )
	{
		success = false;
		LogMessage( _T("GetEventActionHandlers: Unexpected action order - ") );
		SUBREPORT( false );
	}

	reverse( gssmn.Names.begin(), gssmn.Names.end() );

	vector<IHashString *> names;
	copy( gssmn.Names.begin(), gssmn.Names.end(), back_inserter( names ) );

	static DWORD msgHash_SetEventActionHandlers = CHashString( _T("SetEventActionHandlers") ).GetUniqueID();
	retval = m_ToolBox->SendMessage( msgHash_SetEventActionHandlers, sizeof(gssmn), &gssmn, &hszEventName, &m_CQHStateMachineEventType );
	assert(retval == MSG_HANDLED);

	gssmn.Names.clear();
	retval = m_ToolBox->SendMessage( msgHash_GetEventActionHandlers, sizeof(gssmn), &gssmn, &hszEventName, &m_CQHStateMachineEventType );
	assert(retval == MSG_HANDLED);

	if( gssmn.Names.size() != names.size() || 
		!equal(gssmn.Names.begin(), gssmn.Names.end(), names.begin()) )
	{
		success = false;
		LogMessage( _T("GetEventActionHandlers: Unexpected action order after repordering - ") );
		SUBREPORT( false );
	}

	DeleteObject( &m_TestStateMachine1 );
	return success;
}

bool CStateMachineTest::TestChangeParent()
{
	bool res = true;
	LoadTestStateMachineFile1();

	StateMachineRootNode *initialState = new StateMachineRootNode;
	bool success = FillStateMachineStruct( &m_TestStateMachine1, initialState );
	StateMachineRootNode *expectedState = new StateMachineRootNode;
	FillStateMachineStruct( &m_TestStateMachine1, expectedState );
	CHashString startLabel = _T("Test_Start_State");
	StateMachineStateNode *startState = expectedState->subStates[startLabel.GetUniqueID()];
	assert( startState != NULL );
	IHashString *startName = &startState->objectName;
	CHashString endLabel = _T("Test_End_State");
	StateMachineStateNode *endState = expectedState->subStates[endLabel.GetUniqueID()];
	assert( endState != NULL );
	IHashString *endName = &endState->objectName;

	CHANGEOBJECTPARENT cop;
	cop.objectName = endName;
	cop.newParentName = startName;
	cop.objectCompType = &m_CQHStateType;
	static DWORD msgHash_ChangeObjectParent = CHashString(_T("ChangeObjectParent")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage( msgHash_ChangeObjectParent, sizeof(cop), &cop, endName, &m_CQHStateType );
	assert(retval == MSG_HANDLED);
	
	// compare with expected state machine structure
	expectedState->subStates.erase(endLabel.GetUniqueID());
	--expectedState->numStates;
	startState->subStates[endLabel.GetUniqueID()] = endState;
	++startState->numSubStates;
	LogMessage(_T("Checking expected state machine structure:\n") );
	success = CheckFileStructure( &m_TestStateMachine1, expectedState );
	res = res && success;

	// rolling back object parent
	cop.newParentName = &m_TestStateMachine1;
	retval = m_ToolBox->SendMessage( msgHash_ChangeObjectParent, sizeof(cop), &cop, endName, &m_CQHStateType );
	assert(retval == MSG_HANDLED);
	LogMessage(_T("Checking expected state machine structure:\n") );
	success = CheckFileStructure( &m_TestStateMachine1, initialState );
	res = res && success;

	DeleteObject( &m_TestStateMachine1 );
	return res;
}

////////////////////////////////////////////////////////////////////////////////////