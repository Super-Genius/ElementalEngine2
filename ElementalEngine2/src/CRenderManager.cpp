///=====================================================================
/// \file	CRenderManager.cpp
/// \brief	Implementation of the RenderManager
///			Inherits from IManager
/// \date	3/9/2005
/// \author	Marvin Gouw
///=====================================================================

#include "StdAfx.h"
#include "CController.H"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON( CRenderManager );

REGISTER_MESSAGE_HANDLER(AddRenderList, OnAddRenderList, CRenderManager);
REGISTER_MESSAGE_HANDLER(RenderListToTexture, OnRenderListToTexture, CRenderManager);
REGISTER_MESSAGE_HANDLER(CreateRenderContext, OnCreateRenderContext, CRenderManager);
REGISTER_MESSAGE_HANDLER(DeleteRenderContext, OnDeleteRenderContext, CRenderManager);
REGISTER_MESSAGE_HANDLER(DrawToRenderContext, OnDrawToRenderContext, CRenderManager);
REGISTER_MESSAGE_HANDLER(DestroyRenderer, OnDestroyRenderer, CRenderManager);
REGISTER_MESSAGE_HANDLER(GetVideoSize, OnGetVideoSize, CRenderManager);
REGISTER_MESSAGE_HANDLER(SetWindowSize, OnSetWindowSize, CRenderManager);
REGISTER_MESSAGE_HANDLER(GetWindowSize, OnGetWindowSize, CRenderManager);
REGISTER_MESSAGE_HANDLER(SetViewPort, OnSetViewPort, CRenderManager);
REGISTER_MESSAGE_HANDLER(SetVideoSize, OnSetVideoSize, CRenderManager);
REGISTER_MESSAGE_HANDLER(SetFullScreen, OnSetFullScreen, CRenderManager); 
//REGISTER_MESSAGE_HANDLER(AddRenderObjectToMap, OnAddRenderObjectToMap, CRenderManager);
//REGISTER_MESSAGE_HANDLER(GetRenderObject, OnGetRenderObject, CRenderManager);
REGISTER_MESSAGE_HANDLER(SetRendererMouse, OnSetRenderMouse, CRenderManager );
REGISTER_MESSAGE_HANDLER(GetRendererMouse, OnGetRenderMouse, CRenderManager );
REGISTER_MESSAGE_HANDLER(CreateMeshObject, OnCreateMeshObject, CRenderManager );
REGISTER_MESSAGE_HANDLER(GetRendererProfilingSnapshot, OnGetRendererProfilingSnapshot, CRenderManager);
REGISTER_MESSAGE_HANDLER(GetObjectCullDistance, OnGetObjectCullDistance, CRenderManager);
REGISTER_MESSAGE_HANDLER(SetObjectCullDistance, OnSetObjectCullDistance, CRenderManager);
REGISTER_MESSAGE_HANDLER(Start, OnStart, CRenderManager);
REGISTER_MESSAGE_HANDLER(Stop, OnStop, CRenderManager);
REGISTER_MESSAGE_HANDLER(SetWireFrameMode, OnSetWireFrameMode, CRenderManager ); 
REGISTER_MESSAGE_HANDLER( CreateController, OnCreateController, CRenderManager );
REGISTER_MESSAGE_HANDLER( DestroyController, OnDestroyController, CRenderManager );
REGISTER_MESSAGE_HANDLER( CreateSceneSettings, OnCreateSceneSettings, CRenderManager );
REGISTER_MESSAGE_HANDLER( DestroySceneSettings, OnDestroySceneSettings, CRenderManager );
REGISTER_MESSAGE_HANDLER( CreateGeometry, OnCreateGeometry, CRenderManager );
REGISTER_MESSAGE_HANDLER( DestroyGeometry, OnDestroyGeometry, CRenderManager );
REGISTER_MESSAGE_HANDLER( AddPostProcessEffect, OnAddPostProcessEffect, CRenderManager );
REGISTER_MESSAGE_HANDLER( RemovePostProcessEffect, OnRemovePostProcessEffect, CRenderManager );
REGISTER_MESSAGE_HANDLER_PRIORITY( RenderUpdate, OnRenderUpdate, CRenderManager, 0xffff0000 );
REGISTER_MESSAGE_HANDLER(GetRenderFlagValue, OnGetRenderFlagValue, CRenderManager );
REGISTER_MESSAGE_HANDLER(SetNullRenderer, OnSetNullRenderer, CRenderManager );
REGISTER_MESSAGE_HANDLER(CreateMaterialOverride, OnCreateMaterialOverride, CRenderManager );
REGISTER_MESSAGE_HANDLER(AddVisibleEntitiesToScene, OnAddVisibleEntitiesToScene, CRenderManager );
REGISTER_MESSAGE_HANDLER(GetDepthTargetAsTexture, OnGetDepthTargetAsTexture, CRenderManager );
REGISTER_MESSAGE_HANDLER(SetFogValue, OnSetFogValue, CRenderManager );
REGISTER_MESSAGE_HANDLER(GetFogValue, OnGetFogValue, CRenderManager );
REGISTER_MESSAGE_HANDLER(SetColorCorrection, OnSetColorCorrection, CRenderManager );

