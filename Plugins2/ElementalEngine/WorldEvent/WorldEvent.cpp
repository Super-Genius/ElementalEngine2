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

class CEntityComponentTypeVisitor : public IVisitor
{
public:
	CEntityComponentTypeVisitor()
	{
		m_EntityList.clear();
	}

	~CEntityComponentTypeVisitor() {}

	bool Visit( IComponent * component, bool bVisitEnter )
	{
		IObject *theObject;
		IHashString *name;
		IHashString *type;
		std::string str;

		theObject = dynamic_cast<IObject *>(component);
		assert(theObject);
		name = theObject->GetName();
		type = theObject->GetComponentType();
		static CHashString entityTypeName(_T("CEntity"));

		// check if object is a CEntity
		if( type->GetUniqueID() ==  entityTypeName.GetUniqueID())
		{
			ISENTITYOFTYPEPARAMS ieot;
			ieot.entityType = &m_EntityTypeFilter;
			ieot.bResult = false;
			static DWORD msgHash_IsEntityTypeOf = CHashString(_T("IsEntityTypeOf")).GetUniqueID();
			EngineGetToolBox()->SendMessage(msgHash_IsEntityTypeOf, sizeof(ieot), &ieot, name );
			if (ieot.bResult)
			{
				// if the types match push it onto the stack
				m_EntityList.push_back( theObject );
			}
		}

		return true;
	}

	bool IsHierarchicalVisitor()
	{
		return false;
	}

	void SetEntityTypeFilter( StdString& type )
	{
		m_EntityTypeFilter = type;
	}

	OBJECTLIST* GetEntityList()
	{
		return &m_EntityList;
	}

private:
	CHashString m_EntityTypeFilter;
	OBJECTLIST m_EntityList;
};

REGISTER_COMPONENT(CWorldEvent);
REGISTER_MESSAGE_HANDLER(TriggerWorldEvent, OnTriggerWorldEvent, CWorldEvent);
REGISTER_MESSAGE_HANDLER(GetWorldEventInfo, OnGetWorldEventInfo, CWorldEvent);
REGISTER_MESSAGE_HANDLER(SetWorldEventEvent, OnSetWorldEventEvent, CWorldEvent);

IComponent *CWorldEvent::Create(int nArgs, va_list argptr)
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
		self = new CWorldEvent(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CWorldEvent::CWorldEvent(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE( CWorldEventManager, CWorldEvent, IObject, parentName, name)
{
	AddToHierarchy();
	m_TriggerArchive = NULL;
}

CWorldEvent::~CWorldEvent()
{
	DeInit();
	RemoveFromHierarchy();
}

void CWorldEvent::Init()
{
	static CHashString hsWorldEventTriggerType(_T("CWorldEventTrigger"));
	static DWORD msgHash_AddWorldEvent = CHashString(_T("AddWorldEvent")).GetUniqueID();
	CHashString hsTriggerName(m_szTriggerName);
	m_ToolBox->SendMessage(msgHash_AddWorldEvent, sizeof(IObject), this, &hsTriggerName, &hsWorldEventTriggerType );

	if (m_szEventParam.GetString()[0] != '\0')
	{
		CREATEARCHIVE ca;
		CHashString memType(_T("Memory"));
		
		ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
		ca.streamData = NULL; // self expanding mem buffer
		ca.streamSize = 0;
		ca.streamType = &memType;

		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("World Event: could not create mem archive!\n"));
			return;
		}

		m_TriggerArchive = ca.archive;

		m_TriggerArchive->Write(m_szEventParam.GetString());
	}
}

void CWorldEvent::DeInit()
{
	static DWORD msgHash_RemoveWorldEvent = CHashString(_T("RemoveWorldEvent")).GetUniqueID();
	CHashString hsTriggerName(m_szTriggerName);
	m_ToolBox->SendMessage(msgHash_RemoveWorldEvent, sizeof(IObject), this, &hsTriggerName);

	if (m_TriggerArchive)
	{
		m_TriggerArchive->Close();
	}
}

