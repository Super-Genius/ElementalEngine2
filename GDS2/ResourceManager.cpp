//---------------------------------------------------------------------
// File:		ResourceManager.cpp
// Description:	Manages Resources
// Created:		11/30/03
// Author:		Kenneth L. Hurley
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
//---------------------------------------------------------------------

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if 0

static UINT AddHierarchyResource = RegisterWindowMessage(ADD_HIER_RESOURCE_MSG);
static UINT RemoveHierarchyResource = RegisterWindowMessage(REMOVE_HIER_RESOURCE_MSG);
static UINT ClearHierarchyResources = RegisterWindowMessage(CLEAR_HIER_ALL_RESOURCE_MSG);

// {6D547917-153C-4255-96A7-7F64383D70CB}
static const GUID GUID_WORLD = 
{ 0x6d547917, 0x153c, 0x4255, { 0x96, 0xa7, 0x7f, 0x64, 0x38, 0x3d, 0x70, 0xcb } };

//---------------------------------------------------------------------
// Function:	CResourceManger
// Description:	Construction code, should be singleton
// Parameters:	ProgramInfo = information relating to the program
// Returns:		.
//---------------------------------------------------------------------
CResourceManager::CResourceManager()
{
#if 0
	m_HeadResource = new CBaseObject();
	m_HeadResource->Initialize(&GUID_WORLD, "World");
	if (m_HeadResource != NULL)
	{
		m_VecHead.push_back(m_HeadResource);
		m_HeadResource->SetRelationships(NULL, m_VecHead.begin());
	}
#endif
}

//---------------------------------------------------------------------
// Function:	~CResourceManger
// Description:	Destructior
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
CResourceManager::~CResourceManager()
{
#if 0
	delete m_HeadResource;
#endif
}

//---------------------------------------------------------------------
// Function:	GetResources
// Description:	Get the Resources stored in STL vector
// Parameters:	.
// Returns:		iterator of type VECHIEROBJ, Vector of Hierarchy Resources
//---------------------------------------------------------------------
VECHIEROBJ::iterator CResourceManager::GetResources()
{
#if 0
	return m_HeadResource->GetChildren();
#endif
}

//---------------------------------------------------------------------
// Function:	GetHeadResource
// Description:	Get the head Resource in the vector list
// Parameters:	.
// Returns:		IBaseResource, pointer to head Resource
//---------------------------------------------------------------------
IBaseObject *CResourceManager::GetHeadResource()
{
#if 0
	return m_HeadResource;
#endif
}

//---------------------------------------------------------------------
// Function:	AddResource
// Description:	Adds an Resource to the internal Resource manager hierarchy
//				also broadcasts a message.
// Parameters:	Resource = pointer to create base Resource
//				parent = NULL, for none, or real parent Resource in hier.
//				before = to insert before a specific indexed Resource
// Returns:		TRUE, if succesful
//---------------------------------------------------------------------
BOOL CResourceManager::AddResource(CBaseObject *Resource, CBaseObject *parent,
			DWORD before)
{
	// if parent is NULL, use the head Resource
	if (parent == NULL)
	{
		parent = m_HeadResource;
	}

	// add Resource with error check
	if (!parent->AddChild(Resource, before))
	{
		// failed
		return FALSE;
	}

	HIEROBJMESSINFO homi;

	homi.object = Resource;
	homi.enable = TRUE;
	theApp.BroadcastMessage(AddHierarchyResource, &homi);
	return TRUE;

}

//---------------------------------------------------------------------
// Function:	RemoveResource
// Description:	Removes an Resource from the hierarchy
// Parameters:	srcObj = pointer to base Resource to remove
// Returns:		.
//---------------------------------------------------------------------
void CResourceManager::RemoveResource(CBaseObject *srcObj)
{
	HIEROBJMESSINFO homi;
	CBaseObject *parent;

	homi.object = srcObj;
	homi.enable = FALSE;
	// allow others to still have access to the Resource, before we remove
	// it from the Resource manager
	theApp.BroadcastMessage(RemoveHierarchyResource, &homi);

	// get parent of Resource, so we can remove child
	parent = srcObj->GetParent();
	// and remove child
	parent->RemoveChild(srcObj);
}

//---------------------------------------------------------------------
// Function:	RemoveAllResources
// Description:	Removes all Resources from the hierarchy
// Parameters:	.
// Returns:		.
//---------------------------------------------------------------------
void CResourceManager::RemoveAllResources()
{
	// allow others to still have access to the Resources, before we remove
	// them from the Resource manager
	static DWORD msgHash_ClearHierarchyResources = CHashString(_T("ClearHierarchyResources")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_ClearHierarchyResources, 0, NULL, NULL, NULL);

	m_HeadResource->RemoveAllChildren();
}
#endif