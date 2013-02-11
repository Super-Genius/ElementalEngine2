///============================================================================
/// \file		GUIEditorComponent.cpp
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(CGUIEditorComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CGUIEditorComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CGUIEditorComponent);
REGISTER_MESSAGE_HANDLER(EditGUIEditor, OnEditGUIEditor, CGUIEditorComponent);
REGISTER_MESSAGE_HANDLER(FileLoaded_gxl, OnFileLoadedGXL, CGUIEditorComponent);
REGISTER_MESSAGE_HANDLER(GUIEditorDocOpen, OnGUIEditorDocOpen, CGUIEditorComponent);
REGISTER_MESSAGE_HANDLER(GUIEditorDocClose, OnGUIEditorDocClose, CGUIEditorComponent);
REGISTER_MESSAGE_HANDLER(UpdateGUIAnimBarPos, OnUpdateGUIAnimBarPos, CGUIEditorComponent);

// get the singleton for this
IComponent *CGUIEditorComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CGUIEditorComponent);
}

// serialize function for this object
void CGUIEditorComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CGUIEditorComponent::GetComponentType()
{
	static CHashString compName(_T("CGUIEditorComponent"));
	return &compName;
}

bool CGUIEditorComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for GUIEditor component (interface to Elemental Engine)
void CGUIEditorComponent::SetParent(CGUIEditor *parent)
{
	m_Parent = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CGUIEditorComponent::OnCmdMsg(DWORD size, void *param)
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
DWORD CGUIEditorComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);
	m_View = NULL;
	return MSG_HANDLED_PROCEED;
}

// message to handle editing of GUIEditor document
DWORD CGUIEditorComponent::OnEditGUIEditor(DWORD size, void *param)
{
	// verify message size
//	VERIFY_MESSAGE_SIZE(size, sizeof(XXXXX));

	// *param would contain pointer to loaded document
//	m_Parent->EditDocument(0, 0);

	return MSG_HANDLED_PROCEED;
}

DWORD CGUIEditorComponent::OnFileLoadedGXL(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(LPTSTR));
	LPTSTR fileName = (LPTSTR) param;

	CHashString hashFile((const TCHAR*) fileName);
	GETPAGELIST getPL;
	getPL.fileName = &hashFile;

	static DWORD msgHash_GetPageList = CHashString(_T("GetPageList")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_GetPageList, sizeof(GETPAGELIST), &getPL);
	if (retval == MSG_HANDLED)
	{
		m_Parent->LoadGXL(getPL.gpl, fileName);
	}
	return MSG_HANDLED_PROCEED;
}

DWORD CGUIEditorComponent::OnGUIEditorDocOpen(DWORD size, void *param)
{
	m_Parent->SetDocOpen(true);
	return MSG_HANDLED_STOP;
}

DWORD CGUIEditorComponent::OnGUIEditorDocClose(DWORD size, void *param)
{
	m_Parent->SetDocOpen(false);
	return MSG_HANDLED_STOP;
}

DWORD CGUIEditorComponent::OnUpdateGUIAnimBarPos(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(UINT));
	UINT pos = *(UINT*) param;
	m_Parent->UpdateBarPos(pos);
	return MSG_HANDLED_STOP;
}