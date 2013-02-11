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

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

TOLUA_API int tolua_LuaNSM_open (lua_State* tolua_S);

REGISTER_COMPONENT_SINGLETON(CLuaStateHandler);

REGISTER_MESSAGE_HANDLER(RegisterLuaActions, OnRegisterLuaActions, CLuaStateHandler);
REGISTER_MESSAGE_HANDLER(UnregisterLuaActions, OnUnregisterLuaActions, CLuaStateHandler);

CLuaStateHandler::CLuaStateHandler()
{
	m_RegisteredSelf = false;
	// save for convenience
	m_ToolBox = EngineGetToolBox();

	m_LuaState = NULL;

	static DWORD msgHash_GetMasterScriptState = CHashString(_T("GetMasterScriptState")).GetUniqueID();
	if(m_ToolBox->SendMessage(msgHash_GetMasterScriptState, sizeof(lua_State *), &m_LuaState) != MSG_HANDLED)
	{
		StdString error;
		error = _T("Error missing Master Script State Data\n");

		// log error
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
		EngineGetToolBox()->Log(LOGWARNING, error);
    }
	else
	{
		// register our additional structures/handlers with LUA master
		tolua_LuaNSM_open(m_LuaState);
	}
}

CLuaStateHandler::~CLuaStateHandler()
{
	if (m_RegisteredSelf)
	{	
		static DWORD msgHash_ActionSetUnRegister = CHashString(_T("ActionSetUnRegister")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_ActionSetUnRegister, sizeof(IActionHandler*), this);
	}
}

//
// We only have a singleton, so return that
IComponent *CLuaStateHandler::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CLuaStateHandler);
}

// should CLuaStateHandler serialize their data?
void CLuaStateHandler::Serialize(IArchive &ar)
{
}

IHashString *CLuaStateHandler::GetComponentType()
{
	static CHashString CMCT(_T("CLuaStateHandler"));
	return &CMCT;
}

bool CLuaStateHandler::IsKindOf(IHashString *compType)
{
	return ( compType->GetUniqueID() == GetComponentType()->GetUniqueID() );	
}

IHashString* CLuaStateHandler::GetNameSpace()
{
	IHashString *idiot;
	idiot = NULL;
	return idiot;
}

// General Lua Action Function 
void CLuaStateHandler::LuaActionHandler(IArchive *ar, IHashString *name, IHashString *parentName)
{
	//int status;


	// Get Script Object Name
	// IHashString *scriptObjName = GetScriptObjectName();
	// Create a struct with the function name and archive
	// LUACALLSTRUCT lcs;
	// lcs.ar = ar;
	// lcs.name = name;
	// SendMessage(_T("CallFn"), sizeof(LUACALLSTRUCT), &lcs, scriptObjName)
    // *OR*
	// Get Script Object
	// CLuaScript *script;
	// script->CallFn(ar, name)

	PARENTTOACTIONHANDLERSMAP::iterator ptahIter;
	DWORD uniqueID;
	StdString error;
	static CHashString gDummyHash(_T(""));

	if (!parentName)
	{
		// GLOBAL SCRIPTS
		uniqueID = gDummyHash.GetUniqueID();
	}
	else
	{
		// Scripts stored under a particular parent
		uniqueID = parentName->GetUniqueID();
	}

	// see if there is a parent that has had a script register it's action handlers
	ptahIter = m_ParentToActionHandlerMap.find(uniqueID);

	// if we are not at the end, then we have action handlers associated with this parent
	if (ptahIter != m_ParentToActionHandlerMap.end())
	{
		ACTIONHANDLERTOSCRIPTMAP *ahtosm;
		// grab a pointer that we are holding that contains a map of ACTIONHANDLER names to scripts
		ahtosm = &(ptahIter->second);
		ACTIONHANDLERTOSCRIPTMAP::iterator ahtsStartIter;
		ACTIONHANDLERTOSCRIPTMAP::iterator ahtsEndIter;

		// get everybody that handles this ACTIONHANDLER name from multimap
		ahtsStartIter = ahtosm->lower_bound(name->GetUniqueID());
		ahtsEndIter = ahtosm->upper_bound(name->GetUniqueID());
		// check if this parent didn't have any child scripts register this function name
		if (ahtsStartIter == ahtsEndIter)
		{
			// name doesn't exist can't call function!
			error = parentName->GetString();
			error += _T(":");
			error += name->GetString();
			error += _T(" > action handler function call never registered.\n");
			// log error
			EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
			EngineGetToolBox()->Log(LOGWARNING, error);
			return;
		}

		// ok, now iterator through all scripts that have this action handler under this particular parent
		for (;ahtsStartIter != ahtsEndIter; ++ahtsStartIter)
		{
			// get the script object
			ILuaScript *script = ahtsStartIter->second;
			// and finally call it
			script->Call(ar, name);
		}

	}
	else	// if no parent by that name is in our map, then no action handlers were registered inside that script
	{
		// name doesn't exist can't call function!
		error = parentName->GetString();
		error += _T(":");
		error += name->GetString();
		error += _T(" > parent does not exit or is inaccessable, no action handlers were registered.\n");
		// log error
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
		EngineGetToolBox()->Log(LOGWARNING, error);
		return;

	}
}

