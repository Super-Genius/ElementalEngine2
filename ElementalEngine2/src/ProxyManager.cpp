///=====================================================================
/// \file	ProxyManager.cpp
/// \brief	Manager for proxy objects 
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
///=====================================================================

#include "stdafx.h"
#include "ProxyManager.h"

REGISTER_COMPONENT_SINGLETON(CProxyManager);

CProxyManager::CProxyManager():
CManager(_T("CProxyManager"), 0, 0)
{
	m_ToolBox->RegisterMessagePump(this, (static_cast< MSGPUMPFUNC > (&CProxyManager::PumpHandler)), PROXYPUMPPRIORITY, true);
	m_bPumpRegistered = true;
}

CProxyManager::~CProxyManager()
{
	if (m_bPumpRegistered)
	{
		m_ToolBox->UnregisterMessagePump(this, PROXYPUMPPRIORITY);
		m_bPumpRegistered = false;
	}
}

IComponent* CProxyManager::Create( int nArgs, va_list argptr )
{
	return SINGLETONINSTANCE(CProxyManager);
}

DWORD CProxyManager::PumpHandler(DWORD size, void *data, 
		IHashString *name, IHashString *compType, MSGFUNCTION msgFunction)
{
	DWORD retVal;

	// default to not handled
	retVal = MSG_NOT_HANDLED;
	if (name != NULL)
	{
		// find name in obj list
		MAPGROUPLISTS *pGroupList = GetGroupList();
		MAPGROUPLISTS::iterator mglIter;
		for (mglIter = pGroupList->begin(); mglIter != pGroupList->end(); ++mglIter)
		{
			IDTOOBJECTMAP *ntom = mglIter->second;
			IDTOOBJECTMAP::iterator objIter;
			objIter = ntom->find(name->GetUniqueID());
			if (objIter != ntom->end())
			{
				IObject * destObject;
				destObject = objIter->second;
				if ( (compType == NULL) || destObject->IsKindOf(compType))
				{
					//send the message to the child object
					retVal = (destObject->*msgFunction)(size, data);
					if (retVal != MSG_NOT_HANDLED)
					{
						break;
					}
				}
			}
		}
	}
	return retVal;
}