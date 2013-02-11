///============================================================================
/// \file		QHStateObject.cpp
/// \brief		Implentation of CQHStateObject Class
/// \date		09-22-2006
/// \author		Richard Nguyen
/// \note   Elemental Engine
///         Copyright (c)  2005-2008 Signature Devices, Inc.
///
///         This code is redistributable under the terms of the EE License.
///
///         This code is distributed without warranty or implied warranty of
///			merchantability or fitness for a particular purpose. See the 
///			EE License for more details.
///
///         You should have received a copy of the EE License along with this
///			code; If not, write to Signature Devices, Inc.,
///			3200 Bridge Parkway Suite 102, Redwood City, CA 94086 USA.
///============================================================================

#include "stdafx.h"
#include "PerformanceProfiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CQHStateObject);
REGISTER_MESSAGE_HANDLER(TriggerEvent, OnTriggerEvent, CQHStateObject);
REGISTER_MESSAGE_HANDLER(TriggerRecursiveEvent, OnTriggerRecursiveEvent, CQHStateObject);
REGISTER_MESSAGE_HANDLER(GetCurrentState, OnGetCurrentState, CQHStateObject);
REGISTER_MESSAGE_HANDLER(QueryStateMachineEvents, OnQueryStateMachineEvents, CQHStateObject);
REGISTER_MESSAGE_HANDLER(ExecuteCommand, OnExecuteCommand, CQHStateObject);
REGISTER_MESSAGE_HANDLER( GetSchema, OnGetSchema, CQHStateObject );

// Default Constructor
CQHStateObject::CQHStateObject( IHashString *parentName, IHashString *name, bool bAddToHier ) :
	OBJECTTEMPLATE( CQHStateMachineManager, CQHStateObject, IObject, parentName, name )
{
	m_StateMachine = NULL;
	m_CurrentState = NULL;
    m_ToolBox = EngineGetToolBox();

	m_bAddToHierarchy = bAddToHier;
	if( m_bAddToHierarchy )
	{
		AddToHierarchy();
	}
}


// Default Destructor
CQHStateObject::~CQHStateObject()
{
#if defined _PROFILE || defined _DEBUG
	StdString objectDescription = GetName()->GetString();
	PERFORMANCE_PROFILER_TYPE_START( objectDescription, _T("State Updates") );
	PERFORMANCE_PROFILER_TYPE_STOP( objectDescription, _T("State Updates") );
#endif // _PROFILE
	CQHStateMachineManager *amanager = GetStateMachineManager();
	amanager->RemoveStateObjectFromUpdateQueue( this );

	if( m_bAddToHierarchy )
	{
        RemoveFromHierarchy();
	}
}

IComponent *CQHStateObject::Create( int nArgs, va_list argptr )
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new CQHStateObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CQHStateObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CQHStateObject::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		StdString astateNameString;
		StdString szTempFileName, szTempStateMachine;
		ar.Read( szTempFileName, _T("FileName") );
		ar.Read( szTempStateMachine, _T("StateMachine") );
		ar.Read( astateNameString, _T("StartState") );
		ar.Read( m_iUpdatePriority, _T("UpdatePriority") );
		m_szFileName.Init( szTempFileName );
		m_szStateMachine.Init( szTempStateMachine );
		CHashString hszStateNameString(astateNameString);

		CQHStateMachineManager *amanager = GetStateMachineManager();

		m_StateMachine = amanager->GetStateMachineByFile( &m_szFileName );
		if( m_StateMachine == NULL )
		{
			amanager->AddStateMachine( &m_szFileName );
			m_StateMachine = amanager->GetStateMachineByFile( &m_szFileName );
		}

		if( m_StateMachine == NULL )
		{
			m_ToolBox->Log( LOGERROR, _T( "Specified state machine not found: %s\n" ), m_szStateMachine );
		}
		else
		{
			if( astateNameString == "" )
			{
				m_CurrentState = m_StateMachine->GetStartState();
			}
			else
			{
				m_CurrentState = m_StateMachine->GetGlobalStateByLabel( &hszStateNameString );
				if( m_CurrentState == NULL )
				{
					IHashString *astateMachineName = m_StateMachine->GetName();
					if (astateMachineName)
					{
						m_ToolBox->Log( LOGERROR, _T( "%s could not find starting state: %s\n" ), astateMachineName->GetString(), hszStateNameString.GetString() );
						DELETEOBJECTPARAMS dop;
						dop.name = astateMachineName;
						static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
						DWORD result = 0;
						result = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof( DELETEOBJECTPARAMS ), &dop, NULL, NULL );
						if (result != MSG_HANDLED)
							m_ToolBox->Log( LOGERROR, _T("%s(%i): Delete Invalid State Machine: %s\n" ), astateMachineName->GetString(), hszStateNameString.GetString() );
						assert(0); // I WANT PEOPLE TO SEE THIS!
						return;
					}
				}
			}
			TriggerEntryEvent( NULL );
			amanager->AddStateObjectToUpdateQueue( this );
		}
	}
	else
	{
		StdString astateNameString = _T("");
	
		CQHState *astate = m_StateMachine->GetStartState();
		if (astate == NULL)
		{
			m_ToolBox->Log(LOGERROR, _T("QHStateObject: no starting state in serialize!\n"));
		}
		else
		{
			astateNameString = astate->GetStateName()->GetString();
		}
		//ar.Write( m_szName, _T("Name") );
		ar.Write( m_szFileName.GetString(), _T("FileName") );
		ar.Write( m_szStateMachine.GetString(), _T("StateMachine") );
		ar.Write( astateNameString, _T("StartState") );
		ar.Write( m_iUpdatePriority, _T("UpdatePriority") );
	}
}

