///============================================================================
/// \file		QHState.cpp
/// \brief		Implmentation of CQHState Class
/// \date		09-22-2006
/// \author		Richard Nguyen
/// \author		Kenneth Hurley, 2-19-2007, Major rewrite
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CQHState);
REGISTER_MESSAGE_HANDLER( GetSchema, OnGetSchema, CQHState );
REGISTER_MESSAGE_HANDLER( SetAbstract, OnSetAbstract, CQHState );
REGISTER_MESSAGE_HANDLER( GetAbstract, OnGetAbstract, CQHState );
REGISTER_MESSAGE_HANDLER( VisitHierarchy, OnVisitHierarchy, CQHState );
REGISTER_MESSAGE_HANDLER( GetStateMachineStateLabels, OnGetStateMachineStateLabels, CQHState );
REGISTER_MESSAGE_HANDLER( GetChildStateNames, OnGetChildStateNames, CQHState );
REGISTER_MESSAGE_HANDLER( GetStateEventLabels, OnGetStateEventLabels, CQHState );
REGISTER_MESSAGE_HANDLER( GetStateAvailableEventLabels, OnGetStateAvailableEventLabels, CQHState );
REGISTER_MESSAGE_HANDLER( ChangeObjectParent, OnChangeObjectParent, CQHState );
REGISTER_MESSAGE_HANDLER( GetEventNameFromLabel, OnGetEventNameFromLabel, CQHState );
REGISTER_MESSAGE_HANDLER( IsLabelEditble, OnIsLabelEditble, CQHState );

const float CQHState::m_fCurrentVersion = 1.2f;

// Default Constructor
CQHState::CQHState( IHashString *parentName, IHashString *name ) :
	OBJECTTEMPLATE( CQHStateMachineManager, CQHState, IAttributeObject, parentName, name ), 
	m_Name(_T("")),
	m_EntryEvent(_T("")),
	m_UpdateEvent(_T("")),
	m_ExitEvent(_T("")),
	m_StateMachine( NULL ),
	m_bAbstract( false )
{
}

// Default Destructor
CQHState::~CQHState()
{
}

IComponent *CQHState::Create( int nArgs, va_list argptr )
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = new CQHState(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

/// Set the parent name for this object
/// \param inName = CHashString name for parent
void CQHState::SetParentName(IHashString *inName)
{
	UnregisterState();
	CObjectTemplate<IAttributeObject>::SetParentName( inName );
	RegisterState();
}

void CQHState::Init()
{
	UnregisterState();
	m_Name = m_NewName;
	RegisterState();
}

bool CQHState::Update()
{
	// TODO: rnguyen
	return true;
}

void CQHState::DeInit()
{
	UnregisterState();

	static DWORD msgHash_DeleteObject = CHashString( _T("DeleteObject") ).GetUniqueID();
	DELETEOBJECTPARAMS dop;
	// delete child actions
	IDTOSTATEMAP::iterator itState = m_ChildStates.begin();
	while( itState != m_ChildStates.end() )
	{
		dop.name = itState->second->GetName();
		m_ToolBox->SendMessage( msgHash_DeleteObject, sizeof( dop ), &dop );
		itState = m_ChildStates.begin();
	}

	// delete child events
	IDTOEVENTMAP::iterator itEvent = m_Events.begin();
	while( itEvent != m_Events.end() )
	{
		dop.name = itEvent->second->GetName();
		m_ToolBox->SendMessage( msgHash_DeleteObject, sizeof( dop ), &dop );
		itEvent = m_Events.begin();
	}
}

void CQHState::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		float fVersion;
		ar.Read( fVersion, _T("version") );

		if( 1.2f == fVersion )
		{
			ReadFromVersion_1_2( ar );
		}
		else if( 1.1f == fVersion )
		{
			ReadFromVersion_1_1( ar );
		}
		else if( 1.0f == fVersion )
		{
			ReadFromVersion_1_0( ar );
		}
		else
		{
			LPCTSTR fmt = _T("%s(%d): Unsupported version (%f) of state machine state");
			m_ToolBox->Log( LOGERROR, fmt, __FILE__, __LINE__, fVersion );
			return;
		}
	}
	else
	{
		ar.Write( m_fCurrentVersion, _T("version") );
		ar.Write( m_Name.GetString(), _T("name") );
		ar.Write( m_EntryEvent.GetString(), _T("entryEvent") );
		ar.Write( m_UpdateEvent.GetString(), _T("updateEvent") );
		ar.Write( m_ExitEvent.GetString(), _T("exitEvent") );
	}
}