/// \brief Initializes the camera parameters
DWORD CRenderManager::OnGetRenderMouse(DWORD size, void *params)
{
	// verift and cast the message
	MOUSECOORDS *message;
	VERIFY_MESSAGE_SIZE( size, sizeof(MOUSECOORDS) );
	message = (MOUSECOORDS *)params;
	message->m_lXPos = (DWORD)m_MouseCoord.x;
	message->m_lYPos = (DWORD)m_MouseCoord.y;
	return MSG_HANDLED_PROCEED;
}

/// \brief Initializes the camera parameters
DWORD CRenderManager::OnSetRenderMouse(DWORD size, void *params)
{
	// verift and cast the message
	MOUSECOORDS *message;
	VERIFY_MESSAGE_SIZE( size, sizeof(MOUSECOORDS) );
	message = (MOUSECOORDS *)params;
	m_MouseCoord.x = (float)message->m_lXPos;
	m_MouseCoord.y = (float)message->m_lYPos;
	return MSG_HANDLED_PROCEED;
}

/// \brief The Default Constructor for CRenderManager
/// \return void
CRenderManager::CRenderManager() : 
	CManager(_T("CRenderManager"), RENDERPRIORITY),
	m_ccameraTypeName(_T("CCamera"))
{
	m_LuminanceTime = 0;
	m_LuminanceShiftTime = DEFAULT_LUMINANCE_SHIFT_TIME;
	m_AverageLuminosity[0] = 0;
	m_AverageLuminosity[0] = 0;
	m_CurLuminanceSlot = 0;
	m_IRenderer = NULL;
	m_NextContextID = 0;
	m_DepthTexture = NULL;
	m_bUseDepthTextureMRT = false;
	
	m_FogStart = 8000;
	m_FogDistance = 8000;
	m_FogColor = Vec3( .8f, .8f, 1.f );
	// we don't need pump messages for this manager?
	//EnablePumpHandler(RENDERPRIORITY);

	m_fObjectCullDistance = 999999.0f;

	m_bIsRunning = true;
	//instantiate null context
	m_bNullRenderer = false;
}

/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CRenderManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CRenderManager );
}

/// \brief The Default Destructor for CRenderManager
/// \return void
CRenderManager::~CRenderManager( )
{
	//loop through contexts and release resources
	IRenderContext * iContext;
	RENDERCONTEXTLIST::iterator iter = m_RenderContextList.begin();
	for( ;iter != m_RenderContextList.end(); iter++ )
	{
		iContext = (*iter).second;
		iContext->DestroyContext();
	}

}

DWORD CRenderManager::OnDeleteRenderContext(DWORD size, void *params)
{
	UINT contextID;
	IRenderContext * iContext;

	contextID = *(UINT *)params;
	VERIFY_MESSAGE_SIZE(sizeof(UINT), size);
	RENDERCONTEXTLIST::iterator iter = m_RenderContextList.find( contextID );
	// try to delete root context?
	if ((contextID == 0) && (m_RenderContextList.size() != 1))
	{
		return MSG_ERROR;
	}

	if( iter != m_RenderContextList.end() )
	{
		//remove
		iContext = (*iter).second;
		iContext->DestroyContext();
		m_RenderContextList.erase( iter );

		m_CurrentSceneContextList.erase( iContext );
	}

	m_RenderObjectLists.erase( iContext );

	return MSG_HANDLED_PROCEED;
	
}

DWORD CRenderManager::OnDrawToRenderContext(DWORD size, void *params)
{
	UINT contextID;
	IRenderContext * iContext;

	contextID = *((UINT *)params);
	VERIFY_MESSAGE_SIZE(sizeof(UINT), size);
	RENDERCONTEXTLIST::iterator iter = m_RenderContextList.find( contextID );
	if( iter != m_RenderContextList.end() )
	{
		iContext = (*iter).second;
		m_CurrentRenderContexts.push( iContext );
	}	
	return MSG_HANDLED_PROCEED;
}

DWORD CRenderManager::OnSetWindowSize( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(VIEWPORTPARAMS));
	VIEWPORTPARAMS *rect = (VIEWPORTPARAMS*) params;

	if( m_IRenderer )
	{
		m_IRenderer->SetWindowDimensions( rect->width, rect->height );
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CRenderManager::OnGetWindowSize( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(VIEWPORTPARAMS));
	VIEWPORTPARAMS *rect = (VIEWPORTPARAMS*) params;
	rect->offsetx = 0;
	rect->offsety = 0;
	if( m_IRenderer )
	{
		m_IRenderer->GetWindowDimensions( rect->width, rect->height );
	}

	return MSG_HANDLED_PROCEED;
}

