///============================================================================
/// \file		LuaManager.cpp
/// \brief		Implementation file for CLuaManager
/// \date		05-27-2005
/// \author		Josh Lulewicz
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
#include "PerformanceProfiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static const char THIS_FILE[] = __FILE__;
#endif

#define LUA_MAX_GARBAGE_SIZE	512 // in Kb

using namespace ElementalEngine;

// Exported function 
TOLUA_API int tolua_EELuaScript_open (lua_State* tolua_S);

REGISTER_COMPONENT_SINGLETON(CLuaManager);

REGISTER_MESSAGE_HANDLER(AddScript, OnAddScript, CLuaManager);
REGISTER_MESSAGE_HANDLER(RemoveScript, OnRemoveScript, CLuaManager);
//REGISTER_MESSAGE_HANDLER(UpdateScript, OnUpdateScript, CLuaManager);
REGISTER_MESSAGE_HANDLER(LoadAndStartScript, OnLoadAndStartScript, CLuaManager);
REGISTER_MESSAGE_HANDLER(GetScript, OnGetScript, CLuaManager);
REGISTER_MESSAGE_HANDLER(ReleaseScript, OnReleaseScript, CLuaManager);
REGISTER_MESSAGE_HANDLER(PauseScript, OnPauseScript, CLuaManager);
REGISTER_MESSAGE_HANDLER(RestartScript, OnRestartScript, CLuaManager);
REGISTER_MESSAGE_HANDLER(GetMasterScriptState, OnGetMasterScriptState, CLuaManager);
REGISTER_MESSAGE_HANDLER(GetLuaArchive, OnGetLuaArchive, CLuaManager);
REGISTER_MESSAGE_HANDLER(GetScriptStats, OnGetScriptStats, CLuaManager);

// table of function names and entry points that will be registered with Lua
static const luaL_reg scriptLib[] =
{
	{"Beep",         &CLuaManager::LuaBeep},
	{"WaitFrame",    &CLuaManager::LuaWaitFrame},
	{"WaitTime",     &CLuaManager::LuaWaitTime},
	{"WaitSec",      &CLuaManager::LuaWaitSec},
	{NULL, NULL}
};

void LuaOpenScriptLib(lua_State*  l)
{
	// register our library of routines under the table name Script
	luaL_openlib(l, "Script", scriptLib, 0);
}

void OutputDebug(const char* show)
{
	if (show != NULL)
	{
		EngineGetToolBox()->Log(LOGINFORMATION, show);
	}
	else
	{
		EngineGetToolBox()->Log(LOGINFORMATION, _T("Somebody output NULL!"));
	}
};

void OutputDebug(StdString& it)
{
	OutputDebug((const TCHAR*)it);
};

void OutputDebug(int logLevel, const char* show)
{
	if (show != NULL)
	{
		EngineGetToolBox()->Log((DWORD)logLevel, show);
	}
	else
	{
		EngineGetToolBox()->Log(logLevel, _T("Somebody output NULL!"));
	}
}

void OutputDebug(int logLevel, StdString& it)
{
	OutputDebug((DWORD)logLevel, (const TCHAR*)it);
}

void LuaCBreak()
{
	DebugBreak();
    //__debugbreak();
};

void *CLuaManager::Allocator(void *ud, void *ptr, size_t osize, size_t nsize) {
  (void)ud;
  (void)osize;
  if (nsize == 0) {
    dlfree(ptr);
    return NULL;
  }
  else
    return dlrealloc(ptr, nsize);
}

CLuaManager::CLuaManager(void) : 
	CManager(_T("CLuaManager"), LUAPRIORITY)
{
	m_CurrentExecuteScript = NULL;

	try 
	{
		m_pMasterState = lua_newstate(&CLuaManager::Allocator, NULL);
	}
	catch (...)
	{
		m_ToolBox->Log(LOGERROR, "%s, %s: Error caught\n", __FILE__, __LINE__);
		return;
	}
	
    if (m_pMasterState)
	{
 #ifdef LUA_VERSION_NUM /* lua 5.1 */
		luaL_openlibs(m_pMasterState);
 #else
        // open any auxiliary libaraies we may need
        luaopen_base(m_pMasterState); 
        luaopen_io(m_pMasterState); 
        luaopen_string(m_pMasterState); 
        luaopen_math(m_pMasterState);
		luaopen_table(m_pMasterState);
#endif
		tolua_EELuaScript_open(m_pMasterState);
		LuaOpenScriptLib(m_pMasterState);

		lua_atpanic(m_pMasterState, CLuaManager::LuaPanic);
		m_iLastGarbageCollectionCount = lua_getgccount(m_pMasterState);
	}

	m_fLastTime = ((float)GetTickCount()/1000.0f);
	m_iGlobalCount = 0;
    m_pLuaArchive =0;
}