void CQHState::ReadFromVersion_1_2( IArchive &ar )
{
	StdString s;
	ar.Read( s, _T("name") );
	m_NewName = s;
	ar.Read( s, _T("entryEvent") );
	m_EntryEvent = s;
	ar.Read( s, _T("updateEvent") );
	m_UpdateEvent = s;
	ar.Read( s, _T("exitEvent") );
	m_ExitEvent = s;
}

void CQHState::ReadFromVersion_1_1( IArchive &ar )
{
	static const LPCTSTR *editorValueNames = GetEditorValueNames();
	StdString s;
	int iTemp;
	ar.Read( s, _T("name") );
	m_NewName = s;

	for( int i = 0; i < 4; ++i )
	{
		ar.Read( iTemp, editorValueNames[i] );
	}

	ar.Read( iTemp, _T("numSubStates") );
	ar.Read( iTemp, _T("numEvents") );
	ar.Read( s, _T("entryEvent") );
	m_EntryEvent = s;
	ar.Read( s, _T("updateEvent") );
	m_UpdateEvent = s;
	ar.Read( s, _T("exitEvent") );
	m_ExitEvent = s;

	for( int i = 4; i < 8; ++i )
	{
		ar.Read( iTemp, editorValueNames[i] );
	}
}

void CQHState::ReadFromVersion_1_0( IArchive &ar )
{
	static const LPCTSTR *editorValueNames = GetEditorValueNames();
	StdString s;
	int iTemp;
	ar.Read( s, _T("name") );
	m_NewName = s;

	for( int i = 0; i < 4; ++i )
	{
		ar.Read( iTemp, editorValueNames[i] );
	}

	ar.Read( iTemp, _T("numSubStates") );
	ar.Read( iTemp, _T("numEvents") );
	ar.Read( s, _T("entryEvent") );
	m_EntryEvent = s;
	ar.Read( s, _T("updateEvent") );
	m_UpdateEvent = s;
	ar.Read( s, _T("exitEvent") );
	m_ExitEvent = s;
}

void CQHState::GetAttribute( IHashString *hszAttributeName, IArchive *ar )
{
	if( ar != NULL )
	{
		ar->SetIsWriting( true );

		// TODO <rnguyen>: Very hard-coded. Consider indexing a table of attributes
		static DWORD hName = CHashString( "name" ).GetUniqueID();
		static DWORD hEntryEvent = CHashString( "entryEvent" ).GetUniqueID();
		static DWORD hUpdateEvent = CHashString( "updateEvent" ).GetUniqueID();
		static DWORD hExitEvent = CHashString( "exitEvent" ).GetUniqueID();
		const DWORD hashName = hszAttributeName->GetUniqueID();
		if( hashName == hName )
		{
			ar->Write( m_Name.GetString() );
		}
		else if( hashName == hEntryEvent )
		{
			ar->Write( m_EntryEvent.GetString() );
		}
		else if( hashName == hUpdateEvent )
		{
			ar->Write( m_UpdateEvent.GetString() );
		}
		else if( hashName == hExitEvent )
		{
			ar->Write( m_ExitEvent.GetString() );
		}
	}
}

void CQHState::SetAttribute( IHashString *hszAttributeName, IArchive *ar )
{
}

