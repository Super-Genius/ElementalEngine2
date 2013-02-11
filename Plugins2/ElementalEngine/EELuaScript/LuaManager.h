///============================================================================
/// \file		LuaManager.h
/// \brief		Header file for CLuaManager
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

#ifndef LUAMANAGER_H
#define LUAMANAGER_H

struct SCRIPTOBJECTSTRUCT
{
	/// CLuaScript Object
	IObject *obj;
	/// pause/unpause flag
	bool	updateFlag;
	/// script file name
	CHashString fileName;
}; 

struct SCRIPTBODYSTRUCT
{
	/// body of the lua script
	char *scriptBody;
	/// compiled lua script
	void *compiledScript;
	/// compile lua script size
	int compiledSize;
	/// reference count for particular body
	int refCount;
	/// size of scriptBody
	int size;
};

/// file name, script body info
typedef map<DWORD, SCRIPTBODYSTRUCT> SCRIPTBODYMAP;
/// object name, object info
typedef map<DWORD, SCRIPTOBJECTSTRUCT> SCRIPTOBJECTMAP;

class CLuaScript;

typedef struct lua_State lua_State;
// tolua_begin

/// Permits debug printing from a lua script
/// \param a lua string
void OutputDebug(const char* show);
void OutputDebug(StdString& it);
void OutputDebug(int logLevel, const char* show);
void OutputDebug(int logLevel, StdString& it);

/// Force a break into VC Debugger, from Lua.
void LuaCBreak();

class CLuaManager : public CManager
{
// tolua_end
private:
	SINGLETONCONSTRUCTOROVERRIDE( CLuaManager );
public:
	/// \brief	Default Destructor
	///			Since CLuaManager is implemented as a singleton the
	///			constructor is private.
	CLuaManager();

	virtual ~CLuaManager(void);

	/// Get the object list from a name
	/// \param name = unicode name of object to get
	/// \return = pointer to STL based list of IObject pointers
	virtual OBJECTSET *GetObjectSet(IHashString *name) { return &m_Objects; }

	/// \brief	Function to get this component.
	///			Returns the singleton instance in this case
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief	Update function for the Lua Manager
	/// \param tickCount - current tick count
	virtual void Update(DWORD tickCount);
	
	/// \brief	DeInitializes the Lua Manager
	/// \return	true on sucess, false on failure
	virtual bool DeInit();

	/// \brief Function to add an object to the manager
	/// \param Object is the IObject derived class pointer to add
	/// \return true if success, otherwise false
	virtual bool AddObject( IObject *Object, IHashString *group);

	/// \brief Function to delete an object from the manager
	/// \param Object is the IObject derived
	virtual bool DeleteObject( IObject *Object );

	// ----------- Messages Functions ---------------//

	/// \brief Add a Script to the Manager
	/// \param size - sizeof a SCRIPTPARAMS struct
	/// \param *params - pointer to a SCRIPTPARAMS structure
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnAddScript(DWORD size, void *params);

	/// \brief Remove a Script from the Manager
	/// \param size - sizeof a IHashString
	/// \param *params - pointer to a IHashString
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnRemoveScript(DWORD size, void *params);

	/// \brief Load and Execute a Script
	/// \param size - sizeof a EXECUTESCRIPTSTRUCT struct
	/// \param *params - pointer to a EXECUTESCRIPTSTRUCT struct
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnLoadAndStartScript(DWORD size, void *params);

	/// \brief Execute a Script
	/// \param size - sizeof a STARTSCRIPTSTRUCT struct
	/// \param *params - pointer to a STARTSCRIPTSTRUCT struct
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnStartScript(DWORD size, void *params);

	/// \brief Get text of a Script
	/// \param size - sizeof a SCRIPTPARAMS struct
	/// \param *params - pointer to a SCRIPTPARAMS struct
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnGetScript(DWORD size, void *params);

	/// \brief Release reference to script
	/// \param size - sizeof a SCRIPTPARAMS struct
	/// \param *params - pointer to a SCRIPTPARAMS struct
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnReleaseScript(DWORD size, void *params);

	/// \brief Pause a script
	/// \param size - sizeof a SCRIPTPAUSEPARAMS struct
	/// \param *params - pointer to a SCRIPTPAUSEPARAMS struct
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnPauseScript(DWORD size, void *params);

	/// \brief Restart a Script
	/// \param size - sizeof a IHashString
	/// \param *params - pointer to a IHashString
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnRestartScript(DWORD size, void *params);

