///============================================================================
/// \file		QHStateMachine.cpp
/// \brief		Implentation of CQHStateMachine Class
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CQHStateMachine);
REGISTER_MESSAGE_HANDLER( MapToFileName, OnMapToFileName, CQHStateMachine );
REGISTER_MESSAGE_HANDLER( GetSchema, OnGetSchema, CQHStateMachine );
REGISTER_MESSAGE_HANDLER( VisitHierarchy, OnVisitHierarchy, CQHStateMachine );
REGISTER_MESSAGE_HANDLER( GetStateMachineStateLabels, OnGetStateMachineStateLabels, CQHStateMachine );
REGISTER_MESSAGE_HANDLER( GetChildStateNames, OnGetChildStateNames, CQHStateMachine );

const float	CQHStateMachine::m_fCurrentVersion = 1.3f;

// Default Constructor
CQHStateMachine::CQHStateMachine( IHashString *parentName, IHashString *name ) :
	OBJECTTEMPLATE( CQHStateMachineManager, CQHStateMachine, IObject, parentName, name ),
	m_StartState( NULL ),
	m_EndState( NULL )
{
}

// Default Destructor
CQHStateMachine::~CQHStateMachine()
{
}

IComponent *CQHStateMachine::Create( int nArgs, va_list argptr )
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
		self = new CQHStateMachine(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CQHStateMachine::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		StdString s;
		float fVersion;
		ar.Read( fVersion, _T("version") );
		if( fVersion > m_fCurrentVersion )
		{
			LPCTSTR fmt = _T("%s(%d): Unsupported version (%f) of state machine");
			m_ToolBox->Log( LOGERROR, fmt, __FILE__, __LINE__, fVersion );
			return;
		}
		// Dummy reading of the name. It should already be stored as CObjectTemplate
		ar.Read( s, _T("name") );
		if( fVersion < 1.4f )
		{
			ar.Read( m_iNumStates, _T("numStates") );
		}
		ar.Read( s, _T("startState") );
		m_NewParameters.m_StartStateName = s;
		ar.Read( s, _T("endState") );
		m_NewParameters.m_EndStateName = s;
		if( fVersion >= 1.3f )
		{
			ar.Read( s, _T("parentStateMachineFile") );
			s.MakeSafeFileName();
			m_NewParameters.m_ParentStateMachineFile = s;
			if( fVersion == 1.3f )
			{
				// this value exists in version 1.3 only
				ar.Read( s, _T("parentStateMachine") );
			}
		}
	}
	else
	{
		ar.Write( m_fCurrentVersion, _T("version") );
		ar.Write( GetName()->GetString(), _T("name") );
		m_iNumStates = m_BaseStates.size();
		ar.Write( m_iNumStates, _T("numStates") );
		ar.Write( m_Parameters.m_StartStateName.GetString(), _T("startState") );
		ar.Write( m_Parameters.m_EndStateName.GetString(), _T("endState") );
		ar.Write( m_Parameters.m_ParentStateMachineFile.GetString(), _T("parentStateMachineFile") );
		ar.Write( m_Parameters.m_ParentStateMachine.GetString(), _T("parentStateMachine") );
	}
}

void CQHStateMachine::Init()
{
	m_StartState = NULL;
	m_EndState = NULL;

	if( m_Parameters.m_ParentStateMachineFile != m_NewParameters.m_ParentStateMachineFile )
	{
		if( !m_Parameters.m_ParentStateMachineFile.IsEmpty() )
		{
			// remove states from old parent state machine
			vector<IHashString *> abstractStates;
			abstractStates.reserve( m_BaseStates.size() );

			IDTOSTATEMAP::iterator itMap = m_BaseStates.begin();
			for( ; itMap != m_BaseStates.end(); ++itMap )
			{
				if( itMap->second->IsAbstract() )
				{
					abstractStates.push_back( itMap->second->GetName() );
				}
			}

			DELETEOBJECTPARAMS dop;
			vector<IHashString *>::iterator it = abstractStates.begin();
			for( ; it != abstractStates.end(); ++it )
			{
				dop.name = *it;
				static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
				m_ToolBox->SendMessage( msgHash_DeleteObject, sizeof(dop), &dop );
			}
		}

		m_Parameters = m_NewParameters;

		if( !m_Parameters.m_ParentStateMachineFile.IsEmpty() )
		{
			CQHStateMachine	*stateMachine;
			stateMachine = RegisterStateMachine( &m_Parameters.m_ParentStateMachineFile );
			if( stateMachine != NULL )
			{
				m_Parameters.m_ParentStateMachine = stateMachine->GetName();
				CQHStateMachineCloneVisitor visitor( this, stateMachine );
				visitor.CloneSourceToDestination();
			}
		}
	}
	else
	{
		m_Parameters = m_NewParameters;
	}
}

