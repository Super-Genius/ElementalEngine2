///============================================================================
/// \file		CToolBox.h
/// \brief		Header file for tool box interface 
/// \date		2-16-2005
/// \author		Kenneth Hurley
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

#ifndef CTOOLBOX_H
#define CTOOLBOX_H

#include "PerformanceProfiler.h"

#include <string.h>

#define MSG_ID_FIRST				0xABADCAFE
#define	HASHSTRING_ID_FIRST			0xD00FC0DE

typedef struct MSGPUMP
{
	IComponent	*m_Comp;			/// the component that handles the pump
	MSGPUMPFUNC m_MsgPumpFunc;		/// pointer to the function
	bool m_bHierarchical;			/// flag that handles message hierarchically, for efficiency
	bool m_bComponentFilter;		/// indicates if particular pump handler only handles messages to certain component types
} MSGPUMP;

typedef struct MSGHANDLER
{
	MSGFUNCTION m_MsgFunc;				/// pointer to the handler
	IHashString *m_ClassName;		/// class name for the handler
} MSGHANDLER;

typedef map <DWORD, COMPONENTCREATEFUNC *> COMPONENTMAP;
typedef multimap<DWORD, MSGPUMP> MSGPUMPMAP;			// key is message pump priority
typedef multimap<DWORD, MSGHANDLER> MSGPRIORITYMAP;		// msg priority in list is key value
typedef map<DWORD, MSGPRIORITYMAP *> MSGIDHANDLERMAP;	// msg value to a list of message handlers
typedef map<DWORD, DWORD> MSGNAMEMAP;
typedef pair<DWORD, MSGHANDLER> MHPAIR;
typedef map<DWORD, const TCHAR *> UNIQUEIDTOSTRMAP;
typedef set< DWORD > MSGPUMPSET;
typedef map< DWORD, MSGPUMPSET > COMPTYPETOMSGPUMPMAP;

//typedef map< DWORD, int > PROFILERMESSAGECOUNTMAP;

/// \class CToolBox
/// \brief Coolbox interface for system
///
/// The Toolbox class is holder for singleton systems and is the only
///	access point for all components in the system.  You should only
/// derive off of this class and make it a Singleton
class CToolBox : public IToolBox
{
	SINGLETONCONSTRUCTOROVERRIDE(CToolBox);
	CToolBox();
	~CToolBox();
public:
	/// Register a component in the toolbox, so it can be created
	/// \param componentName = The string name for the class for CreatebyName
	/// \param createFunc = Pointer to the IComponent class create function
	/// \param GETFunc = Pointer to the IComponent class GetInstance function
	void RegisterComponent(IHashString *componentName,
		COMPONENTCREATEFUNC *createFunc, COMPONENTCREATEFUNC *GETFunc);

	/// Unregister a component fromt the toolbox/system, DLL Unload, etc.
	/// \param componentName = The string name for the class for CreatebyName
	virtual void UnRegisterComponent(IHashString *ComponentName);

	/// Create a component from the toolbox that was previously registered
	///	for singletons, this grabs the instance
	/// \param componentName = name of the component to create 
	IComponent *CreateComponent(IHashString *componentName, int nArgs, ...);

	/// Get a component from the toolbox that was previously registered
	///	for singletons, this grabs the instance, for others it returns NULL
	/// \param componentName = name of the component to create 
	IComponent *GetComponent(IHashString *componentName);

	// send a message with uniqueID
	DWORD SendMessage(DWORD msg, DWORD size, void *data, 
		IHashString *name = NULL, IHashString *compType = NULL);

	// send a messgage with _T("Message")
//	DWORD SendMessage(const TCHAR *msg, DWORD size, void *data, 
//		IHashString *name = NULL, IHashString *compType = NULL);

	// Register a message handler in the toolbox
	void RegisterMessageHandler(DWORD msg, MSGFUNCTION,
		IHashString *msgClass, DWORD msgPriority);

