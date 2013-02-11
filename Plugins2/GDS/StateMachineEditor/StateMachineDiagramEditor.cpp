///============================================================================
/// \file		StateMachineDiagramEditor.cpp
/// \brief		Class implementation for CStateMachineDiagramEditor
/// \date		03-16-2007
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

///	This class sets desired entity as current in constructor and sets previous 
///	layer entity back in destructor
class CLayerSetter
{
public:
	CLayerSetter( CStateMachineDiagramEditor *pEditor, CDiagramEntity *pEntity )
		: m_pEditor( pEditor )
		, m_pEntity( pEditor->GetCurrentLayerEntity() )
		, m_pObjs( pEditor->GetDiagramEntityContainer() )
	{
		if( pEntity != NULL)
		{
			pEntity->SetAsCurrentPage( m_pEditor );
		}
		else
		{
			m_pEditor->SetCurrentDiagramLayerToRoot();
		}
	}

	~CLayerSetter()
	{
		m_pEditor->SetCurrentDiagramLayer( m_pObjs, m_pEntity );
	}

private:
	CStateMachineDiagramEditor *m_pEditor;
	CDiagramEntity *m_pEntity;
	CDiagramEntityContainer *m_pObjs;
};

CStateMachineDiagramEditor::CStateMachineDiagramEditor()
	: CDiagramEditor()
	, m_ToolBox( EngineGetToolBox() )
	, m_hszStateMachineType( _T("CQHStateMachine") )
	, m_hszStateType( _T("CQHState") )
	, m_hszEventType( _T("CQHStateMachineEvent") )
	, m_hszActionType( _T("CQHStateMachineActionHandler") )
	, m_hszTransitionType( _T("CQHStateMachineTransition") )
	, m_pDocument( NULL )
	, m_pStartState( NULL )
	, m_pEndState( NULL )
	, m_pHierarchyModeEntity( NULL )
	, m_pTransitionSource( NULL )
	, m_bTransitionDrawingMode( false )
	, m_pDrawingTransition( NULL )
	, m_pMousePlacement( new CDiagramEntity )
	, m_pClipboard( NULL )
	, m_pValidator( NULL )
	, m_bValid( true )
	, m_bIsCascadeUpdating( false )
	, m_bCreateObjectMode( false )
{
	SetPopupMenu( new CStateMachineDiagramMenu );
}

CStateMachineDiagramEditor::~CStateMachineDiagramEditor()
{
	RemoveAll();
	delete m_pMousePlacement;
	delete m_pClipboard;
	delete m_pValidator;
}

BEGIN_MESSAGE_MAP( CStateMachineDiagramEditor, CDiagramEditor )
	//{{AFX_MSG_MAP( CStateMachineDiagramEditor )
	//}}AFX_MSG_MAP

	ON_COMMAND( ID_EDITOR_BACKONELEVEL, OnLayerUp )
	ON_COMMAND( ID_EDITOR_GROUP, OnEditGroup )
	ON_COMMAND( ID_EDITOR_UNGROUP, OnEditUngroup )

	ON_COMMAND( ID_EDIT_UNDO, OnUndo )
	ON_UPDATE_COMMAND_UI( ID_EDIT_UNDO, OnUpdateUndo )
	ON_COMMAND( ID_EDIT_REDO, OnRedo )
	ON_UPDATE_COMMAND_UI( ID_EDIT_REDO, OnUpdateRedo )

	ON_COMMAND_RANGE( ID_ENTITY_UP, ID_ENTITY_UP + 500, OnObjectCommand )
END_MESSAGE_MAP()

void CStateMachineDiagramEditor::AddObject( CDiagramEntity *obj )
{
	AddObject( dynamic_cast<CSimpleDiagramEntity *>( obj ), true );
}

void CStateMachineDiagramEditor::AddObject( CSimpleDiagramEntity *obj, bool addToMap )
{
	if( addToMap )
	{
		ASSERT( !obj->GetName().IsEmpty() );
		DWORD hash = CHashString( obj->GetName() ).GetUniqueID();
		m_Entities.insert( make_pair( hash, obj ) );
	}

	SetupScrollbars();

	Base::AddObject( obj );
}

void CStateMachineDiagramEditor::AddObject( CDiagramEntity *pParentEntity, CSimpleDiagramEntity *pEntity, bool addToMap )
{
	CLayerSetter setter( this, pParentEntity );
	AddObject( pEntity, addToMap );
}

void CStateMachineDiagramEditor::AddFlatModeObject( CSimpleDiagramEntity *obj )
{
	m_FlatModeEntities.Add( obj );
}

///	\brief	delete all selected entities from the editor and EE hierarchy
void CStateMachineDiagramEditor::DeleteAllSelected()
{
	// we need to save selection before processing
	// some deleting objects may delete driven entities in the same container
	// transition entity does this for instance
	list<CString> selection;
	GetSelectedObjects( selection );

	if( !selection.empty() )
	{
		CUndoCommandGuard undoCommand;
		SaveUndoSelection();
		// now we can process each object in selection list
		for_each( selection.begin(), selection.end(),
				  bind1st( mem_fun( &CStateMachineDiagramEditor::DeleteObjectByName ), this) );

		SetupScrollbars();
		Invalidate();
	}
}

///	\brief	delete passed object and its children from EE hierarchy and from the editor
///	\param	pEntity - pointer to deleting object
void CStateMachineDiagramEditor::DeleteObject( CDiagramEntity *pEntity )
{
	ASSERT( pEntity != NULL );
	if( static_cast<CSimpleDiagramEntity *>( pEntity )->IsAbstract() )
	{
		// we cannot delete derived object
		return;
	}
	CLayerSetter setter( this, pEntity );

	CDiagramEntityContainer *objs = GetDiagramEntityContainer();
	if( objs != NULL )
	{
		while( objs->GetSize() > 0 )
		{
			DeleteObject( objs->GetAt( 0 ) );
		}
	}

	CHashString hszName = pEntity->GetName();

	// we have to delete entity from flat view while lead entity is in m_Entities
	// driven entity can reach lead entity and update its state
	CDiagramEntity *pFlatModeEntity = GetFlatModeEntity( hszName.GetString() );
	if( pFlatModeEntity != NULL )
	{
		m_FlatModeEntities.Remove( pFlatModeEntity );
	}

	Entities::iterator it = m_Entities.find( hszName.GetUniqueID() );
	if( it != m_Entities.end() )
	{
		CHashString hszType = pEntity->GetType();
		CHashString hszParent = GetParentName(pEntity);
		IUndoRedoAction *pAction;
		pAction = new CStateMachineRemoveEntityAction( this, &hszName );
		AddRedoAction( pAction );
		if( hszType == m_hszStateType )
		{
			DeleteTransitionsForState( pEntity );
		}

		pAction = new CStateMachineAddEntityAction( this, &hszParent, &hszName );
		AddUndoAction( pAction );
		UndoForDeleteObject( &hszName, &hszParent, &hszType );

		DeleteEEObject( &hszName );

		if( it->second != pEntity )
		{
			CDiagramEntity *pLead = it->second;
			m_Entities.erase( it );
			DeleteObject( pLead );
			RemoveEntity( pLead );
		}
		else
		{
			m_Entities.erase( it );
			RemoveEntity( pEntity );
		}
	}
}

///	\brief	delete passed object and its children from EE hierarchy and from the editor
///	\param	szName - name of the deleting object
void CStateMachineDiagramEditor::DeleteObjectByName( LPCTSTR szName )
{
	CHashString hszName = szName;
	CSimpleDiagramEntity *pEntity = GetEntity( &hszName );
	// Check if passed object exists. It may be deleted before
	if( pEntity != NULL )
	{
		DeleteObject( pEntity );
	}
}

///	\brief	delete transitions which refer to passed state
///	\param	pState - pointer to state entity
void CStateMachineDiagramEditor::DeleteTransitionsForState( CDiagramEntity *pState )
{
	CDiagramEntityContainer *objs = pState->GetParentContainer();
	int i = objs->GetSize();
	while( i-- )
	{
		CDiagramEntity *pEntity = objs->GetAt( i );
		CBaseTransitionDiagramEntity *pTransition;
		pTransition = dynamic_cast<CBaseTransitionDiagramEntity *>( pEntity );
		if( pTransition != NULL )
		{
			if( pState == pTransition->GetTarget() )
			{
				DeleteObjectByName( pTransition->GetName() );
			}
		}
	}
}

///	\brief	delete all entities in the editor
void CStateMachineDiagramEditor::RemoveAll()
{
	SetCurrentDiagramLayerToRoot();
	CDiagramEntityContainer *objs = GetDiagramEntityContainer();
	// container may be NULL if this method is called without ant added entities
	if( objs != NULL )
	{
		while( objs->GetSize() > 0 )
		{
			CDiagramEntity *pEntity = objs->GetAt( 0 );
			RemoveObject( pEntity );
		}
	}
}