void CQHStateMachine::DeInit()
{
	DELETEOBJECTPARAMS dop;
	IDTOSTATEMAP::iterator stateIterator = m_BaseStates.begin();
	while( stateIterator != m_BaseStates.end() )
	{
		dop.name = stateIterator->second->GetName();
		stateIterator = m_BaseStates.erase( stateIterator );
		static DWORD msgHash_DeleteObject = CHashString( _T("DeleteObject") ).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_DeleteObject, sizeof( dop ), &dop );
	}
}

DWORD CQHStateMachine::OnMapToFileName( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(IHashString) );
	IHashString *hszFileName = (IHashString*)params;

	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CQHStateMachineManager *amanager = (CQHStateMachineManager*)amanagerComponent;

	amanager->MapStateMachineToFileName( this, hszFileName, &m_FileName );
	m_FileName = hszFileName;

	return MSG_HANDLED_STOP;
}

bool CQHStateMachine::AddBaseState( CQHState *aQHState )
{
	IHashString *stateName;
	IHashString *stateLabel;
	stateName = aQHState->GetName();
	stateLabel = aQHState->GetStateName();
	if (stateName != NULL)
	{
		m_BaseStates[stateName->GetUniqueID()] = aQHState;
		m_BaseStatesByLabel[stateLabel->GetUniqueID()] = aQHState;
		return true;
	}

	m_ToolBox->Log(LOGERROR, "State Names cannot be null, inside CQHStateMachine::AddBaseState\n");
	return false;

}

bool CQHStateMachine::AddGlobalState( CQHState *aQHState )
{
	IHashString *stateName;
	IHashString *stateLabel;
	stateName = aQHState->GetName();
	stateLabel = aQHState->GetStateName();
	if (stateName != NULL)
	{
		m_GlobalStates[stateName->GetUniqueID()] = aQHState;
		m_GlobalStatesByLabel[stateLabel->GetUniqueID()] = aQHState;
		return true;
	}

	m_ToolBox->Log(LOGERROR, "State Names cannot be null, inside CQHStateMachine::AddGlobalState\n");
	return false;

}

void CQHStateMachine::RemoveBaseState( CQHState *aQHState )
{
	IHashString *aStateName;
	IHashString *aStateLabel;
	
	aStateName = aQHState->GetName();
	aStateLabel = aQHState->GetStateName();
	if (aStateName != NULL)
	{
		m_BaseStates.erase( aStateName->GetUniqueID() );
		m_BaseStatesByLabel.erase( aStateLabel->GetUniqueID() );
	}
}

void CQHStateMachine::RemoveGlobalState( CQHState *aQHState )
{
	IHashString *aStateName;
	IHashString *aStateLabel;

	aStateName = aQHState->GetName();
	aStateLabel = aQHState->GetStateName();
	if (aStateName != NULL)
	{
		m_GlobalStates.erase( aStateName->GetUniqueID() );
		m_GlobalStatesByLabel.erase( aStateLabel->GetUniqueID() );
	}
}

CQHState *CQHStateMachine::GetStartState()
{
	if( m_StartState == NULL )
	{
		m_StartState = GetGlobalStateByLabel( &m_Parameters.m_StartStateName );
	}

	return m_StartState;
}