const LPCTSTR *CQHState::GetEditorValueNames()
{
	static const LPCTSTR names[EDITOR_VALUES_COUNT] = {
		CHashString( _T("x") ).GetString(),
		CHashString( _T("y") ).GetString(),
		CHashString( _T("width") ).GetString(),
		CHashString( _T("height") ).GetString(),
		CHashString( _T("flatX") ).GetString(),
		CHashString( _T("flatY") ).GetString(),
		CHashString( _T("flatWidth") ).GetString(),
		CHashString( _T("flatHeight") ).GetString()
	};
	return names;
}

///	\brief	register state in parent object and state machine
void CQHState::RegisterState()
{
	if( m_Name.IsEmpty() )
	{
		return;
	}

	IHashString *parentName = GetParentName();
	CQHStateMachineManager *amanager = GetManager();

	// Parent is either a state or state machine
	CQHState *parentState = amanager->GetState( parentName );
	if( parentState != NULL )
	{
		parentState->AddChildState( this );
		GetStateMachine()->AddGlobalState( this );
	}
	else
	{
		CQHStateMachine *parentStateMachine = amanager->GetStateMachine( parentName );
		if( parentStateMachine != NULL )
		{
			parentStateMachine->AddBaseState( this );
			parentStateMachine->AddGlobalState( this );
		}
		else
		{
			m_ToolBox->Log( LOGERROR, _T("Could not find parent object %s for state %s."), parentName->GetString(), GetName()->GetString() );
		}
	}
}

///	\brief	unregister state from parent object and state machine
void CQHState::UnregisterState()
{
	if( m_Name.IsEmpty() )
	{
		return;
	}

	IHashString *parentName = GetParentName();
	CQHStateMachineManager *amanager = GetManager();
	CQHState *parentState = amanager->GetState( parentName );
	if( parentState != NULL )
	{
		parentState->RemoveChildState( this );
		GetStateMachine()->RemoveGlobalState( this );
	}
	else
	{
		CQHStateMachine *parentStateMachine = GetStateMachine();
		assert( parentStateMachine != NULL );
		parentStateMachine->RemoveBaseState( this );
		parentStateMachine->RemoveGlobalState( this );
	}
}

CQHStateMachine *CQHState::GetStateMachine()
{
	if( m_StateMachine == NULL )
	{
		IHashString *parentName = GetParentName();
		CQHStateMachineManager *amanager = GetManager();
		CQHState *parentState = amanager->GetState( parentName );
		if( parentState != NULL )
		{
			m_StateMachine = parentState->GetStateMachine();
		}
		else
		{
			m_StateMachine = amanager->GetStateMachine( parentName );
			assert( m_StateMachine != NULL);
		}
	}

	return m_StateMachine;
}

CQHState *CQHState::GetParentState()
{
	CQHStateMachineManager *amanager = GetManager();
	CQHState *parentState = amanager->GetState( GetParentName() );
	return parentState;
}

void CQHState::AddEvent( CQHStateMachineEvent *aevent )
{
	m_Events[aevent->GetName()->GetUniqueID()] = aevent;
	m_EventsByLabel[aevent->GetEventName()->GetUniqueID()] = aevent;
}

void CQHState::RemoveEvent( CQHStateMachineEvent *aevent )
{
	m_Events.erase( aevent->GetName()->GetUniqueID() );
	m_EventsByLabel.erase( aevent->GetEventName()->GetUniqueID() );
}

void CQHState::AddChildState( CQHState *achildState )
{
	DWORD key = achildState->GetName()->GetUniqueID();
	assert( m_ChildStates.count( key ) == 0 );
	m_ChildStates.insert( make_pair( key, achildState ) );
}

void CQHState::RemoveChildState( CQHState *achildState )
{
	m_ChildStates.erase( achildState->GetName()->GetUniqueID() );
}

