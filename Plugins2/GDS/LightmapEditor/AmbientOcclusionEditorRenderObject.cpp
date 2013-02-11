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

#include "StdAfx.h"
#include "AmbientOcclusionEditor.h"
#include "AmbientOcclusionEditorRenderObject.h"

REGISTER_COMPONENT(CAmbientOcclusionEditorRenderObject);

CAmbientOcclusionEditorRenderObject::CAmbientOcclusionEditorRenderObject(IHashString *parentName, IHashString *name) : 
	CRenderObject(_T("LightMapManager"), _T("CAmbientOcclusionEditorRenderObject"), parentName, name)
{
	m_pAccumTexture = NULL;
	m_iAccumTextureResolution = 256;
	m_pDepthTexture = NULL;
	m_iDepthTextureResolution = 512;

	//get renderer
	GETRENDERERINTERFACEPARAMS RInterface;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	if (m_ToolBox->SendMessage( msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS), &RInterface ) != MSG_HANDLED)
	{
		return;
	}
	m_pRenderer = RInterface.m_RendererInterface;

	CHashString hszAmbientOcclusionEffect(_T("shaders/Cal_GenAmbientOcclusion.efx"));
	m_pAmbientOcclusionEffect = LoadEffect( &hszAmbientOcclusionEffect );

	CHashString hszAmbientOcclusionShadowEffect(_T("shaders/Cal_GenAmbientOcclusionShadow.efx"));
	m_pAmbientOcclusionShadowEffect = LoadEffect( &hszAmbientOcclusionShadowEffect );
}

CAmbientOcclusionEditorRenderObject::~CAmbientOcclusionEditorRenderObject()
{
	DeInit();
}

void CAmbientOcclusionEditorRenderObject::Init()
{
	StdString szTempName;
	CREATETEXTUREPARAMS createtextureparams;

	// generate accum texture name
	szTempName = GetName()->GetString();
	szTempName += _T("_Accum");
	m_hszAccumTextureName.Init( szTempName );

	// generate depth texture name
	szTempName = GetName()->GetString();
	szTempName += _T("_Depth");
	m_hszDepthTextureName.Init( szTempName );

	// create light pov camera
	szTempName = GetName()->GetString();
	szTempName += _T("_Camera");
	m_hszLightCameraName.Init( szTempName );

	// create accum texture
	createtextureparams.Name = &m_hszAccumTextureName;
	createtextureparams.sizeX = m_iAccumTextureResolution;
	createtextureparams.sizeY = m_iAccumTextureResolution;
	createtextureparams.bitDepth = 128;
	createtextureparams.bRenderTargetTexture = RENDER_TARGET_FLOATINGPOINT;
	static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_CreateTexture, sizeof(createtextureparams), &createtextureparams );
	m_pAccumTexture = dynamic_cast<ITextureObject*>( createtextureparams.TextureObjectInterface );

	// create depth texture
	createtextureparams.Name = &m_hszDepthTextureName;
	createtextureparams.sizeX = m_iDepthTextureResolution;
	createtextureparams.sizeY = m_iDepthTextureResolution;
	createtextureparams.bitDepth = 32;
	createtextureparams.bRenderTargetTexture = RENDER_TARGET_DEPTH_LUMINANCE;
//	createtextureparams.bRenderTargetTexture = RENDER_TARGET_COLOR_BUFFER;
	m_ToolBox->SendMessage( msgHash_CreateTexture, sizeof(createtextureparams), &createtextureparams );
	m_pDepthTexture = dynamic_cast<ITextureObject*>( createtextureparams.TextureObjectInterface );

	CREATEOBJECTPARAMS cop;
	cop.name = &m_hszLightCameraName;
	cop.parentName = NULL;
	static DWORD msgHash_CreateCamera = CHashString(_T("CreateCamera")).GetUniqueID();
    m_ToolBox->SendMessage(msgHash_CreateCamera, sizeof(cop), &cop);

	SCENEOBJECTPARAMS ubbp;
	ubbp.object = this;
	ubbp.SortID = RENDEROBJECT3D;
	static DWORD msgHash_UpdateBoundingBox = CHashString(_T("UpdateBoundingBox")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_UpdateBoundingBox, sizeof(SCENEOBJECTPARAMS), &ubbp );
}

