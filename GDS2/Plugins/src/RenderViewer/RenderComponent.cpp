///============================================================================
/// \file		RenderComponent.cpp
/// \brief		Implementation file for Render Viewer to hook up to Elemental Engine
/// \date		05-12-2005
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
#include "RenderComponent.h"
#include "FileHelpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(CRenderViewerComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CRenderViewerComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CRenderViewerComponent);
REGISTER_MESSAGE_HANDLER(GlobalUpdateTick, OnUpdateTick, CRenderViewerComponent);
REGISTER_MESSAGE_HANDLER(StopPlaying, OnStopPlaying, CRenderViewerComponent);
REGISTER_MESSAGE_HANDLER(SetCameraMoveSensitivity, OnSetCameraMoveSensitivity, CRenderViewerComponent);
///priority message to get before anything else so that the world is destroyed
/// and the user can be prompted with a save current scene option
REGISTER_MESSAGE_HANDLER_PRIORITY(LoadFileByExtension, OnLoadNewFile, CRenderViewerComponent, 0x70001000 );


// constructor
CRenderViewerComponent::CRenderViewerComponent()
{
}

// we don't want elemental or anybody else creating us.
IComponent *CRenderViewerComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CRenderViewerComponent);
}

// serialize function for this object
void CRenderViewerComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CRenderViewerComponent::GetComponentType()
{
	static CHashString compName(_T("CRenderViewerComponent"));
	return &compName;
}

bool CRenderViewerComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// Set the parent of the component
void CRenderViewerComponent::SetParent(CRenderViewer *parent)
{
	m_Parent = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CRenderViewerComponent::OnCmdMsg(DWORD size, void *param)
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
DWORD CRenderViewerComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	// UNDONE: Move this ViewDocument call to a better location, here for now for testing
	// This should be created when an EE hierarchy is known to exist and destroyed when the EE 
	// hierarchy is destroyed
	m_Parent->ViewDocument();

	return MSG_HANDLED_PROCEED;
}

DWORD CRenderViewerComponent::OnUpdateTick( DWORD size, void * param )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GLOBALTICKPARAMS));
	GLOBALTICKPARAMS *globalTick;

	globalTick = (GLOBALTICKPARAMS *)param;
	m_Parent->OnUpdateTick(globalTick->tickCount);
	return MSG_HANDLED_PROCEED;
}

DWORD CRenderViewerComponent::OnStopPlaying( DWORD size, void * param )
{
	AfxGetMainWnd()->OnCmdMsg(ID_STOP, 0, NULL, NULL);
	return MSG_HANDLED_PROCEED;
}

DWORD CRenderViewerComponent::OnSetCameraMoveSensitivity( DWORD size, void * param )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float f = *(float*)param;
	m_Parent->SetCameraMoveSensitivity( f );
	return MSG_HANDLED_PROCEED;
}


DWORD CRenderViewerComponent::OnLoadNewFile( DWORD size, void * param )
{
	LOADFILEEXTPARAMS *lfep;
	VERIFY_MESSAGE_SIZE(size, sizeof(*lfep));
	lfep = static_cast<LOADFILEEXTPARAMS *>(param);
	//check extension
	LPCTSTR szExt = FileHelpers::GetExtension(lfep->fileName);
	if (szExt != NULL && !_tcsicmp(szExt, _T("wxl")))
	{
		// Renderer does not work correctly if RenderView is not active while world is loading
		CView *pView = m_Parent->GetFirstView();
		static DWORD msgHash_SetActiveView = CHashString(_T("SetActiveView")).GetUniqueID();
		if (MSG_HANDLED != EngineGetToolBox()->SendMessage(msgHash_SetActiveView, sizeof(pView), pView))
		{
			return MSG_ERROR;
		}
		return MSG_HANDLED_PROCEED;
	}
	return MSG_NOT_HANDLED;
}
