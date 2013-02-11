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
#ifndef __STATE_MACHINE_UNDO_ACTIONS_H__
#define __STATE_MACHINE_UNDO_ACTIONS_H__

/////////////// CStateMachineSelectionAction ///////////////////////////////////
class CStateMachineSelectionAction : public CBaseUndoRedoAction
{
	typedef vector<DWORD> Hashes;
public:
	///	\brief	CStateMachineSelectionAction constructor
	///	\param	pEditor - pointer to assigned to action editor
	CStateMachineSelectionAction( CStateMachineDiagramEditor *pEditor );

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

private:
	CStateMachineDiagramEditor *m_pEditor;
	DWORD m_hashLayerEntity;
	CDiagramEntityContainer *m_objs;
	Hashes m_selection;
};

/////////////// CStateMachineAddEntityAction ///////////////////////////////////
class CStateMachineAddEntityAction : public CBaseUndoRedoAction
{
public:
	///	\brief	CStateMachineSelectionAction constructor
	///	\param	pEditor - pointer to assigned to action editor
	///	\param	pParent - pointer to IHashString with parent name of the added EE object
	///	\param	pName - pointer to IHashString with name of the added EE object
	CStateMachineAddEntityAction( CStateMachineDiagramEditor *pEditor,
								  IHashString *pParent,
								  IHashString *pName );

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

private:
	CStateMachineDiagramEditor *m_pEditor;
	DWORD m_hashParent;
	DWORD m_hashName;
};

/////////////// CStateMachineRemoveEntityAction ////////////////////////////////
class CStateMachineRemoveEntityAction : public CBaseUndoRedoAction
{
public:
	///	\brief	CStateMachineSelectionAction constructor
	///	\param	pEditor - pointer to assigned to action editor
	///	\param	pName - pointer to IHashString with name of the added EE object
	CStateMachineRemoveEntityAction( CStateMachineDiagramEditor *pEditor,
									 IHashString *pName );

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

private:
	CStateMachineDiagramEditor *m_pEditor;
	DWORD m_hashName;
};

/////////////// CStateMachineChangeParentAction ////////////////////////////////
class CStateMachineChangeParentAction : public CBaseUndoRedoAction
{
	typedef vector<DWORD> Hashes;
public:
	///	\brief	CStateMachineChangeParentAction constructor
	///	\param	pEditor - pointer to assigned to action editor
	///	\param	parent - pointer to IHashString with new parent name
	CStateMachineChangeParentAction( CStateMachineDiagramEditor *pEditor, IHashString *parent );

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

	///	\brief	register state name in states list
	///	\param	state - pointer to IHashString with state name
	void AddState( IHashString *state );

private:
	CStateMachineDiagramEditor *m_pEditor;
	Hashes m_states;
	DWORD m_parent;
};

/////////////// CStateMachineSetAbstractAction /////////////////////////////////
class CStateMachineSetAbstractAction : public CBaseUndoRedoAction
{
	typedef vector<DWORD> Names;
public:
	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

	///	\brief	register object for setting as abstract
	///	\param	name - pointer to IHashString with object name
	void AddObject( IHashString *name );

private:
	Names m_names;
};

/////////////// CStateMachineClearAction ///////////////////////////////////////
class CStateMachineClearAction : public CBaseUndoRedoAction
{
public:
	///	\brief	CStateMachineClearAction constructor
	///	\param	stateMachineName - pointer to IHashString with state machine name
	CStateMachineClearAction( IHashString *stateMachineName );

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

private:
	DWORD m_stateMachineName;
};

/////////////// CStateMachineRefreshHierarchyAction ////////////////////////////
class CStateMachineRefreshHierarchyAction : public CBaseUndoRedoAction
{
public:
	///	\brief	CStateMachineRefreshHierarchyAction constructor
	///	\param	pEditor - pointer to editor to interact with
	CStateMachineRefreshHierarchyAction( CStateMachineDiagramEditor *pEditor );

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

private:
	CStateMachineDiagramEditor *m_pEditor;
};

#endif //__STATE_MACHINE_UNDO_ACTIONS_H__
