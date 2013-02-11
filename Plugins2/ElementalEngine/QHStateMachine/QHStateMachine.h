///============================================================================
/// \file		QHState.h
/// \brief		Declaration of CQHStateMachine Class
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

#ifndef __QHSTATEMACHINE_H__
#define __QHSTATEMACHINE_H__

#include "stdafx.h"

class CQHStateMachine : public CObjectTemplate<IObject>
{
	struct CParameters
	{
		CParameters()
			: m_StartStateName( _T("") )
			, m_EndStateName( _T("") )
			, m_ParentStateMachineFile( _T("") )
			, m_ParentStateMachine( _T("") )
		{
		}

		CHashString	m_StartStateName;
		CHashString	m_EndStateName;
		CHashString	m_ParentStateMachineFile;
		CHashString	m_ParentStateMachine;
	};

public:
	/// Default Constructor
	CQHStateMachine( IHashString *parentName, IHashString *name );

	/// Default Destructor
	~CQHStateMachine();

	/// \fn		virtual void Init()
	/// \brief	Initialize the object
	void Init();

	/// \fn		virtual void DeInit()
	/// \brief	Deinitialize the object
	void DeInit();

	static IComponent *Create(int nArgs, va_list argptr);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// Message handler for MapToFileName. Tells the manager to map this object to the file name
	DWORD OnMapToFileName( DWORD size, void *params );

	DWORD OnGetSchema( DWORD size, void *params );

	/// Visitor for hierarchically serializing the state machines and sub entities.
	DWORD OnVisitHierarchy( DWORD size, void* params );

	/// message handler for GetStateMachineStateLabels. provide list of state names
	DWORD OnGetStateMachineStateLabels( DWORD size, void* params );

	/// message handler for GetChildStateNames. provide list of base state names
	DWORD OnGetChildStateNames( DWORD size, void* params );

	CQHStateMachine *GetParentStateMachine();

	/// Adds a non-child state to the state machine
	bool AddBaseState( CQHState *astate );

	/// Index the state for the entire state machine to see
	bool AddGlobalState( CQHState *astate );

	/// Removes the state from the index of sub states.
	void RemoveBaseState( CQHState *astate );

	/// Removes the state from the index of states belonging to the entire state machine.
	void RemoveGlobalState( CQHState *astate );

	CQHState *GetStartState();

	CQHState *GetEndState();

	IHashString *GetFileName();

	bool EventExists( IHashString *aeventName );

	/// Finds the instance of a CQHState with the requested label from the state machine
	/// \return the state if found, otherwise NULL
	CQHState *GetBaseStateByLabel( IHashString *astateLabel );

	CQHState *GetGlobalStateByLabel( IHashString *astateLabel );

	void QueryEvents( QUERYEVENTSLIST *queryEventsList );

	void VisitNode( IVisitor *visitor );

	///	\brief	extract labels from all state machine states
	///	\param	[out]labels - reference to container with labels to fill
	void FillStateLabels( list<IHashString *> &labels );

private:
	///	\brief	extract labels from specific states container
	///	\param	[in]states - reference to map with labels to fill
	///	\param	[out]labels - reference to container with labels to fill
	void FillStateLabels( IDTOSTATEMAP & states, list<IHashString *> &labels );

	CQHStateMachine *RegisterStateMachine( IHashString *file );

private:
	static const float	m_fCurrentVersion;

	CHashString		m_FileName;
	IDTOSTATEMAP	m_BaseStates;
	IDTOSTATEMAP	m_BaseStatesByLabel;
	IDTOSTATEMAP	m_GlobalStates;
	IDTOSTATEMAP	m_GlobalStatesByLabel;
	CQHState	   *m_StartState;
	CQHState	   *m_EndState;
	int				m_iNumStates;

	// current state machine parameters
	CParameters		m_Parameters;
	// new serialized state machine parameters. They are applied by Init() call
	CParameters		m_NewParameters;
};

#endif //#ifndef __QHSTATEMACHINE_H__
