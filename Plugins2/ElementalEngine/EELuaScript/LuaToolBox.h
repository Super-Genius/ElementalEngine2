///============================================================================
/// \file		LUAToolBox.h
/// \brief		Header file for a LUA Toolbox
/// \date		05-26-2005
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

#ifndef _LUATOOLBOX_H
#define _LUATOOLBOX_H

typedef map<DWORD, CPerformanceProfiler> PERFPROFMAPBYNAME;

class CLuaManager;
// tolua_begin
class CLuaToolBox
{
// tolua_end
private:
	SINGLETONCONSTRUCTOROVERRIDE(CLuaToolBox);
public:
	CLuaToolBox(void);
	virtual ~CLuaToolBox(void);

	/// Register a component in the toolbox, so it can be created
	/// \param componentName = The string name for the class for CreatebyName
	/// \param createFunc = Pointer to the IComponent class create function
	virtual void RegisterComponent(IHashString *ComponentName,
		COMPONENTCREATEFUNC *createFunc, COMPONENTCREATEFUNC *getFunc);

	/// Unregister a component fromt the toolbox/system, DLL Unload, etc.
	/// \param componentName = The string name for the class for CreatebyName
	virtual void UnRegisterComponent(IHashString *ComponentName);

	/// Get a component from the toolbox that was previously registered
	/// \param componentName = name of the component to create 
	virtual IComponent *CreateComponent(IHashString *componentName, int nArgs, ...);

	/// Get a component from the toolbox that was previously registered
	/// This should fail on everything except singletons
	/// \param componentName = name of the component to create 
	virtual IComponent *GetComponent(IHashString *componentName);

// tolua_begin
	/// Send a message to one of the components
	/// \param msg = message ID to send to the component
	/// \param size = size of the data that *data points to
	/// \param data = pointer to the data
	/// \param name = name to send to. If NULL all dynamic components
	/// \param compType = string of component type, NULL means any type
	/// \return DWORD = result, MSG_NOT_HANDLED, etc.
	virtual UINT SendMessage(DWORD msg, DWORD size, void *data, 
		IHashString *name = NULL, IHashString *compType = NULL);

	/// Send a message to one of the components
	/// \param msg = const TCHAR *message name
	/// \param size = size of the data that *data points to
	/// \param data = pointer to the data
	/// \param name = name to send to. If NULL all dynamic components
	/// \param compType = string of component type, NULL means any type
	/// \return DWORD = result, MSG_NOT_HANDLED, etc.
	virtual UINT SendMessage(const TCHAR *msg, DWORD size, void *data, 
		IHashString *name = NULL, IHashString *compType = NULL);

	virtual UINT SendMessage(IHashString *msg, DWORD size, void *data, 
		IHashString *name = NULL, IHashString *compType = NULL);

//	virtual UINT SendMessage(IHashString *msg, DWORD size, DWORD data, 
//		IHashString *name = NULL, IHashString *compType = NULL);

	virtual UINT SendMessageDWORD(IHashString *msg, DWORD data, 
		IHashString *name = NULL, IHashString *compType = NULL);

	virtual UINT SendMessageFloat(IHashString *msg, float data, 
		IHashString *name = NULL, IHashString *compType = NULL);

	/// Send a message to one of the components
	/// \param msg = const TCHAR *message name
	/// \param size = size of the data that *data points to
	/// \param data = pointer to the data
	/// \param name = name to send to. If NULL all dynamic components
	/// \param compType = string of component type, NULL means any type
	/// \return DWORD = result, MSG_NOT_HANDLED, etc.
	virtual UINT SendMessage(const TCHAR *msg, DWORD size, TCHAR *data, 
		IHashString *name = NULL, IHashString *compType = NULL);
// tolua_end

	/// Register a message handler with the system
	/// \param msg - System defined message ID
	/// \param MSGFUNCTION pointer to message handler function
	/// \param msgClass - class name of message handler
	virtual void RegisterMessageHandler(DWORD msg, MSGFUNCTION,
		IHashString *msgClass, DWORD msgPriority);

	/// UnRegister a message handler with the system
	/// \param msg - System defined message ID
	/// \param msgClass - class name of message handler
	virtual void UnRegisterMessageHandler(DWORD msg, 
				IHashString *msgClass);

	/// Get a Message ID from a IHashString 
	/// \param msgName - The textual message name
	/// \return DWORD - MessageID for specific message
	virtual DWORD GetMessageID(IHashString *msgName);

	/// Register a message pump with the system
	/// \param - comp - The base component that gets the pumped messages
	/// \param - pumpFunc, the function to call to pump messages
	/// \param - priority - the priority of the pump function
	virtual void RegisterMessagePump(IComponent *comp, 
			MSGPUMPFUNC pumpFunc, DWORD priority, bool bHierarchical = false, bool bComponentFilter = true);

