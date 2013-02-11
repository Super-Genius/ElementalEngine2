///============================================================================
/// \file		WorldEventEditorComponent.cpp
/// \brief		Implementation of WorldEventEditor Component
/// \date		01-30-2007
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

REGISTER_COMPONENT_SINGLETON(CWorldEventEditorComponent);
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CWorldEventEditorComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CWorldEventEditorComponent);
REGISTER_MESSAGE_HANDLER(SelectObject, OnSelectObject, CWorldEventEditorComponent);


IComponent *CWorldEventEditorComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CWorldEventEditorComponent);
}

// serialize function for this object
void CWorldEventEditorComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CWorldEventEditorComponent::GetComponentType()
{
	static CHashString compName(_T("CWorldEventEditorComponent"));
	return &compName;
}

bool CWorldEventEditorComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for Database component (interface to Elemental Engine)
void CWorldEventEditorComponent::SetParent(CWorldEventEditor *parent)
{
	m_Parent = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CWorldEventEditorComponent::OnCmdMsg(DWORD size, void *param)
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
DWORD CWorldEventEditorComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	return MSG_HANDLED_PROCEED;
}

DWORD CWorldEventEditorComponent::OnSelectObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(SELECTOBJECTPARAMS));
	SELECTOBJECTPARAMS *pData = (SELECTOBJECTPARAMS*)param;

	m_Parent->SetSelectedObject( pData->object );

	return MSG_HANDLED_PROCEED;
}