	/// UnRegister a message handler with the system
	/// \param msg - System defined message ID
	/// \param msgClass - class name of message handler
	void UnRegisterMessageHandler(DWORD msg, IHashString *msgClass);

	// Get a unique message ID from a IHashString
	DWORD GetMessageID(IHashString *msgName);

	/// Register a message pump with the toolbox
	void RegisterMessagePump(IComponent *comp, MSGPUMPFUNC pumpFunc, 
		DWORD priority, bool bHierarchical = false, bool bComponentFilter = true);

	/// Unregister a message pump with the toolbox
	void UnregisterMessagePump(IComponent *comp, DWORD priority);

	/// Adds a ComponentType to a Message Pump Filter
	void AddMessagePumpFilter( DWORD MsgPumpComponent, DWORD ComponentType );

	/// get the HashStrings data
	/// \return pointer to HASHSTRING map table
	HASHSTRINGMAP *GetHashStringMap();

	/// get a unique ID for a hash string just added to table
	/// \return DWORD next uniqueID for HashString added to table
	DWORD GetNextUniqueID();

	/// Adds the Hashstring to the data
    TCHAR* AddHashString(const TCHAR* szName, DWORD uniqueID );

	/// get a const TCHAR * given a UniqueID
	/// \param uniqueID = string table unique ID
	/// \return const TCHAR * of that ID
	const TCHAR *GetHashString(DWORD uniqueID);

	/// get a relative path based on a full pathname, uses current
	/// resource directory for base path extraction
	/// \param srcName = pathName (path+ optional filename) of source file
	/// \return relative path name or old name, if relative not there
	const TCHAR *GetRelativePath(const TCHAR *srcName);
/*
	/// get a filename based on a full pathname
	/// \param filepath = pathname from which filename is to be retrieved.
	/// \return filename = buffer to which the filename will be stored. Define as "TCHAR buffer[_MAX_PATH];"
	void GetFilenameFromPath(TCHAR * filename, const TCHAR * filepath);
*/
	/// Creates a directory at the specified path
	/// \param filepath = path for which the directory will be created.
	/// \return bool = Directory created successfuly(true) or failed(false).
	bool MakeDirectory(const TCHAR *dirpath);

	/// Gets the current working drive
	const TCHAR* GetDrive();

	/// Sets the working directories
	void SetDirectories(StdString *baseDir, StdString *resourceDir);

	/// Gets the working directories
	void GetDirectories(StdString *baseDir, StdString *resourceDir);

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

	/// Get message profile map
	virtual PROFILERMESSAGECOUNTMAP* GetMessageProfilerCountMap();

	virtual ITimer* GetTimer();

private:
	COMPONENTMAP m_ComponentsCreate;				/// System components map, create
	COMPONENTMAP m_ComponentsGet;					/// System components map, get
	MSGPUMPMAP m_MsgPumps;							/// Message Pump functions
	MSGIDHANDLERMAP m_MsgHandlers;					/// Message Handler function storage
	MSGNAMEMAP m_MsgIDs;							/// current list of Message IDs assigned
	DWORD m_CurMsgID;								/// current message ID allocated
	HASHSTRINGMAP m_HashStringMap;					/// return the hash string map
	DWORD m_CurHashStringUnique;					/// current unique ID for hash string
	UNIQUEIDTOSTRMAP m_UniqueIDToString;			/// mapping from unique ID to string map
	StdString	m_ExecutableDirectory;				/// the executable directory
	StdString	m_ResourceDirectory;				/// where the base directory for resources are
	StdString	m_Drive;							/// specially specified drive letter(s)
	DWORD m_GlobalErrorValue;						/// global error code
	COMPTYPETOMSGPUMPMAP m_CompTypeToMsgPumpMap;	/// map of Component Types to MsgPumps

	
	PROFILERMESSAGECOUNTMAP m_MessageProfileMap;	/// Profiler map of messages

	ITimer* m_Timer;								/// pointer to timer interface
};

/// Get the global toolbox that houses singletons and interfaces
IToolBox *EngineGetToolBox();

#endif // ITOOLBOX_H
