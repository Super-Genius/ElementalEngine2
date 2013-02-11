///=====================================================================
/// \file	StateMachineValidator.cpp
/// \brief	Implementation of CStateMachineValidator
/// \date	12/28/2007
/// \author	Andrey Ivanov
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
///=====================================================================
#include "stdafx.h"

///	\brief	constructor
///	\param	name - name of the validated state machine
CStateMachineValidator::CStateMachineValidator( IHashString *name )
	: m_pToolBox( EngineGetToolBox() )
	, m_hszStateMachineName( name )
{
}

///	\brief	reset internal caches
void CStateMachineValidator::ResetCaches()
{
	m_StateLabels.clear();
}

///	\brief	validate data for state machine's object
///	\param	type - type of the object
///	\param	objectName - pointer to IHashString with object name
///	\param	ar - archive with checked data
///	\param	checkName - set to true if name from archive data should be 
///					    checked for uniqueness. This value may be set to 
///						false if existing state machine data is checking
///	\param	errors - list with found errors
///	\return	true if passed data is valid for state machine object
bool CStateMachineValidator::ValidateObject( IHashString *type,
											 IHashString *objectName,
											 IArchive *ar,
											 bool checkName,
											 list<CValidationError> &errors )
{
	ASSERT( type != NULL );
	ASSERT( objectName != NULL );
	ASSERT( ar != NULL );

	static const DWORD hashStateMachine = CHashString( _T("CQHStateMachine") ).GetUniqueID();
	static const DWORD hashState = CHashString( _T("CQHState") ).GetUniqueID();
	static const DWORD hashEvent = CHashString( _T("CQHStateMachineEvent") ).GetUniqueID();
	static const DWORD hashAction = CHashString( _T("CQHStateMachineActionHandler") ).GetUniqueID();
	static const DWORD hashTransition = CHashString(_T("CQHStateMachineTransition")).GetUniqueID();

	const DWORD hashType = type->GetUniqueID();
	if( hashType == hashState )
	{
		return DoValidateState( objectName, ar, checkName, errors );
	}
	else if( hashType == hashEvent )
	{
		return DoValidateEvent( objectName, ar, checkName, errors );
	}
	else if( hashType == hashAction )
	{
		return DoValidateAction( objectName, ar, errors );
	}
	else if( hashType == hashTransition )
	{
		return DoValidateTransition( objectName, ar, errors );
	}
	else if( hashType == hashStateMachine )
	{
		return DoValidateStateMachine( objectName, ar, checkName, errors );
	}

	// this is not a state machine object and we cannot validate it
	// so nothing to do with passed data
	ASSERT( !"Unsupported object type" );
	m_pToolBox->Log( LOGERROR, _T("%s(%d): Unsupported object type"), __FILE__, __LINE__ );
	return true;
}

///	\brief	check passed serialization data for error
///	\param	sop - pointer to serialize object parameters
///	\param	errors - list with found errors
///	\return	true if passed data is valid for object
bool CStateMachineValidator::ValidateObject( SERIALIZEOBJECTPARAMS *sop, list<CValidationError> &errors )
{
	ASSERT( sop != NULL );
	IHashString *objectName = sop->name;
	ASSERT( objectName != NULL );
	IArchive *ar = sop->archive;
	ASSERT( ar != NULL );
	ASSERT( ar->IsReading() );
	DWORD pos = ar->SeekTo( 0, SEEK_CUR );

	CHashString hszType = GetComponentType( objectName );
	bool valid = ValidateObject( &hszType, objectName, ar, true, errors );

	ar->SeekTo( pos );
	return valid;
}

///	\brief	check state machine with passed name for error based on whole 
///			rule list
///	\param	errors - list with found errors
///	\return	true if state machine is valid
bool CStateMachineValidator::Validate( list<CValidationError> &errors )
{
	bool valid = true;
	IArchive *ar = CreateMemoryArchive();
	vector<IObject *> children;

	ResetCaches();

	if( MSG_HANDLED == GetEEObjectChildren( &m_hszStateMachineName, children, true ) )
	{
		vector<ObjectInfo> objects;
		for( vector<IObject *>::iterator it = children.begin(); it != children.end(); ++it )
		{
			IObject *object = *it;
			ar->SetIsWriting( true );
			ar->SeekTo( 0 );
			object->Serialize( *ar );
			ar->SetIsWriting( false );
			ar->SeekTo( 0 );
			if( !ValidateObject( object->GetComponentType(), object->GetName(), ar, false, errors ) )
			{
				valid = false;
				break;
			}
		}
	}

	ar->Close();

	return valid;
}