	/// \brief Get master Script state
	/// \param size - sizeof a lua_State *
	/// \param *params - pointer to a lua_State pointer
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnGetMasterScriptState(DWORD size, void *params);

    /// Used by "EE to Lua script" callback mechanism for argument wrapping.
    /// \param CREATELUAARCHIVE
    /// \return message return code (e.g. MSG_HANDLED_STOP)
    DWORD OnGetLuaArchive(DWORD size, void *params);

    /// Returns the current statistics of the script system, ie. memory usage
    /// \param SCRIPTSTATS
    /// \return message return code (e.g. MSG_HANDLED_STOP)
	DWORD OnGetScriptStats(DWORD size, void *params);

	// tolua_begin
	/// Get name of script object that is currently running
	/// \param void
	/// \return name of script object current running
	static IHashString* GetScriptObjectName(lua_State *luaState) 
	{ 
		ILuaScript *scriptObj;
		GetScriptObject(scriptObj, luaState);
		return scriptObj->GetName();
	}

	/// Get parent name of script object that is currently running
	/// \param void
	/// \return parent name of script object current running
	static IHashString* GetScriptObjectParentName(lua_State *luaState) 
	{ 
		ILuaScript *scriptObj;
		GetScriptObject(scriptObj, luaState);
		return scriptObj->GetParentName();
	}

	static IHashString* GetScriptObjectPropertyTable(lua_State *luaState) 
	{ 
		ILuaScript *scriptObj;
		GetScriptObject(scriptObj, luaState);
		return scriptObj->GetPropertyTableName();
	}

	// tolua_end

	/// Function to handle Lua panics
	/// \param scriptState - the script state active that caused the panic
	static int LuaPanic(lua_State *scriptState);

	/// Function to handle lua memory allocations
	/// It uses Doug Lea's memory allocation library implemented in 
	/// CPoolAllocater.cpp of the Elemental Enging
	/// \param nsize - number of necessary allocations. if 0, ptr is freed
	static void *Allocator(void *ud, void *ptr, size_t osize, size_t nsize);

	/// Function to dump debug information from lua
	/// \param scriptState - the script state that is active
	/// \param scriptName - the script name that is/was active
	/// \param panic - whether this is a sever error or not
	void DumpDebug(lua_State *scriptState, TCHAR *scriptName, bool panic = false);

	/// Function to print and error message of any type and output lua stack
	/// \param error - TCHAR * of error message to display
	void PrintLuaError(const TCHAR *error );

	void ProcessErrorCode( lua_State *scriptState, const TCHAR *scriptName, int loaderrorcode);

	/// Get the currently executing script object
	CLuaScript *GetCurrentExecutingScript();

	/// sets the currently executing script object
	CLuaScript *SetCurrentExecutingScript(CLuaScript *inExecuteScript);

	static int LuaBeep(lua_State *l);
	static int LuaWaitFrame(lua_State *l);
	static int LuaWaitTime(lua_State *l);
	static int LuaWaitSec(lua_State *l);

private:
	static int CompiledLuaWriter(lua_State* L, const void* p, size_t size, void* pUserData);
	bool CompileLuaScript( SCRIPTBODYSTRUCT *pScriptBody, const char *szScript, int iScriptSize, const TCHAR *szScriptName );

	// TODO: This Manager needs to be updated and inherit from CManager
	// instead of IManager.

	/// \var	IObject	*m_Objects
	/// pointer to list of objects that we hold
	OBJECTSET m_Objects;

	/// Map of lua scripts
	SCRIPTOBJECTMAP m_LuaScripts;

	/// Map of lua script names
	SCRIPTBODYMAP m_ScriptNames;

	/// last time that update was called
	float m_fLastTime;

	/// global script count
	int m_iGlobalCount;

	/// master state for lua
	lua_State	*m_pMasterState;

    /// A singleton referenced by the "EE to lua script" callback mechanism (i.e. LuaNSM)
    CLuaArchive *m_pLuaArchive;

	void *m_CoFunc;

	/// Previous garbage collection count
	int m_iLastGarbageCollectionCount;

	/// currently executing script object
	CLuaScript *m_CurrentExecuteScript;

	struct LUAWRITEINFO {
		IArchive *pArchive;
		int iDataWritten;
	};

// tolua_begin
};
// tolua_end

#endif // #ifndef LuaMANAGER_H
