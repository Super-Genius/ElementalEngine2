///============================================================================
/// \file		LuaScript.cpp
/// \brief		Implemenation file for CLuaScript
/// \date		05-25-2005
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

#include "stdafx.h"

#include "ldebug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CLuaScript);

REGISTER_MESSAGE_HANDLER(StartScript, OnStartScript, CLuaScript);
REGISTER_MESSAGE_HANDLER(SetTableValue, OnSetTableValue, CLuaScript);
REGISTER_MESSAGE_HANDLER(SetPropertyTableValue, OnSetPropertyTableValue, CLuaScript);
REGISTER_MESSAGE_HANDLER(SetGlobalValue, OnSetGlobalValue, CLuaScript);
	
static void *m_pByeBye;

CLuaScript::CLuaScript(IHashString *parentName, IHashString *name, bool bAddToHier) : 
	OBJECTTEMPLATE(CLuaManager, CLuaScript, ILuaScript, parentName, name)
{

	LUATHREADINFO *curInfo;
	DWORD i;

	// grab the tool box
	m_ToolBox = EngineGetToolBox();

	m_LuaManager = SINGLETONINSTANCE(CLuaManager);

	// initialize add to hierarchy to true (add it by default)
	m_bAddToHierarchy = bAddToHier;

	if (m_bAddToHierarchy)
	{
		// add it to the hierarchy
		AddToHierarchy();		
	}

	// grab the master script state	
	static DWORD msgHash_GetMasterScriptState = CHashString(_T("GetMasterScriptState")).GetUniqueID();
	if(m_ToolBox->SendMessage(msgHash_GetMasterScriptState, sizeof(lua_State *), &m_MasterScriptState) != MSG_HANDLED)
	{
		StdString error;
		error = _T("Error missing state data\n");
		// log error
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
		EngineGetToolBox()->Log(LOGWARNING, error);
		assert(0);
	}

	//MAIN THREAD
    //Not documented in Lua: a newthread goes automatically onto the global stack,
    //but needs to get moved elsewhere (your choice) for proper management.
    //If it isn't moved elsewhere, then the global stack will keep growing
    //with each new thread created (leads to a crash eventually).  So
    //in conjunction with creating the thread, we're also moving
    //it to the registry for safe-keeping.
	try 
	{
		lua_pushlightuserdata(m_MasterScriptState, this);
		m_pThreadState = lua_newthread(m_MasterScriptState);
		lua_settable(m_MasterScriptState, LUA_REGISTRYINDEX);
	}
	catch (...)
	{
		m_LuaManager->DumpDebug(m_pThreadState, (TCHAR *)m_ScriptName.c_str());
		return;
	}
    
	curInfo = &m_ThreadInfos[m_pThreadState];
	curInfo->m_RunState = LSS_NOTLOADED;
	curInfo->m_iWaitFrame = 0;
	curInfo->m_fWaitTimestamp = 0.0f;
    //The object thread is referenced in the LUA_REGISTRY with "this" pointer.
    //It is only the "sub-threads" that will use Lua-made references for LUA_REGISTRY tracking,
    //So by setting this to LUA_REFNIL, we show this "object" thread is a special case.
    curInfo->m_iLuaRef=LUA_REFNIL;

    // save a pointer to the thread manager object in the global table
    // using the new thread's vm pointer as a key
    lua_pushlightuserdata(m_MasterScriptState, m_pThreadState);
    lua_pushlightuserdata(m_MasterScriptState, this );
    lua_settable(m_MasterScriptState, LUA_GLOBALSINDEX );

	// allocate NUMPREALLOCTHREADS more threads initially
	for (i= 0; i< NUMPREALLOCTHREADS; i++)
	{
		lua_State *coRoutineState;
        //This call *also* places the thread on the stack.
		coRoutineState = lua_newthread(m_pThreadState);

		curInfo = &m_ThreadInfos[coRoutineState];
		curInfo->m_RunState = LSS_NOTLOADED;
		// does this allow locals from the main function to be accessed?
		curInfo->m_iWaitFrame = 0;
		curInfo->m_fWaitTimestamp = 0.0f;
        //This call pops the thread from the stack - puts it in LUA_REGISTRY
        curInfo->m_iLuaRef = luaL_ref(m_pThreadState, LUA_REGISTRYINDEX);
		// save a pointer to the thread manager object in the global table
		// using the new thread's vm pointer as a key
		lua_pushlightuserdata(m_MasterScriptState, coRoutineState);
		lua_pushlightuserdata(m_MasterScriptState, this );
		lua_settable(m_MasterScriptState, LUA_GLOBALSINDEX );
	}

	// initialize the rest of the variables
	m_ScriptName = "";
	m_bAutoStart = false;
	m_ScriptBody = NULL;
	m_fTime = 0.0f;

	static DWORD msgHash_GetLuaArchive = CHashString(_T("GetLuaArchive")).GetUniqueID();
	if(m_ToolBox->SendMessage(msgHash_GetLuaArchive,sizeof(IArchive*),&m_pLuaArchive) != MSG_HANDLED)
    {
        assert("LuaScript could not acquire CLuaArchive wrapper" && 0);
    }
}