///	\brief	remove passed entity from the editor
void CStateMachineDiagramEditor::RemoveObject( CDiagramEntity *pEntity )
{
	ASSERT( pEntity != NULL );
	CLayerSetter setter( this, pEntity );

	CDiagramEntityContainer *objs = GetDiagramEntityContainer();
	if( objs != NULL )
	{
		while( objs->GetSize() > 0 )
		{
			RemoveObject( objs->GetAt( 0 ) );
		}
	}

	CHashString hszName = pEntity->GetName();

	// we have to delete entity from flat view while lead entity is in m_Entities
	// driven entity can reach lead entity and update its state
	CDiagramEntity *pFlatModeEntity = GetFlatModeEntity( hszName.GetString() );
	if( pFlatModeEntity != NULL )
	{
		m_FlatModeEntities.Remove( pFlatModeEntity );
	}

	Entities::iterator it = m_Entities.find( hszName.GetUniqueID() );
	if( it != m_Entities.end() )
	{
		if( it->second != pEntity )
		{
			CDiagramEntity *pLead = it->second;
			m_Entities.erase( it );
			RemoveObject( pLead );
			RemoveEntity( pLead );
		}
		else
		{
			m_Entities.erase( it );
			RemoveEntity( pEntity );
		}
	}
}

///	\brief	remove passed entity from parent. Handles this event for other purposes
///	\param	pEntity - pointer to entity to remove
void CStateMachineDiagramEditor::RemoveEntity( CDiagramEntity *pEntity )
{
	ASSERT( pEntity != NULL );
	
	if( pEntity == m_pStartState )
	{
		m_pStartState = NULL;
	}
	if( pEntity == m_pEndState )
	{
		m_pEndState = NULL;
	}

	pEntity->GetParentContainer()->Remove( pEntity );
}

void CStateMachineDiagramEditor::ShowProperties()
{
	if( GetSelectCount() == 0 && GetCurrentLayerEntity() == NULL)
	{
		IHashString *stateMachineName = GetStateMachineName();
		if( stateMachineName != NULL )
		{
			SELECTOBJECTPARAMS sop;
			sop.object = stateMachineName;
			sop.compType = &m_hszStateMachineType;
			static DWORD msgHash_SelectObject = CHashString(_T("SelectObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SelectObject, sizeof(sop), &sop );
		}
	}
	else
	{
		Base::ShowProperties();
	}
}

CSize CStateMachineDiagramEditor::GetVirtualSize() const
{
	CRect rect( 0, 0, 0, 0 );
	if( m_hWnd )
	{
		GetClientRect( &rect );
	}

	CDiagramEntityContainer *objs = GetDiagramEntityContainer();
	if( objs )
	{
		// we take container's total size into account as well
		CSize bounds = GetContainingSize();
		if( bounds.cx < rect.right )
		{
			bounds.cx = rect.right;
		}
		if( bounds.cy < rect.bottom )
		{
			bounds.cy = rect.bottom;
		}
		CSize size = objs->GetVirtualSize();
		if( size.cx < bounds.cx )
		{
			size.cx = bounds.cx;
		}
		if( size.cy < bounds.cy )
		{
			size.cy = bounds.cy;
		}
		return size;
	}

	return rect.Size();
}

void CStateMachineDiagramEditor::MoveObject( CDiagramEntity* obj, const CRect& rect )
{
	if( 5 > rect.right || 5 > rect.bottom )
	{
		CRect rc = rect;
		if( 5 > rc.right )
		{
			rc.OffsetRect( -rc.right + 5, 0 );
		}
		if( 5 > rc.bottom )
		{
			rc.OffsetRect( 0, -rc.bottom + 5 );
		}
		Base::MoveObject( obj, rc );
	}
	else
	{
		Base::MoveObject( obj, rect );
	}
	SetupScrollbars();
}

void CStateMachineDiagramEditor::SetDocument( CStateMachineEditorDoc *pDocument )
{
	m_pDocument = pDocument;
}

void CStateMachineDiagramEditor::SetStateMachineName( IHashString *stateMachineName )
{
	m_StateMachineName = *stateMachineName;
	delete m_pValidator;
	m_pValidator = new CStateMachineValidator( GetStateMachineName() );
	UpdateValidate();
}

///	refreshes whole state machine hierarchy
void CStateMachineDiagramEditor::RefreshHierarchy()
{
	RemoveAll();
	ASSERT(m_Entities.empty());
	m_Entities.clear();
	DoRefreshHierarchy( GetStateMachineName() );
	UpdateStateMachineLabels();

	if( IsFlatMode() )
	{
		m_pHierarchyModeEntity = NULL;
		SetCurrentDiagramLayer( &m_FlatModeEntities, NULL );
	}
	else
	{
		SetCurrentDiagramLayerToRoot();
	}
}

///	refreshes hierarchy for passed object only
///	\param	name - name of changed object
void CStateMachineDiagramEditor::RefreshHierarchy( IHashString *name )
{
	RemoveObject( GetEntity( name ) );
	DoRefreshHierarchy( name );
	UpdateStateMachineLabels();
	SelectObject( name );
}

void CStateMachineDiagramEditor::DoRefreshHierarchy( IHashString *name )
{
	ASSERT( GetSafeHwnd() != NULL );

	vector<IObject *> children;
	if( MSG_HANDLED == GetEEObjectChildren( name, children, true ) )
	{
		vector<ObjectInfo> objects;
		objects.reserve( children.size() );
		vector<IObject *>::iterator it = children.begin();
		for( ; it != children.end(); ++it )
		{
			IObject *pObject = *it;
			objects.resize( objects.size() + 1 );
			ObjectInfo &info = objects.back();
			info.hszType = pObject->GetComponentType();
			info.hszName = pObject->GetName();
			if( pObject->GetParentName() != NULL )
			{
				info.hszParent = pObject->GetParentName();
			}
		}
		AddEntities( objects );
	}
}

///	\brief	set passed entity as current layer
///	\param	pEnity - pointer to layer entity. If NULL is passed as value 
///					 then root diagram layer set
void CStateMachineDiagramEditor::SetCurrentLayerEntity( CDiagramEntity *pEntity )
{
	if( pEntity != NULL)
	{
		pEntity->SetAsCurrentPage( this );
	}
	else
	{
		SetCurrentDiagramLayerToRoot();
	}
}

///	\brief	add new state to the editor
///	\param	pParentName - name of the parent object
///	\param	pName - name of the state object
CDiagramEntity *CStateMachineDiagramEditor::AddState( IHashString * pParentName, IHashString * pName )
{
	CDiagramEntity *pParent = GetEntity( pParentName );
	IHashString *pStateMachineName = GetStateMachineName();
	CSimpleDiagramEntity *pEntity = new CStateDiagramEntity( pParent, pStateMachineName );
	pEntity->SetName( pName->GetString() );
	AddObject( pParent, pEntity, true );

	CSimpleDiagramEntity *pFlatState = new CFlatStateModeDiagramEntity( NULL, pStateMachineName );
	pFlatState->SetName( pName->GetString() );
	AddFlatModeObject( pFlatState );
	return pEntity;
}

///	\brief	add new event to the editor
///	\param	pParentName - name of the parent object
///	\param	pName - name of the event object
///	\return	newly added event entity
CDiagramEntity *CStateMachineDiagramEditor::AddEvent( IHashString * pParentName, IHashString * pName )
{
	CDiagramEntity *pParent = GetEntity( pParentName );
	CSimpleDiagramEntity *pEntity = new CEventDiagramEntity( pParent );
	pEntity->SetName( pName->GetString() );
	AddObject( pParent, pEntity, true );
	return pEntity;
}

///	\brief	add new action to the editor
///	\param	pParentName - name of the parent object
///	\param	pName - name of the action object
void CStateMachineDiagramEditor::AddAction( IHashString * pParentName, IHashString * pName )
{
	CDiagramEntity *pParent = GetEntity( pParentName );
	CSimpleDiagramEntity *pEntity = new CSimpleDiagramEntity( pParent );
	pEntity->SetType( _T("CQHStateMachineActionHandler") );
	pEntity->SetName( pName->GetString() );
	AddObject( pParent, pEntity, true );
}

///	\brief	add new transition entity to the editor
///	\param	pParentName - name of the parent object
///	\param	pName - name of the transition object
void CStateMachineDiagramEditor::AddTransition( IHashString * pParentName, IHashString * pName )
{
	CDiagramEntity *pParent = GetEntity( pParentName );
	// this entity adds to entity container itself
	CSimpleDiagramEntity *pEntity = new CTransitionDiagramEntity( pParent, this );
	pEntity->SetName( pName->GetString() );
	AddObject( pParent, pEntity, true );
}

///	function adds a number of objects to editor. Entities are added in two 
/// passes: states and events first, then transitions are added. This allows
///	for transition to refer states deeper in hierarchy
///	\param	objects - list with object information to add
void CStateMachineDiagramEditor::AddEntities( vector<ObjectInfo> &objects )
{
	vector<ObjectInfo> transitions;
	vector<ObjectInfo>::iterator it = objects.begin();
	for( ; it != objects.end(); ++it )
	{
		static const DWORD hashState = m_hszStateType.GetUniqueID();
		static const DWORD hashEvent = m_hszEventType.GetUniqueID();
		static const DWORD hashAction = m_hszActionType.GetUniqueID();
		static const DWORD hashTransition = m_hszTransitionType.GetUniqueID();
		const DWORD hashType = it->hszType.GetUniqueID();
		if( hashState == hashType )
		{
			AddState( &(it->hszParent), &(it->hszName) );
		}
		else if( hashEvent == hashType )
		{
			AddEvent( &(it->hszParent), &(it->hszName) );
		}
		else if( hashTransition == hashType )
		{
			transitions.push_back( *it );
		}
		else if( hashAction == hashType )
		{
			AddAction( &(it->hszParent), &(it->hszName) );
		}
	}

	// create transition entities after states. 
	// Transition entity may refer to state below in hierarchy
	for( it = transitions.begin(); it != transitions.end(); ++it )
	{
		AddTransition( &(it->hszParent), &(it->hszName) );
	}
}

///	\brief	function select entities from objects list which are in current
///	editor layer. Objects are processed by their names
///	\param	objects - list with object information
void CStateMachineDiagramEditor::SelectEntities( vector<ObjectInfo> &objects )
{
	CDiagramEntityContainer *layerContainer = GetDiagramEntityContainer();
	for( vector<ObjectInfo>::const_iterator it = objects.begin(); it != objects.end(); ++it )
	{
		CDiagramEntity *pEntity = GetEntity( &(it->hszName) );
		if( pEntity->GetParentContainer() == layerContainer )
		{
			pEntity->Select( TRUE );
		}
	}
}

///	\brief	this function refreshes entity with passed object name hash
///	\param	name - hash of the object name
void CStateMachineDiagramEditor::RefreshEntity( IHashString *name )
{
	ASSERT( name != NULL );
	DWORD hashName = name->GetUniqueID();
	Entities::const_iterator it = m_Entities.find( hashName );
	if( it != m_Entities.end() )
	{
		CSimpleDiagramEntity *pEntity = it->second;
		const StdString oldLabel = pEntity->GetTitle();
		pEntity->LoadProperties();
		const StdString newLabel = pEntity->GetTitle();
		if( pEntity->GetType() == _T("CQHState") )
		{
			LPCTSTR szName = name->GetString();
			CDiagramEntity *p = GetFlatModeEntity( szName );
			CSimpleDiagramEntity *pFlatModeEntity;
			pFlatModeEntity = dynamic_cast<CSimpleDiagramEntity *>( p );
			ASSERT( pFlatModeEntity != NULL);
			pFlatModeEntity->LoadProperties();
			UpdateStateLabel( oldLabel, newLabel );
		}
		else if( pEntity->GetType() == _T("CQHStateMachineEvent") )
		{
			if( oldLabel != newLabel )
			{
				UpdateEventLabel( oldLabel, pEntity );
			}
		}
	}
	else if( hashName == GetStateMachineName()->GetUniqueID() )
	{
		RefreshStateMachine();
	}

	if( !m_bIsCascadeUpdating && m_hWnd != NULL )
	{
		SelectObject( name );
		Invalidate();
		UpdateValidate();
	}
}

///	\breif	check that any transition refers to state with passed label
///	\brief	label - checked state label
bool CStateMachineDiagramEditor::IsStateReferenced( const StdString &label ) const
{
	StdString startState;
	StdString endState;
	StdString parentStateMachineFile;
	IArchive *ar = GetEEObjectProperties( GetStateMachineName() );
	if( !ReadStateMachineProperties( ar, startState, endState, parentStateMachineFile ) )
	{
		ar->Close();
		return false;
	}
	ar->Close();

	if( startState == label || endState == label )
	{
		return true;
	}

	for( Entities::const_iterator it = m_Entities.begin(); it != m_Entities.end(); ++it )
	{
		if( it->second->IsStateReferred( label ) )
		{
			return true;
		}
	}
	return false;
}

///	this function refreshes all entities with passed labels. If old label is
///	equal to new label then this means that state references should now be 
///	updated. Nevertheless object should update own state based on passed label
/// because user may select to not update references
///	\param	oldLabel - old state label
///	\param	newLabel - new state label.
void CStateMachineDiagramEditor::UpdateStateLabel( const StdString &oldLabel, const StdString &newLabel )
{
	if( m_bIsCascadeUpdating )
	{
		return;
	}

	if( IsUndoExecuting() )
	{
		// undo action are executing now, so we do not need cascade update logic
		return;
	}

	StdString curLabel = newLabel;
	if ( oldLabel != curLabel && IsStateReferenced( oldLabel ) )
	{
		CString title = FormatString( IDS_CONFIRM );
		CString msg = FormatString( IDS_CONFIRM_STATE_RENAME );
		if( IDYES != MessageBox( msg, title, MB_YESNO | MB_ICONINFORMATION ) )
		{
			curLabel = oldLabel;
		}
	}

	CSimpleDiagramEntity *pState = GetStateByTitle( newLabel.c_str() );
	ASSERT( pState != NULL );
	CHashString hszStateName = pState->GetName();
	if( oldLabel != newLabel )
	{
		static DWORD msgHash_RedoSaveObject = CHashString( _T("RedoSaveObject") ).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_RedoSaveObject, sizeof( IHashString ), &hszStateName );
	}

	SaveUndoSelection();
	m_bIsCascadeUpdating = true;
	if( oldLabel != curLabel )
	{
		StdString startState;
		StdString endState;
		StdString parentStateMachineFile;
		IArchive *ar = GetEEObjectProperties( GetStateMachineName() );
		if( !ReadStateMachineProperties( ar, startState, endState, parentStateMachineFile ) )
		{
			ar->Close();
			return;
		}
		ar->Close();
		if( startState == oldLabel )
		{
			CObjectsStateUndo objectState( GetStateMachineName() );
			SetEEObjectProperty( GetStateMachineName(), _T("startState"), curLabel );
		}
		if( endState == oldLabel )
		{
			CObjectsStateUndo objectState( GetStateMachineName() );
			SetEEObjectProperty( GetStateMachineName(), _T("endState"), curLabel );
		}
	}

	for( Entities::iterator it = m_Entities.begin(); it != m_Entities.end(); ++it )
	{
		it->second->OnStateUpdate( oldLabel, curLabel );
	}

	if( oldLabel != newLabel )
	{
		SetEEObjectProperty( &hszStateName, _T("name"), oldLabel );
		static DWORD msgHash_UndoSaveObject = CHashString( _T("UndoSaveObject") ).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_UndoSaveObject, sizeof( IHashString ), &hszStateName );
		SetEEObjectProperty( &hszStateName, _T("name"), newLabel );
	}
	SaveRedoSelection();
	m_bIsCascadeUpdating = false;
}