DWORD CRenderManager::OnSetViewPort( DWORD size, void *params )
{
	VIEWPORTPARAMS *setviewparams;
	
	setviewparams = (VIEWPORTPARAMS *)params;
	VERIFY_MESSAGE_SIZE(sizeof(VIEWPORTPARAMS), size);

	if( !m_IRenderer || !m_IRenderer->IsInitialized() )
	{
		//Renderer not even initialized!!!
		return MSG_ERROR;
	}
	/*else if( !setviewparams->immediatemode )
	{					
		m_IRenderer->SetViewport( setviewparams->offsetx, setviewparams->offsety, 
								setviewparams->width, setviewparams->height );
		return MSG_HANDLED_PROCEED;
	}*/
	else
	{
		m_IRenderer->SetViewport(setviewparams->offsetx, setviewparams->offsety, setviewparams->width, setviewparams->height );
		return MSG_HANDLED_PROCEED;
	}
}
DWORD CRenderManager::OnGetVideoSize( DWORD size, void *params )
{
	VIEWPORTPARAMS* data;
	data = (VIEWPORTPARAMS *)params;
	VERIFY_MESSAGE_SIZE(sizeof(VIEWPORTPARAMS), size);
	
	if( m_IRenderer )
	{
		m_IRenderer->GetViewDimensions( data->width, data->height );
	}
	return MSG_HANDLED_PROCEED;
}

DWORD CRenderManager::OnSetVideoSize( DWORD size, void *params )
{
	VIEWPORTPARAMS* data;
	data = (VIEWPORTPARAMS *)params;
	VERIFY_MESSAGE_SIZE(sizeof(VIEWPORTPARAMS), size);
	
	if( m_IRenderer )
	{
		m_IRenderer->ResizeScreen( data->width, data->height );
	}
	return MSG_HANDLED_PROCEED;
}

DWORD CRenderManager::OnSetFullScreen( DWORD size, void *params )
{	
	if( !m_IRenderer )
	{		
		return MSG_ERROR;
	}

	m_IRenderer->SetFullScreen();

	return MSG_HANDLED_PROCEED;
}

DWORD CRenderManager::OnCreateRenderContext(DWORD size, void *params)
{
	if( !m_IRenderer )
	{
		static CHashString DX9RendererName("CDX9Renderer");
		m_IRenderer = dynamic_cast< IRenderer* > ( m_ToolBox->CreateComponent( &DX9RendererName, 0) );
		assert( m_IRenderer );
	}

	CREATERENDERCONTEXTPARAMS * pCreateContextParams;
	pCreateContextParams = (CREATERENDERCONTEXTPARAMS *)params;
	VERIFY_MESSAGE_SIZE(sizeof(CREATERENDERCONTEXTPARAMS), size);

	if( !m_IRenderer )
	{		
		return MSG_ERROR;
	}
	if( m_NextContextID == 0 )
	{	
		//Initialize the Renderer
		if( !m_IRenderer->Initialize(  pCreateContextParams->hwindow,
										pCreateContextParams->fullscreen,
										pCreateContextParams->width,
										pCreateContextParams->height,
										pCreateContextParams->depthbits,
										pCreateContextParams->colorbits ) )
		{
			//could not create renderer!
			return MSG_ERROR;
		}
	}	

	// only create render context if not the main screen
	if ( m_NextContextID != 0 )
	{
		//need a render context info block to hold height/width attributes that
		IRenderContext *	newrc;	
		newrc = m_IRenderer->CreateNewContext( pCreateContextParams->hwindow,
												pCreateContextParams->width,
												pCreateContextParams->height,
												pCreateContextParams->depthbits,
												pCreateContextParams->colorbits );
		if( !newrc )
		{
			return MSG_ERROR;
		}
		//Otherwise Add
		m_RenderContextList.insert( RENDERCONTEXTLIST_ELEMENT( m_NextContextID, newrc ) );
		pCreateContextParams->context = newrc;
		pCreateContextParams->uiRenderContextID = m_NextContextID;
	}
	//Return the context ID
//	*pCreateContextParams->retVal_RenderID = m_NextContextID;
	
	m_NextContextID++;
	return MSG_HANDLED_PROCEED;
}

/// \brief Destroys the Vison Renderer
DWORD CRenderManager::OnDestroyRenderer(DWORD size, void *params)
{
	// Renderer is in a seperate DLL Process space now, it should handle it's own deinits
	m_IRenderer = NULL;
	return MSG_HANDLED_PROCEED;
}

// Create an Object of known types
DWORD CRenderManager::OnAddRenderList(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(ADDRENDERLISTPARAMS), size);
	ADDRENDERLISTPARAMS *renderparams = (ADDRENDERLISTPARAMS *)params;

	AddListObjects( *(renderparams->objList), renderparams->dwListPriorityKey, renderparams->dwFuncCallback,
						renderparams->dwFuncPostRenderCallback, renderparams->renderContext );

	return MSG_HANDLED_PROCEED;
}