void CQHStateObject::Init()
{
	// TODO: rnguyen
}

bool CQHStateObject::Update()
{
	// TODO <rnguyen>: Might get iffy here. Consider locking this during state transitions.
	IHashString *aupdateEventName = m_CurrentState->GetUpdateEventName();
	if( aupdateEventName != NULL )
	{
		TriggerEvent( aupdateEventName, NULL );
	}

	return true; // TODO <rnguyen>: Would we need to return false for any reason?
}

void CQHStateObject::DeInit()
{
	// TODO: rnguyen
}

IHashString *CQHStateObject::GetCurrentStateName()
{
	return m_CurrentState->GetStateName();
}

CQHStateMachineManager *CQHStateObject::GetStateMachineManager()
{
	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CQHStateMachineManager *amanager = static_cast<CQHStateMachineManager*>( amanagerComponent );

	return amanager;
}

DWORD CQHStateObject::TriggerEvent( IHashString *aeventName, IArchive *aarchive )
{
	DWORD areturn = MSG_NOT_HANDLED;

	if( aeventName == NULL || *aeventName->GetString() == '\0' )
	{
		areturn = MSG_HANDLED_STOP;
	}
	else if( m_CurrentState != NULL )
	{
		CQHStateMachineEvent *aevent = m_CurrentState->GetEvent( aeventName );
		if( aevent != NULL )
		{
			// Event was found within the state, so we execute its action handlers
			// and perform the state transition.

			aevent->ExecuteActionHandlers( aarchive, GetParentName());
			HandleTransition( aevent );
			areturn = MSG_HANDLED_STOP;
		}
		else
		{
			// Event was not found in the current state, so we search the
			// entire state machine. If it exists, we add the event to
			// an event queue.

			EVENTQUEUEOBJECT aeventQueueObject;
			aeventQueueObject.EventName = aeventName->GetString();
			aeventQueueObject.ArchiveParams = aarchive;
			if( m_StateMachine->EventExists( aeventName ) )
			{
				m_ToolBox->Log(
					LOGINFORMATION, _T("%s %s: Event '%s' not found in state '%s'\n"),
					m_StateMachine->GetName()->GetString(),
					GetName()->GetString(),
					aeventQueueObject.EventName.GetString(),
					m_CurrentState->GetStateName()->GetString() );

				m_EventQueue.push_back( aeventQueueObject );
				areturn = MSG_HANDLED_STOP;
			}
		}
	}

	return areturn;
}

DWORD CQHStateObject::OnTriggerEvent(DWORD size, void* aeventParams)
{
	VERIFY_MESSAGE_SIZE(sizeof(TRIGGEREVENTPARAMS), size);

	TRIGGEREVENTPARAMS* TriggerParams = static_cast<TRIGGEREVENTPARAMS*>( aeventParams );

	DWORD areturn = TriggerEvent( TriggerParams->EventName, TriggerParams->EventParamsArchive );

	return areturn;
}

