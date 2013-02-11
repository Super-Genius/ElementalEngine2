///============================================================================
/// \file		ObjectComponent.cpp
/// \brief		Header file for <insert brief description here>
/// \date		04-22-2005
/// \author		Kenneth Hurley
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
///============================================================================

#include "StdAfx.h"
#include "ObjectComponent.hpp"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(CObjectComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CObjectComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CObjectComponent);
REGISTER_MESSAGE_HANDLER(EditObject, OnEditObject, CObjectComponent);
REGISTER_MESSAGE_HANDLER(LeftClickEditorObject, OnLeftClickEditorObject, CObjectComponent);
REGISTER_MESSAGE_HANDLER(LeftClickObject, OnLeftClickObject, CObjectComponent);
REGISTER_MESSAGE_HANDLER(ChangeObjectName, OnChangeObjectName, CObjectComponent);
REGISTER_MESSAGE_HANDLER(MouseMove, OnMouseMove, CObjectComponent);
REGISTER_MESSAGE_HANDLER(ItemDropped, OnItemDropped, CObjectComponent);
REGISTER_MESSAGE_HANDLER(GlobalUpdateTick, OnGlobalUpdateTick, CObjectComponent);
REGISTER_MESSAGE_HANDLER(GetObjectGridToolData, OnGetObjectGridToolData, CObjectComponent);
REGISTER_MESSAGE_HANDLER(EditorMoveObject, OnEditorMoveObject, CObjectComponent);
REGISTER_MESSAGE_HANDLER(SelectObject, OnSelectObject, CObjectComponent);
REGISTER_MESSAGE_HANDLER(EditorGetObjectToolConfig, OnEditorGetObjectToolConfig, CObjectComponent);
REGISTER_MESSAGE_HANDLER(EditorSetObjectToolConfig, OnEditorSetObjectToolConfig, CObjectComponent);


// get the singleton for this
IComponent *CObjectComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CObjectComponent);
}

CObjectComponent::CObjectComponent()
{
	REGISTER_EDITOR_MESSAGE regEdParams;

	m_LastObjectID = 0;
	m_LastCompTypeID = 0;

	regEdParams.editorName = GetComponentType();

	// since the hashstrings need to be around for the life of the object, we keep
	// the m_EditableObjects class around.
	m_EditableObjects.clear();
	m_EditableObjects.push_back(new CHashString("CEntity"));
	regEdParams.objectNames = &m_EditableObjects;

    m_ToolBox=EngineGetToolBox();

	static DWORD msgHash_RegisterEditor = CHashString(_T("RegisterEditor")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterEditor, sizeof(REGISTER_EDITOR_MESSAGE), &regEdParams );

	// create a grid tool render object
	CHashString szGridToolRenderObjectClass(_T("CObjectGridToolRenderObject"));
	CHashString szGridToolRenderObjectName(_T("ObjectGridToolRenderObject"));
	CREATEOBJECTPARAMS cop;
	cop.name = &szGridToolRenderObjectName;
	cop.parentName = NULL;
	cop.typeName = &szGridToolRenderObjectClass;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_CreateObject, sizeof(cop), &cop );
	// init grid tool render object
	INITOBJECTPARAMS iop;
	iop.name = &szGridToolRenderObjectName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
}

CObjectComponent::~CObjectComponent()
{
	CHashString szGridToolRenderObjectName(_T("ObjectGridToolRenderObject"));
	DELETEOBJECTPARAMS dop;
	dop.name = &szGridToolRenderObjectName;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop );

	EDITABLELIST::iterator elIter;
	
	// free up the hash strings associated with this list
	for (elIter = m_EditableObjects.begin(); elIter != m_EditableObjects.end(); ++elIter)
	{
		delete (*elIter);
	}

	// and clear the list now.
	m_EditableObjects.clear();
}

// serialize function for this object
void CObjectComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CObjectComponent::GetComponentType()
{
	static CHashString compName(_T("CObjectComponent"));
	return &compName;
}

bool CObjectComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for Object component (interface to Elemental Engine)
void CObjectComponent::SetParent(CObjectEditor *parent)
{
	m_Parent = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CObjectComponent::OnCmdMsg(DWORD size, void *param)
{
	CMDMESSAGE *pCmdMsg;
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDMESSAGE));
	pCmdMsg = (CMDMESSAGE *)param;

	// forward to parent.
	if (m_Parent->OnCmdMsg(pCmdMsg->m_nID, pCmdMsg->m_nCode, pCmdMsg->m_pExtra,
		pCmdMsg->m_pHandlerInfo))
	{
		pCmdMsg->retVal = true;
		return MSG_HANDLED_PROCEED;
	}

	// it wasn't handled by parent, so return that
	return MSG_NOT_HANDLED;
}

