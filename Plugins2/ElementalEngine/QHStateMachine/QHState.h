///============================================================================
/// \file		QHState.h
/// \brief		Declaration of CQHState Object Class
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

#ifndef __QHSTATE_H__
#define __QHSTATE_H__

#include "stdafx.h"

class CQHStateMachine;
class CQHStateMachineManager;

struct LessIHashString : public binary_function <const IHashString *, const IHashString *, bool> 
{
	bool operator()( const IHashString *_Left, const IHashString *_Right ) const
	{
		return _Left->GetUniqueID() < _Right->GetUniqueID();
	}
};

class CQHState : public CObjectTemplate<IAttributeObject>
{
	enum { EDITOR_VALUES_COUNT = 8 };
public:
	// Default constructor
	CQHState( IHashString *parentName, IHashString *name );

	// Default destructor
	~CQHState();

	/// Set the parent name for this object
	/// \param inName = CHashString name for parent
	virtual void SetParentName(IHashString *inName);

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

	/// Get the value of specified attribute and writes it to archive
	/// \param hszAttributeName = IHashString name of attribute
	/// \param ar = Archive to write
	void GetAttribute( IHashString *hszAttributeName, IArchive *ar );

	/// Read the value of specified attribute from archive
	/// \param hszAttributeName = IHashString name of attribute
	/// \param ar = Archive to read
	void SetAttribute( IHashString *hszAttributeName, IArchive *ar );

	CQHState *GetParentState();

	void AddEvent( CQHStateMachineEvent *aevent);

	void RemoveEvent( CQHStateMachineEvent *aevent);

	void AddChildState( CQHState *achildState);

	void RemoveChildState( CQHState *achildState );

	/// Gets the event given the event label name (not object name)
	/// returns the pointer to the actual event object
	CQHStateMachineEvent *GetEvent( IHashString *aeventName );

	IHashString *GetStateName();

	bool EventExists( IHashString *aeventName );

	CQHStateMachine *GetStateMachine();

	IHashString *GetEntryEventName();

	IHashString *GetExitEventName();

	IHashString *GetUpdateEventName();

	bool IsBaseParent( CQHState *atestState );

	/// Populates the supplied list with event names recursively through children
	void QueryEvents( QUERYEVENTSLIST *queryEventsList );

	/// Populates the supplied list with events recursively through parents
	void GenerateAvailableEventsList( OBJECTLIST *availableEvents );

	void VisitNode( IVisitor *visitor );

	DWORD OnGetSchema( DWORD size, void *params );
	DWORD OnSetAbstract( DWORD size, void *params );
	DWORD OnGetAbstract( DWORD size, void *params );
	DWORD OnVisitHierarchy( DWORD size, void *params );
	DWORD OnGetStateMachineStateLabels( DWORD size, void *params );
	/// message handler for GetChildStateNames. provide list of base state names
	DWORD OnGetChildStateNames( DWORD size, void* params );
	DWORD OnGetStateEventLabels( DWORD size, void *params );
	///	collect all events up to state machine hierarchy
	DWORD OnGetStateAvailableEventLabels( DWORD size, void *params );
	DWORD OnChangeObjectParent( DWORD size, void *params );
	///	\brief	return event object name by event label
	///	\param	size - sizeof(IHashString)
	///	\param	params - [in] label of the event [out] - event object name
	DWORD OnGetEventNameFromLabel( DWORD size, void *params );
	///	\brief	return true if state name is editable (object is not abstract)
	DWORD OnIsLabelEditble( DWORD size, void *params );

	///	\brief	add state's available event labels to the set
	///	\param	labels - result set with labels
	///	\param	addInternal - if value set to false then entry, update and exit 
	///						  events are not added to the label list
	void GetAvailableEventLabels( set<IHashString *, LessIHashString> &labels, bool addInternal );

	bool IsAbstract() const
	{
		return m_bAbstract;
	}

private:
	///	\brief	register state in parent object and state machine
	void RegisterState();
	///	\brief	unregister state from parent object and state machine
	void UnregisterState();

	static const LPCTSTR *GetEditorValueNames();

	CQHStateMachineManager *GetManager();

	void ReadFromVersion_1_2( IArchive &ar );
	void ReadFromVersion_1_1( IArchive &ar );
	void ReadFromVersion_1_0( IArchive &ar );

private:
	static const float m_fCurrentVersion;
	CHashString	m_Name;
	CHashString	m_NewName;

	CHashString	m_EntryEvent;
	CHashString	m_UpdateEvent;
	CHashString	m_ExitEvent;

	bool		m_bAbstract;
	
	CQHStateMachine	*m_StateMachine;
	IDTOSTATEMAP m_ChildStates;
	IDTOEVENTMAP m_Events;
	IDTOEVENTMAP m_EventsByLabel;
};

#endif //#ifndef __QHSTATE_H__