void CAmbientOcclusionEditorRenderObject::DeInit()
{
	TEXTUREOBJECTPARAMS tobj;
	tobj.bLoad = false;

	// remove accum texture
	tobj.Name = &m_hszAccumTextureName;
	static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_RemoveTexture, sizeof(tobj), &tobj );
	m_pAccumTexture = NULL;

	// remove depth texture
	tobj.Name = &m_hszDepthTextureName;
	m_ToolBox->SendMessage( msgHash_RemoveTexture, sizeof(tobj), &tobj );
	m_pDepthTexture = NULL;

	SCENEOBJECTPARAMS ubbp;
	ubbp.pObject = this;
	ubbp.sortID = RENDEROBJECT3D;
	static DWORD msgHash_RemoveBoundingBoxedObject = CHashString(_T("RemoveBoundingBoxedObject")).GetUniqueID();
	m_ToolBox->SendMessage( msgHash_RemoveBoundingBoxedObject, sizeof( SCENEOBJECTPARAMS), &ubbp );
}

void CAmbientOcclusionEditorRenderObject::GenerateAmbientOcclusion()
{
	Init();

	OBJECTLIST targetObjects;
	SINGLETONINSTANCE(CAmbientOcclusionEditor)->GenerateTargetObjectList( targetObjects );
	OBJECTLIST::iterator itrTargetObject;
	for (itrTargetObject = targetObjects.begin(); itrTargetObject != targetObjects.end(); itrTargetObject++)
	{
		IObject *pTargetObject = (*itrTargetObject);
/*
		if (!SINGLETONINSTANCE(CAmbientOcclusionEditor)->GenerateAtlas( pTargetObject->GetName(), pTargetObject->GetParentName(), m_iAccumTextureResolution ))
		{
			continue;
		}
*/
		ADDLIGHTMAPPABLEOBJECT addLightmappableMsg;
		addLightmappableMsg.name = pTargetObject->GetName();
		addLightmappableMsg.type = pTargetObject->GetComponentType();
		static DWORD msgHash_AddLightmappableObject = CHashString("AddLightmappableObject").GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_AddLightmappableObject, sizeof( ADDLIGHTMAPPABLEOBJECT ),&addLightmappableMsg ) != MSG_HANDLED)
			continue;

		// generate accum texture name
		StdString szTempName;
		szTempName = pTargetObject->GetName()->GetString();
		szTempName += _T("_lightmap.dds");
		CHashString hszOutTextureName( szTempName );

		TEXTUREOBJECTPARAMS gettex;
		gettex.Name = &hszOutTextureName;
		static DWORD msgHash_GetTexture = CHashString(_T("GetTexture")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetTexture, sizeof(gettex), &gettex ) != MSG_HANDLED)
			continue;
		ITextureObject *pOutTexture = dynamic_cast< ITextureObject*>(gettex.TextureObjectInterface);
		if (!pOutTexture)
			continue;

		Vec3 targetPos;
		float targetRadius;
		pTargetObject->GetBoundingSphere( targetPos, targetRadius );

		LIGHTSAMPLELIST lightList;
		SINGLETONINSTANCE(CAmbientOcclusionEditor)->GenerateHemisphereLightSampleList( 
			lightList, 
			1, // TODO: light sample count should be variable
			targetPos, 
			800.0f // TODO: light radius should be variable
		);

		OBJECTLIST occluderList;
		SINGLETONINSTANCE(CAmbientOcclusionEditor)->GenerateOccluderObjectList( 
			occluderList, 
			targetPos, 
			1000.0f // TODO: occlusion radius should be variable
			);
