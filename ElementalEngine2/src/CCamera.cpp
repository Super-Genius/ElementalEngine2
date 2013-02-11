///==========================================================================
/// \file	CCamera.cpp
/// \brief	CCamera implementation
/// \date	2-22-2005
/// \author Kyle Swaim
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
///==========================================================================

#include "StdAfx.h"
#include "CCamera.h"
#include "IRenderObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CCamera);
REGISTER_MESSAGE_HANDLER(UpdateCamera, OnUpdateCamera, CCamera);
REGISTER_MESSAGE_HANDLER(ActivateCamera, OnActivateCamera, CCamera);
REGISTER_MESSAGE_HANDLER(GetCameraFrustum, OnGetCameraFrustum, CCamera);
REGISTER_MESSAGE_HANDLER(MoveCamera, OnMoveCamera, CCamera);
REGISTER_MESSAGE_HANDLER(RotateCamera, OnRotateCamera, CCamera);
REGISTER_MESSAGE_HANDLER(SetLookAtPosition, OnSetLookAtPosition, CCamera);
REGISTER_MESSAGE_HANDLER(GetCameraVectors, OnGetCameraVectors, CCamera);
REGISTER_MESSAGE_HANDLER(SetGlobalPosition, OnSetGlobalPosition, CCamera);
REGISTER_MESSAGE_HANDLER(GetGlobalPosition, OnGetGlobalPosition, CCamera);
REGISTER_MESSAGE_HANDLER(GetGlobalEuler, OnGetGlobalEuler, CCamera);
REGISTER_MESSAGE_HANDLER(SetGlobalEuler, OnSetGlobalEuler, CCamera);
REGISTER_MESSAGE_HANDLER(SetCameraHierarchyStatus, OnSetCameraHierarchyStatus, CCamera);
REGISTER_MESSAGE_HANDLER(SetProjection, OnSetProjection, CCamera);
REGISTER_MESSAGE_HANDLER(SetNearClip, OnSetNearClip, CCamera);
REGISTER_MESSAGE_HANDLER(SetFarClip, OnSetFarClip, CCamera);
REGISTER_MESSAGE_HANDLER(GetCameraProjectionMatrix, OnGetCameraProjectionMatrix, CCamera);
REGISTER_MESSAGE_HANDLER(GetCameraViewMatrix, OnGetCameraViewMatrix, CCamera);
REGISTER_MESSAGE_HANDLER(SetCameraUseBackBufferAspectRatio, OnSetCameraUseBackBufferAspectRatio, CCamera);
REGISTER_MESSAGE_HANDLER(GetNearFarClip, OnGetNearFarClip, CCamera);
REGISTER_MESSAGE_HANDLER(ObjectInCamera, OnObjectInCamera, CCamera);
REGISTER_MESSAGE_HANDLER(SetCameraZoom, OnSetCameraZoom, CCamera);
///
/// Constructor / Destructor
///

// we masquerade as as a CCamera, so we put that through the #define
CCamera::CCamera(IHashString *parentName, IHashString *name, bool bAddToHier ) : 
	SCENEOBJECTTEMPLATE(CCameraManager, CCamera, ICamera, parentName, name)
{
	m_bInHierarchy = bAddToHier;
	if (bAddToHier)
	{
		AddToHierarchy();
	}
	m_bViewFixed = false;
	m_bEntityFixed = false;
	m_vLookAt = Vec3(0,0,0); //init to zeros
	m_vUpVector.Set( 0, 1, 0 );//init to up
	m_vViewVector.Set( 0, 0, 1 );
	m_bFreeze = false;
	m_CamProjection = PERSPECTIVE_CAMERA;
	m_bUpdateFrustum = true;
	m_bUseBackBufferScreenSize = false;

	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;
	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetRendererInterface, 
						sizeof(GETRENDERERINTERFACEPARAMS), 
						&renderInterface, 
						NULL, 
						NULL);
	m_Renderer = renderInterface.m_RendererInterface;

	m_FOVAngle_H = 0.0f;
	m_FOVAngle_V = 90.0f;

	m_fZoom = 1.0f;
	m_fZoomET = 0.0f;
	m_fZoomTime = 0.0f;
	m_fStartZoom = 1.0f;
	m_fEndZoom = 1.0f;
	m_Timer = m_ToolBox->GetTimer();

	m_bZooming = false;
}

