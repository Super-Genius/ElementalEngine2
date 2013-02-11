///============================================================================
/// \file		QHStateObject.h
/// \brief		Declaration of CQHStateObject Object Class
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

#ifndef __QHSTATEOBJECT_H__
#define __QHSTATEOBJECT_H__

#include "stdafx.h"

struct EVENTQUEUEOBJECT
{
	CHashString EventName;
	IArchive	*ArchiveParams;
};

typedef list<EVENTQUEUEOBJECT> EVENTQUEUEOBJECTLIST;

/// An instantiable class which acts as an adapter between
///	a state machine and its corresponding objects. This class
/// will keep track of an object's current and will be responsible
/// for correctly transitioning the object between states.
class CQHStateObject : public CObjectTemplate<IObject>
{
public:
	// Default constructor
	CQHStateObject( IHashString *parentName, IHashString *name, bool bAddToHier );

	// Default destructor
	~CQHStateObject();

	/// \fn		virtual void Init()
	/// \brief	Initialize the object
	void Init();
 
	/// \fn		virtual void Update()
	/// \brief	Update the object
	bool Update();

	/// \fn		virtual void DeInit()
	/// \brief	Deinitialize the object
	void DeInit();

	static IComponent *Create(int nArgs, va_list argptr);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	IHashString *GetCurrentStateName();

		/// Triggers the event if found in the current state
	DWORD TriggerEvent( IHashString *aeventName, IArchive *aarchive );

	DWORD OnTriggerEvent(DWORD size, void* aeventParams);

	/// Triggers event and returns a MSG_HANDLED_PROCEED if successful to
	/// allow the message to be passed to all child entities
	DWORD OnTriggerRecursiveEvent(DWORD size, void* aeventParams);

	DWORD OnGetCurrentState( DWORD size, void *params );

	DWORD OnQueryStateMachineEvents( DWORD size, void* aeventParams );

	/// Message handler for ExecuteCommand. Will execute script action handlers accordingly
	DWORD OnExecuteCommand( DWORD size, void* aeventParams );

	DWORD OnGetSchema( DWORD size, void *params );

	/// Returns true if the state object has reached the ending state of the state machine
	bool HasExpired();

	int GetUpdatePriority();

private:
	/// Helper method for TriggerEvent to handle transitions.
	void HandleTransition( CQHStateMachineEvent *aevent );

	/// Helper method for TriggerEvent to execute queued events.
	void IterateEventQueue();

	/// Transitions the current state of the object accordingly
	/// to the event. Return true if successful, otherwise false.
	bool TransitionState( CQHStateMachineEvent *aevent );

	/// Triggers entry event for the current state
	void TriggerEntryEvent( IHashString *acommonParent );

	/// Triggers entry event for the given state
	void TriggerEntryEvent( CQHState *astate, IHashString *acommonParent );

	/// Triggers exit event for the current state
	void TriggerExitEvent( IHashString *acommonParent );

	/// Triggers exit event for the given state
	void TriggerExitEvent( CQHState *astate, IHashString *acommonParent );

	/// Finds the first common parent of the current state with the specified state.
	IHashString *GetCommonParent( CQHState *astate );

	CQHStateMachineManager *GetStateMachineManager( void );

	CHashString				m_szStateMachine;
	CHashString				m_szFileName;
	CQHStateMachine			*m_StateMachine;
	CQHState				*m_CurrentState;
	bool					m_bAddToHierarchy;
	int						m_iUpdatePriority;

	EVENTQUEUEOBJECTLIST	m_EventQueue;
};

#endif //#ifndef __QHSTATEOBJECT_H__