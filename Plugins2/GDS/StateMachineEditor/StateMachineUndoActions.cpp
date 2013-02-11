///=====================================================================
/// \file	StateMachineUndoActions.h
/// \brief	Declaration of actions for filling undo and redo stacks
/// \date	1/3/2008
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

/////////////// CStateMachineSelectionAction ///////////////////////////////////
///	\brief	CStateMachineSelectionAction constructor
///	\param	pEditor - pointer to assigned to action editor
CStateMachineSelectionAction::CStateMachineSelectionAction( CStateMachineDiagramEditor *pEditor )
	: m_pEditor( pEditor )
	, m_hashLayerEntity( 0 )
	, m_objs( pEditor->GetDiagramEntityContainer() )
{
	CDiagramEntity *pEntity = pEditor->GetCurrentLayerEntity();
	if( pEntity != NULL )
	{
		ASSERT( pEntity->GetType() == _T("CQHState") );
		m_hashLayerEntity = CHashString( pEntity->GetName() ).GetUniqueID();
	}
	int i = m_objs->GetSize();
	while( i-- )
	{
		static const DWORD hashState = CHashString( _T("CQHState") ).GetUniqueID();
		static const DWORD hashEvent = CHashString( _T("CQHStateMachineEvent") ).GetUniqueID();
		static const DWORD hashTransition = CHashString( _T("CQHStateMachineTransition") ).GetUniqueID();
		CDiagramEntity *pEntity = m_objs->GetAt( i );
		if( pEntity->IsSelected() )
		{
			DWORD name = CHashString( pEntity->GetName() ).GetUniqueID();
			m_selection.push_back( name );
		}
	}

	sort( m_selection.begin(), m_selection.end() );
}

///	\brief	execute command
///	\return	code of EE message result
DWORD CStateMachineSelectionAction::Execute()
{
	if( 0 == m_hashLayerEntity )
	{
		m_pEditor->SetCurrentDiagramLayer( m_objs, NULL );
	}
	else
	{
		CDiagramEntity *pEntity;
		CHashString hszName = m_pToolBox->GetHashString( m_hashLayerEntity );
		pEntity = m_pEditor->GetEntity( &hszName );
		pEntity->SetAsCurrentPage( m_pEditor );
	}

	CStateMachineEditor *pEditor = SINGLETONINSTANCE( CStateMachineEditor );
	CStateMachineToolBar *pToolBar = pEditor->GetToolBar();
	pToolBar->SetFlatMode( m_pEditor->IsFlatMode() );

	CDiagramEntityContainer *objs = m_pEditor->GetDiagramEntityContainer();
	int i = objs->GetSize();
	while( i-- )
	{
		CDiagramEntity *pEntity = m_objs->GetAt( i );
		DWORD name = CHashString( pEntity->GetName() ).GetUniqueID();
		if( binary_search( m_selection.begin(), m_selection.end(), name ) )
		{
			pEntity->Select( TRUE );
		}
		else
		{
			pEntity->Select( FALSE );
		}
	}

	m_pEditor->ShowProperties();
	m_pEditor->Invalidate();

	return MSG_HANDLED_PROCEED;
}

/////////////// CStateMachineAddEntityAction ///////////////////////////////////
///	\brief	CStateMachineSelectionAction constructor
///	\param	pEditor - pointer to assigned to action editor
///	\param	pParent - pointer to IHashString with parent name of the added EE object
///	\param	pName - pointer to IHashString with name of the added EE object
CStateMachineAddEntityAction::CStateMachineAddEntityAction( 
										CStateMachineDiagramEditor *pEditor,
										IHashString *pParent,
										IHashString *pName )
	: m_pEditor( pEditor )
	, m_hashParent( pParent->GetUniqueID() )
	, m_hashName( pName->GetUniqueID() )
{
}

///	\brief	execute command
///	\return	code of EE message result
DWORD CStateMachineAddEntityAction::Execute()
{
	ObjectInfo info;
	info.hszParent = m_pToolBox->GetHashString( m_hashParent );
	info.hszName = m_pToolBox->GetHashString( m_hashName );
	info.hszType = GetComponentType( &info.hszName );

	vector<ObjectInfo> objects;
	objects.push_back( info );
	m_pEditor->AddEntities( objects );
	
	// preparing parameters for create object 
	CREATEOBJECTPARAMS cop;
	cop.typeName = &info.hszType;
	cop.name = &info.hszName;
	cop.parentName = &info.hszParent;

	// send out a message that an object needs to be added
	static DWORD msgHash_AddObject = CHashString( _T("AddObject") ).GetUniqueID();
	m_pToolBox->SendMessage( msgHash_AddObject, sizeof( cop ), &cop );

	m_pEditor->SelectObject( &info.hszName );

	m_pEditor->Invalidate();

	return MSG_HANDLED_PROCEED;
}	

/////////////// CStateMachineRemoveEntityAction ////////////////////////////////
///	\brief	CStateMachineSelectionAction constructor
///	\param	pEditor - pointer to assigned to action editor
///	\param	pName - pointer to IHashString with name of the added EE object
CStateMachineRemoveEntityAction::CStateMachineRemoveEntityAction( 
						CStateMachineDiagramEditor *pEditor, IHashString *pName )
	: m_pEditor( pEditor )
	, m_hashName( pName->GetUniqueID() )
{
}

///	\brief	execute command
///	\return	code of EE message result
DWORD CStateMachineRemoveEntityAction::Execute()
{
	CHashString hszName = m_pToolBox->GetHashString( m_hashName );
	m_pEditor->SelectObject( &hszName );
	CDiagramEntity *pEntity = m_pEditor->GetEntity( &hszName );
	m_pEditor->RemoveObject( pEntity );

	m_pEditor->Invalidate();

	return MSG_HANDLED_PROCEED;
}