CLuaManager::~CLuaManager(void)
{
	DeInit();
}

IComponent* CLuaManager::Create(int nArgs, va_list argptr)
{
	// Return the singleton instance of this class
	return SINGLETONINSTANCE( CLuaManager );
}

//
//	adds an Object to the Manager
//
bool CLuaManager::AddObject( IObject *Object, IHashString *group)
{
	static CHashString CLSName(_T("CLuaScript"));
	// add object to map
	if (Object->IsKindOf( &CLSName ))
	{
		//The proper value for this will be set in OnGetScript() handler
		//newScript.fileName = 

		SCRIPTOBJECTSTRUCT newScript;
		newScript.obj = Object;
		newScript.updateFlag = true;
		IHashString *tempHash = Object->GetName();
		m_LuaScripts[tempHash->GetUniqueID()] = newScript;
	}

	// add to object list
	m_Objects.insert(Object);

	return true;
}

//
// Deletes a Lua Object and removes it from the Lua System
//
bool CLuaManager::DeleteObject( IObject* Object )
{
	OBJECTLIST::iterator olIter;

	CHashString typeName(_T("CLuaScript"));
	if (Object->IsKindOf( &typeName ))
	{
		SCRIPTOBJECTMAP::iterator itrScript;
		//Decrement reference count to associated .lua file.
		//The AddObject routine does not have the symmetric counterpart (increment ref count),
		//instead CLuaManager::OnGetScript() has that duty.  This is because AddObject() is
		//called by the CObjectTemplate constructor of CLuaScript, wherein the name of the
		//.lua file to associate the "increment" with, is not yet known.  Only after CLuaScript
		//has serialized and gone into its "Init()", is it then ready to know its .lua filename.
		//And it is at that point which OnGetScript is called.  I think it would be better if
		//CLuaManager had a seperate "SetAssociation(<scriptobjname>,<filename>)" method which 
		//CLuaScript::Init() would call for the purpose of incrementing the reference count, 
		//but we're good for now.
		IHashString *tempHash = Object->GetName();
		itrScript = m_LuaScripts.find(tempHash->GetUniqueID());
		if (itrScript!=m_LuaScripts.end())
		{
			CHashString& fileName = itrScript->second.fileName;
			SCRIPTBODYMAP::iterator itrScriptName = m_ScriptNames.find(fileName.GetUniqueID());
			assert("LuaScript must have name matched to map" && itrScriptName!=m_ScriptNames.end());
			--itrScriptName->second.refCount;
		}
		else
		{
			StdString error;
			error = _T( __FILE__ _T(" DeleteObject(): cannot delete undocumented object\n") );
			assert( (const char*)error && 0 );
			EngineGetToolBox()->SetErrorValue( WARN_INVALID_OPERATION );
			EngineGetToolBox()->Log( LOGWARNING, error );
		}

		// erase the object in our map
		m_LuaScripts.erase(tempHash->GetUniqueID());
	}

	// remove from object list
	m_Objects.erase(Object);

	return true;
}

void CLuaManager::Update(DWORD tickCount)
{
//	PERFORMANCE_PROFILER_AUTO( Lua_Update );

	// get global elapsed time
	float deltaTime = (float)(tickCount) / 1000.0f;

	for(SCRIPTOBJECTMAP::iterator itor = m_LuaScripts.begin(); itor != m_LuaScripts.end(); )
	{
		// in case script goes away we want the next valid itor.
		SCRIPTOBJECTMAP::iterator itorNext = itor;
		itorNext++;

		
		SCRIPTOBJECTSTRUCT script = itor->second;
		
		if (script.updateFlag)
		{
			m_CurrentExecuteScript = dynamic_cast<CLuaScript *>(script.obj);
			m_CurrentExecuteScript->Update(deltaTime);
			m_CurrentExecuteScript = NULL;
		}
		itor = itorNext;
	}

	lua_gc(m_pMasterState, LUA_GCSTEP, 96);

	// forward mem usage to profiler
	SCRIPTSTATS ss;
	OnGetScriptStats(sizeof(SCRIPTSTATS), &ss);

	static DWORD msgUpdatePerformanceProfiler = CHashString(_T("UpdatePerformanceProfiler")).GetUniqueID();
	PERFORMANCEPROFILERUPDATE ppu;
	static CHashString hsMemUsage(_T("Memory Usage"));
	static CHashString hsLuaScriptMemUsage(_T("Lua Script Memory Usage"));
	ppu.name = &hsLuaScriptMemUsage;
	ppu.time = (float)ss.iMemoryUsageKb;
	ppu.type = &hsMemUsage;
	ppu.displayType = NAME_NUMBER;

	m_ToolBox->SendMessage(msgUpdatePerformanceProfiler, sizeof(PERFORMANCEPROFILERUPDATE), &ppu);
}