///	\brief	find top state that contains event with passed label
///	\param	state - pointer to state for search
///	\param	label - event label to find
///	\return	top state which has event with passed label
CDiagramEntity *CStateMachineDiagramEditor::GetTopStateWithEvent( CDiagramEntity *state, const StdString &label ) const
{
	if( state == NULL || state->GetParentEntity() == NULL )
	{
		return NULL;
	}

	CDiagramEntity *top = NULL;
	while( state != NULL )
	{
		CDiagramEntityContainer *objs = state->GetParentContainer();
		state = state->GetParentEntity();
		int i = objs->GetSize();
		while( i-- )
		{
			CDiagramEntity *entity = objs->GetAt( i );
			ASSERT( entity != NULL );
			if( entity->GetTitle() == label.c_str() && 
				entity->GetType() == _T("CQHStateMachineEvent") )
			{
				top = state;
				break;
			}
		}
	}
	return top;
}

///	\brief	collect entities which refer to event with passed label
///	\param	state - pointer to state for visit
///	\param	label - event label to find
///	\param	states - list with states entities
///	\param	events - list with event entities
///	\return	top state which has event with passed label
void CStateMachineDiagramEditor::CollectEventReferences( CSimpleDiagramEntity *state,
														 const StdString &label,
														 vector<CSimpleDiagramEntity *> &states, 
														 vector<CSimpleDiagramEntity *> &events )
{
	// save current layer data
	ASSERT( state->GetType() == _T("CQHState") );
	StdString entryEvent;
	StdString updateEvent;
	StdString exitEvent;
	GetStateEventReferences( state, entryEvent, updateEvent, exitEvent );
	if( label == entryEvent ||
		label == updateEvent ||
		label == exitEvent )
	{
		states.push_back( state );
	}

	// the set passed state as current layer
	CLayerSetter setter( this, state );
	CDiagramEntityContainer *objs = GetDiagramEntityContainer();
	ASSERT( objs != NULL );
	int i = objs->GetSize();
	while( i-- )
	{
		// process all children
		CSimpleDiagramEntity *entity;
		entity = static_cast<CSimpleDiagramEntity *>( objs->GetAt( i ) );
		CString type = entity->GetType();
		if( type == _T("CQHStateMachineEvent") )
		{
			if( entity->GetTitle() == label.c_str() )
			{
				events.push_back( entity );
			}
		}
		else if( type == _T("CQHState") )
		{
			CollectEventReferences( entity, label, states, events );
		}
	}
}

///	\brief	extract event names from passed state
///	\param	state - pointer to state entity
///	\param	entryEvent [out] - name of entry event
///	\param	updateEvent [out] - name of exit event
void CStateMachineDiagramEditor::GetStateEventReferences( CSimpleDiagramEntity *state,
														  StdString &entryEvent,
														  StdString &updateEvent,
														  StdString &exitEvent )
{
	IAttributeObject *attrObject = state->GetAttributeObject();
	IArchive *ar = CreateMemoryArchive();
	static CHashString hszEntryEvent = _T("entryEvent");
	static CHashString hszUpdateEvent = _T("updateEvent");
	static CHashString hszExitEvent = _T("exitEvent");
	attrObject->GetAttribute( &hszEntryEvent, ar );
	attrObject->GetAttribute( &hszUpdateEvent, ar );
	attrObject->GetAttribute( &hszExitEvent, ar );
	ar->SetIsWriting( false );
	ar->SeekTo( 0 );
	ar->Read( entryEvent, _T("entryEvent") );
	ar->Read( updateEvent, _T("updateEvent") );
	ar->Read( exitEvent, _T("exitEvent") );
	ar->Close();
}