///	\brief	validate data for state machine object
///	\param	objectName - pointer to IHashString with object name
///	\param	ar - archive with checked data
///	\param	checkName - set to true if name from archive data should be 
///					    checked for uniqueness. This value may be set to 
///						false if existing state machine data is checking
///	\param	errors - list with found errors
///	\return	true if passed data is valid for state machine object
bool CStateMachineValidator::DoValidateStateMachine( IHashString *objectName,
													 IArchive *ar,
													 bool checkName,
													 list<CValidationError> &errors )
{
	bool valid = true;
	ASSERT( ar->IsReading() );
	float fVersion;
	ar->Read( fVersion, _T("version") );
	if( fVersion <= 1.4f )
	{
		// validate state machine name
		StdString name;
		ar->Read( name, _T("name") );
		if( name.empty() )
		{
			CString msg = FormatString( IDS_VALIDATE_MACHINE_EMPTY_NAME );
			errors.push_back( CValidationError( objectName, msg ) );
			valid = false;
		}
		else if( checkName && 
				 name != objectName->GetString())
		{
			// validate state machine name uniqueness
			CHashString hszName = name;
			if( DoesEEObjectExist( &hszName ) )
			{
				CString msg = FormatString( IDS_VALIDATE_MACHINE_BAD_NAME, name.c_str() );
				errors.push_back( CValidationError( objectName, msg ) );
				valid = false;
			}
		}
		if( fVersion <= 1.3f )
		{
			int numStates;
			ar->Read( numStates, _T("numStates") );
		}
		StdString state;
		ar->Read( state, _T("startState") );
		if( state.empty() )
		{
			CString msg = FormatString( IDS_VALIDATE_MACHINE_EMPTY_START_STATE );
			errors.push_back( CValidationError( objectName, msg ) );
			valid = false;
		}
		else if( !IsStateLabel( state ) )
		{
			CString msg = FormatString( IDS_VALIDATE_MACHINE_BAD_START_STATE, state.c_str() );
			errors.push_back( CValidationError( objectName, msg ) );
			valid = false;
		}
		ar->Read( state, _T("endState") );
		if( !state.empty() && !IsStateLabel( state ) )
		{
			CString msg = FormatString( IDS_VALIDATE_MACHINE_BAD_END_STATE, state.c_str() );
			errors.push_back( CValidationError( objectName, msg ) );
			valid = false;
		}
		if( fVersion >= 1.3f )
		{
			// check parent state machine file name
			StdString s;
			ar->Read( s, _T("parentStateMachineFile") );
			if( !s.empty() )
			{
				FILE *f = _tfopen( s.c_str(), _T("rt") );
				if( f != NULL )
				{
					fclose( f );
				}
				else
				{
					CString msg = FormatString( IDS_VALIDATE_MACHINE_BAD_PARENT_FILE, s.c_str() );
					errors.push_back( CValidationError( objectName, msg ) );
					valid = false;
				}
			}
			if( fVersion == 1.3f )
			{
				ar->Read( s, _T("parentStateMachine") );
			}
		}
	}
	else
	{
		ASSERT( !"Unsupported version" );
	}

	return valid;
}

///	\brief	validate data for state object
///	\param	objectName - pointer to IHashString with object name
///	\param	ar - archive with checked data
///	\param	checkName - set to true if name from archive data should be 
///					    checked for uniqueness. This value may be set to 
///						false if existing state machine data is checking
///	\param	errors - list with found errors
///	\return	true if passed data is valid for state object
bool CStateMachineValidator::DoValidateState( IHashString *objectName,
											  IArchive *ar,
											  bool checkName,
											  list<CValidationError> &errors )
{
	bool valid = true;
	ASSERT( ar->IsReading() );
	float fVersion;
	ar->Read( fVersion, _T("version") );
	if( fVersion <= 1.2f )
	{
		StdString name;
		ar->Read( name, _T("name") );
		if( name.empty() )
		{
			CString msg = FormatString( IDS_VALIDATE_STATE_EMPTY_LABEL );
			errors.push_back( CValidationError( objectName, msg ) );
			valid = false;
		}
		else if( checkName )
		{
			StdString currentName;
			IArchive *current = GetEEObjectProperties( objectName );
			float fCurrentVersion;
			current->Read( fCurrentVersion, _T("version") );
			ASSERT( fCurrentVersion == 1.2f );
			current->Read( currentName, _T("name") );
			// validate state label uniqueness
			if( name != currentName && IsStateLabel( name ) )
			{
				// this state label already exists
				CString msg = FormatString( IDS_VALIDATE_STATE_BAD_LABEL, name.c_str() );
				errors.push_back( CValidationError( objectName, msg ) );
				valid = false;
			}
			current->Close();
		}

		if( fVersion <= 1.1)
		{
			int iTemp;
			ar->Read( iTemp, _T("x") );
			ar->Read( iTemp, _T("y") );
			ar->Read( iTemp, _T("width") );
			ar->Read( iTemp, _T("height") );
			ar->Read( iTemp, _T("numSubStates") );
			ar->Read( iTemp, _T("numEvents") );
		}
		// prepare list with event labels
		Labels events;
		FillStateAvailableEvents( objectName, events );

		StdString event;
		// check entry event label correctness
		ar->Read( event, _T("entryEvent") );
		if( !event.empty() && !IsEventLabel( events, event ) )
		{
			CString msg = FormatString( IDS_VALIDATE_STATE_BAD_ENTRY_EVENT, event.c_str() );
			errors.push_back( CValidationError( objectName, msg ) );
			valid = false;
		}

		// check update event label correctness
		ar->Read( event, _T("updateEvent") );
		if( !event.empty() && !IsEventLabel( events, event ) )
		{
			CString msg = FormatString( IDS_VALIDATE_STATE_BAD_UPDATE_EVENT, event.c_str() );
			errors.push_back( CValidationError( objectName, msg ) );
			valid = false;
		}

		// check exit event label correctness
		ar->Read( event, _T("exitEvent") );
		if( !event.empty() && !IsEventLabel( events, event ) )
		{
			CString msg = FormatString( IDS_VALIDATE_STATE_BAD_EXIT_EVENT, event.c_str() );
			errors.push_back( CValidationError( objectName, msg ) );
			valid = false;
		}
	}
	else
	{
		ASSERT( !"Unsupported version" );
	}

	return valid;
}