/*
		// create out texture
		static CHashString dxt5(_T("DXT5"));
		CREATETEXTUREPARAMS createtextureparams;
		createtextureparams.Name = &hszOutTextureName;
		createtextureparams.sizeX = m_iAccumTextureResolution;
		createtextureparams.sizeY = m_iAccumTextureResolution;
		createtextureparams.bitDepth = 32;
		createtextureparams.Format = &dxt5;
		static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_CreateTexture, sizeof(createtextureparams), &createtextureparams );
		ITextureObject *pOutTexture = dynamic_cast<ITextureObject*>( createtextureparams.TextureObjectInterface );
*/

		OBJECTLIST targetList;
		targetList.push_back( pTargetObject );
		GenerateAmbientOcclusionMap( targetPos, targetList, occluderList, lightList, m_pAccumTexture );

		unsigned char *pOutTextureBuffer = new unsigned char[m_iAccumTextureResolution*m_iAccumTextureResolution*4];
		float *pAccumTextureData = (float*)m_pAccumTexture->Read(0, true);

		// convert accumulation buffer to a format suitable for file output
		// unify the normals and normalize the occlusion term by the number of lights
		if (pAccumTextureData)
		{
			float fLightSamples = (float)lightList.size();
			float fOcclusionTermFactor = 255.0f / fLightSamples;
			float fNormalFactor = 255.0f / 2.0f;
			unsigned char *pCurrOut = pOutTextureBuffer;
			float *pCurrAccum = pAccumTextureData;
			int iTexelCount = m_iAccumTextureResolution * m_iAccumTextureResolution;
			for (int i=0; i < iTexelCount; i++)
			{
/*				Vec3 n( pCurrAccum[0], pCurrAccum[1], pCurrAccum[2] );
				n.Normalize();

				pCurrOut[0] = (unsigned char)((n.x + 1.0f) * fNormalFactor);
				pCurrOut[1] = (unsigned char)((n.y + 1.0f) * fNormalFactor);
				pCurrOut[2] = (unsigned char)((n.z + 1.0f) * fNormalFactor);
				pCurrOut[3] = (unsigned char)(pCurrAccum[3] * fOcclusionTermFactor);
*/
				pCurrOut[0] = (unsigned char)(pCurrAccum[0] * 255.0f);
				pCurrOut[1] = (unsigned char)(pCurrAccum[1] * 255.0f);
				pCurrOut[2] = (unsigned char)(pCurrAccum[2] * 255.0f);
				pCurrOut[3] = (unsigned char)(pCurrAccum[3] * 255.0f);

				pCurrAccum += 4;
				pCurrOut += 4;
			}
		}

		pOutTexture->Write( pOutTextureBuffer );
		SAFE_DELETE_ARRAY( pOutTextureBuffer );

//		static DWORD msgHash_SaveFileByExtension = CHashString(_T("SaveFileByExtension")).GetUniqueID();
//		m_ToolBox->SendMessage( msgHash_SaveFileByExtension, sizeof(LPTSTR), (void*)hszOutTextureName.GetString() );

		// remove out texture
//		TEXTUREOBJECTPARAMS tobj;
//		tobj.bLoad = false;
//		tobj.Name = &hszOutTextureName;
//		static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
//		m_ToolBox->SendMessage( msgHash_RemoveTexture, sizeof(tobj), &tobj );
	}

	DeInit();
}

