///============================================================================
/// \file		CQHStateMachineEvent.cpp
/// \brief		Implentation of CQHStateMachineEvent Class
/// \date		09-25-2006
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CQHStateMachineEvent);
REGISTER_MESSAGE_HANDLER( GetSchema, OnGetSchema, CQHStateMachineEvent );
REGISTER_MESSAGE_HANDLER( SetAbstract, OnSetAbstract, CQHStateMachineEvent );
REGISTER_MESSAGE_HANDLER( GetAbstract, OnGetAbstract, CQHStateMachineEvent );
REGISTER_MESSAGE_HANDLER( VisitHierarchy, OnVisitHierarchy, CQHStateMachineEvent );
REGISTER_MESSAGE_HANDLER( GetEventActionHandlers, OnGetEventActionHandlers, CQHStateMachineEvent );
REGISTER_MESSAGE_HANDLER( SetEventActionHandlers, OnSetEventActionHandlers, CQHStateMachineEvent );
REGISTER_MESSAGE_HANDLER( GetEventTransitions, OnGetEventTransitions, CQHStateMachineEvent );
REGISTER_MESSAGE_HANDLER( GetStateMachineStateLabels, OnGetStateMachineStateLabels, CQHStateMachineEvent );
REGISTER_MESSAGE_HANDLER( IsLabelEditble, OnIsLabelEditble, CQHStateMachineEvent );

const float CQHStateMachineEvent::m_fCurrentVersion = 1.2f;

// Default Constructor
CQHStateMachineEvent::CQHStateMachineEvent( IHashString *parentName, IHashString *name ) :
	OBJECTTEMPLATE( CQHStateMachineManager, CQHStateMachineEvent, IAttributeObject, parentName, name )
{
	m_bAbstract = false;
	m_iTotalTransitionWeight = 0;
	m_bTransitionsDirty = false;
}

// Default Destructor
CQHStateMachineEvent::~CQHStateMachineEvent()
{
}

IComponent *CQHStateMachineEvent::Create( int nArgs, va_list argptr )
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
		self = new CQHStateMachineEvent(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CQHStateMachineEvent::Init()
{
	// TODO: rnguyen
}

bool CQHStateMachineEvent::Update()
{
	// TODO: rnguyen
	return true;
}

void CQHStateMachineEvent::DeInit()
{
	OBJECTLIST::iterator actionIterator = m_ActionHandlers.begin();
	while( actionIterator != m_ActionHandlers.end() )
	{
		CQHStateMachineActionHandler *iteratedAction = (CQHStateMachineActionHandler*)*actionIterator;

		DELETEOBJECTPARAMS dop;
		dop.name = iteratedAction->GetName();
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_DeleteObject, sizeof(dop), &dop );
		actionIterator = m_ActionHandlers.begin();
	}

	OBJECTLIST::iterator transitionIterator = m_Transitions.begin();
	while( transitionIterator != m_Transitions.end() )
	{
		CQHStateMachineTransition *iteratedTransition = (CQHStateMachineTransition*)*transitionIterator;

		DELETEOBJECTPARAMS dop;
		dop.name = iteratedTransition->GetName();
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop );
		transitionIterator = m_Transitions.begin();
	}

	CQHState *parentState = GetParentState();
	if (parentState)
	{
		parentState->RemoveEvent( this );
	}
}

void CQHStateMachineEvent::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		float fVersion;
		ar.Read( fVersion, _T("version") );
		if( fVersion > m_fCurrentVersion )
		{
			LPCTSTR fmt = _T("%s(%d): Unsupported version (%f) of state machine event");
			m_ToolBox->Log( LOGERROR, fmt, __FILE__, __LINE__, fVersion );
			return;
		}

		StdString tempStr;
		ar.Read( tempStr, _T("name") );
		m_Name = tempStr.c_str();
		
		if( 1.1f >= fVersion )
		{
			int iTemp;
			ar.Read( iTemp, _T("x") );
			ar.Read( iTemp, _T("y") );

			if( 1.0f == fVersion)
			{
				ar.Read( tempStr, _T("guard") );
				ar.Read( iTemp, _T("numActions") );
				ar.Read( iTemp, _T("numTransitions"));
			}
		}

		CQHState *parentState = GetParentState();
		if (parentState)
		{
			parentState->AddEvent( this );
		}
	}
	else
	{
		ar.Write( m_fCurrentVersion, _T("version") );
		ar.Write( m_Name.GetString(), _T("name") );
	}
}

