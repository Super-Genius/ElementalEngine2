///============================================================================
/// \file		QHStateMachineManager.cpp
/// \brief		Implmentation of CQHStateMachineManager Class
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

TOLUA_API int tolua_QHStateMachineScript_open (lua_State* tolua_S);

REGISTER_COMPONENT_SINGLETON(CQHStateMachineManager);
REGISTER_MESSAGE_HANDLER(Start, OnStart, CQHStateMachineManager);
REGISTER_MESSAGE_HANDLER(Stop, OnStop, CQHStateMachineManager);
REGISTER_MESSAGE_HANDLER(ChangeObjectName, OnChangeObjectName, CQHStateMachineManager);
REGISTER_MESSAGE_HANDLER(LoadAndInitStateObject, OnLoadAndInitStateObject, CQHStateMachineManager);
REGISTER_MESSAGE_HANDLER(TriggerEventInStateMachine, OnTriggerEventInStateMachine, CQHStateMachineManager);
REGISTER_MESSAGE_HANDLER(QueryStateMachineEvents, OnQueryStateMachineEvents, CQHStateMachineManager);
REGISTER_MESSAGE_HANDLER(GetStateMachineName, OnGetStateMachineName, CQHStateMachineManager);


// Default Constructor
CQHStateMachineManager::CQHStateMachineManager( void ) : 
	CManager(_T("CQHStateMachineManager"), STATEMACHINEPRIORITY, STATEMACHINEPRIORITY),
	m_stateObjectComponentType(_T("CQHStateObject")),
	m_stateMachineComponentType(_T("CQHStateMachine")),
	m_derivedStateComponentType(_T("CQHDerivedState")),
	m_stateComponentType(_T("CQHState")),
	m_eventComponentType(_T("CQHStateMachineEvent")),
	m_actionHandlerComponentType(_T("CQHStateMachineActionHandler")),
	m_transitionComponentType(_T("CQHStateMachineTransition")),
	m_ManagerComponentType( _T("CQHStateMachineManager") )
{
	m_bIsRunning = true;
	m_LuaState = NULL;
	m_ToolBox = EngineGetToolBox();

	RegisterLuaDefinitions();
}

// Default Destructor
CQHStateMachineManager::~CQHStateMachineManager( void )
{
	DeInit();
}

void CQHStateMachineManager::RegisterLuaDefinitions()
{
	static DWORD msgHash_GetMasterScriptState = CHashString(_T( "GetMasterScriptState" )).GetUniqueID();
	DWORD amessageHandle = m_ToolBox->SendMessage(msgHash_GetMasterScriptState, sizeof( lua_State * ), &m_LuaState );
	if( amessageHandle != MSG_HANDLED)
	{
		StdString error( "Error missing Master Script State Data\n" );

		// log error
		m_ToolBox->SetErrorValue(WARN_INVALID_OPERATION);
		m_ToolBox->Log(LOGWARNING, error);
    }
	else
	{
		// register our additional structures/handlers with LUA master
		tolua_QHStateMachineScript_open(m_LuaState);
	}
}

IComponent *CQHStateMachineManager::Create(int nArgs, va_list argptr)
{
//	// Return the singleton instance of this class
  	return SINGLETONINSTANCE( CQHStateMachineManager );
}

bool CQHStateMachineManager::Init()
{
	return true;
}

bool CQHStateMachineManager::DeInit()
{
	DELETEOBJECTPARAMS dop;

	// Call DeleteObject for each state machine
	IDTOOBJECTMAP *stateMachines = GetObjectMap( &m_stateMachineComponentType );
	if( stateMachines != NULL )
	{
		IDTOOBJECTMAP::iterator it = stateMachines->begin();
		while( it != stateMachines->end() )
		{
			dop.name = it->second->GetName();
			static DWORD msgHash_DeleteObject = CHashString( _T("DeleteObject") ).GetUniqueID();
			m_ToolBox->SendMessage( msgHash_DeleteObject, sizeof( dop ), &dop );
			it = stateMachines->begin();
		}

		assert( stateMachines->empty() );
	}

	return CManager::DeInit();
}


