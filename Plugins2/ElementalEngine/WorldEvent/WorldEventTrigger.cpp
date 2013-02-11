///============================================================================
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CWorldEventTrigger);
REGISTER_MESSAGE_HANDLER(AddWorldEvent, OnAddWorldEvent, CWorldEventTrigger);
REGISTER_MESSAGE_HANDLER(RemoveWorldEvent, OnRemoveWorldEvent, CWorldEventTrigger);
REGISTER_MESSAGE_HANDLER(QueryWorldEvents, OnQueryWorldEvents, CWorldEventTrigger);
REGISTER_MESSAGE_HANDLER(GetWorldEventInfo, OnGetWorldEventInfo, CWorldEventTrigger);

IComponent *CWorldEventTrigger::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = new CWorldEventTrigger(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CWorldEventTrigger::CWorldEventTrigger(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE( CWorldEventManager, CWorldEventTrigger, IObject, parentName, name)
{
	m_pWorldEventManager = (CWorldEventManager*)m_Manager;
	AddToHierarchy();
}

CWorldEventTrigger::~CWorldEventTrigger()
{
	DeInit();
	RemoveFromHierarchy();
}

void CWorldEventTrigger::Init()
{
	m_pWorldEventManager->AddWorldEventTrigger( this );
}

void CWorldEventTrigger::DeInit()
{
	m_pWorldEventManager->DeleteWorldEventTrigger( this );
}

void CWorldEventTrigger::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
		StdString szEntityName, szEntityType, szEventName;
		ar.Read( szEntityName, _T("EntityName") );
		ar.Read( szEntityType, _T("EntityType") );
		ar.Read( szEventName, _T("EventName") );
		m_szEntityName = szEntityName;
		m_szEntityType = szEntityType;
		m_szEventName = szEventName;
	}
	else
	{
		ar.Write( m_szEntityName.GetString(), _T("EntityName") );
		ar.Write( m_szEntityType.GetString(), _T("EntityType") );
		ar.Write( m_szEventName.GetString(), _T("EventName") );
	}
}

bool CWorldEventTrigger::IsTriggeredByEntityName() const
{
	return m_szEntityName.GetString()[0] != '\0';
}

bool CWorldEventTrigger::IsTriggeredByEntityType() const
{
	return m_szEntityType.GetString()[0] != '\0';
}

DWORD CWorldEventTrigger::OnAddWorldEvent(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IObject));
	IObject *pWorldEvent = (IObject*)in_params;

	m_WorldEvents.push_back( pWorldEvent );
	return MSG_HANDLED_PROCEED;
}

DWORD CWorldEventTrigger::OnRemoveWorldEvent(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IObject));
	IObject *pWorldEvent = (IObject*)in_params;

	m_WorldEvents.remove( pWorldEvent );
	return MSG_HANDLED_PROCEED;
}

bool CWorldEventTrigger::BroadcastWorldEvent(BROADCASTWORLDEVENTPARAMS *pParams)
{
	static CHashString hsWorldEventType(_T("CWorldEvent"));

	bool bTrigger = false;
	if (m_szEventName == *pParams->pEventName)
	{
		if (m_szEntityName == *pParams->pEntityName)
			bTrigger = true;
		else if (m_szEntityName == _T("") && m_szEntityType == *pParams->pEntityType)
			bTrigger = true;
	}
	if (!bTrigger)
		return false;

	WORLDEVENTPTRLIST::iterator itrWorldEvent = m_WorldEvents.begin();
	while (itrWorldEvent != m_WorldEvents.end())
	{
		IHashString *pWorldEventName = (*itrWorldEvent)->GetName();

		static DWORD msgHash_TriggerWorldEvent = CHashString(_T("TriggerWorldEvent")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TriggerWorldEvent, 0, NULL, pWorldEventName, &hsWorldEventType );

		itrWorldEvent++;
	}
	return true;
}

DWORD CWorldEventTrigger::OnQueryWorldEvents(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(QUERYWORLDEVENTSPARAMS));
	QUERYWORLDEVENTSPARAMS *pParams = (QUERYWORLDEVENTSPARAMS*)in_params;

	if (pParams->pWorldEventList == NULL)
		return MSG_ERROR;

	WORLDEVENTPTRLIST::iterator itr = m_WorldEvents.begin();
	while (itr != m_WorldEvents.end())
	{
		pParams->pWorldEventList->push_back( (*itr)->GetName()->GetUniqueID() );
		itr++;
	}

	return MSG_HANDLED_STOP;
}

DWORD CWorldEventTrigger::OnGetWorldEventInfo(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(WORLDEVENTINFOPARAMS));
	WORLDEVENTINFOPARAMS *pParams = (WORLDEVENTINFOPARAMS*)in_params;

	pParams->bUseEntityName = IsTriggeredByEntityName();
	pParams->bUseEntityType = IsTriggeredByEntityType();
	pParams->hszEventName = m_szEventName;
	pParams->hszEntityName = m_szEntityName;
	pParams->hszEntityType = m_szEntityType;

	return MSG_HANDLED_STOP;
}