CLuaScript::~CLuaScript()
{
#if 0
    THREADINFOMAP::iterator tiIter;
	for (tiIter = m_ThreadInfos.begin(); tiIter != m_ThreadInfos.end(); ++tiIter)
	{
		if (tiIter->first != m_pThreadState)
		{
			lua_closethread(m_pThreadState, tiIter->first);
			//luaE_freethread(m_pThreadState, tiIter->first);
		}
	}

	//luaE_freethread(m_MasterScriptState, m_pThreadState);
	//lua_close(m_pThreadState);
    //lua_pushthread(m_pThreadState);
#endif
	if (m_bAddToHierarchy)
	{
		RemoveFromHierarchy();
	}
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CLuaScript::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
    
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new CLuaScript(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CLuaScript(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		EngineGetToolBox()->Log(LOGINFORMATION, _T("Somebody output NULL!"));
		return NULL;
	}

	return self;
}

void CLuaScript::Init()
{
	int status;
	SCRIPTPARAMS sp;
	CHashString name(m_ScriptName.c_str());

	sp.fileName = &name;
	sp.scriptObjName = GetName();

	if (m_ScriptBody == NULL)
	{
		// get script based on the file name
		DWORD msgHash_GetScript = CHashString(_T("GetScript")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetScript, 
										 sizeof(SCRIPTPARAMS), 
										 &sp) == MSG_HANDLED)
		{
			// extract script body
			m_ScriptBody = sp.scriptData;

			assert(m_pThreadState);
			assert(m_ScriptBody);

			status = luaL_loadbuffer( m_pThreadState, m_ScriptBody, sp.dataSize, name.GetString() );

			if (status)
			{
				// log error!
				FormatError(m_pThreadState, status);
				return;
			}
		}
	}		

	StdString szPropertiesTableName( GetName()->GetString() );
	szPropertiesTableName += _T("_PropertiesTable");
	m_szPropertiesTableName.Init( szPropertiesTableName );

	CreatePropertiesTable();
 
	if (m_bAutoStart && m_ScriptBody != NULL)
	{
		// hack to determine if this script object is deleted after returning from resume
		m_pByeBye = NULL;
		void *temp = this;
		
		CLuaScript *oldScript = m_LuaManager->SetCurrentExecutingScript(this);
		m_CurActiveState = m_pThreadState;
		try 
		{
			status = lua_resume(m_pThreadState, 0);
		}
		catch (...)
		{
			m_LuaManager->SetCurrentExecutingScript(oldScript);
			m_LuaManager->DumpDebug(m_pThreadState, (TCHAR *)m_ScriptName.c_str());
			return;
		}

		m_LuaManager->SetCurrentExecutingScript(oldScript);
		if(m_pByeBye == temp)
		{
			// script object is deleted..return
			m_pByeBye = NULL;
			return;
		}

		//ResumeScript(0.0f);
		
		if (status)
		{
			// log error!
			FormatError(m_pThreadState, status);
			return;
		}
	}
}