void CQHStateMachineManager::Update(DWORD tickCount)
{
	if (m_bIsRunning)
	{
//		PERFORMANCE_PROFILER_AUTO(QHStateMachineManager_update);
		SOUPDATEMAP::iterator astateObjectIterator = m_StateObjectUpdateMap.begin();
		CQHStateObject *aiteratedStateObject = NULL;
		std::list<DWORD> *stateObjectDeleteQueue = NULL;

		for(astateObjectIterator = m_StateObjectUpdateMap.begin();
				astateObjectIterator != m_StateObjectUpdateMap.end();
				astateObjectIterator++ )
		{
			aiteratedStateObject = astateObjectIterator->second;
			if( !aiteratedStateObject->HasExpired() )
			{
#if defined _PROFILE || defined _DEBUG
				StdString objectDescription = aiteratedStateObject->GetName()->GetString();
				PERFORMANCE_PROFILER_TYPE_START( objectDescription, _T("State Updates") );
#endif // _PROFILE

				aiteratedStateObject->Update();

#if defined _PROFILE || defined _DEBUG
				PERFORMANCE_PROFILER_TYPE_STOP( objectDescription, _T("State Updates") );
#endif // _PROFILE
			}
			else
			{
				if( stateObjectDeleteQueue == NULL )
				{
					stateObjectDeleteQueue = new std::list<DWORD>;
				}
				stateObjectDeleteQueue->push_back( aiteratedStateObject->GetName()->GetUniqueID() );
			}
		}

		if( stateObjectDeleteQueue != NULL )
		{
			std::list<DWORD>::iterator stateObjectDeleteIterator = stateObjectDeleteQueue->begin();
			CHashString stateObjectToDelete;
			for( ; stateObjectDeleteIterator != stateObjectDeleteQueue->end(); stateObjectDeleteIterator++ )
			{
				stateObjectToDelete = m_ToolBox->GetHashString( *stateObjectDeleteIterator );

				DELETEOBJECTPARAMS dop;
				dop.name = &stateObjectToDelete;
				static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof( DELETEOBJECTPARAMS ), &dop, NULL, NULL );
			}
			delete stateObjectDeleteQueue;
			stateObjectDeleteQueue = NULL;
		}
	}
}

void CQHStateMachineManager::Serialize( IArchive &ar )
{
	IDTOOBJECTMAP *stateMachines = GetObjectMap( &m_stateMachineComponentType );
	IDTOOBJECTMAP::iterator objectIterator = stateMachines->begin();
	CQHStateMachine *iteratedStateMachine;
	for( ; objectIterator != stateMachines->end(); objectIterator++ )
	{
		iteratedStateMachine = dynamic_cast<CQHStateMachine *>(objectIterator->second);
		if (iteratedStateMachine != NULL)
		{
			iteratedStateMachine->Serialize( ar );
		}
	}
}

bool CQHStateMachineManager::AddStateMachine( IHashString *afileName )
{
	bool areturn = false;
	if( afileName != NULL && 
		afileName->GetString() != NULL &&
		!afileName->IsEmpty() )
	{
		StdString file = afileName->GetString();
		file.MakeSafeFileName();
		LOADFILEEXTPARAMS lfep;
		lfep.fileName = (TCHAR*)file.c_str();
		lfep.bInternalLoad = true;

		static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		DWORD retVal = m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
	}

	return areturn;
}

CQHStateMachineEvent *CQHStateMachineManager::GetEvent( IHashString *aeventName )
{
	IDTOOBJECTMAP *objectMap = GetObjectMap( &m_eventComponentType );
	IObject *aobject = GetMapObject( aeventName, objectMap );
	CQHStateMachineEvent *areturn = dynamic_cast<CQHStateMachineEvent*>( aobject );

	return areturn;
}

CQHState *CQHStateMachineManager::GetState( IHashString *astateName )
{
	IDTOOBJECTMAP *objectMap = GetObjectMap( &m_stateComponentType );
	IObject *aobject = GetMapObject( astateName, objectMap );
	CQHState *areturn = dynamic_cast<CQHState*>( aobject );

	return areturn;
}

CQHStateMachine *CQHStateMachineManager::GetStateMachine( IHashString *astateMachineName )
{
	IDTOOBJECTMAP *objectMap = GetObjectMap( &m_stateMachineComponentType);
	IObject *aobject = GetMapObject( astateMachineName, objectMap );
	CQHStateMachine *areturn = dynamic_cast<CQHStateMachine*>( aobject );

	return areturn;
}

