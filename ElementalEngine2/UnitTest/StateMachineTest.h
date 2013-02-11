#ifndef STATEMACHINETEST_H
#define STATEMACHINETEST_H

struct StateMachineStateNode;
struct StateMachineEventNode;
struct StateMachineActionHandlerNode;
struct StateMachineTransitionNode;
typedef std::map<DWORD, StateMachineStateNode*> STATESLIST;
typedef std::map<DWORD, StateMachineEventNode*> EVENTSLIST;
typedef std::map<DWORD, StateMachineActionHandlerNode*> ACTIONSLIST;
typedef std::map<DWORD, StateMachineTransitionNode*> TRANSITIONSLIST;

template<typename T>
void DeleteMap(map<DWORD, T*> &items)
{
	for( map<DWORD, T*>::iterator it = items.begin(); it != items.end(); ++it )
	{
		delete it->second;
	}
	items.clear();
}

struct StateMachineTransitionNode
{
	CHashString objectName;
	StdString name;
	int weight;
	StdString targetState;
};

struct StateMachineActionHandlerNode
{
	CHashString objectName;
	StdString name;
	StdString actionName;
};

struct StateMachineEventNode
{
	~StateMachineEventNode()
	{
		DeleteMap(actions);
		DeleteMap(transitions);
	}
	CHashString objectName;
	StdString name;
	int numActions;
	int numTransitions;
	ACTIONSLIST actions;
	TRANSITIONSLIST transitions;
};

struct StateMachineStateNode
{
	~StateMachineStateNode()
	{
		DeleteMap(events);
		DeleteMap(subStates);
	}

	CHashString objectName;
	StdString name;
	int numSubStates;
	int numEvents;
	StdString entryEvent;
	StdString updateEvent;
	StdString exitEvent;
	EVENTSLIST events;
	STATESLIST subStates;
};

struct StateMachineRootNode
{
	~StateMachineRootNode()
	{
		DeleteMap(subStates);
	}

	StdString name;
	int numStates;
	StdString startState;
	StdString endState;
	StdString parentStateMachinefile;
	StdString parentStateMachine;
	STATESLIST subStates;
};

class CStateMachineTest : public CTestBase
{
public:
	CStateMachineTest( CRichEditCtrl *dialogEditControl );
	~CStateMachineTest();
	void Initialize();
	void DeInitialize();

	bool TestQueryStateMachineEvents1();

	bool TestLoadStateMachine1();
	bool TestCreateStateObject1();
	bool TestStateMachineEvents1();
	bool TestDerivedStateMachine();
	bool TestDerivedStateMachineEvents();

	bool TestActionsReordering();
	bool TestChangeParent();

private:
	void CreateStateObject( IHashString *stateObjectName, IHashString *stateMachineFile, IHashString *stateMachineName, int updatePriority );

	bool CheckFileStructure( IHashString *stateMachineName, StateMachineRootNode *expectedRoot );
	void GenerateExpectedFile1Structure( StateMachineRootNode *expectedRoot );
	void GenerateExpectedFile3Structure( StateMachineRootNode *expectedRoot );
	void GenerateExpectedFile4Structure( StateMachineRootNode *expectedRoot );

	void LoadTestStateMachineFile1();
	void LoadTestStateMachineFile3();
	void LoadTestStateMachineFile4();

	bool FillStateMachineStruct( IHashString *objectName, StateMachineRootNode *rootStruct );
	bool FillStateStruct( IHashString *objectName, StateMachineStateNode *stateStruct );
	bool FillEventStruct( IHashString *objectName, StateMachineEventNode *eventStruct );
	bool FillActionHandlerStruct( IHashString *objectName, StateMachineActionHandlerNode *actionStruct );
	bool FillTransitionStruct( IHashString *objectName, StateMachineTransitionNode *transitionStruct );

	bool CompareRootStructs( StateMachineRootNode *root1, StateMachineRootNode *root2 );
	bool CompareStateStructs( StateMachineStateNode *stateStruct1, StateMachineStateNode *stateStruct2 );
	bool CompareEventStructs( StateMachineEventNode *eventStruct1, StateMachineEventNode *eventStruct2 );
	bool CompareActionHandlerStructs( StateMachineActionHandlerNode *actionStruct1, StateMachineActionHandlerNode *actionStruct2 );
	bool CompareTransitionStructs( StateMachineTransitionNode *transitionStruct1, StateMachineTransitionNode *transitionStruct2 );

	bool CompareSubStates( STATESLIST &statesList1, STATESLIST &statesList2 );
	bool CompareEvents( EVENTSLIST &eventsList1, EVENTSLIST &eventsList2 );
	bool CompareActionHandlers( ACTIONSLIST &actionsList1, ACTIONSLIST &actionsList2 );
	bool CompareTransitions( TRANSITIONSLIST &transitionsList1, TRANSITIONSLIST &transitionsList2 );

	DWORD ReadObjectState( IHashString *objectName );

	DWORD CreateAction( IHashString *parentEvent, IHashString *objectName, LPCTSTR actionName );
private:
	CHashString m_TestStateObject1;
	CHashString m_TestStateObject3;
	CHashString m_StateObjectComponentType;
	CHashString m_TestStateMachineFile1;
	CHashString m_TestStateMachineFile2;
	CHashString m_TestStateMachineFile3;
	CHashString m_TestStateMachineFile4;
	CHashString m_TestStateMachine1;
	CHashString m_TestStateMachine3;
	CHashString m_TestStateMachine4;

	CHashString m_CQHStateMachineType;
	CHashString m_CQHStateType;
	CHashString m_CQHStateMachineEventType;
	CHashString m_CQHStateMachineTransitionType;
	CHashString m_CQHStateMachineActionHandlerType;

	IArchive *m_archive;

};

#endif //#ifndef STATEMACHINETEST_H