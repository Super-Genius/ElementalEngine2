///=====================================================================
/// \file	CObjectFactory.cpp
/// \brief	Implementation of CObjectFactory Class
/// \date	05/01/2005
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
///=====================================================================

#include "StdAfx.h"
#include "CObjectFactory.h"
#include "CDelayedDeleteListener.h"

REGISTER_COMPONENT_SINGLETON(CObjectFactory);
REGISTER_MESSAGE_HANDLER(AddObjectToFactory, OnAddObject, CObjectFactory);
REGISTER_MESSAGE_HANDLER(CreateObject, OnCreateObject, CObjectFactory);
REGISTER_MESSAGE_HANDLER(DeleteObject, OnDeleteObject, CObjectFactory);
REGISTER_MESSAGE_HANDLER(DelayedDeleteObject, OnDelayedDeleteObject, CObjectFactory); ///////////////////////
REGISTER_MESSAGE_HANDLER(InitObject, OnInitObject, CObjectFactory);
REGISTER_MESSAGE_HANDLER(SerializeObject, OnSerializeObject, CObjectFactory);
REGISTER_MESSAGE_HANDLER(FindObject, OnFindObject, CObjectFactory);
REGISTER_MESSAGE_HANDLER(RenameObject, OnRenameObject, CObjectFactory);
REGISTER_MESSAGE_HANDLER(GetComponentType, OnGetComponentType, CObjectFactory);
REGISTER_MESSAGE_HANDLER(GenerateUniqueObjectName, OnGenerateUniqueObjectName, CObjectFactory);
REGISTER_MESSAGE_HANDLER(FlagDuplicateObjectChecking, OnFlagDuplicateObjectChecking, CObjectFactory);


IComponent *CObjectFactory::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CObjectFactory);
}

CObjectFactory::CObjectFactory() : m_ComponentType(CHashString(_T("CObjectFactory")))
{
	m_ToolBox = EngineGetToolBox();

	static CHashString CLFHS(_T("CListenerFactory"));
	// we have no objects currently
	// Create a link to the system listener
	m_DelayedDeleteListener = new CDelayedDeleteListener(this);
	m_ToolBox = EngineGetToolBox();
	m_bDuplicateObjectCheck = false;

	if (DELAYEDMESSAGEPRIORITY != 0)
	{
		IListenerFactory *iListenFactory;
		iListenFactory = dynamic_cast<IListenerFactory *>(
			m_ToolBox->CreateComponent(&CLFHS, 0));
		// one had better be installed
		assert(iListenFactory);
		if (iListenFactory != NULL)
		{
			iListenFactory->Register(dynamic_cast<IListener *>(m_DelayedDeleteListener),
				DELAYEDMESSAGEPRIORITY);
		}
	}
}

CObjectFactory::~CObjectFactory()
{
	// delete all remaining delayed objects
	DeleteDelayedObjects();
	// delete all remaining created objects
	ObjectKeyMap_t::iterator mapItr = m_ObjectKeyMap.begin();
	while (mapItr != m_ObjectKeyMap.end())
	{
		// creat a hashstring from the uniqueid
		CHashString name(m_ToolBox->GetHashString(mapItr->first));
		// Send the delete command
		DELETEOBJECTPARAMS dop;
		dop.name = &name;
		OnDeleteObject(sizeof(DELETEOBJECTPARAMS), &dop);

		// next object
		mapItr = m_ObjectKeyMap.begin();
	}

	delete m_DelayedDeleteListener;

	m_ToolBox->UnRegisterComponent( &m_ComponentType );
}

void CObjectFactory::Serialize(IArchive &ar)
{
}

IHashString *CObjectFactory::GetComponentType()
{
	return &m_ComponentType;
}

bool CObjectFactory::IsKindOf(IHashString *compType)
{
  return (compType->GetUniqueID() == m_ComponentType.GetUniqueID());
}