CQHStateObject *CQHStateMachineManager::GetStateObject( IHashString *astateObjectName )
{
	IDTOOBJECTMAP *objectMap = GetObjectMap( &m_stateObjectComponentType );
	IObject *aobject = GetMapObject( astateObjectName, objectMap );
	CQHStateObject *areturn = dynamic_cast<CQHStateObject*>( aobject );

	return areturn;
}

IObject *CQHStateMachineManager::GetMapObject( IHashString *aObjectName, IDTOOBJECTMAP *aObjectMap )
{
	IObject *areturn = NULL;

	if( aObjectName != NULL && aObjectMap != NULL )
	{
		IDTOOBJECTMAP::iterator aObjectsIterator = aObjectMap->find(aObjectName->GetUniqueID());
		if (aObjectsIterator != aObjectMap->end())
		{
			areturn = aObjectsIterator->second;
		}
	}

	return areturn;
}

void CQHStateMachineManager::ResetParentInObjectSet( IHashString *componentType, IHashString *oldName, IHashString *newName )
{
	IDTOOBJECTMAP *objectMap = GetObjectMap( componentType );

	if( objectMap != NULL )
	{
		IDTOOBJECTMAP::iterator aiterator = objectMap->find(oldName->GetUniqueID());
		if (aiterator != objectMap->end())
		{
			IObject *theObject = aiterator->second;
			theObject->SetParentName( newName );
		}
	}
}

DWORD CQHStateMachineManager::OnStart( DWORD size, void *params )
{
	m_bIsRunning = true;
	return MSG_HANDLED_PROCEED;
}

DWORD CQHStateMachineManager::OnStop( DWORD size, void *params )
{
	m_bIsRunning = false;
	return MSG_HANDLED_PROCEED;
}

DWORD CQHStateMachineManager::OnChangeObjectName( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(CHANGEOBJNAME) );

	CHANGEOBJNAME *data = (CHANGEOBJNAME*)params;
	IHashString *oldName = data->oldName;
	IHashString *newName = data->newName;
	IHashString *compType = data->compType;

	ResetParentInObjectSet( &m_stateComponentType, oldName, newName );
	ResetParentInObjectSet( &m_stateMachineComponentType, oldName, newName );
	ResetParentInObjectSet( &m_actionHandlerComponentType, oldName, newName );
	ResetParentInObjectSet( &m_transitionComponentType, oldName, newName );

	return MSG_HANDLED_PROCEED;
}

