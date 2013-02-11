///=====================================================================
/// \file	CAIObjectCommand.cpp
/// \brief	Implementation of CAIObjectCommand Class
///         This file implements the CAIObjectCommand class
///			Inherits from IObject
/// \date	05/23/2007
/// \author	Richard Nguyen
/// \note	Copyright (c) 2007 Signature Devices, Inc.
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

REGISTER_COMPONENT(CAIObjectCommand);

REGISTER_MESSAGE_HANDLER( GetCommandString, OnGetCommandString, CAIObjectCommand );
REGISTER_MESSAGE_HANDLER( CompleteCommand, OnCompleteCommand, CAIObjectCommand );
REGISTER_MESSAGE_HANDLER( CancelCommand, OnCancelCommand, CAIObjectCommand );
REGISTER_MESSAGE_HANDLER( GetSchema, OnGetSchema, CAIObjectCommand );

/// \fn void CAIObjectCommand( void )
/// \brief The Default Constructor for CAIObjectCommand
CAIObjectCommand::CAIObjectCommand(IHashString *parentName, IHashString *name) :
	OBJECTTEMPLATE(CAIManager, CAIObjectCommand, IObject, parentName, name)
{
	AddToHierarchy();
}


/// \fn void ~CAIObjectCommand( void )
/// \brief The Default Destructor for CAIObjectCommand
CAIObjectCommand::~CAIObjectCommand()
{
	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CAIManager *amanager = static_cast<CAIManager*>( amanagerComponent );
	CAIObject *aparentAIObject = amanager->GetAIObject( GetParentName() );
	if( aparentAIObject != NULL )
	{
		aparentAIObject->RemoveAIObjectCommand( this );
	}
	RemoveFromHierarchy();
}

/// \fn void Create()
/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CAIObjectCommand::Create(int nArgs, va_list argptr)
{
	IObject *self;

	IHashString *name = va_arg( argptr, IHashString * );
	IHashString *parentName = va_arg( argptr, IHashString * );
	try
	{
		self = new CAIObjectCommand( parentName, name );
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}


void CAIObjectCommand::Serialize(IArchive &ar)
{
	if( ar.IsReading() )
	{
		StdString szTempString;
		ar.Read( szTempString, _T("Command") );

		m_hszCommandString.Init( szTempString );

		IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
		CAIManager *amanager = static_cast<CAIManager*>( amanagerComponent );
		CAIObject *aparentAIObject = amanager->GetAIObject( GetParentName() );
		if( aparentAIObject != NULL )
		{
			aparentAIObject->AddAIObjectCommand( this );
		}
		else
		{
			m_ToolBox->Log( LOGERROR, _T("Could not find parent AI Object %s for Command %s."), GetParentName()->GetString(), GetName()->GetString() );
		}
	}
	else
	{
		ar.Write( m_hszCommandString.GetString(), _T("Command") );
	}
}

bool CAIObjectCommand::Update( void )
{
	return true;
}

void CAIObjectCommand::DeInit( void )
{
}

DWORD CAIObjectCommand::OnGetCommandString( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( sizeof(IHashString), size );

	IHashString *hszWriteString = (IHashString*)params;
	*hszWriteString = m_hszCommandString.GetString();
	return MSG_HANDLED_STOP;
}

DWORD CAIObjectCommand::OnCompleteCommand( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(COMMANDSEQUENCEPARAMS) );
	COMMANDSEQUENCEPARAMS *commandSequenceParams = (COMMANDSEQUENCEPARAMS*)params;

	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CAIManager *aAIManager = (CAIManager*)amanagerComponent;

	CAIObject *aAIObject = aAIManager->GetAIObject( GetParentName() );
	aAIObject->ExecuteCurrentCommand( commandSequenceParams->targetName );

	return MSG_HANDLED_STOP;
}

DWORD CAIObjectCommand::OnCancelCommand( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(COMMANDSEQUENCEPARAMS) );
	COMMANDSEQUENCEPARAMS *commandSequenceParams = (COMMANDSEQUENCEPARAMS*)params;

	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CAIManager *aAIManager = (CAIManager*)amanagerComponent;

	CAIObject *aAIObject = aAIManager->GetAIObject( GetParentName() );
	aAIObject->CancelCommandSequence( commandSequenceParams->targetName );

	return MSG_HANDLED_STOP;
}

DWORD CAIObjectCommand::OnGetSchema( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETSCHEMAPARAMS));
	GETSCHEMAPARAMS *getSchemaParams = (GETSCHEMAPARAMS*)params;
	IXMLArchive *archive = getSchemaParams->ar;
	if( archive != NULL )
	{
		archive->StartClass( GetComponentType()->GetString() );
		archive->Write( GetName()->GetString(), _T("Name") );
		archive->Write( 1, _T("Num") );

		archive->StartClass(_T("edit") );
		archive->Write( _T("string"), _T("Type") );
		archive->Write( _T("Command String"), _T("Name") );
		archive->EndClass();

		archive->EndClass();
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Attempt to get CAIObjectCommand schema. No archive specified.\n" ) );
	}

	return MSG_HANDLED_STOP;
}