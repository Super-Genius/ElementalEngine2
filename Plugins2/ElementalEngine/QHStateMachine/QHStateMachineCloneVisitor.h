///============================================================================
/// \file		CQHStateMachineCloneVisitor.h
/// \brief		Header file for visitor pattern to state machine hierarchy
/// \date		06-21-2007
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
#ifndef _CQHSTATEMACHINECLONEVISITOR_H_
#define _CQHSTATEMACHINECLONEVISITOR_H_

#include "StdAfx.h"

class CQHStateMachineCloneVisitor : public IVisitor
{
public:
	CQHStateMachineCloneVisitor( CQHStateMachine *destinationStateMachine, CQHStateMachine *sourceStateMachine );
	~CQHStateMachineCloneVisitor();

	void CloneSourceToDestination();
	bool Visit( IComponent *component, bool bVisitEnter );
	bool IsHierarchicalVisitor( void );

private:
	void GenerateObjectName( IHashString *objectName, IHashString *nodeType );
	void CreateCloneObject( IObject *object, IHashString *cloneObjectName );

	IToolBox *m_ToolBox;
	CQHStateMachine *m_DestinationStateMachine;
	CQHStateMachine *m_SourceStateMachine;
	map<DWORD, DWORD> m_cloneParentIDMap;
	IArchive *m_archive;
};

#endif		// #ifndef _CQHSTATEMACHINECLONEVISITOR_H_