bool CLuaManager::DeInit()
{
	// script body map iterator
	SCRIPTBODYMAP::iterator sbmIter;
	// script object map iterator
	SCRIPTOBJECTMAP::iterator somIter;
	SCRIPTBODYSTRUCT *tempBodyStruct;

    somIter = m_LuaScripts.begin();
	DELETEOBJECTPARAMS dop;
    while( !m_LuaScripts.empty() )
    {
    	CHashString hszName( m_ToolBox->GetHashString(somIter->first) );
		dop.name = &hszName;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop, NULL, NULL ); 
		somIter = m_LuaScripts.begin();
    }

	// free all scripts
	for ( sbmIter = m_ScriptNames.begin(); sbmIter != m_ScriptNames.end(); ++sbmIter )
	{
		// grab the body struct
		tempBodyStruct = &sbmIter->second;

		// free script body
		SAFE_DELETE_ARRAY( tempBodyStruct->scriptBody );
		
		//The macro version did not compile under all platforms
		if (tempBodyStruct->compiledScript) 
		{
			delete[] ( unsigned char* )(tempBodyStruct->compiledScript);
			tempBodyStruct->compiledScript = NULL;
		}
		
		// make sure that this script body isn't referenced by anyone
		if ( tempBodyStruct->refCount != 0 )
		{
			EngineGetToolBox()->Log( LOGWARNING, _T( __FILE__ _T(" DeInit(): Reference count is wrong\n") ) );
		}
	}

    m_ScriptNames.clear();
	m_LuaScripts.clear();

	if (m_pMasterState)
	{
		try
		{
			try 
			{
				lua_close( m_pMasterState );
				m_pMasterState = NULL;
			}
			catch (DWORD errorCode)
			{
				m_ToolBox->Log(LOGERROR, "%s %s: Error %d caught\n", __FILE__, __LINE__, errorCode);
			}
		}
		catch(...)
		{
			m_ToolBox->Log(LOGERROR, "%s %s: Some error was caught\n", __FILE__, __LINE__);
		}
	}

    if (m_pLuaArchive)
    {
		m_pLuaArchive->SetArchive(NULL); // remove any leftover archive reference before calling delete
        SAFE_DELETE( m_pLuaArchive );
    }

	CManager::DeInit();

	m_ToolBox->UnregisterMessagePump(this, LUAPRIORITY);

	return true;
}