DWORD CQHStateMachineManager::OnLoadAndInitStateObject( DWORD size, void* aloadParams )
{
	StdString error;
	VERIFY_MESSAGE_SIZE(sizeof(LOADANDINITSTATEOBJECTPARAMS), size);
	DWORD areturn = MSG_NOT_HANDLED;

	LOADANDINITSTATEOBJECTPARAMS *SOParams = (LOADANDINITSTATEOBJECTPARAMS *)aloadParams;

	if( SOParams->StateMachineFileName == NULL ||
		SOParams->StateMachineName == NULL ||
		SOParams->StateObjectName == NULL )
	{
		areturn = MSG_ERROR;
		m_ToolBox->Log( LOGWARNING, _T("%s OnLoadAndInitStateObject(): null parameters from message\n"),
			__FILE__ );
		return areturn;
	}

	IHashString* Name = SOParams->StateObjectName;
	IHashString* parentName = SOParams->StateObjectParentName;

	CQHStateObject *pQHStateObject = NULL;
	pQHStateObject = dynamic_cast<CQHStateObject *>(m_ToolBox->CreateComponent(&m_stateObjectComponentType, 3, Name, parentName, SOParams->bIsInHierarchy));
	if( pQHStateObject == NULL )
	{
		areturn = MSG_ERROR;
		m_ToolBox->Log( LOGWARNING, _T("%s OnLoadAndInitStateObject(): cannot create object\n"), __FILE__);
		return areturn;
	}

	// add object to object factory
	OFACTORYADDPARAMS ofap;
	ofap.name = Name;
	ofap.component = pQHStateObject;
	static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
	areturn = m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(OFACTORYADDPARAMS), &ofap);
	if( areturn != MSG_HANDLED )
	{
		StdString error( __FILE__ " OnLoadAndInitStateObject(): cannot add object to factory\n" );
		m_ToolBox->Log( LOGWARNING, error );
		return areturn;
	}
	else
	{
		CREATEARCHIVE ca;
		CHashString memType(_T("Memory"));
		
		ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
		ca.streamData = NULL;
		ca.streamSize = 0;
		ca.streamType = &memType;

		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		areturn = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
		if (areturn != MSG_HANDLED)
		{
			StdString error( __FILE__ " OnLoadAndInitStateObject(): cannot create archive\n" );
			m_ToolBox->Log( LOGWARNING, error );
			return areturn;
		}

		IArchive *ar = ca.archive;

		// We checked earlier if these were NULL        
		ar->Write( SOParams->StateMachineFileName->GetString() );
		ar->Write( SOParams->StateMachineName->GetString() );
		if( SOParams->StateMachineStartState != NULL )
		{
			ar->Write( SOParams->StateMachineStartState->GetString() );
		}
		else
		{
			ar->Write( _T("") );
		}
		ar->Write( SOParams->iUpdatePriority );
        
		SERIALIZEOBJECTPARAMS sop;
		sop.name = Name;
		sop.archive = ar;

		// serialize object
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		areturn = m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);
		ar->Close();

		if( areturn != MSG_HANDLED )
		{
			StdString error( __FILE__ " OnLoadAndInitStateObject(): cannot serialize object\n" );
			m_ToolBox->Log( LOGWARNING, error );
			return areturn;
		}
		else
		{
			// initialize object
			INITOBJECTPARAMS iop;
			iop.name = Name;
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			areturn = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
			if( areturn != MSG_HANDLED )
			{
				StdString error( __FILE__ _T(" OnLoadAndInitStateObject(): cannot init object\n") );
				m_ToolBox->Log( LOGWARNING, error );
				return areturn;
			}
		}
	}

	return areturn;
}

DWORD CQHStateMachineManager::OnTriggerEventInStateMachine(DWORD size, void* aeventParams)
{
	VERIFY_MESSAGE_SIZE(sizeof(TRIGGEREVENTINMGRPARAMS), size);

	TRIGGEREVENTINMGRPARAMS *atriggerEventParams = static_cast<TRIGGEREVENTINMGRPARAMS *>( aeventParams );
	CQHStateObject *astateObject = GetStateObject( atriggerEventParams->StateObjectName );
	IArchive *aparamsArchive = atriggerEventParams->EventParamsArchive;

	DWORD areturn;
	if ( astateObject != NULL )
	{
		areturn = astateObject->TriggerEvent( atriggerEventParams->EventName, aparamsArchive );
	}
	else
	{
		areturn = MSG_NOT_HANDLED;
	}

	return areturn ;
}

DWORD CQHStateMachineManager::OnQueryStateMachineEvents( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE(sizeof(QUERYEVENTSPARAMS), size);
	QUERYEVENTSPARAMS *queryEventsParams = (QUERYEVENTSPARAMS*)params;

	if( queryEventsParams->StateMachineName == NULL )
	{
		m_ToolBox->Log( LOGWARNING, _T("Cannot query events. No state machine specified.\n") );
	}
	else
	{

		CQHStateMachine *stateMachine = GetStateMachine( queryEventsParams->StateMachineName );
		if( stateMachine == NULL )
		{
			m_ToolBox->Log( LOGWARNING, _T("Cannot query events. Specified state machine %s cannot be found.\n"), queryEventsParams->StateMachineName->GetString() );
		}
		else
		{
			stateMachine->QueryEvents( &queryEventsParams->QueryEventsList );
		}
	}

	return MSG_HANDLED_STOP;
}

DWORD CQHStateMachineManager::OnGetStateMachineName( DWORD size, void* params )
{
	VERIFY_MESSAGE_SIZE(sizeof(GETSTATEMACHINEPARAMS), size);
	GETSTATEMACHINEPARAMS *getStateMachineParams = (GETSTATEMACHINEPARAMS*)params;

	if( getStateMachineParams != NULL && getStateMachineParams->StateMachineFilePath != NULL )
	{
		CQHStateMachine *stateMachine = GetStateMachineByFile( getStateMachineParams->StateMachineFilePath );
		if( stateMachine != NULL )
		{
			getStateMachineParams->StateMachineName = stateMachine->GetName();
		}
	}

	return MSG_HANDLED_STOP;
}

