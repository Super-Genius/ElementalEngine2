///==========================================================================
/// \file	PrecacheManager.cpp
/// \brief	Implementation of Precache Manager
/// \date	12/06/2007
/// \author	Patrick Ghiocel
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

#define PRECACHEMANAGERPRIORITY     0x0584

REGISTER_COMPONENT_SINGLETON( CPrecacheManager );
REGISTER_MESSAGE_HANDLER(Start, OnStart, CPrecacheManager);
REGISTER_MESSAGE_HANDLER(Stop, OnStop, CPrecacheManager);

REGISTER_MESSAGE_HANDLER(LoadFileByExtension, OnLoadFile, CPrecacheManager);
REGISTER_MESSAGE_HANDLER(RegisterLoadSaveComponent, OnRegisterLoadSave, CPrecacheManager);

REGISTER_MESSAGE_HANDLER(AddPrecache, OnAddPrecache, CPrecacheManager);
REGISTER_MESSAGE_HANDLER(RemovePrecache, OnRemovePrecache, CPrecacheManager);
REGISTER_MESSAGE_HANDLER(CommitPrecache, OnCommitPrecache, CPrecacheManager);


IComponent *CPrecacheManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CPrecacheManager );
}

CPrecacheManager::CPrecacheManager() : 
	CManager(_T("CPrecacheManager"), PRECACHEMANAGERPRIORITY, 0)
{
	m_bIsRunning = true;
	m_ExtensionMap.clear();
	m_ResourceMap.clear();
	m_PrecacheStack.clear();
	m_fLoadingUpdateFPS = 20.0f;
	Init();
}

CPrecacheManager::~CPrecacheManager()
{
	DeInit();
}

bool CPrecacheManager::Init()
{
	return CManager::Init();
}

bool CPrecacheManager::DeInit()
{
	// Remove all precaches from our precache list
	m_PrecacheStack.clear();

	return CManager::DeInit();
}

void CPrecacheManager::Update(DWORD tickCount)
{

}

bool CPrecacheManager::AddObject( IObject *Object, IHashString *group)
{
	// Let CManager add this precache
	return CManager::AddObject( Object, group );
}

bool CPrecacheManager::DeleteObject( IObject* Object )
{
	// Unload any decremented resources;

	// Remove it from our precache list
	PRECACHESTACK::iterator itr = m_PrecacheStack.begin();
	while( itr != m_PrecacheStack.end() )
	{
		if( Object->GetName()->GetUniqueID() == (*itr)->GetName()->GetUniqueID() )
		{
			m_PrecacheStack.erase( itr );
			break;
		}
		itr++;
	}

	// Let CManager remove this precache
	return CManager::DeleteObject( Object );
}

bool CPrecacheManager::CheckExtension( StdString szFileName )
{
	if( strlen( szFileName ) == 0 )
	{
		return false;
	}

	CHashString hszExt( strrchr(szFileName, '.') );
	EXTENSIONMAP::iterator itr = m_ExtensionMap.find(hszExt.GetUniqueID());

	if( itr != m_ExtensionMap.end() )
	{
		if( itr->second.m_pfnCallback != NULL  )
			return itr->second.m_pfnCallback(&hszExt);
		else if( itr->second.m_bIsPrecachable == true )
			return true;
		else
			return false;
	}
	return false;
}

void CPrecacheManager::CreatePrecacheObject( IHashString* pFileName )
{
	if( pFileName == NULL )
		return;

	static CHashString hszWorld( _T("World") );
	static CHashString hszType( _T("CPrecacheObject") );
	CHashString hszName( pFileName );

	CREATEOBJECTPARAMS cop;
	cop.parentName = NULL;
	cop.typeName = &hszType;
	cop.name = &hszName;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

	INITOBJECTPARAMS iop;
	iop.name = &hszName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
}

DWORD CPrecacheManager::OnStart(DWORD size, void *data)
{
	m_bIsRunning = true;
	return MSG_HANDLED_PROCEED;
}

DWORD CPrecacheManager::OnStop(DWORD size, void *data)
{
	m_bIsRunning = false;
	return MSG_HANDLED_PROCEED;
}

DWORD CPrecacheManager::OnLoadFile(DWORD size, void *data)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(LOADFILEEXTPARAMS));
	
	LOADFILEEXTPARAMS *lfep = (LOADFILEEXTPARAMS *)data;

	CHashString hszFileResource( lfep->fileName );

	if( CheckExtension( lfep->fileName ) )
	{
		RESOURCEMAP::iterator itr = m_ResourceMap.find( hszFileResource.GetUniqueID() );
		if( itr == m_ResourceMap.end() )
		{
			// A new resource, add the entry to the map
			RESOURCEINFO tmpInfo;
			tmpInfo.m_bLoaded = true;
			tmpInfo.m_iRefCount = 1;
			m_ResourceMap[hszFileResource.GetUniqueID()] = tmpInfo;

			// Add the new resource to the current Precache file if it exists
			if( m_PrecacheStack.size() != 0 )
			{
				m_PrecacheStack.front()->AddResource( &hszFileResource );
			}
		}
	}
	
	return MSG_HANDLED_PROCEED;
}