void CQHStateMachineEvent::GetAttribute( IHashString *hszAttributeName, IArchive *ar )
{
	if( ar != NULL )
	{
		ar->SetIsWriting( true );

		static DWORD hName = CHashString( "name" ).GetUniqueID();
		if( hszAttributeName->GetUniqueID() == hName )
		{
			ar->Write( m_Name.GetString() );
		}
	}
}

void CQHStateMachineEvent::SetAttribute( IHashString *hszAttributeName, IArchive *ar )
{
}

void CQHStateMachineEvent::AddActionHandler( CQHStateMachineActionHandler *aactionHandler )
{
	OBJECTLIST::const_iterator it;
	it = find( m_ActionHandlers.begin(), m_ActionHandlers.end(), aactionHandler );
	if( it == m_ActionHandlers.end() )
	{
		m_ActionHandlers.push_back( aactionHandler );
	}
}

void CQHStateMachineEvent::AddTransition( CQHStateMachineTransition *atransition )
{
	OBJECTLIST::iterator it;
	it = find( m_Transitions.begin(), m_Transitions.end(), atransition );
	if( it == m_Transitions.end() )
	{
		int weight = atransition->GetWeight();
		// finding suitable position for insert based on transtion weight
		it = find_if( m_Transitions.begin(), m_Transitions.end(),
					  bind2nd( ptr_fun( IsTransitionLighter ), weight ) );
		m_Transitions.insert( it, atransition );
	}
	m_bTransitionsDirty = true;
}

void CQHStateMachineEvent::RemoveActionHandler( CQHStateMachineActionHandler *aaction )
{
	m_ActionHandlers.remove( aaction );
}

void CQHStateMachineEvent::RemoveTransition( CQHStateMachineTransition *atransition )
{
	m_Transitions.remove( atransition );
	m_bTransitionsDirty = true;
}

void CQHStateMachineEvent::ExecuteActionHandlers(IArchive *arch, IHashString *zEntityName)
{
	OBJECTLIST::iterator iter;
	iter = m_ActionHandlers.begin();

	for ( ; iter != m_ActionHandlers.end(); iter++ )
	{
		((CQHStateMachineActionHandler *)(*iter))->SendActionMessage(arch, zEntityName);
	}
}

bool CQHStateMachineEvent::HasTransitions()
{
	return !m_Transitions.empty();
}

CQHStateMachineTransition *CQHStateMachineEvent::GetTransition()
{
	CQHStateMachineTransition *retTransition = NULL;
	int transitionsCount = m_Transitions.size();

	if( transitionsCount == 1 )
	{
		OBJECTLIST::iterator transitionsIterator = m_Transitions.begin();
		retTransition = (CQHStateMachineTransition*)( *transitionsIterator );
	}
	else if( transitionsCount > 1 )
	{
		if( m_bTransitionsDirty )
		{
			m_iTotalTransitionWeight = GetTotalTransitionWeight();
			m_bTransitionsDirty = false;
		}
		int randomNumber = rand() % m_iTotalTransitionWeight;
		retTransition = GetTransition( randomNumber );
	}

	return retTransition;
}

IHashString *CQHStateMachineEvent::GetEventName()
{
	return &m_Name;
}

void CQHStateMachineEvent::VisitNode( IVisitor *visitor )
{
	visitor->Visit( this, true );

	OBJECTLIST::iterator actionsIterator;
	for( actionsIterator = m_ActionHandlers.begin(); actionsIterator != m_ActionHandlers.end(); actionsIterator++ )
	{
		CQHStateMachineActionHandler *iteratedAction = dynamic_cast<CQHStateMachineActionHandler*>( *actionsIterator );
		iteratedAction->VisitNode( visitor );
	}

	OBJECTLIST::iterator transitionsIterator;
	for( transitionsIterator = m_Transitions.begin(); transitionsIterator != m_Transitions.end(); transitionsIterator++ )
	{
		CQHStateMachineTransition *iteratedTransition = dynamic_cast<CQHStateMachineTransition*>( *transitionsIterator );
		iteratedTransition->VisitNode( visitor );
	}

	visitor->Visit( this, false );
}