DWORD CObjectFactory::OnAddObject(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(OFACTORYADDPARAMS), size);
	OFACTORYADDPARAMS *ofap = (OFACTORYADDPARAMS*)params;
	CHashString name(ofap->name);

	if (m_bDuplicateObjectCheck)
	{
		bool bObjectFound = m_ObjectKeyMap.find( ofap->name->GetUniqueID() ) != m_ObjectKeyMap.end();
		if (bObjectFound)
		{
			m_ToolBox->Log( LOGFATALERROR, _T("Duplicate object found: %s\n"), ofap->name->GetString() );
			return MSG_ERROR;
		}
	}

	// add object to map!
	m_ObjectKeyMap.insert(pair<DWORD,IComponent*>( name.GetUniqueID(), ofap->component ));

	return MSG_HANDLED_PROCEED;
}

DWORD CObjectFactory::OnCreateObject(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(CREATEOBJECTPARAMS), size);
	CREATEOBJECTPARAMS *pCOP = (CREATEOBJECTPARAMS*)params;

	if (pCOP->name == NULL)
	{
		m_ToolBox->Log(LOGERROR, _T("Bad name pointer passed in OnCreateObject\n"));
		return MSG_ERROR;
	}

	IComponent *pComponent = dynamic_cast<IComponent*>(
		m_ToolBox->CreateComponent(pCOP->typeName, 2, pCOP->name, pCOP->parentName));
	if (pComponent == NULL)
	{
		// UNDONE: error message "Could not find component type %s", pCOP->typeName
		return MSG_ERROR;
	}
	if (m_bDuplicateObjectCheck)
	{
		bool bObjectFound = m_ObjectKeyMap.find( pCOP->name->GetUniqueID() ) != m_ObjectKeyMap.end();
		if (bObjectFound)
		{
			m_ToolBox->Log( LOGFATALERROR, _T("Duplicate object found: %s\n"), pCOP->name->GetString() );
			return MSG_ERROR;
		}
	}
	m_ObjectKeyMap.insert(pair<DWORD,IComponent*>( pCOP->name->GetUniqueID(), pComponent ));

	return MSG_HANDLED_PROCEED;
}

