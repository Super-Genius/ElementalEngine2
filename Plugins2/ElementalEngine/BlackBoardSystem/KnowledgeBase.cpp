///============================================================================
/// \file		KnowledgeBase.cpp
/// \brief		Implemenation file for KnowledgeBase
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

REGISTER_COMPONENT(CLuaScript);

REGISTER_MESSAGE_HANDLER(StartScript, OnStartScript, CLuaScript);

CLuaScript::CLuaScript(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE(CLuaManager, CLuaScript, IObject, parentName, name)
{
	m_EngineToolBox = EngineGetToolBox();

	lua_State *temp;
	static DWORD msgHash_GetMasterScriptState = CHashString(_T("GetMasterScriptState")).GetUniqueID();
	if(m_EngineToolBox->SendMessage(msgHash_GetMasterScriptState, sizeof(lua_State *), &temp) != MSG_HANDLED)
	{
		StdString error;
		error = _T("Error missing state data\n");
		// log error
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
		EngineGetToolBox()->Log(LOGWARNING, error);
		assert(0);
	}
	
	// create a thread/state for this object
    m_pThreadState = lua_newthread(temp);
    
    // save a pointer to the thread manager object in the global table
    // using the new thread's vm pointer as a key
    lua_pushlightuserdata(temp, m_pThreadState);
    lua_pushlightuserdata(temp, this );
    lua_settable(temp, LUA_GLOBALSINDEX );

	AddToHierarchy();

	m_bAutoStart = false;
	m_ScriptBody = NULL;
}

CLuaScript::~CLuaScript()
{
	RemoveFromHierarchy();
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CLuaScript::Create(va_list argptr)
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
		self = new CLuaScript(parentName, name);
	}
	catch(...)
	{
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

	if (m_ScriptBody == NULL)
	{
		// get script based on the file name
		static DWORD msgHash_GetScript = CHashString(_T("GetScript")).GetUniqueID();
		if (m_EngineToolBox->SendMessage(msgHash_GetScript, sizeof(SCRIPTPARAMS), &sp) == MSG_HANDLED)
		{
			// extract script body
			m_ScriptBody = sp.scriptData;

			assert(m_pThreadState);
			assert(m_ScriptBody);

			status = luaL_loadbuffer(m_pThreadState, m_ScriptBody, strlen(m_ScriptBody), "Console");

			if (status)
			{
				// log error!
				FormatError();
				return;
			}
		}
	}		
	if (m_bAutoStart && m_ScriptBody != NULL)
	{
		SINGLETONINSTANCE(CLuaManager)->SetScriptObjectName(GetName());
		SINGLETONINSTANCE(CLuaManager)->SetScriptObjectParentName(GetParentName());
		status = lua_pcall(m_pThreadState, lua_gettop(m_pThreadState)-1, 0, 0);
		SINGLETONINSTANCE(CLuaManager)->SetScriptObjectName(NULL);
		SINGLETONINSTANCE(CLuaManager)->SetScriptObjectParentName(NULL);
		
		if (status)
		{
			// log error!
			FormatError();
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

/// \fn void DeInit( void )
/// \brief The DeInit Function for the Object
/// \param void
/// \return void
void CLuaScript::DeInit()
{
	CObjectTemplate<IObject>::DeInit();
}

void CLuaScript::Serialize(IArchive &ar)
{
	if(ar.IsReading())
	{
		ar.Read(m_ScriptName);
		ar.Read(m_bAutoStart);
	}
	else
	{
		ar.Write(m_ScriptName);
		ar.Write(m_bAutoStart);
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
    m_fTime += elapsedSec;

	SINGLETONINSTANCE(CLuaManager)->SetScriptObjectName(GetName());
	SINGLETONINSTANCE(CLuaManager)->SetScriptObjectParentName(GetParentName());
    
    switch(m_State)
	{
	case LSS_WAITTIME:
		if (m_fTime >= m_fWaitTimestamp)
		{
			ResumeScript(0.0f);
		}
        break;
	case LSS_WAITFRAME:
		m_iWaitFrame--;
		if (m_iWaitFrame <= 0)
		{
			ResumeScript(0.0f);
		}
        break;
	case LSS_NOTLOADED:
		break;
//  case LSS_DONE:
    default :
		break;
	}

	SINGLETONINSTANCE(CLuaManager)->SetScriptObjectName(NULL);
	SINGLETONINSTANCE(CLuaManager)->SetScriptObjectParentName(NULL);
}

///////////////////////////////////////////////////////////////////////////////
//    RESUME SCRIPT
//    Runs or resumes a Lua script.  Executes until a yield is encountered or
//    the script terminates.
//
//    The parameter to this function is pushed on the stack as the return value
//    of the previous yield.
///////////////////////////////////////////////////////////////////////////////
void CLuaScript::ResumeScript(float param)
{
	int status;

    // we're about to run/resume the thread, so set the global
    m_State  = LSS_RUNNING;

    // param is treated as a return value from the function that yielded
    lua_pushnumber(m_pThreadState, param);

    status = lua_resume(m_pThreadState, 1);

    if (status)
	{
        FormatError();
        OutputError("Runtime Error:");
	}
}

///////////////////////////////////////////////////////////////////////////////
//    FORMAT ERROR
//    Grabs the last error message and stuffs it in lastErrorString.
///////////////////////////////////////////////////////////////////////////////
void CLuaScript::FormatError()
{
    const char* msg;
	StdString error;

    msg = lua_tostring(m_pThreadState, -1);

    if (msg == NULL)
	{
        msg = "(error with no message)";
	}

	error = _T("Error in Lua Script while excuting: ");
	error += msg;

	// log error
	EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
	EngineGetToolBox()->Log(LOGWARNING, error);

    lua_pop(m_pThreadState, 1);

    strcpy(m_LastErrorString, msg);
}

void CLuaScript::OutputError(char *strType)
{
    printf("%s %s \n", strType, m_LastErrorString);
}

/*
// Exported function 
TOLUA_API int tolua_EELuaScript_open (lua_State* tolua_S);

CLuaScript::CLuaScript(void)
{
	lua_State* L = lua_open();
	luaopen_base(L);
	luaopen_io(L);
	luaopen_string(L);
	luaopen_table(L);
	luaopen_math(L);
	luaopen_debug(L);

	tolua_EELuaScript_open(L);
}

CLuaScript::~CLuaScript(void)
{
}
*/

/*
CLuaScript::CLuaScript(lua_State *state)
{
	m_pManager = mgr;
    m_State = LSS_NOTLOADED;
    m_fTime = 0;
    strcpy(lastErrorString, "No error.\n");

    // create a thread/state for this object
    threadState = lua_newthread(m_pManager->m_pMasterState);
    
    // save a pointer to the thread manager object in the global table
    // using the new thread's vm pointer as a key
    lua_pushlightuserdata(m_pManager->m_pMasterState, m_pThreadState);
    lua_pushlightuserdata(m_pManager->m_pMasterState, this );
    lua_settable(m_pManager->m_pMasterState, LUA_GLOBALSINDEX );
}


CLuaScript::~CLuaScript()
{
    // Unhook from the script manager
//    m_pManager->UnlinkScript(this);
}







///////////////////////////////////////////////////////////////////////////////
//    UPDATE
//    The purpose of update is basically to check if any suspended scripts need
//    to be turned back on again!
//
//    Returns the next script in the list.  This assists in linked list traversal
//    where this script may delete it's next sibling.
///////////////////////////////////////////////////////////////////////////////
CLuaScript* CLuaScript::Update(float elapsedSec)
{
    m_fTime += elapsedSec;

    switch(m_State)
	{
	case LSS_WAITTIME:
		if (m_fTime >= m_fWaitTimestamp)
		{
			ResumeScript(0.0f);
		}
        break;
	case LSS_WAITFRAME:
		m_iWaitFrame--;
		if (m_iWaitFrame <= 0)
		{
			ResumeScript(0.0f);
		}
        break;
	case LSS_NOTLOADED:
		break;
//  case LSS_DONE:
    default :
            // if we aren't waiting and we get here, the script must be done
//            if (autoDelete)
//                {
//                LUASCRIPT*  sNext;
//
//                sNext = next;
//                delete(this);
//                return(sNext);
//                }
		break;
	}

    return(next);
}


///////////////////////////////////////////////////////////////////////////////
//    ABORT WAIT
//    Resumes esecution of the script, pushing 1 as a return value from the
//    call that suspended the script.
///////////////////////////////////////////////////////////////////////////////
void CLuaScript::AbortWait()
{
    ResumeScript(1.0f);
}

///////////////////////////////////////////////////////////////////////////////
//    CALL FN
//    Calls a Lua function.  Shows how to pass a single parameter, but not how
//    to retrieve return values.
///////////////////////////////////////////////////////////////////////////////
void CLuaScript::CallFn(char *fnName, int iParam)
{
    int status;

    // find the lua function and push it on the stack
    lua_getglobal (m_pThreadState, fnName);

    // push our single argument
    lua_pushnumber(m_pThreadState, iParam);

    // now, call into Lua
    status = lua_pcall(m_pThreadState, 1, 0, 0);

    if (status)
	{
        FormatError();
        OutputError("Runtime Error:");
	}
}



*/