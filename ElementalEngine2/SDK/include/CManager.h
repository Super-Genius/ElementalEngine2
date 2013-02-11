///=====================================================================
/// \file	CManagerListener.h
/// \brief	Definition of CManagerListener Interface
///         This class is the concrete class for IListener for managers.
///			it allows the callback to be propogated upwards
/// \date	02/16/2005
/// \author	Kenneth Hurley
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
///=====================================================================

// NOTE!:
// This class should only be inherited from classes that
// need a listener. Otherwise you should inherit from
// IManager.h instead

#ifndef _CMANAGER__H__
#define _CMANAGER__H__

#include "IManager.h"
#include "CHashString.hpp"

namespace ElementalEngine
{

class CManagerListener;

typedef map<DWORD, IDTOOBJECTMAP *> MAPGROUPLISTS;

/// \class CManagerListener
/// \brief	This class defines the base class for listeners. Listener are
//			really callback functions, in this case
class CManager : public IManager
{
protected:
	/// constructor that attaches listener and message pump
	/// \param listenpriority = priority of listener in system, if 0 then NO listener
	/// \param pumpPriority = priority of pump in system, if 0 then NO messagePump
	/// \param compType = component class name for this manager
	CManager(const TCHAR *compType, DWORD listenPriority = 0, DWORD pumpPriority = 0);

private:
	/// Enables the pump handler for this manager
	/// the pump is the function that pumps messages to objects that
	/// we manage
	/// \param priority = the priority of the handler relative to other pump handlers
	void EnablePumpHandler(DWORD priority);

	/// diable the pump handler message flow
	void DisablePumpHandler();

	/// hides Update function when a manager
	/// because Listeners get Update(tickCount)
	void Update() {}

public:

	/// virtual destructor for free functions after base class is destroyed
	virtual ~CManager();

	/// \fn virtual void Init()
	/// \brief Function call back when system is being started up
	virtual bool Init(void);

	/// \fn virtual void Update()
	/// \brief Function call back when system is being updated
	/// \param tickCount = system tick counter
	virtual void Update(DWORD tickCount) {};
	
	/// \fn virtual void DeInit()
	/// \brief Function call back when system is being shutdown
	virtual bool DeInit(void);

	/// Function to add an object to the manager
	/// \param Object is the IObject derived class pointer to add
	/// \return true if success, otherwise false
	virtual bool AddObject( IObject *Object, IHashString *group);

	/// Deletes an Object and removes it from the System
	/// \param Object is the IObject derived class pointer to delete
	/// \return true if success, otherwise false
	virtual bool DeleteObject( IObject* Object );

	/// Get the object map from a component name
	/// \param name = compoenent name of object to get
	/// \return = pointer to STL based list of IObject pointers
	virtual IDTOOBJECTMAP *GetObjectMap(IHashString *name);

	/// Get an object map from a name
	/// \param name = name of object to get
	/// \param component = component of object to get
	/// \return = pointer to STL based list of IObject pointers
	virtual IObject* GetObjectByName(IHashString *name, IHashString *component);

	/// Get the Group List (map is Group name -> NAMEOBJECTMAP)
	/// \return pointer to the map of groups 
	virtual MAPGROUPLISTS *GetGroupList() { return &m_GroupLists; }

	/// serialize the component
	/// \param ar = reference to an archive to serialize with
	virtual void Serialize(IArchive &ar);

	/// Checks if this component type is similar to a base class's componet type
	/// \return true if similar, else false
	virtual bool IsKindOf(IHashString *compType);

	virtual IHashString *GetComponentType() { return &m_ComponentType; }
protected:
	/// \var	IToolBox *m_Toolbox
	/// toolbox pointer that is holding our Singleton class
	IToolBox *m_ToolBox;
	/// \var	ITimer* m_Timer
	/// pointer to the timer
	ITimer* m_Timer;
	/// Pump Hanlder to route messages to objects we manage. 
	/// Called through C++ callback
	/// \param msg = the ID of the message to send
	/// \param size = the size of the data associated with *data
	/// \param data = void pointer to data specific to message
	///	\param name = the name of the object to send the message to,
	///					if NULL, then compType cannot be null and all
	///					components of type compType will be called if they
	///					have message handler registered
	///	\param compType = name of component type NULL - means any type
	/// \return DWORD = message return code, MSG_HANDLED_STOP, MSG_HANDLED_PROCEED, 
	///						error
	virtual DWORD PumpHandler(DWORD size, void *data, 
		IHashString *name, IHashString *compType, MSGFUNCTION msgFunction);

	#if defined(_DEBUG) || defined(_PROFILE)
	void UpdateProfiler( void );
	#endif

private:

	/// \var	IObject	*m_Objects
	/// pointer to list of objects that we hold
	IDTOOBJECTMAP m_Objects;
	/// Grouping of m_Objects by type
	MAPGROUPLISTS m_GroupLists;
	// the default listener
	CManagerListener *m_Listener;
	/// pump function is enabled or not
	bool m_bPumpEnabled;
	/// the priority of the pump handler
	DWORD m_PumpPriority;

	CHashString m_ComponentType;

};

} // namespace ElementalEngine

#endif