CQHState *CQHStateMachine::GetEndState()
{
	if( m_EndState == NULL )
	{
		m_EndState = GetGlobalStateByLabel( &m_Parameters.m_EndStateName );
	}

	return m_EndState;
}

IHashString *CQHStateMachine::GetFileName()
{
	return &m_FileName;
}

CQHState *CQHStateMachine::GetBaseStateByLabel( IHashString *astateLabel )
{
	CQHState *astate = m_BaseStatesByLabel[astateLabel->GetUniqueID()];

	return astate;
}

CQHState *CQHStateMachine::GetGlobalStateByLabel( IHashString *astateLabel )
{
	CQHState *astate = m_GlobalStatesByLabel[astateLabel->GetUniqueID()];

	return astate;
}

bool CQHStateMachine::EventExists( IHashString *aeventName )
{
	bool areturn = false;

	IDTOSTATEMAP::iterator astatesIterator = m_BaseStates.begin();
	CQHState *aiteratedState = NULL;
	for( ; astatesIterator != m_BaseStates.end() && areturn == false; astatesIterator++ )
	{
		aiteratedState = astatesIterator->second;
		areturn = aiteratedState->EventExists( aeventName );
	}

	return areturn;
}

void CQHStateMachine::QueryEvents( QUERYEVENTSLIST *queryEventsList )
{
	IDTOSTATEMAP::iterator statesIterator = m_BaseStates.begin();
	for( ; statesIterator != m_BaseStates.end(); statesIterator++ )
	{
		CQHState *iteratedState = statesIterator->second;
		iteratedState->QueryEvents( queryEventsList );
	}
}

