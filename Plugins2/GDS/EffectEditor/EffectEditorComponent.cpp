///============================================================================
/// \file		EffectEditorComponent.cpp
/// \brief		Implementation of EffectEditor Component
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

REGISTER_COMPONENT_SINGLETON(CEffectEditorComponent);
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CEffectEditorComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CEffectEditorComponent);
REGISTER_MESSAGE_HANDLER(SelectObject, OnSelectObject, CEffectEditorComponent);


IComponent *CEffectEditorComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CEffectEditorComponent);
}

// serialize function for this object
void CEffectEditorComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CEffectEditorComponent::GetComponentType()
{
	static CHashString compName(_T("CEffectEditorComponent"));
	return &compName;
}

bool CEffectEditorComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for Database component (interface to Elemental Engine)
void CEffectEditorComponent::SetParent(CEffectEditor *parent)
{
	m_Parent = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CEffectEditorComponent::OnCmdMsg(DWORD size, void *param)
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
DWORD CEffectEditorComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	return MSG_HANDLED_PROCEED;
}

DWORD CEffectEditorComponent::OnSelectObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(SELECTOBJECTPARAMS));
	SELECTOBJECTPARAMS *pData = (SELECTOBJECTPARAMS*)param;

//	m_Parent->SetSelectedObject( pData->object );

	return MSG_HANDLED_PROCEED;
}
