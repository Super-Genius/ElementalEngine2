///============================================================================
/// \file	CLightManager.cpp
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
#include "CLightManager.h"
#include "math.h"

#define DEPTH_SHADOW_TEXTURE_SIZE 512

#define FILE_LPM_OMNI	_T("shaders/LPM_P.efx" )
#define FILE_LPM_HEMI	_T("shaders/LPM_P_Hemi.efx" )

REGISTER_COMPONENT_SINGLETON( CLightManager );

REGISTER_MESSAGE_HANDLER(EnableDepthShadows, OnEnableDepthShadows, CLightManager);
REGISTER_MESSAGE_HANDLER(RegisterLight, OnRegisterLight, CLightManager);
REGISTER_MESSAGE_HANDLER(UnregisterLight, OnUnregisterLight, CLightManager);
REGISTER_MESSAGE_HANDLER(GetLightObjPtrByName, OnGetLightObjPtrByName, CLightManager);
REGISTER_MESSAGE_HANDLER(GetLightShadowTexture, OnGetLightShadowTexture, CLightManager);
REGISTER_MESSAGE_HANDLER(GetGlobalLightArray, OnGetLightArray, CLightManager);
REGISTER_MESSAGE_HANDLER(GenerateLightPerspectiveMap, OnGenerateLightPerspectiveMap, CLightManager);

IEffect * g_LPMEffectDirectional = NULL;
IEffect * g_LPMEffectPointOmni = NULL;
IEffect * g_LPMEffectPointHemi = NULL;

CLightManager::CLightManager() : 
	CManager(_T("CLightManager"), LIGHTPRIORITY),
	m_LightType(_T("CLightObject"))
{
	OnEnableDepthShadows(0,0);
	m_ShadowFpsInterval = SHADOW_FPS_INTERVAL;
	m_bLightTextureUpdating = false;
	m_LPMTempIndex = 0;
	m_ShadowMapPoolCount = DEFAULT_SHADOW_POOL_COUNT;
	m_ShadowMapMaxPerScreen = DEFAULT_SHADOW_GENERATION_PER_FRAME;
}


IComponent *CLightManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CLightManager );
}

CLightManager::~CLightManager( )
{
	
}

void CLightManager::Update(  DWORD tickCount  )
{	
	// call update on light objects
	IDTOOBJECTMAP *objMap = GetObjectMap(&m_LightType);
	if ( objMap )
	{
		for( IDTOOBJECTMAP::iterator objIter = objMap->begin();objIter != objMap->end(); ++objIter)
		{
			IObject *lightObject;
			lightObject = objIter->second;
			lightObject->Update();
		}
	}

	// need to get frustum from the camera
	DWORD retValue;
	CFrustum *camFrustum = NULL;
	CONTEXTCAMPARAMS ccp;
	ccp.pRC = NULL;
	CHashString hszCameraName;
	ccp.szCameraName = &hszCameraName;
	static DWORD msgHash_GetActiveCamera = CHashString(_T("GetActiveCamera")).GetUniqueID();
	retValue = m_ToolBox->SendMessage(msgHash_GetActiveCamera, sizeof( CONTEXTCAMPARAMS ), &ccp);
	if( ccp.szCameraName && ( retValue == MSG_HANDLED ) )
	{
		static DWORD msgHash_GetCameraFrustum = CHashString(_T("GetCameraFrustum")).GetUniqueID();
		static CHashString camName(_T("CCamera"));
		retValue = m_ToolBox->SendMessage(msgHash_GetCameraFrustum, sizeof(CFrustum**), &camFrustum, 
			ccp.szCameraName, &camName);
		if (retValue != MSG_HANDLED)
			camFrustum = NULL;
	}
	CONTEXTCAMVECTORPARAMS ccvp;
	Vec3 camPosition;
	ccvp.pRC = NULL;
	ccvp.vdata = &camPosition;
	static DWORD msgHash_GetActiveCameraPosition = CHashString(_T("GetActiveCameraPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetActiveCameraPosition, sizeof(CONTEXTCAMVECTORPARAMS), &ccvp );	

	m_CurrentTick = m_Timer->GetTime();

	// Do a frustum visit to determin which light objects are in view to
	// update the objects they affect in the scene
	CLightInFrustumVisitor lightVisitor;
	lightVisitor.SetCameraPos( camPosition );

	VISITSCENEFRSUTUMPARAMS vsp;
	vsp.visitor = (IVisitor*)&lightVisitor;
	vsp.frustum = camFrustum;
	vsp.sortID = LIGHTOBJECT3D;
	static DWORD msg_VisitSceneFrustum = CHashString(_T("VisitSceneFrustum")).GetUniqueID();
	m_ToolBox->SendMessage( msg_VisitSceneFrustum, sizeof(VISITSCENEFRSUTUMPARAMS), &vsp, NULL, NULL );
	lightVisitor.UpdateLightsAndShadows();
}

DWORD CLightManager::OnSetShadowFrameInterval( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( int ) );
	int * parms = (int*)params;
	if( parms )
	{
		m_ShadowFpsInterval = *parms;
		if( m_ShadowFpsInterval <= 0 )
		{
			m_ShadowFpsInterval = 0;
		}
	}
	return MSG_HANDLED_STOP;
}

DWORD CLightManager::OnGenerateLightPerspectiveMap( DWORD size, void *params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof(GENERATELIGHTPERSPECTIVEMAPPARAMS) );
	GENERATELIGHTPERSPECTIVEMAPPARAMS* glpmp = (GENERATELIGHTPERSPECTIVEMAPPARAMS*)params;
	if( glpmp )
	{
		m_bLightTextureUpdating = true;
		GenerateLightPerspectiveMap( glpmp->lightObject, glpmp->renderObjects );
		m_bLightTextureUpdating = false;
	}
	return MSG_HANDLED_STOP;
}