// Register Lua Action Function List
DWORD CLuaStateHandler::OnRegisterLuaActions(DWORD size, void *params)
{
	DWORD uniqueID;
	IHashString *parentName = NULL;
	ILuaScript *script = NULL;
	ACTIONHANDLERTOSCRIPTMAP *ahtosm;	// Action handler to script object map
	VERIFY_MESSAGE_SIZE(size, sizeof(ACTIONHANDLERLIST));
    if (!m_RegisteredSelf)
    {
		static DWORD msgHash_ActionSetRegister = CHashString(_T("ActionSetRegister")).GetUniqueID();
    	m_ToolBox->SendMessage(msgHash_ActionSetRegister, sizeof(IActionHandler*), this);
        m_RegisteredSelf = true;
    }

	ACTIONHANDLERLIST *ahl = (ACTIONHANDLERLIST *)params;
	// construct strings
	FUNCTIONLIST actionList = ahl->m_FuncList;
	FUNCTIONLIST::iterator flIter;

	// get the script object
	script = ahl->scriptObj;
	if (script == NULL)
	{
		return MSG_HANDLED_PROCEED;
	}

	// if we got a valid script object
	if (script)
	{
		static CHashString gDummyHash(_T(""));

		// get the parent name of the script object
		parentName = script->GetParentName();
		if (!parentName)
		{
			// if there is no parent make a blank one
			uniqueID = gDummyHash.GetUniqueID();
		}
		else
		{
			// otherwise use it to create a unique id
			uniqueID = parentName->GetUniqueID();
		}

		// get the parent's multi-map storage of ACTIONHANDLERS to script objects
		ahtosm = &m_ParentToActionHandlerMap[uniqueID];
	}


	// loop through action handlers and point to callbacks by
	// name and insert map from ACTIONHANDLER name to script object
	for (flIter = actionList.begin(); flIter != actionList.end(); ++ flIter)
	{	
		CHashString tempHash;
		// grab the name
		tempHash = (*flIter);
		// store the callback namp in the map
		m_Callbacks[tempHash.GetUniqueID()] = static_cast<ACTIONCALLBACK>(&CLuaStateHandler::LuaActionHandler);
		// store the script object pointer at the the unique ID of the ACTIONHANDLER name key in the multimap
		ahtosm->insert(pair<DWORD, ILuaScript *>(tempHash.GetUniqueID(), script));
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CLuaStateHandler::OnUnregisterLuaActions(DWORD size, void *params)
{
	IHashString *parentName;
	DWORD uniqueID;
	VERIFY_MESSAGE_SIZE(size, sizeof(IHashString));
	static CHashString gDummyHash(_T(""));

	parentName = (IHashString*)params;
	
	if(!parentName)
	{
		uniqueID = gDummyHash.GetUniqueID();
	}
	else
	{
		uniqueID = parentName->GetUniqueID();
	}
	
	PARENTTOACTIONHANDLERSMAP::iterator ptahmIter = m_ParentToActionHandlerMap.find(uniqueID);
	
	if(ptahmIter != m_ParentToActionHandlerMap.end())
	{
		m_ParentToActionHandlerMap.erase(ptahmIter);
	}

	return MSG_HANDLED_STOP;
}

// report errors
void CLuaStateHandler::ReportError()
{
	const char* msg;
	StdString error;

	msg = lua_tostring(m_LuaState, -1);

	if (msg == NULL)
	{
		msg = "(error with no message)";
	}

	error = _T("Error in Lua Script while executing: ");
	error += msg;

	// log error
	EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
	EngineGetToolBox()->Log(LOGWARNING, error);

	lua_pop(m_LuaState, 1);
}