///	\brief	this function refreshes event with passed label
///	\sa		UpdateStateLabel
///	\param	oldLabel - old event label
///	\param	event - pointer to renamed event
void CStateMachineDiagramEditor::UpdateEventLabel( const StdString &oldLabel, CSimpleDiagramEntity *event )
{
	if( m_bIsCascadeUpdating )
	{
		// prevent from recursive event update
		return;
	}

	if( IsUndoExecuting() )
	{
		// undo action are executing now, so we do not need cascade update logic
		return;
	}
	ASSERT( event != NULL );
	CDiagramEntity *parent = event->GetParentEntity();
	CDiagramEntity *topState = GetTopStateWithEvent( parent, oldLabel );
	if( topState == NULL )
	{
		topState = parent;
	}

	vector<CSimpleDiagramEntity *> states;
	vector<CSimpleDiagramEntity *> events;
	CollectEventReferences( static_cast<CSimpleDiagramEntity *>( topState ), oldLabel, states, events );

	if( events.empty() && states.empty() )
	{
		// noting to update
		return;
	}

	CString title = FormatString( IDS_CONFIRM );
	CString msg = FormatString( IDS_CONFIRM_EVENT_RENAME );
	if( IDYES != MessageBox( msg, title, MB_YESNO | MB_ICONINFORMATION ) )
	{
		return;
	}

	m_bIsCascadeUpdating = true;

	SaveUndoSelection();

	StdString newLabel = event->GetTitle();
	vector<CSimpleDiagramEntity *>::iterator it = states.begin();
	for( ; it != states.end(); ++it )
	{
		CHashString hszName = (*it)->GetName();
		StdString entryEvent;
		StdString updateEvent;
		StdString exitEvent;
		GetStateEventReferences( *it, entryEvent, updateEvent, exitEvent );
		bool isEntry = ( oldLabel == entryEvent );
		bool isUpdate = ( oldLabel == updateEvent );
		bool isExit = ( oldLabel == exitEvent );
		if( isEntry || isUpdate || isExit )
		{
			CObjectsStateUndo objectProperty( &hszName );
			if( isEntry )
			{
				SetEEObjectProperty( &hszName, _T("entryEvent"), newLabel );
			}
			if( isUpdate )
			{
				SetEEObjectProperty( &hszName, _T("updateEvent"), newLabel );
			}
			if( isExit )
			{
				SetEEObjectProperty( &hszName, _T("exitEvent"), newLabel );
			}
		}
	}

	for( it = events.begin(); it != events.end(); ++it )
	{
		CHashString hszName = (*it)->GetName();
		(*it)->SetTitle( newLabel.c_str() );

		CObjectsStateUndo objectProperty( &hszName );

		SetEEObjectProperty( &hszName, _T("name"), newLabel );
	}

	SaveRedoSelection();
	m_bIsCascadeUpdating = false;
}

///	\brief	find existing entity by object name
///	\param	pName - pointer to IHashString with object name
///	\return pointer to found entity or NULL otherwise
CSimpleDiagramEntity *CStateMachineDiagramEditor::GetEntity( const IHashString *pName ) const
{
	if( pName != NULL && !pName->IsEmpty() )
	{
		const DWORD hash = pName->GetUniqueID();
		Entities::const_iterator it = m_Entities.find( hash );
		if( it != m_Entities.end() )
		{
			return it->second;
		}
	}
	return NULL;
}

///	\brief	find existing entity by object name in flat mode container
///	\param	szName - string with object name
///	\return pointer to found entity or NULL otherwise
CDiagramEntity *CStateMachineDiagramEditor::GetFlatModeEntity( LPCTSTR szName ) const
{
	int i = m_FlatModeEntities.GetSize();
	while( i-- )
	{
		CDiagramEntity *pEntity = m_FlatModeEntities.GetAt( i );
		ASSERT( pEntity != NULL );
		if( pEntity->GetName() == szName )
		{
			return pEntity;
		}
	}
	return NULL;
}

///	\brief	find existing state entity by entity title
///	\param	szTitle - title of the entity
///	\return pointer to found entity or NULL otherwise
CSimpleDiagramEntity *CStateMachineDiagramEditor::GetStateByTitle( LPCTSTR szTitle ) const
{
	if( szTitle != NULL && *szTitle != _T('\0') )
	{
		Entities::const_iterator it = m_Entities.begin();
		for( ; it != m_Entities.end(); ++it )
		{
			if( it->second->GetTitle() == szTitle &&
				it->second->GetType() == _T("CQHState") )
			{
				return it->second;
			}
		}
	}
	return NULL;
}

///	\brief	this function select entity. It switches layer also
///	\param	name - hash of the object name
void CStateMachineDiagramEditor::SelectObject( const IHashString *name )
{
	if( name == NULL )
	{
		UnselectAll();
		return;
	}

	if( 1 == GetSelectCount() &&
		GetSelectedObject()->GetName() == name->GetString() )
	{
		// this object is selected already
		return;
	}

	CDiagramEntity *pEntity = NULL;
	if( IsFlatMode() )
	{
		pEntity = GetFlatModeEntity( name->GetString() );
	}
	else
	{
		pEntity = GetEntity( name );
	}

	if( pEntity != NULL && !pEntity->IsSelected() )
	{
		if( GetCurrentLayerEntity() == pEntity ||
			GetDiagramEntityContainer() == pEntity->GetParentContainer() )
		{
			UnselectAll();
			pEntity->Select( TRUE );
		}
		else if( pEntity->GetType() == _T("CQHStateMachineTransition") )
		{
			ASSERT( !IsFlatMode() );
			// we get parent event then parent state for this event
			CDiagramEntity *parent = pEntity->GetParentEntity();
			ASSERT( parent != NULL );
			CDiagramEntityContainer *objs = parent->GetParentContainer();
			ASSERT( objs != NULL );
			// finding drawing entity for current layer
			for( int i = objs->GetSize() - 1; i >= 0; --i )
			{
				pEntity = objs->GetAt( i );
				if( pEntity->GetName() == name->GetString() )
				{
					// select state layer
					ASSERT( parent->GetParentEntity() != NULL );
					parent->GetParentEntity()->SetAsCurrentPage( this );
					UnselectAll();
					// and select transition line
					pEntity->Select( TRUE );
					break;
				}
			}
		}
		else if( pEntity->GetType() == _T("CQHStateMachineActionHandler") )
		{
			ASSERT( !IsFlatMode() );
			// we get parent event then parent state for this event
			CDiagramEntity *event = pEntity->GetParentEntity();
			ASSERT( event != NULL );
			event->GetParentEntity()->SetAsCurrentPage( this );
			UnselectAll();
			// and select event
			event->Select( TRUE );
		}
		else
		{
			ASSERT( !IsFlatMode() );
			SetCurrentLayerEntity( pEntity->GetParentEntity() );
			UnselectAll();
			pEntity->Select( TRUE );
		}

		Invalidate();
	}
}

///	\return true if all state machine objects are valid
bool CStateMachineDiagramEditor::IsValid() const
{
	return m_bValid;
}

///	\brief	update state machine objects errors flag
///	\return true if all state machine objects are valid
bool CStateMachineDiagramEditor::UpdateValidate()
{
	ASSERT( m_pValidator != NULL );
	ASSERT( GetStateMachineName() != NULL );
	list<CValidationError> errors;
	m_bValid = m_pValidator->Validate( errors );
	return m_bValid;
}

///	\brief	check state machine objects for errors and show message box with
///			text if error exists
void CStateMachineDiagramEditor::Validate()
{
	ASSERT( !m_bValid );
	ASSERT( m_pValidator != NULL );
	ASSERT( GetStateMachineName() != NULL );
	list<CValidationError> errors;
	m_bValid = m_pValidator->Validate( errors );
	if( !m_bValid )
	{
		ASSERT( !errors.empty() );
		const IHashString *name = errors.front().GetName();
		LPCTSTR message = errors.front().GetMessage();
		CString title = FormatString( IDS_ERROR );
		SelectObject( name );
		ShowProperties();
		MessageBox( message, title, MB_ICONERROR );
	}
}

///	\brief	validate passed value for object
///	\param	sop - pointer to SERIALIZEOBJECTPARAMS with object name and archive
///	\return	MSG_ERROR - if passed data is not valid for object
///			MSG_HANDLED - if passed data is valid for object
///			MSG_NOT_HANDLED - if object data was not processed
DWORD CStateMachineDiagramEditor::ValidateObject( SERIALIZEOBJECTPARAMS *sop )
{
	// For now, skip this assert since it is being triggered on the creation of a state machine
	// ASSERT( m_pValidator != NULL );
	if( m_pValidator != NULL )
	{
		ASSERT( GetStateMachineName() != NULL );
		bool isStateMachine = *GetStateMachineName() == *sop->name;
		if( ( isStateMachine || GetEntity( sop->name ) != NULL ) && 
			!IsUndoExecuting() )
		{
			list<CValidationError> errors;
			m_pValidator->ResetCaches();
			if( !m_pValidator->ValidateObject( sop, errors ) )
			{
				ASSERT( !errors.empty() );
				CString title = FormatString( IDS_ERROR );
				CWnd *pFocus = GetFocus();
				MessageBox( errors.front().GetMessage(), title, MB_ICONERROR );
				if( pFocus != NULL )
				{
					pFocus->SetFocus();
				}
				return MSG_ERROR;
			}
			if( isStateMachine )
			{
				if( !ProcessStateMachineChanges( sop->archive ) )
				{
					// user stopped state machine changes
					return MSG_ERROR;
				}
			}

			return MSG_HANDLED_PROCEED;
		}
	}
	
	return MSG_NOT_HANDLED;
}