CCamera::~CCamera()
{
	if (m_bInHierarchy)
	{
		RemoveFromHierarchy();
	}
	DeInit();
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CCamera::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new CCamera(parentName, name, true);
		}
		else if (nArgs == 3)
		{	
			bAddToHier = va_arg(argptr, bool);
			self = new CCamera(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CCamera::Init( void )
{
	//Nothing to do
}

/// \brief Initializes the camera parameters
void CCamera::BuildCamera( Vec3* position, Vec3* upvector, Vec3* viewvector,
							float nearclip, float farclip, float FOVangle )
{
	Vec3 vectemp;

	m_fFarClip	= farclip;
	m_fNearClip = nearclip;	
	m_FOVAngle_H = FOVangle;
	m_FOVAngle_V = 0;
	vectemp.Set( position->x, position->y, position->z );
	SetPosition( &vectemp );
	m_vUpVector.Set( upvector->x, upvector->y, upvector->z  );
	m_vViewVector.Set( viewvector->x, viewvector->y, viewvector->z );
}

float CCamera::GetVerticalFOV()
{
	VIEWPORTPARAMS param;
	//choose between window size or backbuffer size
	DWORD retval = 0;
	if( m_bUseBackBufferScreenSize )
	{
		static DWORD msgHash_GetVideoSize = CHashString(_T("GetVideoSize")).GetUniqueID();
		retval = EngineGetToolBox()->SendMessage(msgHash_GetVideoSize, sizeof(param), &param );
	}else
	{
		static DWORD msgHash_GetWindowSize = CHashString(_T("GetWindowSize")).GetUniqueID();
		retval = EngineGetToolBox()->SendMessage(msgHash_GetWindowSize, sizeof(param), &param );
	}
	if( retval == MSG_HANDLED )
	{
		float aspectratio;
		if (param.height == 0)
			aspectratio = 1.0;
		else
			aspectratio = (float)param.width/(float)param.height;
		if( m_FOVAngle_V == 0 && m_FOVAngle_H != 0 )
		{
			float radangle = (m_FOVAngle_H / 180.f)*3.1415927f;
			float fFOV_V = 2.0f * atan((tan( radangle / 2.0f) / aspectratio) / m_fZoom);
			//convert to degrees again
			fFOV_V = (fFOV_V / 3.1415927f )*180.f;
			return fFOV_V;
		}
	}

	return m_FOVAngle_V;
}

float CCamera::GetHorizontalFOV()
{
	VIEWPORTPARAMS param;
	DWORD retval = 0;
	if( m_bUseBackBufferScreenSize )
	{
		static DWORD msgHash_GetVideoSize = CHashString(_T("GetVideoSize")).GetUniqueID();
		retval = EngineGetToolBox()->SendMessage(msgHash_GetVideoSize, sizeof(param), &param );
	}else
	{
		static DWORD msgHash_GetWindowSize = CHashString(_T("GetWindowSize")).GetUniqueID();
		retval = EngineGetToolBox()->SendMessage(msgHash_GetWindowSize, sizeof(param), &param );
	}
	if( retval == MSG_HANDLED )
	{
		float aspectratio = (float)param.width/(float)param.height;
		if( m_FOVAngle_H == 0 && m_FOVAngle_V != 0 )
		{
			float radangle = (m_FOVAngle_V / 180.f)*3.1415927f;
			float fFOV_H = 2.0f * atan((tan( radangle / 2.0f) / (1.f / aspectratio)) / m_fZoom);
			//convert to degrees again
			fFOV_H = (fFOV_H / 3.1415927f )*180.f;
			return fFOV_H;
		}
	}

	return m_FOVAngle_H;
}

void CCamera::UpdateFrustum()
{
	if ( !m_bFreeze)
	{
		//Update the frustum here
		m_Frustum.Build( m_vPosition, m_vViewVector, m_vUpVector, GetHorizontalFOV(), GetVerticalFOV(),
			m_fNearClip, m_fFarClip );
	}
	UpdateMatricies();
	m_bUpdateFrustum = false;
}

bool CCamera::Update()
{
	if (m_bZooming)
	{
		static CHashString hsGameTimer(_T("GameDeltaTimer"));
		m_fZoomET += m_Timer->GetTimeDelta(&hsGameTimer);
			
		float lerpDelta = m_fEndZoom - m_fStartZoom;
		if (m_fZoomTime > 0.0f)
		{
			float curPos = m_fZoomET / m_fZoomTime;
			if (curPos > 1.0f)
			{
				curPos = 1.0f;
				m_bZooming = false;
			}

			m_fZoom = m_fStartZoom + (lerpDelta * curPos);
		}
		else
		{
			m_fZoom = m_fEndZoom;
			m_bZooming = false;
		}	

		// TODO: do we want to maintain the externally set FOV? for now, we'll just assume 
		// horizontal fov is supposed to be 90 degrees at a zoom of 1

		// recalc horizontal fov here; vertical will recalc when it needs too
		m_FOVAngle_V = 0.0f;
		m_FOVAngle_H = 2.0f * (atan(1.0f / m_fZoom) * (180.0f / 3.1415927f));
	}

	// if frustum hasn't already been updated...
	UpdateTransform();

	if (m_bUpdateFrustum)
		UpdateFrustum();
	m_bUpdateFrustum = true;

	return true;
}

void CCamera::UpdateTransform()
{
	if (m_bTransformIsDirty)
	{
		if (m_pBaseObject)
		{
			Matrix4x4 attachMat;
			m_pBaseObject->GetAttachTransform(&m_hsAttachNodeName, attachMat);
			static Vec3 trans;
			trans = attachMat.GetTranslation();
			static EulerAngle rot;
			rot = attachMat.GetRotation();
			SetPosition( &trans);
			SetRotation( &rot );
			m_bTransformIsDirty = false;
		}
	}
}

CFrustum * CCamera::GetFrustum()
{
	CFrustum * pfrustum = (CFrustum*)(&m_Frustum);
	return pfrustum;
}

void CCamera::DeInit()
{
	CObjectTemplate<ICamera>::DeInit();
}

void CCamera::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
		ar.Read( m_vPosition, "Position" );
		ar.Read( m_vUpVector, "UpVector" );
		ar.Read( m_vViewVector, "ViewVector" );
		ar.Read( m_FOVAngle_H, "FOVAngle" );
		ar.Read( m_fNearClip, "NearClip" );
		ar.Read( m_fFarClip, "FarClip" );

		SetPosition( &m_vPosition );
		UpdateFromVectors();
	}
	else
	{
		Vec3 &position = GetPosition();
		ar.Write( position, "Position" );
		ar.Write( m_vUpVector, "UpVector" );
		ar.Write( m_vViewVector, "ViewVector" );
		ar.Write( m_FOVAngle_H, "FOVAngle" );
		ar.Write( m_fNearClip, "NearClip" );
		ar.Write( m_fFarClip, "FarClip" );
	}
}

