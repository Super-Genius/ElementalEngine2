///=====================================================================
/// \file	UndoStack.h
/// \brief	Declaration of CUndoStack
///			This class implements functions for separate undo stack
/// \date	10/18/2007
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
#ifndef _GDS_UNDOREDO_UNDO_STACK_H_
#define _GDS_UNDOREDO_UNDO_STACK_H_

class CUndoCommandData;

class CUndoStack
{
public:
	// command data container type
	typedef vector<CUndoCommandData*> CommandList;

	///	\brief	Initialize new stack
	///	\param	pName - name of the stack. It may be active scene name or path
	///					to opened document
	CUndoStack(IHashString *pName);
	~CUndoStack();

	///	\brief	clear undo stack
	void ClearStack();

	///	\brief	set save point to the current stack size
	void SetSavePoint();

	///	\brief	check if current stack matches to save point
	///	\return	true if save point is reached
	bool IsSavePoint() const;

	///	\brief add new undo data to the stack
	///	\param	pData - pointer to undo data
	void AddUndo(CUndoCommandData *pData);

	/// \brief	undo last command from stack
	void Undo();

	/// \brief	redo last rolled back command
	void Redo();

	///	\brief	check that undo command can be executed
	///	\return true if undo can be executed
	bool CanUndo() const
	{
		return !m_UndoCommands.empty();
	}

	///	\brief	check that redo command can be executed
	///	\return true if redo can be executed
	bool CanRedo() const
	{
		return !m_RedoCommands.empty();
	}

	///	\brief	set new name for the stack
	///	\param	pNewName - new name of the stack
	void SetName(IHashString *pNewName)
	{
		m_hszName = pNewName;
	}

	bool operator ==(IHashString *pName)
	{
		return m_hszName == *pName;
	}

private:
	//////////////////// Helper functions //////////////////////////////////////

	///	\brief	delete all pointers from the list then clear list
	///	\param	list - reference to list
	static void ClearList(CommandList &list);

private:
	static IToolBox	   *m_pToolBox;
	CHashString			m_hszName;
	CommandList			m_UndoCommands;
	CommandList			m_RedoCommands;
	///	this value stores index of the save point in the stack
	/// TODO: implement this functionality
	int					m_nSavePoint;
};

#endif //_GDS_UNDOREDO_UNDO_STACK_H_