DWORD CPrecacheManager::OnRegisterLoadSave(DWORD size, void *data)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(LSRC_MESSAGE));

    LSRC_MESSAGE* msg = (LSRC_MESSAGE *)data;

	LPCTSTR testExt;
	LPCTSTR tStr;
	size_t tLen;

	tStr = msg->m_FilterStrings.m_LoadFilter;
	if (tStr != NULL)
	{
		while ((*tStr != '\0') && (*(tStr+1) != '\0'))
		{
			// get length of open/import filters
			tLen = _tcslen(tStr)+1;
			// bump past description strings
			tStr += tLen;
			testExt = _tcschr(tStr, '.');
			// search until end of string
			while (testExt != NULL)
			{
				EXTENSIONINFO extinfo;
				extinfo.m_bIsPrecachable = msg->m_bPrecachable;
				extinfo.m_pfnCallback = msg->m_pfnCallback;
				CHashString hszExt( testExt );
				m_ExtensionMap[hszExt.GetUniqueID()] = extinfo;
				testExt = _tcschr(testExt+1, '.');
			}
			tStr += _tcslen(tStr)+1;
		}
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CPrecacheManager::OnAddPrecache(DWORD size, void *data)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));
	// Get the file name
	IHashString* file = (IHashString*)data;

	// Check to see if the object exists already
	IObject* Object = NULL;
	GetObjectByName( file, NULL );
	if( Object != NULL )
	{
		m_ToolBox->Log( LOGWARNING, _T("Precache file %s already loaded!"), file->GetString() );
		return MSG_ERROR;
	}

	// Load the precache
	LOADFILEEXTPARAMS lfep;
	lfep.fileName = (TCHAR*)file->GetString();
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);

	// If for some reason the file could not be loaded
	if( retval != MSG_HANDLED )
	{
		// Manually create the CPrecache Object ourselves
		CreatePrecacheObject( file );
	}

	// Add it to our stack
	Object = GetObjectByName( file, NULL );
	CPrecacheObject *pPrecacheObject = dynamic_cast<CPrecacheObject*>( Object );
	m_PrecacheStack.push_front( pPrecacheObject );

	// Add Resources to the Current Manager list
	RESOURCESET* tmpSet = pPrecacheObject->GetResourceList();
	RESOURCESET::iterator itrRSet = tmpSet->begin();
	while( itrRSet != tmpSet->end() )
	{
		DWORD hashval = (*itrRSet);
		RESOURCEMAP::iterator itrRMap = m_ResourceMap.find( hashval );
		if( itrRMap == m_ResourceMap.end() )
		{
			// A new resource, add the entry to the map
			RESOURCEINFO tmpInfo;
			tmpInfo.m_bLoaded = false;
			tmpInfo.m_iRefCount = 1;
			m_ResourceMap[hashval] = tmpInfo;
		}
		else
		{
			itrRMap->second.m_iRefCount++;
		}
		itrRSet++;
	}
		
	return MSG_HANDLED_STOP;
}

DWORD CPrecacheManager::OnRemovePrecache(DWORD size, void *data)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString*));
	// Get the file name
	IHashString* file = (IHashString*)data;

	CPrecacheObject *pPrecacheObject = NULL;
		
	/// Check for the object to be loaded
	PRECACHESTACK::iterator itr = m_PrecacheStack.begin();
	while( itr != m_PrecacheStack.end() )
	{
		if( (*itr)->GetName()->GetUniqueID() == file->GetUniqueID() )
		{
			pPrecacheObject = *itr;
			break;
		}
		else
		{
            itr++;
		}
	}

	// If the Precahce wasn't loaded to begin with
	if( pPrecacheObject == NULL )
	{
		m_ToolBox->Log( LOGWARNING, _T("Could not remove Precache file %s . File was not loaded."), file->GetString() );
		// Just exit out
		return MSG_HANDLED_STOP;
	}

	// Else go thru its resource set and decrement all resources in the main resourcemap
	RESOURCESET* tmpSet = pPrecacheObject->GetResourceList();
	RESOURCESET::iterator itrRSet = tmpSet->begin();
	while( itrRSet != tmpSet->end() )
	{
		DWORD hashval = (*itrRSet);
		RESOURCEMAP::iterator itrRMap = m_ResourceMap.find( hashval );
		if( itrRMap != m_ResourceMap.end() )
			itrRMap->second.m_iRefCount--;
		itrRSet++;
	}
	
	// Remove from the Precache List
	m_PrecacheStack.erase( itr );

	// Delete the object (it will save itself out)
	DELETEOBJECTPARAMS dop;
	dop.name = file;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop );

	return MSG_HANDLED_STOP;
}