bool CLightManager::GenerateLightPerspectiveMap( ILightObject* lightObject, OBJECTLIST* renderObjects )
{
	OBJECTLIST::iterator iter;
	GEOMETRYLIST::iterator iterg;
	
	Light_Info & rinfo = m_LightList[ lightObject ];
	rinfo.lastUpdate = m_CurrentTick;
	if( m_LastShadowGenerationFrames.find( lightObject ) == m_LastShadowGenerationFrames.end() )
	{
		m_LastShadowGenerationFrames[ lightObject ] = 0;
	}
	else
	{
		m_LastShadowGenerationFrames[ lightObject ]--;
		if( m_LastShadowGenerationFrames[ lightObject ] <= 0 )
		{
			m_LastShadowGenerationFrames[ lightObject ] = m_ShadowFpsInterval;
			//render shadow
		}
		else
		{
			return false;//skip shadow rendering this frame
		}
	}

	//Update texture	
	rinfo.ShadowMapIndex = GetLastUsedShadowMapIndex();
	rinfo.LightPerspectiveMap = GetValidShadowMap( rinfo.ShadowMapIndex );
	if( rinfo.LightPerspectiveMap != NULL )
	{
		m_ShadowMaps[ rinfo.ShadowMapIndex ].m_LastFrameSet = m_CurrentTick;
		lightObject->GetPosition( rinfo.lastLightPosition );
		rinfo.numLastRendered = 0;//reset number of rendered, this is used to optimize so that
								//textures with no units rendered on it don't use the shadow shader
		//now update the depth shadow map
		if( lightObject->GetLightType() == HEMI_POINT_LIGHT )
		{
			//got the object list, send a render to texture with it for the light, first check that the light's texture
			//is valid
			if( rinfo.LightPerspectiveMap != NULL )
			{
				GenerateLightPerspectiveTexture( rinfo.LightPerspectiveMap, lightObject, renderObjects );
				rinfo.numLastRendered += (int)renderObjects->size();
			}
		}
		else if( lightObject->GetLightType() == OMNI_POINT_LIGHT )
		{
			if( rinfo.LightPerspectiveMap != NULL )
			{
				GenerateLightPerspectiveTexture( rinfo.LightPerspectiveMap, lightObject, renderObjects );
				rinfo.numLastRendered += (int)renderObjects->size();
			}

			//got the object list, send a render to texture with it for the light, first check that the light's texture
			//is valid
			if( rinfo.LightPerspectiveMap != NULL )
			{
				GenerateLightPerspectiveTexture( rinfo.LightPerspectiveMap, lightObject, renderObjects, 2 );
				rinfo.numLastRendered += (int)renderObjects->size();
			}
		}
		else if( lightObject->GetLightType() == DIRECTIONAL_LIGHT )//directional 
		{		
			if( rinfo.LightPerspectiveMap != NULL )
			{
				GenerateDirectionalLightPerspectiveTexture( rinfo.LightPerspectiveMap, lightObject, renderObjects );
				rinfo.numLastRendered += (int)renderObjects->size();
			}
		}
		return true;
	}
	return false;
}

