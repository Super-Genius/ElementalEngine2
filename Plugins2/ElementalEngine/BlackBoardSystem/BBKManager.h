///============================================================================
/// \file		BBKManager.h
/// \brief		Header file for CBBKManager
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

#ifndef _BBKMANAGER_H
#define _BBKMANAGER_H


typedef struct SCRIPTOBJECTSTRUCT
{
	/// CLuaScript Object
	IObject *obj;
	/// pause/unpause flag
	bool	updateFlag;
} SCRIPTOBJECTSTRUCT; 

typedef struct SCRIPTBODYSTUCT
{
	/// body of the lua script
	char *scriptBody;
	/// reference count for particular body
	int refCount;
} SCRIPTBODYSTRUCT;

/// file name, script body info
typedef map<DWORD, SCRIPTBODYSTRUCT> SCRIPTBODYMAP;
/// object name, object info
typedef map<DWORD, SCRIPTOBJECTSTRUCT> SCRIPTOBJECTMAP;

class CLuaListener;

typedef struct lua_State lua_State;
// tolua_begin
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
	static IComponent *Create(va_list argptr);

	/// \brief	Initializes the Lua Manager
	/// \return	true on sucess, false on failure
	virtual bool Init();

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

	/// \brief Get output from lua script
	/// \param size - sizeof a IHashstring
	/// \param *params - pointer to a IHashstring
	/// \return DWORD = message return code, MSG_HANDLED_PROCEED, 
	///					MSG_ERROR
	DWORD OnOutputDebug(DWORD size, void *params);

	/// Set script object's name
	/// \param name - name of script object
	/// \return void
	void SetScriptObjectName(IHashString *name) { m_ScriptObjectName = name; }

	/// Set script object's parent name
	/// \param name - name of parent
	/// \return void
	void SetScriptObjectParentName(IHashString *name) { m_ScriptObjectParentName = name; }

	// tolua_begin
	/// Get name of script object that is currently running
	/// \param void
	/// \return name of script object current running
	static IHashString* GetScriptObjectName() { return SINGLETONINSTANCE(CLuaManager)->m_ScriptObjectName; }

	/// Get parent name of script object that is currently running
	/// \param void
	/// \return parent name of script object current running
	static IHashString* GetScriptObjectParentName() { return SINGLETONINSTANCE(CLuaManager)->m_ScriptObjectParentName; }
	// tolua_end

private:
	/// \var	IObject	*m_Objects
	/// pointer to list of objects that we hold
	OBJECTSET m_Objects;

	/// Map of lua scripts
	SCRIPTOBJECTMAP m_LuaScripts;

	// Map of lua script names
	SCRIPTBODYMAP m_ScriptNames;

	/// last time that update was called
	float m_fLastTime;

	/// listener pointer
	CLuaListener *m_Listener;

	/// global script count
	int m_iGlobalCount;

	/// master state for lua
	lua_State	*m_pMasterState;

	/// parent name of the script object that is currently running
	IHashString *m_ScriptObjectParentName;
	/// name of the script object that is currently running
	IHashString *m_ScriptObjectName;
// tolua_begin
};
// tolua_end

#endif // #ifndef _BBKMANAGER_H