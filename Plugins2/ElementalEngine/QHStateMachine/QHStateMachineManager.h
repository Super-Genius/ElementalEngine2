///============================================================================
/// \file		QHStateMachineManager.h
/// \brief		Declaration of CQHStateMachineManager Class
/// \date		09-22-2006
/// \author		Richard Nguyen
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

#ifndef __QHSTATEMACHINEMANAGER_H__
#define __QHSTATEMACHINEMANAGER_H__

class CQHStateObject;
class CQHStateMachine;

typedef multimap<DWORD, CQHStateObject*> SOUPDATEMAP;
typedef map<DWORD, DWORD> IDTOIDMAP;

class CQHStateMachineManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CQHStateMachineManager );

public:
	// Default Constructor
	CQHStateMachineManager();

	// Default Destructor
	virtual ~CQHStateMachineManager( void );

	/// \brief	Function to get this component.
	///			Returns the singleton instance in this case
	static IComponent *Create(int nArgs, va_list argptr);

	/// Initializes the component and returns true on success,
	/// otherwise false
	virtual bool Init();

	/// Initializes the component and returns true on success,
	/// otherwise false
	virtual bool DeInit();

	/// \fn virtual void Update()
	/// \brief Function call back when system is being updated
	virtual void Update(DWORD tickCount);

	/// \brief Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// \brief return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// \brief Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType) { return false; }

	/// Searches for the first instance of a CQHStateMachine with the requested name
	/// \return the state machine if found, otherwise NULL
	CQHStateMachine *GetStateMachine( IHashString *astateMachineName );

	/// Given the file name, returns the corresponding state machine.
	/// \return the state machine if found, otherwise NULL
	CQHStateMachine *GetStateMachineByFile( IHashString *fileName );

	/// Finds the instance of a CQHState with the requested object name from the manager
	/// \return the state if found, otherwise NULL
	CQHState *GetState( IHashString *astateName );

	/// Finds the instance of a CQHStateMachineEvent with the requested object name from the manager
	/// \return the event if found, otherwise NULL
	CQHStateMachineEvent *GetEvent( IHashString *aeventName );

	/// Generic helper function for getting specific objects from this manager.
	IObject *GetMapObject( IHashString *aobjectName, IDTOOBJECTMAP *aobjectMap );

	/// Load state machine from a specified xms file.
	/// \return true if the state machine has been loaded, otherwise false.
	bool AddStateMachine( IHashString *afileName );

	DWORD OnStart(DWORD size, void *params);	
	DWORD OnStop(DWORD size, void *params);	
	
	/// Handles reparenting all objects under the renamed object.
	DWORD OnChangeObjectName(DWORD size, void *params);	

	/// EE Message: Loads a State Machine, State Object, and Inits it
	/// \return MSG_HANDLED_STOP if ok, lese MSG_ERROR
	DWORD OnLoadAndInitStateObject(DWORD size, void* aeventParams);

	DWORD OnTriggerEventInStateMachine(DWORD size, void* aeventParams);

	/// Queries all the events for the specified state machine.
	DWORD OnQueryStateMachineEvents( DWORD size, void* params );

	/// Gets the name of the state machine by specified file.
	DWORD OnGetStateMachineName( DWORD size, void* params );

	///	removes previous state machine mapping to file and adds new one
	///	\param	stateMachine - pointer to state machine to map
	///	\param	fileName - current file name for state machine
	///	\param	oldFileName - old mapping file name. This value may be NULL
	void MapStateMachineToFileName( IObject *stateMachine, IHashString *fileName, IHashString *oldFileName = NULL );

	/// Adds the state object to the update order based on its priority.
	void AddStateObjectToUpdateQueue( CQHStateObject *stateObject );

	void RemoveStateObjectFromUpdateQueue( CQHStateObject *stateObject );

	/// Deletes an AI Object and removes it to the AI System
	/// \param Object is the IObject derived class pointer to delete
	/// \return true if success, otherwise false
	virtual bool DeleteObject( IObject* Object );

private:
	/// Helper function for constructor
	void RegisterLuaDefinitions( void );

	/// Function to delete a CQHStateObject from the manager
	bool DeleteStateObject( IObject *Object );

	void ResetParentInObjectMap( StdString componentType, IHashString *oldName, IHashString *newName );

	void ResetParentInObjectSet( IHashString *componentType, IHashString *oldName, IHashString *newName );

	CQHStateObject *GetStateObject( IHashString *astateObjectName );

	lua_State *m_LuaState;
	IToolBox *m_ToolBox;
	/// the map of update objects sorted by priority
	SOUPDATEMAP m_StateObjectUpdateMap;
	/// the filename to state machine object map
	IDTOOBJECTMAP m_FilenameToStateMachine;
	IDTOIDMAP m_StateMachineToFileName;

    // cannot be static and has to be here so that we can use it in destructor
	CHashString m_ManagerComponentType;
	CHashString m_stateObjectComponentType;
	CHashString m_stateMachineComponentType;
	CHashString m_stateComponentType;
	CHashString m_derivedStateComponentType;
	CHashString m_eventComponentType;
	CHashString m_actionHandlerComponentType;
	CHashString m_transitionComponentType;

	bool m_bIsRunning;
};
#endif // #ifndef __QHSTATEMACHINEMANAGER_H__