DWORD CQHStateMachineEvent::OnGetSchema( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETSCHEMAPARAMS));
	GETSCHEMAPARAMS *getSchemaParams = (GETSCHEMAPARAMS*)params;
	IXMLArchive *archive = getSchemaParams->ar;
	if( archive != NULL )
	{
		archive->StartClass( GetComponentType()->GetString() );
		archive->Write( m_Name.GetString(), _T("Name") );
		archive->Write( 2 );

		archive->StartClass(_T("hidden") );
		archive->Write( _T("float"), _T("Type") );
		archive->EndClass();

		if( m_bAbstract )
		{
			archive->StartClass( _T("static") );
			archive->Write( _T("string"), _T("Type") );
			archive->Write( _T("Event Name"), _T("Name") );
			archive->EndClass();
		}
		else
		{
			archive->StartClass( _T("edit") );
			archive->Write( _T("string"), _T("Type") );
			archive->Write( _T("Event Name"), _T("Name") );
			archive->Write( false, _T("Key") );
			archive->EndClass();
		}

		archive->EndClass();
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Attempt to get CQHStateMachineEvent schema. No archive specified.\n" ) );
	}

	return MSG_HANDLED_STOP;
}

DWORD CQHStateMachineEvent::OnSetAbstract( DWORD size, void *params )
{
	m_bAbstract = true;
	return MSG_HANDLED_STOP;
}

DWORD CQHStateMachineEvent::OnGetAbstract( DWORD size, void *params )
{
	bool *p = static_cast<bool *>( params );
	VERIFY_MESSAGE_SIZE( size, sizeof( *p ) );
	assert( p != NULL );
	*p = m_bAbstract;
	return MSG_HANDLED_STOP;
}

DWORD CQHStateMachineEvent::OnVisitHierarchy( DWORD size, void *params )
{
	VISITHIERARCHYDATA *vhd = reinterpret_cast<VISITHIERARCHYDATA *>(params);
	VERIFY_MESSAGE_SIZE( size, sizeof(*vhd) );
	VisitNode( vhd->visitor );
	return MSG_HANDLED_STOP;
}

int CQHStateMachineEvent::GetTotalTransitionWeight()
{
	int totalTransitionWeight = 0;
	OBJECTLIST::iterator transitionsIterator = m_Transitions.begin();
	for( transitionsIterator; transitionsIterator != m_Transitions.end(); transitionsIterator++ )
	{
		CQHStateMachineTransition *iteratedTransition = (CQHStateMachineTransition*)( *transitionsIterator );
		totalTransitionWeight += iteratedTransition->GetWeight();
	}

	return totalTransitionWeight;
}

CQHStateMachineTransition *CQHStateMachineEvent::GetTransition( int iWeight )
{
	int accumulatedWeight = 0;
	CQHStateMachineTransition *retTransition = NULL;
	OBJECTLIST::iterator transitionsIterator = m_Transitions.begin();
	for( transitionsIterator; transitionsIterator != m_Transitions.end() && retTransition == NULL; transitionsIterator++ )
	{
		CQHStateMachineTransition *iteratedTransition = (CQHStateMachineTransition*)( *transitionsIterator );
		accumulatedWeight += iteratedTransition->GetWeight();
		if( iWeight < accumulatedWeight )
		{
			retTransition = iteratedTransition;
		}
	}

	return retTransition;
}

DWORD CQHStateMachineEvent::OnGetEventActionHandlers( DWORD size, void *params )
{
	GETSETSTATEMACHINENAMESPARAMS *gssmn;
	VERIFY_MESSAGE_SIZE( size, sizeof( *gssmn ) );
	gssmn = reinterpret_cast<GETSETSTATEMACHINENAMESPARAMS *>(params);

	gssmn->Names.clear();
	// copy object names from m_ActionHandlers to gssmn->Names
	transform(m_ActionHandlers.begin(), m_ActionHandlers.end(),
			  back_inserter(gssmn->Names),
			  mem_fun(&IObject::GetName));

	return MSG_HANDLED_STOP;
}

