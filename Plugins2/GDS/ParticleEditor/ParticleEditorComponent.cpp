///============================================================================
/// \file		ParticleEditor.cpp
/// \brief		Implementation of ParticleEditor Editor Component
/// \date		08-24-2005
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
#include "ParticleEditorComponent.h"

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(CParticleEditorComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CParticleEditorComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CParticleEditorComponent);
REGISTER_MESSAGE_HANDLER(FileLoaded_pml, OnFileLoaded, CParticleEditorComponent);
REGISTER_MESSAGE_HANDLER(GlobalUpdateTick, OnGlobalUpdateTick, CParticleEditorComponent);

// get the singleton for this
IComponent *CParticleEditorComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CParticleEditorComponent);
}

// serialize function for this object
void CParticleEditorComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CParticleEditorComponent::GetComponentType()
{
	static CHashString compName(_T("CParticleEditor"));
	return &compName;
}

bool CParticleEditorComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for Database component (interface to Elemental Engine)
void CParticleEditorComponent::SetParent(CParticleEditor *parent)
{
	m_Parent = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CParticleEditorComponent::OnCmdMsg(DWORD size, void *param)
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
DWORD CParticleEditorComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	return MSG_HANDLED_PROCEED;
}

DWORD CParticleEditorComponent::OnFileLoaded(DWORD size, void *param)
{
	// verify message size
	LPTSTR p;
//	VERIFY_MESSAGE_SIZE(size, sizeof(XXXXX));
	p = (LPTSTR)param;
	CHashString filename(p);
	// *param would contain pointer to loaded document
	m_Parent->EditDocument((LPTSTR)filename.GetString());
	return MSG_HANDLED_PROCEED;
}

void CParticleEditorComponent::RegisterView(CParticleRenderView *pView)
{
	ASSERT(pView != NULL);
	m_ViewList.push_front(pView);
}

void CParticleEditorComponent::DeRegisterView(CParticleRenderView *pView)
{
	ASSERT(std::count(m_ViewList.begin(), m_ViewList.end(), pView) > 0);
	m_ViewList.remove(pView);
}

DWORD CParticleEditorComponent::OnGlobalUpdateTick(DWORD size, void *param)
{
    VERIFY_MESSAGE_SIZE(size, sizeof(GLOBALTICKPARAMS));

	// TODO: Collect view from CMultiDocTemplate?
	// Cycle through Registered MFC classes to send them
	// EE Engine Update calls.
	std::for_each(m_ViewList.begin(), m_ViewList.end(), mem_fun<void, CParticleRenderView>(&CParticleRenderView::Update));

	return MSG_HANDLED_PROCEED;
}