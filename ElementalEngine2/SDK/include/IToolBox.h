///============================================================================
/// \file		IToolBox.h
/// \brief		Header file for tool box interface 
/// \date		2-16-2005
/// \author		Kenneth Hurley
/// \note		Copyright (c) 2004 Signature Devices, Inc.
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

#ifndef ITOOLBOX_H
#define ITOOLBOX_H


#include <hash_map>

using namespace std;
#ifdef _MSC_VER
using namespace stdext;
#endif

namespace std
{
	/// less operator function for TCHAR *
	template<>
	struct less<const TCHAR *> : public binary_function <const TCHAR *, const TCHAR *, bool> 
	{
		bool operator()(const TCHAR *_Left, const TCHAR *_Right) const
		{
			return (_tcscmp(_Left, _Right) < 0);
		}
	};
}

namespace ElementalEngine
{

class IListenerFactory;
class IManager;
class IObject;
class ITimer;

/// MSGFUNCTION is message handler function typedef
typedef DWORD (IComponent::*MSGFUNCTION)(DWORD msgSize, void *msgData);

/// MSGPUMP is a message pump function
typedef DWORD (IComponent::*MSGPUMPFUNC)(DWORD , void *, 
		IHashString *, IHashString *, MSGFUNCTION);

#ifdef _XHASH_
/// HashString map 
typedef hash_map<const TCHAR *, DWORD, hash_compare<const TCHAR *,
	less<const TCHAR *> > > HASHSTRINGMAP;
#else
typedef hash_map<const TCHAR *, DWORD, hash<const TCHAR*>, less<const TCHAR*> > HASHSTRINGMAP;
#endif


typedef map< DWORD, int > PROFILERMESSAGECOUNTMAP;

/// \interface IToolBox
/// \brief Toolbox interface for system
///
/// The Toolbox class is holder for singleton systems and is the only
///	access point for all components in the system.  You should only
/// derive off of this class and make it a Singleton
class IToolBox 
{
public:

	virtual ~IToolBox() {};
	/// Register a component in the toolbox, so it can be created
	/// \param componentName = The string name for the class for CreatebyName
	/// \param createFunc = Pointer to the IComponent class create function
	/// \param getFunc - The Get function for Singletons, or NULL, if dynamic object
	virtual void RegisterComponent(IHashString *ComponentName,
		COMPONENTCREATEFUNC *createFunc, COMPONENTCREATEFUNC *getFunc) = 0;

	/// Unregister a component fromt the toolbox/system, DLL Unload, etc.
	/// \param ComponentName = The string name for the class for CreatebyName
	virtual void UnRegisterComponent(IHashString *ComponentName) = 0;

	/// Get a component from the toolbox that was previously registered
	/// \param componentName = name of the component to create 
	virtual IComponent *CreateComponent(IHashString *componentName, int nArgs, ...) = 0;

	/// Get a component from the toolbox that was previously registered
	/// This should fail on everything except singletons
	/// \param componentName = name of the component to create 
	virtual IComponent *GetComponent(IHashString *componentName) = 0;

	/// Send a message to one of the components
	/// \param msg = message ID to send to the component
	/// \param size = size of the data that *data points to
	/// \param data = pointer to the data
	/// \param name = name to send to. If NULL all dynamic components
	/// \param compType = string of component type, NULL means any type
	/// \return DWORD = result, MSG_NOT_HANDLED, etc.
	virtual DWORD SendMessage(DWORD msg, DWORD size, void *data, 
		IHashString *name = NULL, IHashString *compType = NULL) = 0;

	/// Send a message to one of the components
	/// \param msg = const TCHAR *message name
	/// \param size = size of the data that *data points to
	/// \param data = pointer to the data
	/// \param name = name to send to. If NULL all dynamic components
	/// \param compType = string of component type, NULL means any type
	/// \return DWORD = result, MSG_NOT_HANDLED, etc.
	//virtual DWORD SendMessage(const TCHAR *msg, DWORD size, void *data, 
	//	IHashString *name = NULL, IHashString *compType = NULL) = 0;

	/// Register a message handler with the system
	/// \param msg - System defined message ID
	/// \param mFunc MSGFUNCTION pointer to message handler function
	/// \param msgClass - class name of message handler
	virtual void RegisterMessageHandler(DWORD msg, MSGFUNCTION mFunc,
		IHashString *msgClass, DWORD msgPriority) = 0;

	/// UnRegister a message handler with the system
	/// \param msg - System defined message ID
	/// \param msgClass - class name of message handler
	virtual void UnRegisterMessageHandler(DWORD msg, 
				IHashString *msgClass) = 0;

	/// Get a Message ID from a IHashString 
	/// \param msgName - The textual message name
	/// \return DWORD - MessageID for specific message
	virtual DWORD GetMessageID(IHashString *msgName) = 0;