CQHStateMachineEvent *CQHState::GetEvent( IHashString *aeventName )
{
	const DWORD hashName = aeventName->GetUniqueID();
	IDTOEVENTMAP::iterator it = m_EventsByLabel.find( hashName );
	if( it != m_EventsByLabel.end() )
	{
		return dynamic_cast<CQHStateMachineEvent*>( it->second );
	}
	CQHState *parentState = GetParentState();
	if( parentState != NULL )
	{
		return parentState->GetEvent( aeventName );
	}

	return NULL;
}

IHashString *CQHState::GetStateName()
{
	return &m_Name;
}

bool CQHState::EventExists( IHashString *aeventName )
{
	bool areturn = false;

	if( m_Events.find( aeventName->GetUniqueID() ) != m_Events.end() )
	{
		areturn = true;
	}

	// TODO: Linear iteration. Any way to avoid this?
	if( !areturn )
	{
		IDTOSTATEMAP::iterator astatesIterator = m_ChildStates.begin();
		CQHState *aiteratedState = NULL;

		for( ; astatesIterator != m_ChildStates.end() && areturn == false; astatesIterator++ )
		{
			aiteratedState = astatesIterator->second;
			areturn = aiteratedState->EventExists( aeventName );
		}
	}

	return areturn;
}

IHashString* CQHState::GetEntryEventName()
{
	return &m_EntryEvent;
}

IHashString* CQHState::GetExitEventName()
{
	return &m_ExitEvent;
}

IHashString* CQHState::GetUpdateEventName()
{
	return &m_UpdateEvent;
}

bool CQHState::IsBaseParent( CQHState *atestState )
{
	bool areturn = false;
	CQHState *aiteratedParent = GetParentState();

	while( !areturn && aiteratedParent != NULL )
	{
		if( aiteratedParent == atestState )
		{
			areturn = true;
		}
		else
		{
			aiteratedParent = aiteratedParent->GetParentState();
		}
	}

	return areturn;
}

void CQHState::QueryEvents( QUERYEVENTSLIST *queryEventsList )
{
	OBJECTLIST availableEvents;
	GenerateAvailableEventsList( &availableEvents );

	OBJECTLIST::iterator eventsIterator = availableEvents.begin();
	for( ; eventsIterator != availableEvents.end(); eventsIterator++ )
	{
		CQHStateMachineEvent *iteratedEvent = (CQHStateMachineEvent*)*eventsIterator;

		QUERYEVENTITEM queryEventItem;
		queryEventItem.StateName = &m_Name;
		queryEventItem.EventName = iteratedEvent->GetEventName();
		queryEventsList->push_back( queryEventItem );
	}

	IDTOSTATEMAP::iterator statesIterator = m_ChildStates.begin();
	for( ; statesIterator != m_ChildStates.end(); statesIterator++ )
	{
		CQHState *iteratedState = statesIterator->second;
		iteratedState->QueryEvents( queryEventsList );
	}
}

void CQHState::GenerateAvailableEventsList( OBJECTLIST *availableEvents )
{
	CQHState *parentState = GetParentState();
	if( parentState != NULL )
	{
		parentState->GenerateAvailableEventsList( availableEvents );
	}

	IDTOEVENTMAP::iterator eventsIterator = m_Events.begin();
	for( ; eventsIterator != m_Events.end(); eventsIterator++ )
	{
		CQHStateMachineEvent *iteratedEvent = eventsIterator->second;
		IHashString *eventName = iteratedEvent->GetEventName();
		if( parentState == NULL || parentState->GetEvent( eventName ) == NULL )
		{
			availableEvents->push_back( iteratedEvent );
		}
	}
}

