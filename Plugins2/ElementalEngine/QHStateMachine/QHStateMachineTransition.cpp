///============================================================================
/// \file		CQHStateMachineTransition.cpp
/// \brief		Implentation of CQHStateMachineTransition Class
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

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CQHStateMachineTransition);
REGISTER_MESSAGE_HANDLER( GetSchema, OnGetSchema, CQHStateMachineTransition );
REGISTER_MESSAGE_HANDLER( SetAbstract, OnSetAbstract, CQHStateMachineTransition );
REGISTER_MESSAGE_HANDLER( GetAbstract, OnGetAbstract, CQHStateMachineTransition );
REGISTER_MESSAGE_HANDLER( IsLabelEditble, OnIsLabelEditble, CQHStateMachineTransition );

const float CQHStateMachineTransition::m_fCurrentVersion = 1.0;

// Default Constructor
CQHStateMachineTransition::CQHStateMachineTransition( IHashString *parentName, IHashString *name ) :
	OBJECTTEMPLATE( CQHStateMachineManager, CQHStateMachineTransition, IObject, parentName, name ),
	m_bAbstract( false )
{
}

// Default Destructor
CQHStateMachineTransition::~CQHStateMachineTransition()
{
}

IComponent *CQHStateMachineTransition::Create( int nArgs, va_list argptr )
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = new CQHStateMachineTransition(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CQHStateMachineTransition::Init()
{
	// TODO: rnguyen
}

bool CQHStateMachineTransition::Update()
{
	// TODO: rnguyen
	return true;
}

void CQHStateMachineTransition::DeInit()
{
	CQHStateMachineEvent *parentEvent = GetEvent();

	if( parentEvent != NULL )
	{
		parentEvent->RemoveTransition( this );
	}
}

void CQHStateMachineTransition::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		float fVersion;
		StdString tempStr;
		ar.Read( fVersion, _T("version") );
		if( fVersion > m_fCurrentVersion )
		{
			LPCTSTR fmt = _T("%s(%d): Unsupported version (%f) of state machine transtion");
			m_ToolBox->Log( LOGERROR, fmt, __FILE__, __LINE__, fVersion );
			return;
		}
		ar.Read( tempStr, _T("name") );
		m_szName.Init( tempStr );
		ar.Read( m_iWeight, _T("weight") );
		ar.Read( tempStr, _T("targetState") );
		
		CQHStateMachineEvent *aparentEvent = GetEvent();
		if( aparentEvent != NULL )
		{
			aparentEvent->RemoveTransition( this );
		}

		m_szTargetState.Init( tempStr );
		aparentEvent = GetEvent();
		if( aparentEvent != NULL )
		{
			aparentEvent->AddTransition( this );
		}
		else
		{
			m_ToolBox->Log( LOGERROR, _T("Could not find parent event %s for transition %s."), GetParentName()->GetString(), GetName()->GetString() );
		}
	}
	else
	{
		ar.Write( m_fCurrentVersion, _T("version") );
		ar.Write( m_szName.GetString(), _T("name") );
		ar.Write( m_iWeight, _T("weight") );
		ar.Write( m_szTargetState.GetString(), _T("targetState") );
	}
}

IHashString *CQHStateMachineTransition::GetTransitionName()
{
	return &m_szName;
}

IHashString *CQHStateMachineTransition::GetTargetState()
{
	return &m_szTargetState;
}

void CQHStateMachineTransition::VisitNode( IVisitor *visitor )
{
	visitor->Visit( this, true );
	visitor->Visit( this, false );
}

DWORD CQHStateMachineTransition::OnGetSchema( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETSCHEMAPARAMS));
	GETSCHEMAPARAMS *getSchemaParams = (GETSCHEMAPARAMS*)params;
	IXMLArchive *archive = getSchemaParams->ar;
	if( archive != NULL )
	{
		archive->StartClass( GetComponentType()->GetString() );
		archive->Write( m_szName.GetString(), _T("Name") );
		archive->Write( 4 );

		archive->StartClass(_T("hidden") );
		archive->Write( _T("float"), _T("Type") );
		archive->EndClass();

		if( m_bAbstract )
		{
			archive->StartClass( _T("static") );
			archive->Write( _T("string"), _T("Type") );
			archive->Write( _T("Name"), _T("Name") );
			archive->EndClass();

			archive->StartClass( _T("static") );
			archive->Write( _T("int"), _T("Type") );
			archive->Write( _T("Weight"), _T("Name") );
			archive->EndClass();

			archive->StartClass( _T("static") );
			archive->Write( _T("string"), _T("Type") );
			archive->Write( _T("Target State Name"), _T("Name") );
			archive->EndClass();
		}
		else
		{
			archive->StartClass( _T("edit") );
			archive->Write( _T("string"), _T("Type") );
			archive->Write( _T("Name"), _T("Name") );
			archive->Write( false, _T("Key") );
			archive->EndClass();

			archive->StartClass( _T("edit") );
			archive->Write( _T("int"), _T("Type") );
			archive->Write( _T("Weight"), _T("Name") );
			archive->Write( false, _T("Key") );
			archive->EndClass();

			archive->StartClass( _T("combo") );
			archive->Write( _T("string"), _T("Type") );
			archive->Write( _T("Target State Name"), _T("Name") );
			archive->Write( false, _T("Key") );
			archive->Write( _T(""), _T("DefSelect") );

			list<IHashString *> labels;
			GetEvent()->GetStateMachineStateLabels( labels );
			archive->Write( (int)labels.size(), _T("Count") );
			int comboItemCounter = 0;
			TCHAR szComboNum[] = _T("ComboItem_XXXX");
			LPTSTR szNum = szComboNum + sizeof(szComboNum) / sizeof(*szComboNum) - 4;
			for( list<IHashString *>::const_iterator it = labels.begin(); it != labels.end(); ++it )
			{
				_ltot( ++comboItemCounter, szNum, 10 );
				archive->Write( (*it)->GetString(), szComboNum );
			}
			archive->EndClass();
		}

		archive->EndClass();
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Attempt to get CQHStateMachineEvent schema. No archive specified.\n" ) );
	}

	return MSG_HANDLED_STOP;
}

DWORD CQHStateMachineTransition::OnSetAbstract( DWORD size, void *params )
{
	m_bAbstract = true;
	return MSG_HANDLED_STOP;
}

DWORD CQHStateMachineTransition::OnGetAbstract( DWORD size, void *params )
{
	bool *p = static_cast<bool *>( params );
	VERIFY_MESSAGE_SIZE( size, sizeof( *p ) );
	assert( p != NULL );
	*p = m_bAbstract;
	return MSG_HANDLED_STOP;
}

///	\brief	return true if state name is editable (object is not abstract)
DWORD CQHStateMachineTransition::OnIsLabelEditble( DWORD size, void *params )
{
	bool *p = static_cast<bool *>( params );
	VERIFY_MESSAGE_SIZE( size, sizeof( *p ) );
	assert( p != NULL );
	*p = !m_bAbstract;
	return MSG_HANDLED_STOP;
}

int CQHStateMachineTransition::GetWeight()
{
	return m_iWeight;
}

///	\brief	return parent event object for transition
CQHStateMachineEvent *CQHStateMachineTransition::GetEvent()
{
	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CQHStateMachineManager *amanager;
	amanager = static_cast<CQHStateMachineManager*>( amanagerComponent );
	return amanager->GetEvent( GetParentName() );
}