/// Set the Rotation of the camera
void CCamera::SetRotation( float roll, float pitch, float yaw )
{
	//build view vector
	//temp, not implemented yet, will use matrix3x3 axis rotation
	
	//set the orientation
	m_vEulerAngles.Set( roll, pitch, yaw);
	UpdateFromEuler();
	UpdateFrustum();
}

void CCamera::SetRotation( EulerAngle * rot )
{
	SetRotation( rot->GetRoll(), rot->GetPitch(), rot->GetYaw() );
}

/// Sets the Position of the camera
void CCamera::SetPosition( float x, float y, float z )
{
	m_vPosition.x = x;
	m_vPosition.y = y;
	m_vPosition.z = z;
	//rebuild lookat
	m_vLookAt = m_vPosition + m_vViewVector;
	UpdateFrustum();
}

void CCamera::SetPosition( Vec3 * pos )
{
	SetPosition( pos->x, pos->y, pos->z );
}

/// Sets the Look At Vector of the camera, builds the internal view vector
void CCamera::SetLookAt( float x, float y, float z ) //Vec3 &orientUp )
{
	m_vLookAt =  Vec3( x, y, z );
//	m_vLookAt.Normalize();

	//Build view vector
	m_vViewVector = m_vLookAt - m_vPosition;
/*	m_vViewVector.Normalize();

	Vec3 binormal = m_vViewVector.Cross( orientUp );
	binormal.Normalize();

	m_vUpVector = m_vViewVector.Cross( binormal );
	m_vUpVector.normalize();
*/
	UpdateFromVectors();
	UpdateFrustum();
}
void CCamera::SetLookAt( Vec3 * lookat )
{
	SetLookAt( lookat->x, lookat->y, lookat->z );
}