DWORD CQHStateMachineEvent::OnSetEventActionHandlers( DWORD size, void *params )
{
	GETSETSTATEMACHINENAMESPARAMS *gssmn;
	VERIFY_MESSAGE_SIZE( size, sizeof( *gssmn ) );
	gssmn = reinterpret_cast<GETSETSTATEMACHINENAMESPARAMS *>(params);

	// check that all action handers are already registered
	if( gssmn->Names.size() != m_ActionHandlers.size() )
	{
		m_ToolBox->Log( LOGERROR, _T("%s(%d): Failed to adjust event action handlers order"), __FILE__, __LINE__ );
		return MSG_ERROR;
	}

	// extract handlers map from manager
	static CHashString hszActionType = _T("CQHStateMachineActionHandler");
	IComponent *managerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CQHStateMachineManager *manager = dynamic_cast<CQHStateMachineManager*>( managerComponent );
	IDTOOBJECTMAP *objectMap = manager->GetObjectMap( &hszActionType );

	// fill new action handler list
	OBJECTLIST ActionHandlers;
	list<IHashString *>::const_iterator it = gssmn->Names.begin();
	for( ; it != gssmn->Names.end(); ++it )
	{
		IObject *object = manager->GetMapObject( *it, objectMap );
		if( object == NULL )
		{
			m_ToolBox->Log( LOGERROR, _T("%s(%d): Failed to find event action handlers"), __FILE__, __LINE__ );
			return MSG_ERROR;
		}
		if( *GetName() != *object->GetParentName() )
		{
			m_ToolBox->Log( LOGERROR, _T("%s(%d): Failed to set event action handlers order"), __FILE__, __LINE__ );
			return MSG_ERROR;
		}
		ActionHandlers.push_back( object );
	}

	// we may set action list if we reach this point
	m_ActionHandlers.swap( ActionHandlers );

	return MSG_HANDLED_STOP;
}

DWORD CQHStateMachineEvent::OnGetEventTransitions( DWORD size, void *params )
{
	GETSETSTATEMACHINENAMESPARAMS *gssmn;
	VERIFY_MESSAGE_SIZE( size, sizeof( *gssmn ) );
	gssmn = reinterpret_cast<GETSETSTATEMACHINENAMESPARAMS *>(params);

	gssmn->Names.clear();
	// copy object names from m_Transitions to gssmn->Names
	transform(m_Transitions.begin(), m_Transitions.end(),
			  back_inserter(gssmn->Names),
			  mem_fun(&IObject::GetName));

	return MSG_HANDLED_STOP;
}

DWORD CQHStateMachineEvent::OnGetStateMachineStateLabels( DWORD size, void *params )
{
	GETSETSTATEMACHINENAMESPARAMS *gssmn;
	VERIFY_MESSAGE_SIZE( size, sizeof( *gssmn ) );
	gssmn = reinterpret_cast<GETSETSTATEMACHINENAMESPARAMS *>(params);
	GetStateMachineStateLabels( gssmn->Names );
	return MSG_HANDLED_STOP;
}

///	\brief	extract labels from all state machine states
///	\param	[out]labels - reference to container with labels to fill
void CQHStateMachineEvent::GetStateMachineStateLabels( list<IHashString *> &labels )
{
	CQHStateMachine *machine = GetParentState()->GetStateMachine();
	machine->FillStateLabels( labels );
}

///	\brief	return true if state name is editable (object is not abstract)
DWORD CQHStateMachineEvent::OnIsLabelEditble( DWORD size, void *params )
{
	bool *p = static_cast<bool *>( params );
	VERIFY_MESSAGE_SIZE( size, sizeof( *p ) );
	assert( p != NULL );
	*p = !m_bAbstract;
	return MSG_HANDLED_STOP;
}

CQHState *CQHStateMachineEvent::GetParentState()
{
	IComponent *managerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CQHStateMachineManager *manager = dynamic_cast<CQHStateMachineManager*>( managerComponent );
	CQHState *parentState = manager->GetState( GetParentName() );
	// when the engine shuts down, the states get deleted first, followed by the events
	// so null state here is ok
	assert( parentState != NULL );
	return parentState;
}

/// this function is used for sorting transitions in the list by weight
///	\param	transtion - pointer to CQHStateMachineTransition object
///	\param	weight - weight to compare with transition weight
///	\return	true if transition has less weight than passed weight value
bool CQHStateMachineEvent::IsTransitionLighter( IObject *transtion , int weight )
{
	int transtionWeight = static_cast<CQHStateMachineTransition *>( transtion )->GetWeight();
	return transtionWeight < weight;
}