///	removes previous state machine mapping to file and adds new one
///	\param	stateMachine - pointer to state machine to map
///	\param	fileName - current file name for state machine
///	\param	oldFileName - old mapping file name. This value may be NULL
void CQHStateMachineManager::MapStateMachineToFileName( IObject *stateMachine, 
														IHashString *fileName, 
														IHashString *oldFileName )
{
	if( oldFileName != NULL && !oldFileName->IsEmpty() && oldFileName->GetString() != NULL )
	{
		StdString oldFile = oldFileName->GetString();
		oldFile.MakeSafeFileName();
		CHashString safeOldFileName( oldFile );

		m_StateMachineToFileName.erase( stateMachine->GetName()->GetUniqueID() );
		m_FilenameToStateMachine.erase( safeOldFileName.GetUniqueID() );
	}

	StdString file = fileName->GetString();
	file.MakeSafeFileName();
	CHashString safeFileName( file );

	// Map file unique IDs to corresponding state machines and vice versa
	// TODO: There seems to be a problem calling GetUniqueID() from the hash string returned from CQHStateMachine::GetFileName
	m_StateMachineToFileName[stateMachine->GetName()->GetUniqueID()] = safeFileName.GetUniqueID();
	m_FilenameToStateMachine[safeFileName.GetUniqueID()] = stateMachine;
}

CQHStateMachine *CQHStateMachineManager::GetStateMachineByFile( IHashString *fileName )
{
	CQHStateMachine *areturn = NULL;

	StdString file = fileName->GetString();
	file.MakeSafeFileName();
	CHashString srcFileName(file);

	static CHashString cqhsmTypeName(_T("CQHStateMachine"));
	IDTOOBJECTMAP::iterator fnameToSM = m_FilenameToStateMachine.find(srcFileName.GetUniqueID());
	if (fnameToSM != m_FilenameToStateMachine.end())
	{
		areturn = (CQHStateMachine*)(fnameToSM->second);
	}
	return areturn;
}

IHashString* CQHStateMachineManager::GetComponentType()
{
	return &m_ManagerComponentType;
}

void CQHStateMachineManager::AddStateObjectToUpdateQueue( CQHStateObject *stateObject )
{
	DWORD priority = stateObject->GetUpdatePriority();
	m_StateObjectUpdateMap.insert( make_pair( priority, stateObject ) );
}

void CQHStateMachineManager::RemoveStateObjectFromUpdateQueue( CQHStateObject *stateObject )
{
	DWORD priority = stateObject->GetUpdatePriority();
	SOUPDATEMAP::iterator startIter, endIter;
	startIter = m_StateObjectUpdateMap.lower_bound(priority);
	endIter = m_StateObjectUpdateMap.upper_bound(priority);
	while (startIter != endIter)
	{
		if (startIter->second == stateObject)
		{
			m_StateObjectUpdateMap.erase( startIter );
			break;
		}
		++startIter;
	}
}

//
// Deletes an Object and removes it from the manager
//
bool CQHStateMachineManager::DeleteObject( IObject* Object )
{
	// only remove object if we are mapping filename->StateMachine
	if (Object->IsKindOf(&m_stateMachineComponentType))
	{
		IHashString *objName = Object->GetName();
		IDTOIDMAP::iterator fileIDIter = m_StateMachineToFileName.find(objName->GetUniqueID());
		if( fileIDIter != m_StateMachineToFileName.end() )
		{
			DWORD fileNameID = fileIDIter->second;
			m_StateMachineToFileName.erase(fileIDIter);
			// see if in our map
			IDTOOBJECTMAP::iterator olIter = m_FilenameToStateMachine.find(fileNameID);
			// check to make sure it was found before deleting it
			if (olIter != m_FilenameToStateMachine.end())
			{
				m_FilenameToStateMachine.erase(olIter);
			}
		}
	}

	return CManager::DeleteObject(Object);
}