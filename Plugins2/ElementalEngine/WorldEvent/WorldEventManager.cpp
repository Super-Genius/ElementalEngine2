///==========================================================================
/// \file	CWorldEventManager.cpp
/// \brief	Implementation of Terrain
/// \date	03-24-2005
/// \author	Brian Bazyk
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

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WORLDEVENTMANAGERPRIORITY     0x0581

TOLUA_API int tolua_WorldEventScript_open (lua_State* tolua_S);

REGISTER_COMPONENT_SINGLETON( CWorldEventManager );
REGISTER_MESSAGE_HANDLER(Start, OnStart, CWorldEventManager);
REGISTER_MESSAGE_HANDLER(Stop, OnStop, CWorldEventManager);
REGISTER_MESSAGE_HANDLER(QueryWorldEventTriggers, OnQueryWorldEventTriggers, CWorldEventManager);
REGISTER_MESSAGE_HANDLER(GetWorldEvents, OnGetWorldEvents, CWorldEventManager);
REGISTER_MESSAGE_HANDLER(FindWorldEventTrigger, OnFindWorldEventTrigger, CWorldEventManager);
REGISTER_MESSAGE_HANDLER(BroadcastWorldEvent, OnBroadcastWorldEvent, CWorldEventManager);


IComponent *CWorldEventManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CWorldEventManager );
}

CWorldEventManager::CWorldEventManager() : 
	CManager(_T("CWorldEventManager"), 0, 0),
	m_hsWorldEventTriggerTypeName(_T("CWorldEventTrigger")), 
	m_hsWorldEventTypeName(_T("CWorldEvent"))
{
	m_bIsRunning = true;
	RegisterLuaDefinitions();
	Init();
}

CWorldEventManager::~CWorldEventManager()
{
	DeInit();
}

bool CWorldEventManager::Init()
{
	return CManager::Init();
}

bool CWorldEventManager::DeInit()
{
	return CManager::DeInit();
}

void CWorldEventManager::AddWorldEventTrigger( CWorldEventTrigger *pTrigger )
{
	if (pTrigger->IsTriggeredByEntityName())
	{
		m_EntityNameToTriggerListMap[pTrigger->GetEntityName().GetUniqueID()].push_back( pTrigger->GetName()->GetUniqueID() );
	}
	else if (pTrigger->IsTriggeredByEntityType())
	{
		m_EntityTypeToTriggerListMap[pTrigger->GetEntityType().GetUniqueID()].push_back( pTrigger->GetName()->GetUniqueID() );
	}
}

void CWorldEventManager::DeleteWorldEventTrigger( CWorldEventTrigger *pTrigger )
{
	if (pTrigger->IsTriggeredByEntityName())
	{
		m_EntityNameToTriggerListMap[pTrigger->GetEntityName().GetUniqueID()].remove( pTrigger->GetName()->GetUniqueID() );
	}
	else if (pTrigger->IsTriggeredByEntityType())
	{
		m_EntityTypeToTriggerListMap[pTrigger->GetEntityType().GetUniqueID()].remove( pTrigger->GetName()->GetUniqueID() );
	}
}

DWORD CWorldEventManager::OnStart(DWORD size, void *data)
{
	m_bIsRunning = true;
	return MSG_HANDLED_PROCEED;
}

DWORD CWorldEventManager::OnStop(DWORD size, void *data)
{
	m_bIsRunning = false;
	return MSG_HANDLED_PROCEED;
}

void CWorldEventManager::RegisterLuaDefinitions()
{
	static DWORD msgHash_GetMasterScriptState = CHashString(_T( "GetMasterScriptState" )).GetUniqueID();
	DWORD amessageHandle = m_ToolBox->SendMessage(msgHash_GetMasterScriptState, sizeof( lua_State * ), &m_LuaState );
	if( amessageHandle != MSG_HANDLED)
	{
		StdString error;
		error = _T("Error missing Master Script State Data\n");

		// log error
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
		EngineGetToolBox()->Log(LOGWARNING, error);
    }
	else
	{
		// register our additional structures/handlers with LUA master
		tolua_WorldEventScript_open(m_LuaState);
	}
}