void CStateMachineDiagramEditor::LayerUp()
{
	EndTransitionDrawing();
	Base::LayerUp();
	SetupScrollbars();
	ShowProperties();
}

void CStateMachineDiagramEditor::OnLButtonDown( UINT nFlags, CPoint point )
{
	if( m_bTransitionDrawingMode )
	{
		m_pMousePlacement->SetRect( CRect( point, point ) );
		CDiagramEntity *pEntity = GetEntity( point );
		if( pEntity == NULL )
		{
			// user clicked somewhere so stop adding transition
			m_pTransitionSource = NULL;
		}
		else if( m_pTransitionSource != NULL )
		{
			ConfirmTransitionDrawing( point );
			return;
		}
		else
		{
			m_pTransitionSource = dynamic_cast<CEventDiagramEntity *>( pEntity );
			if( m_pTransitionSource != NULL && 
				!m_pTransitionSource->IsAbstract() )
			{
				// start transition drawing from clicked event
				m_pDrawingTransition = new CDraggingTransitionDiagramEntity( GetCurrentLayerEntity(), m_pTransitionSource, m_pMousePlacement );
				AddObject( GetCurrentLayerEntity(), m_pDrawingTransition, false );
				m_pDrawingTransition->GetParentContainer()->Front( m_pDrawingTransition );
				// we cannot add transition from event to event anyway
				SetCursor( LoadCursor( NULL, IDC_NO) );
				return;
			}
		}
	}
	else
	{
		m_pTransitionSource = NULL;
	}

	EndTransitionDrawing();

	m_DownClickPoint = point;
	Base::OnLButtonDown( nFlags, point );
	int interactMode = GetInteractMode();
	if( !m_bCreateObjectMode && 
		( interactMode == MODE_MOVING || interactMode == MODE_RESIZING ) )
	{
		list<CString> selection;
		GetSelectedObjects( selection );
		if( !selection.empty() )
		{
			m_UndoGroup.StartUndoCommand();
			SaveUndoSelection();
			list<CString>::const_iterator it = selection.begin();
			for( ; it != selection.end() ; ++it )
			{
				CHashString hszName = *it;
				m_UndoGroup.AddUndoObject( &hszName );
			}
		}
	}
}

void CStateMachineDiagramEditor::OnLButtonUp( UINT nFlags, CPoint point )
{
	int interactMode = GetInteractMode();
	if( m_pDrawingTransition != NULL )
	{
		ConfirmTransitionDrawing( point );
		m_pMousePlacement->SetRect( CRect( point, point ) );
		Invalidate();
	}
	else if( !IsFlatMode() && interactMode == MODE_MOVING )
	{
		vector<CStateDiagramEntity *> states;
		CDiagramEntity *parentState = NULL;
		parentState = GetMovingStates( point, states );

		if( parentState != NULL && !states.empty() )
		{
			CString msg = FormatString( IDS_CONFIRM_STATE_DROP, 
										 (LPCTSTR)parentState->GetTitle() );
			CString title = FormatString( IDS_CONFIRM );
			if( IDYES != MessageBox( msg, title, MB_YESNO | MB_ICONINFORMATION ) )
			{
				parentState = NULL;
			}
		}

		Base::OnLButtonUp( nFlags, point );

		if( parentState != NULL && !states.empty() )
		{
			CHashString hszNewParentName = parentState->GetName();
			CHashString hszStateMachineName = GetStateMachineName();
			CHashString hszParentName;
			if( parentState->GetParentEntity() != NULL )
			{
				hszParentName = parentState->GetParentEntity()->GetName();
			}
			else
			{
				hszParentName = hszStateMachineName;
			}

			CStateMachineChangeParentAction *pUndoAction = NULL;
			pUndoAction = new CStateMachineChangeParentAction( this, &hszParentName );
			CStateMachineChangeParentAction *pRedoAction = NULL;
			pRedoAction = new CStateMachineChangeParentAction( this, &hszNewParentName );

			AddRedoAction( pRedoAction );

			vector<CStateDiagramEntity *>::iterator it = states.begin();
			for( ; it != states.end(); ++it )
			{
				CHashString hszName = (*it)->GetName();
				(*it)->SetParentName( &hszNewParentName, this );
				pUndoAction->AddState( &hszName );
				pRedoAction->AddState( &hszName );
			}

			AddUndoAction( pUndoAction );

			Invalidate();

			static DWORD msgHash_HierarchyChanged = CHashString( _T("HierarchyChanged") ).GetUniqueID();
			GETSETACTIVESCENE gsas = { &hszStateMachineName };
			m_ToolBox->SendMessage( msgHash_HierarchyChanged, sizeof( gsas ), &gsas );
		}
	}
	else
	{
		Base::OnLButtonUp( nFlags, point );
	}

	if( m_bCreateObjectMode )
	{
		EndCreateObjectMode();
	}
	else if( interactMode == MODE_MOVING || interactMode == MODE_RESIZING )
	{
		if( m_DownClickPoint != point )
		{
			SaveRedoSelection();
			m_UndoGroup.EndUndoCommand();
		}
		else
		{
			m_UndoGroup.CancelUndoCommand();
		}
	}
}

void CStateMachineDiagramEditor::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	Base::OnLButtonDblClk( nFlags, point );
	ShowProperties();
}

void CStateMachineDiagramEditor::OnMouseMove( UINT nFlags, CPoint point )
{
	if( m_pDrawingTransition != NULL )
	{
		m_pMousePlacement->SetRect( CRect( point, point ) );
		CDiagramEntity *pEntity = GetEntity( point );
		if( NULL != dynamic_cast<CStateDiagramEntity *>( pEntity ) )
		{
			SetCursor( LoadCursor( NULL, IDC_CROSS ) );
		}
		else
		{
			SetCursor( LoadCursor( NULL, IDC_NO ) );
		}
		Invalidate();
	}
	else if( !IsFlatMode() && GetInteractMode() == MODE_MOVING )
	{
		vector<CStateDiagramEntity *> states;
		CDiagramEntity *parentState = NULL;
		parentState = GetMovingStates( point, states );

		if( parentState != NULL && !states.empty() )
		{
			SetCursor( LoadCursor( NULL, IDC_HAND ) );
		}
		else
		{
			SetCursor( LoadCursor( NULL, IDC_SIZEALL ) );
		}
		Base::OnMouseMove( nFlags, point );
	}
	else
	{
		Base::OnMouseMove( nFlags, point );
	}
}

void CStateMachineDiagramEditor::OnRButtonDown(UINT nFlags, CPoint point)
{
	EndTransitionDrawing();
	Base::OnRButtonDown( nFlags, point );
}

void CStateMachineDiagramEditor::OnRButtonUp( UINT nFlags, CPoint point )
{
	if( m_bCreateObjectMode )
	{
		EndCreateObjectMode();
	}
	Base::OnRButtonUp( nFlags, point );
}

void CStateMachineDiagramEditor::EndCreateObjectMode()
{
	ASSERT( m_bCreateObjectMode );
	ASSERT( GetSelectCount() == 1 );
	CDiagramEntity *pAddedObject = GetSelectedObject();
	CHashString hszParent = GetParentName( pAddedObject ) ;
	CHashString hszName = pAddedObject->GetName();
	CHashString hszType = pAddedObject->GetType();
	CUndoCommandGuard undoCommand;
	UndoForCreateObject( &hszParent, &hszName, &hszType );
	IUndoRedoAction *pAction;
	pAction = new CStateMachineRemoveEntityAction( this, &hszName );
	AddUndoAction( pAction );
	pAction = new CStateMachineAddEntityAction( this, &hszParent, &hszName );
	AddRedoAction( pAction );
	m_bCreateObjectMode = false;
}

///	\brief	collect selected states and find top state entity under passed point
CDiagramEntity *CStateMachineDiagramEditor::GetMovingStates( CPoint point, vector<CStateDiagramEntity *> &states )
{
	CDiagramEntity * parentState = NULL;
	CDiagramEntityContainer *objs = GetDiagramEntityContainer();
	int i = objs->GetSize();
	while( i-- )
	{
		CDiagramEntity *entity = objs->GetAt( i );
		if( entity->IsSelected() )
		{
			if( entity->GetType() == _T("CQHState") )
			{
				CStateDiagramEntity *state;
				state = dynamic_cast<CStateDiagramEntity *>( entity );
				ASSERT( state != NULL );
				if( state->IsAbstract() )
				{
					// we cannot change parent for abstract states
					states.clear();
					return NULL;
				}
				states.push_back( state );
			}
			else
			{
				// at least one none-state entity is selected
				// we may change parent for states only
				states.clear();
				return NULL;
			}
		}
		else if( parentState == NULL &&
				 DEHT_BODY == entity->GetHitCode( point ) )
		{
			if( entity->GetType() == _T("CQHState") )
			{
				parentState = entity;
			}
			else
			{
				states.clear();
				return NULL;
			}
		}
	}
	return parentState;
}

