///============================================================================
/// \file		LightmapComponent.cpp
/// \brief		Implementation of Lightmap Editor Component
/// \date		09-07-2005
/// \author		Marvin Gouw
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
#include "LightmapComponent.h"

REGISTER_COMPONENT_SINGLETON(CLightmapComponent);
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CLightmapComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CLightmapComponent);


IComponent *CLightmapComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CLightmapComponent);
}

// serialize function for this object
void CLightmapComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CLightmapComponent::GetComponentType()
{
	static CHashString compName(_T("CLightmapComponent"));
	return &compName;
}

bool CLightmapComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for Database component (interface to Elemental Engine)
void CLightmapComponent::SetParent(CLightmapEditor *parent)
{
	m_Parent = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CLightmapComponent::OnCmdMsg(DWORD size, void *param)
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
DWORD CLightmapComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	return MSG_HANDLED_PROCEED;
}
