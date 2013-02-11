///==========================================================================
/// \file	ProxyObject.cpp
/// \brief	Implementation for proxy objects
/// \date	03/20/2007
/// \author	Nick Tourte
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
///==========================================================================

#include "stdafx.h"
#include "ProxyObject.h"

CProxyObject::CProxyObject():
		m_hsProxyManagerType(_T("CProxyManager")),
		m_ProxiedObject(NULL),
		m_ProxyManager(NULL)
{	
}

CProxyObject::~CProxyObject()
{
	DeInit();
	if (m_ProxiedObject)
	{
		if (m_ProxyManager)
		{
			m_ProxyManager->DeleteObject(m_ProxiedObject);		
		}
		// because of the name duplicity, we couldn't add the object to the object factory, so
		// we have to explicitly delete the object here
        try
		{
			delete m_ProxiedObject;
			m_ProxiedObject = NULL;
		}
		catch(...)
		{
			m_ToolBox->Log(LOGERROR, _T("Proxy object: could not delete the proxied object!\n"));
			assert(0);
		}
	}
}

bool CProxyObject::CreateProxiedObject(IHashString *parentName, IHashString *name, IHashString *className)
{
	m_ToolBox = EngineGetToolBox();	

	// some rough assumptions to make... hopefully this will always work
	// 3rd argument is add to hierarchy, which will be false
	IComponent *proxiedComp = m_ToolBox->CreateComponent(className, 3, name, parentName, false);

	if (proxiedComp == NULL)
	{
		m_ToolBox->Log(LOGERROR, _T("Proxy object: could not create component %s!\n"), className->GetString());
		return false;
	}

	m_ProxiedObject = dynamic_cast<IObject*>(proxiedComp);

	if (m_ProxiedObject == NULL)
	{
		m_ToolBox->Log(LOGERROR, _T("Proxy object: dynamic cast to IObject failed for object %s!\n"), className->GetString());
		return false;
	}

	IComponent *proxyManagerComp = m_ToolBox->CreateComponent(&m_hsProxyManagerType, 0);

	if (proxyManagerComp == NULL)
	{
		m_ToolBox->Log(LOGERROR, _T("Proxy object: could not create proxy manager component!\n"));
		return false;
	}

	m_ProxyManager = dynamic_cast<IManager*>(proxyManagerComp);

	if (m_ProxyManager == NULL)
	{
		m_ToolBox->Log(LOGERROR, _T("Proxy object: dynamic cast to IManager failed for proxy manager component!\n"));
		return false;
	}

	m_ProxyManager->AddObject(m_ProxiedObject, className);

	return true;
}

void CProxyObject::Serialize(IArchive &ar)
{
	if (m_ProxiedObject)
	{
		m_ProxiedObject->Serialize(ar);
	}
}

void CProxyObject::Init()
{
	if (m_ProxiedObject)
	{
		m_ProxiedObject->Init();
	}
}

void CProxyObject::DeInit()
{
	if (m_ProxiedObject)
	{
		m_ProxiedObject->DeInit();
	}
}

void CProxyObject::SetParentName(IHashString *inName)
{
	if (m_ProxiedObject)
	{
		m_ProxiedObject->SetParentName(inName);
	}
}

bool CProxyObject::Update()
{
	if (m_ProxiedObject)
	{
		return m_ProxiedObject->Update();
	}
	else return false;
}

IHashString* CProxyObject::GetComponentType()
{
	if (m_ProxiedObject)
	{
		return m_ProxiedObject->GetComponentType();
	}
	else
	{
		return NULL;
	}
}
	
bool CProxyObject::IsKindOf(IHashString *compType)
{
	if (m_ProxiedObject)
	{
		return m_ProxiedObject->IsKindOf(compType);
	}
	else return false;
}

IHashString* CProxyObject::GetName()
{
	if (m_ProxiedObject)
	{
		return m_ProxiedObject->GetName();
	}
	else
	{
		return NULL;
	}
}

void CProxyObject::SetName(IHashString *inName)
{
	if (m_ProxiedObject)
	{
		m_ProxiedObject->SetName(inName);
	}
}

IHashString* CProxyObject::GetParentName()
{
	if (m_ProxiedObject)
	{
		return m_ProxiedObject->GetParentName();
	}
	else
	{
		return NULL;
	}
}
