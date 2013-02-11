///============================================================================
/// \file		QHStateMachineEvent.h
/// \brief		Declaration of CQHStateMachineEvent Object Class
/// \date		09-25-2006
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

#ifndef __QHSTATEMACHINEEVENT_H__
#define __QHSTATEMACHINEEVENT_H__

#include "stdafx.h"

class CQHStateMachineEvent : public CObjectTemplate<IAttributeObject>
{
public:
	// Default constructor
	CQHStateMachineEvent( IHashString *parentName, IHashString *name );

	// Default destructor
	virtual ~CQHStateMachineEvent();

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
	void Serialize( IArchive &ar );

	/// Get the value of specified attribute and writes it to archive
	/// \param hszAttributeName = IHashString name of attribute
	/// \param ar = Archive to write
	void GetAttribute( IHashString *hszAttributeName, IArchive *ar );

	/// Read the value of specified attribute from archive
	/// \param hszAttributeName = IHashString name of attribute
	/// \param ar = Archive to read
	void SetAttribute( IHashString *hszAttributeName, IArchive *ar );

	/// Add a CQHStateMachineActionHandler to the action handlers object list
	void AddActionHandler( CQHStateMachineActionHandler *aactionHandler );

	/// Add a CQHStateMachineTransition to the transitions object list
	void AddTransition( CQHStateMachineTransition *atransition );

	/// Removes the specified CQHStateMachineActionHandler from the actions object list
	void RemoveActionHandler( CQHStateMachineActionHandler *aaction );

	/// Removes the specified CQHStateMachineTransition from the transitions object list
	void RemoveTransition( CQHStateMachineTransition *atransition );

	/// Will go through the action's under this event and execute them in turn.
	void ExecuteActionHandlers(IArchive *arch, IHashString *zEntityName);

	bool HasTransitions( void );

	CQHStateMachineTransition *GetTransition();

	/// Returns the labelled name of the event.
	IHashString *GetEventName();

	void VisitNode( IVisitor *visitor );

	DWORD OnGetSchema( DWORD size, void *params );
	DWORD OnSetAbstract( DWORD size, void *params );
	DWORD OnGetAbstract( DWORD size, void *params );
	DWORD OnVisitHierarchy( DWORD size, void *params );
	DWORD OnGetEventActionHandlers( DWORD size, void *params );
	DWORD OnSetEventActionHandlers( DWORD size, void *params );
	DWORD OnGetEventTransitions( DWORD size, void *params );
	DWORD OnGetStateMachineStateLabels( DWORD size, void *params );
	///	\brief	return true if state name is editable (object is not abstract)
	DWORD OnIsLabelEditble( DWORD size, void *params );

	///	\brief	extract labels from all state machine states
	///	\param	[out]labels - reference to container with labels to fill
	void GetStateMachineStateLabels( list<IHashString *> &labels );

private:
	int GetTotalTransitionWeight();
	CQHStateMachineTransition *GetTransition( int iWeight );
	CQHState *GetParentState();

	/// this function is used for sorting transitions in the list by weight
	///	\param	transtion - pointer to CQHStateMachineTransition object
	///	\param	weight - weight to compare with transition weight
	///	\return	true if transition has less weight than passed weight value
	static bool IsTransitionLighter( IObject *transtion , int weight );

private:
	static const float			m_fCurrentVersion;
	CHashString					m_Name;

	OBJECTLIST					m_ActionHandlers;

	bool						m_bAbstract;

	// m_Transitions is ordered by transtion weights (heaviest weights first)
	OBJECTLIST					m_Transitions;

	bool						m_bTransitionsDirty;
	int							m_iTotalTransitionWeight;
	
};

#endif //#ifndef __QHSTATEMACHINEEVENT_H__