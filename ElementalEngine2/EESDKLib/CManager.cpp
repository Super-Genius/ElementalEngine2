///=====================================================================
/// \file	CManager.cpp
/// \brief	concreate class for Manager
/// \date	02/16/2005
/// \author	Kenneth Hurley
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

#include "StdAfx.h"

CManager::CManager(const TCHAR *compType, DWORD listenPriority, DWORD pumpPriority) :
	m_ComponentType(compType)
{
	static CHashString CLFHS(_T("CListenerFactory"));

	// Create a link to the system listener
	m_Listener = new CManagerListener(this);
	m_ToolBox = EngineGetToolBox();
	m_Timer = m_ToolBox->GetTimer();

	m_bPumpEnabled = false;
	m_PumpPriority = 0;

	if (listenPriority != 0)
	{
		IListenerFactory *iListenFactory;
		iListenFactory = dynamic_cast<IListenerFactory *>(
			m_ToolBox->CreateComponent(&CLFHS, 0));
		// one had better be installed
		if (iListenFactory != NULL)
		{
			iListenFactory->Register(dynamic_cast<IListener *>(m_Listener),
				listenPriority);
		}
		else
		{
			m_ToolBox->SetErrorValue(ERR_NAME_NOT_FOUND);
			m_ToolBox->Log(LOGERROR, _T("The Listener Factory isn't present in the system\n"));
		}
	}

	if (pumpPriority != 0)
	{
		EnablePumpHandler(pumpPriority);
	}
}

bool CManager::Init()
{
	return true;
}

bool CManager::DeInit()
{
	DELETEOBJECTPARAMS dop;

	// Call DeleteObject on every object we manage from every group
	MAPGROUPLISTS::iterator mglIter;
	for (mglIter = m_GroupLists.begin(); mglIter != m_GroupLists.end(); ++mglIter)
	{
		IDTOOBJECTMAP *ntom;

		// loop through all the objects in this group
		ntom = mglIter->second;
		IDTOOBJECTMAP::iterator ntomIter = ntom->begin();
		while (ntomIter != ntom->end())
		{
			IObject *delObject;
			delObject = ntomIter->second;
			dop.name = delObject->GetName();
			static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof( DELETEOBJECTPARAMS ), &dop, NULL, NULL );
			ntomIter = ntom->begin();
		}

		// no more objects in the list.
		ntom->clear();
	}

	return true;
}

CManager::~CManager()
{
	IDTOOBJECTMAP *groupList;
	MAPGROUPLISTS::iterator mglIter;

	for (mglIter = m_GroupLists.begin(); mglIter != m_GroupLists.end(); mglIter++)
	{
		groupList = mglIter->second;
		delete groupList;
	}

	if (m_PumpPriority)
	{
		m_ToolBox->UnregisterMessagePump(this, m_PumpPriority);
		m_PumpPriority = 0;
	}

	delete m_Listener;
}

//
//	adds an Object to the Manager
//
bool CManager::AddObject( IObject *Object, IHashString *group)
{
	IDTOOBJECTMAP *objMap;
	IHashString *objName;
	MAPGROUPLISTS::iterator mglIter;

	objName = Object->GetName();

	// see if a list exists yet
	mglIter = m_GroupLists.find(group->GetUniqueID());
	if (mglIter == m_GroupLists.end())
	{
		// nope so allocate one
		objMap = new IDTOOBJECTMAP;
		m_GroupLists[group->GetUniqueID()] = objMap;
		m_ToolBox->AddMessagePumpFilter( GetComponentType()->GetUniqueID(), group->GetUniqueID() );
	}
	else
	{
		objMap = mglIter->second;
	}

	objMap->insert(pair<DWORD, IObject *>(objName->GetUniqueID(), Object));

	#if defined(_DEBUG) || defined(_PROFILE)
	UpdateProfiler();
	#endif

	return true;
}

//
// Deletes an Object and removes it from the manager
//
bool CManager::DeleteObject( IObject* Object )
{
	IDTOOBJECTMAP::iterator olIter;
	IHashString *objName;

	objName = Object->GetName();

	// Find object in m_GroupList and remove it from that
	MAPGROUPLISTS::iterator mglIter;
	IHashString *group = Object->GetComponentType();
	mglIter = m_GroupLists.find(group->GetUniqueID());

	if( mglIter != m_GroupLists.end() )
	{
		IDTOOBJECTMAP *os;
		os = mglIter->second;
		olIter = os->find(objName->GetUniqueID());
		if( olIter != os->end() )
		{
			//found object...remove from this list
			os->erase(olIter);

			#if defined(_DEBUG) || defined(_PROFILE)
			UpdateProfiler();
			#endif

			return true;
		}
	}

    // not found..error
	return false;
}