/// \brief	Update function for the Render Manager
///			Renders immediately, the queued list of renderobjects
/// \return	void
void CRenderManager::Update(  DWORD tickCount  )
{
//	PERFORMANCE_PROFILER_AUTO( RenderManager_Update );

	if (!m_IRenderer || !m_IRenderer->IsInitialized() || !m_bIsRunning)
		return;

	CONTEXTTOPRIORITYPASSMAP::iterator itrContext = m_RenderObjectLists.begin();
	while (itrContext != m_RenderObjectLists.end())
	{
		IRenderContext *pContext = itrContext->first;

		//Set our camera
		CHashString hszCameraName;
		CONTEXTCAMPARAMS ccp;
		ccp.pRC = pContext;
		ccp.szCameraName = &hszCameraName;
		static DWORD msgHash_GetActiveCamera = CHashString(_T("GetActiveCamera")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetActiveCamera, sizeof(ccp), &ccp );
		static DWORD msgHash_ActivateCamera = CHashString(_T("ActivateCamera")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_ActivateCamera, NULL, NULL, ccp.szCameraName, &m_ccameraTypeName );

		m_IRenderer->RenderToContext( pContext );

		UINT r=150, g=150, b=150, a=0; // pre-defined clear color
		if (pContext)
			pContext->GetBGColor(&r, &g, &b, &a);			
		m_IRenderer->SetBackgroundColor(r, g, b, a);

		PERFORMANCE_PROFILER_START(RenderManager_BeginScene);
		
		m_IRenderer->BeginScene( false );
		if( m_bUseDepthTextureMRT )//post process
		{
			if( !m_DepthTexture )
			{
				//create the depth target texture
				CreateDepthTargetTexture();
			}
			if( m_DepthTexture )
			{
				BindDepthTextureMRT();
			}
		}		
		m_IRenderer->ClearScreen( true, true );
		PERFORMANCE_PROFILER_STOP(RenderManager_BeginScene);

		// render pass lists
		PRIORITYTOPASSLISTMAP &priorityMap = itrContext->second;
		for (PRIORITYTOPASSLISTMAP::iterator itrPriority = priorityMap.begin(); 
			itrPriority != priorityMap.end(); 
			itrPriority++)
		{
			UINT iPriority = itrPriority->first;

			// stop at the front buffer priority so we can end the scene and do post processing effects
			if (iPriority == RENDERPRIORITY_FRONTBUFFER)
				break;

			PASSLIST &passList = itrPriority->second;
			RenderPassList( passList, true );
		}

		PERFORMANCE_PROFILER_START(RenderManager_EndScene);
		m_IRenderer->EndScene();
		PERFORMANCE_PROFILER_STOP(RenderManager_EndScene);

		// Post Processing
		PERFORMANCE_PROFILER_START(RenderManager_PostProcess);
		DoPostProcess( pContext );
		PERFORMANCE_PROFILER_STOP(RenderManager_PostProcess);

		// Render the front buffer after the post process
		PRIORITYTOPASSLISTMAP::iterator itrFrontBufferPriority = priorityMap.find( RENDERPRIORITY_FRONTBUFFER );
		if (itrFrontBufferPriority != priorityMap.end())
		{
			m_IRenderer->BeginScene( true );
			while (itrFrontBufferPriority != priorityMap.end())
			{
				PASSLIST &passList = itrFrontBufferPriority->second;
				RenderPassList( passList, false );
				itrFrontBufferPriority++;
			}
			m_IRenderer->EndScene();
		}

		m_IRenderer->Present( pContext );

		itrContext++;
	}

	m_RenderObjectLists.clear();
}

void CRenderManager::RenderPassList( PASSLIST &passList, bool bSort )
{
	//loop through passes
	UINT numPasses = (UINT)passList.size();
	for( UINT i = 0; i < numPasses; i++ )
	{
		SINGLEPASS *pPass = &passList[i];

		// pass callback
		if (pPass->m_Callback)
			pPass->m_Callback();

		if (bSort)
			sort( pPass->m_List.begin(), pPass->m_List.end(), RENDERPAIRCOMPARE() );

		// render each object in pass list
		if (!m_bNullRenderer)
		{
			RENDEROBJECTLIST::iterator itrObject;
			for( itrObject = pPass->m_List.begin();
				itrObject != pPass->m_List.end();
				++itrObject )
			{
				(*itrObject).Render( 0 );
			}
		}
		// post pass callback
		if (pPass->m_PostCallback)
			pPass->m_PostCallback();

		size_t cursize = pPass->m_List.size();
		pPass->m_List.clear();
		pPass->m_List.reserve( cursize );
	}
}

/// \brief	Adds the list of objects to the RENDERLIST
/// \return	void
bool CRenderManager::AddListObjects( OBJECTLIST &VisibleObjList, DWORD priorityKey, void (*funcCallback)(),
									void (*funcPostRenderCallback)(), IRenderContext * pContext )
{
	Vec3 pos, camdistance;
	float radius;
	Vec3 campos( 0.0, 0.0, 0.0 );
	//Get the camera position	
	CONTEXTCAMVECTORPARAMS ccvp;
	ccvp.vdata = &campos;
	ccvp.pRC = pContext;
	static DWORD msgHash_GetActiveCameraPosition = CHashString(_T("GetActiveCameraPosition")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetActiveCameraPosition, sizeof(ccvp), &ccvp );

	SINGLEPASS pass;
	pass.m_Callback = funcCallback;
	pass.m_PostCallback = funcPostRenderCallback;

	// add objects to pass
	RenderPair newPair;
	OBJECTLIST::iterator RenderObjIter = VisibleObjList.begin();
	for( ; RenderObjIter != VisibleObjList.end(); ++RenderObjIter )
	{
		IRenderObject *pCurrentObject = (IRenderObject*)(*RenderObjIter);

		// distance to camera
		pCurrentObject->GetBoundingSphere( pos, radius );
		camdistance = pos - campos;
		newPair.fDistanceToCamera = camdistance.x*camdistance.x + camdistance.y*camdistance.y + camdistance.z*camdistance.z;
		newPair.iRenderPriority = pCurrentObject->GetRenderPriority();

		UINT iGeometryCount = pCurrentObject->GetNumGeometry();
		if (iGeometryCount > 0)
		{
			// add as individual geometry chunks
			for (UINT i=0; i < iGeometryCount; i++)
			{
				IGeometry *pGeometry = pCurrentObject->GetGeometry(i);
				newPair.SetGeom( pGeometry );
				pass.m_List.push_back( newPair );
			}
		}
		else
		{
			// add as an entire render object
			newPair.SetRenderObject( pCurrentObject );
			pass.m_List.push_back( newPair );
		}
	}

	// add new pass
	PRIORITYTOPASSLISTMAP &priorityMap = m_RenderObjectLists[pContext];
	priorityMap[priorityKey].push_back( pass );

	return true;
}

