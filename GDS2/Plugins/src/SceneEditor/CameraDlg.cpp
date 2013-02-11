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
#include "SceneEditor.h"
#include "CameraDlg.h"
#include ".\cameradlg.h"


// CCameraDlg dialog

IMPLEMENT_DYNAMIC(CCameraDlg, CDialog)
CCameraDlg::CCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCameraDlg::IDD, pParent)
	, m_fCameraMoveSpeed(40.0f)
{
}

CCameraDlg::~CCameraDlg()
{
}

void CCameraDlg::UpdateTick()
{
	CONTEXTCAMVECTORPARAMS ccvp;
	ccvp.pRC = NULL;
	ccvp.vdata = &m_CameraPosition;
	ccvp.rotation = &m_CameraRotation;

	static DWORD msgHash_GetActiveCameraPosition = CHashString(_T("GetActiveCameraPosition")).GetUniqueID();
	if (MSG_HANDLED != EngineGetToolBox()->SendMessage(msgHash_GetActiveCameraPosition, sizeof(CONTEXTCAMVECTORPARAMS), &ccvp))
	{
		EngineGetToolBox()->Log(LOGWARNING, _T("%s(%i): GetCameraPosition failed. \n"), __FILE__, __LINE__ );
	}
	static DWORD msgHash_GetActiveCameraOrientation = CHashString(_T("GetActiveCameraOrientation")).GetUniqueID();
	if (MSG_HANDLED != EngineGetToolBox()->SendMessage(msgHash_GetActiveCameraOrientation, sizeof(CONTEXTCAMVECTORPARAMS), &ccvp))
	{
		EngineGetToolBox()->Log(LOGWARNING, _T("%s(%i): GetCameraOrientation failed. \n"), __FILE__, __LINE__ );
	}

	UpdateData(FALSE);
}


void CCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CAMERAPOSX, m_CameraPosition.x);
	DDX_Text(pDX, IDC_CAMERAPOSY, m_CameraPosition.y);
	DDX_Text(pDX, IDC_CAMERAPOSZ, m_CameraPosition.z);
	DDX_Text(pDX, IDC_CAMERAROTX, m_CameraRotation.roll);
	DDX_Text(pDX, IDC_CAMERAROTY, m_CameraRotation.pitch);
	DDX_Text(pDX, IDC_CAMERAROTZ, m_CameraRotation.yaw);
	DDX_Text(pDX, IDC_CAMERAMOVESPEED, m_fCameraMoveSpeed);
}

BEGIN_MESSAGE_MAP(CCameraDlg, CDialog)
	ON_EN_CHANGE(IDC_CAMERAPOSX, OnEnChangeCameraPos)
	ON_EN_CHANGE(IDC_CAMERAPOSY, OnEnChangeCameraPos)
	ON_EN_CHANGE(IDC_CAMERAPOSZ, OnEnChangeCameraPos)
	ON_EN_CHANGE(IDC_CAMERAROTX, OnEnChangeCameraRot)
	ON_EN_CHANGE(IDC_CAMERAROTY, OnEnChangeCameraRot)
	ON_EN_CHANGE(IDC_CAMERAROTZ, OnEnChangeCameraRot)
	ON_EN_CHANGE(IDC_CAMERAMOVESPEED, OnEnChangeCameraMovespeed)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CCameraDlg message handlers

void CCameraDlg::OnEnChangeCameraPos()
{
	UpdateData(TRUE);

	CONTEXTCAMVECTORPARAMS ccvp;
	ccvp.pRC = NULL;
	ccvp.vdata = &m_CameraPosition;
	ccvp.rotation = NULL;
	static DWORD msgHash_SetActiveCameraPosition = CHashString(_T("SetActiveCameraPosition")).GetUniqueID();
	if (MSG_HANDLED != EngineGetToolBox()->SendMessage(msgHash_SetActiveCameraPosition, sizeof(CONTEXTCAMVECTORPARAMS), &ccvp))
	{
		EngineGetToolBox()->Log(LOGWARNING, _T("%s(%i): SetCameraPosition failed. \n"), __FILE__, __LINE__ );
	}
}

void CCameraDlg::OnEnChangeCameraRot()
{
	UpdateData(TRUE);

	CONTEXTCAMVECTORPARAMS ccvp;
	ccvp.pRC = NULL;
	ccvp.vdata = NULL;
	ccvp.rotation = &m_CameraRotation;
	static DWORD msgHash_SetActiveCameraOrientation = CHashString(_T("SetActiveCameraOrientation")).GetUniqueID();
	if (MSG_HANDLED != EngineGetToolBox()->SendMessage(msgHash_SetActiveCameraOrientation, sizeof(CONTEXTCAMVECTORPARAMS), &ccvp))
	{
		EngineGetToolBox()->Log(LOGWARNING, _T("%s(%i): SetCameraOrientation failed. \n"), __FILE__, __LINE__ );
	}
}

void CCameraDlg::OnEnChangeCameraMovespeed()
{
	UpdateData(TRUE);

	static DWORD msgHash_SetCameraMoveSensitivity = CHashString(_T("SetCameraMoveSensitivity")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_SetCameraMoveSensitivity, sizeof(float), &m_fCameraMoveSpeed);
}


void CCameraDlg::OnClose()
{
	MessageBox("test", "", MB_OK);
}