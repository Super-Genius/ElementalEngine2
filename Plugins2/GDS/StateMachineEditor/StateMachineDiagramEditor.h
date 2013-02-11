///============================================================================
/// \file		StateMachineDiagramEditor.h
/// \brief		CDiagramEditor wrapper class for State Machine Editor
/// \date		06-15-2007
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

#ifndef __STATEMACHINEDIAGRAMEDITOR_H__
#define __STATEMACHINEDIAGRAMEDITOR_H__

class CStateDiagramEntity;
class CStateMachineEditorDoc;

class CStateMachineDiagramEditor : public CDiagramEditor
{
	typedef map<DWORD, CSimpleDiagramEntity *> Entities;
	// state labels to object name map
	typedef map<DWORD, IHashString *> StateMap;
	// state and event labels pair to event object name map
	typedef map<pair<DWORD, DWORD>, IHashString *> EventMap;

	typedef CDiagramEditor Base;
public:
	CStateMachineDiagramEditor();
	virtual ~CStateMachineDiagramEditor();

	virtual BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, CDiagramEntityContainer* data = NULL );
	void	SetDocument( CStateMachineEditorDoc *pDocument );
	void	SetStateMachineName( IHashString *stateMachineName );
	///	refreshes whole state machine hierarchy
	void	RefreshHierarchy();
	///	refreshes hierarchy for passed object only
	///	\param	name - name of changed object
	void	RefreshHierarchy( IHashString *name );

	virtual void	LayerUp();
	virtual void	ShowProperties();

	virtual	CSize	GetVirtualSize() const;
	virtual void	MoveObject( CDiagramEntity* obj, const CRect& rect );

	virtual void	AddObject( CDiagramEntity *obj );
	void	AddObject( CSimpleDiagramEntity *obj, bool addToMap );
	void	AddObject( CDiagramEntity *pParentEntity, CSimpleDiagramEntity *pEntity, bool addToMap );
	void	AddFlatModeObject( CSimpleDiagramEntity *obj );
	///	\brief	delete all selected entities from the editor and EE hierarchy
	virtual void	DeleteAllSelected();
	///	\brief	delete passed object and its children from EE hierarchy and from the editor
	///	\param	pEntity - pointer to deleting object
	void	DeleteObject( CDiagramEntity *pEntity );
	///	\brief	delete passed object and its children from EE hierarchy and from the editor
	///	\param	szName - name of the deleting object
	void	DeleteObjectByName( LPCTSTR szName );
	///	\brief	delete transitions which refer to passed state
	///	\param	pState - pointer to state entity
	void	DeleteTransitionsForState( CDiagramEntity *pState );
	///	\brief	remove all entities in the editor
	void	RemoveAll();
	///	\brief	remove passed entity from the editor
	void	RemoveObject( CDiagramEntity *pEntity );

	///	\brief	this function refreshes entity with passed object name hash
	///	\param	name - hash of the object name
	void RefreshEntity( IHashString *name );

	///	\breif	check that any transition refers to state with passed label
	///	\brief	label - checked state label
	bool IsStateReferenced( const StdString &label ) const;

	///	refreshes state machine properties: start and end state, parent state 
	///	machine file
	void UpdateStateMachineLabels();

	///	this function refreshes events with passed labels. If old label is
	///	equal to new label then this means that state references should now be 
	///	updated. Nevertheless object should update own state based on passed label
	/// because user may select to not update references
	///	\param	oldLabel - old state label
	///	\param	newLabel - new state label.
	void UpdateStateLabel( const StdString &oldLabel, const StdString &newLabel );

	///	\brief	this function refreshes event with passed label
	///	\sa		UpdateStateLabel
	///	\param	oldLabel - old event label
	///	\param	event - pointer to renamed event
	void UpdateEventLabel( const StdString &oldLabel, CSimpleDiagramEntity *event );

	///	\brief	find existing entity by object name
	///	\param	pName - pointer to IHashString with object name
	///	\return pointer to found entity or NULL otherwise
	CSimpleDiagramEntity *GetEntity( const IHashString *pName ) const;

	///	\brief	find existing entity by object name in flat mode container
	///	\param	szName - string with object name
	///	\return pointer to found entity or NULL otherwise
	CDiagramEntity *GetFlatModeEntity( LPCTSTR szName ) const;

	///	\brief	find existing state entity by entity title
	///	\param	szTitle - title of the state
	///	\return pointer to found entity or NULL otherwise
	CSimpleDiagramEntity *GetStateByTitle( LPCTSTR szTitle ) const;

	///	\brief	this function select entity. It switches layer also
	///	\param	name - hash of the object name
	void SelectObject( const IHashString *name );
	
	///	\return true if all state machine objects are valid
	bool IsValid() const;

	///	\brief	update state machine objects errors flag
	///	\return true if all state machine objects are valid
	bool UpdateValidate();

	///	\brief	check state machine objects for errors and show message box with
	///			text if error exists
	void Validate();

	///	\brief	validate passed value for object
	///	\param	sop - pointer to SERIALIZEOBJECTPARAMS with object name and archive
	///	\return	MSG_ERROR - if passed data is not valid for object
	///			MSG_HANDLED - if passed data is valid for object
	///			MSG_NOT_HANDLED - if object data was not processed
	DWORD ValidateObject( SERIALIZEOBJECTPARAMS *sop );

	///	\brief	switch editor to transition drawing mode or back
	///	\param	drawing - defines mode for editor. false cancels current drawing
	void SetTransitionDrawing( bool drawing );

	bool IsTransitionDrawing() const
	{
		return m_bTransitionDrawingMode;
	}

	///	\brief	switch editor between hierarchical and flat modes
	///	\param	flat - true switches editor to flat mode, false - to hierarchical
	void SetFlatMode( bool flat );

	bool IsFlatMode() const;

	virtual void		Cut();
	virtual void		Copy();

	virtual void		Paste();

	bool CanPaste() const;

	void AddNewState();
	void AddNewEvent();
	void AddEntryEvent();
	void AddUpdateEvent();
	void AddExitEvent();

	///	function adds a number of objects to editor. Entities are added in two 
	/// passes: states and events first, then transitions are added. This allows
	///	for transition to refer states deeper in hierarchy
	///	\param	objects - list with object information to add
	void AddEntities( vector<ObjectInfo> &objects );

	///	\brief	add new event to the editor
	///	\param	pParentName - name of the parent object
	///	\param	pName - name of the event object
	///	\return	newly added event entity
	CDiagramEntity *AddEvent( IHashString * pParentName, IHashString * pName );

	///	\brief	add new transition entity to the editor
	///	\param	pParentName - name of the parent object
	///	\param	pName - name of the transition object
	void AddTransition( IHashString * pParentName, IHashString * pName );

	void GetSelectedObjects( list<CString> &names );

	void SaveUndoSelection();
	void SaveRedoSelection();

	CStateMachineEditorDoc *GetDocument() const
	{
		ASSERT( m_pDocument != NULL );
		return m_pDocument;
	}

	IHashString *GetStateMachineName() const
	{
		return (IHashString*)&m_StateMachineName;
	}

