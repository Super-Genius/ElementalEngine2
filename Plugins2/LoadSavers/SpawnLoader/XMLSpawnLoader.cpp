///============================================================================
/// \file		XMLSpawnLoader.cpp
/// \brief		Implementation file for XML based Spawn Loader
/// \date		08-12-2005
/// \author		Josh Lulewicz
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
#include "IXMLArchive.h"
#include "BaseMessages.h"
#include "PerformanceProfiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON(CXMLSpawnLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadSpawnFile, CXMLSpawnLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveSpawnFile, CXMLSpawnLoader);
REGISTER_MESSAGE_HANDLER(CloseFile, OnRemoveSpawnFile, CXMLSpawnLoader);
REGISTER_MESSAGE_HANDLER(GetSpawnedName, OnGetSpawnedName, CXMLSpawnLoader);
REGISTER_MESSAGE_HANDLER(SetSpawnParentName, OnSetSpawnParentName, CXMLSpawnLoader);
REGISTER_MESSAGE_HANDLER(SetSpawnPosition, OnSetSpawnPosition, CXMLSpawnLoader);
REGISTER_MESSAGE_HANDLER(SetSpawnEuler, OnSetSpawnEuler, CXMLSpawnLoader);
REGISTER_MESSAGE_HANDLER(SpawnFromXMLArchive, OnSpawnFromXMLArchive, CXMLSpawnLoader);
REGISTER_MESSAGE_HANDLER(CacheSpawnFile, OnCacheSpawnFile, CXMLSpawnLoader);

CHashString CXMLSpawnLoader::m_HashName(_T("CXMLSpawnLoader"));

LSRC_MESSAGE CXMLSpawnLoader::m_LoadSaveRegisterData = {
		&CXMLSpawnLoader::m_HashName,
		_T("XML Spawn Files (*.sxl)\0" "*.sxl\0"
		"\0\0"),
		_T("XML Spawn Files (*.sxl)\0" "*.sxl\0"
		"\0\0"),
		true, NULL,
};

CXMLSpawnLoader::CXMLSpawnLoader() 
{
	// init last spawned name
	m_LastSpawnedName = _T("");
	m_NextSpawnParentName = _T("");
	m_InitName = _T("");
	m_bParentNameSet = false;
	m_bIntializingObject = false;
	// save for convenience
	m_ToolBox = EngineGetToolBox();
	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE), &m_LoadSaveRegisterData);

	m_vSpawnedPosition.Set(0.0f, 0.0f, 0.0f);
}

CXMLSpawnLoader::~CXMLSpawnLoader()
{
	// iterator for spawn map
	SPAWNMAP::iterator smIter;
	// temp xml-archive pointer
	IXMLArchive *tempAr;

	// go through map and close the archives
	for (smIter = m_SpawnFilesLoadedMap.begin(); smIter != m_SpawnFilesLoadedMap.end(); ++smIter)
	{
		// grab xml archive pointer
		tempAr = smIter->second;
		// close archive
		tempAr->Close();
	}
}


// create function for this world loader
IComponent *CXMLSpawnLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CXMLSpawnLoader );
}

IHashString *CXMLSpawnLoader::GetComponentType()
{
	return &m_HashName;
}

bool CXMLSpawnLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// load a file if we recognize the type.
DWORD CXMLSpawnLoader::OnLoadSpawnFile(DWORD size, void *params)
{
	IXMLArchive *Archive = NULL;
	bool first = true;
	// iterator for spawn map
	SPAWNMAP::iterator smIter;
	// keep name on local stack in case of recursion
	CHashString curName;

	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	// use both a hashstring and a pointer so that the first iteration of the loop
	// will pass SetParentName a NULL instead of an address of a hashstring
	// hashed to ""
	CHashString hszParentName(_T(""));
	CHashString hszName(_T(""));
	CHashString hszTypeName(_T(""));
	CHashString streamType(_T("File"));
	
	// hash the file name
	CHashString file(pFileName);

	// see if the file is already loaded
	smIter = m_SpawnFilesLoadedMap.find(file.GetUniqueID());

	// if not
	if (smIter == m_SpawnFilesLoadedMap.end())
	{
		// create a new archive and load the file
		CREATEARCHIVE ca;
		ca.streamData = (void *)pFileName;
		ca.mode = STREAM_MODE_READ;
		ca.streamType = &streamType;
		// call the Archive factory to create an XML archive
		static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
		{
			return MSG_ERROR;
		}

		Archive = dynamic_cast<IXMLArchive *>(ca.archive);

		// add new archive to map
		m_SpawnFilesLoadedMap[file.GetUniqueID()] = Archive;
	}
	else
	{
		// otherwise grab the existing archive!
		Archive = smIter->second;
		// reset it
		Archive->SeekTo(0);
	}

	DWORD result = OnSpawnFromXMLArchive(sizeof(Archive), Archive);
	return result;
}

