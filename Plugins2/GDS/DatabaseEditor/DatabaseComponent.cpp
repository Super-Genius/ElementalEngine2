///============================================================================
/// \file		DatabaseComponent.cpp
/// \brief		Header file for Database component
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
REGISTER_COMPONENT_SINGLETON(CDatabaseComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CDatabaseComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CDatabaseComponent);
REGISTER_MESSAGE_HANDLER(FileLoaded_dxl, OnDatabaseLoad, CDatabaseComponent);
REGISTER_MESSAGE_HANDLER(FileLoaded_dbx, OnDefinedDatabaseLoad, CDatabaseComponent);

// get the singleton for this
IComponent *CDatabaseComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CDatabaseComponent);
}

// serialize function for this object
void CDatabaseComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CDatabaseComponent::GetComponentType()
{
	static CHashString compName(_T("CDatabaseComponent"));
	return &compName;
}

bool CDatabaseComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for Database component (interface to Elemental Engine)
void CDatabaseComponent::SetParent(CDatabaseEditor *parent)
{
	m_Parent = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CDatabaseComponent::OnCmdMsg(DWORD size, void *param)
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
DWORD CDatabaseComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	return MSG_HANDLED_PROCEED;
}

// come here when a database file is loaded
DWORD CDatabaseComponent::OnDatabaseLoad(DWORD size, void *param)
{
	LPTSTR name = (LPTSTR)param;
	VERIFY_MESSAGE_SIZE(size, sizeof(LPTSTR));
	m_Parent->EditDocument(name);
	return MSG_HANDLED_PROCEED;
}

// come here when a defined database file is loaded
DWORD CDatabaseComponent::OnDefinedDatabaseLoad(DWORD size, void *param)
{
	LPTSTR name = (LPTSTR)param;
	VERIFY_MESSAGE_SIZE(size, sizeof(LPTSTR));
	m_Parent->EditDocument(name);
	return MSG_HANDLED_PROCEED;
}