DWORD CQHStateMachine::OnGetSchema( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETSCHEMAPARAMS));
	GETSCHEMAPARAMS *getSchemaParams = (GETSCHEMAPARAMS*)params;
	IXMLArchive *archive = getSchemaParams->ar;
	if( archive != NULL )
	{
		TCHAR szComboNum[] = _T("ComboItem_XXXX");
		LPTSTR szNum = szComboNum + sizeof(szComboNum) / sizeof(*szComboNum) - 4;

		archive->StartClass( GetComponentType()->GetString() );
		archive->Write( GetName()->GetString(), _T("Name") );
		archive->Write( 6 );

		archive->StartClass(_T("hidden") );
		archive->Write( _T("float"), _T("Type") );
		archive->EndClass();

		archive->StartClass( _T("edit") );
		archive->Write( _T("string"), _T("Type") );
		archive->Write( _T("State Machine Name"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("static") );
		archive->Write( _T("int"), _T("Type") );
		archive->Write( _T("States"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->EndClass();

		archive->StartClass( _T("combo") );
		archive->Write( _T("string"), _T("Type") );
		archive->Write( _T("Start State"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->Write( _T(""), _T("DefSelect") );
		
		int stateMapCount = m_GlobalStatesByLabel.size();
		IDTOSTATEMAP::iterator stateMapIterator = m_GlobalStatesByLabel.begin();
		archive->Write( stateMapCount, _T("Count") );
		int comboItemCounter = 0;
		for(; stateMapIterator != m_GlobalStatesByLabel.end(); stateMapIterator++ )
		{
			_ltot( ++comboItemCounter, szNum, 10 );
			CQHState *iteratedState = (CQHState*)stateMapIterator->second;
			archive->Write( iteratedState->GetStateName()->GetString(), szComboNum );
		}
		archive->EndClass();

		archive->StartClass( _T("combo") );
		archive->Write( _T("string"), _T("Type") );
		archive->Write( _T("End State"), _T("Name") );
		archive->Write( false, _T("Key") );
		archive->Write( _T(""), _T("DefSelect") );
		
		stateMapIterator = m_GlobalStatesByLabel.begin();
		// reserve place for empty string combo item
		archive->Write( stateMapCount + 1, _T("Count") );
		comboItemCounter = 0;
		_ltot( ++comboItemCounter, szNum, 10 );
		archive->Write( _T(""), szComboNum );
		for(; stateMapIterator != m_GlobalStatesByLabel.end(); stateMapIterator++ )
		{
			_ltot( ++comboItemCounter, szNum, 10 );
			CQHState *iteratedState = (CQHState*)stateMapIterator->second;
			archive->Write( iteratedState->GetStateName()->GetString(), szComboNum );
		}
		archive->EndClass();

		archive->StartClass( _T("file") );
		archive->Write( _T("string"), _T("Type") );
		archive->Write( _T("Parent State Machine File"), _T("Name") );
		archive->Write( _T(""), _T("DefFile") );
		archive->Write( _T("*.xms"), _T("defExt") );
		archive->Write( _T("State Machine Files|*.xms|"), _T("filters") );
		archive->EndClass();

		archive->EndClass();
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Attempt to get CQHStateMachine schema. No archive specified.\n" ) );
	}

	return MSG_HANDLED_STOP;
}

DWORD CQHStateMachine::OnVisitHierarchy( DWORD size, void* params )
{
	VISITHIERARCHYDATA *vhd = reinterpret_cast<VISITHIERARCHYDATA *>(params);
	VERIFY_MESSAGE_SIZE( size, sizeof(*vhd) );
	VisitNode( vhd->visitor );
	return MSG_HANDLED_STOP;
}

DWORD CQHStateMachine::OnGetStateMachineStateLabels( DWORD size, void* params )
{
	GETSETSTATEMACHINENAMESPARAMS *gssmn;
	VERIFY_MESSAGE_SIZE( size, sizeof( *gssmn ) );
	gssmn = reinterpret_cast<GETSETSTATEMACHINENAMESPARAMS *>(params);
	FillStateLabels( gssmn->Names );
	return MSG_HANDLED_STOP;
}

/// message handler for GetChildStateNames. provide list of base state names
DWORD CQHStateMachine::OnGetChildStateNames( DWORD size, void* params )
{
	GETSETSTATEMACHINENAMESPARAMS *gssmn;
	VERIFY_MESSAGE_SIZE( size, sizeof( *gssmn ) );
	gssmn = reinterpret_cast<GETSETSTATEMACHINENAMESPARAMS *>(params);
	gssmn->Names.clear();
	IDTOSTATEMAP::iterator it = m_BaseStates.begin();
	for( ; it != m_BaseStates.end(); ++it )
	{
		gssmn->Names.push_back( it->second->GetName() );
	}
	return MSG_HANDLED_STOP;
}

void CQHStateMachine::VisitNode( IVisitor *visitor )
{
	visitor->Visit( this, true );

	for( IDTOSTATEMAP::iterator it = m_BaseStates.begin(); it != m_BaseStates.end(); ++it )
	{
		it->second->VisitNode( visitor );
	}
	
	visitor->Visit( this, false );
}

///	\brief	extract labels from all state machine states
///	\param	[out]labels - reference to container with labels to fill
void CQHStateMachine::FillStateLabels( list<IHashString *> &labels )
{
	labels.clear();
	FillStateLabels( m_GlobalStatesByLabel, labels );
}

///	\brief	extract labels from specific states container
///	\param	[in]states - reference to map with labels to fill
///	\param	[out]labels - reference to container with labels to fill
void CQHStateMachine::FillStateLabels( IDTOSTATEMAP & states, list<IHashString *> &labels )
{
	IDTOSTATEMAP::iterator it = states.begin();
	for( ; it != states.end(); ++it )
	{
		labels.push_back( it->second->GetStateName() );
	}
}

CQHStateMachine *CQHStateMachine::RegisterStateMachine( IHashString *file )
{
	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CQHStateMachineManager *amanager = dynamic_cast<CQHStateMachineManager*>( amanagerComponent );

	CQHStateMachine *stateMachine = amanager->GetStateMachineByFile( file );

	if( stateMachine == NULL )
	{
		amanager->AddStateMachine( file );
		stateMachine = amanager->GetStateMachineByFile( file );
		if( stateMachine == NULL )
		{
			m_ToolBox->Log( LOGERROR, _T( "Specified state machine file not found: %s\n" ), file->GetString() );
		}
	}
	return stateMachine;
}