// Save a file if we recognize the type.
DWORD CXMLSpawnLoader::OnSaveSpawnFile(DWORD size, void *params)
{
	// Check the message to verify the size
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	if( pFileName == NULL )
	{
		return MSG_ERROR;
	}

	IXMLArchive *XMLOutput;
	CREATEARCHIVE ca;
	ca.streamData = pFileName;
	ca.mode = STREAM_MODE_WRITE;

	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return MSG_ERROR;
	}

	XMLOutput = dynamic_cast<IXMLArchive *>(ca.archive);

	CSpawnVisitor SpawnVisitor;
	SpawnVisitor.SetArchiver( XMLOutput );
	
	VISITHIERARCHYDATA vhd;
	vhd.name = NULL;
	vhd.visitor = &SpawnVisitor;

	static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(VISITHIERARCHYDATA), &vhd, NULL, NULL);

	XMLOutput->Close();

	return MSG_HANDLED_STOP;
}

// close a file if we recognize the type.
DWORD CXMLSpawnLoader::OnRemoveSpawnFile(DWORD size, void *params)
{
	// Check the message to verify the size
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)params;

	if( pFileName == NULL )
	{
		return MSG_ERROR;
	}

	SPAWNMAP::iterator smIter;

	CHashString fileName(pFileName);

	smIter = m_SpawnFilesLoadedMap.find(fileName.GetUniqueID());

	if (smIter != m_SpawnFilesLoadedMap.end())
	{
		IXMLArchive *ar = smIter->second;
		
		ar->Close();
	}
	else
	{
		// spawn file not open, unable to close
		return MSG_NOT_HANDLED;
	}

	return MSG_HANDLED_STOP;
}

DWORD CXMLSpawnLoader::OnGetSpawnedName(DWORD size, void *params)
{
	// Check the message to verify the size
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));

	IHashString *name = (IHashString*)params;
	if (m_bIntializingObject)
	{
		*name = m_InitName;
	}
	else
	{
		*name = m_LastSpawnedName;
	}

	return MSG_HANDLED_STOP;
}

////////////////////////////////////////////////////////////////////////
/// this is for setting the parent name of the spawn item
/// this is temporary for TESTING only
DWORD CXMLSpawnLoader::OnSetSpawnParentName(DWORD size, void *params)
{
	// Check the message to verify the size
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));

	IHashString *name = (IHashString*)params;

	m_NextSpawnParentName = name;
	m_bParentNameSet = true;

	return MSG_HANDLED_STOP;
}

DWORD CXMLSpawnLoader::OnSetSpawnPosition(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(Vec3));

	Vec3 pos = *(Vec3*) params;

	m_vSpawnedPosition.Set(pos);

	return MSG_HANDLED_STOP;
}

DWORD CXMLSpawnLoader::OnSetSpawnEuler(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(EulerAngle));

	EulerAngle euler = *(EulerAngle*) params;

	m_eSpawnedEuler.Set(euler);

	return MSG_HANDLED_STOP;
}

