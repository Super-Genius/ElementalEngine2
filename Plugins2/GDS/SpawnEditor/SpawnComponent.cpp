///============================================================================
/// \file		SpawnComponent.cpp
/// \brief		Implementation file for Spawn Component
/// \date		08-15-2005
/// \author		Josh Lulewicz
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
REGISTER_COMPONENT_SINGLETON(CSpawnComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(ItemDropped, OnItemDropped, CSpawnComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CSpawnComponent);
REGISTER_MESSAGE_HANDLER(SpawnEditorItem, OnSpawnEditorItem, CSpawnComponent);
REGISTER_MESSAGE_HANDLER(SpawnItemAtPosition, OnSpawnItemAtPosition, CSpawnComponent);

// get the singleton for this
IComponent *CSpawnComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CSpawnComponent);
}

// serialize function for this object
void CSpawnComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CSpawnComponent::GetComponentType()
{
	static CHashString compName(_T("CSpawnComponent"));
	return &compName;
}

bool CSpawnComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for Spawn component (interface to Elemental Engine)
void CSpawnComponent::SetParent(CSpawnEditor *parent)
{
	m_Parent = parent;
}

DWORD CSpawnComponent::OnSpawnEditorItem(DWORD size, void *param)
{
	LOADFILEEXTPARAMS *lfep;

	VERIFY_MESSAGE_SIZE(size, sizeof(LOADFILEEXTPARAMS));

	lfep = (LOADFILEEXTPARAMS*)param;

	// spawn object
	if (!m_Parent->Spawn(lfep))
	{
		// failed return error
		return MSG_ERROR;
	}

	return MSG_HANDLED_STOP;
}


// message to handle OnCmdMsgs from main window
DWORD CSpawnComponent::OnItemDropped(DWORD size, void *param)
{
	DROPOBJECTINFO *doi;
	
	VERIFY_MESSAGE_SIZE(size, sizeof(DROPOBJECTINFO));
	
	// cast out drop object info
	doi = (DROPOBJECTINFO *)param;

	// spawn from file drop
	return m_Parent->SpawnFromFileDrop(doi);
}

// message to handle WindowsCreate function
DWORD CSpawnComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	return MSG_HANDLED_PROCEED;
}

DWORD CSpawnComponent::OnSpawnItemAtPosition(DWORD size, void *param)
{
	SPAWNATPOSITONPARAMS *sapp;

	VERIFY_MESSAGE_SIZE(size, sizeof(SPAWNATPOSITONPARAMS));

	// extract parameters
	sapp = (SPAWNATPOSITONPARAMS*)param;

	// spawn at position
	if (!m_Parent->SpawnAtPosition(sapp))
	{
		// failed return error
		return MSG_ERROR;
	}

	return MSG_HANDLED_STOP;
}