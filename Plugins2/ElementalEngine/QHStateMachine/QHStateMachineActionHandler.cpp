///============================================================================
/// \file		CQHStateMachineActionHandler.cpp
/// \brief		Implentation of CQHStateMachineActionHandler Class
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
#include "PerformanceProfiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CQHStateMachineActionHandler);
REGISTER_MESSAGE_HANDLER( GetSchema, OnGetSchema, CQHStateMachineActionHandler );
REGISTER_MESSAGE_HANDLER( SetAbstract, OnSetAbstract, CQHStateMachineActionHandler );
REGISTER_MESSAGE_HANDLER( GetAbstract, OnGetAbstract, CQHStateMachineActionHandler );
REGISTER_MESSAGE_HANDLER( IsLabelEditble, OnIsLabelEditble, CQHStateMachineActionHandler );

const float CQHStateMachineActionHandler::m_fCurrentVersion = 1.0;

// Default Constructor
CQHStateMachineActionHandler::CQHStateMachineActionHandler( IHashString *parentName, IHashString *name ) :
	OBJECTTEMPLATE( CQHStateMachineManager, CQHStateMachineActionHandler, IObject, parentName, name )
{
	m_bAbstract = false;
}

// Default Destructor
CQHStateMachineActionHandler::~CQHStateMachineActionHandler()
{
}

IComponent *CQHStateMachineActionHandler::Create( int nArgs, va_list argptr )
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
		self = new CQHStateMachineActionHandler(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CQHStateMachineActionHandler::Init()
{
	// TODO: rnguyen
	static CHashString man(_T("CLuaStateHandler"));
	m_pAH = dynamic_cast<IActionHandler *>(m_ToolBox->CreateComponent(&man, 0));
}

bool CQHStateMachineActionHandler::Update()
{
	// TODO: rnguyen
	return true;
}

void CQHStateMachineActionHandler::DeInit()
{
	IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
	CQHStateMachineManager *amanager = static_cast<CQHStateMachineManager*>( amanagerComponent );
	CQHStateMachineEvent *parentEvent = amanager->GetEvent( GetParentName() );

	if( parentEvent != NULL )
	{
		parentEvent->RemoveActionHandler( this );
	}
}

void CQHStateMachineActionHandler::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		float fVersion;
		StdString tempStr;
		ar.Read( fVersion, _T("version") );
		if( fVersion > m_fCurrentVersion )
		{
			LPCTSTR fmt = _T("%s(%d): Unsupported version (%f) of state machine action handler");
			m_ToolBox->Log( LOGERROR, fmt, __FILE__, __LINE__, fVersion );
			return;
		}
		ar.Read( tempStr, _T("name") );
		m_szName.Init( tempStr );
		ar.Read( tempStr, _T("actionName") );
		m_szActionName.Init( tempStr );

		IComponent *amanagerComponent = m_ToolBox->GetComponent( GetManagerName() );
		CQHStateMachineManager *amanager = static_cast<CQHStateMachineManager*>( amanagerComponent );
		CQHStateMachineEvent *aparentEvent = amanager->GetEvent( GetParentName() );
		if( aparentEvent != NULL )
		{
			aparentEvent->AddActionHandler( this );
		}
		else
		{
			m_ToolBox->Log( LOGERROR, _T("Could not find parent event %s for action handler %s."), GetParentName()->GetString(), GetName()->GetString() );
		}
	}
	else
	{
		ar.Write( m_fCurrentVersion, _T("version") );
		ar.Write( m_szName.GetString(), _T("name") );
		ar.Write( m_szActionName.GetString(), _T("actionName") );
	}
}