DWORD CWorldEventManager::OnQueryWorldEventTriggers(DWORD size, void *in_params)
{
	static DWORD msgHash_QueryWorldEvents = CHashString(_T("QueryWorldEvents")).GetUniqueID();
	VERIFY_MESSAGE_SIZE(size, sizeof(QUERYWORLDEVENTTRIGGERSPARAMS));
	QUERYWORLDEVENTTRIGGERSPARAMS *pParams = (QUERYWORLDEVENTTRIGGERSPARAMS*)in_params;

	if (pParams->pWorldEventTriggerMap == NULL)
		return MSG_ERROR;

	IDTOOBJECTMAP *pTriggerMap = GetObjectMap(&m_hsWorldEventTriggerTypeName);
	if (!pTriggerMap)
	{
		return MSG_HANDLED_STOP; 
	}

	if (pParams->pEntityName != NULL)
	{
		WORLDEVENTTRIGGERMAP::iterator itrMap = m_EntityNameToTriggerListMap.find( pParams->pEntityName->GetUniqueID() );
		if (itrMap != m_EntityNameToTriggerListMap.end())
		{
			WORLDEVENTLIST::iterator itrList = itrMap->second.begin();
			while (itrList != itrMap->second.end())
			{
				// get the triggers name id
				DWORD triggerName = *itrList;
				IDTOOBJECTMAP::iterator itrObject = pTriggerMap->find(triggerName);
				if (itrObject != pTriggerMap->end())
				{
					// query the world events attached to this trigger
					QUERYWORLDEVENTSPARAMS qwep;
					qwep.pWorldEventList = &(*pParams->pWorldEventTriggerMap)[triggerName];
					m_ToolBox->SendMessage(msgHash_QueryWorldEvents, sizeof(qwep), &qwep, itrObject->second->GetName() );
				}
				itrList++;
			}
		}
	}
	else if (pParams->pEntityType != NULL)
	{
		WORLDEVENTTRIGGERMAP::iterator itrMap = m_EntityTypeToTriggerListMap.find( pParams->pEntityType->GetUniqueID() );
		if (itrMap != m_EntityTypeToTriggerListMap.end())
		{
			WORLDEVENTLIST::iterator itrList = itrMap->second.begin();
			while (itrList != itrMap->second.end())
			{
				// get the triggers name id
				DWORD triggerName = *itrList;
				IDTOOBJECTMAP::iterator itrObject = pTriggerMap->find(triggerName);
				if (itrObject != pTriggerMap->end())
				{
					// query the world events attached to this trigger
					QUERYWORLDEVENTSPARAMS qwep;
					qwep.pWorldEventList = &(*pParams->pWorldEventTriggerMap)[triggerName];
					m_ToolBox->SendMessage(msgHash_QueryWorldEvents, sizeof(qwep), &qwep, itrObject->second->GetName() );
				}
				itrList++;
			}
		}
	}
	else
	{
		IDTOOBJECTMAP::iterator itrObject = pTriggerMap->begin();
		while( itrObject != pTriggerMap->end() )
		{
			QUERYWORLDEVENTSPARAMS qwep;
			qwep.pWorldEventList = &(*pParams->pWorldEventTriggerMap)[itrObject->first];
			m_ToolBox->SendMessage(msgHash_QueryWorldEvents, sizeof(qwep), &qwep, itrObject->second->GetName() );
			itrObject++;
		}
	}

	return MSG_HANDLED_STOP;
}

DWORD CWorldEventManager::OnGetWorldEvents(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(QUERYWORLDEVENTSPARAMS));
	QUERYWORLDEVENTSPARAMS *pParams = (QUERYWORLDEVENTSPARAMS*)in_params;

	if( pParams->pWorldEventList == NULL )
		return MSG_ERROR;

	IDTOOBJECTMAP *pTriggerMap = GetObjectMap(&m_hsWorldEventTypeName);

	if( !pTriggerMap )
	{
		return MSG_HANDLED_STOP;
	}

	IDTOOBJECTMAP::iterator itrObject = pTriggerMap->begin();

	while( itrObject != pTriggerMap->end() )
	{
		pParams->pWorldEventList->push_back( itrObject->first );
		itrObject++;
	}
	return MSG_HANDLED_STOP;
}

DWORD CWorldEventManager::OnFindWorldEventTrigger(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(FINDWORLDEVENTPARAMS));
	FINDWORLDEVENTPARAMS *pParams = (FINDWORLDEVENTPARAMS*)in_params;

	pParams->bFound = false;

	IDTOOBJECTMAP *pTriggerMap = GetObjectMap(&m_hsWorldEventTriggerTypeName);
	if (!pTriggerMap)
	{
		return MSG_HANDLED_STOP; 
	}

	IDTOOBJECTMAP::iterator itrObject = pTriggerMap->begin();
	while( itrObject != pTriggerMap->end() )
	{
		CWorldEventTrigger *pTrigger = (CWorldEventTrigger*)itrObject->second;

		if (pTrigger->GetEventName().GetUniqueID() == pParams->pEventName->GetUniqueID())
		{
			if ((pTrigger->IsTriggeredByEntityName() && pParams->pEntityName) && 
				(pTrigger->GetEntityName().GetUniqueID() == pParams->pEntityName->GetUniqueID()))
			{
				if (pParams->pFoundTriggerName)
					*pParams->pFoundTriggerName = pTrigger->GetName()->GetString();
				pParams->bFound = true;
				return MSG_HANDLED_STOP;
			}
			if ((pTrigger->IsTriggeredByEntityType() && pParams->pEntityType) && 
				(pTrigger->GetEntityType().GetUniqueID() == pParams->pEntityType->GetUniqueID()))
			{
				if (pParams->pFoundTriggerName)
					*pParams->pFoundTriggerName = pTrigger->GetName()->GetString();
				pParams->bFound = true;
				return MSG_HANDLED_STOP;
			}
		}
		itrObject++;
	}

	return MSG_HANDLED_STOP;
}

DWORD CWorldEventManager::OnBroadcastWorldEvent(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(BROADCASTWORLDEVENTPARAMS));
	BROADCASTWORLDEVENTPARAMS *pParams = (BROADCASTWORLDEVENTPARAMS*)in_params;

	IDTOOBJECTMAP *pTriggerMap = GetObjectMap(&m_hsWorldEventTriggerTypeName);
	if (pTriggerMap)
	{
		for (IDTOOBJECTMAP::iterator itrTrigger = pTriggerMap->begin(); itrTrigger != pTriggerMap->end(); itrTrigger++)
		{
			CWorldEventTrigger *pTrigger = (CWorldEventTrigger*)itrTrigger->second;
			pTrigger->BroadcastWorldEvent( pParams );
		}
	}
	return MSG_HANDLED_STOP; 
}
