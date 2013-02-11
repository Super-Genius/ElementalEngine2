///============================================================================
/// \file		ModelViewComponent.cpp
/// \brief		Header file for <insert brief description here>
/// \date		04-22-2005
/// \author		Kenneth Hurley
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

// registers ourselves with Elemental Engine, so we get messages
// it's not really a singleton to us, but to EE it is.
REGISTER_COMPONENT_SINGLETON(CModelViewComponent);
// the messages we receive
REGISTER_MESSAGE_HANDLER(GlobalUpdateTick, OnGlobalUpdateTick, CModelViewComponent);
REGISTER_MESSAGE_HANDLER(WindowsCmdMsg, OnCmdMsg, CModelViewComponent);
REGISTER_MESSAGE_HANDLER(FileLoaded_cfg, OnFileLoaded, CModelViewComponent);
REGISTER_MESSAGE_HANDLER(FileLoaded_hrc, OnFileLoaded, CModelViewComponent);
REGISTER_MESSAGE_HANDLER(WindowsMainCreate, OnCreate, CModelViewComponent);
REGISTER_MESSAGE_HANDLER(EditModelView, OnEditModelView, CModelViewComponent);
REGISTER_MESSAGE_HANDLER(TriggerEvent, OnAnimationCallback, CModelViewComponent);

// get the singleton for this
IComponent *CModelViewComponent::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CModelViewComponent);
}

// serialize function for this object
void CModelViewComponent::Serialize(IArchive &ar)
{
}

// get component name for this object
IHashString *CModelViewComponent::GetComponentType()
{
	static CHashString compName(_T("CModelViewComponent"));
	return &compName;
}

bool CModelViewComponent::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

// constructor for ModelView component (interface to Elemental Engine)
void CModelViewComponent::SetParent(CModelViewEditor *parent)
{
	m_Parent = parent;
}

DWORD CModelViewComponent::OnGlobalUpdateTick(DWORD size, void *param)
{
    VERIFY_MESSAGE_SIZE(size, sizeof(GLOBALTICKPARAMS));

	m_Parent->Update();

	return MSG_HANDLED_PROCEED;
}

// message to handle OnCmdMsgs from main window
DWORD CModelViewComponent::OnCmdMsg(DWORD size, void *param)
{
	CMDMESSAGE *pCmdMsg;
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDMESSAGE));
	pCmdMsg = (CMDMESSAGE *)param;

	// forward to parent.
	if (m_Parent->OnCmdMsg(pCmdMsg->m_nID, pCmdMsg->m_nCode, pCmdMsg->m_pExtra,
		pCmdMsg->m_pHandlerInfo))
	{
		pCmdMsg->retVal = true;
		return MSG_HANDLED_PROCEED;
	}

	// it wasn't handled by parent, so return that
	return MSG_NOT_HANDLED;
}

// message to handle WindowsCreate function
DWORD CModelViewComponent::OnCreate(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CMDIFrameWnd *));
	m_Parent->OnCreate((CMDIFrameWnd *)param);

	return MSG_HANDLED_PROCEED;
}

DWORD CModelViewComponent::OnFileLoaded(DWORD size, void *param)
{
	// verify message size
	VERIFY_MESSAGE_SIZE(size, sizeof(LPCTSTR));

	LPCTSTR szFilename = reinterpret_cast<LPCTSTR>(param);
	// try to open passed
	m_Parent->EditDocument(szFilename);
	return MSG_HANDLED_PROCEED;
}

// message to handle editing of ModelView document
DWORD CModelViewComponent::OnEditModelView(DWORD size, void *param)
{
	// verify message size
	//VERIFY_MESSAGE_SIZE(size, sizeof(XXXXX));

	// TODO: Process parameter?
	// *param would contain pointer to loaded document
	m_Parent->EditDocument(NULL);

	return MSG_HANDLED_PROCEED;
}

void CModelViewComponent::PlayAnimation(CModelViewRender *pView)
{
	static CHashString hszCal3DRenderObject = _T("Cal3DRenderObject");
	IToolBox *pToolBox = EngineGetToolBox();
	IHashString *pEntity = pView->GetEntityName();

	static DWORD msgStopAllAnimationActions = CHashString(_T("StopAllAnimationActions")).GetUniqueID();
	pToolBox->SendMessage( msgStopAllAnimationActions, 0, NULL, pEntity, &hszCal3DRenderObject );

	RENDEROBJECTANIMATIONPARAMS roap;
	roap.AnimationID = pView->GetCurrentAnimationID();
	roap.bLoop = false;
	roap.LockLastFrame = true;
	static DWORD msgHash_PlayAnimation = CHashString(_T("PlayAnimation")).GetUniqueID();
	DWORD res = pToolBox->SendMessage(msgHash_PlayAnimation, sizeof(roap), &roap, pEntity, &hszCal3DRenderObject);
	if (MSG_HANDLED != res)
	{
		// TODO: reproduce this situation and check for memory leak
		//pArchive->Close();
		pToolBox->Log(LOGERROR, _T("%s(%i): PlayAnimation not handled. \n"), __FILE__, __LINE__);
	}
}

