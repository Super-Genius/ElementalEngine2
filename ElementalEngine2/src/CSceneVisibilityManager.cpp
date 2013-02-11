///============================================================================
/// \file	CSceneVisibility.cpp
/// \brief	Implementation of the CSceneVisibility
///			Inherits from IManager
/// \date	3/9/2005
/// \author	Marvin Gouw
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

#include "StdAfx.h"
#include "CSceneVisibilityManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON( CSceneVisibilityManager );
REGISTER_MESSAGE_HANDLER(Start, OnStart, CSceneVisibilityManager);
REGISTER_MESSAGE_HANDLER(Stop, OnStop, CSceneVisibilityManager);
REGISTER_MESSAGE_HANDLER(SetActiveScene, OnSetActiveScene, CSceneVisibilityManager );
REGISTER_MESSAGE_HANDLER(SetSceneRenderContext, OnSetSceneRenderContext, CSceneVisibilityManager );

/// the singleton instance used in static functions for the 3dobject prepscreen callback
CSceneVisibilityManager* CSceneVisibilityManager::m_SingletonInstance = NULL;

/// \brief The Default Constructor for CSceneVisibilityManager
/// \return void
CSceneVisibilityManager::CSceneVisibilityManager() : 
	CManager(_T("CSceneVisibilityManager"), SCENEVISPRIORITY ),
	m_ccamTypeName(_T("CCamera"))
{
	m_CurrentCamera = NULL;
	m_pActiveSceneRenderContext = NULL;
	m_SingletonInstance = this;

	m_bIsRunning = true;
}


/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CSceneVisibilityManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CSceneVisibilityManager );
}

/// \brief The Default Destructor for CSceneVisibilityManager
/// \return void
CSceneVisibilityManager::~CSceneVisibilityManager( )
{

}