bool CQHStateMachineActionHandler::SendActionMessage(IArchive *arc, IHashString *zParentName)
{

	// m_szName has the name of a lua function that should be called
	// so we'll have to construct a message to do that.  

	IActionHandler::ACTIONCALLBACKMAP *callBackMap;
	IActionHandler::ACTIONCALLBACKMAP::iterator iter;

	if( m_pAH != NULL )
	{
		callBackMap = m_pAH->GetActionHandlers();
		CHashString luaFunctionName(m_szActionName);
		iter = callBackMap->find(luaFunctionName.GetUniqueID());
		
		if ( iter != callBackMap->end() )
		{
			// it has been found
			IActionHandler::ACTIONCALLBACK callBack;
			callBack = iter->second;
			try
			{
#if defined _PROFILE || defined _DEBUG
				StdString objectDescription = _T("");
				if( zParentName != NULL )
				{
					objectDescription = zParentName->GetString();
				}
				objectDescription += _T(" - ");
				objectDescription += m_szActionName.GetString();
				PERFORMANCE_PROFILER_TYPE_START( objectDescription, _T("Action Handlers") );
#endif // _PROFILE

				(m_pAH->*callBack)(arc, &luaFunctionName, zParentName);

#if defined _PROFILE || defined _DEBUG
				PERFORMANCE_PROFILER_TYPE_STOP( objectDescription, _T("Action Handlers") );
#endif // _PROFILE
			}
			catch(...)
			{
				return false;
			}
		}
		else
		{
			m_ToolBox->SetErrorValue( WARN_OBJECT_NOT_FOUND );
			m_ToolBox->Log( LOGWARNING, _T( "CQHStateMachineActionHandler: Could not find Lua function: %s\n" ), luaFunctionName.GetString() );
		}
	}
	else
	{
		m_ToolBox->SetErrorValue( WARN_OBJECT_NOT_FOUND );
		m_ToolBox->Log( LOGWARNING, _T( "CQHStateMachineActionHandler: CLuaStateHandler not loaded.\n" ) );
	}

	return true;
}

void CQHStateMachineActionHandler::VisitNode( IVisitor *visitor )
{
	visitor->Visit( this, true );
	visitor->Visit( this, false );
}

DWORD CQHStateMachineActionHandler::OnGetSchema( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GETSCHEMAPARAMS));
	GETSCHEMAPARAMS *getSchemaParams = (GETSCHEMAPARAMS*)params;
	IXMLArchive *archive = getSchemaParams->ar;
	if( archive != NULL )
	{
		archive->StartClass( GetComponentType()->GetString() );
		archive->Write( m_Name.GetString(), _T("Name") );
		archive->Write( 3 );

		archive->StartClass(_T("hidden") );
		archive->Write( _T("float"), _T("Type") );
		archive->EndClass();

		archive->StartClass(_T("hidden") );
		archive->Write( _T("string"), _T("Type") );
		archive->EndClass();

		if( m_bAbstract )
		{
			archive->StartClass( _T("static") );
			archive->Write( _T("string"), _T("Type") );
			archive->Write( _T("Action Name"), _T("Name") );
			archive->EndClass();
		}
		else
		{
			archive->StartClass( _T("edit") );
			archive->Write( _T("string"), _T("Type") );
			archive->Write( _T("Action Name"), _T("Name") );
			archive->Write( false, _T("Key") );
			archive->EndClass();
		}
	}
	else
	{
		m_ToolBox->Log( LOGERROR, _T("Attempt to get CQHStateMachineEvent schema. No archive specified.\n" ) );
	}

	return MSG_HANDLED_STOP;
}

DWORD CQHStateMachineActionHandler::OnSetAbstract( DWORD size, void *params )
{
	m_bAbstract = true;
	return MSG_HANDLED_STOP;
}

DWORD CQHStateMachineActionHandler::OnGetAbstract( DWORD size, void *params )
{
	bool *p = static_cast<bool *>( params );
	VERIFY_MESSAGE_SIZE( size, sizeof( *p ) );
	assert( p != NULL );
	*p = m_bAbstract;
	return MSG_HANDLED_STOP;
}

///	\brief	return true if state name is editable (object is not abstract)
DWORD CQHStateMachineActionHandler::OnIsLabelEditble( DWORD size, void *params )
{
	bool *p = static_cast<bool *>( params );
	VERIFY_MESSAGE_SIZE( size, sizeof( *p ) );
	assert( p != NULL );
	*p = !m_bAbstract;
	return MSG_HANDLED_STOP;
}
