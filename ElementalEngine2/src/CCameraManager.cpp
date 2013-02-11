///=====================================================================
/// \file	CCameraManager.cpp
/// \brief	Implementation of the RenderManager
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
///=====================================================================

#include "StdAfx.h"

#include "CCameraManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT_SINGLETON( CCameraManager );

REGISTER_MESSAGE_HANDLER(GetActiveCamera, OnGetActiveCamera, CCameraManager);
REGISTER_MESSAGE_HANDLER(SetActiveCamera, OnSetActiveCamera, CCameraManager);
REGISTER_MESSAGE_HANDLER(GetActiveCameraPosition, OnGetActiveCameraPosition, CCameraManager );
REGISTER_MESSAGE_HANDLER(SetActiveCameraPosition, OnSetActiveCameraPosition, CCameraManager );
REGISTER_MESSAGE_HANDLER(GetActiveCameraOrientation, OnGetActiveCameraOrientation, CCameraManager );
REGISTER_MESSAGE_HANDLER(SetActiveCameraOrientation, OnSetActiveCameraOrientation, CCameraManager );
REGISTER_MESSAGE_HANDLER(Start, OnStart, CCameraManager);
REGISTER_MESSAGE_HANDLER(Stop, OnStop, CCameraManager);
REGISTER_MESSAGE_HANDLER(CreateCamera, OnCreateCamera, CCameraManager );
REGISTER_MESSAGE_HANDLER(GetActiveCameraZMinMax, OnGetActiveCameraZMinMax, CCameraManager );
REGISTER_MESSAGE_HANDLER_PRIORITY( DestroyRenderer, OnDestroyRenderer, CCameraManager, 0x70000000 );
REGISTER_MESSAGE_HANDLER(SetActiveCameraZoom, OnSetActiveCameraZoom, CCameraManager );

#define DEFAULT_CAMERA_NAME "CCamManager_Default_Cam"

/// \brief The Default Constructor for CCameraManager
/// \return void
CCameraManager::CCameraManager() : 
	CManager(_T("CCameraManager"), CAMERAPRIORITY, CAMERAPRIORITY),
	m_DefaultCamera( _T(DEFAULT_CAMERA_NAME) ),
	m_ccameraTypeName(_T("CCamera"))
{
	m_ToolBox = EngineGetToolBox();
	m_DefaultCameraObject = NULL;

	m_bIsRunning = true;

	Init();
}

/// \brief	Function to get this component.  This is a singleton
/// \param	void
/// \return	IComponent*: This Manager
IComponent *CCameraManager::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( CCameraManager );
}

/// \brief The Default Destructor for CCameraManager
/// \return void
CCameraManager::~CCameraManager( )
{
	DeInit();

}


DWORD CCameraManager::OnSetActiveCamera(DWORD size, void *params)
{
//	VERIFY_MESSAGE_SIZE(sizeof(IHashString*), size);
//	IHashString * camSet;
	VERIFY_MESSAGE_SIZE(size, sizeof(CONTEXTCAMPARAMS));
	CONTEXTCAMPARAMS ccp = *(CONTEXTCAMPARAMS*) params;
	if( !ccp.szCameraName || !(ccp.szCameraName->GetString()) )
	{
		InitializeDefaultCamera();
		//set the cur cam to be the default camera
		m_ConCamMap[ccp.pRC] = m_DefaultCameraObject;
		m_ToolBox->Log(LOGWARNING, _T("CCameraManager::OnSetActiveCamera, archive parameter szCameraName is NULL. Setting Default Camera.\n"));
	}
	else
	{
		//set the current camera
		m_ConCamMap[ccp.pRC] = GetCamera( ccp.szCameraName->GetUniqueID() );
	}
	return MSG_HANDLED_STOP;
}

DWORD CCameraManager::OnGetActiveCamera(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CONTEXTCAMPARAMS));
	CONTEXTCAMPARAMS *ccp = (CONTEXTCAMPARAMS*) params;

	if (m_ConCamMap.find(ccp->pRC) == m_ConCamMap.end())
	{
		OnSetActiveCamera(sizeof(CONTEXTCAMPARAMS), ccp);
	}

	if ( ccp->szCameraName != NULL )
	{
		// Both need to be referenced to avoid a pointer copy.
		*ccp->szCameraName = *m_ConCamMap[ccp->pRC]->GetName();
		return MSG_HANDLED_STOP;
	}else
	{
		m_ToolBox->Log(LOGERROR, _T("CCameraManager::OnGetActiveCamera, archive parameter szCameraName is NULL.\n"));
	}

	return MSG_ERROR;
}