void CQHState::VisitNode( IVisitor *visitor )
{
	visitor->Visit( this, true );

	IDTOEVENTMAP::iterator eventsIterator = m_Events.begin();
	CQHStateMachineEvent *iteratedEvent;
	for( ; eventsIterator != m_Events.end(); eventsIterator++ )
	{
		iteratedEvent = eventsIterator->second;
		iteratedEvent->VisitNode( visitor );
	}

	IDTOSTATEMAP::iterator statesIterator = m_ChildStates.begin();
	CQHState *iteratedState;
	for( ; statesIterator != m_ChildStates.end(); statesIterator++ )
	{
		iteratedState = statesIterator->second;
		iteratedState->VisitNode( visitor );
	}

	visitor->Visit( this, false );
}

DWORD CQHState::OnGetSchema( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETSCHEMAPARAMS));
	GETSCHEMAPARAMS *getSchemaParams = (GETSCHEMAPARAMS*)params;
	IXMLArchive *archive = getSchemaParams->ar;
	if( archive != NULL )
	{
		archive->StartClass( GetComponentType()->GetString() );
		archive->Write( m_Name.GetString(), _T("Name") );
		archive->Write( 5 );

		archive->StartClass(_T("hidden") );
		archive->Write( _T("float"), _T("Type") );
		archive->EndClass();

		if( m_bAbstract )
		{
			archive->StartClass( _T("static") );
			archive->Write( _T("string"), _T("Type") );
			archive->Write( _T("State Name"), _T("Name") );
			archive->EndClass();

			archive->StartClass( _T("static") );
			archive->Write( _T("string"), _T("Type") );
			archive->Write( _T("Entry Event"), _T("Name") );
			archive->EndClass();

			archive->StartClass( _T("static") );
			archive->Write( _T("string"), _T("Type") );
			archive->Write( _T("Update Event"), _T("Name") );
			archive->EndClass();

			archive->StartClass( _T("static") );
			archive->Write( _T("string"), _T("Type") );
			archive->Write( _T("Exit Event"), _T("Name") );
			archive->EndClass();
		}
		else
		{
			TCHAR szComboNum[] = _T("ComboItem_XXXX");
			LPTSTR szNum = szComboNum + sizeof(szComboNum) / sizeof(*szComboNum) - 4;


			archive->StartClass( _T("edit") );
			archive->Write( _T("string"), _T("Type") );
			archive->Write( _T("State Name"), _T("Name") );
			archive->Write( false, _T("Key") );
			archive->EndClass();

			archive->StartClass( _T("combo") );
			archive->Write( _T("string"), _T("Type") );
			archive->Write( _T("Entry Event"), _T("Name") );
			archive->Write( false, _T("Key") );
			archive->Write( _T(""), _T("DefSelect") );

			OBJECTLIST eventList;
			GenerateAvailableEventsList( &eventList );
			int eventListCount = eventList.size() + 1;
			OBJECTLIST::iterator eventListIterator = eventList.begin();
			archive->Write( eventListCount, _T("Count") );
			int comboItemCounter = 0;
			_ltot( ++comboItemCounter, szNum, 10 );
			archive->Write( _T(""), szComboNum );
			for(; eventListIterator != eventList.end(); eventListIterator++ )
			{
				_ltot( ++comboItemCounter, szNum, 10 );
				CQHStateMachineEvent *iteratedEvent = (CQHStateMachineEvent*)(*eventListIterator);
				archive->Write( iteratedEvent->GetEventName()->GetString(), szComboNum );
			}
			archive->EndClass();

			archive->StartClass( _T("combo") );
			archive->Write( _T("string"), _T("Type") );
			archive->Write( _T("Update Event"), _T("Name") );
			archive->Write( false, _T("Key") );
			archive->Write( _T(""), _T("DefSelect") );

			eventListIterator = eventList.begin();
			archive->Write( eventListCount, _T("Count") );
			comboItemCounter = 0;
			_ltot( ++comboItemCounter, szNum, 10 );
			archive->Write( _T(""), szComboNum );
			for(; eventListIterator != eventList.end(); eventListIterator++ )
			{
				_ltot( ++comboItemCounter, szNum, 10 );
				CQHStateMachineEvent *iteratedEvent = (CQHStateMachineEvent*)(*eventListIterator);
				archive->Write( iteratedEvent->GetEventName()->GetString(), szComboNum );
			}
			archive->EndClass();

			archive->StartClass( _T("combo") );
			archive->Write( _T("string"), _T("Type") );
			archive->Write( _T("Exit Event"), _T("Name") );
			archive->Write( false, _T("Key") );
			archive->Write( _T(""), _T("DefSelect") );

			eventListIterator = eventList.begin();
			archive->Write( eventListCount, _T("Count") );
			comboItemCounter = 0;
			_ltot( ++comboItemCounter, szNum, 10 );
			archive->Write( _T(""), szComboNum );
			for(; eventListIterator != eventList.end(); eventListIterator++ )
			{
				_ltot( ++comboItemCounter, szNum, 10 );
				CQHStateMachineEvent *iteratedEvent = (CQHStateMachineEvent*)(*eventListIterator);
				archive->Write( iteratedEvent->GetEventName()->GetString(), szComboNum );
			}
			archive->EndClass();
		}

		archive->EndClass();
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Attempt to get CQHState schema. No archive specified.\n" ) );
	}

	return MSG_HANDLED_STOP;
}

