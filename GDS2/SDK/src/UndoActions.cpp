///=====================================================================
/// \file	UndoActions.cpp
/// \brief	Implementations of simple undo actions for common usage
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
#include "stdafx.h"

namespace GDS_SDK_Helpers
{

/////////////// CBaseUndoRedoAction ////////////////////////////////////////////
IToolBox *CBaseUndoRedoAction::m_pToolBox = EngineGetToolBox();

///	\brief	initialize this action
///			the action cannot be executed if result is differ from MSG_HANDLED
///	\return	code of EE message result
DWORD CBaseUndoRedoAction::Initialize()
{
	return MSG_HANDLED;
}

///	\brief	determine useless undo action
///			undo action checks against current state
///	\return	true if no changes exists
bool CBaseUndoRedoAction::HasChanges() const
{
	return true;
}

/////////////// CObjectSelectAction ////////////////////////////////////////////
///	\brief	execute command
///	\return	code of EE message result
DWORD CObjectSelectAction::Execute()
{
	CHashString hszName = m_pToolBox->GetHashString(m_hashName);
	
	SELECTOBJECTPARAMS sop;
	// reset selection first
	static DWORD msgHash_SelectObject = CHashString(_T("SelectObject")).GetUniqueID();
	m_pToolBox->SendMessage(msgHash_SelectObject, sizeof(sop), &sop);
	// then set new selected object
	sop.object = &hszName;
	m_pToolBox->SendMessage(msgHash_SelectObject, sizeof(sop), &sop);

	return MSG_HANDLED;
}

/////////////// CSetSectorModifiedAction ///////////////////////////////////////
///	\brief	execute command
///	\return	code of EE message result
DWORD CSetSectorModifiedAction::Execute()
{
	CHashString hszSectorName = m_pToolBox->GetHashString(m_hashName);

	static DWORD msgHash_SetSectorModified = CHashString(_T("SetSectorModified")).GetUniqueID();
	m_pToolBox->SendMessage(msgHash_SetSectorModified, sizeof(IHashString), &hszSectorName);

	return MSG_HANDLED;
}

/////////////// CObjectCreateAction ////////////////////////////////////////////

CObjectCreateAction::CObjectCreateAction(IHashString *pParentName, IHashString *pName, IHashString *pType)
	: m_hashParentName(0)
	, m_hashName(pName->GetUniqueID())
	, m_hashType(0)
{
	if (pParentName != NULL)
	{
		m_hashParentName = pParentName->GetUniqueID();
	}
	if (pType != NULL)
	{
		m_hashType = pType->GetUniqueID();
	}
}

///	\brief	initialize this action
///			the action cannot be executed if result is differ from MSG_HANDLED
///	\return	code of EE message result
DWORD CObjectCreateAction::Initialize()
{
	IToolBox *pToolBox = EngineGetToolBox();
	
	// check that passed object was created successfully
	CHashString hszName = pToolBox->GetHashString(m_hashName);
	if (!DoesEEObjectExist(&hszName))
	{
		pToolBox->Log(LOGERROR, _T("Failed to intialize undo action. Object %s was not found\n"));
		return MSG_ERROR;
	}

	// extracting object type if it was not passed
	CHashString hszType;
	if (0 == m_hashType)
	{
		hszType = GetComponentType(&hszName);
		if (hszType == _T(""))
		{
			// cannot determine object type
			return MSG_ERROR;
		}
		m_hashType = hszType.GetUniqueID();
	}
	else
	{
		hszType = pToolBox->GetHashString(m_hashType);
	}

	return MSG_HANDLED;
}

///	\brief	execute command
///	\return	code of EE message result
DWORD CObjectCreateAction::Execute()
{
	IToolBox *pToolBox = EngineGetToolBox();

	// preparing parameters for create object 
	CHashString hszName = pToolBox->GetHashString(m_hashName);
	CHashString hszType = pToolBox->GetHashString(m_hashType);

	CREATEOBJECTPARAMS cop;
	cop.typeName = &hszType;
	cop.name = &hszName;
	cop.parentName = NULL;

	CHashString hszParentName;
	if (0 != m_hashParentName)
	{
		hszParentName = pToolBox->GetHashString(m_hashParentName);
		cop.parentName = &hszParentName;
	}

	// then creating it
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	DWORD res = pToolBox->SendMessage(msgHash_CreateObject, sizeof(cop), &cop);
	if (res != MSG_HANDLED)
	{
		// log error
		pToolBox->SetErrorValue(WARN_OBJECT_NOT_FOUND);
		pToolBox->Log(LOGWARNING, _T("%s(%i): Type '%s' not found, create failed!\n"), __FILE__, __LINE__, hszType.GetString());
	}
	return res;
}

///	\brief	determine useless undo action
///			undo action checks against current state
///	\return	true if no changes exists
bool CObjectCreateAction::HasChanges() const
{
	// TODO: implement correct check
	return true;
}

/////////////// CObjectRecreateAction //////////////////////////////////////////
///	\brief	CObjectRecreateAction constructor
///	\param	pParentName - name of object's parent
///	\param	pName - name of the object
///	\param	pType - type of the created object. This value may be NULL, 
///					function determines object type in this case
CObjectRecreateAction::CObjectRecreateAction(IHashString *pParentName, IHashString *pName, IHashString *pType)
	: CObjectCreateAction(pParentName, pName, pType)
{
}

///	\brief	execute command
///	\return	code of EE message result
DWORD CObjectRecreateAction::Execute()
{
	CHashString hszName = EngineGetToolBox()->GetHashString(m_hashName);
	if (!DoesEEObjectExist(&hszName))
	{
		return CObjectCreateAction::Execute();
	}
	return MSG_HANDLED;
}

/////////////// CObjectDeleteAction ////////////////////////////////////////////
///	\brief	execute command
///	\return	code of EE message result
DWORD CObjectDeleteAction::Execute()
{
	CHashString hszName = m_pToolBox->GetHashString(m_hashName);
	DeleteEEObject(&hszName);
	return MSG_HANDLED;
}

/////////////// CObjectRenameAction ////////////////////////////////////////////
///	\brief	execute command
///	\return	code of EE message result
DWORD CObjectRenameAction::Execute()
{
	// preparing parameters for create object 
	CHashString hszName = m_pToolBox->GetHashString(m_hashName);
	CHashString hszNewName = m_pToolBox->GetHashString(m_hashNewName);
	CHANGEOBJNAME con;
	con.oldName = &hszName;
	con.newName = &hszNewName;
	static DWORD msgHash_RenameObject = CHashString(_T("RenameObject")).GetUniqueID();
	m_pToolBox->SendMessage(msgHash_RenameObject, sizeof(con), &con);

	SELECTOBJECTPARAMS sop;
	sop.object = &hszNewName;
	static DWORD msgHash_SelectObject = CHashString(_T("SelectObject")).GetUniqueID();
	m_pToolBox->SendMessage(msgHash_SelectObject, sizeof(sop), &sop);

	return MSG_HANDLED;
}

/////////////// CObjectChangeParentAction //////////////////////////////////////
///	\brief	execute command
///	\return	code of EE message result
DWORD CObjectChangeParentAction::Execute()
{
	// preparing parameters for create object 
	CHashString hszName = m_pToolBox->GetHashString(m_hashName);
	CHashString hszNewParent = m_pToolBox->GetHashString(m_hashNewParent);
	CHANGEOBJECTPARENT cop;
	cop.objectName = &hszName;
	cop.objectCompType = NULL;
	cop.newParentName = &hszNewParent;

	static DWORD msgHash_ChangeObjectParent = CHashString(_T("ChangeObjectParent")).GetUniqueID();
	m_pToolBox->SendMessage(msgHash_ChangeObjectParent, sizeof(cop), &cop);

	SELECTOBJECTPARAMS sop;
	sop.object = &hszName;
	static DWORD msgHash_SelectObject = CHashString(_T("SelectObject")).GetUniqueID();
	m_pToolBox->SendMessage(msgHash_SelectObject, sizeof(sop), &sop);

	return MSG_HANDLED;
}

} // namespace GDS_SDK_Helpers