DWORD CQHStateObject::OnTriggerRecursiveEvent(DWORD size, void* aeventParams)
{
	VERIFY_MESSAGE_SIZE(sizeof(TRIGGEREVENTPARAMS), size);

	TRIGGEREVENTPARAMS* TriggerParams = static_cast<TRIGGEREVENTPARAMS*>( aeventParams );

	DWORD areturn = TriggerEvent( TriggerParams->EventName, TriggerParams->EventParamsArchive );

	if( areturn == MSG_HANDLED_STOP )
	{
		areturn = MSG_HANDLED_PROCEED;
	}

	return areturn;
}

DWORD CQHStateObject::OnGetCurrentState( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(sizeof(CHashString), size);

	if( m_CurrentState != NULL )
	{
		CHashString *hszCurrentState = (CHashString*)params;
		if( hszCurrentState != NULL )
		{
			hszCurrentState->Init( m_CurrentState->GetStateName()->GetString() );
		}
	}

	return MSG_HANDLED_STOP;
}

DWORD CQHStateObject::OnExecuteCommand( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(sizeof(IHashString), size);

	IHashString *hszCommandObjectName = (IHashString*)params;
	CHashString hszCommandString( _T("") );
	CHashString msgGetCommandString( _T("GetCommandString") );
	m_ToolBox->SendMessage( msgGetCommandString.GetUniqueID(), sizeof( IHashString ), &hszCommandString, hszCommandObjectName );

	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	CHashString memType(_T("Memory"));
	ca.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();

	DWORD retVal = MSG_NOT_HANDLED;
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) == MSG_HANDLED)
	{
		ca.archive->SetIsWriting( true );
		ca.archive->SeekTo( 0 );
		ca.archive->Write( hszCommandObjectName->GetString() );
		ca.archive->SetIsWriting( false );

		//m_ToolBox->Log(
		//			LOGINFORMATION, _T("%s executing command '%s' at state '%s'\n"),
		//			GetName()->GetString(),
		//			hszCommandString.GetString(),
		//			m_CurrentState->GetStateName()->GetString() );
		retVal = TriggerEvent( &hszCommandString, ca.archive );

		ca.archive->Close();
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Unable to create archive for executing command %s\n"), hszCommandString );
	}

	return retVal;
}

