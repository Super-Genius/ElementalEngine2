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

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

// Exported function 
TOLUA_API int tolua_EELuaScript_open (lua_State* tolua_S);

REGISTER_COMPONENT_SINGLETON(CLuaManager);

REGISTER_MESSAGE_HANDLER(AddScript, OnAddScript, CLuaManager);
REGISTER_MESSAGE_HANDLER(RemoveScript, OnRemoveScript, CLuaManager);
REGISTER_MESSAGE_HANDLER(LoadAndStartScript, OnLoadAndStartScript, CLuaManager);
REGISTER_MESSAGE_HANDLER(GetScript, OnGetScript, CLuaManager);
REGISTER_MESSAGE_HANDLER(PauseScript, OnPauseScript, CLuaManager);
REGISTER_MESSAGE_HANDLER(RestartScript, OnRestartScript, CLuaManager);
REGISTER_MESSAGE_HANDLER(GetMasterScriptState, OnGetMasterScriptState, CLuaManager);

// testing only
REGISTER_MESSAGE_HANDLER(OutputDebug, OnOutputDebug, CLuaManager);

CLuaManager::CLuaManager(void) : 
	CManager(_T("CLuaManager"), LUAPRIORITY)
{
	m_pMasterState = lua_open();

    if (m_pMasterState)
	{
        // open any auxiliary libaraies we may need
        lua_baselibopen (m_pMasterState); 
      //lua_iolibopen   (m_pMasterState); 
      //lua_strlibopen  (m_pMasterState); 
        lua_mathlibopen (m_pMasterState); 
		tolua_EELuaScript_open(m_pMasterState);
		lua_tablibopen(m_pMasterState);
    }

	m_fLastTime = ((float)GetTickCount()/1000.0f);
	m_iGlobalCount = 0;
}

CLuaManager::~CLuaManager(void)
{
	DeInit();

	if (m_pMasterState)
	{
		lua_close( m_pMasterState );
	}

	if (m_Listener)
	{
		delete m_Listener;
	}
}

IComponent* CLuaManager::Create(va_list argptr)
{
	// Return the singleton instance of this class
	return SINGLETONINSTANCE( CLuaManager );
}

//
//	adds an Object to the Manager
//
bool CLuaManager::AddObject( IObject *Object, IHashString *group)
{
	SCRIPTOBJECTSTRUCT newScript;

	newScript.obj = Object;
	newScript.updateFlag = true;

	// add object to map
	IHashString *tempHash = Object->GetName();
	m_LuaScripts[tempHash->GetUniqueID()] = newScript;

	// add to object list
	m_Objects.insert(Object);

	return true;
}

//
// Deletes a Lua Object and removes it to the Lua System
//
bool CLuaManager::DeleteObject( IObject* Object )
{
	OBJECTLIST::iterator olIter;

	SCRIPTOBJECTMAP::iterator smIter;

	// find the object in our map
	IHashString *tempHash = Object->GetName();
	smIter = m_LuaScripts.find(tempHash->GetUniqueID());

	// if the object isn't found
	if (smIter == m_LuaScripts.end())
	{
		// log not found error
		return false;
	}
	else
	{
		// if found remove it
		m_LuaScripts.erase(smIter);
	}

	// add to object list
	m_Objects.erase(Object);

	return true;
}

void CLuaManager::Update(DWORD tickCount)
{
	// get global elapsed time
	float deltaTime = (float)(tickCount) / 1000.0f;

	for(SCRIPTOBJECTMAP::iterator itor = m_LuaScripts.begin(); itor != m_LuaScripts.end(); ++itor)
	{
		SCRIPTOBJECTSTRUCT script = itor->second;
		
		if (script.updateFlag)
		{
			CLuaScript *luascript;
			luascript = dynamic_cast<CLuaScript *>(script.obj);
			luascript->Update(deltaTime);
		}
	}
}

bool CLuaManager::DeInit()
{
	// script body map iterator
	SCRIPTBODYMAP::iterator sbmIter;
	// script object map iterator
	SCRIPTOBJECTMAP::iterator somIter;

	// free all scripts
	for (sbmIter = m_ScriptNames.begin(); sbmIter != m_ScriptNames.end(); ++sbmIter)
	{
		// free script bodies (KEN - is this ok -> then . ??)
		GLOBALFREE(sbmIter->second.scriptBody);
	}

	// start at the head
	somIter = m_LuaScripts.begin();
	while (somIter != m_LuaScripts.end())
	{
		// delete objects
		if (somIter->second.obj)
		{
			delete somIter->second.obj;
		}
		// get new position
		somIter = m_LuaScripts.begin();
	}

	m_ScriptNames.clear();
	m_LuaScripts.clear();

	return CManager::DeInit();
}