/// \brief	DeInitializes the Render Manager
/// \return	bool: Success
bool CRenderManager::DeInit( void )
{
	m_CurrentCamera = NULL;
	m_IRenderer = NULL;
	m_PostProcessContextShaders.clear();
	return CManager::DeInit();
}

DWORD CRenderManager::OnCreateMeshObject(DWORD size, void *params)
{
	CREATEMESHOBJECTPARAMS * createmeshobjectparams;
	VERIFY_MESSAGE_SIZE( sizeof(CREATEMESHOBJECTPARAMS), size );
	createmeshobjectparams = (CREATEMESHOBJECTPARAMS*)params;
	//create it
	static CHashString msgCreateObject(_T("CMeshObject"));
	createmeshobjectparams->MeshObjectInterface
		= dynamic_cast< IMeshObject*>( m_ToolBox->CreateComponent( &msgCreateObject, NULL, NULL ) );
	return MSG_HANDLED_PROCEED;
}

DWORD CRenderManager::OnRenderListToTexture(DWORD size, void *params)
{
	RENDERLISTTOTEXTUREPARAMS * renderparams;
	Matrix4x4 * matworld = NULL;
	renderparams = (RENDERLISTTOTEXTUREPARAMS *)params;
	VERIFY_MESSAGE_SIZE(sizeof(RENDERLISTTOTEXTUREPARAMS), size);

	if(	renderparams 
		&& renderparams->RenderTexture
		&& m_IRenderer )
	{
		CHashString hszCameraName;
		CONTEXTCAMPARAMS oldCCP;
		oldCCP.pRC = NULL;		
		oldCCP.szCameraName = &hszCameraName;
		static DWORD msgHash_GetActiveCamera = CHashString(_T("GetActiveCamera")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GetActiveCamera, sizeof(CONTEXTCAMPARAMS), &oldCCP );
		CHashString oldCameraName;
		bool oldcamerawasset = false;
		if( oldCCP.szCameraName != NULL )
		{
			oldcamerawasset = true;
			oldCameraName.Init( oldCCP.szCameraName->GetString() );
		}

		m_IRenderer->RenderToTexture( renderparams->RenderTexture );

		//Set our camera
		if( renderparams->Camera )
		{
static DWORD msgHash_ActivateCamera = CHashString(_T("ActivateCamera")).GetUniqueID();
            //m_ToolBox->SendMessage(msgHash_ActivateCamera, NULL, NULL, renderparams->Camera );
			CONTEXTCAMPARAMS ccp;
			ccp.pRC = NULL;
			ccp.szCameraName = renderparams->Camera;
			static DWORD msgHash_SetActiveCamera = CHashString(_T("SetActiveCamera")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SetActiveCamera, sizeof( CONTEXTCAMPARAMS ), &ccp );
		}
		//test
		//Make sure depth texture is not bound
		if( m_bUseDepthTextureMRT )//post process
		{
			if( m_DepthTexture )
			{
				UnbindDepthTextureMRT();
			}
		}
		m_IRenderer->BeginScene( false );
		m_IRenderer->ClearScreen( renderparams->clearDepth, renderparams->clearTarget );
		//Set our camera
		if( renderparams->Camera )
		{
static DWORD msgHash_ActivateCamera = CHashString(_T("ActivateCamera")).GetUniqueID();
            m_ToolBox->SendMessage(msgHash_ActivateCamera, NULL, NULL, renderparams->Camera, &m_ccameraTypeName );	
		}
		//render objects here:
		{
			IRenderObject * CurrentObject = NULL;
			OBJECTLIST::iterator iter = renderparams->objList->begin();
			for( ;iter != renderparams->objList->end(); ++iter )
			{
				//render
				CurrentObject =  (IRenderObject*)(*iter);	
				int numpass = CurrentObject->GetNumPass();
				if( CurrentObject->GetNumGeometry() > 0 )
				{
					for( int i = 0; i < (int)CurrentObject->GetNumGeometry(); i++ )
					{
						IGeometry * geom = CurrentObject->GetGeometry( i );
						if( geom )
						{
							IController * control = geom->GetController();
							if( control )
							{
								matworld =  control->GetMatrix();
								m_IRenderer->SetMatrix( WORLD_MATRIX, (CONST FLOAT*)matworld->GetMatrix() );
							}
							geom->Render( renderparams->effectOverride );
						}
					}
				}
				else
				{
					//must be at least one pass!
					for( int i = 0; i < numpass;i++ )
					{
						CurrentObject->Render( (UINT)i, renderparams->effectOverride );
					}
				}
			}
		}
		m_IRenderer->EndScene();
		m_IRenderer->RenderToTexture( NULL );
		//reset old camera
		if( oldcamerawasset )
		{			
			static DWORD msgHash_ActivateCamera = CHashString(_T("ActivateCamera")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_ActivateCamera, NULL, NULL, &oldCameraName, &m_ccameraTypeName );

			CONTEXTCAMPARAMS ccp;
			ccp.pRC = NULL;
			ccp.szCameraName = &oldCameraName;
			static DWORD msgHash_SetActiveCamera = CHashString(_T("SetActiveCamera")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SetActiveCamera, sizeof( CONTEXTCAMPARAMS ),&ccp );			
		}
	}

	return MSG_HANDLED_PROCEED;
}