DWORD CPrecacheManager::OnCommitPrecache(DWORD size, void *data)
{
	// No need to check this message. No arguments
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	static DWORD msgHash_UnloadFileByExtension = CHashString(_T("UnloadFileByExtension")).GetUniqueID();

	RESOURCEMAP::iterator itrRMapDel;
	RESOURCEMAP::iterator itrRMap = m_ResourceMap.begin();
	UINT mapSize = m_ResourceMap.size();
	UINT pos = 0;

	float secondsPerFrame = 1.0f / m_fLoadingUpdateFPS;
	ITimer *timer = EngineGetToolBox()->GetTimer();
	float lastLoadTime = timer->GetRealTime();
	float elapsedTime = 0.0f;
	// Run thru the list of resources 
	while( itrRMap != m_ResourceMap.end() )
	{
		// If a resource has a ref count > 0 
		if(itrRMap->second.m_iRefCount > 0)
		{
			// If bLoaded == false, Load the resource
			if( itrRMap->second.m_bLoaded == false )
			{
				// hacky mchackington
				static DWORD msgCacheSpawnFile = CHashString(_T("CacheSpawnFile")).GetUniqueID();
				TCHAR ext[_MAX_EXT];
				TCHAR *fileName = (TCHAR*)m_ToolBox->GetHashString( itrRMap->first );
				_tsplitpath(fileName, NULL, NULL, NULL, ext);

				if (!_tcscmp(_T(".sxl"), ext))
				{
					DWORD retval = EngineGetToolBox()->SendMessage(msgCacheSpawnFile, sizeof(TCHAR*), fileName);
					if( retval != MSG_HANDLED )
					{
						// Error Loading Resource. Removing it from the list
						m_ToolBox->Log( LOGWARNING, _T("Could not load resource %s from CommitPrecache call.\n"), m_ToolBox->GetHashString(itrRMap->first) );
						itrRMapDel = itrRMap;
						itrRMap++;
						m_ResourceMap.erase( itrRMapDel );
					}
					else
					{
						itrRMap->second.m_bLoaded = true;
						itrRMap++;
					}
				}
				else
				{
					LOADFILEEXTPARAMS lfep;
					lfep.fileName = fileName;
					lfep.bInternalLoad = true;
					DWORD retval = EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
					if( retval != MSG_HANDLED )
					{
						// Error Loading Resource. Removing it from the list
						m_ToolBox->Log( LOGWARNING, _T("Could not load resource %s from CommitPrecache call.\n"), m_ToolBox->GetHashString(itrRMap->first) );
						itrRMapDel = itrRMap;
						itrRMap++;
						m_ResourceMap.erase( itrRMapDel );
					}
					else
					{
						itrRMap->second.m_bLoaded = true;
						itrRMap++;
					}

					elapsedTime += timer->GetRealTime() - lastLoadTime;
					lastLoadTime = timer->GetRealTime();
					if (elapsedTime > secondsPerFrame)
					{
						elapsedTime = 0.0f;
						// update the gui, renderer, and audio engines
						LOADINGUPDATEPARAMS lup;
						lup.currentPosition = (DWORD)pos;
						lup.totalSize = (DWORD)mapSize;
						static DWORD msgHash_LoadingUpdate = CHashString(_T("LoadingUpdate")).GetUniqueID();
						m_ToolBox->SendMessage(msgHash_LoadingUpdate, sizeof(LOADINGUPDATEPARAMS), &lup);
					}
				}
			}
			else
			{
				itrRMap++;
			}
			pos++;
		}
		// Else the resource has an ref count of 0. Unload the resource and remove it from the list
		else
		{
			// Commented out because of inter-dependency issues with various resources
			
			LOADFILEEXTPARAMS lfep;
			lfep.fileName = (TCHAR*)m_ToolBox->GetHashString( itrRMap->first );
			lfep.bInternalLoad = true;	
			DWORD retval = EngineGetToolBox()->SendMessage(msgHash_UnloadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
			if( retval != MSG_HANDLED )
			{
				// Error Loading Resource. Removing it from the list
				m_ToolBox->Log( LOGWARNING, _T("Could not unload resource %s from CommitPrecache call.\n"), m_ToolBox->GetHashString(itrRMap->first) );
			}	
			
			itrRMapDel = itrRMap;
			itrRMap++;
			m_ResourceMap.erase( itrRMapDel );
		}
	}
	return MSG_HANDLED_STOP;
}
