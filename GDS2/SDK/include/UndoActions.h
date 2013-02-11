///=====================================================================
/// \file	UndoActions.h
/// \brief	Declarations of simple undo actions for common usage
/// \date	11/02/2007
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
#ifndef _GDS_UNDO_ACTIONS_H_
#define _GDS_UNDO_ACTIONS_H_

#include "IUndoRedoAction.h"

namespace GDS_SDK_Helpers
{

/////////////// CBaseUndoRedoAction ////////////////////////////////////////////
///	\brief	Base class for simple undo actions. It provides simple Initialize() 
///			and HasChanges() implementation
class CBaseUndoRedoAction : public IUndoRedoAction
{
public:
	///	\brief	initialize this action
	///			the action cannot be executed if result is differ from MSG_HANDLED
	///	\return	code of EE message result
	virtual DWORD Initialize();

	///	\brief	determine useless undo action
	///			undo action checks against current state
	///	\return	true if no changes exists
	virtual bool HasChanges() const;

protected:
	static IToolBox *m_pToolBox;
};

/////////////// CObjectSelectAction ////////////////////////////////////////////
class CObjectSelectAction : public CBaseUndoRedoAction
{
public:
	///	\brief	CObjectCreateAction constructor
	///	\param	pName - name of the object
	///	\param	pNewParent - new Parent of the object
	CObjectSelectAction(IHashString *pName)
		: m_hashName(pName->GetUniqueID())
	{
	};

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();
private:
	DWORD	m_hashName;
};

/////////////// CSetSectorModifiedAction ///////////////////////////////////////
class CSetSectorModifiedAction : public CBaseUndoRedoAction
{
public:
	///	\brief	CSetSectorModifiedAction constructor
	///	\param	pName - name of the object
	///	\param	pNewParent - new Parent of the object
	CSetSectorModifiedAction(IHashString *pName)
		: m_hashName(pName->GetUniqueID())
	{
	};

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

private:
	DWORD	m_hashName;
};

/////////////// CObjectCreateAction ////////////////////////////////////////////
class CObjectCreateAction : public IUndoRedoAction
{
public:
	///	\brief	CObjectCreateAction constructor
	///	\param	pParentName - name of object's parent
	///	\param	pName - name of the object
	///	\param	pType - type of the created object. This value may be NULL, 
	///					function determines object type in this case
	CObjectCreateAction(IHashString *pParentName, IHashString *pName, IHashString *pType);

	///	\brief	initialize this action
	///			the action cannot be executed if result is differ from MSG_HANDLED
	///	\return	code of EE message result
	virtual DWORD Initialize();

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

	///	\brief	determine useless undo action
	///			undo action checks against current state
	///	\return	true if no changes exists
	virtual bool HasChanges() const;

protected:
	const DWORD	m_hashName;

private:
	DWORD	m_hashParentName;
	DWORD	m_hashType;
};

/////////////// CObjectRecreateAction //////////////////////////////////////////
///	\brief	This action check object existence before creating object. If object
///			exists then action does nothing
class CObjectRecreateAction : public CObjectCreateAction
{
public:
	///	\brief	CObjectRecreateAction constructor
	///	\param	pParentName - name of object's parent
	///	\param	pName - name of the object
	///	\param	pType - type of the created object. This value may be NULL, 
	///					function determines object type in this case
	CObjectRecreateAction(IHashString *pParentName, IHashString *pName, IHashString *pType);

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();
};

/////////////// CObjectDeleteAction ////////////////////////////////////////////
class CObjectDeleteAction : public CBaseUndoRedoAction
{
public:
	///	\brief	CObjectCreateAction constructor
	///	\param	pName - name of the object
	CObjectDeleteAction(IHashString *pName)
		: m_hashName(pName->GetUniqueID())
	{
	};

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

private:
	DWORD	m_hashName;
};

/////////////// CObjectRenameAction ////////////////////////////////////////////
class CObjectRenameAction : public CBaseUndoRedoAction
{
public:
	///	\brief	CObjectCreateAction constructor
	///	\param	pName - name of the object
	///	\param	pNewName - new name of the object
	CObjectRenameAction(IHashString *pName, IHashString *pNewName)
		: m_hashName(pName->GetUniqueID())
		, m_hashNewName(pNewName->GetUniqueID())
	{
	};

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

private:
	DWORD	m_hashName;
	DWORD	m_hashNewName;
};

/////////////// CObjectChangeParentAction //////////////////////////////////////
class CObjectChangeParentAction : public CBaseUndoRedoAction
{
public:
	///	\brief	CObjectCreateAction constructor
	///	\param	pName - name of the object
	///	\param	pNewParent - new Parent of the object
	CObjectChangeParentAction(IHashString *pName, IHashString *pNewParent)
		: m_hashName(pName->GetUniqueID())
		, m_hashNewParent(pNewParent->GetUniqueID())
	{
	};

	///	\brief	execute command
	///	\return	code of EE message result
	virtual DWORD Execute();

private:
	DWORD	m_hashName;
	DWORD	m_hashNewParent;
};

} // namespace GDS_SDK_Helpers

#endif //_GDS_UNDO_ACTIONS_H_