/// Sets the Field of View Angle of the camera in Degrees
void CCamera::SetFOVAngle( float angle )
{
	m_FOVAngle_H = angle;
	m_FOVAngle_V = 0;
	UpdateFrustum();
}

/// Sets the clipping planes of the camera in distance relative to position
void CCamera::SetClipping( float nearclip, float farclip )
{
	m_fNearClip = nearclip;
	m_fFarClip = farclip;
	UpdateFrustum();
}

/// Sets the upvector of the camera. Default up vector is (0,0,1)
void CCamera::SetUpVector( float x, float y, float z )
{
	m_vUpVector = Vec3( x, y, z );
	UpdateFromVectors();
	UpdateFrustum();
}

void CCamera::SetUpVector( Vec3 * upvector )
{
	m_vUpVector = Vec3( upvector->x, upvector->y, upvector->z  );
	UpdateFromVectors();
	UpdateFrustum();
}

void CCamera::GetRightVector( Vec3 *right )
{
	Vec3 vRight = m_vViewVector.Cross( m_vUpVector );
	right->Set( vRight );
}

void CCamera::UpdateFromEuler()
{
	// Update Vectors from Euler Angles
	Matrix3x3 rotMatrix;
	Vec3 rotmat1, rotmat2;
	//extract view and up vectors here
	m_vEulerAngles.GetMatrix( rotMatrix );

	rotmat2.Set( rotMatrix.m[0], rotMatrix.m[1], rotMatrix.m[2] );
	rotmat1.Set( rotMatrix.m[3], rotMatrix.m[4], rotMatrix.m[5] );
	m_vViewVector.Set( rotMatrix.m[6], rotMatrix.m[7], rotMatrix.m[8] );
	m_vUpVector = m_vViewVector.Cross( rotmat2 );

	//reset lookat
	m_vLookAt = m_vPosition + m_vViewVector;
}

void CCamera::UpdateFromVectors()
{
	// Update Euler Angles from Vectors
	Vec3 viewVecX, viewVecY;
	//Build the matrix for rotation
	viewVecX = m_vUpVector.Cross( m_vViewVector );    // cross one
	viewVecX.Normalize ();
	viewVecY  = m_vViewVector.Cross( viewVecX ); // cross two
	viewVecY.Normalize ();
	// create roation matrix of the vectors
	Matrix3x3 rotMatrix( viewVecX.x, viewVecX.y, viewVecX.z,
						viewVecY.x, viewVecY.y, viewVecY.z,
						m_vViewVector.x, m_vViewVector.y, m_vViewVector.z );
    //set the euler rotation
	m_vEulerAngles.SetFromMatrix( rotMatrix );

}

/// Sets the view vector of the camera
void CCamera::SetViewVector( float x, float y, float z )
{
	m_vViewVector =  Vec3( x, y, z );
	m_vViewVector.Normalize();			
	//set the new lookat:
	m_vLookAt = m_vPosition + m_vViewVector;

	UpdateFromVectors();
	UpdateFrustum();
}

void CCamera::SetViewVector( Vec3 * viewvector )
{
	SetViewVector( viewvector->x, viewvector->y, viewvector->z );
}

const Vec3* CCamera::GetViewVector()
{
	return &m_vViewVector;
}

const Vec3* CCamera::GetUpVector()
{
	return &m_vUpVector;
}

Vec3& CCamera::GetPosition()
{
	return m_vPosition;
}

EulerAngle& CCamera::GetRotation()
{
	return m_vEulerAngles;
}