void CAmbientOcclusionEditorRenderObject::GenerateAmbientOcclusionMap( Vec3 targetPoint, OBJECTLIST &targetList, OBJECTLIST &occluderList, LIGHTSAMPLELIST &lightList, ITextureObject *pOutTexture )
{
	static DWORD msgHash_RenderListToTexture = CHashString(_T("RenderListToTexture")).GetUniqueID();

	if (!m_pAmbientOcclusionEffect)
		return;

	// render settings
//	m_pRenderer->SetPolygonZBias(1,0);
	m_pRenderer->SetDepthWrite( true );

	int pass = 0;
	LIGHTSAMPLELIST::iterator itrLight;
	for (itrLight = lightList.begin(); itrLight != lightList.end(); itrLight++)
	{
		CAmbientOcclusionLight *pLight = &(*itrLight);

//		static DWORD msgHash_RegisterLight = CHashString(_T("RegisterLight")).GetUniqueID();
//		if (m_ToolBox->SendMessage( msgHash_RegisterLight, sizeof(pLight), pLight ) != MSG_HANDLED)
//			continue;

		RENDERLISTTOTEXTUREPARAMS params;

		// render to depth texture
		OBJECTLIST::iterator itrObj = occluderList.begin();
		while (itrObj != occluderList.end())
		{
			IRenderObject *pRenderObj = dynamic_cast<IRenderObject*>(*itrObj);
			if (pRenderObj)
			{
				if (pRenderObj->GetSceneSettings())
				{
//					pRenderObj->GetSceneSettings()->PushFlagState();
//					pRenderObj->GetSceneSettings()->SetShaderFlags( 0 );
//					pRenderObj->GetSceneSettings()->AddShaderFlag( AMBIENT_OCCLUSION_CAST );
//					pRenderObj->GetSceneSettings()->RemoveShaderFlag( SHADOW_RECEIVE_HEMI | SHADOW_RECEIVE_OMNI |
//													SHADOW_RECEIVE_DIR | SHADOW_RECEIVE | LIGHT_34 | LIGHT_12 | ALPHA);
//					pRenderObj->GetSceneSettings()->AddShaderFlag( SHADOW_CAST_OMNI );

					// force a light on the object
					pRenderObj->DetachAllLights();
					pRenderObj->AttachLight( pLight );

					itrObj++;
					continue;
				}
			}

			itrObj = occluderList.erase( itrObj );
		}

//		static DWORD msgHash_GenerateLightPerspectiveMap = CHashString(_T("GenerateLightPerspectiveMap")).GetUniqueID();
//		if (m_ToolBox->SendMessage( msgHash_GenerateLightPerspectiveMap, sizeof(pLight), pLight ) != MSG_HANDLED)
//			continue;

		// set camera position and direction to face the object from the light's position
		static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
		if (m_ToolBox->SendMessage( msgHash_SetGlobalPosition, sizeof(Vec3), &pLight->position, &m_hszLightCameraName, &CHashString(_T("CCamera")) ) != MSG_HANDLED)
			continue;
		static DWORD msgHash_SetLookAtPosition = CHashString(_T("SetLookAtPosition")).GetUniqueID();
		if (m_ToolBox->SendMessage( msgHash_SetLookAtPosition, sizeof(Vec3), &targetPoint, &m_hszLightCameraName, &CHashString(_T("CCamera")) ) != MSG_HANDLED)
			continue;

		Matrix4x4 mCameraView;
		static DWORD msgHash_GetCameraViewMatrix = CHashString(_T("GetCameraViewMatrix")).GetUniqueID();
		if (m_ToolBox->SendMessage( msgHash_GetCameraViewMatrix, sizeof(Matrix4x4), &mCameraView, &m_hszLightCameraName, &CHashString(_T("CCamera")) ) != MSG_HANDLED)
			continue;

		Matrix4x4 mCameraProj;
		static DWORD msgHash_GetCameraProjectionMatrix = CHashString(_T("GetCameraProjectionMatrix")).GetUniqueID();
		if (m_ToolBox->SendMessage( msgHash_GetCameraProjectionMatrix, sizeof(Matrix4x4), &mCameraProj, &m_hszLightCameraName, &CHashString(_T("CCamera")) ) != MSG_HANDLED)
			continue;

		mCameraView.SetInverse();
		m_pAmbientOcclusionShadowEffect->SetVertexConstant( 20, mCameraView.m, 4 );
		m_pAmbientOcclusionShadowEffect->SetVertexConstant( 24, mCameraProj.m, 4 );

		m_pRenderer->SetBlendMode( BLEND_NORMAL );
		m_pRenderer->SetColorMask( true, true, true, true );
		m_pRenderer->SetBackgroundColor( 255, 255, 255 );
		params.dwSortMethod = 0;
		params.objList = &occluderList;
		params.RenderTexture = m_pDepthTexture;
		params.Camera = &m_hszLightCameraName;
		params.effectOverride = m_pAmbientOcclusionShadowEffect;
		params.clearDepth = true;
		params.clearTarget = true;
		m_ToolBox->SendMessage( msgHash_RenderListToTexture, sizeof(params), &params );

		for (OBJECTLIST::iterator itrObj=occluderList.begin(); itrObj != occluderList.end(); itrObj++)
		{
			IRenderObject *pRenderObj = dynamic_cast<IRenderObject*>(*itrObj);
			if (pRenderObj)
			{
//				pRenderObj->GetSceneSettings()->PopFlagState();
				pRenderObj->DetachAllLights();
			}
		}

		// render to accum texture
		for (OBJECTLIST::iterator itrObj=targetList.begin(); itrObj != targetList.end(); itrObj++)
		{
			IRenderObject *pRenderObj = dynamic_cast<IRenderObject*>(*itrObj);
			if (pRenderObj)
			{
				pRenderObj->GetSceneSettings()->PushFlagState();
				pRenderObj->GetSceneSettings()->SetShaderFlags( 0 );
				pRenderObj->GetSceneSettings()->AddShaderFlag( AMBIENT_OCCLUSION_RECEIVE );

				// force a light on the object
				pRenderObj->DetachAllLights();
				pRenderObj->AttachLight( pLight );
			}
		}

		m_pRenderer->SetBlendMode( BLEND_ADDITIVE );
		m_pRenderer->SetColorMask( true, true, true, true );
		m_pRenderer->SetBackgroundColor( 255, 255, 255 );
		m_pRenderer->PushMatrix( PROJECTION_MATRIX );
		m_pRenderer->PushMatrix( WORLD_MATRIX );
		m_pRenderer->PushMatrix( VIEW_MATRIX );
		m_pRenderer->SetOrtho2DScreenSize(1, 1);

		m_pRenderer->SetTexture( 0, m_pDepthTexture );

		m_pRenderer->SetRenderState( RENDERSTATE_CULLMODE, RENDERSTATEPARAM_CULLNONE );

		params.dwSortMethod = 0;
		params.objList = &targetList;
		params.RenderTexture = pOutTexture;
		params.Camera = &m_hszLightCameraName;
		params.effectOverride = m_pAmbientOcclusionEffect;
		params.clearDepth = true;
		params.clearTarget = (pass == 0);
		m_ToolBox->SendMessage( msgHash_RenderListToTexture, sizeof(params), &params );

		m_pRenderer->PopMatrix( PROJECTION_MATRIX );
		m_pRenderer->PopMatrix( WORLD_MATRIX );
		m_pRenderer->PopMatrix( VIEW_MATRIX );

		for (OBJECTLIST::iterator itrObj=targetList.begin(); itrObj != targetList.end(); itrObj++)
		{
			IRenderObject *pRenderObj = dynamic_cast<IRenderObject*>(*itrObj);
			if (pRenderObj)
			{
				pRenderObj->GetSceneSettings()->PopFlagState();
				pRenderObj->DetachAllLights();
			}
		}

		pass++;
	}

	// Unregister lights
/*	for (itrLight = lightList.begin(); itrLight != lightList.end(); itrLight++)
	{
		CAmbientOcclusionLight *pLight = &(*itrLight);

		static DWORD msgHash_UnregisterLight = CHashString(_T("UnregisterLight")).GetUniqueID();
		if (m_ToolBox->SendMessage( msgHash_UnregisterLight, sizeof(pLight), pLight ) != MSG_HANDLED)
			continue;
	}
*/
	// restore settings
	m_pRenderer->SetPolygonZBias(0,0);
	m_pRenderer->SetColorMask( true, true, true, true );
}