DWORD CLuaScript::OnStartScript(DWORD size, void *params)
{
	m_bAutoStart = true;
	Init();

	return MSG_HANDLED_PROCEED;
}

DWORD CLuaScript::OnSetPropertyTableValue(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( SETTABLEVALUEPARAMS ) );
	SETTABLEVALUEPARAMS* stv = (SETTABLEVALUEPARAMS*)params;
	stv->TableName = &m_szPropertiesTableName;
	return OnSetTableValue( size, params );
}

DWORD CLuaScript::OnSetTableValue(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( SETTABLEVALUEPARAMS ) );
	SETTABLEVALUEPARAMS* stv = (SETTABLEVALUEPARAMS*)params;

	static DWORD dwString = CHashString(_T("String")).GetUniqueID();
	static DWORD dwBool = CHashString(_T("Bool")).GetUniqueID();
	static DWORD dwInt = CHashString(_T("Int")).GetUniqueID();
	static DWORD dwFloat = CHashString(_T("Float")).GetUniqueID();
	static DWORD dwTrue = CHashString(_T("true")).GetUniqueID();

	if( stv->TableType->GetUniqueID() == dwString )
	{
		SetTableStringValue( stv->TableName->GetString(), stv->TableKey->GetString(), stv->TableValue->GetString() );
	}
	else if( stv->TableType->GetUniqueID() == dwBool )
	{
		if( stv->TableValue->GetUniqueID() == dwTrue )
			SetTableBoolValue( stv->TableName->GetString(), stv->TableKey->GetString(), true );
		else
			SetTableBoolValue( stv->TableName->GetString(), stv->TableKey->GetString(), false );
    }
	else if( stv->TableType->GetUniqueID() == dwInt )
	{
		SetTableIntegerValue( stv->TableName->GetString(), stv->TableKey->GetString(), (int)atoi(stv->TableValue->GetString()) );
	}
	else if( stv->TableType->GetUniqueID() == dwFloat )
	{
		SetTableNumberValue( stv->TableName->GetString(), stv->TableKey->GetString(), (float)atof(stv->TableValue->GetString()) );
	}
	else
	{
		SetTableValue( stv->TableName->GetString(), stv->TableKey->GetString(), stv->TableValue->GetString() );
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CLuaScript::OnSetGlobalValue(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( SETTABLEVALUEPARAMS ) );
	SETTABLEVALUEPARAMS* stv = (SETTABLEVALUEPARAMS*)params;

	SetGlobalValue( stv->TableKey->GetString(), stv->TableValue->GetString() );

	return MSG_HANDLED_PROCEED;
}

/// \fn void DeInit( void )
/// \brief The DeInit Function for the Object
/// \param void
/// \return void
void CLuaScript::DeInit()
{
	LUAFUNCTIONS::iterator lfIter;
	// name of destructor function
	CHashString destructor(_T("Destructor"));

	// make sure the destructor has been registered with this script
	lfIter = m_LuaFunctions.find(destructor.GetUniqueID());
	// if found
	if (lfIter != m_LuaFunctions.end())
	{
		// call destructor
		Call(NULL, &destructor);
	}

    //release all our sub-threads for garbage collection.
    THREADINFOMAP::iterator tiIter;
	for (tiIter = m_ThreadInfos.begin(); tiIter != m_ThreadInfos.end(); ++tiIter)
	{
        //m_pThreadState is part of this collection being "unref"ed, but
        //unrefing it is a NOP, since its ref is set to LUA_REFNIL
        int r = tiIter->second.m_iLuaRef;
        luaL_unref(m_pThreadState, LUA_REGISTRYINDEX, r);
    }
    

	try 
	{
		//clear mapping from (this) C++ object to its lua object thread
		//(so the thread will be garbage collected)
		lua_pushlightuserdata(m_MasterScriptState, this);  
		lua_pushnil(m_MasterScriptState);
		lua_settable(m_MasterScriptState, LUA_REGISTRYINDEX);
	}
	catch (...)
	{
		m_LuaManager->DumpDebug(m_MasterScriptState, (TCHAR *)m_ScriptName.c_str());
		return;
	}
    
	try 
	{
		//clear mapping from lua object thread to (this) C++ object
		lua_pushlightuserdata(m_MasterScriptState, m_pThreadState);
		lua_pushnil(m_MasterScriptState);
		lua_settable(m_MasterScriptState, LUA_GLOBALSINDEX );
	}
	catch (...)
	{
		m_LuaManager->DumpDebug(m_MasterScriptState, (TCHAR *)m_ScriptName.c_str());
		return;
	}	
	//CObjectTemplate<ILuaScript>::DeInit();

	// freeing all the local function references for this script object
	for (lfIter = m_LuaFunctions.begin(); lfIter != m_LuaFunctions.end(); ++lfIter)
	{
		try 
		{
			luaL_unref(m_MasterScriptState, LUA_REGISTRYINDEX, lfIter->second);
		}
		catch (...)
		{
			m_LuaManager->DumpDebug(m_MasterScriptState, (TCHAR *)m_ScriptName.c_str());
			return;
		}
	}

	// send message to luastate handler to remove this object's action handler list
	static DWORD msgHash_UnregisterLuaActions = CHashString(_T("UnregisterLuaActions")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_UnregisterLuaActions, sizeof(IHashString), GetParentName());

	m_pByeBye = this;
}