float CCamera::GetNearClip()
{
	return m_fNearClip;
}

float CCamera::GetFarClip()
{
	return m_fFarClip;
}

//private methods
void CCamera::Activate()
{
	if( !m_Renderer )
	{
		return;
	}
	// only do this if we haven't done it this frame
	if (m_bRebuildMatricies)
	{
		UpdateMatricies();				
	}
	m_bRebuildMatricies = true;

	m_Renderer->SetMatrix( PROJECTION_MATRIX, m_ProjectionMatrix.GetMatrix());
	m_Renderer->SetMatrix( VIEW_MATRIX, m_ViewMatrix.GetMatrix());	
}

void CCamera::FixView(Vec3 &vPos)
{
	if(m_bEntityFixed)
	{
		UnFixViewOnEntity();
	}
	m_bViewFixed = true;
	m_vLookAt.Set( vPos );
}

void CCamera::UnFixView()
{
	m_bViewFixed = false;
}

void CCamera::FixViewOnEntity(IObject *pObject)
{
	if(m_bViewFixed)
	{
		UnFixView();
	}
	m_bEntityFixed = true;
	//add more
	assert(pObject);
	m_pObjEntity = pObject;
}

void CCamera::UnFixViewOnEntity()
{
	m_bEntityFixed = false;
}

void CCamera::UpdateFixedView()
{
}

DWORD CCamera::OnUpdateCamera(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(UPDATECAMERAPARAMS));
	UPDATECAMERAPARAMS *pParams = (UPDATECAMERAPARAMS*)in_params;

	if (pParams->bSave)
	{
		m_FOVAngle_H = pParams->fHorizontalFOVAngle;
		m_FOVAngle_V = pParams->fVerticalFOVAngle;
		m_fNearClip = pParams->fNearClip;
		m_fFarClip = pParams->fFarClip;

		// params to cam
		if( pParams->vPosition != NULL )
		{
			SetPosition( pParams->vPosition );
		}

		if (pParams->eRotateType == kCameraRotateSetEulerAngles)
		{
			if( pParams->vEuler != NULL )
			{
				SetRotation( pParams->vEuler );

				UpdateFromEuler();
			}
		}
		else if(pParams->eRotateType == kCameraRotateSetVectors)
		{
			if( pParams->vUpVector != NULL )
			{
				m_vUpVector.Set( *pParams->vUpVector );
			}
			if( pParams->vViewVector != NULL )
			{
				m_vViewVector.Set( *pParams->vViewVector );
			}

			UpdateFromVectors();
		}
		
		UpdateFrustum();
	}
	else
	{
		// cam to params
		if( pParams->vPosition )
		{
			pParams->vPosition->Set( GetPosition() );
		}
		if( pParams->vEuler )
		{
			pParams->vEuler->Set( m_vEulerAngles );
		}
		if( pParams->vUpVector )		
		{
			pParams->vUpVector->Set( m_vUpVector );
		}
		if( pParams->vViewVector )
		{
			pParams->vViewVector->Set( m_vViewVector );
		}
		pParams->fHorizontalFOVAngle = m_FOVAngle_H;
		pParams->fVerticalFOVAngle = m_FOVAngle_V;
		pParams->fNearClip = m_fNearClip;
		pParams->fFarClip = m_fFarClip;
	}


	return MSG_HANDLED_STOP;
}

DWORD CCamera::OnSetCameraHierarchyStatus(DWORD size, void *in_params)
{
	bool * res;
	VERIFY_MESSAGE_SIZE( size, sizeof( bool * ) );
	res = (bool*)in_params;
	if( *res && !m_bInHierarchy ) //not in hierarchy but want to put back in
	{
		AddToHierarchy();
		m_bInHierarchy = true;
	}
	else if( !(*res) && m_bInHierarchy ) //in hierarchy but want to remove
	{
		static DWORD msgHash_RemoveObject = CHashString(_T("RemoveObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RemoveObject, sizeof(IHashString*), GetName());
		m_bInHierarchy = false;
	}
	return MSG_HANDLED_STOP;
}


DWORD CCamera::OnActivateCamera(DWORD size, void *in_params)
{
	Activate();
	return MSG_HANDLED_STOP;
}

DWORD CCamera::OnGetCameraFrustum(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CFrustum**));
	CFrustum **pParams = (CFrustum**)in_params;
	*pParams = (CFrustum *)&m_Frustum;
	return MSG_HANDLED_STOP;
}