/// \brief AddScript message handler
/// This message is usually sent by the LuaLoader
DWORD CLuaManager::OnAddScript(DWORD size, void *params)
{
	// make sure the size is correct
	VERIFY_MESSAGE_SIZE(size, sizeof(SCRIPTPARAMS));

	// extract the SCRIPTPARAMS from the void pointer
	SCRIPTPARAMS *script = (SCRIPTPARAMS *)params;

	SCRIPTBODYSTRUCT newBody;

	// create a script body structure
	// giving it the data in script
	newBody.scriptBody = script->scriptData;
	// and a reference count
	newBody.refCount = 1;
	// store the size also (for writing)
	newBody.size = script->dataSize;
	// attempt to make a compiled version of the lua script
	newBody.compiledScript = NULL;
	newBody.compiledSize = 0;
	CompileLuaScript( &newBody, script->scriptData, script->dataSize, script->fileName->GetString() );

	// add the script filename and body to the map
	IHashString *tempHash = script->fileName;
	m_ScriptNames[tempHash->GetUniqueID()] = newBody;

	return MSG_HANDLED_PROCEED;
}
DWORD CLuaManager::OnRemoveScript(DWORD size, void *params)
{
	// verify the message we have received is the the right size
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
	SCRIPTBODYMAP::iterator smIter;
	SCRIPTBODYSTRUCT *tempBodyStruct;

	// cast to a IHashString
	IHashString *name = (IHashString *)params;

	smIter = m_ScriptNames.find(name->GetUniqueID());

	// if the name is found
	if (smIter != m_ScriptNames.end())
	{
		// grab the body struct
		tempBodyStruct = &smIter->second;

		// since someone is trying to remove the script
		// decrement ref count
		--tempBodyStruct->refCount;

		// make sure that this script body isn't referenced by anyone else
		// before we try to remove it
		if (tempBodyStruct->refCount <= 1)
		{
			// free the old script
			SAFE_DELETE_ARRAY( tempBodyStruct->scriptBody );
			if ( tempBodyStruct->compiledScript ) {
				delete[] ( unsigned char* ) tempBodyStruct->compiledScript;
				tempBodyStruct->compiledScript = NULL;
			}

			// remove map entry
			m_ScriptNames.erase(smIter);
		}
		else
		{
			// attempt to free a script currently being used!
			return MSG_ERROR;
		}
	}
	else 
	{
		// script not found
		return MSG_NOT_HANDLED;
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CLuaManager::OnLoadAndStartScript(DWORD size, void *params)
{
	OFACTORYADDPARAMS ofap;
	INITOBJECTPARAMS iop;
	StdString str;
	TCHAR strNum[10];
	DWORD check;

	VERIFY_MESSAGE_SIZE(size, sizeof(EXECUTESCRIPTSTRUCT));

	EXECUTESCRIPTSTRUCT *ess = (EXECUTESCRIPTSTRUCT *)params;

	// begin name
	str = _T("Global");

	// append number to each name
	_ltot(m_iGlobalCount, strNum, 10);
	str = str + strNum;
	
	// get name, parent and type
	CHashString name(str.c_str());
	CHashString typeName(_T("CLuaScript"));

	if (ess->bScriptSingleton == true)
	{
		StdString lwrFileName(ess->fileName);
		lwrFileName.MakeSafeFileName();
		CHashString hszFileName(lwrFileName);

		SCRIPTOBJECTMAP::iterator scriptObjIter = m_LuaScripts.find(hszFileName.GetUniqueID());
		if (scriptObjIter != m_LuaScripts.end())
		{
			// this is a global script singleton; we don't need more than one instance here!
			return MSG_HANDLED_STOP;
		}

		// reassign name to filename
		name = hszFileName;
	}

	// increment global count
	++m_iGlobalCount;
	
	// create object (global)
	CLuaScript *pLuaScript = dynamic_cast<CLuaScript *>(
		m_ToolBox->CreateComponent(&typeName, 3, &name, NULL, false));

	// create parameters for object add
	ofap.name = &name;
	ofap.component = pLuaScript;

	// add object to object factory
	static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
	check = m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(OFACTORYADDPARAMS), &ofap);

	if (check == MSG_HANDLED)
	{
		// create archive
		CREATEARCHIVE ca;
		SERIALIZEOBJECTPARAMS sop;
		IArchive *ar;
		CHashString memType(_T("Memory"));
		char * memBuffer;
		DWORD dataSize = 0;

		dataSize += sizeof(TCHAR)*((DWORD)_tcslen(ess->fileName)+1);
		dataSize += sizeof(bool);

		memBuffer = new char[dataSize];
		memset(memBuffer, 0, sizeof(char)*dataSize);

		CHashString currentVersion = _T("1.5");
		CHashString oldVersion = _T("");
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = m_ToolBox->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &oldVersion);
		if (retval != MSG_HANDLED)
		{
			m_ToolBox->Log(LOGERROR, _T("Could not get world version!"));
			return MSG_ERROR;
		}
		static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
		retval = m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &currentVersion);

		ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
		ca.streamData = memBuffer;
		ca.streamSize = sizeof(char)*dataSize;
		ca.streamType = &memType;

		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
        check = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);

		if (check != MSG_HANDLED)
		{
			static DWORD msgHash_SetFileVersion = CHashString(_T("SetFileVersion")).GetUniqueID();
			DWORD retval = m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
			StdString error;
			error = _T(__FILE__ " OnLoadAndStartScript(): cannot create archive\n");
			EngineGetToolBox()->Log( LOGWARNING, error );
			if( memBuffer != NULL )
			{
				delete [] memBuffer;
			}
			memBuffer = NULL;			
			return check;
		}
		ar = ca.archive;

		ar->Write(ess->fileName);
		ar->Write(ess->bAutoStart);

		sop.name = &name;
		sop.archive = ar;
		// serialize object
		DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_SerializeObject,
			                       sizeof(SERIALIZEOBJECTPARAMS), 
								   &sop) == MSG_HANDLED)
		{
			// initialize object
			iop.name = &name;
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop) != MSG_HANDLED)
			{
				StdString error;
				error = _T(__FILE__ _T(" OnLoadAndStartScript(): Init failed\n"));
				EngineGetToolBox()->Log( LOGWARNING, error );
			}

			if (ess->scriptName != NULL)
				*(ess->scriptName) = name.GetString();	
		}
		
		ar->Close();
		if( memBuffer != NULL )
		{
			delete [] memBuffer;
		}
		memBuffer = NULL;

		retval = m_ToolBox->SendMessage(msgHash_SetFileVersion, sizeof(IHashString), &oldVersion);
	}
	
	return check;
}
	

