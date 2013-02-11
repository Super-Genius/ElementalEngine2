///============================================================================
/// \file		CLuaNSM.h
/// \brief		Header file for Lua NSM Proxy
/// \date		06-29-2005
/// \author		Josh Lulewicz
///============================================================================

///============================================================================

#ifndef CLUANSM_H
#define CLUANSM_H

// map of names to ILuaScript Objects
typedef multimap<DWORD, ILuaScript *> ACTIONHANDLERTOSCRIPTMAP;
typedef map<DWORD, ACTIONHANDLERTOSCRIPTMAP> PARENTTOACTIONHANDLERSMAP;
typedef int lua_Function;

class CLuaStateHandler : public IActionHandler
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CLuaStateHandler);
	CLuaStateHandler();
public:
	~CLuaStateHandler();

	/// Get action handlers.
	/// \param void
	/// \return - Map of action names to call back member functions
	virtual ACTIONCALLBACKMAP *GetActionHandlers() { return &m_Callbacks; } 

	/// Get the namespace that protects actions from name clashes.  Compliant Lua scripts specify
    /// this name when loaded, whereas C++ actions supply this name merely by using the
    /// name of the derived class (derived from IActionHandler).
    /// \param void
    /// \return 
	virtual IHashString* GetNameSpace();

	/// Function to get this component.  This is a singleton
	/// in this case
	static IComponent *Create(int nArgs, va_list argptr);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	/// General lua action callback function
	/// \param parms - data for lua action function
	/// \param name - name of lua action function to call
	/// \return void
	void LuaActionHandler(IArchive *parms, IHashString *name, IHashString *parentName);

	// ------------------ Messages ------------------ //
	/// Register a list of lua action functions
	/// \param size - size of LUAACTIONLIST
	/// \param params - pointer to a LUAACTIONLIST
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnRegisterLuaActions(DWORD size, void *params);

	/// Remove the action handler 
	///	\param	size - size of IHashString
	///	\param	params - ptr to IHashString
	///	\return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnUnregisterLuaActions(DWORD size, void *params);


private:
	/// Report any errors resulting from the lua function call
	/// \param void
	/// \return void
	void ReportError();

	ACTIONCALLBACKMAP m_Callbacks;
	PARENTTOACTIONHANDLERSMAP m_ParentToActionHandlerMap;
	IToolBox *m_ToolBox;
	lua_State *m_LuaState;
    bool m_RegisteredSelf;
};


// list of function names
typedef list<char *> FUNCTIONLIST;

// tolua_begin
struct ACTIONHANDLERLIST
{
// tolua_end
	FUNCTIONLIST m_FuncList;		// the function list
	ILuaScript *scriptObj;
// tolua_begin
	void add(lua_State *luaState, char *funcName, lua_Function funcIdx)
	{
		int funcRef;
		lua_pushvalue(luaState, funcIdx);
		funcRef = luaL_ref(luaState, LUA_REGISTRYINDEX);
		m_FuncList.push_back(funcName);
		scriptObj->RegisterActionHandler(funcName, funcRef);
	}

	ACTIONHANDLERLIST(lua_State* l)
	{
		GetScriptObject(scriptObj, l);
	}
};
// tolua_end

#endif	// #ifndef CLUANSM_H