/////////////// CStateMachineChangeParentAction ////////////////////////////////
///	\brief	CStateMachineChangeParentAction constructor
///	\param	pEditor - pointer to assigned to action editor
///	\param	parent - pointer to IHashString with new parent name
CStateMachineChangeParentAction::CStateMachineChangeParentAction( 
											CStateMachineDiagramEditor *pEditor, 
											IHashString *parent  )
	: m_pEditor( pEditor )
	, m_parent( parent->GetUniqueID() )
{
}

///	\brief	execute command
///	\return	code of EE message result
DWORD CStateMachineChangeParentAction::Execute()
{
	IToolBox *pToolBox = EngineGetToolBox();
	CHashString hszParent = pToolBox->GetHashString( m_parent );

	CHANGEOBJECTPARENT cop;
	cop.newParentName = &hszParent;

	for( Hashes::const_iterator it = m_states.begin(); it != m_states.end(); ++it )
	{
		CHashString hszName = pToolBox->GetHashString( *it );
		CHashString hszType = GetComponentType( &hszName );
		cop.objectCompType = &hszType;
		cop.objectName = &hszName;
		static DWORD msgHash = CHashString( _T("ChangeObjectParent") ).GetUniqueID();
		DWORD res = pToolBox->SendMessage( msgHash, sizeof( cop ), &cop, &hszName, &hszType );
		if( MSG_HANDLED_PROCEED == res )
		{
			m_pEditor->RefreshHierarchy( &hszName );
		}
		else
		{
			pToolBox->Log( LOGERROR, _T("%s(%d): Failed to change object name\n"), __FILE__, __LINE__ );
		}
	}

	static DWORD msgHash_HierarchyChanged = CHashString( _T("HierarchyChanged") ).GetUniqueID();
	CHashString hszStateMachineName = m_pEditor->GetStateMachineName();
	GETSETACTIVESCENE gsas = { &hszStateMachineName };
	pToolBox->SendMessage( msgHash_HierarchyChanged, sizeof( gsas ), &gsas );

	return MSG_HANDLED_PROCEED;
}

///	\brief	register state name in states list
///	\param	state - pointer to IHassString with state name
void CStateMachineChangeParentAction::AddState( IHashString *state )
{
	ASSERT( state != NULL );
	ASSERT( !state->IsEmpty() );
	m_states.push_back( state->GetUniqueID() );
}

/////////////// CStateMachineSetAbstractAction /////////////////////////////////
///	\brief	execute command
///	\return	code of EE message result
DWORD CStateMachineSetAbstractAction::Execute()
{
	IToolBox *pToolBox = EngineGetToolBox();
	Names::const_iterator it = m_names.begin();
	for( ; it != m_names.end(); ++it )
	{
		CHashString hszName = pToolBox->GetHashString( *it );
		CHashString hszType = GetComponentType( &hszName );

		static DWORD msgHash = CHashString( _T("SetAbstract") ).GetUniqueID();
		pToolBox->SendMessage( msgHash, 0, NULL, &hszName, &hszType );
	}
	return MSG_HANDLED_PROCEED;
}

///	\brief	register object for setting as abstract
///	\param	name - pointer to IHashString with object name
void CStateMachineSetAbstractAction::AddObject( IHashString *name )
{
	m_names.push_back( name->GetUniqueID() );
}

/////////////// CStateMachineClearAction ///////////////////////////////////////
///	\brief	CStateMachineClearAction constructor
///	\param	stateMachineName - pointer to IHashString with state machine name
CStateMachineClearAction::CStateMachineClearAction( IHashString *stateMachineName )
	: m_stateMachineName( stateMachineName->GetUniqueID() )
{
}

///	\brief	execute command
///	\return	code of EE message result
DWORD CStateMachineClearAction::Execute()
{
	IToolBox *pToolBox = EngineGetToolBox();
	CHashString hszStateMachineName = pToolBox->GetHashString( m_stateMachineName );
	vector<IObject *> children;
	vector<IHashString *> topLevelStates;
	GetEEObjectChildren( &hszStateMachineName, children, true );
	vector<IObject *>::iterator it = children.begin();
	for( ; it != children.end(); ++it )
	{
		IObject *obj = *it;
		if( *obj->GetParentName() == hszStateMachineName )
		{
			topLevelStates.push_back( obj->GetName() );
		}
	}

	// delete top level states
	for_each( topLevelStates.begin(), topLevelStates.end(), DeleteEEObject );
	// entities in editor will be refreshed by CStateMachineRefreshHierarchyAction later

	return MSG_HANDLED_PROCEED;
}

/////////////// CStateMachineRefreshHierarchyAction ////////////////////////////
///	\brief	CStateMachineRefreshHierarchyAction constructor
///	\param	pEditor - pointer to editor to interact with
CStateMachineRefreshHierarchyAction::CStateMachineRefreshHierarchyAction( 
		CStateMachineDiagramEditor *pEditor )
	: m_pEditor( pEditor )
{
	ASSERT( m_pEditor != NULL );
}

///	\brief	execute command
///	\return	code of EE message result
DWORD CStateMachineRefreshHierarchyAction::Execute()
{
	m_pEditor->RefreshHierarchy();
	static DWORD msgHash_HierarchyChanged = CHashString( _T("HierarchyChanged") ).GetUniqueID();
	GETSETACTIVESCENE gsas = { m_pEditor->GetStateMachineName() };
	EngineGetToolBox()->SendMessage( msgHash_HierarchyChanged, sizeof( gsas ), &gsas );
	return MSG_HANDLED_PROCEED;
}