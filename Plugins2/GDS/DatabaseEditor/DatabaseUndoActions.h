///=====================================================================
/// \file	DatabaseUndoActions.h
/// \brief	Declaration of actions for filling undo and redo stacks
/// \date	11/27/2007
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
#ifndef _DATABASE_UNDO_ACTIONS_H_
#define _DATABASE_UNDO_ACTIONS_H_

#include "UndoActions.h"

/////////////// CDatabaseCellChangeAction //////////////////////////////////////
class CDatabaseCellChangeAction : public CBaseUndoRedoAction
{
public:
	///	\brief	CDatabaseCellChangeAction constructor
	///	\param	pView - pointer to assigned to action view
	///	\param	nRow - DB row for edit
	///	\param	nCol - DB col for edit
	///	\param	szText - be text for cell
	CDatabaseCellChangeAction(CDatabaseView *pView, int nRow, int nCol, LPCTSTR szText)
		: m_pView(pView)
		, m_nRow(nRow)
		, m_nCol(nCol)
		, m_sText(szText)
	{
	}

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

private:
	CDatabaseView  *m_pView;
	int				m_nRow;
	int				m_nCol;
	CString			m_sText;
};

/////////////// CDatabaseAddRowAction //////////////////////////////////////////
class CDatabaseAddRowAction : public CBaseUndoRedoAction
{
public:
	typedef vector<CString> Values;
	///	\brief	CDatabaseAddRowAction constructor
	///	\param	pView - pointer to assigned to action view
	///	\param	nRow - position of the new row
	///	\param	values - list with cell values. constructor swaps container content
	CDatabaseAddRowAction(CDatabaseView *pView, int nRow, Values &values)
		: m_pView(pView)
		, m_nRow(nRow)
	{
		m_Values.swap(values);
	}

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

private:
	CDatabaseView  *m_pView;
	int				m_nRow;
	Values			m_Values;
};

/////////////// CDatabaseRemoveRowAction ///////////////////////////////////////
class CDatabaseRemoveRowAction : public CBaseUndoRedoAction
{
public:
	///	\brief	CDatabaseRemoveRowAction constructor
	///	\param	pView - pointer to assigned to action view
	///	\param	nRow - position of the deleting row
	CDatabaseRemoveRowAction(CDatabaseView *pView, int nRow)
		: m_pView(pView)
		, m_nRow(nRow)
	{
	}

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

private:
	CDatabaseView  *m_pView;
	int				m_nRow;
};

#endif //_DATABASE_UNDO_ACTIONS_H_