void CLuaScript::Serialize(IArchive &ar)
{
	if(ar.IsReading())
	{
		// Script File
		ar.Read(m_ScriptName, _T("Filename"));
		
		// Autostart script
		ar.Read(m_bAutoStart, _T("autostart"));
	}

	else
	{
		// Script File
		ar.Write(m_ScriptName, _T("Filename"));
		
		// Autostart script
		ar.Write(m_bAutoStart, _T("autostart"));
	}

}

///////////////////////////////////////////////////////////////////////////////
//    UPDATE
//    The purpose of update is basically to check if any suspended scripts need
//    to be turned back on again!
//
//    Returns the next script in the list.  This assists in linked list traversal
//    where this script may delete it's next sibling.
///////////////////////////////////////////////////////////////////////////////
void CLuaScript::Update(float elapsedSec)
{
	THREADINFOMAP::iterator tiIter;
	LUATHREADINFO *curInfo;
	lua_State *curLuaState;
	m_fTime += elapsedSec;
    
	for (tiIter = m_ThreadInfos.begin(); tiIter != m_ThreadInfos.end(); ++tiIter)
	{
		curLuaState = tiIter->first;
		curInfo = &tiIter->second;

		switch(curInfo->m_RunState)
		{
		case LSS_WAITTIME:
			if (m_fTime >= curInfo->m_fWaitTimestamp)
			{
				if (!ResumeScript(curLuaState, &curInfo->m_RunState, 0.0f))
				{
					return;
				}
			}
			break;
		case LSS_WAITFRAME:
			curInfo->m_iWaitFrame--;
			if (curInfo->m_iWaitFrame <= 0)
			{
				if (!ResumeScript(curLuaState, &curInfo->m_RunState, 0.0f))
				{
					return;
				}
			}
			break;
		case LSS_NOTLOADED:
			break;
		case LSS_DONE:
		default :
			break;
		}
	}

}

