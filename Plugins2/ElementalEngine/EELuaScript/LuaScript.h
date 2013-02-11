///============================================================================
/// \file		LuaScript.h
/// \brief		Header file for Lua Script
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

#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#ifndef	_COBJECTTEMPLATE_HPP
#include "CObjectTemplate.hpp"
#endif

typedef struct lua_State lua_State;
typedef map<DWORD, int> LUAFUNCTIONS;

typedef enum luaScriptStateTag
{
	LSS_WAITFRAME,
	LSS_WAITTIME,
	LSS_RUNNING,
	LSS_NOTLOADED,
	LSS_DONE
} LUASCRIPTSTATE;

struct LUATHREADINFO
{
	LUASCRIPTSTATE m_RunState;
	float m_fWaitTimestamp;		/// time to wake up
	int	m_iWaitFrame;			/// number of frames to wait
    int m_iLuaRef;              /// key to find luathread values in LUA_REGISTRY
};

typedef map<lua_State *, LUATHREADINFO> THREADINFOMAP;
using namespace std;

#define NUMPREALLOCTHREADS 2
class CLuaScript : public CObjectTemplate<ILuaScript> 
{
private:
	CLuaScript(IHashString *parentName, IHashString *name, bool bAddToHier);

public:
    ~CLuaScript(void);

	/// \brief Create the a instance of IObject
	/// \param void
	/// \returns IComponent*: Pointer to This
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief The Init Function for the ScriptObject
	/// \param void
	/// \return void
	virtual void Init( void );

	/// \brief The DeInit Function for the ScriptObject
	/// \param void
	/// \return void
	virtual void DeInit( void );

	/// override object template override
	virtual void Serialize(IArchive &ar);

	/// Create a Script Thread
	/// \param void
	/// \return void
    void CreateThread(void);

	/// Update
	/// \param elapsedSec - seconds elapsed since last update
	/// \return void
    void Update(float elapsedSec);

	/// Call a function inside the lua script
	/// \param ar - parameters passed to lua
	/// \param name - name of the function to be called
	/// \return void
	virtual void Call(IArchive *ar, IHashString *name);

	/// Hold the script for a specified number of seconds
	/// \param l - lua state
	/// \return int - return value of lua_yield call 
	virtual int WaitSec(lua_State *l);

	/// Hold the script until a time stamp
	/// \param l - lua state
	/// \return int - return value of lua_yield call 
	virtual int WaitTime(lua_State *l);

	/// Hold the script for a specified number of frames
	/// \param l - lua state
	/// \return int - return value of lua_yield call 
	virtual int WaitFrame(lua_State *l);

	/// Do some beeps
	/// \param l - lua state
	/// \return int - always 0
	virtual int LuaBeep(lua_State *l);

	virtual IHashString* GetPropertyTableName() { return &m_szPropertiesTableName; }

	/// RegisterActionHandler
	/// \param name = name of function in ascii
	/// \param func = void pointer to LUA Object
	void RegisterActionHandler(char *name, int funcRef);

	/// SetGlobalValue
	/// \param table = name of table to look for
	/// \param key = char pointer to key in table
	/// \param value = char pointer to value
	void SetTableValue( const char *table, const char *key, const char *value );
	void SetTableStringValue( const char *table, const char *key, const char *value );
	void SetTableBoolValue( const char *table, const char *key, bool value );
	void SetTableIntegerValue( const char *table, const char *key, int value );
	void SetTableNumberValue( const char *table, const char *key, float value );

	/// SetGlobalValue
	/// \param table = name of table to look for
	/// \param key = char pointer to key in table
	/// \param value = char pointer to value
	void SetGlobalValue( const char *key, const char *value );

	// ----------- Messages Functions ---------------//

	/// \brief Start a this script object
	/// \param size - currently size is not important
	/// \param *params - pointer to nothing
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnStartScript(DWORD size, void *params);

	/// \brief Sets a table value in the script object
	/// \param size - currently size is not important
	/// \param *params - pointer to nothing
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnSetTableValue(DWORD size, void *params);

	/// same as OnSetTableValue but assumes the properties table
	DWORD OnSetPropertyTableValue(DWORD size, void *params);

	/// \brief Sets a table value in the script object
	/// \param size - currently size is not important
	/// \param *params - pointer to nothing
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnSetGlobalValue(DWORD size, void *params);

	/// Get the script name for this object
	/// \return StdString pointer to name
	__inline StdString *GetScriptName()
	{
		return &m_ScriptName;
	}

	/// Outputs what it can of current script stack.
	void OutputScriptStack();

private:
	/// Resume Script
	/// \param luaState - the current lua state operating on
	/// \param runState - pointer to LUASCRIPTSTATE to modify
	/// \param param - lua stack parameter
	/// \return bool - false if script thread exited.
	bool ResumeScript(lua_State *luaState, LUASCRIPTSTATE *runState, float param);

	/// Format error message
	/// \param luaState - the lua state with the error
	/// \return void
    void FormatError(lua_State *luaState, int errorcode = 0);

	/// Output Error that occured
	/// \param strType - error type
	/// \return void
    void OutputError(char *strType);

	/// Creates a uniquely named properties table for the lua script
	void CreatePropertiesTable();

	/// current thread information list
	THREADINFOMAP m_ThreadInfos;

	/// last error
    char m_LastErrorString[256];

	// name of script
	StdString m_ScriptName;

	/// body of the lua script
	char *m_ScriptBody;

	/// auto start flag
	bool m_bAutoStart;

	/// hierarchy addition flag
	bool m_bAddToHierarchy;

	// this threads main
	lua_State *m_pThreadState;

	/// current time
    float m_fTime;               

	lua_State *m_MasterScriptState;

	/// the currently running state
	lua_State *m_CurActiveState;

	/// the lua archive for passing parms to state functions
	ILuaArchive* m_pLuaArchive;

	/// a map of names to LUA function
	LUAFUNCTIONS m_LuaFunctions;

	CHashString m_szPropertiesTableName;

	/// convenience pointer to lua manager
	CLuaManager *m_LuaManager;

};

#endif //#ifndef LUASCRIPT_H
