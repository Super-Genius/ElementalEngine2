///=====================================================================
/// \file	UndoStack.cpp
/// \brief	Inplementation of CUndoStack
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
#include "StdAfx.h"

IToolBox *CUndoStack::m_pToolBox = EngineGetToolBox();

CUndoStack::CUndoStack(IHashString *pName)
	: m_hszName(pName)
	, m_nSavePoint(-1)
{
}

CUndoStack::~CUndoStack()
{
	ClearStack();
}

///	\brief	clear undo stack
void CUndoStack::ClearStack()
{
	ClearList(m_UndoCommands);
	ClearList(m_RedoCommands);
	// we cannot reach save point in this case
	m_nSavePoint = -1;
}

///	\brief	set save point to the current stack size
void CUndoStack::SetSavePoint()
{
	m_nSavePoint = (int)m_UndoCommands.size();
}

///	\brief	check if current stack matches to save point
///	\return	true if save point is reached
bool CUndoStack::IsSavePoint() const
{
	return m_nSavePoint == (int)m_UndoCommands.size();
}

///	\brief add new undo data to the stack
///	\param	pData - pointer to undo data
void CUndoStack::AddUndo(CUndoCommandData *pData)
{
	ASSERT(pData != NULL);
	// is not possible to redo after inserting new command
	ClearList(m_RedoCommands);
	// checking if save point is still valid
	if (m_nSavePoint > (int)m_UndoCommands.size())
	{
		m_nSavePoint = -1;
	}
	m_UndoCommands.push_back(pData);
	// TODO: limit undo stack memory usage
}

/// \brief	undo last command from stack
void CUndoStack::Undo()
{
	ASSERT(CanUndo());
	CUndoCommandData *pData = m_UndoCommands.back();
	ASSERT(pData != NULL);
	pData->ExecuteUndo();
	// TODO: log errors?
	m_UndoCommands.pop_back();
	m_RedoCommands.push_back(pData);
}

/// \brief	redo last rolled back command
void CUndoStack::Redo()
{
	ASSERT(CanRedo());
	CUndoCommandData *pData = m_RedoCommands.back();
	ASSERT(pData != NULL);
	pData->ExecuteRedo();
	// TODO: log errors?
	m_RedoCommands.pop_back();
	m_UndoCommands.push_back(pData);
}

///	\brief	delete all pointers from the list then clear list
///	\param	list - reference to list
void CUndoStack::ClearList(CommandList &list)
{
	for_each(list.begin(), list.end(), Deleter<CUndoCommandData>);
	list.clear();
}
