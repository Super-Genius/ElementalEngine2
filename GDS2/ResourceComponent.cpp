///============================================================================
/// \file		ResourceComponent.cpp
/// \brief		Implementation file for ResourceComponent.h
/// \date		05-20-2005
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
#include "ResourceComponent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(CResourceComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(AddResource, OnAddResourceMsg, CResourceComponent);
REGISTER_MESSAGE_HANDLER(RemoveResource, OnRemoveResourceMsg, CResourceComponent);
REGISTER_MESSAGE_HANDLER(ClearResources, OnClearResourcesMsg, CResourceComponent);
REGISTER_MESSAGE_HANDLER(RegisterResourceCollection, 
						 OnRegisterResourceCollectionMsg, 
						 CResourceComponent);
REGISTER_MESSAGE_HANDLER(GetSelectedResources, OnGetSelectedResourcesMsg, CResourceComponent);
REGISTER_MESSAGE_HANDLER(SetSelectedResources, OnSetSelectedResourcesMsg, CResourceComponent);
REGISTER_MESSAGE_HANDLER(GetResourceInfoByRelativePath, OnGetResourceInfoByRelativePath, CResourceComponent );
REGISTER_MESSAGE_HANDLER(PauseThumbnailGenerationThread, OnPauseThumbnailGenerationThread, CResourceComponent);
REGISTER_MESSAGE_HANDLER(GetResourceActiveDirectory, OnGetResourceActiveDirectory, CResourceComponent);

REGISTER_MESSAGE_HANDLER(SetResourceDirectory, OnSetResourceDirectory, CResourceComponent);

// get the singleton for this
IComponent *CResourceComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CResourceComponent);
}

// serialize function for this Resource
void CResourceComponent::Serialize(IArchive &ar)
{
}

// get component name for this Resource
IHashString *CResourceComponent::GetComponentType()
{
	static CHashString compName(_T("CResourceComponent"));
	return &compName;
}

bool CResourceComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for Example component (interface to Elemental Engine)
void CResourceComponent::SetParent(CResourceDoc *parent)
{
	m_Parent = parent;
}

// add a resource
DWORD CResourceComponent::OnAddResourceMsg(DWORD size, void *param)
{
	RESOURCEPAGEINFO *os;
	VERIFY_MESSAGE_SIZE(size, sizeof(RESOURCEPAGEINFO));
	os = (RESOURCEPAGEINFO *)param;

	// forward to parent.
	if (m_Parent && m_Parent->AddResource(os))
	{
		return MSG_HANDLED_PROCEED;
	}

	// it wasn't handled by parent, so return that
	return MSG_NOT_HANDLED;

}

// remove a resource
DWORD CResourceComponent::OnRemoveResourceMsg(DWORD size, void *param)
{
	RESOURCEPAGEINFO *os;
	VERIFY_MESSAGE_SIZE(size, sizeof(RESOURCEPAGEINFO));
	os = (RESOURCEPAGEINFO *)param;

	// forward to parent.
	if (m_Parent && m_Parent->RemoveResource(os))
	{
		return MSG_HANDLED_PROCEED;
	}

	// it wasn't handled by parent, so return that
	return MSG_NOT_HANDLED;
}

// clear all resources
DWORD CResourceComponent::OnClearResourcesMsg(DWORD size, void *param)
{
	RESOURCEPAGEINFO *os;
	VERIFY_MESSAGE_SIZE(size, sizeof(RESOURCEPAGEINFO));
	os = (RESOURCEPAGEINFO *)param;

	// forward to parent
	if (m_Parent && m_Parent->ClearResources(os))
	{
		return MSG_HANDLED_PROCEED;
	}

	// it wasn't handled by parent, so return that
	return MSG_NOT_HANDLED;
}

// register resource collection
DWORD CResourceComponent::OnRegisterResourceCollectionMsg(DWORD size, void *param)
{
	RESOURCECOLLECTION *oc;
	VERIFY_MESSAGE_SIZE(size, sizeof(RESOURCECOLLECTION));
	oc = (RESOURCECOLLECTION *)param;

	// forward to parent.
	if (m_Parent && m_Parent->RegisterResourceCollection(oc))
	{
		return MSG_HANDLED_PROCEED;
	}

	// it wasn't handled by parent, so return that
	return MSG_NOT_HANDLED;
}

DWORD CResourceComponent::OnGetSelectedResourcesMsg( DWORD size, void *param )
{
	list< RESOURCEPAGEINFO > * pInOutList;
	VERIFY_MESSAGE_SIZE(size, sizeof(list< RESOURCEPAGEINFO > * ));
	pInOutList = (list< RESOURCEPAGEINFO > *)param;
	if( pInOutList && m_Parent && m_Parent->GetSelectedResources( *pInOutList ) )
	{
		return MSG_HANDLED_PROCEED;
	}
	return MSG_NOT_HANDLED;
}

DWORD CResourceComponent::OnSetSelectedResourcesMsg( DWORD size, void *param )
{
	list<StdString> * pInOutList;
	VERIFY_MESSAGE_SIZE(size, sizeof(list<StdString>*));
	pInOutList = (list<StdString> *)param;
	if( pInOutList && m_Parent && m_Parent->SetSelectedResources( *pInOutList ) )
	{
		return MSG_HANDLED_PROCEED;
	}
	return MSG_NOT_HANDLED;
}

DWORD CResourceComponent::OnGetResourceInfoByRelativePath( DWORD size, void *param )
{
	GETRESOURCEINFOBYRELATIVEPATH * searchparam;
	VERIFY_MESSAGE_SIZE(size, sizeof( GETRESOURCEINFOBYRELATIVEPATH )); 
	searchparam = (GETRESOURCEINFOBYRELATIVEPATH*)param;
	//create full path and search
	if( m_Parent && m_Parent->GetResourceInfoByRelativePath( searchparam ) )
	{
		return MSG_HANDLED_PROCEED;
	}
	return MSG_NOT_HANDLED;
}

DWORD CResourceComponent::OnPauseThumbnailGenerationThread( DWORD size, void * param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( bool * ) );
	bool * value = (bool*)param;
	if( m_Parent )
	{
		m_Parent->PauseThumbnailGenerationThreads( *value );
	}
	return MSG_HANDLED_PROCEED;
}

DWORD CResourceComponent::OnGetResourceActiveDirectory( DWORD size, void * param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(IHashString*) );
	IHashString * writePath = (IHashString*)param;

	if ( m_Parent )
	{
		*writePath = m_Parent->GetActiveDirectory().c_str();
	}
	return MSG_HANDLED_PROCEED;
}

DWORD CResourceComponent::OnSetResourceDirectory(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(SETWORKINGDIRECTORIESPARAMS));

	if ( m_Parent != NULL )
	{
		// Call the resource document's reset function
		m_Parent->ResetResourceDirectory();
		// force to reload the new resource directory
//		m_Parent->InitResourcePages();
	}
	else
	{
		// there must be some error if the parent pointer is not set correctly
		return MSG_ERROR;
	}

	return MSG_HANDLED_PROCEED;
}