bool CLightManager::DeInit( void )
{	
	g_LPMEffectPointOmni = NULL;
	g_LPMEffectPointHemi = NULL;
	g_LPMEffectDirectional = NULL;
	return CManager::DeInit();
}

DWORD CLightManager::OnGetLightObjPtrByName( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETLIGHTOBJPTRMESSAGE ) );
	GETLIGHTOBJPTRMESSAGE *  msg = (GETLIGHTOBJPTRMESSAGE * )params;

	IDTOOBJECTMAP *pObjMap = NULL;
	IHashString *lightName;

	CHashString loName(_T("CLightObject"));
	pObjMap = GetObjectMap( &loName ); 
	if (!pObjMap) 
		return MSG_ERROR;

	lightName = msg->hszObjName;

	IDTOOBJECTMAP::iterator itr = pObjMap->find(lightName->GetUniqueID());
	if (itr != pObjMap->end())
	{
		msg->pLightObject = dynamic_cast<ILightObject *>(itr->second);
	}

	return MSG_HANDLED_STOP;
}

IEffect * GetEffect( TCHAR* filepath )
{
	//load the Effect
	LOADFILEEXTPARAMS lfep;
	CHashString effectPath(filepath);
	IEffect * mEffect = NULL;
	lfep.fileName = filepath;
	lfep.bInternalLoad = true;
	static DWORD msgHash_LoadFileByExtension = CHashString(_T("LoadFileByExtension")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_LoadFileByExtension, sizeof(LOADFILEEXTPARAMS), &lfep);
	if( retval == MSG_HANDLED )
	{
		CREATEEFFECTINTERFACE cef;
		cef.m_Name = &effectPath;
		static DWORD msgHash_GetEffectInterface = CHashString(_T("GetEffectInterface")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_GetEffectInterface, sizeof(CREATEEFFECTINTERFACE ), &cef );
		mEffect = cef.m_EffectInterface;
	}
	if( !mEffect )
	{
		EngineGetToolBox()->Log( LOGERROR, _T("Could not load a Shadow Generation Effect" ) );
	}
	return mEffect;
}

DWORD CLightManager::OnEnableDepthShadows( DWORD size, void *params)
{
	if( g_LPMEffectPointOmni == NULL ||
		g_LPMEffectPointHemi == NULL ||
		g_LPMEffectDirectional == NULL )
	{
		g_LPMEffectPointOmni		= GetEffect( FILE_LPM_OMNI );
		g_LPMEffectPointHemi		= GetEffect( FILE_LPM_HEMI );
		g_LPMEffectDirectional		= GetEffect( FILE_LPM_OMNI );
	}
	return MSG_HANDLED_STOP;
}

DWORD CLightManager::OnRegisterLight( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( ILightObject * ) );
	ILightObject *  msg = (ILightObject * )params;

	//add to internal mapping with other variables used for light perspective depth shadows
	if( msg )
	{
		m_LightList[ msg ].lastUpdate = m_CurrentTick;
		m_LightList[ msg ].LightPerspectiveMap = NULL;
		if( msg->CanCastShadows() == true )
		{
			//create the light perspective map texture
			if( m_ShadowMaps.size() < m_ShadowMapPoolCount )
			{
				IBaseTextureObject * tex = NULL;
				CreateDepthShadowRenderTarget( &tex, msg);			
				m_LightList[ msg ].ShadowMapIndex = m_ShadowMaps.size();	
				ShadowTexturePoolEntry entry;
				entry.texture = tex;
				entry.m_LastFrameSet = 0;
				m_ShadowMaps.push_back( entry );
			}
			else //assign a null texture for now, textures will be relinked/swapped later
			{
				m_LightList[ msg ].ShadowMapIndex = INVALID_SHADOW_MAP_INDEX;
			}
		}
	}
	return MSG_HANDLED_STOP;
}

