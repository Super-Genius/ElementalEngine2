///============================================================================
/// \file		CQHStateMachineCloneVisitor.cpp
/// \brief		Implementation file for State Visitor. Makes a copy of the visited
///				state machine and copies its contents to the specified state machine.
/// \date		05-17-2007
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

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CQHStateMachineCloneVisitor::CQHStateMachineCloneVisitor( CQHStateMachine *destinationStateMachine, CQHStateMachine *sourceStateMachine )
{
	m_ToolBox = EngineGetToolBox();
	m_DestinationStateMachine = destinationStateMachine;
	m_SourceStateMachine = sourceStateMachine;
	
	CREATEARCHIVE ca;
	CHashString memType(_T("Memory"));
	ca.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	ca.streamData = NULL;
	ca.streamSize = 0;
	ca.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
	m_archive = ca.archive;
}

CQHStateMachineCloneVisitor::~CQHStateMachineCloneVisitor()
{
	m_archive->Close();
}

void CQHStateMachineCloneVisitor::CloneSourceToDestination()
{
	m_SourceStateMachine->VisitNode( this );
}

bool CQHStateMachineCloneVisitor::Visit( IComponent *component, bool bVisitEnter )
{
	if( m_DestinationStateMachine != NULL && component != NULL && bVisitEnter )
	{
		IObject *object = dynamic_cast<IObject *>( component );

		CHashString cloneObjectName( _T("") );
		CreateCloneObject( object, &cloneObjectName );

		if( !cloneObjectName.IsEmpty() )
		{
			m_archive->SetIsWriting( true );
			m_archive->SeekTo( 0 );
			object->Serialize( *m_archive );
			m_archive->SetIsWriting( false );
			m_archive->SeekTo( 0 );

			SERIALIZEOBJECTPARAMS serializeObjectParams;
			serializeObjectParams.name = &cloneObjectName;
			serializeObjectParams.archive = m_archive;
			static DWORD msgHash_SerializeObject = CHashString( _T("SerializeObject") ).GetUniqueID();
			m_ToolBox->SendMessage( msgHash_SerializeObject, sizeof( SERIALIZEOBJECTPARAMS ), &serializeObjectParams, NULL, NULL );

			INITOBJECTPARAMS initObjectParams;
			initObjectParams.name = &cloneObjectName;
			static DWORD msgHash_InitObject = CHashString( _T("InitObject") ).GetUniqueID();
			m_ToolBox->SendMessage( msgHash_InitObject, sizeof(INITOBJECTPARAMS), &initObjectParams, NULL, NULL );

			static DWORD msgHash_SetAbstract = CHashString( _T("SetAbstract") ).GetUniqueID();
			m_ToolBox->SendMessage( msgHash_SetAbstract, 0, NULL, &cloneObjectName, object->GetComponentType() );
		}
	}
	
	return true;
}

bool CQHStateMachineCloneVisitor::IsHierarchicalVisitor( void )
{
	return true;
}

void CQHStateMachineCloneVisitor::GenerateObjectName( IHashString *objectName, IHashString *nodeType )
{
	StdString sPrefix = m_DestinationStateMachine->GetName()->GetString();
	sPrefix += _T('.');
	sPrefix += nodeType->GetString();
	CHashString hszPrefix(sPrefix.c_str());

	GENERATEUNIQUEOBJECTNAMEPARAMS generateNameParams;
	generateNameParams.name = &hszPrefix;
	generateNameParams.newname = objectName;

	static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(generateNameParams), &generateNameParams );
}

void CQHStateMachineCloneVisitor::CreateCloneObject( IObject *object, IHashString *cloneObjectName )
{
	IHashString *name = object->GetName();
	IHashString *type = object->GetComponentType();
	IHashString *parentName = object->GetParentName();

	if( name != NULL && type != NULL && parentName != NULL )
	{
		static const DWORD hashCQHStateMachine = CHashString( _T("CQHStateMachine") ).GetUniqueID();
		if( type->GetUniqueID() != hashCQHStateMachine )
		{
			CHashString objectName;
			GenerateObjectName( &objectName, type );
			CHashString cloneParentName;
			if( parentName->GetUniqueID() == m_SourceStateMachine->GetName()->GetUniqueID() )
			{
				cloneParentName = m_DestinationStateMachine->GetName();
			}
			else
			{
				DWORD parentNameID = m_cloneParentIDMap[ parentName->GetUniqueID() ];
				cloneParentName = m_ToolBox->GetHashString( parentNameID );
			}
			m_cloneParentIDMap[ name->GetUniqueID() ] = objectName.GetUniqueID();

			CREATEOBJECTPARAMS createObjectParams;

			createObjectParams.name = &objectName;
			createObjectParams.parentName = &cloneParentName;
			createObjectParams.typeName = type;
			static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &createObjectParams);

			*cloneObjectName = objectName.GetString();
		}
	}
}