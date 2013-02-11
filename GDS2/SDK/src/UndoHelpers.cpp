///=====================================================================
/// \file	UndoHelpers.cpp
/// \brief	Implementation of helper functions and classes for 
///			Undo functionlaity in GDS.
/// \date	10/03/2007
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

namespace GDS_SDK_Helpers
{

///	\brief	create new undo stack
///	\param	stackName - name of the undo stack
void CreateUndoStack(IHashString *stackName)
{
	UNDOSTACKINFO info;
	info.pStackName = stackName;
	static DWORD msgHash_UndoCreateStack = CHashString(_T("UndoCreateStack")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_UndoCreateStack, sizeof(info), &info);
}

///	\brief	delete undo stack
///	\param	stackName - name of the undo stack
void DeleteUndoStack(IHashString *stackName)
{
	UNDOSTACKINFO info;
	info.pStackName = stackName;
	static DWORD msgHash_UndoDeleteStack = CHashString(_T("UndoDeleteStack")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_UndoDeleteStack, sizeof(info), &info);
}

///	\brief	rename undo stack
///	\param	stackName - name of the undo stack
///	\param	newStackName - new name for the stack
void RenameUndoStack(IHashString *stackName, IHashString *newStackName)
{
	UNDOSTACKRENAME rename;
	rename.pStackName = stackName;
	rename.pNewStackName = newStackName;
	static DWORD msgHash_UndoRenameStack = CHashString(_T("UndoRenameStack")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_UndoRenameStack, sizeof(rename), &rename);
}

///	\brief	register new association between view and undo stack
///	\param	view - pointer to CView
///	\param	stackName - name of the undo stack
void RegisterViewUndoStack(CView *view, IHashString *stackName)
{
	UNDOVIEWTOSTACK vts;
	vts.pStackView = view;
	vts.pStackName = stackName;
	static DWORD msgHash_UndoRegisterViewStack = CHashString(_T("UndoRegisterViewStack")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_UndoRegisterViewStack, sizeof(vts), &vts);
}

///	\brief	clear undo stack
///	\param	stackName - name of the undo stack
///					stack of current view is cleared if this value is NULL 
void ClearUndoStack(IHashString *stackName)
{
	UNDOSTACKINFO info;
	info.pStackName = stackName;
	static DWORD msgHash_UndoClearStack = CHashString(_T("UndoClearStack")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_UndoClearStack, sizeof(info), &info);
}

CUndoCommandGuard::CUndoCommandGuard()
	: m_bCancelled(false)
{
	static DWORD msgHash_UndoStartCommand = CHashString(_T("UndoStartCommand")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_UndoStartCommand, 0, NULL);
}

CUndoCommandGuard::~CUndoCommandGuard()
{
	if (!m_bCancelled)
	{
		static DWORD msgHash_UndoEndCommand = CHashString(_T("UndoEndCommand")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_UndoEndCommand, 0, NULL);
	}
}

void CUndoCommandGuard::Cancel()
{
	static DWORD msgHash_UndoCancelCommand = CHashString(_T("UndoCancelCommand")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_UndoCancelCommand, 0, NULL);
	m_bCancelled = true;
}

///	\brief	constructor of helper class
///	\param	name - pointer to IHashString with name of object to process
///					in undo/redo
CObjectsStateUndo::CObjectsStateUndo(IHashString *name)
	: m_hszName(name)
{
	static DWORD msgHash_UndoSaveObject = CHashString(_T("UndoSaveObject")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_UndoSaveObject, sizeof(IHashString), &m_hszName);
}

CObjectsStateUndo::~CObjectsStateUndo()
{
	static DWORD msgHash_RedoSaveObject = CHashString(_T("RedoSaveObject")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_RedoSaveObject, sizeof(IHashString), &m_hszName);
}

///	\brief	constructor of helper class
///	\param	name - pointer to IHashString with name of object to process
///					in undo/redo
CTextureDataUndo::CTextureDataUndo(IHashString *pFilePath)
	: m_hszFilePath(pFilePath)
{
	static DWORD msgHash_UndoSaveTexture = CHashString(_T("UndoSaveTexture")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_UndoSaveTexture, sizeof(IHashString), &m_hszFilePath);
}

CTextureDataUndo::~CTextureDataUndo()
{
	static DWORD msgHash_RedoSaveTexture = CHashString(_T("RedoSaveTexture")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_RedoSaveTexture, sizeof(IHashString), &m_hszFilePath);
}

CUndoObjectGroup::CUndoObjectGroup()
	: m_bUndoCommand(false)
{
}

CUndoObjectGroup::~CUndoObjectGroup()
{
	EndUndoCommand();
}

///	\brief	helper for starting new undo command. This function may be called 
///			multiple times in any suitable places
void CUndoObjectGroup::StartUndoCommand()
{
	if (!m_bUndoCommand)
	{
		static DWORD msgHash_UndoStartCommand = CHashString(_T("UndoStartCommand")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_UndoStartCommand, 0, NULL);
		m_bUndoCommand = true;
	}
}

///	\brief	helper for ending undo command. It may be placed in any suitable 
///			places and without prior StartUndoCommand call
void CUndoObjectGroup::EndUndoCommand()
{
	// finalize undo command if needed
	if (m_bUndoCommand)
	{
		static DWORD msgHash_RedoSaveTexture = CHashString(_T("RedoSaveTexture")).GetUniqueID();
		SendUndoMessages(msgHash_RedoSaveTexture, m_UndoTextures);
		m_UndoTextures.clear();

		static DWORD msgHash_RedoSaveObject = CHashString(_T("RedoSaveObject")).GetUniqueID();
		SendUndoMessages(msgHash_RedoSaveObject, m_UndoObjects);
		m_UndoObjects.clear();

		static DWORD msgHash_UndoEndCommand = CHashString(_T("UndoEndCommand")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_UndoEndCommand, 0, NULL);
		m_bUndoCommand = false;
	}
}

///	\brief	cancel started undo command
void CUndoObjectGroup::CancelUndoCommand()
{
	// finalize undo command if needed
	if (m_bUndoCommand)
	{
		m_UndoTextures.clear();
		m_UndoObjects.clear();

		static DWORD msgHash_UndoCancelCommand = CHashString(_T("UndoCancelCommand")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_UndoCancelCommand, 0, NULL);
		m_bUndoCommand = false;
	}
}

///	\brief	helper for adding object to undo stack. 
///			Name hash is placed in the object list for filling redo data also
///			The same name may be passed multiple times before EndUndoCommand
///			call, only first call with such name does state saving
///	\param	name - pointer to IHashStirng with object name
///	\return	true if object state was added to stack successfully
bool CUndoObjectGroup::AddUndoObject(IHashString *name)
{
	// serialize current object state to undo if needed
	DWORD dwHash = name->GetUniqueID();
	if (m_UndoObjects.end() == find(m_UndoObjects.begin(), m_UndoObjects.end(), dwHash))
	{
		StartUndoCommand();
		static DWORD msgHash_UndoSaveObject = CHashString(_T("UndoSaveObject")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_UndoSaveObject, sizeof(IHashString), name);
		m_UndoObjects.push_back(dwHash);
		return true;
	}
	return false;
}

///	\brief	helper for adding texture to undo stack. 
///			Name hash is placed in the object list for filling redo data also
///	\param	name - pointer to IHashStirng with object name
///	\return	true if texture data was added to stack successfully
bool CUndoObjectGroup::AddUndoTexture(IHashString *name)
{
	// serialize current object state to undo if needed
	DWORD dwHash = name->GetUniqueID();
	if (m_UndoTextures.end() == find(m_UndoTextures.begin(), m_UndoTextures.end(), dwHash))
	{
		StartUndoCommand();
		static DWORD msgHash_UndoSaveTexture = CHashString(_T("UndoSaveTexture")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_UndoSaveTexture, sizeof(IHashString), name);
		m_UndoTextures.push_back(dwHash);
		return true;
	}
	return false;
}

///	\brief	check if object was placed to list for redo operations
///	\param	name - pointer to IHashStirng with object name
///	\return	true if object data is present in the list
bool CUndoObjectGroup::HasUndoObject(IHashString *name) const
{
	DWORD dwHash = name->GetUniqueID();
	return (m_UndoObjects.end() != find(m_UndoObjects.begin(), m_UndoObjects.end(), dwHash));
}

///	\brief	helper for registering action for adding texture to redo stack 
///			on EndCommand call. Name hash is placed in the object list for 
///			filling redo data
///	\param	name - pointer to IHashStirng with object name
void CUndoObjectGroup::RegisterRedoTexture(IHashString *name)
{
	// serialize current object state to undo if needed
	DWORD dwHash = name->GetUniqueID();
	if (m_UndoTextures.end() == find(m_UndoTextures.begin(), m_UndoTextures.end(), dwHash))
	{
		StartUndoCommand();
		m_UndoTextures.push_back(dwHash);
	}
}

///	\brief	helper for processing list with name hashes
///	\param	msgHash - EE message hash
///	\param	list - reference to list with string hashes for processing
void CUndoObjectGroup::SendUndoMessages(DWORD msgHash, const vector<DWORD> &list)
{
	IToolBox *toolBox = EngineGetToolBox();
	vector<DWORD>::const_reverse_iterator it = list.rbegin();
	for (; it != list.rend(); ++it)
	{
		LPCTSTR szName = toolBox->GetHashString(*it);
		if (szName != NULL)
		{
			CHashString hszName = szName;
			toolBox->SendMessage(msgHash, sizeof(IHashString), &hszName);
		}
	}
}

///	\brief	add selection action to undo or redo stack. It frees passed action
///			if action was not added to the stack
///	\param	action - pointer to action object
///	\return	EE return code for added message
DWORD AddUndoAction(IUndoRedoAction *action)
{
	IToolBox *toolBox = EngineGetToolBox();
	static const DWORD msgHash = CHashString(_T("UndoAddAction")).GetUniqueID();
	DWORD res = toolBox->SendMessage(msgHash, sizeof(IUndoRedoAction), action);
	if (MSG_HANDLED != res)
	{
		delete action;
	}
	return res;
}

///	\brief	add selection action to redo stack. It frees passed action
///			if action was not added to the stack
///	\param	action - pointer to action object
///	\return	EE return code for added message
DWORD AddRedoAction(IUndoRedoAction *action)
{
	IToolBox *toolBox = EngineGetToolBox();
	static const DWORD msgHash = CHashString(_T("RedoAddAction")).GetUniqueID();
	DWORD res = toolBox->SendMessage(msgHash, sizeof(IUndoRedoAction), action);
	if (MSG_HANDLED != res)
	{
		delete action;
	}
	return res;
}

///	\brief	add selection action to undo stack
///	\param	name - name of the object
///	\param	bUndo - defines undo or redo stack as target of action
void UndoForSelectObject(IHashString *name, bool bUndo)
{
	IUndoRedoAction *action = new CObjectSelectAction(name);
	if (bUndo)
	{
		AddUndoAction(action);
	}
	else
	{
		AddRedoAction(action);
	}
}

///	\brief	add SetSectorModified action to undo stack
///	\param	sectorName - name of the sector
void UndoForSectorModified(IHashString *sectorName)
{
	IUndoRedoAction *action = NULL;
	// adding undo action
	action = new CSetSectorModifiedAction(sectorName);
	AddUndoAction(action);
	// adding redo action
	action = new CSetSectorModifiedAction(sectorName);
	AddRedoAction(action);
}

///	\brief	add undo actions to stack for creating object on redo and deleting it on undo
///			this function should be called just after object creation
///	\param	parent - name of object's parent
///	\param	name - name of the object
///	\param	type - type of the created object. This value may be NULL, 
///					function determines object type in this case
void UndoForCreateObject(IHashString *parent, IHashString *name, IHashString *type)
{
	IToolBox *toolBox = EngineGetToolBox();

	// try to add create object redo action to stack
	IUndoRedoAction *action = new CObjectCreateAction(parent, name, type);
	DWORD res = AddRedoAction(action);
	if (MSG_HANDLED == res)
	{
		// saving object state for restoring after recreation on redo
		static DWORD msgHash_RedoSaveObject = CHashString(_T("RedoSaveObject")).GetUniqueID();
		res = toolBox->SendMessage(msgHash_RedoSaveObject, sizeof(IHashString), name);
		assert(res == MSG_HANDLED);

		// then trying to add delete object action for undo
		action = new CObjectDeleteAction(name);
		AddUndoAction(action);
	}
}

///	\brief	add undo actions to stack for deleting object on redo and recreating
///			it on undo. This function should be called before object deleting
///	\param	name - name of the object
///	\param	parent - name of object's parent. This value is used when 
///						  object is not in the hierarchy
///	\param	type - type of the created object. This value may be NULL, 
///					function determines object type in this case. This value is 
///					used when object is not in the hierarchy
void UndoForDeleteObject(IHashString *name, IHashString *parent, IHashString *type)
{
	IToolBox *toolBox = EngineGetToolBox();
	static DWORD msgHash_UndoSaveObject = CHashString(_T("UndoSaveObject")).GetUniqueID();

	// collect child objects for recreation on undo
	vector<IObject *> children;
	DWORD res = GetEEObjectChildren(name, children);
	if (MSG_HANDLED != res)
	{
		// this object is not from hierarchy
		toolBox->SendMessage(msgHash_UndoSaveObject, sizeof(IHashString), name);
		// try to add create object undo action to stack
		IUndoRedoAction *action = new CObjectCreateAction(parent, name, type);
		AddUndoAction(action);
	}
	else
	{
		for (vector<IObject *>::reverse_iterator it = children.rbegin(); it != children.rend(); ++it)
		{
			IObject *pObject = *it;
			IHashString *pObjName = pObject->GetName();
			// saving object state for restoring after recreation on redo
			DWORD res = toolBox->SendMessage(msgHash_UndoSaveObject, sizeof(IHashString), pObjName);
			if (MSG_HANDLED == res)
			{
				IHashString *pObjParentName = pObject->GetParentName();
				IHashString *pObjType = pObject->GetComponentType();
				// try to add create object redo action to stack
				IUndoRedoAction *action = new CObjectCreateAction(pObjParentName, pObjName, pObjType);
				AddUndoAction(action);
			}
		}
	}

	// then trying to add delete object action for undo
	IUndoRedoAction *pDeleteAction = new CObjectDeleteAction(name);
	AddRedoAction(pDeleteAction);
}

///	\brief	add undo actions to undo stack for renaming object
///	\param	name - name of the object
///	\param	newName - new name of the object
void UndoForRenameObject(IHashString *name, IHashString *newName)
{
	IToolBox *toolBox = EngineGetToolBox();
	// try to add create object redo action to stack
	IUndoRedoAction *action = new CObjectRenameAction(newName, name);
	AddUndoAction(action);

	action = new CObjectRenameAction(name, newName);
	AddRedoAction(action);
}

///	\brief	add undo actions to undo stack for changing parent
///	\param	name - name of the object
///	\param	oldParent - current parent of the object
///	\param	newParent - new parent of the object
void UndoForChangeParent(IHashString *name, IHashString *oldParent, IHashString *newParent)
{
	IToolBox *toolBox = EngineGetToolBox();
	// try to add create object undo action to stack
	IUndoRedoAction *action = new CObjectChangeParentAction(name, oldParent);
	AddUndoAction(action);
	// try to add create object redo action to stack
	action = new CObjectChangeParentAction(name, newParent);
	AddRedoAction(action);
}

///	\brief	add undo actions to undo stack for last spawned file
///	\return	hash string with name of the spawned object
CHashString UndoForObjectSpawns()
{
	IToolBox *toolBox = EngineGetToolBox();
	CHashString spawnedName(_T(""));
	// TODO: Process spawner files with multiple root entities
	// get last entity created
	static DWORD msgHash_GetSpawnedName = CHashString(_T("GetSpawnedName")).GetUniqueID();
	if (MSG_HANDLED == toolBox->SendMessage(msgHash_GetSpawnedName, sizeof(IHashString), &spawnedName))
	{
		vector<IObject *> children;
		if (MSG_HANDLED == GetEEObjectChildren(&spawnedName, children))
		{
			// saving actions for undo/redo command
			for (vector<IObject *>::iterator it = children.begin(); it != children.end(); ++it)
			{
				IObject *pObject = *it;
				IHashString *name = pObject->GetName();
				IHashString *parent = pObject->GetParentName();
				IHashString *type = pObject->GetComponentType();
				UndoForCreateObject(parent, name, type);
			}
		}
	}

	return spawnedName;
}

} // namespace GDS_SDK_Helpers