DWORD CLuaManager::OnGetScript(DWORD size, void *params)
{
	LOADFILEEXTPARAMS lfep;

	// verify the message we have received is the the right size
	VERIFY_MESSAGE_SIZE(size, sizeof(SCRIPTPARAMS));

	// cast params to a SCRIPTPARAMS
	SCRIPTPARAMS *script = (SCRIPTPARAMS *)params;
	SCRIPTBODYMAP::iterator smIter;

	// store this ScriptObjectName, for future refcount decrements
	SCRIPTOBJECTMAP::iterator itrScript;
//	assert( "Must supply Script Object name" && script->scriptObjName != NULL );
	IHashString* ObjName = script->scriptObjName;
	if (ObjName)
	{
		itrScript = m_LuaScripts.find( ObjName->GetUniqueID() );
		if ( itrScript != m_LuaScripts.end() )
		{
			itrScript->second.fileName = script->fileName;
		}
		else
		{
			StdString error;
			error = _T( __FILE__ _T(" OnGetScript(): Script request requires pre-existing Script object\n") );
			assert( (const char*)error && 0 );
			EngineGetToolBox()->Log( LOGWARNING, error );
			return MSG_ERROR;
		}
	}

	// find the name in our map
	IHashString *tempHash = script->fileName;

	// make sure the hashstring is valid
	if (tempHash)
	{
		smIter = m_ScriptNames.find(tempHash->GetUniqueID());

		// if the name isn't found
		if (smIter == m_ScriptNames.end())
		{
			IHashString *temp = script->fileName;
			lfep.fileName = (TCHAR*)temp->GetString();
			lfep.bInternalLoad = true;
			DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_LoadFileByExtension, 
									sizeof(LOADFILEEXTPARAMS), 
									&lfep) == MSG_HANDLED)
			{
				// try find the name in our map again
				IHashString *tempHash = script->fileName;
				smIter = m_ScriptNames.find(tempHash->GetUniqueID());

				// if the name is found
				if (smIter != m_ScriptNames.end())
				{
					SCRIPTBODYSTRUCT *newBody = &smIter->second;
					if (newBody->compiledScript && !script->getScriptSource)
					{
						script->scriptData = (char*)newBody->compiledScript;
						script->dataSize = newBody->compiledSize;
					}
					else
					{
						script->scriptData = newBody->scriptBody;
						script->dataSize = newBody->size;
					}
					// TODO:
					// increment reference count??
				}
				else
				{
					script->scriptData = NULL;
					StdString error;
					error = _T( __FILE__ _T(" OnGetScript(): Could not load script\n") );
					EngineGetToolBox()->Log( LOGWARNING, error );
					return MSG_ERROR;
				}
			}
			else
			{
				// log?
				return MSG_NOT_HANDLED;
			}
		}
		else
		{
			SCRIPTBODYSTRUCT *newBody = &smIter->second;
			// increment reference count
			++newBody->refCount;
			if (newBody->compiledScript && !script->getScriptSource)
			{
				script->scriptData = (char*)newBody->compiledScript;
				script->dataSize = newBody->compiledSize;
			}
			else
			{
				script->scriptData = newBody->scriptBody;
				script->dataSize = newBody->size;
			}
		}
	}
	else
	{
		return MSG_ERROR;
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CLuaManager::OnReleaseScript(DWORD size, void *params)
{
	// verify the message we have received is the the right size
	VERIFY_MESSAGE_SIZE(size, sizeof(SCRIPTPARAMS));

	// cast params to a SCRIPTPARAMS
	SCRIPTPARAMS *script = (SCRIPTPARAMS *)params;
	SCRIPTBODYMAP::iterator smIter;
	SCRIPTBODYSTRUCT *newBody;

	// find the name in our map
	IHashString *tempHash = script->fileName;
	smIter = m_ScriptNames.find(tempHash->GetUniqueID());

	// if the name isn't found
	if (smIter == m_ScriptNames.end())
	{
		StdString error;
		error = _T( __FILE__ _T(" OnReleaseScript(): Script not found for 'release'\n") );
		assert( (const char*)error && 0 );
		EngineGetToolBox()->Log( LOGWARNING, error );
		return MSG_ERROR;
	}
	else
	{
		newBody = &smIter->second;
		// decrement reference count
		--newBody->refCount;
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CLuaManager::OnPauseScript(DWORD size, void *params)
{
	// verify the message we have received is the the right size
	VERIFY_MESSAGE_SIZE(size, sizeof(SCRIPTPAUSEPARAMS));

	// cast to a IHashString
	SCRIPTPAUSEPARAMS *spp = (SCRIPTPAUSEPARAMS *)params;

	SCRIPTOBJECTMAP::iterator smIter;

	// find the name in our map
	IHashString *tempHash = spp->scriptObjectName;
	smIter = m_LuaScripts.find(tempHash->GetUniqueID());

	// if the name isn't found
	if (smIter == m_LuaScripts.end())
	{
		StdString error;
		error = _T( __FILE__ _T(" OnPauseScript(): Script not found for 'pause'\n") );
		assert( (const char*)error && 0 );
		EngineGetToolBox()->Log( LOGWARNING, error );

		return MSG_ERROR;
	}
	else
	{
		// otherwise pause/unpause
		//SCRIPTOBJECTSTRUCT script = smIter->second;
		//script.updateFlag = spp->pause;
		smIter->second.updateFlag = !(spp->pause);
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CLuaManager::OnRestartScript(DWORD size, void *params)
{
	// verify the message we have received is the the right size
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));

	// cast to a IHashString
	IHashString *name = (IHashString *)params;

	SCRIPTOBJECTMAP::iterator smIter;

	// find the name in our map
	smIter = m_LuaScripts.find(name->GetUniqueID());

	// if the name isn't found
	if (smIter == m_LuaScripts.end())
	{
		StdString error;
		error = _T( __FILE__ _T(" OnRestartScript(): Could not find script for 'restart'\n") );
		EngineGetToolBox()->Log( LOGWARNING, error );
		return MSG_ERROR;
	}
	else
	{
		SCRIPTOBJECTSTRUCT sos = smIter->second;
		// init the script object
		sos.obj->Init();
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CLuaManager::OnGetMasterScriptState(DWORD size, void *params)
{
	// make sure size is correct
	VERIFY_MESSAGE_SIZE(size, sizeof(lua_State *));

	// create a new state pointer
	lua_State **state = (lua_State **)params;
	
	// get the master state and return it
	*state = m_pMasterState;

	return MSG_HANDLED_PROCEED;
}

void CLuaManager::PrintLuaError(const TCHAR *error )
{
	CLuaScript *execScript;
	execScript = GetCurrentExecutingScript();
	StdString stdError;
	stdError += _T("LUA: ");
	stdError += error;
	if (execScript != NULL)
	{
		m_ToolBox->Log(LOGWARNING, stdError.c_str() );
		execScript->OutputScriptStack();
	}
}

DWORD CLuaManager::OnGetLuaArchive(DWORD size, void *params)
{
    VERIFY_MESSAGE_SIZE(size, sizeof(ILuaArchive**));
    if (m_pLuaArchive==0)
    {
        m_pLuaArchive = new CLuaArchive();
    }
    *static_cast<ILuaArchive**>(params)=m_pLuaArchive;

    return MSG_HANDLED_STOP;
}

DWORD CLuaManager::OnGetScriptStats(DWORD size, void *params)
{
    VERIFY_MESSAGE_SIZE(size, sizeof(SCRIPTSTATS));
	SCRIPTSTATS *pStats = (SCRIPTSTATS*)params;

	if (m_pMasterState)
	{
		pStats->iMemoryUsageKb = lua_gc( m_pMasterState, LUA_GCCOUNT, 0 );
	}
	else
	{
		pStats->iMemoryUsageKb = 0;
	}

    return MSG_HANDLED_STOP;
}

int CLuaManager::LuaPanic(lua_State *scriptState)
{
	SINGLETONINSTANCE(CLuaManager)->DumpDebug(scriptState, _T("Unknown"), true);
	return 0;
}

void CLuaManager::ProcessErrorCode( lua_State *scriptState, const TCHAR *scriptName, int loaderrorcode)
{
	StdString ErrorCodeOutput;
	if (0 != loaderrorcode)
	{
		const TCHAR *czLua					= "LUA: ";
		const TCHAR *czRuntime				= "RUNTIME: ";
		const TCHAR *czRuntimeErrHandler	= "RUNTIME_ERRHANDLER: ";
		const TCHAR *czMemaloc				= "MEMALOC: ";
		const TCHAR *czLoadFile				= "LOAD_FILE: ";
		const TCHAR *czSyntax				= "LOAD_SYNTAX: ";
		const TCHAR *czNoError				= "No Error!\n";
		const TCHAR *czUnknown				= "Unknown Error!\n";
		

		ErrorCodeOutput += czLua;
//		int nErrorType = lua_pcall(scriptState,0,0,0);
		int nErrorType = loaderrorcode;
		switch (nErrorType){
		case LUA_ERRRUN:
			ErrorCodeOutput += czRuntime;
			break;
		case LUA_ERRERR:
			ErrorCodeOutput += czRuntimeErrHandler;
			break;
		case LUA_ERRMEM:
			ErrorCodeOutput += czMemaloc;
			break;
		case LUA_ERRFILE:
			ErrorCodeOutput += czLoadFile;
			break;
		case LUA_ERRSYNTAX:
			ErrorCodeOutput += czSyntax;
			break;
		case 0:
			ErrorCodeOutput += czNoError;
			break;
		default:
			ErrorCodeOutput += czUnknown;
			break;
		}
		int IsString = lua_isstring(scriptState, -1);
		if (0 != nErrorType) 
		{
			const char* msg;
			try 
			{
				 msg = lua_tostring(scriptState, -1);
			}
			catch (...)
			{
				DumpDebug(scriptState, const_cast<TCHAR*>(scriptName));
				return;
			}
			if (msg == NULL)
			{
				msg = "(error with no message)";
			}
			ErrorCodeOutput += msg;
			ErrorCodeOutput += _T("\n");
			m_ToolBox->Log(LOGERROR, _T("%s"), ErrorCodeOutput.c_str());
			DumpDebug(scriptState, const_cast<TCHAR*>(scriptName));
			__debugbreak();
		}
	}
}
void CLuaManager::DumpDebug(lua_State *scriptState, TCHAR *scriptName, bool panic)
{
	DWORD errSeverity;
	lua_Debug activeRecord;
	// fill in dummy values
	activeRecord.currentline = -1;
	_tcscpy(activeRecord.short_src, _T("unknown"));
	// for some reason this doesn't work right??
	errSeverity = panic ? LOGFATALERROR : LOGERROR;
	m_ToolBox->Log(errSeverity, "LUADEBUGSTACK:\n");
	int i = 0;
	while(lua_getstack(scriptState, i, &activeRecord))
	{
		lua_getinfo(scriptState, "Snl", &activeRecord);
		// should add abort, retry, ignore and launch debugger here.
		m_ToolBox->Log(errSeverity, "  STACK_INDEX[%i] %s:%i\n", i, activeRecord.short_src, activeRecord.currentline);
		i++;
	}
	if (i == 0)
		m_ToolBox->Log(errSeverity, "  No Stack\n");
}

/// Get the currently executing script object
CLuaScript *CLuaManager::GetCurrentExecutingScript()
{ 
	return m_CurrentExecuteScript; 
}

/// sets the currently executing script object
CLuaScript *CLuaManager::SetCurrentExecutingScript(CLuaScript *inExecuteScript) 
{
	CLuaScript *oldScript = m_CurrentExecuteScript;
	m_CurrentExecuteScript = inExecuteScript; 
	return oldScript;
}

int CLuaManager::CompiledLuaWriter(lua_State* L, const void* p, size_t size, void* pUserData)
{
	LUAWRITEINFO *pWriteInfo = (LUAWRITEINFO*)pUserData;

	pWriteInfo->iDataWritten += size;
	pWriteInfo->pArchive->Write( p, size );

	return 0; // success
}

bool CLuaManager::CompileLuaScript( SCRIPTBODYSTRUCT *pScriptBody, const char *szScript, int iScriptSize, const TCHAR *szScriptName )
{
	pScriptBody->compiledScript = NULL;
	pScriptBody->compiledSize = 0;

	CHashString memType(_T("Memory"));
	CREATEARCHIVE ca;
	ca.mode =  STREAM_MODE_READ | STREAM_MODE_WRITE;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGWARNING, _T("CLuaManager:CompileLuaScript could not create archive to store data!\n"));
	}

	ca.archive->SetIsWriting( true );

	LUAWRITEINFO writeinfo;
	writeinfo.pArchive = ca.archive;
	writeinfo.iDataWritten = 0;

	int iLastStackCheck = lua_gettop( m_pMasterState );
	lua_lock(m_pMasterState);
	int scripterror = 0;
	try {
		scripterror = luaL_loadbuffer( m_pMasterState, szScript, strlen(szScript), szScriptName );
	}
	catch(...)
	{

	}
	if (0 != scripterror)
	{
		ProcessErrorCode(m_pMasterState, szScriptName, scripterror);
	}
	lua_dump(m_pMasterState, CompiledLuaWriter, &writeinfo);
	lua_pop( m_pMasterState, 1 );
	lua_unlock(m_pMasterState);
	int iNewStackCheck = lua_gettop( m_pMasterState );
	assert( iNewStackCheck == iLastStackCheck );

	int size = writeinfo.iDataWritten;

	ca.archive->SetIsWriting( false );
	ca.archive->SeekTo( 0 );

	pScriptBody->compiledScript = new BYTE[size];
	pScriptBody->compiledSize = size;
	ca.archive->Read( pScriptBody->compiledScript, size );

	ca.archive->Close();

/*
	STARTUPINFO siStartupInfo;
	PROCESS_INFORMATION piProcessInfo;
	memset(&siStartupInfo, 0, sizeof(siStartupInfo));
	memset(&piProcessInfo, 0, sizeof(piProcessInfo));
	siStartupInfo.cb = sizeof(siStartupInfo);

	// Launch the console application
	char cmdLine[1024];
	sprintf( cmdLine, "luac.exe %s -o temp.out", "bleh" );

	if (CreateProcess(0, cmdLine, 0, 0, false,
		CREATE_DEFAULT_ERROR_MODE, 0, 0, &siStartupInfo,
		&piProcessInfo) != false)
	{ 
		DWORD exitCode;
		int secondsToWait = 60 * 4; // 4 minutes to wait
		int count = 0;
		
		GetExitCodeProcess(piProcessInfo.hProcess, &exitCode);

		while (exitCode == STILL_ACTIVE && secondsToWait != 0)
		{
			GetExitCodeProcess(piProcessInfo.hProcess, &exitCode);
			Sleep(500);
			count += 500;

			if (count > (secondsToWait * 1000)) 
			{ 
				exitCode = 0;
			} 
		}
	}
	else
	{
		return false;
	}

	CloseHandle(piProcessInfo.hProcess);
	CloseHandle(piProcessInfo.hThread);

	const char *szOutFile = "temp.out";
	struct _stat outStat;
	memset( &outStat, 0, sizeof(outStat) );
	_stat( szOutFile, &outStat );
	FILE *fp = fopen( szOutFile, "rb" );
	if (fp)
	{
		pScriptBody->compiledSize = outStat.st_size;

		SAFE_DELETE_ARRAY(pScriptBody->compiledScript)
		pScriptBody->compiledScript = new BYTE[pScriptBody->compiledSize];

		pScriptBody->compiledSize = fread( pScriptBody->compiledScript, pScriptBody->compiledSize, 1, fp );
		fclose(fp);
	}
*/
	return (pScriptBody->compiledSize != 0);
}

int CLuaManager::LuaBeep(lua_State *l)
{
	ILuaScript *s;

	GetScriptObject(s, l);
	return s->LuaBeep(l);
}

int CLuaManager::LuaWaitFrame(lua_State *l)
{
	ILuaScript *s;

	GetScriptObject(s, l);
	return s->WaitFrame(l);
}

int CLuaManager::LuaWaitTime(lua_State *l)
{
	ILuaScript *s;

	GetScriptObject(s, l);
	return s->WaitTime(l);
}

int CLuaManager::LuaWaitSec(lua_State *l)
{
	ILuaScript *s;

	GetScriptObject(s, l);
	return s->WaitSec(l);
}