DWORD CQHState::OnSetAbstract( DWORD size, void *params )
{
	m_bAbstract = true;
	return MSG_HANDLED_STOP;
}

DWORD CQHState::OnGetAbstract( DWORD size, void *params )
{
	bool *p = static_cast<bool *>( params );
	VERIFY_MESSAGE_SIZE( size, sizeof( *p ) );
	assert( p != NULL );
	*p = m_bAbstract;
	return MSG_HANDLED_STOP;
}

DWORD CQHState::OnVisitHierarchy( DWORD size, void *params )
{
	VISITHIERARCHYDATA *vhd = static_cast<VISITHIERARCHYDATA *>( params );
	VERIFY_MESSAGE_SIZE( size, sizeof( *vhd ) );
	VisitNode( vhd->visitor );
	return MSG_HANDLED_STOP;
}

DWORD CQHState::OnGetStateMachineStateLabels( DWORD size, void *params )
{
	GETSETSTATEMACHINENAMESPARAMS *gssmn;
	VERIFY_MESSAGE_SIZE( size, sizeof( *gssmn ) );
	gssmn = reinterpret_cast<GETSETSTATEMACHINENAMESPARAMS *>(params);
	GetStateMachine()->FillStateLabels( gssmn->Names );
	return MSG_HANDLED_STOP;
}

/// message handler for GetChildStateNames. provide list of base state names
DWORD CQHState::OnGetChildStateNames( DWORD size, void* params )
{
	GETSETSTATEMACHINENAMESPARAMS *gssmn;
	VERIFY_MESSAGE_SIZE( size, sizeof( *gssmn ) );
	gssmn = reinterpret_cast<GETSETSTATEMACHINENAMESPARAMS *>(params);
	gssmn->Names.clear();
	IDTOSTATEMAP::iterator it = m_ChildStates.begin();
	for( ; it != m_ChildStates.end(); ++it )
	{
		gssmn->Names.push_back( it->second->GetName() );
	}
	return MSG_HANDLED_STOP;
}

DWORD CQHState::OnGetStateEventLabels( DWORD size, void *params )
{
	GETSETSTATEMACHINENAMESPARAMS *gssmn;
	VERIFY_MESSAGE_SIZE( size, sizeof( *gssmn ) );
	gssmn = reinterpret_cast<GETSETSTATEMACHINENAMESPARAMS *>(params);
	IDTOEVENTMAP::iterator it = m_Events.begin();
	for( ; it != m_Events.end(); ++it )
	{
		gssmn->Names.push_back( it->second->GetEventName() );
	}
	return MSG_HANDLED_STOP;
}