///	\brief	switch editor to transition drawing mode or back
///	\param	drawing - defines mode for editor. false cancels current drawing
void CStateMachineDiagramEditor::SetTransitionDrawing( bool drawing )
{
	if( m_bTransitionDrawingMode != drawing)
	{
		m_bTransitionDrawingMode = drawing;
		if( m_bTransitionDrawingMode )
		{
			UnselectAll();
		}
		else
		{
			EndTransitionDrawing();
		}
		Invalidate();
	}
}

///	\brief	switch editor between hierarchical and flat modes
///	\param	flat - true switches editor to flat mode, false - to hierarchical
void CStateMachineDiagramEditor::SetFlatMode( bool flat )
{
	if( flat )
	{
		CDiagramEntityContainer *objs = GetDiagramEntityContainer();
		if( objs != &m_FlatModeEntities )
		{
			m_pHierarchyModeEntity = GetCurrentLayerEntity();
		}
		SetCurrentDiagramLayer( &m_FlatModeEntities, NULL );
	}
	else
	{
		SetCurrentLayerEntity( m_pHierarchyModeEntity );
	}
	SetupScrollbars();
	Invalidate();
}

bool CStateMachineDiagramEditor::IsFlatMode() const
{
	return( GetDiagramEntityContainer() == &m_FlatModeEntities );
}

void CStateMachineDiagramEditor::ConfirmTransitionDrawing( const CPoint &point )
{
	ASSERT( m_pTransitionSource != NULL );

	CDiagramEntity *pEntity = GetEntity( point );
	if( pEntity == NULL )
	{
		return;
	}

	CStateDiagramEntity *pState = dynamic_cast<CStateDiagramEntity *>( pEntity );
	// creating transition from m_pTransitionSource to pState
	if( pState != NULL )
	{
		ASSERT( m_pDocument != NULL );
		CHashString hszParent = m_pTransitionSource->GetName();
		CHashString hszName = m_pDocument->CreateTransition( &hszParent, pState->GetTitle() );
		AddTransition( &hszParent, &hszName );

		// saving information for undo and redo operations
		CUndoCommandGuard undoCommand;
		UndoForCreateObject( &hszParent, &hszName, &m_hszTransitionType );
		IUndoRedoAction *pAction;
		pAction = new CStateMachineRemoveEntityAction( this, &hszName );
		AddUndoAction( pAction );
		pAction = new CStateMachineAddEntityAction( this, &hszParent, &hszName );
		AddRedoAction( pAction );

		EndTransitionDrawing();
	}
}

void CStateMachineDiagramEditor::EndTransitionDrawing()
{
	m_pTransitionSource = NULL;

	if( m_pDrawingTransition != NULL )
	{
		RemoveEntity( m_pDrawingTransition );
		m_pDrawingTransition = NULL;
		SetCursor( LoadCursor( NULL, IDC_ARROW ) );
	}
}

///	\brief	find entity under point. Function finds in current entity container
///	\param	point - screen coordinates to check
///	\return	pointer to found entity or NULL
CDiagramEntity *CStateMachineDiagramEditor::GetEntity( const CPoint &point ) const
{
	CDiagramEntityContainer *objs = GetDiagramEntityContainer();
	if( objs != NULL )
	{
		CPoint virtpoint( point );
		ScreenToVirtual( virtpoint );

		int i = objs->GetSize();
		while( i-- )
		{
			CDiagramEntity *pEntity = objs->GetAt( i );
			if( pEntity->GetHitCode( virtpoint ) == DEHT_BODY )
			{
				return pEntity;
			}
		}
	}
	return NULL;
}

void CStateMachineDiagramEditor::OnObjectCommand( UINT nID )
{
	switch ( nID )
	{
		case CMD_COPY:
			Copy();
			break;
		case CMD_CUT:
			Cut();
			break;
		default:
			Base::OnObjectCommand( nID );
	}
}

void CStateMachineDiagramEditor::Cut()
{
	ASSERT( !IsFlatMode() );
	ASSERT( m_pClipboard != NULL );
	Copy();
	DeleteAllSelected();
}

void CStateMachineDiagramEditor::Copy()
{
	ASSERT( !IsFlatMode() );
	ASSERT( m_pClipboard != NULL );
	list<CString> names;

	GetSelectedObjects( names );

	if( !m_pClipboard->Copy( names ) )
	{
		MessageBox( FormatString( IDS_ERR_COPY_FAILED ), FormatString( IDS_ERROR ), MB_ICONERROR );
	}
}

void CStateMachineDiagramEditor::Paste()
{
	if( CanPaste() )
	{
		CDiagramEntity *pEntity = GetCurrentLayerEntity();
		CHashString hszParent;
		if( pEntity == NULL )
		{
			hszParent = GetStateMachineName();
		}
		else
		{
			hszParent = pEntity->GetName();
		}

		if( m_pClipboard->CanPastePartially( &hszParent ) )
		{
			CString strConfirmation = FormatString( IDS_CONFIRM_PASTE );
			if( IDYES != MessageBox( strConfirmation, FormatString( IDS_CONFIRM ), MB_ICONQUESTION | MB_YESNO ) )
			{
				// user has canceled the operation
				return;
			}
		}

		vector<ObjectInfo> objects;
		if( m_pClipboard->Paste( &hszParent, objects ) )
		{
			if( !objects.empty() )
			{
				AddEntities( objects );
				UnselectAll();
				// select entities, pasted to the current level 
				SelectEntities( objects );
				Invalidate();
			}
		}
		else
		{
			MessageBox( FormatString( IDS_ERR_PASTE_FAILED ), FormatString( IDS_ERROR ), MB_ICONERROR );
		}
	}
}

bool CStateMachineDiagramEditor::CanPaste() const
{
	ASSERT( m_pClipboard != NULL );
	return !IsFlatMode() && m_pClipboard->CanPaste();
}

BOOL CStateMachineDiagramEditor::Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, CDiagramEntityContainer* data )
{
	BOOL bRes = Base::Create( dwStyle, rect, pParentWnd, data );
	if( bRes )
	{
		m_pClipboard = new CStateMachineClipboard( GetSafeHwnd() );
	}
	return bRes;
}

void CStateMachineDiagramEditor::AddNewState()
{
	ASSERT( !IsFlatMode() );
	ASSERT( !m_bCreateObjectMode );
	m_bCreateObjectMode = true;

	CDiagramEntity *currentLayer = GetCurrentLayerEntity();
	CHashString	hszParent;
	if( currentLayer != NULL )
	{
		hszParent = currentLayer->GetName();
	}
	else
	{
		hszParent = GetStateMachineName();
	}

	ASSERT( m_pDocument != NULL );
	CHashString hszName = m_pDocument->CreateState( &hszParent );

	UnselectAll();
	CDiagramEntity *pState = AddState( &hszParent, &hszName );
	ASSERT( pState != NULL );
	pState->Select( TRUE );

	// start entity moving
	OnLButtonDown( 0, pState->GetRect().CenterPoint() );
}

void CStateMachineDiagramEditor::AddNewEvent()
{
	ASSERT( !IsFlatMode() );
	ASSERT( !m_bCreateObjectMode );
	m_bCreateObjectMode = true;

	CDiagramEntity *currentLayer = GetCurrentLayerEntity();
	ASSERT( currentLayer != NULL );
	CHashString	hszParent = currentLayer->GetName();

	ASSERT( m_pDocument != NULL );
	CHashString hszName = m_pDocument->CreateEvent( &hszParent );

	UnselectAll();
	CDiagramEntity *pEvent = AddEvent( &hszParent, &hszName );
	ASSERT( pEvent != NULL );
	pEvent->Select( TRUE );

	// start entity moving
	OnLButtonDown( 0, pEvent->GetRect().CenterPoint() );
}

void CStateMachineDiagramEditor::AddEntryEvent()
{
	ASSERT( !IsFlatMode() );
	ASSERT( GetCurrentLayerEntity() != NULL );
	ASSERT( GetCurrentLayerEntity()->GetType() == _T("CQHState") );
	CStateDiagramEntity *state = (CStateDiagramEntity *)GetCurrentLayerEntity();
	state->AddEntryEvent( this );
}

void CStateMachineDiagramEditor::AddUpdateEvent()
{
	ASSERT( !IsFlatMode() );
	ASSERT( GetCurrentLayerEntity() != NULL );
	ASSERT( GetCurrentLayerEntity()->GetType() == _T("CQHState") );
	CStateDiagramEntity *state = (CStateDiagramEntity *)GetCurrentLayerEntity();
	state->AddUpdateEvent( this );
}

void CStateMachineDiagramEditor::AddExitEvent()
{
	ASSERT( !IsFlatMode() );
	ASSERT( GetCurrentLayerEntity() != NULL );
	ASSERT( GetCurrentLayerEntity()->GetType() == _T("CQHState") );
	CStateDiagramEntity *state = (CStateDiagramEntity *)GetCurrentLayerEntity();
	state->AddExitEvent( this );
}