	/// Register a message pump with the system
	/// \param comp - The base component that gets the pumped messages
	/// \param pumpFunc, the function to call to pump messages
	/// \param priority - the priority of the pump function
	/// \param hierarchical - handles message hierarchicially, which is
	///				less efficient than straight to component.  If you
	///				send a message without component type, it is less
	///				effecient and will generate non-terminating errors.
	virtual void RegisterMessagePump(IComponent *comp, MSGPUMPFUNC pumpFunc, 
		DWORD priority, bool bHierarchical = false, bool bComponentFilter = true) = 0;

	/// Unregister a message pump with the system
	/// \param comp = component to unregister
	/// \param priority - the priority of the pump
	virtual void UnregisterMessagePump(IComponent *comp, DWORD priority) = 0;

	/// Adds a ComponentType to a Message Pump Filter
	/// \param MsgPumpComponent = component that has msg pump
	/// \param ComponentType = component type to filter
	virtual void AddMessagePumpFilter( DWORD MsgPumpComponent, DWORD ComponentType ) = 0;

	/// get the HashStrings data
	/// \return pointer to HASHSTRING map table
	virtual HASHSTRINGMAP *GetHashStringMap() = 0;

	/// Allocates a String for the Hashtable
	/// \param szName - the name
	/// \param uniqueID = the unique ID of the hashString
	/// \return Allocated TChar
	virtual TCHAR* AddHashString(const TCHAR* szName, DWORD uniqueID) = 0;

	/// get a unique ID for a hash string just added to table
	/// \return DWORD next uniqueID for HashString added to table
	virtual DWORD GetNextUniqueID() = 0;

	/// get a const TCHAR * given a UniqueID
	/// \param uniqueID = string table unique ID
	/// \return const TCHAR * of that ID
	virtual const TCHAR *GetHashString(DWORD uniqueID) = 0;

	/// get a relative path based on a full pathname, uses current
	/// resource directory for base path extraction
	/// \param srcName = pathName (path+ optional filename) of source file
	/// \return relative path name or old name, if relative not there
	virtual const TCHAR *GetRelativePath(const TCHAR *srcName) = 0;
/*
	/// get a filename based on a full pathname
	/// \param filepath = pathname from which filename is to be retrieved.
	/// \return filename = buffer to which the filename will be stored. Define as "TCHAR buffer[_MAX_PATH];"
	virtual void GetFilenameFromPath(TCHAR * filename, const TCHAR * filepath) = 0;
*/
	/// Creates a directory at the specified path
	/// \param filepath = path for which the directory will be created.
	/// \return bool = Directory created successfuly(true) or failed(false).
	virtual bool MakeDirectory(const TCHAR *dirpath) = 0;

	/// Gets the current working drive
	virtual const TCHAR* GetDrive() = 0;

	/// set the directories
	/// \param BaseDirectory = Executable directory
	/// \param ResourceDirectory = Resource directory to load resources from
	virtual void SetDirectories( StdString *BaseDirectory, StdString *ResourceDirectory) = 0;

	/// get the directories
	/// BaseDirectory = Executable directory
	/// ResourceDirectory = Resource directory to load resources from
	virtual void GetDirectories( StdString *BaseDirectory, StdString *ResourceDirectory) = 0;

	/// Log values to a file and/or assert
	/// \param severity = LOGWARNINGS, LOGERROR
	/// \param fmt = the formatting of the string, i.e. printf like
	/// \param ... = depends of formatting of the string
	virtual void Log(DWORD severity, const TCHAR *fmt, ...) = 0;

	/// Set a system wide error value
	/// \param value - the error # to set
	virtual void SetErrorValue(DWORD value) = 0;

	/// Get the system wide error value
	/// \return DWORD the error # that was previously set
	virtual DWORD GetErrorValue() = 0;

	/// Get the system wide error value in string form
	/// \param value - the error # to get the string for
	/// \return TCHAR * string of error
	virtual const TCHAR *GetErrorString(DWORD value) = 0;

	/// Get message profile map
	virtual PROFILERMESSAGECOUNTMAP* GetMessageProfilerCountMap() = 0;

	virtual ITimer* GetTimer() = 0;
};

#ifndef _LIB
#ifdef EE_EXPORTING
   #define ELEMENTAL_DECLSPEC __declspec(dllexport)
#else
   #define ELEMENTAL_DECLSPEC __declspec(dllimport)
#endif
#endif

/// Get the global toolbox that houses singletons and interfaces
#ifdef _LIB
IToolBox *EngineGetToolBox();
#else
ELEMENTAL_DECLSPEC IToolBox *EngineGetToolBox();
#endif

}	//namespace ElementalEngine

#endif // ITOOLBOX_H
