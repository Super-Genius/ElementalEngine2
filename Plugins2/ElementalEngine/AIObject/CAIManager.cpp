///=====================================================================
/// \file	CAIManager.cpp
/// \brief	Implements of CAIManager Class
///         This file implements a CAIManager which is the interface
///			between AI Module and Red Phoenix.
///			Inherits from IManager
/// \date	02/22/2005
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

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TOLUA_API int tolua_AIObjectScript_open (lua_State* tolua_S);

REGISTER_COMPONENT_SINGLETON(CAIManager);

REGISTER_MESSAGE_HANDLER(Start, OnStart, CAIManager);
REGISTER_MESSAGE_HANDLER(Stop, OnStop, CAIManager);


/// \brief The Default Constructor for CAIManageR
CAIManager::CAIManager() :
	CManager(_T("CAIManager"), AIPRIORITY),
	m_AIObjectComponentType(_T("CAIObject"))
{
	RegisterLuaDefinitions();
}

/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CAIManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CAIManager);
}

/// \brief The Default Destructor for CAIManager
CAIManager::~CAIManager( )
{
}

void CAIManager::RegisterLuaDefinitions()
{
	static DWORD msgHash_GetMasterScriptState = CHashString(_T( "GetMasterScriptState" )).GetUniqueID();
	DWORD amessageHandle = m_ToolBox->SendMessage(msgHash_GetMasterScriptState, sizeof( lua_State * ), &m_LuaState );
	if( amessageHandle != MSG_HANDLED)
	{
		StdString error( "Error missing Master Script State Data\n" );

		// log error
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
		EngineGetToolBox()->Log(LOGWARNING, error);
    }
	else
	{
		// register our additional structures/handlers with LUA master
		tolua_AIObjectScript_open(m_LuaState);
	}
}

/// \brief	Initializes the AI Manager and AI System
/// \return	bool: Success
bool CAIManager::Init()
{
	// Set to run by default
	m_IsRunning = true;

	return CManager::Init();
}

/// \brief	Start running the physics simulation
DWORD CAIManager::OnStart(DWORD size, void *params)
{
	m_IsRunning = true;

	return MSG_HANDLED_PROCEED;
}

/// \brief	Stop running the physics simulation
DWORD CAIManager::OnStop(DWORD size, void *params)
{
	m_IsRunning = false;

	return MSG_HANDLED_PROCEED;
}

CAIObject *CAIManager::GetAIObject( IHashString *objectName )
{
	CAIObject *areturn = NULL;
	IDTOOBJECTMAP *objectMap = GetObjectMap( &m_AIObjectComponentType );

	if( objectName != NULL && objectMap != NULL )
	{
		IDTOOBJECTMAP::iterator aObjectsIterator = objectMap->find(objectName->GetUniqueID());
		if (aObjectsIterator != objectMap->end())
		{
			areturn = (CAIObject*)aObjectsIterator->second;
		}
	}

	return areturn;
}