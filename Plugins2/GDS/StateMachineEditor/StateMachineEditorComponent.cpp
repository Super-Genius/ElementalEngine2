///============================================================================
/// \file		StateMachineEditorComponent.cpp
/// \brief		Header file for <insert brief description here>
/// \date		03-05-2007
/// \author		Richard Nguyen
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(CStateMachineEditorComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CStateMachineEditorComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CStateMachineEditorComponent);
REGISTER_MESSAGE_HANDLER(FileLoaded_xms, OnFileLoadedXMS, CStateMachineEditorComponent);
REGISTER_MESSAGE_HANDLER(SelectObject, OnSelectObject, CStateMachineEditorComponent);
REGISTER_MESSAGE_HANDLER_PRIORITY(InitObject, OnInitObject, CStateMachineEditorComponent, 0xa0000000);
REGISTER_MESSAGE_HANDLER_PRIORITY(SerializeObject, OnValidateObject, CStateMachineEditorComponent, 0x70000000);

// get the singleton for this
IComponent *CStateMachineEditorComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CStateMachineEditorComponent);
}

// serialize function for this object
void CStateMachineEditorComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CStateMachineEditorComponent::GetComponentType()
{
	static CHashString compName(_T("CStateMachineEditorComponent"));
	return &compName;
}

bool CStateMachineEditorComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for StateMachineEditor component (interface to Elemental Engine)
void CStateMachineEditorComponent::SetEditorInstance(CStateMachineEditor *parent)
{
	m_StateMachineEditor = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CStateMachineEditorComponent::OnCmdMsg(DWORD size, void *param)
{
	CMDMESSAGE *pCmdMsg;
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDMESSAGE));
	pCmdMsg = (CMDMESSAGE *)param;

	// forward to parent.
	if (m_StateMachineEditor->OnCmdMsg(pCmdMsg->m_nID, pCmdMsg->m_nCode, pCmdMsg->m_pExtra,
		pCmdMsg->m_pHandlerInfo))
	{
		pCmdMsg->retVal = true;
		return MSG_HANDLED_PROCEED;
	}

	// it wasn't handled by parent, so return that
	return MSG_NOT_HANDLED;
}

// message to handle WindowsCreate function
DWORD CStateMachineEditorComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_StateMachineEditor->OnCreate((CMDIFrameWnd *)param);
//	m_View = NULL;
	return MSG_HANDLED_PROCEED;
}

DWORD CStateMachineEditorComponent::OnFileLoadedXMS(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(LPTSTR));
	LPTSTR fileName = (LPTSTR) param;

	CHashString hashFile((const TCHAR*) fileName);

	m_StateMachineEditor->LoadXMS( &hashFile );

	return MSG_HANDLED_PROCEED;
}

DWORD CStateMachineEditorComponent::OnInitObject(DWORD size, void *param)
{
	INITOBJECTPARAMS *pIOP = static_cast<INITOBJECTPARAMS *>( param );
	VERIFY_MESSAGE_SIZE(size, sizeof(LPTSTR));
	if (pIOP->name != NULL)
	{
		m_StateMachineEditor->OnInitObject(pIOP->name);
	}
	return MSG_HANDLED;
}

DWORD CStateMachineEditorComponent::OnSelectObject(DWORD size, void * param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(SELECTOBJECTPARAMS));
	SELECTOBJECTPARAMS *pData = (SELECTOBJECTPARAMS*)param;
	m_StateMachineEditor->SelectObject(pData->object);
	return MSG_HANDLED_PROCEED;
}

DWORD CStateMachineEditorComponent::OnValidateObject(DWORD size, void* param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(SERIALIZEOBJECTPARAMS));
	SERIALIZEOBJECTPARAMS *sop = (SERIALIZEOBJECTPARAMS*)param;
	if (sop->archive->IsReading())
	{
		return m_StateMachineEditor->ValidateObject(sop);
	}
	return MSG_NOT_HANDLED;
}