void CStateMachineDiagramEditor::SaveUndoSelection()
{
	IUndoRedoAction *pAction = new CStateMachineSelectionAction( this );
	AddUndoAction( pAction );
}

void CStateMachineDiagramEditor::SaveRedoSelection()
{
	IUndoRedoAction *pAction = new CStateMachineSelectionAction( this );
	AddRedoAction( pAction );
}

void CStateMachineDiagramEditor::GetSelectedObjects( list<CString> &names )
{
	CDiagramEntityContainer *objs = GetDiagramEntityContainer();
	int	i = objs->GetSize();
	while( i-- )
	{
		CDiagramEntity* obj = objs->GetAt( i );
		if( obj->IsSelected() )
		{
			names.push_back( obj->GetName() );
		}
	}
}

LPCTSTR CStateMachineDiagramEditor::GetParentName( const CDiagramEntity * pEntity ) const
{
	CDiagramEntity *pParentEntity = pEntity->GetParentEntity();
	if( pParentEntity != NULL )
	{
		return pParentEntity->GetName();
	}
	else
	{
		return GetStateMachineName()->GetString();
	}
}

void CStateMachineDiagramEditor::OnUndo()
{
	AfxGetMainWnd()->OnCmdMsg( ID_GDS_EDIT_UNDO, CN_COMMAND, NULL, NULL );
}

void CStateMachineDiagramEditor::OnUpdateUndo( CCmdUI* pCmdUI )
{
	AfxGetMainWnd()->OnCmdMsg( ID_GDS_EDIT_UNDO, CN_UPDATE_COMMAND_UI, pCmdUI, NULL );
}

void CStateMachineDiagramEditor::OnRedo()
{
	AfxGetMainWnd()->OnCmdMsg( ID_GDS_EDIT_REDO, CN_COMMAND, NULL, NULL );
}

void CStateMachineDiagramEditor::OnUpdateRedo( CCmdUI* pCmdUI )
{
	AfxGetMainWnd()->OnCmdMsg( ID_GDS_EDIT_REDO, CN_UPDATE_COMMAND_UI, pCmdUI, NULL );
}

bool CStateMachineDiagramEditor::IsUndoExecuting() const
{
	bool isUndo = false;
	static const DWORD hashMsg = CHashString( _T("UndoIsExecuting") ).GetUniqueID();
	if( MSG_HANDLED == m_ToolBox->SendMessage( hashMsg, sizeof(isUndo), &isUndo ) )
	{
		return isUndo;
	}
	return false;
}

void CStateMachineDiagramEditor::SetStateType( CDiagramEntity *state, CStateDiagramEntity::StateType type )
{
	if( state != NULL )
	{
		CStateDiagramEntity *stateEntity = dynamic_cast<CStateDiagramEntity *>( state );
		stateEntity->SetStateType( type );

		CDiagramEntity *p = GetFlatModeEntity( state->GetName() );
		CStateDiagramEntity *flatState = dynamic_cast<CStateDiagramEntity *>( p );
		ASSERT( flatState != NULL );
		flatState->SetStateType( type );
	}
}

void CStateMachineDiagramEditor::UpdateStateMachineLabels()
{
	SetStateType( m_pStartState, CStateDiagramEntity::Start );
	SetStateType( m_pEndState, CStateDiagramEntity::End );

	StdString startState;
	StdString endState;
	StdString parentStateMachineFile;
	IArchive *ar = GetEEObjectProperties( GetStateMachineName() );
	if( !ReadStateMachineProperties( ar, startState, endState, parentStateMachineFile ) )
	{
		ar->Close();
		return;
	}
	ar->Close();

	if( m_pStartState != NULL )
	{
		if( m_pStartState->GetTitle() != startState.c_str() )
		{
			SetStateType( m_pStartState, CStateDiagramEntity::Normal );
			m_pStartState = GetStateByTitle( startState );
			SetStateType( m_pStartState, CStateDiagramEntity::Start );
		}
	}
	else
	{
		m_pStartState = GetStateByTitle( startState );
		SetStateType( m_pStartState, CStateDiagramEntity::Start );
	}

	if( m_pEndState != NULL )
	{
		if( m_pEndState->GetTitle() != endState.c_str() )
		{
			SetStateType( m_pEndState, CStateDiagramEntity::Normal );
			m_pEndState = GetStateByTitle( endState );
			SetStateType( m_pEndState, CStateDiagramEntity::End );
		}
	}
	else
	{
		m_pEndState = GetStateByTitle( endState );
		SetStateType( m_pEndState, CStateDiagramEntity::End );
	}
}

bool CStateMachineDiagramEditor::ProcessStateMachineChanges( IArchive *ar )
{
	ASSERT( ar != NULL );
	ASSERT( ar->IsReading() );

	StdString startState;
	StdString endState;
	StdString parentStateMachineFile;
	DWORD pos = ar->SeekTo( 0, SEEK_CUR );
	bool read = ReadStateMachineProperties( ar, startState, endState, parentStateMachineFile );
	ar->SeekTo( pos );
	if( !read )
	{
		return true;
	}

	if( parentStateMachineFile == m_parentStateMachineFile )
	{
		return true;
	}

	bool proceed = true;
	Entities::const_iterator itEntity = m_Entities.begin();
	for( ; itEntity != m_Entities.end(); ++itEntity )
	{
		if( IsDerivedEntity( itEntity->second ) )
		{
			CString title = FormatString( IDS_CONFIRM );
			CString msg = FormatString( IDS_CONFIRM_CHANGE_PARENT_MACHINE );
			if( IDYES != MessageBox( msg, title, MB_YESNO | MB_ICONINFORMATION ) )
			{
				// such entities will be deleted after state machine reloading
				return false;
			}
			break;
		}
	}

	vector<IObject *> children;
	GetEEObjectChildren( GetStateMachineName(), children, true );
	ASSERT( *children.front()->GetName() == *GetStateMachineName() );
	// removes state machine object from list
	children.erase( children.begin() );

	// we need to refresh hierarchy in the editor after restoring on undo
	IUndoRedoAction *pAction;
	pAction = new CStateMachineRefreshHierarchyAction( this );
	AddUndoAction( pAction );

	// we need to refresh hierarchy in the editor after restoring on undo
	CStateMachineSetAbstractAction *pSetAbstractAction;
	pSetAbstractAction = new CStateMachineSetAbstractAction();
	AddUndoAction( pSetAbstractAction );

	static const DWORD msgHash_UndoSaveObject = CHashString( _T("UndoSaveObject") ).GetUniqueID();
	static const DWORD hashMsg_GetAbstract = CHashString( _T("GetAbstract") ).GetUniqueID();
	// transitions may refer to states so they should be created after them
	vector<IObject *>::iterator itTransition = children.begin();
	while( itTransition != children.end() )
	{
		IObject *obj = *itTransition;
		IHashString *type = obj->GetComponentType();
		if( *type == m_hszTransitionType )
		{
			IHashString *name = obj->GetName();
			IHashString *parent = obj->GetParentName();

			// save abstract flag for restoring on undo
			bool abstract = false;
			m_ToolBox->SendMessage( hashMsg_GetAbstract, sizeof( abstract ), &abstract, name, type );
			if( abstract )
			{
				pSetAbstractAction->AddObject( name );
			}

			m_ToolBox->SendMessage( msgHash_UndoSaveObject, sizeof( IHashString ), name );
			// try to add create object undo action to stack
			IUndoRedoAction *pCreateAction = new CObjectCreateAction( parent, name, type );
			AddUndoAction( pCreateAction );

			itTransition = children.erase( itTransition );
		}
		else
		{
			++itTransition;
		}
	}

	// processes list in reverse order. Objects will be recreated in right order in this case
	vector<IObject *>::reverse_iterator it = children.rbegin();
	for( it = children.rbegin(); it != children.rend(); ++it )
	{
		IObject *obj = *it;
		IHashString *name = obj->GetName();
		IHashString *parent = obj->GetParentName();
		IHashString *type = obj->GetComponentType();

		// save abstract flag for restoring on undo
		bool abstract = false;
		m_ToolBox->SendMessage( hashMsg_GetAbstract, sizeof( abstract ), &abstract, name, type );
		if( abstract )
		{
			pSetAbstractAction->AddObject( name );
		}

		m_ToolBox->SendMessage( msgHash_UndoSaveObject, sizeof( IHashString ), name );
		// try to add create object undo action to stack
		IUndoRedoAction *pCreateAction = new CObjectCreateAction( parent, name, type );
		AddUndoAction( pCreateAction );
	}

	pAction = new CStateMachineClearAction( GetStateMachineName() );
	AddUndoAction( pAction );

	m_ToolBox->SendMessage( msgHash_UndoSaveObject, sizeof(IHashString), GetStateMachineName() );

	return true;
}

