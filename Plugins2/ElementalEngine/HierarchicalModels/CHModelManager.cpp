///============================================================================
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

#include "Stdafx.h"
#include "CHModelManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON( CHModelManager );

TOLUA_API int tolua_HierarchicalModelsLua_open (lua_State* tolua_S);

/// \brief The Default Constructor for CHModelManager
/// \return void
CHModelManager::CHModelManager() : 
	CManager(_T("CHModelManager"), RENDEROBJECTPRIORITY, HIEARCHICALMODELPRIORITY )
{
	m_ToolBox = EngineGetToolBox();	
	m_ManagerComponentType = _T("CHModelManager");
	Init();
}


/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CHModelManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CHModelManager );
}

/// \brief The Default Destructor for CHModelManager
/// \return void
CHModelManager::~CHModelManager( )
{
	DeInit();
}
// Initializes the manager
bool CHModelManager::Init()
{
	m_ToolBox = EngineGetToolBox();

	lua_State * LuaState;
	// hook into LUA
	LuaState = NULL;

	static DWORD msgHash_GetMasterScriptState = CHashString(_T("GetMasterScriptState")).GetUniqueID();
	if(m_ToolBox->SendMessage(msgHash_GetMasterScriptState, sizeof(lua_State *), &LuaState) == MSG_HANDLED)
	{
		// register our additional structures/handlers with LUA master
		tolua_HierarchicalModelsLua_open(LuaState);
	}
	else
	{
		StdString error;
		error = _T("Error missing Master Script State Data\n");

		// log error
		EngineGetToolBox()->SetErrorValue(WARN_INVALID_OPERATION);
		EngineGetToolBox()->Log(LOGWARNING, error);
    }
	return true;
}

/// \brief	Update function for the Cal3DModelManager
///			Renders immediately, the queued list of renderobjects
/// \return	void
void CHModelManager::Update( DWORD tickCount )
{
	static CHashString hierTypeName(_T("CHierarchicalModel"));
	OBJECTLIST	objList;
	//send a dummy file so we get a callback for this first phase	
	IDTOOBJECTMAP::iterator objIter;
	IDTOOBJECTMAP * objMap = GetObjectMap( &hierTypeName );
	//update our objects
	if( objMap )
	{		
		IObject * destObject;
		for( objIter = objMap->begin();objIter != objMap->end(); ++objIter )
		{
			destObject = objIter->second;
			destObject->Update();
		}
	}
}

IHashString* CHModelManager::GetComponentType()
{
	return &m_ManagerComponentType;
}


bool CHModelManager::AddObject(IObject *Object, IHashString *group)
{
	CManager::AddObject(Object, group);

	return true;
}
	
bool CHModelManager::DeleteObject(IObject *Object)
{
	CManager::DeleteObject(Object);
	return true;
}

HierarchicalModelType * CHModelManager::GetHierarchicalModelType(IHashString *hszModelName)
{
	if (hszModelName)
	{
		static CHashString hszHierarchicalModelType(_T("HierarchicalModelType"));
		IDTOOBJECTMAP *typeMap = GetObjectMap(&hszHierarchicalModelType);
		if (typeMap)
		{
			IDTOOBJECTMAP::iterator typeIter = typeMap->find(hszModelName->GetUniqueID());
			if (typeIter != typeMap->end())
			{
				return dynamic_cast<HierarchicalModelType *>(typeIter->second);
			}
		}
	}
	return NULL;
}
