///==========================================================================
/// \file	CClothManager.cpp
/// \brief	Implementation of Cloth Manager
/// \date	07-23-2007
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

///==========================================================================

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CLOTHMANAGERPRIORITY     0x0666

//TOLUA_API int tolua_WaterScript_open (lua_State* tolua_S);

REGISTER_COMPONENT_SINGLETON( CClothManager );
REGISTER_MESSAGE_HANDLER(Start, OnStart, CClothManager);
REGISTER_MESSAGE_HANDLER(Stop, OnStop, CClothManager);


IComponent *CClothManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CClothManager );
}

CClothManager::CClothManager() : 
	CManager(_T("CClothManager"), CLOTHMANAGERPRIORITY, CLOTHMANAGERPRIORITY),
	m_hszClothType(_T("CClothRenderObject"))
{
	m_bIsRunning = true;
	RegisterLuaDefinitions();
	Init();
}

CClothManager::~CClothManager()
{
	DeInit();
}

bool CClothManager::Init()
{
	return CManager::Init();
}

bool CClothManager::DeInit()
{
	return CManager::DeInit();
}

void CClothManager::Update(DWORD tickCount)
{
	// update all water objects
	IDTOOBJECTMAP *objMap = GetObjectMap( &m_hszClothType );
	if( objMap )
	{		
		IDTOOBJECTMAP::iterator objIter;
		for( objIter = objMap->begin(); objIter != objMap->end(); objIter++ )
		{
			CClothRenderObject *pObject = (CClothRenderObject*)objIter->second;
			pObject->Update();
		}
	}
}

DWORD CClothManager::OnStart(DWORD size, void *data)
{
	m_bIsRunning = true;
	return MSG_HANDLED_PROCEED;
}

DWORD CClothManager::OnStop(DWORD size, void *data)
{
	m_bIsRunning = false;
	return MSG_HANDLED_PROCEED;
}

void CClothManager::RegisterLuaDefinitions()
{
	static DWORD msgHash_GetMasterScriptState = CHashString(_T( "GetMasterScriptState" )).GetUniqueID();
	DWORD amessageHandle = m_ToolBox->SendMessage(msgHash_GetMasterScriptState, sizeof( lua_State * ), &m_LuaState );
	if( amessageHandle != MSG_HANDLED)
	{
		StdString error;
		error = _T("Error missing Master Script State Data\n");

		// log error
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
		EngineGetToolBox()->Log(LOGWARNING, error);
    }
	else
	{
		// register our additional structures/handlers with LUA master
//		tolua_WaterScript_open(m_LuaState);
	}
}