///	collect all events up to state machine hierarchy
DWORD CQHState::OnGetStateAvailableEventLabels( DWORD size, void *params )
{
	GETSETSTATEMACHINENAMESPARAMS *gssmn;
	VERIFY_MESSAGE_SIZE( size, sizeof( *gssmn ) );
	gssmn = reinterpret_cast<GETSETSTATEMACHINENAMESPARAMS *>(params);

	set<IHashString *, LessIHashString> labels;
	GetAvailableEventLabels( labels, true );

	copy( labels.begin(), labels.end(), back_inserter( gssmn->Names ) );

	return MSG_HANDLED_STOP;
}

///	\brief	add state's available event labels to the set
///	\param	labels - result set with labels
///	\param	addInternal - if value set to false then entry, update and exit 
///						  events are not added to the label list
void CQHState::GetAvailableEventLabels( set<IHashString *, LessIHashString> &labels, bool addInternal )
{
	CQHState *parent = GetParentState();
	if( parent != NULL )
	{
		parent->GetAvailableEventLabels( labels, false );
	}

	IDTOEVENTMAP::iterator it = m_Events.begin();
	if( addInternal )
	{
		for( ; it != m_Events.end(); ++it )
		{
			assert( !it->second->GetEventName()->IsEmpty() );
			labels.insert( it->second->GetEventName() );
		}
	}
	else
	{
		for( ; it != m_Events.end(); ++it )
		{
			IHashString *eventName = it->second->GetEventName(); 
			if( m_EntryEvent != *eventName &&
				m_UpdateEvent != *eventName &&
				m_ExitEvent != *eventName )
			{
				assert( !eventName->IsEmpty() );
				labels.insert( eventName );
			}
		}
	}
}

CQHStateMachineManager *CQHState::GetManager()
{
	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CQHStateMachineManager *amanager = dynamic_cast<CQHStateMachineManager*>( amanagerComponent );
	return amanager;
}

DWORD CQHState::OnChangeObjectParent( DWORD size, void *params )
{
	CHANGEOBJECTPARENT *cop = static_cast<CHANGEOBJECTPARENT *>( params );
	VERIFY_MESSAGE_SIZE( sizeof( *cop ), size );

	static const DWORD hashType = CHashString( _T("CQHState") ).GetUniqueID();
	if( cop->objectCompType != NULL &&
		cop->objectCompType->GetUniqueID() != hashType )
	{
		assert( !"Unmatched type" );
		return MSG_ERROR;
	}

	if( *GetName() != *cop->objectName )
	{
		assert( !"Unmatched name" );
		return MSG_ERROR;
	}

	SetParentName( cop->newParentName );
	return MSG_HANDLED_PROCEED;
}

///	return event object name by event label
///	\param	size - sizeof(IHashString)
///	\param	params - [in] label of the event [out] - event object name
DWORD CQHState::OnGetEventNameFromLabel( DWORD size, void *params )
{
	IHashString *name = static_cast<IHashString *>( params );
	VERIFY_MESSAGE_SIZE( sizeof( *name ), size );

	if( name == NULL &&
		name->IsEmpty() )
	{
		assert( !"Empty label name is passed" );
		return MSG_ERROR;
	}

	CQHStateMachineEvent *event = GetEvent( name );
	if( event == NULL )
	{
		assert( !"Invalid label" );
		return MSG_ERROR;
	}

	*name = event->GetName()->GetString();

	return MSG_HANDLED_STOP;
}

///	\brief	return true if state name is editable (object is not abstract)
DWORD CQHState::OnIsLabelEditble( DWORD size, void *params )
{
	bool *p = static_cast<bool *>( params );
	VERIFY_MESSAGE_SIZE( size, sizeof( *p ) );
	assert( p != NULL );
	*p = !m_bAbstract;
	return MSG_HANDLED_STOP;
}
