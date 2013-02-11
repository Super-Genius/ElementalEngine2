///=====================================================================
/// \file	CAIObject.cpp
/// \brief	Implementation of CAIObject Class
///         This file implements a CAIObject which is an AI Object
///			for entities
///			Inherits from IObject
/// \date	02/15/2005
/// \author	Patrick Ghiocel
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

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CAIObject);

REGISTER_MESSAGE_HANDLER( StartCommandSequence, OnStartCommandSequence, CAIObject );
REGISTER_MESSAGE_HANDLER( CompleteCurrentCommand, OnCompleteCurrentCommand, CAIObject );
REGISTER_MESSAGE_HANDLER( ClearCommandSequence, OnClearCommandSequence, CAIObject );

/// \fn void CAIObject( void )
/// \brief The Default Constructor for CAIObject
CAIObject::CAIObject(IHashString *parentName, IHashString *name) :
	OBJECTTEMPLATE(CAIManager, CAIObject, IObject, parentName, name),
	m_msgExecuteCommand( _T("ExecuteCommand") )
{
	AddToHierarchy();
}


/// \fn void ~CAIObject( void )
/// \brief The Default Destructor for CAIObject
CAIObject::~CAIObject()
{
	RemoveFromHierarchy();
}

/// \fn void Create()
/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CAIObject::Create(int nArgs, va_list argptr)
{
	IObject *self;

	IHashString *name = va_arg( argptr, IHashString * );
	IHashString *parentName = va_arg( argptr, IHashString * );
	try
	{
		self = new CAIObject( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}


void CAIObject::Serialize(IArchive &ar)
{
	if( ar.IsReading() )
	{
	}
	else
	{
	}
}

/// \fn bool Update( void )
/// \brief The Update Function for the Object
/// \return bool: Success
bool CAIObject::Update( void )
{
	return true;
}

/// \fn void DeInit( void )
/// \brief The DeInit Function for the Object
/// \return void
void CAIObject::DeInit( void )
{
}

void CAIObject::AddAIObjectCommand( CAIObjectCommand *object )
{
	m_AICommandSequence.push_back( object );
}

void CAIObject::RemoveAIObjectCommand( CAIObjectCommand *object )
{
	m_AICommandSequence.remove( object );
}

void CAIObject::ExecuteCurrentCommand( IHashString *targetName )
{
	DWORD targetNameID = targetName->GetUniqueID();

	if( m_SequenceIteratorMap.find( targetNameID ) != m_SequenceIteratorMap.end() )
	{
		CAIObjectCommand *currentCommand = *m_SequenceIteratorMap[ targetNameID ];
		m_ToolBox->SendMessage( m_msgExecuteCommand.GetUniqueID(), sizeof(IHashString), currentCommand->GetName(), targetName );
		if( *m_SequenceIteratorMap[ targetNameID ] == m_AICommandSequence.back() )
		{
			m_SequenceIteratorMap.erase( targetNameID );
		}
		else
		{
			m_SequenceIteratorMap[ targetNameID ]++;
		}
	}
}

void CAIObject::CancelCommandSequence( IHashString *targetName )
{
	DWORD targetNameID = targetName->GetUniqueID();

	if( m_SequenceIteratorMap.find( targetNameID ) != m_SequenceIteratorMap.end() )
	{
		m_SequenceIteratorMap.erase( targetNameID );
	}
}

DWORD CAIObject::OnStartCommandSequence( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(COMMANDSEQUENCEPARAMS) );
	COMMANDSEQUENCEPARAMS *commandSequenceParams = (COMMANDSEQUENCEPARAMS*)params;
	if( m_AICommandSequence.begin() != m_AICommandSequence.end() )
	{
		DWORD targetNameID = commandSequenceParams->targetName->GetUniqueID();
		m_SequenceIteratorMap[ targetNameID ] = m_AICommandSequence.begin();

		ExecuteCurrentCommand( commandSequenceParams->targetName );
	}

	return MSG_HANDLED_STOP;
}

DWORD CAIObject::OnCompleteCurrentCommand( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(COMMANDSEQUENCEPARAMS) );
	COMMANDSEQUENCEPARAMS *commandSequenceParams = (COMMANDSEQUENCEPARAMS*)params;

	ExecuteCurrentCommand( commandSequenceParams->targetName );

	return MSG_HANDLED_STOP;
}

DWORD CAIObject::OnClearCommandSequence( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(COMMANDSEQUENCEPARAMS) );
	COMMANDSEQUENCEPARAMS *commandSequenceParams = (COMMANDSEQUENCEPARAMS*)params;
	DWORD targetNameID = commandSequenceParams->targetName->GetUniqueID();

	m_SequenceIteratorMap.erase( targetNameID );

	return MSG_HANDLED_STOP;
}