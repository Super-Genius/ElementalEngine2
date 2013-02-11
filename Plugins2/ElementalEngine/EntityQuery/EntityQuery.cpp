///============================================================================
/// \file		EntityQuery.cpp
/// \brief		Implementation of EntityQuery System
/// \date		12-20-2006
/// \author		Patrick Ghiocel
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

#define ENTITYQUERYPRIORITY 0x280

REGISTER_COMPONENT_SINGLETON(CEntityQuery);

TOLUA_API int tolua_EntityQueryLua_open (lua_State* tolua_S);

REGISTER_MESSAGE_HANDLER( SubmitQuery, OnSubmitQuery, CEntityQuery );
REGISTER_MESSAGE_HANDLER( SubmitTypeQuery, OnSubmitTypeQuery, CEntityQuery );

// the class of object that we handle
CHashString CEntityQuery::m_hszEntityQueryClass(_T("CEntityQuery"));

CEntityQuery::CEntityQuery()
{
	m_ToolBox = EngineGetToolBox();

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
		tolua_EntityQueryLua_open(m_LuaState);
	}
}

CEntityQuery::~CEntityQuery()
{

}

IComponent *CEntityQuery::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CEntityQuery);
}

void CEntityQuery::Serialize( IArchive &ar )
{

}

IHashString* CEntityQuery::GetComponentType( void ) 
{ 
	static CHashString dbCT = _T("CEntityQuery");
	return &dbCT; 
}

bool CEntityQuery::IsKindOf( IHashString *compType ) 
{ 
	return false; 
}

DWORD CEntityQuery::OnSubmitQuery(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( sizeof( GETOBJECTQUERYPARAMS ), size );
	if( params != NULL )
	{
		GETOBJECTQUERYPARAMS* objectQueryParams = (GETOBJECTQUERYPARAMS*)params;
		if( objectQueryParams->pObjectsList )
		{
			// Set up the entity visitor
			m_Visitor.m_SortedDistanceObjects.clear();
			m_Visitor.SetQuery( objectQueryParams->vPosition, objectQueryParams->hszType );
			
			// Visit the scene
			VISITSCENESPHEREPARAMS vsp;
			vsp.visitor = (IVisitor*)&m_Visitor;
			vsp.center = &objectQueryParams->vPosition;
			vsp.radius = objectQueryParams->fRadius;
			vsp.sortID = ENTITY;

			static DWORD msg_VisitSceneSphere = CHashString(_T("VisitSceneSphere")).GetUniqueID();
			DWORD retval = m_ToolBox->SendMessage( msg_VisitSceneSphere, sizeof(VISITSCENESPHEREPARAMS), &vsp, NULL, NULL );

			if( retval == MSG_HANDLED )
			{
				// Add each object to the list message params.
				DISTANCEOBJECTMAP::iterator iter = m_Visitor.m_SortedDistanceObjects.begin();
				for( ; iter != m_Visitor.m_SortedDistanceObjects.end(); iter++)
				{
					IObject* object = iter->second;

					OBJECTDISTANCEQUERY newquery;
					newquery.distance = iter->first;
					newquery.name = object->GetName();
					objectQueryParams->pObjectsList->push_back( newquery );
				}
			}
		}
	}
	
	return MSG_HANDLED_STOP;
}

DWORD CEntityQuery::OnSubmitTypeQuery(DWORD size, void *params)
{
	DWORD retVal = MSG_ERROR;
	VERIFY_MESSAGE_SIZE( sizeof( GETOBJECTTYPEQUERYPARAMS ), size );
	if( params != NULL )
	{
		GETOBJECTTYPEQUERYPARAMS *queryParams = (GETOBJECTTYPEQUERYPARAMS*)params;
		IHashString *typeNameQuery = queryParams->type;
		OBJECTTYPEQUERYLIST *objectsList = queryParams->pObjectsList;

		if( typeNameQuery != NULL && objectsList != NULL )
		{
			m_EntityTypeVisitor.SetObjectsList( objectsList );
			VISITENTITYDATA visitEntityData;
			visitEntityData.searchString = typeNameQuery;
			visitEntityData.visitor = &m_EntityTypeVisitor;
			static DWORD msgHash_VisitEntities = CHashString(_T("VisitEntities")).GetUniqueID();
			retVal = m_ToolBox->SendMessage(msgHash_VisitEntities, sizeof(VISITENTITYDATA), &visitEntityData );
		}
	}
	return retVal;
}