DWORD CCameraManager::OnCreateCamera( DWORD size, void * params )
{
	CREATEOBJECTPARAMS * pmsg;
	VERIFY_MESSAGE_SIZE( size, sizeof( CREATEOBJECTPARAMS ) );
	pmsg = (CREATEOBJECTPARAMS*)params;
	if( pmsg )
	{
		
		// find name in obj list
		IDTOOBJECTMAP::iterator objIter;
		IDTOOBJECTMAP *objMap;
		objMap = GetObjectMap( &m_ccameraTypeName  );
		if (objMap)
		{
			IHashString *name = pmsg->name;
			objIter = objMap->find(name->GetUniqueID());
			if( objIter != objMap->end())
			{
				//one already created
				return MSG_HANDLED_STOP;
			}
		}

		CREATEOBJECTPARAMS cop;
		cop.name = pmsg->name;
		cop.parentName = pmsg->parentName;
		cop.typeName = &m_ccameraTypeName;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(cop), &cop);
		static DWORD msgHash_RemoveObject = CHashString(_T("RemoveObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveObject, sizeof(IHashString*), pmsg->name);
		// set initial default camera data
		Vec3 position( 0, 0, -50 );
		Vec3 up( 0, 1, 0 );
		Vec3 view( 0, 0, 1 );

		UPDATECAMERAPARAMS camdata;
		camdata.bSave = true; // write to camera
		camdata.vPosition = &position;
		camdata.eRotateType = kCameraRotateSetVectors;
		camdata.vUpVector = &up;
		camdata.vViewVector = &view;
		camdata.fHorizontalFOVAngle = 90.0f;
		camdata.fVerticalFOVAngle = 0.0f;
		camdata.fNearClip = 1.0f;
		camdata.fFarClip = 8000.0f;
		static DWORD msgHash_UpdateCamera = CHashString(_T("UpdateCamera")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_UpdateCamera, sizeof(camdata), &camdata, pmsg->name, &m_ccameraTypeName);

		INITOBJECTPARAMS iop;
		iop.name = pmsg->name;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}

DWORD CCameraManager::OnGetActiveCameraPosition(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CONTEXTCAMVECTORPARAMS));
	CONTEXTCAMVECTORPARAMS *ccvp = (CONTEXTCAMVECTORPARAMS*) params;

	CONTEXTCAMERAMAP::iterator itrCamera = m_ConCamMap.find(ccvp->pRC);
	if (itrCamera == m_ConCamMap.end())
	{
		return MSG_NOT_HANDLED;
	}
	ICamera *pCamera = itrCamera->second;

	*ccvp->vdata = pCamera->GetPosition();
	return MSG_HANDLED_STOP;
}

DWORD CCameraManager::OnSetActiveCameraPosition(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CONTEXTCAMVECTORPARAMS));
	CONTEXTCAMVECTORPARAMS *ccvp = (CONTEXTCAMVECTORPARAMS*) params;

	CONTEXTCAMERAMAP::iterator itrCamera = m_ConCamMap.find(ccvp->pRC);
	if (itrCamera == m_ConCamMap.end())
	{
		return MSG_NOT_HANDLED;
	}
	ICamera *pCamera = itrCamera->second;

	pCamera->SetPosition( ccvp->vdata );
	return MSG_HANDLED_STOP;
}

DWORD CCameraManager::OnGetActiveCameraOrientation(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CONTEXTCAMVECTORPARAMS));
	CONTEXTCAMVECTORPARAMS *ccvp = (CONTEXTCAMVECTORPARAMS*) params;
	
	CONTEXTCAMERAMAP::iterator itrCamera = m_ConCamMap.find(ccvp->pRC);
	if (itrCamera == m_ConCamMap.end())
	{
		return MSG_NOT_HANDLED;
	}
	ICamera *pCamera = itrCamera->second;

	*ccvp->rotation = pCamera->GetRotation();
	return MSG_HANDLED_STOP;
}

DWORD CCameraManager::OnSetActiveCameraOrientation(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CONTEXTCAMVECTORPARAMS));
	CONTEXTCAMVECTORPARAMS *ccvp = (CONTEXTCAMVECTORPARAMS*) params;

	CONTEXTCAMERAMAP::iterator itrCamera = m_ConCamMap.find(ccvp->pRC);
	if (itrCamera == m_ConCamMap.end())
	{
		return MSG_NOT_HANDLED;
	}
	ICamera *pCamera = itrCamera->second;

	pCamera->SetRotation( ccvp->rotation );
	return MSG_HANDLED_STOP;
}

DWORD CCameraManager::OnGetActiveCameraZMinMax( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( Vec3 ) );
	Vec3 * v = (Vec3*)params;
	if( !v || !m_DefaultCameraObject )
		return MSG_NOT_HANDLED;

	v->x = m_DefaultCameraObject->GetNearClip();
	v->y = m_DefaultCameraObject->GetFarClip();
	return MSG_HANDLED_STOP;
}