/// \brief AddScript message handler
/// This message is usually sent by the LuaLoader
DWORD CLuaManager::OnAddScript(DWORD size, void *params)
{
	// make sure the size is correct
	VERIFY_MESSAGE_SIZE(size, sizeof(SCRIPTPARAMS));

	// extract the SCRIPTPARAMS from the void pointer
	SCRIPTPARAMS *script = (SCRIPTPARAMS *)params;

	SCRIPTBODYSTUCT newBody;

	// create a script body structure
	// giving it the data in script
	newBody.scriptBody = script->scriptData;
	// and a reference count
	newBody.refCount = 1;

	// add the script filename and body to the map
	IHashString *tempHash = script->fileName;
	m_ScriptNames[tempHash->GetUniqueID()] = newBody;

	return MSG_HANDLED_PROCEED;
}

DWORD CLuaManager::OnRemoveScript(DWORD size, void *params)
{
	// verify the message we have received is the the right size
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));

	// cast to a IHashString
	IHashString *name = (IHashString *)params;
	DELETEOBJECTPARAMS dop;

	dop.name = name;

	DWORD check;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	check = m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop, NULL, NULL);

	// decrement reference count here?

	return check;
}

DWORD CLuaManager::OnLoadAndStartScript(DWORD size, void *params)
{
	CREATEOBJECTPARAMS cop;
	INITOBJECTPARAMS iop;
	StdString str;
	TCHAR strNum[50];
	DWORD check;

	VERIFY_MESSAGE_SIZE(size, sizeof(EXECUTESCRIPTSTRUCT));

	EXECUTESCRIPTSTRUCT *ess = (EXECUTESCRIPTSTRUCT *)params;

	// begin name
	str = _T("Global");

	// append number to each name
	_ltot(m_iGlobalCount, strNum, 10);
	str = str + strNum;
	
	// increment global count
	++m_iGlobalCount;

	// get name, parent and type
	CHashString name(str.c_str());
	CHashString typeName(_T("CLuaScript"));

	// create parameters for object creation
	cop.name = &name;
	cop.typeName = &typeName;
	cop.parentName = NULL;
	
	// create object
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	check = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

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

		ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
		ca.streamData = memBuffer;
		ca.streamSize = sizeof(char)*dataSize;
		ca.streamType = &memType;

		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		check = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);

		if (check != MSG_HANDLED)
		{
			// log error
			return check;
		}
		ar = ca.archive;

		ar->Write(ess->fileName);
		ar->Write(ess->bAutoStart);

		sop.name = &name;
		sop.archive = ar;
		// serialize object
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop) == MSG_HANDLED)
		{
			ar->Close();

			// initialize object
			iop.name = &name;
			static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop) != MSG_HANDLED)
			{
				// log error
			}

			ess->scriptName = &name;
		}
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

	// find the name in our map
	IHashString *tempHash = script->fileName;
	smIter = m_ScriptNames.find(tempHash->GetUniqueID());

	// if the name isn't found
	if (smIter == m_ScriptNames.end())
	{
		IHashString *temp = script->fileName;
		lfep.fileName = (TCHAR*)temp->GetString();
		lfep.bInternalLoad = true;
		static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep) == MSG_HANDLED)
		{
			// try find the name in our map again
			IHashString *tempHash = script->fileName;
			smIter = m_ScriptNames.find(tempHash->GetUniqueID());

			// if the name is found
			if (smIter != m_ScriptNames.end())
			{
				SCRIPTBODYSTUCT *newBody = &smIter->second;
				script->scriptData = newBody->scriptBody;
				// TODO:
				// increment reference count??
			}
			else
			{
				script->scriptData = NULL;
				// log
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
		SCRIPTBODYSTUCT *newBody = &smIter->second;
		// increment reference count
		++newBody->refCount;
		script->scriptData = newBody->scriptBody;
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CLuaManager::OnPauseScript(DWORD size, void *params)
{
	// verify the message we have received is the the right size
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));

	// cast to a IHashString
	SCRIPTPAUSEPARAMS *spp = (SCRIPTPAUSEPARAMS *)params;

	SCRIPTOBJECTMAP::iterator smIter;

	// find the name in our map
	IHashString *tempHash = spp->fileName;
	smIter = m_LuaScripts.find(tempHash->GetUniqueID());

	// if the name isn't found
	if (smIter == m_LuaScripts.end())
	{
		// log
		return MSG_ERROR;
	}
	else
	{
		// otherwise pause/unpause
		SCRIPTOBJECTSTRUCT script = smIter->second;
		script.updateFlag = spp->pause;
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
		// log
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

/// testing only, this is for testing the system
DWORD CLuaManager::OnOutputDebug(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
//	TCHAR *msg = (TCHAR *)params;
	IHashString *msg = (IHashString *)params;
	TCHAR outStr[1024];

	sprintf(outStr, _T("Lua Says: %s\n"), msg->GetString());
	OutputDebugString(outStr);

	return MSG_HANDLED_PROCEED;
}