///	\brief	validate data for event object
///	\param	objectName - pointer to IHashString with object name
///	\param	ar - archive with checked data
///	\param	checkName - set to true if name from archive data should be 
///					    checked for uniqueness. This value may be set to 
///						false if existing state machine data is checking
///	\param	errors - list with found errors
///	\return	true if passed data is valid for event object
bool CStateMachineValidator::DoValidateEvent( IHashString *objectName,
											  IArchive *ar,
											  bool checkName,
											  list<CValidationError> &errors )
{
	bool valid = true;
	ASSERT( ar->IsReading() );
	float fVersion;
	ar->Read( fVersion, _T("version") );
	if( fVersion <= 1.2f )
	{
		StdString name;
		ar->Read( name, _T("name") );
		if( name.empty() )
		{
			CString msg = FormatString( IDS_VALIDATE_EVENT_EMPTY_LABEL );
			errors.push_back( CValidationError( objectName, msg ) );
			valid = false;
		}
		else if( checkName )
		{
			StdString currentName;
			IArchive *current = GetEEObjectProperties( objectName );
			float fCurrentVersion;
			current->Read( fCurrentVersion, _T("version") );
			ASSERT( fCurrentVersion == 1.2f );
			current->Read( currentName, _T("name") );
			// validate state label uniqueness
			if( name != currentName )
			{
				static CHashString hszEvent = _T("CQHStateMachineEvent");
				IObject *pEvent = GetStateMachineObject( objectName, &hszEvent );
				Labels events;
				FillStateEvents( pEvent->GetParentName(), events );
				if( IsEventLabel( events, name ) )
				{
					// this state label already exists
					CString msg = FormatString( IDS_VALIDATE_EVENT_BAD_LABEL, name.c_str() );
					errors.push_back( CValidationError( objectName, msg ) );
					valid = false;
				}
			}
			current->Close();
		}

		// prepare list with event labels
		Labels events;
		FillStateAvailableEvents( objectName, events );
	}
	else
	{
		ASSERT( !"Unsupported version" );
	}

	return valid;
}

///	\brief	validate data for action object
///	\param	objectName - pointer to IHashString with object name
///	\param	ar - archive with checked data
///	\param	errors - list with found errors
///	\return	true if passed data is valid for action object
bool CStateMachineValidator::DoValidateAction( IHashString *objectName, IArchive *ar,
											   list<CValidationError> &errors )
{
	bool valid = true;
	ASSERT( ar->IsReading() );
	float fVersion;
	ar->Read( fVersion, _T("version") );
	if( fVersion <= 1.0f )
	{
		StdString name;
		ar->Read( name, _T("name") );
		StdString actionName;
		ar->Read( actionName, _T("actionName") );
		if( actionName.empty() )
		{
			CString msg = FormatString( IDS_VALIDATE_ACTION_EMPTY_NAME );
			errors.push_back( CValidationError( objectName, msg ) );
			valid = false;
		}
	}
	else
	{
		ASSERT( !"Unsupported version" );
	}

	return valid;
}