void CLuaScript::Call(IArchive *ar, IHashString *name)
{
	int status;
	LUATHREADINFO *curInfo;
	THREADINFOMAP::iterator tiIter;
	lua_State *newState;
	int funcRef;

    // wrap IArchive into efficient form for lua.
    // each "set" simply overwrites the prior one.
    m_pLuaArchive->SetArchive(ar);

	// maks sure we have the name and parent name stored
	newState = NULL;
	// find empty slot or create one
	for (tiIter = m_ThreadInfos.begin(); tiIter != m_ThreadInfos.end(); ++tiIter)
	{
		curInfo = &tiIter->second;
		if ((curInfo->m_RunState == LSS_NOTLOADED ||
				curInfo->m_RunState == LSS_DONE) && 		
				(tiIter->first != m_pThreadState))
		{
			// found open slot
			newState = tiIter->first;
			break;
		}
	}

	// none found?
	if (newState == NULL)
	{
		// push this pointer so thread won't be set as garbage collectable on m_pThreadState stack
//	    lua_pushlightuserdata(m_MasterScriptState, this);
		//No openings, let's create one.
        //This call *also* places the thread on the stack.
		try 
		{
			newState = lua_newthread(m_pThreadState);
		}
		catch (...)
		{
			m_LuaManager->DumpDebug(m_pThreadState, (TCHAR *)m_ScriptName.c_str());
			return;
		}
		// move the thread from the m_pThreadState to the registry, so we can manually free it
//		lua_settable(m_MasterScriptState, LUA_REGISTRYINDEX);

		curInfo = &m_ThreadInfos[newState];
		curInfo->m_fWaitTimestamp = 0.0f;
		curInfo->m_iWaitFrame = 0;

		try 
		{
			//This call pops the thread from the stack - puts it in LUA_REGISTRY
			curInfo->m_iLuaRef = luaL_ref(m_pThreadState, LUA_REGISTRYINDEX);

			// save a pointer to the thread manager object in the global table
			// using the new thread's vm pointer as a key
			lua_pushlightuserdata(m_MasterScriptState, newState);
			lua_pushlightuserdata(m_MasterScriptState, this );
			lua_settable(m_MasterScriptState, LUA_GLOBALSINDEX );
		}
		catch (...)
		{
			m_LuaManager->DumpDebug(m_MasterScriptState, (TCHAR *)m_ScriptName.c_str());
			return;
		}
	}

	/*
	m_CoRoutineLuaState->ci = m_CoRoutineLuaState->base_ci;  // go back to initial level
	m_CoRoutineLuaState->base = m_CoRoutineLuaState->ci->base;
	m_CoRoutineLuaState->nCcalls = 0;
	luaF_close(m_CoRoutineLuaState, m_CoRoutineLuaState->base);  // close eventual pending closures
	//seterrorobj(m_CoRoutineLuaState, status, m_CoRoutineLuaState->base);
	m_CoRoutineLuaState->stack_last = m_CoRoutineLuaState->stack+m_CoRoutineLuaState->stacksize-1;
	*/

	LUAFUNCTIONS::iterator lfIter = m_LuaFunctions.find(name->GetUniqueID());
	if (lfIter != m_LuaFunctions.end())
	{
		funcRef = lfIter->second;
		try 
		{
			lua_rawgeti(newState, LUA_REGISTRYINDEX, funcRef);
		}
		catch (...)
		{
			m_LuaManager->DumpDebug(newState, (TCHAR *)m_ScriptName.c_str());
			return;
		}
	}
	else
	{
		try 
		{
			// load lua function
			lua_getglobal(newState, name->GetString()); 
		}
		catch (...)
		{
			m_LuaManager->DumpDebug(newState, (TCHAR *)m_ScriptName.c_str());
			return;
		}
	}

	curInfo->m_RunState = LSS_RUNNING;

	try 
	{
		// push args
		tolua_pushusertype(newState, (void*)m_pLuaArchive, "CLuaArchive");
	}
	catch (...)
	{
		m_LuaManager->DumpDebug(newState, (TCHAR *)m_ScriptName.c_str());
		return;
	}

	// hack to determine if this script object is deleted after returning from resume
	m_pByeBye = NULL;
	void *temp = this;

	CLuaScript *oldScript = m_LuaManager->SetCurrentExecutingScript(this);
	m_CurActiveState = newState;
	try
	{
		status = lua_resume(newState, lua_gettop(newState)-1);
	}
	catch (...)
	{
		m_LuaManager->SetCurrentExecutingScript(oldScript);
		m_LuaManager->DumpDebug(newState, (TCHAR *)m_ScriptName.c_str());
		return;
	}

	m_LuaManager->SetCurrentExecutingScript(oldScript);
	if(m_pByeBye == temp)
	{
		// script object is deleted..return
		m_pByeBye = NULL;
		return;
	}

#if LUA_VERSION_NUM >= 501
	if (status && (status != LUA_YIELD))
#else
	if (status)
#endif
	{
		curInfo->m_RunState = LSS_NOTLOADED;
		// log error
		FormatError(newState, status);
	}
	else
	{
		int nres;

		try 
		{
			// get the returned value from the coroutine
			nres = lua_gettop(newState);
		}
		catch (...)
		{
			m_LuaManager->DumpDebug(newState, (TCHAR *)m_ScriptName.c_str());
			return;
		}

		lua_State *parent;

		parent = m_pThreadState;

		if (newState == parent)
		{
			parent = m_MasterScriptState;
		}

		if (!lua_checkstack(parent, nres))
		{
			luaL_error(parent, "too many results to resume");
		}

		try 
		{
			lua_xmove(newState, parent, nres);  /* move yielded values */
		}
		catch (...)
		{
			m_LuaManager->DumpDebug(newState, (TCHAR *)m_ScriptName.c_str());
			return;
		}
  
		// if we didn't go into yield mode kill the thread
		if (curInfo->m_RunState != LSS_WAITTIME && curInfo->m_RunState != LSS_WAITFRAME)
		{
			curInfo->m_RunState = LSS_DONE;
		}
	}

}

