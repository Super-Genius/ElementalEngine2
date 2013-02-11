///=====================================================================
/// \file	ObjectSerializeAction.h
/// \brief	Declaration of CObjectSerializeAction
///			This class implements funcitonality for reading object 
///			proeprties and setting they back on Execute
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
#ifndef _GDS_UNDOREDO_OBJECT_SERIALIZE_ACTION_H_
#define _GDS_UNDOREDO_OBJECT_SERIALIZE_ACTION_H_

#include "IUndoRedoAction.h"

class CObjectSerializeAction : public IUndoRedoAction
{
public:
	///	\brief	serialize current object state for command
	///	\param	pName - pointer to IHashString with name of object to serialize
	CObjectSerializeAction(IHashString *pName);
	virtual ~CObjectSerializeAction();

	///	\brief	initialize this action
	///			the action can be executed if return value is MSG_HANDLED
	///	\return	code of EE message result
	virtual DWORD Initialize();

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

	///	\brief	determine useless undo action
	///	\return	true if any change exists
	virtual bool HasChanges() const;

	// helper functions for interaction with command data class

	///	\brief	get hash of object name
	///	\return	hash value
	DWORD GetName() const
	{
		return m_hashName;
	}

	///	\brief	fill passed buffer with archive data
	///	\param	data [out] - copy of saved archive stream data
	void GetData(Buffer &data) const;

private:
	DWORD			  m_hashName;
	IArchive		 *m_pData;
};

#endif // _GDS_UNDOREDO_OBJECT_SERIALIZE_ACTION_H_