DWORD CXMLSpawnLoader::OnSpawnFromXMLArchive(DWORD size, void *params)
{
	IXMLArchive *Archive = reinterpret_cast<IXMLArchive *>(params);
	VERIFY_MESSAGE_SIZE(size, sizeof(Archive));

	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	SERIALIZEOBJECTPARAMS sop;
	bool first = true;
	PARENTNAMEMAP mapObjectIDtohszParentName;
	PARENTNAMEMAP::iterator pnmIter;
	// iterator for spawn map
	SPAWNMAP::iterator smIter;
	// keep name on local stack in case of recursion
	CHashString curName;

	// use both a hashstring and a pointer so that the first iteration of the loop
	// will pass SetParentName a NULL instead of an address of a hashstring
	// hashed to ""
	CHashString hszParentName(_T(""));
	CHashString hszName(_T(""));
	CHashString hszTypeName(_T(""));

	// if the parent name is not setup
	if (!m_bParentNameSet)
	{
		// default to world
		hszParentName = _T("World");

		// use the currently active scene if set
		GETSETACTIVESCENE gsas;
		gsas.pSceneName = NULL;
		static DWORD msgHash_GetActiveScene = CHashString(_T("GetActiveScene")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetActiveScene, sizeof(gsas), &gsas ) == MSG_HANDLED)
		{
			if (gsas.pSceneName)
				hszParentName = gsas.pSceneName;
		}
	}
	else
	{
		// otherwise use the set parent name
		hszParentName = m_NextSpawnParentName;
		// reset next spawn parent name so the next object that is spawned
		// doesn't use it
		m_NextSpawnParentName = _T("");
		m_bParentNameSet = false;
	}	

	// this version coordinates with the world version 1.5 serialization
	CHashString currentVersion = _T("1.5");
	CHashString oldVersion = _T("");
	static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &oldVersion);
	if (retval != MSG_HANDLED)
	{
		Archive->Close();
		m_ToolBox->Log(LOGERROR, _T("Could not get world version!"));
		return MSG_ERROR;
	}

	//TODO: Fix this so that it can be called after the GetNode() call
	// bug #0000878: Error with GetNumAttributes() in CXMLArchive 
	DWORD numAttributes = Archive->GetNumAttributes();

	StdString szNodeName;
	Archive->GetNode(szNodeName);

	// must start with <Spawn>
	if (szNodeName != _T("CSpawner"))
	{
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_FILE);
		EngineGetToolBox()->Log(LOGWARNING, _T("Invalid spawn file\n"));
		Archive->Close();
		return MSG_ERROR;
	}
	else
	{
		// read the name of the Spawner
		StdString szName;
		Archive->Read( szName, _T("Name") );
		CHashString root(szName.c_str());
		// store name of the root in the map
		mapObjectIDtohszParentName[root.GetUniqueID()] = hszParentName;

		// This Spawn File specifies its version
		if( numAttributes >= 2 )
		{
			StdString tmpVersion;
			Archive->Read( tmpVersion, _T("Version") );
			currentVersion = tmpVersion;
		}

		static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &currentVersion);
		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("Could not set world version!"));
			Archive->Close();
			return MSG_ERROR;
		}
	}

	// obj create error checking
	bool componentFailed = false;
	int failedDepth = 0;

	// check to see if we've set this guy already
	bool bPositionSet = false;
	bool bEulerSet = false;

	while (Archive->GetNode(szNodeName))
	{
		int currDepth = Archive->GetDepth();
		if (componentFailed)
		{
			// if we're a child of the failed component
			if (currDepth > failedDepth)
			{
				continue;
			}
			// if we're a sibling or an uncle, reset the flag
			else
			{
				componentFailed = false;
			}
		}

		hszTypeName.Init( szNodeName.c_str() );

		StdString parentName;
		StdString parentType;
		StdString childType;
		Archive->GotoParentNode( parentType );
		Archive->Read( parentName, _T("Name") );
		if ( parentName != _T("") )
		{
			hszParentName = parentName;
		}
		Archive->ReturnToChildNode( childType );

		StdString szName;
		Archive->Read( szName, _T("Name") );

		CHashString rootName( szName );
		GENERATEUNIQUEOBJECTNAMEPARAMS guonp;
		guonp.name = &rootName;
		guonp.newname = &hszName;
		static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(guonp), &guonp ) != MSG_HANDLED)
		{
			Archive->Close();
			return MSG_ERROR;
		}

#if defined(_DEBUG) || defined(_PROFILE)
		StdString szProfileDesc;