protected:
	///	\brief	set passed entity as current layer
	///	\param	pEnity - pointer to layer entity. If NULL is passed as value 
	///					 then root diagram layer set
	void SetCurrentLayerEntity( CDiagramEntity *pEntity );

	///	\brief	add new state to the editor
	///	\param	pParentName - name of the parent object
	///	\param	pName - name of the state object
	///	\return	newly added state entity
	CDiagramEntity *AddState( IHashString * pParentName, IHashString * pName );

	///	\brief	add new action to the editor
	///	\param	pParentName - name of the parent object
	///	\param	pName - name of the action object
	void AddAction( IHashString * pParentName, IHashString * pName );

	///	\brief	remove passed entity from parent. Handles this event for other purposes
	///	\param	pEntity - pointer to entity to remove
	void RemoveEntity( CDiagramEntity *pEntity );

	///	\brief	function select entities from objects list which are in current
	///	editor layer. Objects are processed by their names
	///	\param	objects - list with object information
	void SelectEntities( vector<ObjectInfo> &objects );

	///	\brief	create new transition if possible
	///	\param	point - screen coordinates to check
	void ConfirmTransitionDrawing( const CPoint &point );

	void EndTransitionDrawing();

	///	\brief	find entity under point. Function finds in current entity container
	///	\param	point - screen coordinates to check
	///	\return	pointer to found entity or NULL
	CDiagramEntity *GetEntity( const CPoint &point ) const;

	///	\brief	find top state that contains event with passed label
	///	\param	state - pointer to state for search
	///	\param	label - event label to find
	///	\return	top state which has event with passed label
	CDiagramEntity *GetTopStateWithEvent( CDiagramEntity *state, const StdString &label ) const;

	///	\brief	collect entities which refer to event with passed label
	///	\param	state - pointer to state for visit
	///	\param	label - event label to find
	///	\param	states - list with states entities
	///	\param	events - list with event entities
	///	\return	top state which has event with passed label
	void CollectEventReferences( CSimpleDiagramEntity *state,
								 const StdString &label,
								 vector<CSimpleDiagramEntity *> &states, 
								 vector<CSimpleDiagramEntity *> &events );

	///	\brief	extract event names from passed state
	///	\param	state - pointer to state entity
	///	\param	entryEvent [out] - name of entry event
	///	\param	updateEvent [out] - name of exit event
	void GetStateEventReferences( CSimpleDiagramEntity *state,
								  StdString &entryEvent,
								  StdString &updateEvent,
								  StdString &exitEvent );

	virtual afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	virtual afx_msg void OnLButtonUp( UINT nFlags, CPoint point );

	virtual afx_msg void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual afx_msg void OnMouseMove( UINT nFlags, CPoint point );
	virtual afx_msg void OnRButtonDown( UINT nFlags, CPoint point );
	virtual afx_msg void OnRButtonUp( UINT nFlags, CPoint point );

	virtual afx_msg void OnObjectCommand( UINT nID );

	afx_msg void OnUndo();
	afx_msg void OnUpdateUndo( CCmdUI* pCmdUI );
	afx_msg void OnRedo();
	afx_msg void OnUpdateRedo( CCmdUI* pCmdUI );

	DECLARE_MESSAGE_MAP()

