///============================================================================
/// \file		QHStateMachineDefines.h
/// \brief		Structs for state machine event parameters
/// \date		10-05-2006
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

#ifndef __QHSTATEMACHINEDEFINES_H__
#define __QHSTATEMACHINEDEFINES_H__

class CQHState;
class CQHDerivedState;
class CQHStateMachineEvent;

typedef map<DWORD, CQHState *> IDTOSTATEMAP;
typedef map<DWORD, CQHDerivedState *> IDTODERIVEDSTATEMAP;
typedef map<DWORD, CQHStateMachineEvent *> IDTOEVENTMAP;

// tolua_begin
struct TRIGGEREVENTINMGRPARAMS
{
	IHashString		*StateObjectName;
	IHashString		*EventName;
	IArchive		*EventParamsArchive;
};
// tolua_end

// tolua_begin
struct TRIGGEREVENTPARAMS
{
	IHashString		*EventName;
	IArchive		*EventParamsArchive;
};
// tolua_end

// tolua_begin
struct LOADANDINITSTATEOBJECTPARAMS
{
	IHashString *StateObjectName;			// Name of the State Object.
	IHashString *StateObjectParentName;		// Name of the State Object.
	IHashString *StateMachineName;			// Name of the State Machine
	IHashString *StateMachineFileName;		// File Name of the State Machine
	IHashString *StateMachineStartState;	// Name of Starting State
	bool bIsInHierarchy;					// Add To The Hierachry
	int iUpdatePriority;					// Prioritizes the object on the update queue
	
	LOADANDINITSTATEOBJECTPARAMS()
	{
		StateObjectName = NULL;
		StateObjectParentName = NULL;
		StateMachineName = NULL;
		StateMachineFileName = NULL;
		StateMachineStartState = NULL;
		bIsInHierarchy = true;
		// divided by two to be a middle priority, so we can set
		// ones above it and below it
		iUpdatePriority = INT_MAX / 2;
	}
};
// tolua_end

struct QUERYEVENTITEM
{
	IHashString* StateName;
	IHashString* EventName;
};

typedef std::list<QUERYEVENTITEM> QUERYEVENTSLIST;

struct QUERYEVENTSPARAMS
{
	QUERYEVENTSPARAMS()
	{
		StateMachineName = NULL;
	}

	IHashString *StateMachineName;
	QUERYEVENTSLIST QueryEventsList;
};

struct GETSTATEMACHINEPARAMS
{
	GETSTATEMACHINEPARAMS()
	{
		StateMachineName = NULL;
		StateMachineFilePath = NULL;
	}

	IHashString* StateMachineName;
	IHashString* StateMachineFilePath;
};

///	this structure is used for messages:
///	GetEventActionHandlers
///	SetEventActionHandlers
///	GetEventTransitions
///	GetStateMachineStates
///	GetStateEventLabels
struct GETSETSTATEMACHINENAMESPARAMS
{
	list<IHashString *> Names;
};

#endif // #ifndef __QHSTATEMACHINEDEFINES_H__