DWORD CObjectFactory::OnDeleteObject(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(DELETEOBJECTPARAMS), size);
	DELETEOBJECTPARAMS *pDOP = (DELETEOBJECTPARAMS*)params;

	ObjectKeyMap_t::iterator mapItr;
	if (pDOP->name == NULL)
	{
		m_ToolBox->Log(LOGERROR, _T("Bad name pointer passed in OnDeleteObject\n"));
		return MSG_ERROR;
	}
	mapItr = m_ObjectKeyMap.find( pDOP->name->GetUniqueID() );
	if (mapItr != m_ObjectKeyMap.end())
	{
		IComponent *pComponent = mapItr->second;

		try
		{
			// if component is an object, call DeInit
			IObject *pObject = dynamic_cast<IObject*>(pComponent);
			if (pObject)
				pObject->DeInit();

			m_ObjectKeyMap.erase( mapItr );
			delete pComponent;
		}
		catch(...)
		{
			assert(0);
		}
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CObjectFactory::OnDelayedDeleteObject(DWORD size, void *params)
{
	ObjectKeyMap_t::iterator mapItr;
	UniqueIDSet_t::iterator idIter;

	VERIFY_MESSAGE_SIZE(sizeof(DELETEOBJECTPARAMS), size);
	DELETEOBJECTPARAMS *pDOP = (DELETEOBJECTPARAMS*)params;
	
	if (pDOP->name == NULL)
	{
		m_ToolBox->Log(LOGERROR, _T("Bad name pointer passed in OnDelayedDeleteObject\n"));
		return MSG_ERROR;
	}

	DWORD nameID = pDOP->name->GetUniqueID();

	if (nameID == 0)
	{
		m_ToolBox->Log(LOGERROR, _T("Empty string or uninitialized hash string passed in OnDelayedDeleteObject\n"));
		return MSG_ERROR;
	}
	
	// check if object is alreay in list..do not add again if found
	idIter = m_DelayedDeleteList.find( nameID );

	if (idIter == m_DelayedDeleteList.end())
	{
		// Add the object to the delayed delete list
		m_DelayedDeleteList.insert( nameID );
	}

	return MSG_HANDLED_PROCEED;	
}

DWORD CObjectFactory::OnInitObject(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(INITOBJECTPARAMS), size);
	INITOBJECTPARAMS *pIOP = (INITOBJECTPARAMS*)params;

	ObjectKeyMap_t::iterator mapItr;
	mapItr = m_ObjectKeyMap.find( pIOP->name->GetUniqueID() );
	if (mapItr != m_ObjectKeyMap.end())
	{
		// if component is an object, call Init
		IComponent *pComponent = mapItr->second;
		IObject *pObject = dynamic_cast<IObject*>(pComponent);
		if (pObject)
			pObject->Init();
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CObjectFactory::OnSerializeObject(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(SERIALIZEOBJECTPARAMS), size);
	SERIALIZEOBJECTPARAMS *pSOP = (SERIALIZEOBJECTPARAMS*)params;

	ObjectKeyMap_t::iterator mapItr;
	if (pSOP->name == NULL)
	{
		m_ToolBox->Log(LOGERROR, _T("Bad name pointer passed in OnSerializeObject\n"));
		return MSG_ERROR;
	}

	if (pSOP->archive == NULL)
	{
		m_ToolBox->Log(LOGERROR, _T("Bad archive pointer passed in OnSerializeObject\n"));
		return MSG_ERROR;
	}

	mapItr = m_ObjectKeyMap.find( pSOP->name->GetUniqueID() );
	if (mapItr != m_ObjectKeyMap.end())
	{
		IComponent *pComponent = mapItr->second;
		pComponent->Serialize( *pSOP->archive );
	}

	return MSG_HANDLED_PROCEED;
}

// find and object with a name that may have been created.
DWORD CObjectFactory::OnFindObject(DWORD size, void *params)
{
	//IHashString *name;
	VERIFY_MESSAGE_SIZE(sizeof(FINDOBJECTPARAMS), size);

	FINDOBJECTPARAMS * p = (FINDOBJECTPARAMS*)params;
	
	if( p->hszName == NULL )
	{
		p->bFound = false;
		return MSG_ERROR;
	}

	ObjectKeyMap_t::iterator mapItr;
	mapItr = m_ObjectKeyMap.find( p->hszName->GetUniqueID() );
	if (mapItr != m_ObjectKeyMap.end())
	{
		// was found OK.
		p->bFound = true;
		return MSG_HANDLED_STOP;
	}

	// no object was found
	p->bFound = false;
	return MSG_HANDLED_STOP;
}

DWORD CObjectFactory::OnRenameObject(DWORD size, void *params)
{
	CHANGEOBJNAME *con;
	VERIFY_MESSAGE_SIZE(sizeof(CHANGEOBJNAME), size);

	ObjectKeyMap_t::iterator mapIter;
	IObject* object;

	con = (CHANGEOBJNAME*)params;
	IHashString *newName = con->newName;
	IHashString *oldName = con->oldName;

	// check to see if new name is not in objectmap
	mapIter = m_ObjectKeyMap.find( newName->GetUniqueID() );
	if (mapIter != m_ObjectKeyMap.end())
	{
		// new name found..error
		return MSG_ERROR;
	}

	// check to see if old name is in objectmap
	mapIter = m_ObjectKeyMap.find( oldName->GetUniqueID() );
	if (mapIter == m_ObjectKeyMap.end())
	{
		// old name not found..error
		return MSG_ERROR;
	}

	// all checks passed...proceed w/ renaming object name
	object = dynamic_cast<IObject*>(mapIter->second);
	if (object == NULL)
	{
		EngineGetToolBox()->SetErrorValue(ERR_NULL_POINTER);
		EngineGetToolBox()->Log(LOGERROR, _T("Cannot cast IComponent* to IObject* %s\n"), 
			oldName->GetString()); 
		return MSG_ERROR;
	}

	object->SetName(newName);

	// update the objectkeymap
	m_ObjectKeyMap.erase(mapIter);
	m_ObjectKeyMap[newName->GetUniqueID()] = object;

	return MSG_HANDLED_PROCEED;
}

DWORD CObjectFactory::OnGetComponentType(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(GETCOMPONENTTYPEPARAMS), size);
	GETCOMPONENTTYPEPARAMS *msgParams = (GETCOMPONENTTYPEPARAMS*)params;

	ObjectKeyMap_t::iterator mapItr;
	if (msgParams->name == NULL)
	{
		m_ToolBox->Log(LOGERROR, _T("Bad name pointer passed in OnGetComponentType\n"));
		return MSG_ERROR;
	}

	mapItr = m_ObjectKeyMap.find( msgParams->name->GetUniqueID() );
	if (mapItr != m_ObjectKeyMap.end())
	{
		IComponent *pComponent = mapItr->second;
		msgParams->componenttype = pComponent->GetComponentType();
		return MSG_HANDLED_STOP;
	}
	else
	{
		msgParams->componenttype = NULL;
		return MSG_ERROR;
	}
}