///	read state machine properties from passed archive
///	\param	ar [in] - pointer to archive to read
///	\param	startState [out] - label of the start state machine state
///	\param	endState [out] - label of the end state machine state
///	\param	parentStateMachineFile [out] - parent state machine filename
///	\retrun	true if values were read from archive successfully
bool CStateMachineDiagramEditor::ReadStateMachineProperties( IArchive *ar,
															 StdString &startState,
															 StdString &endState,
															 StdString &parentStateMachineFile ) const
{
	ASSERT( ar != NULL );
	ASSERT( ar->IsReading() );

	float fVersion;
	ar->Read( fVersion );
	if ( fVersion > 1.4f )
	{
		ASSERT( !"Unsupported version of archive" );
		m_ToolBox->Log( LOGWARNING, _T("%s(%d): Unsupported version of archive"), __FILE__, __LINE__ );
		return false;
	}
	StdString sName;
	ar->Read( sName, _T("name") );
	if( fVersion <= 1.3f )
	{
		int tmp;
		ar->Read( tmp, _T("numStates") );
	}
	ar->Read( startState, _T("startState") );
	ar->Read( endState, _T("endState") );
	ar->Read( parentStateMachineFile, _T("parentStateMachineFile") );
	return true;
}

bool CStateMachineDiagramEditor::IsDerivedEntity( CSimpleDiagramEntity *entity ) const
{
	if( !entity->IsAbstract() )
	{
		// check that all parents in the hierarchy are not-abstract
		while( entity->GetParentEntity() != NULL )
		{
			entity = dynamic_cast<CSimpleDiagramEntity *>( entity->GetParentEntity() );
			if( entity->IsAbstract() )
			{
				return true;
			}
		}
	}
	return false;
}

///	this method is called after OnInit message
void CStateMachineDiagramEditor::RefreshStateMachine()
{
	StdString startState;
	StdString endState;
	StdString parentStateMachineFile;
	IArchive *ar = GetEEObjectProperties( GetStateMachineName() );
	if( ReadStateMachineProperties( ar, startState, endState, parentStateMachineFile ) )
	{
		if( parentStateMachineFile != m_parentStateMachineFile )
		{
			m_parentStateMachineFile = parentStateMachineFile;
			m_pStartState = NULL;
			m_pEndState = NULL;
			// refresh state machine hierarchy during normal user editing only
			// ORDER: We have to load editing properties before hierarchy refresh
			//		  Added entities save own position to editing properties
			//		  and LoadEditingProperties does not work
			LoadEditingProperties( parentStateMachineFile );
			RefreshHierarchy();
		}
	}
	ar->Close();

	UpdateStateMachineLabels();
}

///	copies editing properties from parent state machine
void CStateMachineDiagramEditor::LoadEditingProperties( const StdString &parentStateMachineFile )
{
	if( parentStateMachineFile.empty() )
	{
		// nothing to load
		return;
	}
	// building state and event maps for current state machine
	StateMap states;
	EventMap events;
	GetObjectMaps( GetStateMachineName(), states, events );

	static const DWORD msgHash_GetEditingProperties = CHashString( _T("GetEditingProperties") ).GetUniqueID();
	static const DWORD msgHash_SetEditingProperties = CHashString( _T("SetEditingProperties") ).GetUniqueID();
	// removing states with existing editing properties. we do not need to overwrite these properties
	StateMap::iterator itStates = states.begin();
	while( itStates != states.end() )
	{
		EDITINGPROPERTIESPARAMS epp;
		epp.name = itStates->second;
		if( MSG_HANDLED == m_ToolBox->SendMessage( msgHash_GetEditingProperties, sizeof( epp ), &epp ) )
		{
			itStates = states.erase( itStates );
		}
		else
		{
			++itStates;
		}
	}

	// removing events with existing editing properties. we do not need to overwrite these properties
	EventMap::iterator itEvents = events.begin();
	while( itEvents != events.end() )
	{
		EDITINGPROPERTIESPARAMS epp;
		epp.name = itEvents->second;
		if( MSG_HANDLED == m_ToolBox->SendMessage( msgHash_GetEditingProperties, sizeof( epp ), &epp ) )
		{
			itEvents = events.erase( itEvents );
		}
		else
		{
			++itEvents;
		}
	}

	// processing parent machines
	StdString file = parentStateMachineFile;
	while( !file.empty() )
	{
		// getting state machine name from file
		CHashString hszFile = file.c_str();
		GETSTATEMACHINEPARAMS gsm;
		gsm.StateMachineFilePath = &hszFile;
		gsm.StateMachineName = NULL;
		static const DWORD msgHash_GetStateMachineName = CHashString( _T("GetStateMachineName") ).GetUniqueID();
		DWORD result = m_ToolBox->SendMessage( msgHash_GetStateMachineName, sizeof( gsm ), &gsm );
		// parent state machine should be already loaded by engine
		if( MSG_HANDLED == result && 
			gsm.StateMachineName != NULL && !gsm.StateMachineName->IsEmpty() )
		{
			// filling maps with states and events
			StateMap parentStates;
			EventMap parentEvents;
			GetObjectMaps( gsm.StateMachineName, parentStates, parentEvents );

			// extracting editing properties for remained states from parent states
			for( itStates = states.begin(); itStates != states.end(); )
			{
				StateMap::iterator it = parentStates.find( itStates->first );
				if( it != parentStates.end() )
				{
					// check if editing properties exist for parent state 
					EDITINGPROPERTIESPARAMS epp;
					epp.name = it->second;
					if( MSG_HANDLED == m_ToolBox->SendMessage( msgHash_GetEditingProperties, sizeof( epp ), &epp ) )
					{
						// and copies existing properties to state machine object
						epp.name = itStates->second;
						m_ToolBox->SendMessage( msgHash_SetEditingProperties, sizeof( epp ), &epp );
						// properties for object have been set so delete it
						itStates = states.erase( itStates );
					}
					else
					{
						++itStates;
					}
				}
				else
				{
					++itStates;
				}
			}

			// extracting editing properties for remained events from parent events
			for( itEvents = events.begin(); itEvents != events.end(); )
			{
				EventMap::iterator it = parentEvents.find( itEvents->first );
				if( it != parentEvents.end() )
				{
					// check if editing properties exist for parent event 
					EDITINGPROPERTIESPARAMS epp;
					epp.name = it->second;
					if( MSG_HANDLED == m_ToolBox->SendMessage( msgHash_GetEditingProperties, sizeof( epp ), &epp ) )
					{
						// and copies existing properties to state machine object
						epp.name = itEvents->second;
						m_ToolBox->SendMessage( msgHash_SetEditingProperties, sizeof( epp ), &epp );
						// properties for object have been set so delete it
						itEvents = events.erase( itEvents );
					}
					else
					{
						++itEvents;
					}
				}
				else
				{
					++itEvents;
				}
			}

			// our parent machine may be derived from other machine
			StdString startState;
			StdString endState;
			IArchive *ar = GetEEObjectProperties( gsm.StateMachineName );
			if( !ReadStateMachineProperties( ar, startState, endState, file ) )
			{
				file.clear();
			}
			ar->Close();
		}
		else
		{
			// cannot translate file to parent state machine name
			break;
		}
	}
}

///	fill state and event map for state machine
///	\param	stateMachineName - name of state machine
///	\param	states - [out] map label to object name for states
///	\param	events - [out] map for events
void CStateMachineDiagramEditor::GetObjectMaps( IHashString *stateMachineName, StateMap &states, EventMap &events )
{
	IArchive *ar = CreateMemoryArchive();
	ASSERT( ar != NULL );

	// this map allows to extract parent label for event objects
	map<DWORD, DWORD> state2label;
	// fill list of objects from state machine
	vector<IObject *> children;
	GetEEObjectChildren( stateMachineName, children, true );
	vector<IObject *>::iterator it = children.begin();
	for( ; it != children.end(); ++it )
	{
		IObject *obj = *it;
		// prepare to process states and events
		static const DWORD hashState = m_hszStateType.GetUniqueID();
		static const DWORD hashEvent = m_hszEventType.GetUniqueID();
		static CHashString name = CHashString( _T("name") );
		const DWORD hashType = obj->GetComponentType()->GetUniqueID();
		if( hashType == hashState )
		{
			// processing states:
			// extracting state's label
			CHashString hszLabel = GetObjectLabel( obj, ar ).c_str();
			const DWORD label = hszLabel.GetUniqueID();
			IHashString *name = obj->GetName();
			// and push it to states map
			states.insert( make_pair( label, name ) );
			// also saves label for using by child events
			state2label.insert( make_pair( name->GetUniqueID(), label ) );
		}
		else if( hashType == hashEvent )
		{
			// processing event
			// finding label for parent state
			const DWORD parent = obj->GetParentName()->GetUniqueID();
			map<DWORD, DWORD>::const_iterator it = state2label.find( parent );
			ASSERT( it != state2label.end() );
			const DWORD parentLabel = it->second;
			// extracting event label
			CHashString hszLabel = GetObjectLabel( obj, ar ).c_str();
			const DWORD label = hszLabel.GetUniqueID();
			IHashString *name = obj->GetName();
			// and fill event map
			events.insert( make_pair( make_pair( parentLabel, label ), name ) );
		}
	}

	ar->Close();
}

///	get label for passed object
///	\param	object - pointer to object to interact with
///	\param	ar - this archive object is used during interaction with object
StdString CStateMachineDiagramEditor::GetObjectLabel( IObject *object, IArchive *ar )
{
	IAttributeObject *attr = dynamic_cast<IAttributeObject *>( object );
	assert( attr != NULL );
	ar->SetIsWriting( true );
	ar->SeekTo( 0 );
	static CHashString hszName = _T("name");
	attr->GetAttribute( &hszName, ar );
	ar->SetIsWriting( false );
	ar->SeekTo( 0 );
	StdString label;
	ar->Read( label, hszName.GetString() );
	return label;
}