DWORD CRenderManager::OnGetRendererProfilingSnapshot(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( sizeof( RENDERERPROFILEINFO ), size );
	if( m_IRenderer )
	{
		m_IRenderer->GetFrameProfileInfo( ( RENDERERPROFILEINFO*) params );
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

DWORD CRenderManager::OnGetObjectCullDistance(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(float), size);
	float *pCullDistance = (float*)params;
	*pCullDistance = m_fObjectCullDistance;
	return MSG_HANDLED_STOP;
}

DWORD CRenderManager::OnSetObjectCullDistance(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(float), size);
	float *pCullDistance = (float*)params;
	m_fObjectCullDistance = *pCullDistance;
	return MSG_HANDLED_STOP;
}

DWORD CRenderManager::OnStart(DWORD size, void *data)
{
	m_bIsRunning = true;
	
	return MSG_HANDLED_PROCEED;
}

DWORD CRenderManager::OnStop(DWORD size, void *data)
{
	m_bIsRunning = false;
	
	return MSG_HANDLED_PROCEED;
}

DWORD CRenderManager::OnSetWireFrameMode(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(sizeof(bool), size);
	bool *pCullDistance = (bool*)(params);
	
	if( *pCullDistance )
	{
		m_IRenderer->SetWireframeMode( true );
	}
	else
	{
		m_IRenderer->SetWireframeMode( false );
	}
	return MSG_HANDLED_STOP;
}


DWORD CRenderManager::OnCreateController(DWORD size, void *params)
{
	IController ** control;
	VERIFY_MESSAGE_SIZE(sizeof(IController **), size);
	control = (IController**)(params);
	(*control) = new CController();	
	return MSG_HANDLED_STOP;
}

DWORD CRenderManager::OnDestroyController( DWORD size, void * params)
{
	VERIFY_MESSAGE_SIZE(sizeof(IController**), size);
	IController **ppController = (IController**)(params);
	SAFE_DELETE( *ppController );
	return MSG_HANDLED_STOP;
}

DWORD CRenderManager::OnCreateGeometry( DWORD size, void * params)
{
	IGeometry ** geom;
	VERIFY_MESSAGE_SIZE(sizeof(IGeometry **), size);
	geom = (IGeometry**)(params);
	(*geom) = new CGeometry();	
	return MSG_HANDLED_STOP;
}

DWORD CRenderManager::OnDestroyGeometry( DWORD size, void * params)
{
	VERIFY_MESSAGE_SIZE(sizeof(IGeometry**), size);
	IGeometry **ppGeometry = (IGeometry**)(params);
	SAFE_DELETE( *ppGeometry );
	return MSG_HANDLED_STOP;
}

DWORD CRenderManager::OnCreateSceneSettings( DWORD size, void * params)
{
	ISceneSettings ** ss;
	VERIFY_MESSAGE_SIZE(sizeof(ISceneSettings), size);
	ss = (ISceneSettings**)(params);
	(*ss) = new CObjectSceneSettings();	
	return MSG_HANDLED_STOP;
}

DWORD CRenderManager::OnDestroySceneSettings( DWORD size, void * params)
{
	VERIFY_MESSAGE_SIZE(sizeof(ISceneSettings), size);
	ISceneSettings **ss = (ISceneSettings**)(params);
	SAFE_DELETE( *ss );
	return MSG_HANDLED_STOP;
}

DWORD CRenderManager::OnAddPostProcessEffect(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(POSTPROCESSEFFECTPARAMS));
	POSTPROCESSEFFECTPARAMS &ppep = *(POSTPROCESSEFFECTPARAMS*)params;

	POSTPROCESSSHADERS &postProcessShaders = m_PostProcessContextShaders[ ppep.context ];
	if (postProcessShaders.size() == 0)
		m_IRenderer->EnablePostProcess( true );
	postProcessShaders.push_back( ppep.effect );

	return MSG_HANDLED_STOP;
}

