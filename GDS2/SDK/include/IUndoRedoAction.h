///=====================================================================
/// \file	IUndoRedoAction.h
/// \brief	Declaration of IUndoRedoAction interface
/// \date	10/02/2007
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
#ifndef _GDS_UNDOREDO_IACTION_H_
#define _GDS_UNDOREDO_IACTION_H_

class IUndoRedoAction
{
public:
	virtual ~IUndoRedoAction() {};

	///	\brief	initialize this action
	///			the action cannot be executed if result is differ from MSG_HANDLED
	///	\return	code of EE message result
	virtual DWORD Initialize() = 0;

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute() = 0;

	///	\brief	determine useless undo action
	///			undo action checks against current state
	///	\return	true if no changes exists
	virtual bool HasChanges() const = 0;
};

#endif // _GDS_UNDOREDO_IACTION_H_