DWORD CCamera::OnMoveCamera(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(MOVECAMERAPARAMS));
	MOVECAMERAPARAMS *pParams = (MOVECAMERAPARAMS*)in_params;

	if (pParams->bRelative)
	{
		// move in relative coords
		float fForwardDelta = pParams->fForwardDelta;
		float fUpDelta = pParams->fUpDelta;
		float fRightDelta = pParams->fRightDelta;

		Vec3 vRightVector;
		GetRightVector( &vRightVector );

		Vec3 vPosition;
		vPosition.Set( GetPosition() );
		vPosition += m_vUpVector * fUpDelta + 
			vRightVector * fRightDelta + 
			m_vViewVector * fForwardDelta;

		SetPosition( &vPosition );
	}
	else
	{
		// move in absolute coords
		float fForwardDelta = pParams->fForwardDelta;
		float fUpDelta = pParams->fUpDelta;
		float fRightDelta = pParams->fRightDelta;

		Vec3 vPosition;
		vPosition.Set( GetPosition() );
		vPosition.x += fRightDelta;
		vPosition.z += fForwardDelta;
		vPosition.y += fUpDelta;

		SetPosition( &vPosition );
	}

	return MSG_HANDLED_STOP;
}

DWORD CCamera::OnRotateCamera(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ROTATECAMERAPARAMS));
	ROTATECAMERAPARAMS *pParams = (ROTATECAMERAPARAMS*)in_params;

	if (pParams->eRotateOperation == kCameraRotateSetVectors)
	{
		m_vUpVector.Set( *pParams->vUpVector );
		m_vViewVector.Set( *pParams->vViewVector );

		UpdateFromVectors();
	}
	else if (pParams->eRotateOperation == kCameraRotateSetEulerAngles)
	{
		m_vEulerAngles.Set( *pParams->vEuler );

		UpdateFromEuler();
	}
	else if (pParams->eRotateOperation == kCameraRotateAddEulerAngles ||
		pParams->eRotateOperation == kCameraRotateAddEulerAnglesClampPitch)
	{
		m_vEulerAngles.roll += fmod( pParams->vEuler->GetRoll(), 360.0f );
		m_vEulerAngles.pitch += fmod( pParams->vEuler->GetPitch(), 360.0f );
		m_vEulerAngles.yaw += fmod( pParams->vEuler->GetYaw(), 360.0f );

		if (pParams->eRotateOperation == kCameraRotateAddEulerAnglesClampPitch)
		{
			m_vEulerAngles.pitch = Clamp( m_vEulerAngles.pitch, pParams->fClampPitchMin, pParams->fClampPitchMax );
		}

		UpdateFromEuler();
	}

	UpdateFrustum();

	return MSG_HANDLED_STOP;
}

DWORD CCamera::OnSetLookAtPosition(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(sizeof(Vec3), size);
	Vec3 *pos = (Vec3*)in_params;
	m_vLookAt = *pos;

	Vec3 up(0,1,0);
	Vec3 viewDirectionVec, bitangentVec, trueUpVec;
	viewDirectionVec = *pos - m_vPosition;
#ifdef _DEBUG
//	assert( viewDirectionVec.LengthSQ() < 0.0000001 ); // lookat vector is not long enough to generate a matrix
#endif
	// forward direction vector
	viewDirectionVec.Normalize(); 
	// if dir is the same as assumed up vec (fabsf(viewDirectionVec.y) == fabsf(Vec3(0,1,0).Dot(viewDirectionVec))
	if (fabsf(viewDirectionVec.y) > 0.999f) // viewDirectionVec.y is the same thing as (up dot viewDirectionVec) when up is <0,1,0>
	{
		// use a different up so all one of our axis' is not undefined
		up.Set(0,0,-1); // -z axis used so that z is maintained as forward vector (least amount of flipping done for our coord system)
	}
	bitangentVec = up.Cross(viewDirectionVec);
	bitangentVec.Normalize();
	trueUpVec  = viewDirectionVec.Cross(bitangentVec);
	trueUpVec.Normalize();

	// update local values
	m_vUpVector = trueUpVec;
	m_vViewVector = viewDirectionVec;

	UpdateFromVectors();
	UpdateFrustum();

	return MSG_HANDLED_PROCEED;
}

