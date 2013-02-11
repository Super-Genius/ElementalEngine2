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

#include "stdafx.h"

CViewCamera::CViewCamera()
	: m_hszCameraType(_T("CCamera"))
	, m_bAutocreatedCamera(true)
{
	m_ToolBox = EngineGetToolBox();
	m_fMoveSensitivity = 1.0f;
	m_fRotateSensitivity = 1.0f;
	m_fCameraPitch = 0.0f;
	m_fCameraYaw = 0.0f;
	m_bMouseMove = false;
	m_OldMousePos.SetPoint( 0, 0 );
}

CViewCamera::~CViewCamera()
{
	DeleteCamera();
}

LPCTSTR CViewCamera::GetCameraName() const
{
	return m_hszCameraName.GetString();
}

void CViewCamera::SetCameraName( const TCHAR *szCameraName )
{
	DeleteCamera();
	m_hszCameraName = szCameraName;
}

void CViewCamera::SetMoveSensitivity( float fMoveSensitivity )
{
	m_fMoveSensitivity = fMoveSensitivity;
}

void CViewCamera::SetRotateSensitivity( float fRotateSensitivity )
{
	m_fRotateSensitivity = fRotateSensitivity;
}

void CViewCamera::MoveForward( float fDistance )
{
	MOVECAMERAPARAMS mcp;
	mcp.bRelative = true;
	mcp.fForwardDelta = fDistance * m_fMoveSensitivity;
	static DWORD msgHash_MoveCamera = CHashString(_T("MoveCamera")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_MoveCamera, sizeof(mcp), &mcp, &m_hszCameraName, &m_hszCameraType);

	UpdateFollowers();
}

void CViewCamera::MoveRight( float fDistance )
{
	MOVECAMERAPARAMS mcp;
	mcp.bRelative = true;
	mcp.fRightDelta = fDistance * m_fMoveSensitivity;
	static DWORD msgHash_MoveCamera = CHashString(_T("MoveCamera")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_MoveCamera, sizeof(mcp), &mcp, &m_hszCameraName, &m_hszCameraType);

	UpdateFollowers();
}

void CViewCamera::MoveUp( float fDistance )
{
	MOVECAMERAPARAMS mcp;
	mcp.bRelative = true;
	mcp.fUpDelta = fDistance * m_fMoveSensitivity;
	static DWORD msgHash_MoveCamera = CHashString(_T("MoveCamera")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_MoveCamera, sizeof(mcp), &mcp, &m_hszCameraName, &m_hszCameraType);

	UpdateFollowers();
}

void CViewCamera::Rotate( float fPitch, float fYaw )
{
	float fPitchDelta = fPitch / m_fRotateSensitivity;
	float fYawDelta = fYaw / m_fRotateSensitivity;
	EulerAngle vEuler( 0, fPitchDelta, fYawDelta );
	ROTATECAMERAPARAMS rcp;
	rcp.eRotateOperation = kCameraRotateAddEulerAngles;
	rcp.vEuler = &vEuler;
	static DWORD msgHash_RotateCamera = CHashString(_T("RotateCamera")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RotateCamera, sizeof(rcp), &rcp, &m_hszCameraName, &m_hszCameraType);
}

void CViewCamera::StartMouseMove( CPoint point )
{
	m_bMouseMove = true;
	m_DragStartPoint = point;
}

void CViewCamera::StopMouseMove()
{
	m_bMouseMove = false;
}

void CViewCamera::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint mouseDelta = point - m_OldMousePos;
	m_OldMousePos = point;

	if (m_bMouseMove)
	{
		CPoint dragDelta = point - m_DragStartPoint;
		if (nFlags & MK_CONTROL)
		{
			// camera movement strafe/up+down in camera space
			MoveRight( (float)dragDelta.x );
			MoveUp( (float)dragDelta.y );
		}
		else
		{
			// camera rotation
			Rotate(-(float)dragDelta.y, (float)dragDelta.x);
		}
		m_DragStartPoint = point;
	}
}

void CViewCamera::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	switch (nChar)
	{
		case VK_UP:
		case 'W':
			MoveForward(1.0f);
			break;
		case VK_DOWN:
		case 'S':
			MoveForward(-1.0f);
			break;
		case VK_LEFT:
		case 'A':
			MoveRight(1.0f);
			break;
		case VK_RIGHT:
		case 'D':
			MoveRight(-1.0f);
			break;
	}
}

void CViewCamera::AddFollowingObject( DWORD objectName )
{
	m_Followers.insert( objectName );
	UpdateFollowers();
}

void CViewCamera::RemoveFollowingObject( DWORD objectName )
{
	m_Followers.erase( objectName );
}

void CViewCamera::UpdateFollowers()
{
	for (set<DWORD>::iterator itr=m_Followers.begin(); itr != m_Followers.end(); itr++)
	{
		CHashString hszFollowerName( m_ToolBox->GetHashString(*itr) );

		Vec3 vPos;
		static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
		if (m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &vPos, &m_hszCameraName) == MSG_HANDLED)
		{
			static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), &vPos, &hszFollowerName);
		}
	}
}

///	\brief	create new camera with auto-generated unique name
///	\param	pSceneName - name of scene
///	\return	return true if new camera was created successfully
bool CViewCamera::CreateCamera(IHashString *pSceneName)
{
	DeleteCamera();

	m_hszCameraName = FindUniqueEEObjectName(_T("ViewCamera"));
	CREATEOBJECTPARAMS cop;
	cop.name = &m_hszCameraName;
	cop.parentName = pSceneName;
	static DWORD msgHash_CreateCamera = CHashString(_T("CreateCamera")).GetUniqueID();
	if (MSG_HANDLED != m_ToolBox->SendMessage(msgHash_CreateCamera, sizeof(CREATEOBJECTPARAMS), &cop))
	{
		m_hszCameraName = _T("");
		m_ToolBox->Log(LOGWARNING, _T("%s(%i): CreateCamera failed. \n"), __FILE__, __LINE__ );
		return false;
	}
	m_bAutocreatedCamera = true;
	return true;
}

///	\brief	delete auto-generated camera
void CViewCamera::DeleteCamera()
{
	if (m_bAutocreatedCamera)
	{
		DELETEOBJECTPARAMS dop;
		dop.name = &m_hszCameraName;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(dop), &dop, NULL, NULL);

		m_hszCameraName = _T("");
		m_bAutocreatedCamera = false;
	}
}