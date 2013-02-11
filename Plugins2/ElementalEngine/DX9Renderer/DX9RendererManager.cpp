#include "Stdafx.h"
#include "CHModelManager.h"


REGISTER_COMPONENT_SINGLETON( CHModelManager );

TOLUA_API int tolua_HierarchicalModelsLua_open (lua_State* tolua_S);

/// \brief The Default Constructor for CHModelManager
/// \return void
CHModelManager::CHModelManager() : CManager( RENDEROBJECTPRIORITY )//prioirity?
{
	m_ToolBox = EngineGetToolBox();	
	Init();
}


/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CHModelManager::Create(va_list argptr)
{
	return SINGLETONINSTANCE( CHModelManager );
}

/// \brief The Default Destructor for CHModelManager
/// \return void
CHModelManager::~CHModelManager( )
{
	
}
// Initializes the manager
bool CHModelManager::Init()
{
	m_ToolBox = EngineGetToolBox();

	lua_State * LuaState;
	// hook into LUA
	LuaState = NULL;

	if(m_ToolBox->SendMessage(_T("GetMasterScriptState"), sizeof(lua_State *), &LuaState) == MSG_HANDLED)
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
void CHModelManager::Update( void )
{	
	OBJECTLIST	objList;
	//send a dummy file so we get a callback for this first phase	
	OBJECTSET::iterator objIter;
	OBJECTSET * objSet = GetObjectSet( &CHashString( _T("CHierarchicalModel"))  );
	//update our objects
	if( objSet )
	{		
		IObject * destObject;
		for( objIter = objSet->begin();objIter != objSet->end(); objIter++ )
		{
			destObject = *objIter;
			destObject->Update();
		}
	}
}

/// \brief	DeInitializes the Cal3DModelManager
/// \return	bool: Success
bool CHModelManager::DeInit( void )
{	
	return true;
}