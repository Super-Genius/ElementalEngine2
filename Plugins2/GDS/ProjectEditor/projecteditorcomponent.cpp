///============================================================================
/// \file		ProjectEditorComponent.cpp
/// \brief		Header file for ProjectEditor component
/// \date		12-18-2009
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

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(CProjectEditorComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CProjectEditorComponent);
// register higher priority, because we add main menu items.
REGISTER_MESSAGE_HANDLER_PRIORITY(WindowsMainCreate, OnCreate, CProjectEditorComponent, 0x70000000);
REGISTER_MESSAGE_HANDLER(FileLoaded_gdsprj, OnProjectFileLoaded, CProjectEditorComponent);
REGISTER_MESSAGE_HANDLER(GdsClose, OnGdsClose, CProjectEditorComponent);
REGISTER_MESSAGE_HANDLER(AddProjectSettingsPropertyPage, OnAddProjectSettingsPropertyPageMessage, CProjectEditorComponent);

// get the singleton for this
IComponent *CProjectEditorComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CProjectEditorComponent);
}

// serialize function for this object
void CProjectEditorComponent::Serialize(IArchive &ar)
{
	m_Parent->Serialize(ar);
}

// get component name for this object
IHashString *CProjectEditorComponent::GetComponentType()
{
	static CHashString compName(_T("CProjectEditorComponent"));
	return &compName;
}

bool CProjectEditorComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for ProjectEditor component (interface to Elemental Engine)
void CProjectEditorComponent::SetParent(CProjectEditor *parent)
{
	m_Parent = parent;
}

// message to handle OnCmdMsgs from main window
DWORD CProjectEditorComponent::OnCmdMsg(DWORD size, void *param)
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
DWORD CProjectEditorComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	// this message is called only after GDS window is loaded
	m_Parent->AutoLoadProjectSettings();

	return MSG_HANDLED_PROCEED;
}

// come here when a project file is loaded
DWORD CProjectEditorComponent::OnProjectFileLoaded(DWORD size, void *param)
{
	LPTSTR name = (LPTSTR)param;
	VERIFY_MESSAGE_SIZE(size, sizeof(LPTSTR));

	// save the filename to have it later
	CProjectSettings *projectSettings = SINGLETONINSTANCE( CProjectSettings );
	projectSettings->setSaveFileName( name );

	return MSG_HANDLED_PROCEED;
}

// come here on GDS close event
DWORD CProjectEditorComponent::OnGdsClose(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( bool* ) );
	bool* returnParam = (bool*) param;

	// if project file is modified since last save, ask for confirmation	
	CProjectSettings *projectSettings = SINGLETONINSTANCE( CProjectSettings );
	
	// check for project settings consistency (whether anything is set, or not)
	if ( !projectSettings->IsConsistentState() )
	{
		// the project settings are not correctly set
		// return false without confirmation
		*returnParam = TRUE;
	}
	else
	{
		if ( projectSettings->CloseProjectSettings() )
		{
			// gets here if user confirmed quiting
			*returnParam = TRUE;
		}
		else
		{
			// gets here if user clicked cancel
			*returnParam = FALSE;
		}
	}
	return MSG_HANDLED;
}

DWORD CProjectEditorComponent::OnAddProjectSettingsPropertyPageMessage(DWORD size, void *param)
{
	IProjectSettingsPropertyPage *tProp;

	tProp = (IProjectSettingsPropertyPage *)param;
	VERIFY(tProp->IsKindOf(RUNTIME_CLASS(IProjectSettingsPropertyPage)));

	return 	m_Parent->AddProjectSettingsPropertyPage( tProp );
}