void CRenderManager::DoPostProcess( IRenderContext *pContext )
{
	if (!m_IRenderer)
		return;

	POSTPROCESSSHADERS &postProcessShaders = m_PostProcessContextShaders[ pContext ];
	if (postProcessShaders.size() > 0)
	{
		// clear backbuffer before applying post process shaders to remove noise on components drawn
		// afterwards
		m_IRenderer->SetColorMask( true, true, true, true );
		UINT r=255, g=255, b=255, a=255;
		m_IRenderer->SetBackgroundColor(r, g, b, a);
		m_IRenderer->ClearScreen( true, true );
	}
	int finalpass = (int)postProcessShaders.size() - 1;
	POSTPROCESSSHADERS::iterator iter = postProcessShaders.begin();
	for( int i = 0; iter != postProcessShaders.end(); i++, ++iter )
	{
		IEffect *pEffect = (*iter);
		if( i < finalpass )
		{
			m_IRenderer->RenderPostProcessIntermediate( pEffect );
		}
		else
		{
			m_IRenderer->RenderPostProcessFinal( pEffect, pContext );
			m_IRenderer->SetTexture( 1, NULL );
			m_IRenderer->SetTexture( 2, NULL );
		}
	}
}

DWORD CRenderManager::OnRemovePostProcessEffect(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(POSTPROCESSEFFECTPARAMS));
	POSTPROCESSEFFECTPARAMS &ppep = *(POSTPROCESSEFFECTPARAMS*)params;

	POSTPROCESSSHADERS &postProcessShaders = m_PostProcessContextShaders[ ppep.context ];
	POSTPROCESSSHADERS::iterator itr = postProcessShaders.begin();
	while (itr != postProcessShaders.end())
	{
		if ((*itr) == ppep.effect)
		{
			postProcessShaders.erase( itr );
			break;
		}
		itr++;
	}

	if (postProcessShaders.size() == 0)
		m_IRenderer->EnablePostProcess( false );

	return MSG_HANDLED_STOP;
}

DWORD CRenderManager::OnRenderUpdate(DWORD size, void *param)
{
	Update(0);
	return MSG_HANDLED_PROCEED;
}

DWORD CRenderManager::OnGetRenderFlagValue(DWORD size, void *params)
{
	//little slow for now but not used much at all
	static map< DWORD, CHashString > mHashes;
	mHashes[ ALPHA ].Init( _T("ALPHA") );
	mHashes[ ALPHATEST ].Init( _T("ALPHATEST") );
	mHashes[ LIGHT_RECEIVE_HEMI ].Init( _T("LIGHT_RECEIVE_HEMI") );
	mHashes[ LIGHT_RECEIVE_OMNI ].Init( _T("LIGHT_RECEIVE_OMNI") );
	mHashes[ LIGHT_RECEIVE_DIR ].Init( _T("LIGHT_RECEIVE_DIR") );
	mHashes[ SHADOW_RECEIVE_HEMI ].Init( _T("SHADOW_RECEIVE_HEMI") );
	mHashes[ SHADOW_RECEIVE_OMNI ].Init( _T("SHADOW_RECEIVE_OMNI") );
	mHashes[ SHADOW_RECEIVE_DIR ].Init( _T("SHADOW_RECEIVE_DIR") );
	mHashes[ SHADOW_CAST_HEMI ].Init( _T("SHADOW_CAST_HEMI") );
	mHashes[ SHADOW_CAST_OMNI ].Init( _T("SHADOW_CAST_OMNI") );
	mHashes[ SHADOW_CAST_DIR ].Init( _T("SHADOW_CAST_DIR") );
	mHashes[ LIGHTMAP_RECEIVE ].Init( _T("LIGHTMAP_RECEIVE") );
	mHashes[ LIGHTMAP_CAST ].Init( _T("LIGHTMAP_CAST") );
	mHashes[ SPHERICAL_HARMONICS ].Init( _T("SPHERICAL_HARMONICS") );
	mHashes[ AMBIENT_OCCLUSION ].Init( _T("AMBIENT_OCCLUSION") );
	mHashes[ HIGH_DYNAMIC_RANGE ].Init( _T("HIGH_DYNAMIC_RANGE") );
	mHashes[ BONES_0 ].Init( _T("BONES_0") );
	mHashes[ BONES_1 ].Init( _T("BONES_1") );
	mHashes[ BONES_2 ].Init( _T("BONES_2") );
	mHashes[ BONES_3 ].Init( _T("BONES_3") );
	mHashes[ BONES_4 ].Init( _T("BONES_4") );
	mHashes[ FOG ].Init( _T("FOG") );
	mHashes[ SHADOW_CAST ].Init( _T("SHADOW_CAST") );
	mHashes[ SHADOW_RECEIVE ].Init( _T("SHADOW_RECEIVE" ));
	mHashes[ AMBIENT_OCCLUSION_CAST ].Init( _T("AMBIENT_OCCLUSION_CAST") );
	mHashes[ AMBIENT_OCCLUSION_RECEIVE ].Init( _T("AMBIENT_OCCLUSION_RECEIVE" ));
	mHashes[ SHADER_RF_LIGHT ].Init( _T("SHADER_RF_LIGHT") );
	mHashes[ SHADER_RF_GLOW ].Init( _T("SHADER_RF_GLOW") );

	VERIFY_MESSAGE_SIZE(size, sizeof(GETRENDERFLAGVALUE));
	GETRENDERFLAGVALUE * pVal = (GETRENDERFLAGVALUE*) params;
	if( pVal )
	{
		pVal->m_ReturnValue = 0;
		int size = (int)pVal->m_Hashes.size();
		for( int i = 0; i < size; i++ )
		{
			map< DWORD, CHashString >::iterator iter = mHashes.begin();
			for( ; iter != mHashes.end(); ++iter )
			{
				CHashString &curHash = (*iter).second;
				DWORD hash = curHash.GetUniqueID();
				if( pVal->m_Hashes[ i ] &&
					hash == pVal->m_Hashes[ i ]->GetUniqueID() )
				{
					pVal->m_ReturnValue |= (*iter).first;
					break;
				}
			}
		}
	}
	return MSG_HANDLED_STOP;
}