DWORD CCamera::OnGetCameraVectors(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CAMERAVECTORSPARAMS));
	CAMERAVECTORSPARAMS *pParams = (CAMERAVECTORSPARAMS*)in_params;

	if( pParams->vViewVector )pParams->vViewVector->Set( m_vViewVector );
	if( pParams->vUpVector )pParams->vUpVector->Set( m_vUpVector );
	if( pParams->vRightVector )GetRightVector( pParams->vRightVector );

	return MSG_HANDLED_STOP;
}

DWORD CCamera::OnSetGlobalPosition(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(sizeof(Vec3), size);
	Vec3 *pos = (Vec3*)in_params;
	SetPosition( pos );
	return MSG_HANDLED_PROCEED;
}

DWORD CCamera::OnGetGlobalPosition(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(sizeof(Vec3), size);
	Vec3 *pos = (Vec3*)in_params;
	pos->Set( GetPosition() );
	return MSG_HANDLED_STOP;
}

DWORD CCamera::OnSetGlobalEuler(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(sizeof(EulerAngle), size);
	EulerAngle *rot = (EulerAngle*)in_params;
	SetRotation( rot );
	return MSG_HANDLED_PROCEED;
}

DWORD CCamera::OnGetGlobalEuler(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(sizeof(EulerAngle), size);
	EulerAngle *Euler = (EulerAngle*)in_params;
	
	Vec3 viewVecY, viewVecX;
	//Build the matrix for rotation
	viewVecX = m_vUpVector.Cross( m_vViewVector );    // cross one
	viewVecX.Normalize ();
	viewVecY  = m_vViewVector.Cross( viewVecX ); // cross two
	viewVecY.Normalize ();
	// create roation matrix of the vectors
	Matrix3x3 rotMatrix( viewVecX.x, viewVecX.y, viewVecX.z,
						viewVecY.x, viewVecY.y, viewVecY.z,
						m_vViewVector.x, m_vViewVector.y, m_vViewVector.z );
    //set the euler rotation	
	Euler->SetFromMatrix( rotMatrix );
	
	return MSG_HANDLED_STOP;
}

DWORD CCamera::OnSetProjection(DWORD size, void *in_params)
{
	DWORD * enumer;
	VERIFY_MESSAGE_SIZE(sizeof(DWORD), size);
	enumer = (DWORD*)in_params;
	if( *enumer == ORTHO_CAMERA )
	{
		m_CamProjection = ORTHO_CAMERA;
	}
	else
	{		
		m_CamProjection = PERSPECTIVE_CAMERA;
	}
	UpdateFrustum();
	return MSG_HANDLED_STOP;
}
	
bool CCamera::InHierarchy()
{
	return m_bInHierarchy;
}


bool CCamera::IsKindOf(IHashString *compType)
{
	static CHashString camTypeName(_T("CCamera"));

	if (compType->GetUniqueID() == camTypeName.GetUniqueID())
		return true;

	return CObjectTemplate<ICamera>::IsKindOf(compType);
}

DWORD CCamera::OnSetNearClip(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float nearClip = *(float*) param;
	m_fNearClip = nearClip;
	UpdateFrustum();
	return MSG_HANDLED_STOP;
}
	
DWORD CCamera::OnSetFarClip(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float farClip = *(float*) param;
	m_fFarClip = farClip;
	UpdateFrustum();
	return MSG_HANDLED_STOP;
}