	/// Unregister a message pump with the system
	/// \param - comp - The base component that gets the pumped messages
	/// \param priority - the priority of the pump
	virtual void UnregisterMessagePump(IComponent *comp, DWORD priority);

	virtual void AddMessagePumpFilter( DWORD MsgPumpComponent, DWORD ComponentType );

	/// get the HashStrings data
	/// \return pointer to HASHSTRING map table
	virtual HASHSTRINGMAP *GetHashStringMap();

	/// Allocates a String for the Hashtable
	/// \param szName - the name
	/// \return Allocated TChar
	virtual TCHAR* AddHashString(const TCHAR* szName, DWORD uniqueID );

	/// get a unique ID for a hash string just added to table
	/// \return DWORD next uniqueID for HashString added to table
	virtual DWORD GetNextUniqueID();

	// tolua_begin

	/// get a const TCHAR * given a UniqueID
	/// \param uniqueID = string table unique ID
	/// \return const TCHAR * of that ID
	virtual const TCHAR *GetHashString(DWORD uniqueID);

	// tolua_end

	/// get a relative path based on a full pathname, uses current
	/// resource directory for base path extraction
	/// \param srcName = pathName (path+ optional filename) of source file
	/// \return relative path name or old name, if relative not there
	virtual const TCHAR *GetRelativePath(const TCHAR *srcName);


	/// Creates a directory at the specified path
	/// \param filepath = path for which the directory will be created.
	/// \return bool = Directory created successfuly(true) or failed(false).
	virtual bool MakeDirectory(const TCHAR *dirpath);

	virtual const TCHAR* GetDrive();

	/// set the directories
	/// \param BaseDirectory = Executable directory
	/// \param ResourceDirectory = Resource directory to load resources from
	virtual void SetDirectories( StdString *BaseDirectory, StdString *ResourceDirectory);

	/// get the directories
	/// BaseDirectory = Executable directory
	/// ResourceDirectory = Resource directory to load resources from
	virtual void GetDirectories( StdString *BaseDirectory, StdString *ResourceDirectory);

	/// Log values to a file and/or assert
	/// \param severity = LOGWARNINGS, LOGERROR
	/// \param fmt = the formatting of the string, i.e. printf like
	/// \param ... = depends of formatting of the string
	virtual void Log(DWORD severity, const TCHAR *fmt, ...);

	/// Set a system wide error value
	/// \param DWORD = value
	virtual void SetErrorValue(DWORD value);

	/// Get the system wide error value
	/// \param DWORD = value
	virtual DWORD GetErrorValue();

	/// Get the system wide error value in string form
	/// \param DWORD = value
	/// \return TCHAR * string of error
	virtual const TCHAR *GetErrorString(DWORD value);

	/// get the instance of us
	// tolua_begin
	static	CLuaToolBox *GetLuaToolBox()
	{
		return SINGLETONINSTANCE(CLuaToolBox);
	}

	void LoadLuaDependency(TCHAR *fileName);

	/// Starts a performance profiler for lua stuffs
	/// if it's already running, does nothing
	/// \param name = name of profiler to start
	void StartLuaPerformanceProfiler(IHashString *name);
	void StartLuaPerformanceProfiler(TCHAR *name)
	{
		CHashString hsName(name);
		StartLuaPerformanceProfiler(&hsName);
	}

	/// Starts a performance profiler for lua stuffs
	/// if it's already running, does nothing
	/// \param name = name of profiler to start
	/// \param type = name of tab to put profiler in
	void StartLuaPerformanceProfiler(IHashString *name, IHashString *type);
	void StartLuaPerformanceProfiler(TCHAR *name, TCHAR *type)
	{
		CHashString hsName(name);
		CHashString hsType(type);
		StartLuaPerformanceProfiler(&hsName, &hsType);
	}

	/// Stops a performance profiler for lua stuffs	
	/// \param name = name of profiler to stop
	void StopLuaPerformanceProfiler(IHashString *name);
	void StopLuaPerformanceProfiler(TCHAR *name)
	{
		CHashString hsName(name);
		StopLuaPerformanceProfiler(&hsName);
	}

	/// Get message profile map
	/// Does nothing here for now
	virtual PROFILERMESSAGECOUNTMAP* GetMessageProfilerCountMap() { return NULL; };

	virtual ITimer* GetTimer() { return NULL; };

	// tolua_end
private:
	IToolBox	*m_EngineToolBox;

	void MessageError(const TCHAR *error, const TCHAR *msg, const TCHAR *name);
	
	/// convenient access to manager functions
	CLuaManager	*m_LuaManager;

	PERFPROFMAPBYNAME m_PerformanceProfilerMap;
// tolua_begin
};
// tolua_end

#endif
