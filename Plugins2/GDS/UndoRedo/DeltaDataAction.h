///=====================================================================
/// \file	DeltaDataAction.h
/// \brief	Declaration of CDeltaDataAction
///			This class implements funcitonality for interaction with
///			saved delta data. It interacts with CUnodCommandData class 
/// \date	11/10/2007
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
#ifndef _GDS_UNDOREDO_DELTA_DATA_ACTION_H_
#define _GDS_UNDOREDO_DELTA_DATA_ACTION_H_

#include "IUndoRedoAction.h"

class CUndoCommandData;
typedef DWORD (*DeltaDataFunc)(CUndoCommandData *, DWORD, size_t, size_t);

template<DeltaDataFunc T>
class CDeltaDataAction : public IUndoRedoAction
{
public:
	///	\brief	initialize action with delta data and command
	///	\param	pCommand - pointer to command this action belongs to
	///	\param	hashName - hash of object name to interact
	///	\param	offset - delta data offset in command buffer
	///	\param	size - delta data size in command buffer
	CDeltaDataAction(CUndoCommandData *pCommand, DWORD hashName, size_t offset, size_t size)
		: m_pCommand(pCommand)
		, m_hashName(hashName)
		, m_Offset(offset)
		, m_Size(size)
	{
		ASSERT(pCommand != NULL);
	}

	///	\brief	initialize this action
	///			the action can be executed if return value is MSG_HANDLED
	///	\return	code of EE message result
	virtual DWORD Initialize()
	{
		return MSG_HANDLED;
	}

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute()
	{
		return T(m_pCommand, m_hashName, m_Offset, m_Size);
	}

	///	\brief	determine useless undo action
	///	\return	true if any change exists
	virtual bool HasChanges() const
	{
		return true;
	}

private:
	CUndoCommandData   *m_pCommand;
	DWORD				m_hashName;
	size_t				m_Offset;
	size_t				m_Size;
};

///////// Delta data implementations ///////////////////////////////////////////
///	\brief	delta data processing for object undo
///	\param	pCommand - pointer to command this action belongs to
///	\param	hashName - hash of object name to interact
///	\param	offset - delta data offset in command buffer
///	\param	size - delta data size in command buffer
DWORD UndoObjectData(CUndoCommandData *pCommand, DWORD hashName, size_t offset, size_t size);
///	\brief	delta data processing for object redo
///	\param	pCommand - pointer to command this action belongs to
///	\param	hashName - hash of object name to interact
///	\param	offset - delta data offset in command buffer
///	\param	size - delta data size in command buffer
DWORD RedoObjectData(CUndoCommandData *pCommand, DWORD hashName, size_t offset, size_t size);
///	\brief	undo/redo command buffer processing for object
///	\param	pCommand - pointer to command this action belongs to
///	\param	hashName - hash of object name to interact
///	\param	offset - delta data offset in command buffer
///	\param	size - delta data size in command buffer
DWORD ApplyObjectData(CUndoCommandData *pCommand, DWORD hashName, size_t offset, size_t size);

///	\brief	delta data processing for texture undo
///	\param	pCommand - pointer to command this action belongs to
///	\param	hashName - hash of texture name to interact
///	\param	offset - delta data offset in command buffer
///	\param	size - delta data size in command buffer
DWORD UndoTextureData(CUndoCommandData *pCommand, DWORD hashName, size_t offset, size_t size);
///	\brief	delta data processing for texture redo
///	\param	pCommand - pointer to command this action belongs to
///	\param	hashName - hash of texture name to interact
///	\param	offset - delta data offset in command buffer
///	\param	size - delta data size in command buffer
DWORD RedoTextureData(CUndoCommandData *pCommand, DWORD hashName, size_t offset, size_t size);
///	\brief	undo/redo command buffer processing for texture
///	\param	pCommand - pointer to command this action belongs to
///	\param	hashName - hash of texture name to interact
///	\param	offset - delta data offset in command buffer
///	\param	size - delta data size in command buffer
DWORD ApplyTextureData(CUndoCommandData *pCommand, DWORD hashName, size_t offset, size_t size);

#endif // _GDS_UNDOREDO_DELTA_DATA_ACTION_H_
