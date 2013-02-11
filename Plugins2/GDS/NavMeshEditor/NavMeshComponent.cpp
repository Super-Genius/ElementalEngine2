///============================================================================
/// \file		NavMeshComponent.cpp
/// \brief		Implementation of NavMesh Editor Component
/// \date		09-07-2005
/// \author		Brian Bazyk
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
#include "NavMeshComponent.h"

REGISTER_COMPONENT_SINGLETON(CNavMeshComponent);
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CNavMeshComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CNavMeshComponent);


IComponent *CNavMeshComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CNavMeshComponent);
}

// serialize function for this object
void CNavMeshComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CNavMeshComponent::GetComponentType()
{
	static CHashString compName(_T("CNavMeshComponent"));
	return &compName;
}

bool CNavMeshComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for Database component (interface to Elemental Engine)
void CNavMeshComponent::SetParent(CNavMeshEditor *parent)
{
	m_Parent = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CNavMeshComponent::OnCmdMsg(DWORD size, void *param)
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
DWORD CNavMeshComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	return MSG_HANDLED_PROCEED;
}