void CAmbientOcclusionEditorRenderObject::RenderLightSampleList( LIGHTSAMPLELIST &lightList )
{
	LIGHTSAMPLELIST::iterator itrLight;
	for (itrLight = lightList.begin(); itrLight != lightList.end(); itrLight++)
	{
		CAmbientOcclusionLight *pLight = &(*itrLight);

		m_pRenderer->DrawSphere( pLight->position, 1.0f, 255, 255, 0 );
	}
}

bool CAmbientOcclusionEditorRenderObject::Render( UINT pass, IEffect * override )
{
	if (pass == 0 && m_pRenderer)
	{
//		RenderLightSampleList();
	}
	return true;
}

bool CAmbientOcclusionEditorRenderObject::IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass )
{
	return true;
}

DWORD CAmbientOcclusionEditorRenderObject::GetRenderPriority()
{
    return 0;
}

UINT CAmbientOcclusionEditorRenderObject::GetNumPass()
{
	return 1;
}

IHashString * CAmbientOcclusionEditorRenderObject::GetBaseSortClass()
{
	return GetComponentType();
}

void CAmbientOcclusionEditorRenderObject::GetBoundingSphere( Vec3 &position, float &radius )
{
	position.Set( 0,0,0 );
	radius = 999999.0f;
}

bool CAmbientOcclusionEditorRenderObject::IsAlphaObject()
{
	return false;
}

IMeshObject* CAmbientOcclusionEditorRenderObject::GetMeshObject()
{
	return NULL;
}