DWORD CLightManager::OnUnregisterLight( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( ILightObject * ) );
	//remove from internal mapping with other variables used for light perspective depth shadows
	ILightObject *  msg = (ILightObject * )params;
	if( msg && m_LightList.size() > 0 )
	{
		LIGHTINFOMAPPING::iterator	iter = m_LightList.find( msg );
		if( iter != m_LightList.end() )
		{
			Light_Info &rinfo = (*iter).second;
			if( rinfo.LightPerspectiveMap )
			{
				//unregister render texture
				//TODO, check pool, if new num lights < poolsize, take out a shadowtexture if so
				if( m_LightList.size() - 1 < m_ShadowMaps.size() )
				{
					size_t lastIndex = m_ShadowMaps.size() - 1;
					RemoveDepthShadowRenderTarget( m_ShadowMaps[ lastIndex ].texture );
					m_ShadowMaps.pop_back();
				}
			}
			m_LightList.erase( iter );
		}
	}
	return MSG_HANDLED_STOP;
}


void CLightManager::RemoveDepthShadowRenderTarget( IBaseTextureObject * tex )
{
	TEXTUREOBJECTPARAMS tobj;
	//global(for this class) render target
	if( tex )
	{
		tobj.bLoad = false;
		tobj.Name = NULL;
		tobj.TextureObjectInterface = (ITextureObject*)tex;
		static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
	}
}

void CLightManager::CreateDepthShadowRenderTarget( IBaseTextureObject ** tex, ILightObject * lightObject)
{
	//Check pool cont
	CREATETEXTUREPARAMS createtex;
	CHashString RTT;
	char buffer[MAX_PATH];
	// make unique name and bump.
	sprintf( buffer, "LPM_%d", m_LPMTempIndex++ );
	RTT.Init( (TCHAR*)buffer );
	//create the cached texture
	//createtex.bitDepth = 16;
	///The name here better not be saved off in the create component as a pointer, it better make a copy
	// or else once the scope is out for the chashstring in this function, it will crash
	createtex.Name = &RTT;
	createtex.sizeX = DEPTH_SHADOW_TEXTURE_SIZE;
	createtex.sizeY = DEPTH_SHADOW_TEXTURE_SIZE;
	createtex.bRenderTargetTexture = RENDER_TARGET_DEPTH_LUMINANCE;//RENDER_TARGET_COLOR_BUFFER;//RENDER_TARGET_DEPTH_LUMINANCE;
	createtex.bAutoGenMipMaps = true;
	static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateTexture, sizeof(createtex), &createtex );
	*tex = createtex.TextureObjectInterface;			
}

bool CLightManager::HasValidShadowMap( ILightObject * lightObject )
{
	//get mapping, do a draw2d quad
	if( m_LightList.size() > 0 )
	{
		LIGHTINFOMAPPING::iterator	iter = m_LightList.find( lightObject );
		if( iter != m_LightList.end() )
		{
			Light_Info & rinfo = m_LightList[ lightObject ];
			return (rinfo.numLastRendered > 0 );
		}
	}
	return false;
}

//temp
void CLightManager::RenderLight( ILightObject * lightObject )
{
	//get renderer
	CLightManager * lman = SINGLETONINSTANCE(CLightManager);
	GETRENDERERINTERFACEPARAMS RInterface;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	DWORD retVal = lman->m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS ),&RInterface);
	if( retVal != MSG_HANDLED 
		|| !RInterface.m_RendererInterface)
	{
		return;
	}
	//get mapping, do a draw2d quad
	Light_Info & rinfo = lman->m_LightList[ lightObject ];
	//if( rinfo.LightPerspectiveMap )
	{
	//	RInterface.m_RendererInterface->Draw2DQuad( 0,0,400,400, rinfo.LightPerspectiveMap );
	}	
}