void CWorldEvent::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
		StdString szTriggerName, szEntityName, szEntityType, szEventName, szEventParam;
		ar.Read( szTriggerName, _T("TriggerName") );
		ar.Read( szEntityName, _T("EntityName") );
		ar.Read( szEntityType, _T("EntityType") );
		ar.Read( szEventName, _T("EventName") );
		ar.Read( szEventParam, _T("EventParam") );
		m_szTriggerName = szTriggerName;
		m_szEntityName = szEntityName;
		m_szEntityType = szEntityType;
		m_szEventName = szEventName;
		m_szEventParam = szEventParam;
	}
	else
	{
		ar.Write( m_szTriggerName.GetString(), _T("TriggerName") );
		ar.Write( m_szEntityName.GetString(), _T("EntityName") );
		ar.Write( m_szEntityType.GetString(), _T("EntityType") );
		ar.Write( m_szEventName.GetString(), _T("EventName") );
		ar.Write( m_szEventParam.GetString(), _T("EventParam") );
	}
}

DWORD CWorldEvent::OnTriggerWorldEvent(DWORD size, void *in_params)
{
	if (IsTriggerEntityByType())
	{
		CEntityComponentTypeVisitor entityVisitor;
		StdString SSEntityType(m_szEntityType.GetString());
		entityVisitor.SetEntityTypeFilter( SSEntityType );

		VISITHIERARCHYDATA vhd;
		vhd.name = NULL;
		vhd.visitor = &entityVisitor;
		static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(VISITHIERARCHYDATA), &vhd);

		OBJECTLIST *pEntityList = entityVisitor.GetEntityList();
		OBJECTLIST::iterator itrEntity = pEntityList->begin();
		while (itrEntity != pEntityList->end())
		{
			IObject *pEntity = *itrEntity;

			TRIGGEREVENTPARAMS msg;
			msg.EventName = &m_szEventName;
			msg.EventParamsArchive = m_TriggerArchive; 

			// reset, in case triggered more than once
			if (m_TriggerArchive)
			{
				m_TriggerArchive->SeekTo(0);
			}

			static DWORD msgHash_TriggerEvent = CHashString(_T("TriggerEvent")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_TriggerEvent, sizeof(msg), &msg, pEntity->GetName() );

			itrEntity++;
		}
	}
	else if (IsTriggerEntityByName())
	{
		TRIGGEREVENTPARAMS msg;
		msg.EventName = &m_szEventName;
		msg.EventParamsArchive = m_TriggerArchive; 

		// reset, in case triggered more than once
		if (m_TriggerArchive)
		{
			m_TriggerArchive->SeekTo(0);
		}

		static DWORD msgHash_TriggerEvent = CHashString(_T("TriggerEvent")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_TriggerEvent, sizeof(msg), &msg, &m_szEntityName );
	}

	return MSG_HANDLED_PROCEED;
}

bool CWorldEvent::IsTriggerEntityByName() const
{
	return m_szEntityName.GetString()[0] != '\0';
}

bool CWorldEvent::IsTriggerEntityByType() const
{
	return m_szEntityType.GetString()[0] != '\0';
}

DWORD CWorldEvent::OnGetWorldEventInfo(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(WORLDEVENTINFOPARAMS));
	WORLDEVENTINFOPARAMS *pParams = (WORLDEVENTINFOPARAMS*)in_params;

	pParams->bUseEntityName = IsTriggerEntityByName();
	pParams->bUseEntityType = IsTriggerEntityByType();
	pParams->hszEventName = m_szEventName;
	pParams->hszEntityName = m_szEntityName;
	pParams->hszEntityType = m_szEntityType;
	pParams->hszEventParam = m_szEventParam;

	return MSG_HANDLED_STOP;
}

DWORD CWorldEvent::OnSetWorldEventEvent(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(DWORD));
	DWORD *pParams = (DWORD*)in_params;

	m_szEventName = m_ToolBox->GetHashString( *pParams );

	return MSG_HANDLED_STOP;
}