///	\brief	validate data for transition object
///	\param	objectName - pointer to IHashString with object name
///	\param	ar - archive with checked data
///	\param	errors - list with found errors
///	\return	true if passed data is valid for transition object
bool CStateMachineValidator::DoValidateTransition( IHashString *objectName, IArchive *ar,
												   list<CValidationError> &errors )
{
	bool valid = true;
	ASSERT( ar->IsReading() );
	float fVersion;
	ar->Read( fVersion, _T("version") );
	if( fVersion <= 1.0f )
	{
		StdString name;
		ar->Read( name, _T("name") );

		int weight;
		ar->Read( weight, _T("weight") );
		if( 0 == weight )
		{
			// checking that other transition from event has non-zero weight
			static CHashString hszTransition = _T("CQHStateMachineTransition");
			IObject *pTransition = GetStateMachineObject( objectName, &hszTransition );

			static CHashString hszCQHStateMachineEvent = _T("CQHStateMachineEvent");
			GETSETSTATEMACHINENAMESPARAMS gssmn;
			static DWORD msgHash_GetEventTransitions = CHashString( _T("GetEventTransitions") ).GetUniqueID();
			DWORD result = m_pToolBox->SendMessage( msgHash_GetEventTransitions, sizeof( gssmn ),
													&gssmn, pTransition->GetParentName(), &hszCQHStateMachineEvent );
			if( MSG_HANDLED == result )
			{
				// extract string from pointers to IHashString in list to vector
				list<IHashString *>::const_iterator it = gssmn.Names.begin();
				for( ; it != gssmn.Names.end(); ++it )
				{
					if( *objectName != *( *it ) )
					{
						IArchive *data = GetEEObjectProperties( *it );
						StdString s;
						ar->Read( s, _T("name") );
						int i;
						ar->Read( i, _T("weight") );
						data->Close();
						if( i > 0 )
						{
							break;
						}
					}
				}
				// all event's transitions have zero weight
				if( it == gssmn.Names.end() )
				{
					CString msg = FormatString( IDS_VALIDATE_TRANSITION_BAD_WEIGHT );
					errors.push_back( CValidationError( objectName, msg ) );
					valid = false;
				}
			}

		}

		StdString targetState;
		ar->Read( targetState, _T("targetState") );
		if( targetState.empty() || !IsStateLabel( targetState ) )
		{
			CString msg = FormatString( IDS_VALIDATE_TRANSITION_BAD_TARGET_STATE, targetState.c_str() );
			errors.push_back( CValidationError( objectName, msg ) );
			valid = false;
		}
	}
	else
	{
		ASSERT( !"Unsupported version" );
	}

	return valid;
}

void CStateMachineValidator::FillStateLabels()
{
	if( m_StateLabels.empty() ) 
	{
		static CHashString hszCQHStateMachine = _T("CQHStateMachine");
		GETSETSTATEMACHINENAMESPARAMS gssmn;
		static DWORD msgHash_GetStateMachineStateLabels = CHashString( _T("GetStateMachineStateLabels") ).GetUniqueID();
		DWORD result = m_pToolBox->SendMessage( msgHash_GetStateMachineStateLabels, sizeof( gssmn ),
												&gssmn, &m_hszStateMachineName, &hszCQHStateMachine );
		if( MSG_HANDLED == result )
		{
			// extract string from pointers to IHashString in list to vector
			m_StateLabels.reserve( gssmn.Names.size() );
			transform( gssmn.Names.begin(), gssmn.Names.end(), 
					   back_inserter( m_StateLabels ), mem_fun( &IHashString::GetString ) );
			// and apply sorting
			sort( m_StateLabels.begin(), m_StateLabels.end() );
		}
	}
}

bool CStateMachineValidator::IsStateLabel( const StdString &label )
{
	if( label.empty() )
	{
		return false;
	}

	FillStateLabels();

	return binary_search( m_StateLabels.begin(), m_StateLabels.end(), label );
}

void CStateMachineValidator::FillStateNames( DWORD msg, IHashString *name, Labels &labels ) const
{
	GETSETSTATEMACHINENAMESPARAMS gssmn;
	static CHashString hszCQHState = _T("CQHState");
	DWORD result = m_pToolBox->SendMessage( msg, sizeof( gssmn ), &gssmn, name, &hszCQHState );
	if( MSG_HANDLED == result )
	{
		labels.clear();
		// extract string from pointers to IHashString in list to vector
		labels.reserve( gssmn.Names.size() );
		transform( gssmn.Names.begin(), gssmn.Names.end(), 
				   back_inserter( labels ), mem_fun( &IHashString::GetString ) );
		// and apply sorting
		sort( labels.begin(), labels.end() );
	}
}

void CStateMachineValidator::FillStateEvents( IHashString *name, Labels &events ) const
{
	static DWORD msgHash = CHashString( _T("GetStateEventLabels") ).GetUniqueID();
	FillStateNames( msgHash, name, events );
}

void CStateMachineValidator::FillStateAvailableEvents( IHashString *name, Labels &events ) const
{
	static DWORD msgHash = CHashString( _T("GetStateAvailableEventLabels") ).GetUniqueID();
	FillStateNames( msgHash, name, events );
}
