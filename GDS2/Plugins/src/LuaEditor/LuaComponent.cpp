///============================================================================
/// \file		LuaComponent.cpp
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

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(CLuaComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CLuaComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CLuaComponent);
REGISTER_MESSAGE_HANDLER(FileLoaded_lua, OnScriptLoad, CLuaComponent);
REGISTER_MESSAGE_HANDLER(GetScriptDataSize, OnGetDataSize, CLuaComponent);
REGISTER_MESSAGE_HANDLER(GetScriptData, OnGetEditorData, CLuaComponent);

// get the singleton for this
IComponent *CLuaComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CLuaComponent);
}

CLuaComponent::CLuaComponent()
{
	REGISTER_EDITOR_MESSAGE regEdParams;

	regEdParams.editorName = GetComponentType();

	// since the hashstrings need to be around for the life of the object, we keep
	// the m_EditableObjects class around.
	m_EditableObjects.clear();
	m_EditableObjects.push_back(new CHashString("CLuaScript"));
	regEdParams.objectNames = &m_EditableObjects;

    m_ToolBox = EngineGetToolBox();

	static DWORD msgHash_RegisterEditor = CHashString(_T("RegisterEditor")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterEditor, sizeof(REGISTER_EDITOR_MESSAGE), &regEdParams );
}

CLuaComponent::~CLuaComponent()
{
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
void CLuaComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CLuaComponent::GetComponentType()
{
	static CHashString compName(_T("CLuaComponent"));
	return &compName;
}

bool CLuaComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for Lua component (interface to Elemental Engine)
void CLuaComponent::SetParent(CLuaEditor *parent)
{
	m_Parent = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CLuaComponent::OnCmdMsg(DWORD size, void *param)
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
DWORD CLuaComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	return MSG_HANDLED_PROCEED;
}


DWORD CLuaComponent::OnScriptLoad(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(LPTSTR));
	LPTSTR name = (LPTSTR)param;
	m_Parent->EditDocument(name);
	return MSG_HANDLED_PROCEED;
}

DWORD CLuaComponent::OnGetDataSize(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(SCRIPTPARAMS));
	SCRIPTPARAMS *sp = (SCRIPTPARAMS*)param;
	m_Parent->GetDocSize(sp->dataSize);
	return MSG_HANDLED_PROCEED;
}

DWORD CLuaComponent::OnGetEditorData(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(SCRIPTPARAMS));
	SCRIPTPARAMS *sp = (SCRIPTPARAMS*)param;
	m_Parent->GetDocData(sp->scriptData);
	return MSG_HANDLED_PROCEED;
}


