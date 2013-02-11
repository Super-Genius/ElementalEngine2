///============================================================================
/// \file		SplineComponent.cpp
/// \brief		Implementation file for Spline Component
/// \date		11-14-2006
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

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(CSplineComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(ItemDropped, OnItemDropped, CSplineComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CSplineComponent);
REGISTER_MESSAGE_HANDLER(LoadSplineEditorItem, OnLoadSplineEditorItem, CSplineComponent);

// get the singleton for this
IComponent *CSplineComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CSplineComponent);
}

// serialize function for this object
void CSplineComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CSplineComponent::GetComponentType()
{
	static CHashString compName(_T("CSplineComponent"));
	return &compName;
}

bool CSplineComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for Spline component (interface to Elemental Engine)
void CSplineComponent::SetParent(CSplineEditor *parent)
{
	m_Parent = parent;
}

DWORD CSplineComponent::OnLoadSplineEditorItem(DWORD size, void *param)
{
	LOADFILEEXTPARAMS *lfep;

	VERIFY_MESSAGE_SIZE(size, sizeof(LOADFILEEXTPARAMS));

	lfep = (LOADFILEEXTPARAMS*)param;

	// spline object
	if (!m_Parent->LoadSpline(lfep))
	{
		// failed return error
		return MSG_ERROR;
	}

	return MSG_HANDLED_STOP;
}


// message to handle OnCmdMsgs from main window
DWORD CSplineComponent::OnItemDropped(DWORD size, void *param)
{
	DROPOBJECTINFO *doi;
	
	VERIFY_MESSAGE_SIZE(size, sizeof(DROPOBJECTINFO));
	
	// cast out drop object info
	doi = (DROPOBJECTINFO *)param;

	// spline from file drop
	return m_Parent->LoadSplineFromFileDrop(doi);
}

// message to handle WindowsCreate function
DWORD CSplineComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	return MSG_HANDLED_PROCEED;
}