///============================================================================
/// \file		NodeSystemManager.cpp
/// \brief		Implmentation of CNodeSystemManager Class
/// \date		09-22-2006
/// \author		Richard Nguyen
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

REGISTER_COMPONENT_SINGLETON(CNodeSystemManager);
REGISTER_MESSAGE_HANDLER( EnableNodeSystemRender, OnEnableNodeSystemRender, CNodeSystemManager );
REGISTER_MESSAGE_HANDLER( DisableNodeSystemRender, OnDisableNodeSystemRender, CNodeSystemManager );

// Default Constructor
CNodeSystemManager::CNodeSystemManager( void ) :
	m_hszNodeSystemComponentType( _T("CNodeSystem") ),
	m_hszNodeComponentType( _T("CNode") ),
	m_hszHeightNodeComponentType( _T("CHeightNode") ),
	m_hszNodeConnectionComponentType( _T("CNodeConnection") ),
	m_hszNodeFaceComponentType( _T("CNodeFace") ),
	m_hszNodeRenderObjectType( _T("CNodeRenderObject") ),
	m_hszNodeConnectionRenderObjectType( _T("CNodeConnectionRenderObject") ),
	CManager(_T("CNodeSystemManager"), NODESYSTEMPRIORITY, NODESYSTEMPRIORITY)
{
	m_ToolBox = EngineGetToolBox();

	RegisterLuaDefinitions();
	m_bRender = false;
}

// Default Destructor
CNodeSystemManager::~CNodeSystemManager( void )
{
	CManager::DeInit();
}

void CNodeSystemManager::RegisterLuaDefinitions()
{
	// TODO <rnguyen>: Uncomment this once we have Lua hooks to this component.
	//static DWORD msgHash_GetMasterScriptState = CHashString(_T( "GetMasterScriptState" )).GetUniqueID();
	//DWORD amessageHandle = m_ToolBox->SendMessage(msgHash_GetMasterScriptState, sizeof( lua_State * ), &m_LuaState );
	//if( amessageHandle != MSG_HANDLED)
	//{
	//	StdString error( "Error missing Master Script State Data\n" );

	//	// log error
	//	EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
	//	EngineGetToolBox()->Log(LOGWARNING, error);
	//}
	//else
	//{
	//	// register our additional structures/handlers with LUA master
	//	tolua_NodeSystemScript_open(m_LuaState);
	//}
}

IComponent *CNodeSystemManager::Create(int nArgs, va_list argptr)
{
	// Return the singleton instance of this class
  	return SINGLETONINSTANCE( CNodeSystemManager );
}

bool CNodeSystemManager::Init()
{
	return true;
}

void CNodeSystemManager::Update(DWORD tickCount)
{
	// update all render objects
	IDTOOBJECTMAP *objMap = GetObjectMap( &m_hszNodeRenderObjectType );
	if( objMap )
	{		
		IDTOOBJECTMAP::iterator objIter;
		for( objIter = objMap->begin(); objIter != objMap->end(); objIter++ )
		{
			CNodeRenderObject *pObject = (CNodeRenderObject*)objIter->second;
			pObject->Update();
		}
	}

	objMap = GetObjectMap( &m_hszNodeConnectionRenderObjectType );
	if( objMap )
	{		
		IDTOOBJECTMAP::iterator objIter;
		for( objIter = objMap->begin(); objIter != objMap->end(); objIter++ )
		{
			CNodeRenderObject *pObject = (CNodeRenderObject*)objIter->second;
			pObject->Update();
		}
	}
}

void CNodeSystemManager::Serialize( IArchive &ar )
{
}

IHashString* CNodeSystemManager::GetComponentType()
{
	static CHashString hszComponentType( "CNodeSystemManager" );
	return &hszComponentType;
}

//
// Deletes an Object and removes it from the manager
//
bool CNodeSystemManager::DeleteObject( IObject* Object )
{
	return CManager::DeleteObject(Object);
}

CNodeSystem *CNodeSystemManager::GetNodeSystem( IHashString *nodeSystemName )
{
	CNodeSystem *areturn = NULL;
	IObject *object = GetMapObject( nodeSystemName, &m_hszNodeSystemComponentType );
	if( object != NULL )
	{
		areturn = (CNodeSystem*)object;
	}

	return areturn;
}

CNode *CNodeSystemManager::GetNode( IHashString *nodeName )
{
	CNode *areturn = NULL;
	IObject *object = GetMapObject( nodeName, &m_hszNodeComponentType );
	if( object != NULL )
	{
		areturn = (CNode*)object;
	}

	return areturn;
}

CHeightNode *CNodeSystemManager::GetHeightNode( IHashString *heightNodeName )
{
	CHeightNode *areturn = NULL;
	IObject *object = GetMapObject( heightNodeName, &m_hszHeightNodeComponentType );
	if( object != NULL )
	{
		areturn = (CHeightNode*)object;
	}

	return areturn;
}

bool CNodeSystemManager::IsRendering()
{
	return m_bRender;
}

DWORD CNodeSystemManager::OnEnableNodeSystemRender( DWORD size, void *params )
{
	m_bRender = true;

	return MSG_HANDLED_STOP;
}

DWORD CNodeSystemManager::OnDisableNodeSystemRender( DWORD size, void *params )
{
	m_bRender = false;

	return MSG_HANDLED_STOP;
}

IObject *CNodeSystemManager::GetMapObject( IHashString *objectName, IHashString *componentType )
{
	IObject *areturn = NULL;
	IDTOOBJECTMAP *objectMap = GetObjectMap( componentType );

	if( objectName != NULL && objectMap != NULL )
	{
		IDTOOBJECTMAP::iterator objectsIterator = objectMap->find( objectName->GetUniqueID() );
		if (objectsIterator != objectMap->end())
		{
			areturn = objectsIterator->second;
		}
	}

	return areturn;
}