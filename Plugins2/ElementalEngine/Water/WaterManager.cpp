///==========================================================================
/// \file	CWaterManager.cpp
/// \brief	Implementation of Water Manager
/// \date	07-28-2007
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
///==========================================================================

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WATERMANAGERPRIORITY     0x0582

//TOLUA_API int tolua_WaterScript_open (lua_State* tolua_S);

REGISTER_COMPONENT_SINGLETON( CWaterManager );
REGISTER_MESSAGE_HANDLER(Start, OnStart, CWaterManager);
REGISTER_MESSAGE_HANDLER(Stop, OnStop, CWaterManager);


IComponent *CWaterManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CWaterManager );
}

CWaterManager::CWaterManager() : 
	CManager(_T("CWaterManager"), WATERMANAGERPRIORITY, 0),
	m_hszOceanWaterType(_T("COceanRenderObject"))
{
	m_bIsRunning = true;
	RegisterLuaDefinitions();
	Init();
}

CWaterManager::~CWaterManager()
{
	DeInit();
}

bool CWaterManager::Init()
{
	return CManager::Init();
}

bool CWaterManager::DeInit()
{
	return CManager::DeInit();
}

void CWaterManager::Update(DWORD tickCount)
{
	// update all water objects
	IDTOOBJECTMAP *objMap = GetObjectMap( &m_hszOceanWaterType );
	if( objMap )
	{		
		IDTOOBJECTMAP::iterator objIter;
		for( objIter = objMap->begin(); objIter != objMap->end(); objIter++ )
		{
			COceanRenderObject *pObject = (COceanRenderObject*)objIter->second;
			pObject->Update();
		}
	}
}

DWORD CWaterManager::OnStart(DWORD size, void *data)
{
	m_bIsRunning = true;
	return MSG_HANDLED_PROCEED;
}

DWORD CWaterManager::OnStop(DWORD size, void *data)
{
	m_bIsRunning = false;
	return MSG_HANDLED_PROCEED;
}

void CWaterManager::RegisterLuaDefinitions()
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
