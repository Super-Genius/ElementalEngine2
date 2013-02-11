///=====================================================================
/// \file	UndoCommandData.h
/// \brief	Declaration for CUndoCommandData. This class holds data 
///			for preforming undo and redo commands
/// \date	09/29/2007
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
#ifndef _GDS_UNDO_COMMAND_DATA_H_
#define _GDS_UNDO_COMMAND_DATA_H_

#include "IUndoRedoAction.h"
typedef vector<BYTE> Buffer;

class CUndoCommandData
{
	typedef list<IUndoRedoAction *> ActionList;
public:
	///	\brief	default constructor
	CUndoCommandData();
	~CUndoCommandData();

	///	\brief	add new action to undo action list
	///	\param	pAction - pointer to new action
	void AddUndoAction(IUndoRedoAction *pAction);

	///	\brief	add new action to redo action list
	///	\param	pAction - pointer to new action
	void AddRedoAction(IUndoRedoAction *pAction);

	///	\brief	add new action to redo or redo action lists
	///	\param	pAction - pointer to new action
	///	\param	bUndo - specify target list for action
	///					if true then pAction will be added to undo list
	void AddAction(IUndoRedoAction *pAction, bool bUndo);

	///	\brief	execute undo part of command
	///	\return	code of EE message result
	DWORD ExecuteUndo();

	///	\brief	execute redo part of command
	///	\return	code of EE message result
	DWORD ExecuteRedo();

	///	\brief	determine if undo command data was filled correctly
	///	\return	true if active undo actions exist in the list
	bool HasUndoActions() const;

	///	\brief	compact memory usage by using data delta and compression
	void CompactData();

	///	\brief give access to buffer with uncompressed delta data
	///	\return pointer to buffer
	const LPBYTE GetDeltaData() const
	{
		return m_pData;
	}

private:
	///	\brief	execute commands from stream
	///	\param	actions list with action to execute
	///	\return	code of EE message result
	DWORD Execute(ActionList &actions);

	///	\brief	gather information about name and data from the action
	///	\param	pAction [in] - pointer to action
	///	\param	hashName [out] - hash of object or texture name
	///	\param	data [out] - buffer with archive or texture 
	///	\param	isObjectAction [out] - true if passed action was CObjectSerializeAction
	///	\return	true if hash and data buffer are valid
	static bool GetBuffer(IUndoRedoAction *pAction, DWORD &hashName, Buffer &data, bool &isObjectAction);

	///	\brief	calculate buffer delta. Output buffer is also reduced by trailing 
	///			zeros if both input buffers have the same size
	///	\param	source [in] - source data
	///	\param	target [in,out] - target values. values in this buffer are 
	///							  replaced with delta between source and target
	static void GetBuffersDelta(const Buffer &source, Buffer &target);

private:
	ActionList	m_UndoActions;
	ActionList	m_RedoActions;

	///	\brief	this buffer is used for holding compressed delta data. It is 
	///			replaced with uncompressed data temporary during Execute call.
	LPBYTE		m_pData;
	size_t		m_CompressedLength;
	size_t		m_UncompressedLength;
};

#endif // _GDS_UNDO_COMMAND_DATA_H_
