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

#include "stdafx.h"

/////////////// CDatabaseCellChangeAction //////////////////////////////////////
///	\brief	execute command
///	\return	code of EE message result
DWORD CDatabaseCellChangeAction::Execute()
{
	m_pView->SetCellValue(m_nRow, m_nCol, m_sText);
	m_pView->FocusCell(m_nRow, m_nCol);
	return MSG_HANDLED;
}

/////////////// CDatabaseAddRowAction //////////////////////////////////////////
///	\brief	execute command
///	\return	code of EE message result
DWORD CDatabaseAddRowAction::Execute()
{
	m_pView->AddRow(m_nRow);
	
	Values::const_iterator it = m_Values.begin();
	for (int col = 0; it != m_Values.end(); ++it, ++col)
	{
		m_pView->SetCellValue(m_nRow, col, *it);
	}

	m_pView->FocusRow(m_nRow);

	return MSG_HANDLED;
}

/////////////// CDatabaseRemoveRowAction ///////////////////////////////////////
///	\brief	execute command
///	\return	code of EE message result
DWORD CDatabaseRemoveRowAction::Execute()
{
	m_pView->RemoveRow(m_nRow);
	return MSG_HANDLED;
}