// message to handle WindowsCreate function
DWORD CObjectComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	return MSG_HANDLED_PROCEED;
}

// message to handle editing of Object document
DWORD CObjectComponent::OnEditObject(DWORD size, void *param)
{
	return MSG_HANDLED_PROCEED;
}

DWORD CObjectComponent::OnLeftClickEditorObject(DWORD size, void *param)
{
	CLICKOBJECTPARAMS *mouseParam;	
	VERIFY_MESSAGE_SIZE( size, sizeof(CLICKOBJECTPARAMS) );
	mouseParam = (CLICKOBJECTPARAMS *)param;

	if (m_Parent->GetManipulateState() != MANIPULATE_SELECT)
		return MSG_HANDLED_PROCEED;

	SELECTOBJECTPARAMS sop;
	sop.object = mouseParam->object;
	sop.compType = mouseParam->compType;
	static DWORD msgHash_SelectObject = CHashString(_T("SelectObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SelectObject, sizeof(sop), &sop);

	return MSG_HANDLED_PROCEED;
}

DWORD CObjectComponent::OnChangeObjectName(DWORD size, void *param)
{
	CHANGEOBJNAME *con;
	IHashString *oldName;
	IHashString *newName;
	IHashString *compType;
	VERIFY_MESSAGE_SIZE(size, sizeof(CHANGEOBJNAME));

	con = (CHANGEOBJNAME*)param;
	oldName = con->oldName;
	newName = con->newName;
	compType = con->compType;

	// if the currently edited object is the object w/ the changed name
	if(oldName->GetUniqueID() == m_LastObjectID && compType->GetUniqueID() == m_LastCompTypeID)
	{
		// update the tree with the new name
		m_Parent->UpdateTree(newName, compType);
	}

	return MSG_HANDLED_PROCEED;
}


DWORD CObjectComponent::OnLeftClickObject(DWORD size, void *param)
{
	IHashString *object;
	IHashString *compType;
	VERIFY_MESSAGE_SIZE(size, sizeof(MOUSECLICKPARAMS));
	MOUSECLICKPARAMS *lButtonMess = (MOUSECLICKPARAMS*)param;

	if (m_Parent->GetManipulateState() != MANIPULATE_SELECT)
		return MSG_HANDLED_PROCEED;

	object = lButtonMess->object;
	compType = lButtonMess->compType;

	// select the object in the tree view
	if( object != NULL && compType != NULL )
	{
		SELECTOBJECTPARAMS sop;
		sop.object = object;
		sop.compType = compType;
		static DWORD msgHash_SelectObject = CHashString(_T("SelectObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SelectObject, sizeof(sop), &sop);
	}

	//m_Parent->SetSelected(compType, object);

	// only update tree if the object changes
	if( !object || !compType )
	{
		return MSG_NOT_HANDLED;
	}
	if ((object->GetUniqueID() != m_LastObjectID) || 
			(compType->GetUniqueID() != m_LastCompTypeID))
	{
		m_Parent->UpdateTree(object, compType);
		m_LastObjectID = object->GetUniqueID();
		m_LastCompTypeID = compType->GetUniqueID();
	}

	// Paint/erase on single click
	if (lButtonMess->point)
	{
		m_Parent->SingleClickPaint(lButtonMess->point->x, lButtonMess->point->y);
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CObjectComponent::OnMouseMove(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(MOUSECOORDS));
	MOUSECOORDS *message = (MOUSECOORDS*)param;

	m_Parent->MouseMove(message);

	return MSG_HANDLED_PROCEED;
}

// message to handle OnCmdMsgs from main window
DWORD CObjectComponent::OnItemDropped(DWORD size, void *param)
{
	DROPOBJECTINFO *doi;
	LOADFILEEXTPARAMS lfep;
	CString fileName;
	CString extension;
	VERIFY_MESSAGE_SIZE(size, sizeof(DROPOBJECTINFO));

	// cast out drop object info
	doi = (DROPOBJECTINFO *)param;

	// grab resource page info
	RESOURCEPAGEINFO *rpi = doi->pResourcePageInfo;

	// make sure we are dealing with the proper extension
	extension = m_Parent->GetExtension(rpi->filepath);

	// make sure that the extension is Spawner
	if (extension != ".v3o")
	{
		// message not for the spawn component move on
		return MSG_NOT_HANDLED;
	}

	// strip off the extension of the file
	fileName = m_Parent->GetPathNameNoExt(rpi->filepath);

	fileName += ".sxl";

	// send load message to load the spawner
	lfep.fileName = (TCHAR*)fileName.GetString();
	lfep.bInternalLoad = true;

	static DWORD msgHash_SpawnEditorItem = CHashString(_T("SpawnEditorItem")).GetUniqueID();
    if (MSG_HANDLED == m_ToolBox->SendMessage(msgHash_SpawnEditorItem, sizeof(LOADFILEEXTPARAMS), &lfep))
	{
		CUndoCommandGuard undoCommand;
		UndoForObjectSpawns();
	}

	return MSG_HANDLED_STOP;
}

DWORD CObjectComponent::OnGlobalUpdateTick(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GLOBALTICKPARAMS));

	m_Parent->OnGlobalUpdateTick();

	return MSG_HANDLED_PROCEED;
}

DWORD CObjectComponent::OnGetObjectGridToolData(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(OBJECTGRIDTOOLDATA));
	OBJECTGRIDTOOLDATA *pData = (OBJECTGRIDTOOLDATA*)param;

	pData->bGridActive = m_Parent->IsObjectGridToolActive();
	pData->fGridSize = m_Parent->GetObjectGridToolGridSize();

	return MSG_HANDLED_PROCEED;
}

DWORD CObjectComponent::OnEditorMoveObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(EDITORMOVEOBJECTPARAMS));
	EDITORMOVEOBJECTPARAMS *pData = (EDITORMOVEOBJECTPARAMS*)param;

	m_Parent->MoveObject( pData->pObjectName, pData->vecMove, pData->bUseMoveAccum, 0.0f, 0.0f );

	return MSG_HANDLED_PROCEED;
}