// Initializes the manager
bool CCameraManager::Init()
{
	InitializeDefaultCamera();
	return CManager::Init();
}

void CCameraManager::InitializeDefaultCamera()
{
	bool found = false;

	// find name in obj list
	IDTOOBJECTMAP::iterator objIter;
	IDTOOBJECTMAP * objMap;
	objMap = GetObjectMap( &m_ccameraTypeName );
	if( objMap )
	{
		objIter = objMap->find(m_DefaultCamera.GetUniqueID());
		if (objIter != objMap->end())
		{ 
			found = true;
		}
	}

	if (!found)
	{
		IComponent *camComp = m_ToolBox->CreateComponent(&m_ccameraTypeName, 3, &m_DefaultCamera, NULL, false);

		OFACTORYADDPARAMS ofap;
		ofap.name = &m_DefaultCamera;
		ofap.component = camComp;
		static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(OFACTORYADDPARAMS), &ofap );

		// set initial camera data
		Vec3 position( 0, 0, -50 );
		Vec3 up( 0, 1, 0 );
		Vec3 view( 0, 0, 1 );

		UPDATECAMERAPARAMS camdata;
		camdata.bSave = true; // write to camera
		camdata.vPosition = &position;
		camdata.eRotateType = kCameraRotateSetVectors;
		camdata.vUpVector = &up;
		camdata.vViewVector = &view;
		camdata.fHorizontalFOVAngle = 90.0f;
		camdata.fVerticalFOVAngle = 0.0f;
		camdata.fNearClip = 1.0f;
		camdata.fFarClip = 8000.0f;
		static DWORD msgHash_UpdateCamera = CHashString(_T("UpdateCamera")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_UpdateCamera, sizeof(camdata), &camdata, &m_DefaultCamera, &m_ccameraTypeName);

		INITOBJECTPARAMS iop;
		iop.name = &m_DefaultCamera;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
		objMap = GetObjectMap( &m_ccameraTypeName );
		if( objMap )
		{
			objIter = objMap->find(m_DefaultCamera.GetUniqueID());
		}
	}

	if (objMap && (objIter != objMap->end()))
	{
		// Initialize the IObject
		m_DefaultCameraObject = dynamic_cast<ICamera*>(objIter->second);
		m_ConCamMap[NULL] = m_DefaultCameraObject;
	}
	else
	{
		// something went wrong with Initialization
		m_ToolBox->Log(LOGERROR, _T("Unable to initialize default camera for some reason\n"));
	}
}

/// \brief	Update function for the Render Manager
///			Renders immediately, the queued list of renderobjects
/// \return	void
void CCameraManager::Update(  DWORD tickCount  )
{
	if( m_bIsRunning )
	{
		//call's the active camera's update
		//Initialize the IObject
		// find name in obj list
		IDTOOBJECTMAP::iterator objIter;
		IDTOOBJECTMAP * objMap;
		IObject * destObject;
		static CHashString camName( _T("CCamera"));
		objMap = GetObjectMap( &camName);
		if( objMap )
		{
			for( objIter = objMap->begin();objIter != objMap->end(); ++objIter )
			{			
				destObject = objIter->second;
				destObject->Update();
			}
		}
	}
}

/// \brief	DeInitializes the Render Manager
/// \return	bool: Success
bool CCameraManager::DeInit( void )
{
	m_DefaultCameraObject = NULL; // will be deleted by the CManager

	return CManager::DeInit();
}

DWORD CCameraManager::OnStart(DWORD size, void *data)
{
	m_bIsRunning = true;
	
	return MSG_HANDLED_PROCEED;
}

DWORD CCameraManager::OnStop(DWORD size, void *data)
{
	m_bIsRunning = false;
	
	return MSG_HANDLED_PROCEED;
}

DWORD CCameraManager::OnDestroyRenderer(DWORD size, void *data)
{
	DeInit();
	return MSG_HANDLED_PROCEED;
}

DWORD CCameraManager::OnSetActiveCameraZoom(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CONTEXTCAMZOOMPARAMS));

	CONTEXTCAMZOOMPARAMS cczp = *(CONTEXTCAMZOOMPARAMS*)param;

	if (m_ConCamMap[cczp.pRC] != NULL)
	{
		CAMERAZOOMPARAMS czp;
		czp.fZoomFactor = cczp.fZoomFactor;
		czp.fZoomTime = cczp.fZoomTime;

		static DWORD msgSetCameraZoom = CHashString(_T("SetCameraZoom")).GetUniqueID();
		static CHashString hsCamType(_T("CCamera"));
		m_ToolBox->SendMessage(msgSetCameraZoom, sizeof(CAMERAZOOMPARAMS), &czp, m_ConCamMap[cczp.pRC]->GetName(), &hsCamType);
	}

	return MSG_HANDLED_PROCEED;
}