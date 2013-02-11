///============================================================================
/// \file		TimeOfDayComponent.cpp
/// \brief		Implementation of TimeOfDay Editor Component
/// \date		08-12-2005
/// \author		D. Patrick Ghiocel
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
#include "TimeOfDayComponent.h"

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
//REGISTER_COMPONENT_SINGLETON(CTimeOfDayComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CTimeOfDayComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CTimeOfDayComponent);
REGISTER_MESSAGE_HANDLER(GlobalUpdateTick, OnGlobalUpdateTick, CTimeOfDayComponent);

// get the singleton for this
IComponent *CTimeOfDayComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CTimeOfDayComponent);
}

// serialize function for this object
void CTimeOfDayComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CTimeOfDayComponent::GetComponentType()
{
	static CHashString compName(_T("CTimeOfDayComponent"));
	return &compName;
}

bool CTimeOfDayComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for Database component (interface to Elemental Engine)
void CTimeOfDayComponent::SetParent(CTimeOfDayEditor *parent)
{
	m_Parent = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CTimeOfDayComponent::OnCmdMsg(DWORD size, void *param)
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
DWORD CTimeOfDayComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	return MSG_HANDLED_PROCEED;
}

DWORD CTimeOfDayComponent::OnGlobalUpdateTick(DWORD size, void *param)
{
    VERIFY_MESSAGE_SIZE(size, sizeof(GLOBALTICKPARAMS));
	
	m_Parent->OnGlobalUpdateTick();
	
	return MSG_HANDLED_PROCEED;
}