DWORD CObjectComponent::OnSelectObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(SELECTOBJECTPARAMS));
	SELECTOBJECTPARAMS *pData = (SELECTOBJECTPARAMS*)param;

	if( !pData->object )
	{
		m_LastObjectID = 0;
		m_Parent->UpdateTree(NULL, NULL);
		return MSG_HANDLED_PROCEED;
	}

	m_Parent->SetEditableSelected( pData->object );

	// handle the case where we are selecting something but don't know what it is
	if (pData->compType == NULL)
	{
		GETCOMPONENTTYPEPARAMS comptypemsg;
		comptypemsg.name = pData->object;
		static DWORD msgHash_GetComponentType = CHashString(_T("GetComponentType")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetComponentType, sizeof(comptypemsg), &comptypemsg ) == MSG_HANDLED)
		{
			pData->compType = comptypemsg.componenttype;
		}
		else
		{
			return MSG_ERROR;
		}
	}

	m_Parent->SetSelected(pData->compType, pData->object);

	if ((pData->object->GetUniqueID() != m_LastObjectID) || 
			(pData->compType->GetUniqueID() != m_LastCompTypeID))
	{
		m_Parent->UpdateTree(pData->object, pData->compType);
		m_LastObjectID = pData->object->GetUniqueID();
		m_LastCompTypeID = pData->compType->GetUniqueID();
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CObjectComponent::OnEditorGetObjectToolConfig(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(EDITORGETOBJECTTOOLCONFIGPARAMS));
	EDITORGETOBJECTTOOLCONFIGPARAMS *pData = (EDITORGETOBJECTTOOLCONFIGPARAMS*)param;

	pData->bActive = m_Parent->IsObjectToolActive();
	pData->objectManipulateState = (MANIPULATE_STATE)m_Parent->GetManipulateState();
	pData->objectLockState = (LOCK_STATE)m_Parent->GetObjectMoveLockIndex();

	return MSG_HANDLED_PROCEED;
}

DWORD CObjectComponent::OnEditorSetObjectToolConfig(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(EDITORSETOBJECTTOOLCONFIGPARAMS));
	EDITORSETOBJECTTOOLCONFIGPARAMS *pData = (EDITORSETOBJECTTOOLCONFIGPARAMS*)param;

	if (pData->modifyFlags & OTCONFIG_MODIFY_MANIPULATE_STATE)
	{
		m_Parent->SetManipulateState(pData->objectManipulateState);
	}
	if (pData->modifyFlags & OTCONFIG_MODIFY_LOCK_STATE)
	{
		m_Parent->SetObjectMoveLockIndex(pData->objectLockState);
	}

	return MSG_HANDLED_PROCEED;
}