DWORD CLightManager::OnGetLightShadowTexture( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETLIGHTSHADOWTEXTURE )  );
	GETLIGHTSHADOWTEXTURE* parms= (GETLIGHTSHADOWTEXTURE*) params;
	if( parms && 
		parms->pLightObj &&
		m_bLightTextureUpdating == false )
	{
		parms->pLightTexture = GetLightShadowTexture( parms->pLightObj );
	}else
	{
		parms->pLightTexture = NULL;
	}
	return MSG_HANDLED_STOP;
}

Light_Info * CLightManager::GetLightShadowInfo( ILightObject * lightObject )
{
	if( m_LightList.size() > 0 )
	{
		LIGHTINFOMAPPING::iterator	iter = m_LightList.find( lightObject );
		if( iter != m_LightList.end() )
		{
			return &m_LightList[ lightObject ];
		}
	}
	return NULL;
}

IBaseTextureObject* CLightManager::GetLightShadowTexture( ILightObject * lightObject )
{
	if( m_LightList.size() > 0 )
	{
		LIGHTINFOMAPPING::iterator	iter = m_LightList.find( lightObject );
		if( iter != m_LightList.end() )
		{
			Light_Info & rinfo = m_LightList[ lightObject ];
			if( rinfo.ShadowMapIndex != INVALID_SHADOW_MAP_INDEX )
			{
				rinfo.LightPerspectiveMap = GetValidShadowMap( rinfo.ShadowMapIndex );
				return rinfo.LightPerspectiveMap;
			}
		}
	}
	return NULL;
}

void CLightManager::GenerateDirectionalLightPerspectiveTexture( IBaseTextureObject * tex,
											ILightObject * light,
											OBJECTLIST * renderList )
{
	RENDERLISTTOTEXTUREPARAMS params;
	DWORD retVal;
	if( tex )
	{
		//get renderer
		GETRENDERERINTERFACEPARAMS RInterface;
		static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
		retVal = m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS ),&RInterface);
		if( retVal != MSG_HANDLED 
			|| !RInterface.m_RendererInterface)
		{
			return;
		}
		//RInterface.m_RendererInterface->SetPolygonZBias( m_fZBiasScale, m_fZBiasOffset );		
		//RInterface.m_RendererInterface->SetPolygonZBias( 1.1f, 5.f );
		RInterface.m_RendererInterface->SetPolygonZBias(1,0);
		RInterface.m_RendererInterface->SetDepthWrite( true );

		//set the list for rendering the scene from the light's point of view
		//send an immediate render to texture	
		params.dwSortMethod = 0;
		params.objList = renderList;
		params.RenderTexture = (ITextureObject*)tex;	
		params.Camera = NULL;		
		//set some VEffect constants
		Vec3 vecPos;
		float p[4] = {0,0,0,0};
		light->GetPosition(vecPos);
		p[0] = vecPos.x;
		p[1] = vecPos.y;
		p[2] = vecPos.z;
		float zclip[4] = {0,0,0,0};
		zclip[ 0 ] = 1.0f;
		zclip[ 1 ] = MAX_SHADOW_ATTEN;
		RInterface.m_RendererInterface->SetVertexShaderConstant( C_SHADOWCOORD0, p, 1 );
		RInterface.m_RendererInterface->SetVertexShaderConstant( C_ZCLIP, (float*)zclip, 1 );
		RInterface.m_RendererInterface->SetColorMask( true, false, false, false );
		RInterface.m_RendererInterface->SetBlendMode( BLEND_NORMAL );
		RInterface.m_RendererInterface->SetBackgroundColor( 255,255,255 );
		static DWORD msgHash_RenderListToTexture = CHashString(_T("RenderListToTexture")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RenderListToTexture, sizeof(params), &params );	
		RInterface.m_RendererInterface->SetPolygonZBias(0,0);

		//unset mask
		RInterface.m_RendererInterface->SetColorMask( true, true, true, true );
	}
}

