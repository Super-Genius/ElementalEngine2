///=====================================================================
/// \file	UndoRedoComponent.hpp
/// \brief	Implementation of CUndoRedoMgr
/// \date	08/30/2007
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
#ifndef _GDS_UNDOREDO_COMPONENT_H_
#define _GDS_UNDOREDO_COMPONENT_H_

#include "UndoStack.h"
#include "DependentProcessors.h"

class CUndoCommandData;

class CUndoRedoComponent : public IComponent
{
	// command data container type
	typedef list<CUndoStack *> StackList;
	typedef map<CView *, CUndoStack *> View2StackMap;

	SINGLETONCONSTRUCTOROVERRIDE(CUndoRedoComponent);

	CUndoRedoComponent();
	virtual ~CUndoRedoComponent();

public:
	static IComponent *Create(int nArgs, va_list argptr);

	//////////////////// IComponent overrides //////////////////////////////////

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// return a string of the component type
	/// \return IHashString *reference to textual class name
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is similar, else false
	virtual bool IsKindOf(IHashString *compType);

	//////////////////// EE message handlers ///////////////////////////////////
	
	/// Process creating new window command
	///	\param	size - unused
	///	\param	param - unused
	///	\return EE message code
	DWORD OnCreate(DWORD size, void *param);

	/// Process Undo/Redo menu commands and shortcuts from UI
	///	\param	size - unused
	///	\param	param - unused
	///	\return EE message code
	DWORD OnCmdMsg(DWORD size, void *param);

	///	\brief	notify editors about scene save
	///	\param	size - sizeof(LPCTSTR)
	///	\param	param - filename of the saved scene
	///	\return EE message code
	DWORD OnWorldSave(DWORD size, void *param);

	///	\brief	create new undo stack
	///	\param	size - size of UNDOSTACKINFO structure
	///	\param	param - pointer to UNDOSTACKINFO structure
	DWORD OnUndoCreateStack(DWORD size, void *param);

	///	\brief	delete undo stack
	///	\param	size - size of UNDOSTACKINFO structure
	///	\param	param - pointer to UNDOSTACKINFO structure
	///	\return EE message code
	DWORD OnUndoDeleteStack(DWORD size, void *param);

	///	\brief	rename existing undo stack
	///	\param	size - size of UNDOSTACKRENAME structure
	///	\param	param - pointer to UNDOSTACKRENAME structure
	DWORD OnUndoRenameStack(DWORD size, void *param);

	///	\brief	register reference from view to stack
	///	\param	size - size of UNDOVIEWTOSTACK structure
	///	\param	param - pointer to UNDOVIEWTOSTACK structure
	///	\return EE message code
	DWORD OnUndoRegisterViewStack(DWORD size, void *param);

	///	\brief	clear undo stack. This is useful for operations that cannot be undone
	///	\param	size - unused
	///	\param	param - unused
	///	\return EE message code
	DWORD OnUndoClearStack(DWORD size, void *param);

	///	\brief	start collecting data for next undo command
	///	\param	size - unused
	///	\param	param - unused
	///	\return EE message code
	DWORD OnUndoStartCommand(DWORD size, void *param);
	///	\brief	end collecting data for undo command and place it into undo stack
	///	\param	size - unused
	///	\param	param - unused
	///	\return EE message code
	DWORD OnUndoEndCommand(DWORD size, void *param);
	///	\brief	clear collected data for undo command
	///	\param	size - unused
	///	\param	param - unused
	///	\return EE message code
	DWORD OnUndoCancelCommand(DWORD size, void *param);

	///	\brief	add new undo action to undo stack
	///	\param	size - size of IUndoRedoAction
	///	\param	param - pointer to IUndoRedoAction
	///	\return EE message code
	DWORD OnUndoAddAction(DWORD size, void *param);
	///	\brief	add new redo action to undo stack
	///	\param	size - size of IUndoRedoAction
	///	\param	param - pointer to IUndoRedoAction
	///	\return EE message code
	DWORD OnRedoAddAction(DWORD size, void *param);

	///	\brief	serialize current object state for undo action
	///	\param	size - sizeof(IHashString*)
	///	\param	param - pointer to IHashString with name of object to serialize
	///	\return EE message code
	DWORD OnUndoSaveObject(DWORD size, void *param);
	///	\brief	serialize current object state for redo action
	///	\param	size - sizeof(IHashString*)
	///	\param	param - pointer to IHashString with name of object to serialize
	///	\return EE message code
	DWORD OnRedoSaveObject(DWORD size, void *param);

	///	\brief	serialize current texture data for undo action
	///	\param	size - sizeof(IHashString*)
	///	\param	param - pointer to IHashString with name of texture to serialize
	///	\return EE message code
	DWORD OnUndoSaveTexture(DWORD size, void *param);
	///	\brief	serialize current texture data for redo action
	///	\param	size - sizeof(IHashString*)
	///	\param	param - pointer to IHashString with name of texture to serialize
	///	\return EE message code
	DWORD OnRedoSaveTexture(DWORD size, void *param);

	///	\brief	check if undo(redo) action is executing now
	///	\param	size - sizeof(bool)
	///	\param	param - pointer to bool for output bool value
	///	\return EE message code
	DWORD OnUndoIsExecuting(DWORD size, void *param);

private:
	//////////////////// UI message handlers ///////////////////////////////////

	/// \brief	undo last command from stack
	void OnUndo();
	/// \brief	update Undo menu item state
	void OnUpdateUndo(CCmdUI* pCmdUI);

	/// \brief	redo last rolled back command
	void OnRedo();
	/// \brief	update Redo menu item state
	void OnUpdateRedo(CCmdUI* pCmdUI);

	//////////////////// Helper functions //////////////////////////////////////

	///	\brief	verify passed EE parameter for passing pointer to IHashString
	///	\param	param - pointer to IHashString with name of object to serialize
	///	\return pointer to IHashString with non-empty name
	///			NULL pointer otherwise
	IHashString *GetName(void *param) const;

	///	\brief	add new action to current command
	///	\param	pAction - pointer to new action
	///	\param	bUndo - specify target list for action
	///					if true then pAction will be added to undo list
	DWORD AddAction(IUndoRedoAction *pAction, bool bUndo);

	///	\brief	get pointer to stack by active scene or current view
	///	\return	pointer to active stack or NULL otherwise
	CUndoStack *GetStack();

	///	\brief	get pointer to stack by assigned view
	///	\param	pView - pointer to assigned view.
	///	\return	pointer to assigned stack or NULL otherwise
	CUndoStack *GetStack(CView *pView);

	///	\brief	get pointer to stack by stack name
	///	\param	pName - pointer to IHashString with name of the stack
	///	\return	pointer to found stack or NULL otherwise
	CUndoStack *GetStack(IHashString *pName);

	///	\brief	get stack list iterator by stack name
	///	\param	pName - pointer to IHashString with name of the stack
	///	\return	iterator in m_Stacks container
	StackList::iterator FindStack(IHashString *pName);
	//////////////////// Variables /////////////////////////////////////////////
private:
	IToolBox		   *m_pToolBox;
	StackList			m_Stacks;
	View2StackMap		m_View2Stack;
	CUndoCommandData   *m_pCurrentCommandData;
	CDependentProcessors m_DependantProcessors;
	bool				m_bIsExecuting;
};

#endif // _GDS_UNDOREDO_COMPONENT_H_