//		szProfileDesc = szSpawnerName; // spawner
//		szProfileDesc += _T("::");
		szProfileDesc += hszName.GetString(); // object name
		szProfileDesc += _T("::");
		szProfileDesc += szNodeName; // object type
		PERFORMANCE_PROFILER_TYPE_START(szProfileDesc, _T("Spawn Loader"));
#endif

		// if the is the first item
		if (first)
		{
			// save off the name
			curName = m_InitName = hszName;
			// no longer the first item
			first = false;
		}

		// convert old name to a hashstring
		CHashString oldName(szName.c_str());
		// add to map
		mapObjectIDtohszParentName[oldName.GetUniqueID()] = hszName;

		// find name in the map
		pnmIter = mapObjectIDtohszParentName.find(hszParentName.GetUniqueID());

		if (pnmIter == mapObjectIDtohszParentName.end())
		{
			// log error
			static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
			retval = m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
			return MSG_ERROR;
		}

		// use generated name for parent
		StdString npn = pnmIter->second.GetString();
		CHashString newParentName(npn.c_str());
		cop.parentName = &newParentName;
		cop.typeName = &hszTypeName;
		cop.name = &hszName;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGINFORMATION, _T("Spawn loader: component failed to be created; skipping children!\n"));
			componentFailed = true;
			failedDepth = Archive->GetDepth();
			continue;
		}

		sop.name = &hszName;
		sop.archive = Archive;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

		m_bIntializingObject = true;
		iop.name = &hszName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);

		if (m_vSpawnedPosition.Length() > 0.0f)
		{
			if (!bPositionSet)
			{
				static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
				retval = m_ToolBox->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), &m_vSpawnedPosition, &hszName);
				if (retval == MSG_HANDLED)
				{
					// reset spawned position
					m_vSpawnedPosition.Set(0.0f, 0.0f, 0.0f);
					bPositionSet = true;
				}
			}
		}

		if (m_eSpawnedEuler.GetYaw() != 0.0f && m_eSpawnedEuler.GetPitch() != 0.0f && m_eSpawnedEuler.GetRoll() != 0.0f)
		{
			if (!bEulerSet)
			{
				static DWORD msgHash_SetGlobalEuler = CHashString(_T("SetGlobalEuler")).GetUniqueID();
				retval = m_ToolBox->SendMessage(msgHash_SetGlobalEuler, sizeof(Vec3), &m_eSpawnedEuler, &hszName);
				if (retval == MSG_HANDLED)
				{
					// reset spawned position
					m_eSpawnedEuler.Set(0.0f, 0.0f, 0.0f);
					bEulerSet = true;
				}
			}
		}

		m_bIntializingObject = false;
		// send out a message that an object needs to be added (hierarchy view)
		// may not want this happening for objects that are spawned in game...
		static DWORD msgHash_AddObject = CHashString(_T("AddObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_AddObject, sizeof(CREATEOBJECTPARAMS), &cop);

		PERFORMANCE_PROFILER_TYPE_STOP(szProfileDesc, _T("Spawn Loader"));
	}

	m_LastSpawnedName = curName;

	static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
	retval = m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
	return MSG_HANDLED_STOP;
}

DWORD CXMLSpawnLoader::OnCacheSpawnFile(DWORD size, void *param)
{
	IXMLArchive *Archive = NULL;
	// iterator for spawn map
	SPAWNMAP::iterator smIter;
	
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFileName = (TCHAR *)param;

	CHashString streamType(_T("File"));
	
	// hash the file name
	CHashString file(pFileName);

	// see if the file is already loaded
	smIter = m_SpawnFilesLoadedMap.find(file.GetUniqueID());

	// if not
	if (smIter == m_SpawnFilesLoadedMap.end())
	{
		// create a new archive and load the file
		CREATEARCHIVE ca;
		ca.streamData = (void *)pFileName;
		ca.mode = STREAM_MODE_READ;
		ca.streamType = &streamType;
		// call the Archive factory to create an XML archive
		static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
		{
			return MSG_ERROR;
		}

		Archive = dynamic_cast<IXMLArchive *>(ca.archive);

		// add new archive to map
		m_SpawnFilesLoadedMap[file.GetUniqueID()] = Archive;
	}

	return MSG_HANDLED_PROCEED;
}