DWORD CLightManager::OnGetLightArray( DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE( size, sizeof( GETLIGHTLIST ) );
	GETLIGHTLIST * lp = (GETLIGHTLIST*)params;
	IDTOOBJECTMAP *objMap = GetObjectMap( &m_LightType );
	if( objMap && objMap->size() > 0 )
	{
		IDTOOBJECTMAP::iterator iter = objMap->begin();
		for(;iter != objMap->end(); ++iter)
		{
			ILightObject* light = dynamic_cast<ILightObject *>(iter->second );
			if (light != NULL)
			{
				lp->lightList.push_back( light );
			}
			else
			{
				m_ToolBox->Log(LOGWARNING, _T("Light Object failed dynamic cast\n")
					_T("Is it really a light object?\n"));
			}
		}
	}
	return MSG_HANDLED_STOP;
}

void CLightManager::GenerateLightPerspectiveTexture( IBaseTextureObject* tex,
													ILightObject* light,
													OBJECTLIST* renderList, int pass )
{
	RENDERLISTTOTEXTUREPARAMS params;
	DWORD retVal;
	if( tex )
	{
		//get renderer
		GETRENDERERINTERFACEPARAMS RInterface;
		static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
		retVal = m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS ),&RInterface);
		if( retVal != MSG_HANDLED 
			|| !RInterface.m_RendererInterface)
		{
			return;
		}
		//RInterface.m_RendererInterface->SetPolygonZBias( m_fZBiasScale, m_fZBiasOffset );		
		//RInterface.m_RendererInterface->SetPolygonZBias( 1.1f, 5.f );
		RInterface.m_RendererInterface->SetPolygonZBias(1,0);
		RInterface.m_RendererInterface->SetDepthWrite( true );

		//set the list for rendering the scene from the light's point of view
		//send an immediate render to texture	
		params.dwSortMethod = 0;
		params.objList = renderList;
		params.RenderTexture = (ITextureObject*)tex;	
		params.Camera = NULL;		
		//set some VEffect constants
		//this is the light matrix
		float p[4] = {0,0,0,0};
		Vec3 vecPos;
		light->GetPosition(vecPos);
		p[0] = vecPos.x;
		p[1] = vecPos.y;
		p[2] = vecPos.z;
		float zclip[4] = {0,0,0,0};
		zclip[ 0 ] = 1.0f;
		zclip[ 1 ] = light->GetAttenuationDistance();
		zclip[ 2 ] = ((light->GetLightType() == OMNI_POINT_LIGHT && pass == 0) || 
						(light->GetLightType() == HEMI_POINT_LIGHT)) ? -1.0f : 1.0f;

		RInterface.m_RendererInterface->SetVertexShaderConstant( C_SHADOWCOORD0, p, 1 );
		RInterface.m_RendererInterface->SetVertexShaderConstant( C_ZCLIP, (float*)zclip, 1 );
		if( light->GetLightType() == OMNI_POINT_LIGHT )
		{
			if( pass == 0 )
			{
				// write to red channel
				params.clearDepth = true;
				params.clearTarget = true;
				RInterface.m_RendererInterface->SetColorMask( true, false, false, true );
			}
			else
			{
				// write to green channel
				params.clearDepth = true;
				params.clearTarget = false; // don't clear data in the red channel
				RInterface.m_RendererInterface->SetColorMask( false, true, false, true );
			}
		}
		else if (light->GetLightType() == HEMI_POINT_LIGHT)
		{
			params.clearDepth = true;
			params.clearTarget = true;
			RInterface.m_RendererInterface->SetColorMask( true, true, false, true );
		}
		
		RInterface.m_RendererInterface->SetBlendMode( BLEND_NORMAL );
		RInterface.m_RendererInterface->SetBackgroundColor( 255,255,255 );
		static DWORD msgHash_RenderListToTexture = CHashString(_T("RenderListToTexture")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RenderListToTexture, sizeof(params), &params );	
		RInterface.m_RendererInterface->SetPolygonZBias(0,0);

		//unset mask
		RInterface.m_RendererInterface->SetColorMask( true, true, true, true );
	}
}