static vector< ILightObject * > m_SceneLights;
/// \brief	Update function for Scene manager
///			computes what is visible and not visible and sends a list to the
///			render manager
/// \return	void
void CSceneVisibilityManager::Update(  DWORD tickCount  )
{
	if( m_bIsRunning )
	{
		ADDRENDERLISTPARAMS		addRenderListMessage;
		addRenderListMessage.renderContext = m_pActiveSceneRenderContext;
		DWORD retValue;

		m_SceneLights.clear();

		static DWORD msgHash_AddVisibleEntitiesToScene = CHashString(_T("AddVisibleEntitiesToScene")).GetUniqueID();

		// need to get frustum from the camera
		CFrustum *camFrustum;
		CHashString hszCameraName;
		CONTEXTCAMPARAMS ccp;
		ccp.pRC = m_pActiveSceneRenderContext;
		ccp.szCameraName = &hszCameraName;
		static DWORD msgHash_GetActiveCamera = CHashString(_T("GetActiveCamera")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetActiveCamera, sizeof( CONTEXTCAMPARAMS ), &ccp);
		if( ccp.szCameraName )
		{
			static DWORD msgHash_GetCameraFrustum = CHashString(_T("GetCameraFrustum")).GetUniqueID();
			retValue = m_ToolBox->SendMessage(msgHash_GetCameraFrustum, sizeof(CFrustum**), &camFrustum, 
				ccp.szCameraName,  &m_ccamTypeName);
		}

		if (retValue == MSG_NOT_HANDLED || retValue == MSG_ERROR)
		{
			// failed to get a frustum, don't try to cull anything
			return; 
		}

		PERFORMANCE_PROFILER_START( CSceneVisibilityManager_Update_VisitQuadTree )
		
		// Do a frustum visit on the scene.
		CSceneCullVisitor cullVisitorPattern;

		VISITSCENEFRSUTUMPARAMS vsp;
		vsp.visitor = (IVisitor*)&cullVisitorPattern;
		vsp.frustum = camFrustum;
		vsp.sortID = RENDEROBJECT3D;
		static DWORD msg_VisitSceneFrustum = CHashString(_T("VisitSceneFrustum")).GetUniqueID();
		m_ToolBox->SendMessage( msg_VisitSceneFrustum, sizeof(VISITSCENEFRSUTUMPARAMS), &vsp, NULL, NULL );

		PERFORMANCE_PROFILER_STOP( CSceneVisibilityManager_Update_VisitQuadTree );

#if defined _PROFILE || defined _DEBUG
		static DWORD msgHash_UpdatePerformanceProfiler = CHashString(_T("UpdatePerformanceProfiler")).GetUniqueID();
		CHashString hszType(_T("PVS"));
		PERFORMANCEPROFILERUPDATE msg;
		msg.displayType = NAME_NUMBER;
		msg.type = &hszType;

		CHashString hszLightsName(_T("Lights"));
		msg.name = &hszLightsName;
		msg.time = (float)m_SceneLights.size();
		EngineGetToolBox()->SendMessage(msgHash_UpdatePerformanceProfiler, sizeof(msg), &msg );

		CHashString hszOpaquesName(_T("Opaque Objects"));
		msg.name = &hszOpaquesName;
		msg.time = (float)cullVisitorPattern.GetOpaqueObjects()->size();
		EngineGetToolBox()->SendMessage(msgHash_UpdatePerformanceProfiler, sizeof(msg), &msg );

		CHashString hszAlphasName(_T("Alpha Objects"));
		msg.name = &hszAlphasName;
		msg.time = (float)cullVisitorPattern.GetAlphaObjects()->size();
		EngineGetToolBox()->SendMessage(msgHash_UpdatePerformanceProfiler, sizeof(msg), &msg );
#endif

		PERFORMANCE_PROFILER_START( CSceneVisibilityManager_Update_AddAlpha );
		//set first sort method for alphas
		addRenderListMessage.dwListPriorityKey = RENDERPRIORITY_ALPHA;//Alpha
		//addRenderListMessage.dwSortMethod = SORT_REVERSEDISTANCE;
		addRenderListMessage.dwSortMethod = SORT_FIFO;
		addRenderListMessage.objList = cullVisitorPattern.GetAlphaObjects();	
		addRenderListMessage.dwFuncCallback = NULL;//CSceneVisibilityManager::Prep3DObjectsRender;	
		m_ToolBox->SendMessage(msgHash_AddVisibleEntitiesToScene, sizeof(addRenderListMessage), &addRenderListMessage, NULL, NULL );
		PERFORMANCE_PROFILER_STOP( CSceneVisibilityManager_Update_AddAlpha );

		PERFORMANCE_PROFILER_START( CSceneVisibilityManager_Update_AddOpaque );
		//set second sort method for opaque
		addRenderListMessage.dwListPriorityKey = RENDERPRIORITY_OPAQUE;//Alpha
		addRenderListMessage.dwSortMethod = SORT_MODEL;
		addRenderListMessage.objList = cullVisitorPattern.GetOpaqueObjects();	
		addRenderListMessage.dwFuncCallback = NULL;//CSceneVisibilityManager::Prep3DObjectsRender;
		m_ToolBox->SendMessage(msgHash_AddVisibleEntitiesToScene, sizeof(addRenderListMessage), &addRenderListMessage, NULL, NULL );
		PERFORMANCE_PROFILER_STOP( CSceneVisibilityManager_Update_AddOpaque );

		
	}
}

DWORD CSceneVisibilityManager::OnStart(DWORD size, void *data)
{
	m_bIsRunning = true;
	
	return MSG_HANDLED_PROCEED;
}

DWORD CSceneVisibilityManager::OnStop(DWORD size, void *data)
{
	m_bIsRunning = false;
	
	return MSG_HANDLED_PROCEED;
}

DWORD CSceneVisibilityManager::OnSetActiveScene(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(GETSETACTIVESCENE), size);
	GETSETACTIVESCENE *pParams = (GETSETACTIVESCENE*)params;

	assert( pParams->pSceneName != NULL );
	if (pParams->pSceneName == NULL)
		return MSG_ERROR;

	// check if this scene has had a render context assigned to it
	DWORD sceneNameID = pParams->pSceneName->GetUniqueID();
	SCENETORENDERCONTEXTMAP::iterator itrScene = m_SceneRenderContexts.find( sceneNameID );
	if (itrScene == m_SceneRenderContexts.end())
	{
		// use the default render context
		m_pActiveSceneRenderContext = NULL;
	}
	else
	{
		// use the assigned render context
		m_pActiveSceneRenderContext = m_SceneRenderContexts[sceneNameID];
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CSceneVisibilityManager::OnSetSceneRenderContext(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(SETSCENERENDERCONTEXTPARAMS), size);
	SETSCENERENDERCONTEXTPARAMS *pParams = (SETSCENERENDERCONTEXTPARAMS*)params;

	assert( pParams->pSceneName != NULL );
	if (pParams->pSceneName == NULL)
		return MSG_ERROR;

	DWORD sceneNameID = pParams->pSceneName->GetUniqueID();
	m_SceneRenderContexts[sceneNameID] = pParams->pRenderContext;
	return MSG_HANDLED_STOP;
}