DWORD CModelViewComponent::OnAnimationCallback(DWORD size, void *param)
{
	TRIGGEREVENTPARAMS *tevent;
	VERIFY_MESSAGE_SIZE(size, sizeof(*tevent));
	tevent = reinterpret_cast<TRIGGEREVENTPARAMS *>(param);
	ASSERT(tevent != NULL);

	// check passed archive
	IArchive *pArchive = tevent->EventParamsArchive;
	if (pArchive == NULL)
	{
		ASSERT(!"Trigger event archive was not passed");
		// this is unexpected behavior
		return MSG_ERROR;
	}

	pArchive->SetIsWriting(false);
	pArchive->SeekTo(0);
	StdString sEntityName;
	pArchive->Read(sEntityName);

	ASSERT(!sEntityName.empty());

	CModelViewRender *pView = NULL;
	pView = SINGLETONINSTANCE(CModelViewEditor)->FindView(sEntityName.c_str());
	if (pView)
	{
		pView->SetFlagPlayNextInSequence();
	}

	return MSG_HANDLED_STOP;
}

DWORD CModelViewComponent::OnCreateModelThumbnail(DWORD size, void *param)
{
	//return MSG_NOT_HANDLED;
	VERIFY_MESSAGE_SIZE(size, sizeof(CREATETHUMBNAILPARAMS));
	CREATETHUMBNAILPARAMS * ctp = (CREATETHUMBNAILPARAMS *)param;

	static int thumbnailNumber;

	// declaration
	//CPoint						m_ptLastMousePos;
	CViewCamera					m_ViewCamera;
	//CRenderContext				m_RenderContext;
	Vec3						m_v3CameraPos(0.0f, 0.0f, 200.0f);
	EulerAngle					m_v3Rotation(0.0f, 0.0f, 0.0f);		// Model's rotation HACK
	Vec3						m_v3Position;			// Model's rotation HACK

	/*float						m_fZoomDistance;
	float						m_fScaleOffset;
	float						m_fScaleZoom;*/

	// scene object names
	CHashString					m_hszSceneName(_T("ModelThumbnailScene"));
	CHashString					m_hszModelName;
	CHashString					m_hszEntityName(_T("ModelThumbnailEntity"));
	CHashString					m_hszLightEntityName(_T("ModelThumbnailLightEntity"));
	CHashString					m_hszLightObjectName(_T("ModelThumbnailLight"));

	// ctor up :).

	// oncreate
	static CHashString hszSceneType(_T("CWorld"));
	if (!CreateEEObject(NULL, &hszSceneType, m_hszSceneName, NULL))
	{
		return MSG_NOT_HANDLED;
	}

	// Create texture.
	CREATETEXTUREPARAMS ct;
	// TODO: Add unique name ( + model name )
	// or remove it after usage with REMOVETEXTUREPARAMS
	StdString l_name(_T("model_thumbnail_"));
	CHashString ThumbnailName(l_name.c_str());

	ct.Name = &ThumbnailName;
	ct.sizeX = 64;
	ct.sizeY = 64;
	ct.bitDepth = 32;
	static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_CreateTexture, sizeof(ct), &ct, &m_hszModelName) != MSG_HANDLED)
	{
		return MSG_NOT_HANDLED;
	}

	/// end of create texture

	// InitRenderScene

	// set active scene to model view scene so objects created here will be placed in the correct scene
	//GETSETACTIVESCENE gsas;
	//gsas.pSceneName = &m_hszSceneName;
	//static DWORD msgHash_SetActiveScene = CHashString(_T("SetActiveScene")).GetUniqueID();
	//EngineGetToolBox()->SendMessage(msgHash_SetActiveScene, sizeof(gsas), &gsas );

	//	gri.m_RendererInterface->BeginScene( false );

	//Create Cal3DRenderObject from loaded file
	//StdString szEntityType(_T("EditorObject"));

	// Create Instance of CEntity 
	//static CHashString hszEntity(_T("CEntity"));
	//CreateEEObject(&m_hszSceneName, &hszEntity, m_hszEntityName, NULL);

	//LoadModel();
	CString strPath = ctp->lpszPathName;
	LPCTSTR szPath = strPath;
	TCHAR extStr[_MAX_EXT];
	TCHAR fileName[_MAX_FNAME];

	// some error handling
	if (strPath.IsEmpty())
	{
		EngineGetToolBox()->Log(LOGWARNING, _T("%s(%i):OnCreateModelThumbnail shouldn't have been called with NULL path\n"), __FILE__, __LINE__);
		return MSG_NOT_HANDLED;
	}

	// determine the file extension
	_tsplitpath(szPath, NULL, NULL, fileName, extStr);

	// Let's omit this buggy attemt:
/*
	size_t nPathLen = _tcslen(szPath) - 4;
	ASSERT(nPathLen >= 4);
	LPCTSTR szExtension = szPath + nPathLen;

	StdString szFilename = FileHelpers::GetName(szPath);
*/

	StdString szFilename = fileName;
	szFilename += extStr;
	m_hszModelName = szFilename;

	CFileVersionSetter setter( _T( "2.5" ) );

	if ( _tcsicmp( extStr, _T( ".cfg" ) ) == 0 )
	{
		IArchive *pArchive = CreateMemoryArchive();
		if (pArchive != NULL)
		{
			pArchive->Write(szPath, _T("Filepath"));
			pArchive->SetIsWriting(false);
			static CHashString hszCal3DRenderObject(_T("Cal3DRenderObject"));
			CreateEEObject(&m_hszEntityName, &hszCal3DRenderObject, m_hszModelName, pArchive);
			pArchive->Close();
		}
	}
	else if (_tcsicmp(extStr, _T(".hrc")) == 0)
	{
		IArchive *pArchive = CreateMemoryArchive();
		if (pArchive != NULL)
		{
			pArchive->Write(szPath);
			pArchive->SetIsWriting(false);
			static CHashString hszCal3DRenderObject(_T("CHierarchicalModel"));
			CreateEEObject(&m_hszEntityName, &hszCal3DRenderObject, m_hszModelName, pArchive);
			pArchive->Close();
		}
	}
	else
	{
		// some error hangling
		ASSERT(!"Unknown extension");		
		EngineGetToolBox()->Log(LOGWARNING, _T("%s(%i):OnCreateModelThumbnail was passed %s extension, which is not recognized\n"), __FILE__, __LINE__, extStr);
	}
	// end LoadModel

	Vec3 v3Zero(0.0f, 0.0f, 0.0f);
	Vec3 v3EntityLightPosition(0.0f, 200.0f, 200.0f);
	EulerAngle eaZero;

	// Create the CEntity LightObject Parent /////////////////////////////////////////////////////////
	static CHashString hszCEntity(_T("CEntity"));
	if (CreateEEObject(&m_hszSceneName, &hszCEntity, m_hszLightEntityName, NULL))
	{
		int iVersionNumber = 1000;
		Vec3 v3LightPosition( 0.0, 0.0f, 0.0f );
		Vec3 v3LightDirection( 0.0f, 0.0f, 0.0f );
		float fAttenuation = 99999.0f;
		float fColor[] = { 0.75f, 0.75f, 0.75f, 1.0f };
		StdString szLightType(_T("AMBIENT_LIGHT"));
		bool bCastShadows = false;
		UINT numKeys = 0;

		// Create the Light Object ////////////////////////////////////////////////////////////////
		static CHashString hszCLightObject(_T("CLightObject"));	
		CreateEEObject(&m_hszLightEntityName, &hszCLightObject, m_hszLightObjectName, NULL);
	}
	// Insert render to texture here
	//ADDRENDERLISTPARAMS		addRenderListMessage;
	//DWORD retVal;

	//get renderer
	/*GETRENDERERINTERFACEPARAMS RInterface;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	retVal = m_ToolBox->SendMessage(msgHash_GetRendererInterface, sizeof(GETRENDERERINTERFACEPARAMS ),&RInterface);
	if( retVal != MSG_HANDLED 
	|| !RInterface.m_RendererInterface)
	{
	return MSG_NOT_HANDLED;
	}
	//RInterface.m_RendererInterface->SetPolygonZBias( m_fZBiasScale, m_fZBiasOffset );		
	//RInterface.m_RendererInterface->SetPolygonZBias( 1.1f, 5.f );
	RInterface.m_RendererInterface->SetPolygonZBias(1,0);
	RInterface.m_RendererInterface->SetDepthWrite( true );*/

	//set the list for rendering the scene from the light's point of view
	//send an immediate render to texture	
	//set some VEffect constants
	/*if( g_CurrentLPMEffect )
	{
	//supplant the world to screen matrix with the model to light matrix
	Matrix4x4 temp;
	//this is the light matrix
	float p[4];
	light->GetPosition(p);
	temp.SetIdentity();
	//Construct the projection matrix from the light's POV
	//
	//TODO: CHeck bug here
	temp.m[12] = -p[0];
	temp.m[13] = -p[1];
	temp.m[14] = -p[2];
	//D3DXMatrixTranspose(&temp, &temp);
	temp.SetTranspose(); 
	float zclip[4] = {0,0,0,0};
	zclip[ 0 ] = 1.0f;
	zclip[ 1 ] = light->GetAttenuationDistance();

	g_CurrentLPMEffect->SetVertexConstant( C_WORLD_TO_LIGHT, temp.GetMatrix(), 4 );
	g_CurrentLPMEffect->SetVertexConstant( C_ZCLIP, (float*)zclip, 1 );
	RInterface.m_RendererInterface->SetColorMask( true, false, false, false );
	}*/
	/*RInterface.m_RendererInterface->SetBlendMode( BLEND_NORMAL );
	RInterface.m_RendererInterface->SetBackgroundColor( 255,255,255 );*/
	//RENDERMODELTOTEXTUREPARAMS params;
	//params.RenderTexture = static_cast<ITextureObject *>(ct.TextureObjectInterface);
	//params.SceneName = &m_hszSceneName;
	
	IRenderObject * model;
	static DWORD msgHash_GetRenderObjectInterface = CHashString(_T("GetRenderObjectInterface")).GetUniqueID();
	if (MSG_HANDLED != EngineGetToolBox()->SendMessage(msgHash_GetRenderObjectInterface, sizeof(IRenderObject *), &model, &m_hszModelName))
	{
		return MSG_NOT_HANDLED;
	}
	
	GETLIGHTOBJPTRMESSAGE gtopm;
	gtopm.hszObjName = &m_hszLightObjectName;
	static DWORD msgHash_GetLightObjPtrByName = CHashString(_T("GetLightObjPtrByName")).GetUniqueID();
	if (MSG_HANDLED != EngineGetToolBox()->SendMessage(msgHash_GetLightObjPtrByName, sizeof(gtopm), &gtopm))
	{
		return MSG_NOT_HANDLED;
	}
	
	model->AttachLight(gtopm.pLightObject);
	
	// Create camera.
	CHashString m_hszCameraName(_T("ThumbnailCamera"));
	CHashString m_hszObjectCameraName(_T("CreateCamera"));
	/*CreateEEObject(&m_hszLightEntityName, &m_hszObjectCameraName, m_hszCameraName, NULL);
	if (MSG_HANDLED != CreateEEObject(&m_hszSceneName, &m_hszObjectCameraName, m_hszCameraName, NULL))
	{
		return MSG_NOT_HANDLED;
	}*/
	CREATEOBJECTPARAMS cop;
	cop.name = &m_hszCameraName;
	cop.parentName = &m_hszSceneName; 
	static DWORD msgHash_CreateCamera = CHashString(_T("CreateCamera")).GetUniqueID();
	if (MSG_HANDLED != EngineGetToolBox()->SendMessage(msgHash_CreateCamera, sizeof(CREATEOBJECTPARAMS), &cop))
	{
		return MSG_NOT_HANDLED;
	}

	// End of create camera.

	// Set Camera position.
	static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
	if (MSG_HANDLED != EngineGetToolBox()->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), &m_v3CameraPos, &m_hszCameraName))
	{
		return MSG_NOT_HANDLED;
	}
	
	// SetCameraRoatation
	// end 
	OBJECTLIST list;
	list.push_back(model);

	// Insert render to texture here
	RENDERLISTTOTEXTUREPARAMS rltt;

	rltt.dwSortMethod = 0;
	rltt.objList = &list;
	//params.RenderTexture = (ITextureObject*)tex;	
	rltt.RenderTexture = static_cast<ITextureObject *>(ct.TextureObjectInterface);
	rltt.Camera = &m_hszCameraName;
	static DWORD msgHash_RenderListToTexture = CHashString(_T("RenderListToTexture")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_RenderListToTexture, sizeof(rltt), &rltt );
		
	//RInterface.m_RendererInterface->SetPolygonZBias(0,0);

	//unset mask
	//RInterface.m_RendererInterface->SetColorMask( true, true, true, true );




	// End of render to texture.

	ITextureObject * textureObject = rltt.RenderTexture;
	ctp->pThumbnail = new CBitmap();
	void * l_buffer = textureObject->Read();
	ctp->pThumbnail->CreateBitmap(64, 64, 1, 32, l_buffer); 

	DeleteEEObject(&m_hszLightObjectName);
	// Delete Light Object
	DeleteEEObject(&m_hszLightEntityName);
	// Delete CEntity Object
	DeleteEEObject(&m_hszEntityName);
	// Delete Cal3DRenderObject
	DeleteEEObject(&m_hszModelName);
	// Delete Scene
	DeleteEEObject(&m_hszSceneName);

	return MSG_HANDLED_PROCEED;
}