DWORD CQHStateObject::OnGetSchema( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETSCHEMAPARAMS));
	GETSCHEMAPARAMS *getSchemaParams = (GETSCHEMAPARAMS*)params;
	IXMLArchive *archive = getSchemaParams->ar;
	if( archive != NULL )
	{
		archive->StartClass( GetComponentType()->GetString() );
		archive->Write( m_Name.GetString(), _T("Name") );
		archive->Write( 4 );

		archive->StartClass(_T("edit") );
		archive->Write( _T("string"), _T("Type") );
		archive->Write( _T("File"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("edit") );
		archive->Write( _T("string"), _T("Type") );
		archive->Write( _T("State Machine"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("edit") );
		archive->Write( _T("string"), _T("Type") );
		archive->Write( _T("Start State"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("edit") );
		archive->Write( _T("int"), _T("Type") );
		archive->Write( _T("Update Priority"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->EndClass();
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Attempt to get CQHStateObject schema. No archive specified.\n" ) );
	}

	return MSG_HANDLED_STOP;
}

bool CQHStateObject::HasExpired()
{
	bool areturn = false;
	CQHState *aendingState = m_StateMachine->GetEndState();
	if( aendingState == m_CurrentState )
	{
		areturn = true;
	}

	return areturn;
}

void CQHStateObject::HandleTransition( CQHStateMachineEvent *aevent )
{
	if( aevent->HasTransitions() )
	{
		bool asuccessfulTransition = TransitionState( aevent );

		if( asuccessfulTransition )
		{
			IterateEventQueue();
		}
		else
		{
			const TCHAR *stateName = aevent->GetTransition()->GetTargetState()->GetString();
			m_ToolBox->SetErrorValue( WARN_OBJECT_NOT_FOUND );
			m_ToolBox->Log( LOGWARNING, _T( "CQHStateObject: Could not transition to state: %s\n" ), stateName );
		}
	}
}

bool CQHStateObject::TransitionState( CQHStateMachineEvent *aevent )
{
	bool areturn = false;
	CQHStateMachineTransition *atransition = aevent->GetTransition();
	if( atransition != NULL )
	{
		IHashString *atargetStateName = atransition->GetTargetState();
		CQHState *astate = m_StateMachine->GetGlobalStateByLabel( atargetStateName );
		if( astate != NULL )
		{
			IHashString *acommonParent = GetCommonParent( astate );

			// Exit event before current state is changed.
			TriggerExitEvent( acommonParent );

			m_CurrentState = astate;

			// Entry event after current state is changed.
			TriggerEntryEvent( acommonParent );

			areturn = true;
		}
	}
	return areturn;
}

void CQHStateObject::IterateEventQueue()
{
	EVENTQUEUEOBJECTLIST::iterator aeventQueueIterator = m_EventQueue.begin();
	EVENTQUEUEOBJECT *aiteratedObject;
	while (aeventQueueIterator != m_EventQueue.end())
	{
		aiteratedObject = &(*aeventQueueIterator);
		if( m_CurrentState->EventExists( &aiteratedObject->EventName ) )
		{
			IHashString *aeventName = &aiteratedObject->EventName;
			IArchive *aarchive = aiteratedObject->ArchiveParams;
			aeventQueueIterator = m_EventQueue.erase( aeventQueueIterator );

			TriggerEvent( aeventName, aarchive );

			delete aarchive;
		}
		else
		{
			aeventQueueIterator++;
		}
	}
}

void CQHStateObject::TriggerEntryEvent( IHashString *acommonParent )
{
	TriggerEntryEvent( m_CurrentState, acommonParent );
}

void CQHStateObject::TriggerEntryEvent( CQHState *astate, IHashString *acommonParent )
{
	// The state cascades through parent entry events before executing its own.
	// If parent state is null, then we are at the top level of the state.
	CQHState *aparentState = astate->GetParentState();
	if( aparentState != NULL )
	{
		if( acommonParent == NULL || *aparentState->GetStateName() != *acommonParent )
		{
			TriggerEntryEvent( aparentState, acommonParent );
		}
	}

	TriggerEvent( astate->GetEntryEventName(), NULL );
}

void CQHStateObject::TriggerExitEvent( IHashString *acommonParent )
{
	TriggerExitEvent( m_CurrentState, acommonParent );
}

void CQHStateObject::TriggerExitEvent( CQHState *astate, IHashString *acommonParent )
{
	if( acommonParent == NULL || *astate->GetStateName() != *acommonParent )
	{
		TriggerEvent( astate->GetExitEventName(), NULL );

		// The state step through each parent exit events after executing its own.
		// If parent state is null, then we are at the top level of the state.
		CQHState *aparentState = astate->GetParentState();
		if( aparentState != NULL )
		{
			TriggerExitEvent( aparentState, acommonParent );
		}
	}
}

IHashString *CQHStateObject::GetCommonParent( CQHState *astate )
{
	CQHState *acurrentParentIterator = m_CurrentState->GetParentState();
	CQHState *atargetParentIterator = astate->GetParentState();
	IHashString *acommonParentName = NULL;

	if( m_CurrentState->IsBaseParent( astate ) )
	{
		acurrentParentIterator = astate;
	}
	else if( astate->IsBaseParent( m_CurrentState ) )
	{
		acurrentParentIterator = m_CurrentState;
	}
	// If a state's parent is NULL, then it is a base state. No common parent.
	else if( acurrentParentIterator != NULL && atargetParentIterator != NULL )
	{
		// Iterate through the current state's parents and compare each with
		// all the target state's parents.
		while( acurrentParentIterator != NULL && acurrentParentIterator != atargetParentIterator )
		{
			atargetParentIterator = atargetParentIterator->GetParentState();
			while( atargetParentIterator != NULL && acurrentParentIterator != atargetParentIterator )
			{
				atargetParentIterator = atargetParentIterator->GetParentState();
			}
			if( atargetParentIterator == NULL )
			{
				acurrentParentIterator = acurrentParentIterator->GetParentState();
				atargetParentIterator = astate->GetParentState();
			}
		}
	}

	if( acurrentParentIterator != NULL )
	{
		acommonParentName = acurrentParentIterator->GetStateName();
	}

	return acommonParentName;
}

DWORD CQHStateObject::OnQueryStateMachineEvents( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(sizeof(QUERYEVENTSPARAMS), size);
	QUERYEVENTSPARAMS *queryEventsParams = (QUERYEVENTSPARAMS*)params;
	assert( m_StateMachine != NULL );

	queryEventsParams->StateMachineName = m_StateMachine->GetName();
	m_StateMachine->QueryEvents( &queryEventsParams->QueryEventsList );

	return MSG_HANDLED_STOP;
}
int CQHStateObject::GetUpdatePriority()
{
	return m_iUpdatePriority;
}