int CLuaScript::WaitSec(lua_State *l)
{
	// find state in our map
	LUATHREADINFO *curInfo;
	curInfo = &m_ThreadInfos[l];

    curInfo->m_RunState = LSS_WAITTIME;
	curInfo->m_fWaitTimestamp = m_fTime + (float)luaL_checknumber(l, 1);

	return(lua_yield (l, 0));
}

int CLuaScript::WaitTime(lua_State *l)
{
	// find state in our map
	LUATHREADINFO *curInfo;
	curInfo = &m_ThreadInfos[l];

    curInfo->m_RunState = LSS_WAITTIME;
	curInfo->m_fWaitTimestamp = (float)luaL_checknumber(l, 1);

	return(lua_yield (l, 0));

}

int CLuaScript::WaitFrame(lua_State *l)
{
	// find state in our map
	LUATHREADINFO *curInfo;
	curInfo = &m_ThreadInfos[l];

	curInfo->m_RunState     = LSS_WAITFRAME;
	curInfo->m_iWaitFrame = (int)luaL_checknumber(l, 1);

	return(lua_yield (l, 0));
}

int CLuaScript::LuaBeep(lua_State *l)
{
	return(0);
}

///////////////////////////////////////////////////////////////////////////////
//    RESUME SCRIPT
//    Runs or resumes a Lua script.  Executes until a yield is encountered or
//    the script terminates.
//
//    The parameter to this function is pushed on the stack as the return value
//    of the previous yield.
///////////////////////////////////////////////////////////////////////////////
bool CLuaScript::ResumeScript(lua_State *luaState, LUASCRIPTSTATE *runState, float param)
{
	int status;
	lua_State *mainThreadState = m_pThreadState;

	// we're about to run/resume the thread, so set the global
	*runState = LSS_RUNNING;

	// param is treated as a return value from the function that yielded
	//lua_pushnumber(m_pThreadState, param);

	// hack to determine if this script object is deleted after returning from resume
	m_pByeBye = NULL;
	void *temp = this;

	CLuaScript *oldScript = m_LuaManager->SetCurrentExecutingScript(this);
	m_CurActiveState = luaState;
	try
	{
		status = lua_resume(luaState, 1);
	}
	catch (...)
	{
		m_LuaManager->SetCurrentExecutingScript(oldScript);
		m_LuaManager->DumpDebug(luaState, (TCHAR *)m_ScriptName.c_str());
		return false;
	}

	m_LuaManager->SetCurrentExecutingScript(oldScript);
	if(m_pByeBye == temp)
	{
		// script object is deleted..return
		m_pByeBye = NULL;
		return false;
	}

	// did coroutine yield or complete?
	if (*runState == LSS_RUNNING)
	{
		*runState = LSS_DONE;
	}

	if (status)
	{
		*runState = LSS_NOTLOADED;
		FormatError(luaState, status);
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//    FORMAT ERROR
//    Grabs the last error message and stuffs it in lastErrorString.
///////////////////////////////////////////////////////////////////////////////
void CLuaScript::FormatError(lua_State *luaState, int errorcode)
{
	m_LuaManager->ProcessErrorCode(luaState, (TCHAR *)m_ScriptName.c_str(), errorcode);
	EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
}

void CLuaScript::OutputError(char *strType)
{
    printf("%s %s \n", strType, m_LastErrorString);
}

void CLuaScript::RegisterActionHandler(char *name, int funcRef)
{
	// in case in Unicode, force to wide chars
	StdString convertedName = name;
	CHashString tempHash(convertedName.c_str());
	m_LuaFunctions[tempHash.GetUniqueID()] = funcRef;
}

void CLuaScript::SetTableValue( const char *table, const char *key, const char *value ) 
{ 
	CLuaStackCheck luastackcheck(m_pThreadState);

	lua_getglobal(m_pThreadState, table); 
	lua_pushstring(m_pThreadState, key); 
	lua_pushstring(m_pThreadState, value); 
	lua_rawset(m_pThreadState, - 3); 
	lua_pop(m_pThreadState, 1);
} 

void CLuaScript::SetTableStringValue( const char *table, const char *key, const char *value )
{
	CLuaStackCheck luastackcheck(m_pThreadState);

	lua_getglobal(m_pThreadState, table); 
	lua_pushstring(m_pThreadState, key); 
	lua_pushstring(m_pThreadState, value); 
	lua_rawset(m_pThreadState, - 3); 
	lua_pop(m_pThreadState, 1);
}

void CLuaScript::SetTableBoolValue( const char *table, const char *key, bool value )
{
	CLuaStackCheck luastackcheck(m_pThreadState);

	lua_getglobal(m_pThreadState, table); 
	lua_pushstring(m_pThreadState, key); 
	lua_pushboolean(m_pThreadState, value); 
	lua_rawset(m_pThreadState, - 3); 
	lua_pop(m_pThreadState, 1);
}

void CLuaScript::SetTableIntegerValue( const char *table, const char *key, int value )
{
	CLuaStackCheck luastackcheck(m_pThreadState);

	lua_getglobal(m_pThreadState, table); 
	lua_pushstring(m_pThreadState, key); 
	lua_pushinteger(m_pThreadState, value); 
	lua_rawset(m_pThreadState, - 3); 
	lua_pop(m_pThreadState, 1);
}

void CLuaScript::SetTableNumberValue( const char *table, const char *key, float value )
{
	CLuaStackCheck luastackcheck(m_pThreadState);

	lua_getglobal(m_pThreadState, table); 
	lua_pushstring(m_pThreadState, key); 
	lua_pushnumber(m_pThreadState, value); 
	lua_rawset(m_pThreadState, - 3); 
	lua_pop(m_pThreadState, 1);
}

void CLuaScript::SetGlobalValue( const char *key, const char *value ) 
{ 
	CLuaStackCheck luastackcheck(m_pThreadState);

	lua_pushstring(m_pThreadState, value);
	lua_setglobal( m_pThreadState, key );
} 

void CLuaScript::CreatePropertiesTable()
{
	if (m_pThreadState)
	{
		lua_newtable(m_pThreadState);
		lua_setglobal(m_pThreadState, (const char*)StdString(m_szPropertiesTableName.GetString()));
	}
}

void CLuaScript::OutputScriptStack()
{
	lua_Debug ar;

	// This table is correct when started at 1, not 0. This is odd, but I think it has to do with
	// This getstack loop getting called from the script level not the master state level.
	m_ToolBox->Log(LOGWARNING, " LUADEBUGSTACK:\n");
	int i = 1;
	while (lua_getstack(m_CurActiveState, i, &ar))
	{
		lua_getinfo(m_CurActiveState, "Snl", &ar);
		m_ToolBox->Log(LOGWARNING, _T("  %s:%i\n"), ar.source, ar.currentline);
		i++;
	}
	if (i == 0)
		m_ToolBox->Log(LOGWARNING, "  No Stack\n");
}