void CObjectFactory::DeleteDelayedObjects()
{
	// delete each object in the deleayed delete list
	UniqueIDSet_t::iterator iter;

	// Iterate through each item in the list
	for(iter = m_DelayedDeleteList.begin(); iter != m_DelayedDeleteList.end(); iter++)
	{
		// creat a hashstring from the uniqueid
		CHashString name(m_ToolBox->GetHashString(*iter));
		// Send the delete command
		DELETEOBJECTPARAMS dop;
		//dop.name = *iter;
		dop.name = &name;
		OnDeleteObject(sizeof(DELETEOBJECTPARAMS), &dop);
	}

	// make sure that the list is cleared
	m_DelayedDeleteList.clear();
}
DWORD CObjectFactory::OnFlagDuplicateObjectChecking(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(bool), size);
	if (!params)
	{
		MSG_ERROR;
	}
	m_bDuplicateObjectCheck = *(bool*)params;
	return MSG_HANDLED_STOP;
}
DWORD CObjectFactory::OnGenerateUniqueObjectName(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(GENERATEUNIQUEOBJECTNAMEPARAMS), size);
	GENERATEUNIQUEOBJECTNAMEPARAMS *msgParams = (GENERATEUNIQUEOBJECTNAMEPARAMS*)params;

	if (msgParams->name == NULL || msgParams->newname == NULL)
		return MSG_ERROR;

	TCHAR builtName[256];
	TCHAR originalName[256];
	_tcscpy( originalName, msgParams->name->GetString() );
	CHashString hszOriginalName( originalName );
	DWORD originalNameUniqueID = hszOriginalName.GetUniqueID();
	if( m_GeneratedNameMap.find( originalNameUniqueID ) == m_GeneratedNameMap.end() )
	{
		m_GeneratedNameMap[originalNameUniqueID] = 0;
	}

	ObjectKeyMap_t::iterator mapItr;
	// init i
	int i = m_GeneratedNameMap[originalNameUniqueID];

	// TODO: ugliness to try to continue from a previous generated name, would like to standardize our unqiue name id's
	// so less ugly logic can be used
	size_t len = _tcslen( originalName );
	if (len > 4)
	{
		if (originalName[len-1] >= '0' && originalName[len-1] <= '9' && 
			originalName[len-2] >= '0' && originalName[len-2] <= '9' && 
			originalName[len-3] >= '0' && originalName[len-3] <= '9' && 
			originalName[len-4] == '_')
		{
			_stscanf( &originalName[len-3], "%i", &i );
			originalName[len-4] = '\0';
		}
	}

	// find a unique name
	do
	{
		// build unique name
		sprintf(builtName, _T("%s_%03d"), originalName, i++);
		// convert to hash
		CHashString tempHash(builtName);
		// see if name exists
		mapItr = m_ObjectKeyMap.find( tempHash.GetUniqueID() );
	}
	while(mapItr != m_ObjectKeyMap.end());
	m_GeneratedNameMap[originalNameUniqueID] = i;

	*msgParams->newname = builtName;

	return MSG_HANDLED_STOP;
}
