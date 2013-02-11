///============================================================================
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

//REGISTER_COMPONENT_SINGLETON(CNodeComponent);
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CNodeComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CNodeComponent);
REGISTER_MESSAGE_HANDLER(SelectObject, OnSelectObject, CNodeComponent);
REGISTER_MESSAGE_HANDLER(LeftClickObject, OnLeftClickObject, CNodeComponent);

IComponent *CNodeComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CNodeComponent);
}

// serialize function for this object
void CNodeComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CNodeComponent::GetComponentType()
{
	static CHashString compName(_T("CNodeComponent"));
	return &compName;
}

bool CNodeComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for Database component (interface to Elemental Engine)
void CNodeComponent::SetParent(CNodeSystemEditor *parent)
{
	m_Parent = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CNodeComponent::OnCmdMsg(DWORD size, void *param)
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
DWORD CNodeComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	return MSG_HANDLED_PROCEED;
}


DWORD CNodeComponent::OnSelectObject(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(SELECTOBJECTPARAMS));
	SELECTOBJECTPARAMS *sop = (SELECTOBJECTPARAMS*)param;

	m_Parent->ObjectSelected( sop->object, sop->compType );

	return MSG_HANDLED_PROCEED;
}

DWORD CNodeComponent::OnLeftClickObject(DWORD size, void *param)
{
	DWORD retVal = MSG_NOT_HANDLED;
	VERIFY_MESSAGE_SIZE( size, sizeof(MOUSECLICKPARAMS) );
	MOUSECLICKPARAMS *message = (MOUSECLICKPARAMS *)param;

	if( message->hitPosition != NULL )
	{
		retVal = m_Parent->HandleEditorLeftClick( message->hitPosition );
	}

	return retVal;
}