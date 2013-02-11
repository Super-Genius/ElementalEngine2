///============================================================================
/// \file		SplineManager.cpp
/// \brief		Implmentation of CSplineManager Class
/// \date		09-11-2005
/// \author		D. Patrick Ghiocel
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
#include "SplineManager.h"

TOLUA_API int tolua_SplineScript_open (lua_State* tolua_S);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON( CSplineManager );
REGISTER_MESSAGE_HANDLER( ShowAllSplines, OnShowAllSplines, CSplineManager );
REGISTER_MESSAGE_HANDLER( HideAllSplines, OnHideAllSplines, CSplineManager );

CHashString CSplineManager::m_hszSplineManagerClass( _T("CSplineManager") );
CHashString CSplineManager::m_hszSplineClass( _T("CSplineObject") );
CHashString CSplineManager::m_hszSplinePointClass( _T("CSplinePointObject") );

CSplineManager::CSplineManager( void ) : CManager(_T("CSplineManager"))
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
		tolua_SplineScript_open(m_LuaState);
	}

	m_bToRender = true;
}

CSplineManager::~CSplineManager( void )
{
	DeInit();
}

IComponent* CSplineManager::Create(int nArgs, va_list argptr)
{
	// Return the singleton instance of this class
	return SINGLETONINSTANCE( CSplineManager );
}


bool CSplineManager::GetGlobalRenderFlag()
{
	return m_bToRender;
}

DWORD CSplineManager::OnShowAllSplines(DWORD size, void *param)
{
    DWORD retVal = SetToRender( true );
	return MSG_HANDLED_STOP;
}

DWORD CSplineManager::OnHideAllSplines(DWORD size, void *param)
{
	DWORD retVal = SetToRender( false );
	return MSG_HANDLED_STOP;
}

DWORD CSplineManager::SetToRender( bool value )
{
	static CHashString soTypeName = _T("CSplineObject");
	IDTOOBJECTMAP *objectMap = GetObjectMap(&soTypeName);
	if (objectMap)
	{
		IDTOOBJECTMAP::iterator objIter = objectMap->begin();
		while (objIter != objectMap->end())
		{
			if (objIter->second)
			{
				CSplineObject *splineObj = dynamic_cast<CSplineObject*>(objIter->second);
				
				if (splineObj)
				{
					splineObj->SetToRender(value);
				}
			}
		}
	}

	static CHashString spoTypeName = _T("CSplinePointObject");
	objectMap = GetObjectMap( &spoTypeName );
	if( objectMap )
	{
		IDTOOBJECTMAP::iterator objIter = objectMap->begin();
		while( objIter != objectMap->end() )
		{
			if (objIter->second)
			{
				CSplinePointObject *splinePointObject = dynamic_cast<CSplinePointObject*>(objIter->second);
				
				if( splinePointObject )
				{
					splinePointObject->SetToRender( value );
				}
			}
		}
	}

	m_bToRender = value;
	return MSG_HANDLED_STOP;
}