DWORD CRenderManager::OnSetNullRenderer(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( bool ) );
	bool * pParam = (bool*)params;
	if( pParam )
	{
		m_bNullRenderer = *pParam;
	}
	return MSG_HANDLED_STOP;
}

DWORD CRenderManager::OnCreateMaterialOverride(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(IMaterialOverride**));
	IMaterialOverride **matOver = (IMaterialOverride**)(param);
	(*matOver) = new CMaterialOverride();	
	return MSG_HANDLED_STOP;
}

DWORD CRenderManager::OnAddVisibleEntitiesToScene( DWORD size, void * param )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ADDRENDERLISTPARAMS));
	// send finalrender list ourselves
	OnAddRenderList( size, param );
	return MSG_HANDLED_STOP;
}

void CRenderManager::CreateDepthTargetTexture()
{
	if( !m_DepthTexture &&
		m_IRenderer )
	{
		int viewX, viewY;
		CHashString Msg_CreateTexMsg( _T("CreateTexture") );
		CHashString DepthTargetTex(_T("CRM_DepthTargetTex") );
		m_IRenderer->GetViewDimensions( viewX, viewY );
		CREATETEXTUREPARAMS	cp;
//		cp.bRenderTargetTexture = RENDER_TARGET_COLOR_BUFFER;
		cp.bRenderTargetTexture = RENDER_TARGET_ONE_CHANNEL_FP;
		cp.sizeX = viewX;
		cp.sizeY = viewY;
		cp.Name = &DepthTargetTex;
		cp.Format = NULL;
//		cp.bitDepth = 32;
		cp.bitDepth = 32;
		if( m_ToolBox->SendMessage( Msg_CreateTexMsg.GetUniqueID(), sizeof( cp ), &cp ) == MSG_HANDLED )
		{
			m_DepthTexture = (ITextureObject*)cp.TextureObjectInterface;
		}
	}
}

DWORD CRenderManager::OnGetDepthTargetAsTexture( DWORD size, void * param  )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( TEXTUREOBJECTPARAMS ) );
	TEXTUREOBJECTPARAMS * parms = (TEXTUREOBJECTPARAMS*)param;
	if( parms &&
		m_DepthTexture)
	{
		parms->TextureObjectInterface = m_DepthTexture;
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

void CRenderManager::BindDepthTextureMRT()
{
	if( m_IRenderer )
	{
		m_IRenderer->ApplyRenderTarget( m_DepthTexture, 1 );
	}
}

void CRenderManager::UnbindDepthTextureMRT()
{
	if( m_IRenderer )
	{
		m_IRenderer->UnApplyRenderTarget( m_DepthTexture, 1 );
	}
}

DWORD CRenderManager::OnSetFogValue( DWORD size, void * param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( FOGVALUEMESSAGE ) );
	FOGVALUEMESSAGE * p = (FOGVALUEMESSAGE*)param;
	if( p )
	{
		m_FogStart = p->m_Start;
		m_FogDistance = p->m_FogDistance;
		if( p->m_Color )
		{
			m_FogColor.x = p->m_Color->x;
			m_FogColor.y = p->m_Color->y;
			m_FogColor.z = p->m_Color->z;
		}

	}
	return MSG_HANDLED_STOP;
}

DWORD CRenderManager::OnGetFogValue( DWORD size, void * param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( FOGVALUEMESSAGE ) );
	FOGVALUEMESSAGE * p = (FOGVALUEMESSAGE*)param;
	if( p )
	{
		p->m_Start = m_FogStart;
		p->m_FogDistance = m_FogDistance;
		if( p->m_Color )
		{
			*p->m_Color = m_FogColor;
		}

	}
	return MSG_HANDLED_STOP;
}

void CRenderManager::GetFogStartDistance( float &start, float &distance )
{
	start = m_FogStart;
	distance = m_FogDistance;
}

void CRenderManager::GetFogColor( Vec3 &color )
{
	color = m_FogColor;
}

DWORD CRenderManager::OnSetColorCorrection( DWORD size, void * param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(COLORCORRECTIONPARAMS) );
	COLORCORRECTIONPARAMS *p = (COLORCORRECTIONPARAMS*)param;
	if (m_IRenderer)
	{
		m_IRenderer->SetColorCorrection( p->fBrightness, p->fContrast, p->fGamma );
		return MSG_HANDLED_STOP;
	}
	else
		return MSG_ERROR;
}