void CCamera::UpdateMatricies()
{
	m_bRebuildMatricies = false;

	int width, height;
	m_Renderer->GetWindowDimensions( width, height );
	float fFOV_V = GetVerticalFOV();

	if( m_CamProjection == PERSPECTIVE_CAMERA )
	{		
		//create the projection matrix from the FOVs
		float aspect = 1.0f;
		if (height > 0)
			aspect = (float)width / (float)height;

		m_Renderer->BuildPerspectiveMatrix( fFOV_V, aspect, m_fNearClip, m_fFarClip, m_ProjectionMatrix );
		//create the view matrix from the camera vectors
		m_Renderer->BuildLookAtMatrix( m_vPosition, m_vLookAt, m_vUpVector, m_ViewMatrix );
	}
	else if( m_CamProjection == ORTHO_CAMERA)
	{
		//TODO: somehow find out how to incorporate a width and height into the camera class?
		//right now ortho camera is a unit ortho
		m_Renderer->BuildOrthoMatrix( 0, 1, 0, 1, -1, 1, m_ProjectionMatrix );
		m_ViewMatrix.SetIdentity();
	}
}

DWORD CCamera::OnGetCameraProjectionMatrix(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(Matrix4x4));
	Matrix4x4 *outMat = (Matrix4x4*)param;

	outMat->SetFrom4x4(m_ProjectionMatrix.GetMatrix());

	return MSG_HANDLED_STOP;
}
	
DWORD CCamera::OnGetCameraViewMatrix(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(Matrix4x4));
	Matrix4x4 *outMat = (Matrix4x4*)param;

	outMat->SetFrom4x4(m_ViewMatrix.GetMatrix());

	return MSG_HANDLED_STOP;
}

DWORD CCamera::OnSetCameraUseBackBufferAspectRatio( DWORD size, void * param )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( bool ) );
	bool * bUseBBAspect = (bool*)param;
	if( bUseBBAspect )
	{
		m_bUseBackBufferScreenSize = *bUseBBAspect;
	}
	return MSG_HANDLED_STOP;
}


DWORD CCamera::OnGetNearFarClip( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( Vec3 ) );
	Vec3 * p = (Vec3*)params;
	if( p )
	{
		p->x = m_fNearClip;
		p->y = m_fFarClip;
		return MSG_HANDLED_STOP;
	}
	return MSG_NOT_HANDLED;
}
DWORD CCamera::OnObjectInCamera( DWORD size, void * params )
{
	// Takes in a Message that contains:
	// Global Position of Object
	// Radius of Representation
	// Boolean Return Value
	VERIFY_MESSAGE_SIZE( size, sizeof( OBJECTINCAMERAPARAMS ) );
	OBJECTINCAMERAPARAMS * msg = (OBJECTINCAMERAPARAMS*)params;

	if( msg )
	{
		//Update the frustum here
		Vec3 pos = m_vPosition;
		Vec3 look = m_vLookAt;
		Vec3 diff = m_vLookAt - m_vPosition;
		float fdiff = diff.Length();
		float VerticalArcAngleOffset = msg->fVerticalBuffer / fdiff;
		VerticalArcAngleOffset = VerticalArcAngleOffset * 180.0f / 3.14159265f;
		float HorizontalArcAngleOffset = msg->fHorizontalBuffer / fdiff;
		HorizontalArcAngleOffset = HorizontalArcAngleOffset * 180.0f / 3.14159265f;


		CFrustum Frustum;
		Frustum.Build(	pos, 
						m_vViewVector, 
						m_vUpVector, 
						GetHorizontalFOV() - 2 * HorizontalArcAngleOffset , 
						GetVerticalFOV() - 2 * VerticalArcAngleOffset,
						m_fNearClip, 
						m_fFarClip );

		msg->bReturn = Frustum.IsSphereInView( *msg->v3Position, msg->fRadius );
		return MSG_HANDLED_STOP;
	}
	return MSG_NOT_HANDLED;
}

DWORD CCamera::OnSetCameraZoom(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(CAMERAZOOMPARAMS));

	CAMERAZOOMPARAMS czp = *(CAMERAZOOMPARAMS*)param;

	m_fStartZoom = m_fZoom;
	m_fEndZoom = czp.fZoomFactor;

	m_fZoomTime = czp.fZoomTime;
	m_fZoomET = 0.0f;

	m_bZooming = true;

	return MSG_HANDLED_PROCEED;
}
