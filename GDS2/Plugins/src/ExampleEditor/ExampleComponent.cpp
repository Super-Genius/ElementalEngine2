///============================================================================
/// \file		ExampleComponent.cpp
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
#include "ExampleComponent.hpp"

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(CExampleComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CExampleComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CExampleComponent);
REGISTER_MESSAGE_HANDLER(EditExample, OnEditExample, CExampleComponent);

// get the singleton for this
IComponent *CExampleComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CExampleComponent);
}

// serialize function for this object
void CExampleComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CExampleComponent::GetComponentType()
{
	static CHashString compName(_T("CExampleComponent"));
	return &compName;
}

bool CExampleComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for Example component (interface to Elemental Engine)
void CExampleComponent::SetParent(CExampleEditor *parent)
{
	m_Parent = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CExampleComponent::OnCmdMsg(DWORD size, void *param)
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
DWORD CExampleComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	return MSG_HANDLED_PROCEED;
}

// message to handle editing of example document
DWORD CExampleComponent::OnEditExample(DWORD size, void *param)
{
	// verify message size
//	VERIFY_MESSAGE_SIZE(size, sizeof(XXXXX));

	// *param would contain pointer to loaded document
	m_Parent->EditDocument();

	return MSG_HANDLED_PROCEED;
}