private:
	void DoRefreshHierarchy( IHashString *name );

	///	\brief	collect selected states and find top state entity under passed point
	CDiagramEntity *GetMovingStates( CPoint point, vector<CStateDiagramEntity *> &states );

	LPCTSTR GetParentName( const CDiagramEntity * pEntity ) const;

	bool IsUndoExecuting() const;

	void EndCreateObjectMode();

	void SetStateType( CDiagramEntity *state, CStateDiagramEntity::StateType type );

	///	this method is called after OnInit message
	void RefreshStateMachine();
	///	copies editing properties from parent state machine
	void LoadEditingProperties( const StdString &parentStateMachineFile );
	///	fill state and event map for state machine
	///	\param	stateMachineName - name of state machine
	///	\param	states - [out] map label to object name for states
	///	\param	events - [out] map for events
	void GetObjectMaps( IHashString *stateMachineName, StateMap &states, EventMap &events );
	///	get label for passed object
	///	\param	object - pointer to object to interact with
	///	\param	ar - this archive object is used during interaction with object
	StdString GetObjectLabel( IObject *object, IArchive *ar );

	bool ProcessStateMachineChanges( IArchive *ar );
	
	///	read state machine properties from passed archive
	///	\param	ar [in] - pointer to archive to read
	///	\param	startState [out] - label of the start state machine state
	///	\param	endState [out] - label of the end state machine state
	///	\param	parentStateMachineFile [out] - parent state machine filename
	///	\retrun	true if values were read from archive successfully
	bool ReadStateMachineProperties( IArchive *ar,
									 StdString &startState,
									 StdString &endState,
									 StdString &parentStateMachineFile ) const;
	
	bool IsDerivedEntity( CSimpleDiagramEntity *entity ) const;

private:
	IToolBox*	m_ToolBox;
	CHashString	m_hszStateMachineType;
	CHashString	m_hszStateType;
	CHashString	m_hszEventType;
	CHashString	m_hszActionType;
	CHashString	m_hszTransitionType;

	CStateMachineEditorDoc *m_pDocument;
	CHashString m_StateMachineName;
	StdString	m_parentStateMachineFile;
	Entities	m_Entities;
	// flat mode variables
	CDiagramEntity *m_pHierarchyModeEntity;
	CDiagramEntityContainer m_FlatModeEntities;
	/// pointer to current start state of state machine
	CDiagramEntity *m_pStartState;
	/// pointer to current end state of state machine
	CDiagramEntity *m_pEndState;

	// transition drawing variables
	bool	m_bTransitionDrawingMode;
	CSimpleDiagramEntity	*m_pTransitionSource;
	CSimpleDiagramEntity *m_pDrawingTransition;
	CDiagramEntity	*m_pMousePlacement;
	CStateMachineClipboard *m_pClipboard;
	CStateMachineValidator *m_pValidator;
	bool	m_bValid;
	bool	m_bIsCascadeUpdating;

	// variables for undo processing
	CPoint m_DownClickPoint;
	bool m_bCreateObjectMode;
	CUndoObjectGroup	m_UndoGroup;
};

#endif // #ifndef __STATEMACHINEDIAGRAMEDITOR_H__