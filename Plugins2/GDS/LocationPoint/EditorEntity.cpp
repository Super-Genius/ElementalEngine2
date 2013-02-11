///============================================================================
/// \file		EditorEntity.cpp
/// \brief		Implementation of editor entity class
/// \date		03/20/2007
/// \author		Nick Tourte
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

#include "stdafx.h"
#include "editorentity.h"

REGISTER_COMPONENT(CEditorEntity);

//REGISTER_MESSAGE_HANDLER(SetDirtyTransformFlag, OnSetDirtyTransformFlag, CEditorEntity);

CEditorEntity::CEditorEntity(IHashString *parentName, IHashString *name, bool bAddToHier):
		OBJECTTEMPLATE( CLocationPointManager, CEditorEntity, CProxyObject, parentName, name ), 
		m_hsEntityType(_T("CEntity"))
{
	if (CreateProxiedObject(parentName, name, &m_hsEntityType) == false)
	{
		EngineGetToolBox()->Log(LOGERROR, _T("CEditorEntity: creating proxied object failed!\n"));
		return;
	}
	m_bInHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}
}

CEditorEntity::~CEditorEntity()
{
	DeInit();
	if (m_bInHierarchy)
	{
		RemoveFromHierarchy();
	}
}

IComponent* CEditorEntity::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new CEditorEntity(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CEditorEntity(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}
/*
DWORD CEditorEntity::OnSetDirtyTransformFlag(DWORD size, void *param)
{
	// forward this message to the entity proxy, should be hit in the proxy pump
	static DWORD msgHash_SetDirtyTransformFlag = CHashString(_T("SetDirtyTransformFlag")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetDirtyTransformFlag, 0, NULL, GetName(), &m_hsEntityType);

	return MSG_HANDLED_PROCEED;
}
*/