IDTOOBJECTMAP *CManager::GetObjectMap(IHashString *name)
{
	MAPGROUPLISTS::iterator mglIter;
	IDTOOBJECTMAP *objMap;

	mglIter = m_GroupLists.find(name->GetUniqueID());

	if (mglIter == m_GroupLists.end())
	{
		return NULL;
	}
	else
	{
		objMap = mglIter->second;
	}

	return objMap;
}

IObject* CManager::GetObjectByName(IHashString *name, IHashString *component)
{
	MAPGROUPLISTS::iterator mglIter;
	IDTOOBJECTMAP *objMap;
	IDTOOBJECTMAP::iterator objIter;

	if( component != NULL )
	{
        mglIter = m_GroupLists.find( component->GetUniqueID() );
		if( mglIter != m_GroupLists.end() )
		{
			objMap = mglIter->second;
			objIter = objMap->find( name->GetUniqueID() );
			if( objIter != objMap->end() )
				return objIter->second;
			else
				return NULL;
		}
	}

	mglIter = m_GroupLists.begin();
	while( mglIter != m_GroupLists.end() )
	{
		objMap = mglIter->second;
		objIter = objMap->find( name->GetUniqueID() );
		if( objIter != objMap->end() )
			return objIter->second;
		mglIter++;
	}
	return NULL;
}

// should managers serialize their data?
void CManager::Serialize(IArchive &ar)
{
}

bool CManager::IsKindOf(IHashString *compType)
{
	return ( compType->GetUniqueID() == GetComponentType()->GetUniqueID() );	
}

//
DWORD CManager::PumpHandler(DWORD size, void *data, 
		IHashString *name, IHashString *compType, MSGFUNCTION msgFunction)
{
	DWORD retVal;

	// default to not handled
	retVal = MSG_NOT_HANDLED;
	if ((m_bPumpEnabled) && (name != NULL))
	{
		if (compType == NULL)
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
					//send the message to the child object
					retVal = (destObject->*msgFunction)(size, data);
					if (retVal != MSG_NOT_HANDLED)
					{
						break;
					}
				}
			}
		}
		else
		{
			// find name in obj list
			MAPGROUPLISTS *pGroupList = GetGroupList();
			MAPGROUPLISTS::iterator mglIter = pGroupList->find( compType->GetUniqueID() );
			if (mglIter != pGroupList->end())
			{
				IDTOOBJECTMAP *ntom = mglIter->second;
				IDTOOBJECTMAP::iterator objIter;
				objIter = ntom->find(name->GetUniqueID());
				if (objIter != ntom->end())
				{
					IObject * destObject;
					destObject = objIter->second;
					//send the message to the child object
					retVal = (destObject->*msgFunction)(size, data);
				}
			}
		}
	}
	return retVal;
}

void CManager::EnablePumpHandler(DWORD priority)
{
	m_bPumpEnabled = true;

	// only registered if we haven't already done so.
	if ((m_PumpPriority == 0) && (priority != 0))
	{
		m_ToolBox->RegisterMessagePump(this, (static_cast< MSGPUMPFUNC > (&CManager::PumpHandler)), priority, false );
		m_PumpPriority = priority;
	}
}

void CManager::DisablePumpHandler()
{
	m_bPumpEnabled = false;
}

#if defined(_DEBUG) || defined(_PROFILE)
void CManager::UpdateProfiler( void )
{
	MAPGROUPLISTS *pGroupLists = GetGroupList();
	if (pGroupLists)
	{
		unsigned int iObjectCount = 0;
		MAPGROUPLISTS::iterator itrMaps = pGroupLists->begin();
		while (itrMaps != pGroupLists->end())
		{
			IDTOOBJECTMAP *pObjectMap = itrMaps->second;
			iObjectCount += (unsigned int)pObjectMap->size();
//			IDTOOBJECTMAP::iterator itrObjects = pObjectMap->begin();
//			while (itrObjects != pObjectMap->end())
//			{
//			}
			itrMaps++;
		}

		StdString szName = GetComponentType()->GetString();
		CHashString hszName(szName);
		CHashString hszType(_T("Object Counts"));
		PERFORMANCEPROFILERUPDATE msg;
		msg.name = &hszName;
		msg.time = (float)iObjectCount;
		msg.displayType = NAME_NUMBER;
		msg.type = &hszType;
		static DWORD msgHash_UpdatePerformanceProfiler = CHashString(_T("UpdatePerformanceProfiler")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_UpdatePerformanceProfiler, sizeof(msg), &msg );
	}
}
#endif
