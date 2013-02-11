///============================================================================
/// \file		RenderContext.h
/// \brief		Helper wrapper class for for interaction with EE render context.
/// \date		09-26-2007
/// \author		Andrey Ivanov
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

CRenderContext::CRenderContext()
	: m_pToolBox(EngineGetToolBox())
	, m_pRenderContext(NULL)
	, m_uiRenderContextID(0)
{
}

CRenderContext::~CRenderContext()
{
	Destroy();
}

///	\brief	Create render context
/// \param  hWnd - handle of window for rendering
/// \param  fullscreen - rendering mode for window
///	\return	true if render context was created
bool CRenderContext::Create(HWND hWnd, bool fullscreen)
{
	if (m_pRenderContext != NULL)
	{
		ASSERT(!"Create context called twice");
		return false;
	}
	CRect rc;
	GetClientRect(hWnd, rc);
	// create render context
	CREATERENDERCONTEXTPARAMS crcp; 
	crcp.hwindow = hWnd;
	crcp.fullscreen = fullscreen;
	crcp.colorbits = 32; 
	crcp.depthbits = 24; 
	crcp.width = rc.Width();
	crcp.height = rc.Height();
	static DWORD msgHash_CreateRenderContext = CHashString(_T("CreateRenderContext")).GetUniqueID();
	if (MSG_HANDLED != m_pToolBox->SendMessage(msgHash_CreateRenderContext, sizeof(crcp), &crcp))
	{
		m_pToolBox->Log(LOGERROR, _T("%s(%i): Failed to create render context.\n"), __FILE__, __LINE__ );
		return false;
	}
	m_uiRenderContextID = crcp.uiRenderContextID;
	m_pRenderContext = crcp.context;

	m_pRenderContext->SetBGColor(64,64,64);
	return true;
}

///	\brief	Destroy render context
void CRenderContext::Destroy()
{
	if (m_pRenderContext != NULL)
	{
		CHashString hszEmpty(_T(""));
		SetActiveCamera(&hszEmpty);
		// Delete render context
		static DWORD msgHash_DeleteRenderContext = CHashString(_T("DeleteRenderContext")).GetUniqueID();
		m_pToolBox->SendMessage(msgHash_DeleteRenderContext, sizeof(m_uiRenderContextID), &m_uiRenderContextID);
		m_pRenderContext = NULL;
	}
}

///	\brief	Provide access to raw IRenderContext pointer
///	\return	pointer to created IRenderContext pointer
IRenderContext *CRenderContext::GetRenderContext()
{
	if (m_pRenderContext == NULL)
	{
		ASSERT(!"Render context should be created first");
		return NULL;
	}
	return m_pRenderContext;
}


/// \brief	Set scene render context
/// \param *pSceneName - ptr to the IHashString of scene name
///	\return	true if scene was set
bool CRenderContext::SetScene(IHashString *pSceneName)
{
	if (m_pRenderContext == NULL)
	{
		ASSERT(!"Render context should be created first");
		return false;
	}
	if (pSceneName == NULL)
	{
		ASSERT(!"NULL passed as scene name");
		return false;
	}
	// set model viewer scene's render context
	SETSCENERENDERCONTEXTPARAMS ssrcp;
	ssrcp.pSceneName = pSceneName;
	ssrcp.pRenderContext = m_pRenderContext;
	static DWORD msgHash_SetSceneRenderContext = CHashString(_T("SetSceneRenderContext")).GetUniqueID();
	if (MSG_HANDLED != m_pToolBox->SendMessage(msgHash_SetSceneRenderContext, sizeof(ssrcp), &ssrcp))
	{
		m_pToolBox->Log(LOGERROR, _T("%s(%i): Failed to set scene render context.\n"), __FILE__, __LINE__ );
		return false;
	}

	ADDRENDERLISTPARAMS arlp;
	OBJECTLIST ol; // empty object list
	arlp.dwListPriorityKey = RENDERPRIORITY_FRONTBUFFER;
	arlp.dwSortMethod = SORT_FIFO;
	arlp.objList = &ol;
	arlp.renderContext = m_pRenderContext;
	static DWORD msgHash_AddRenderList = CHashString(_T("AddRenderList")).GetUniqueID();
	if (MSG_HANDLED != m_pToolBox->SendMessage(msgHash_AddRenderList, sizeof(ADDRENDERLISTPARAMS), &arlp))
	{
		m_pToolBox->Log(LOGERROR, _T("%s(%i): Failed to add render list.\n"), __FILE__, __LINE__ );
		return false;
	}
	return true;
}

/// \brief	Set active camera in this context
/// \param *pCameraName - ptr to the IHashString of camera name
///	\return	true if active camera was set
bool CRenderContext::SetActiveCamera(IHashString *pCameraName)
{
	if (m_pRenderContext == NULL)
	{
		ASSERT(!"Render context should be created first");
		return false;
	}
	if (pCameraName == NULL)
	{
		ASSERT(!"NULL passed as camera name");
		return false;
	}
	CONTEXTCAMPARAMS ccp;
	ccp.pRC = m_pRenderContext;
	ccp.szCameraName = pCameraName;
	static DWORD msgHash_SetActiveCamera = CHashString(_T("SetActiveCamera")).GetUniqueID();
	if (MSG_HANDLED != m_pToolBox->SendMessage(msgHash_SetActiveCamera, sizeof(CONTEXTCAMPARAMS), &ccp))
	{
		m_pToolBox->Log(LOGERROR, _T("%s(%i): Failed to set active camera.\n"), __FILE__, __LINE__ );
	}
	return true;
}

/// \brief	Set active camera in this context
/// \param  viewCamera - reference to valid CViewCamera object
///	\return	true if active camera was set
bool CRenderContext::SetActiveCamera(const CViewCamera &viewCamera)
{
	ASSERT(viewCamera.GetCameraName() != NULL);
	ASSERT(_tcslen(viewCamera.GetCameraName()) > 0);
	CHashString hszCameraName = viewCamera.GetCameraName();
	return SetActiveCamera(&hszCameraName);
}

///	\brief	Set camera position in context
/// \param  position - new camera position
void CRenderContext::SetCameraPosition(Vec3 &position)
{
	CONTEXTCAMVECTORPARAMS ccvp;
	ccvp.pRC = m_pRenderContext;
	ccvp.vdata = &position;
	ccvp.rotation = NULL;
	static DWORD msgHash_SetActiveCameraPosition = CHashString(_T("SetActiveCameraPosition")).GetUniqueID();
	if (MSG_HANDLED != m_pToolBox->SendMessage(msgHash_SetActiveCameraPosition, sizeof(CONTEXTCAMVECTORPARAMS), &ccvp))
	{
		m_pToolBox->Log(LOGWARNING, _T("%s(%i): SetCameraPosition failed. \n"), __FILE__, __LINE__ );
	}
}

///	\brief	Set camera orientation in context
/// \param  orientation - new camera orientation
void CRenderContext::SetCameraOrientation(EulerAngle &orientation)
{
	CONTEXTCAMVECTORPARAMS ccvp;
	ccvp.pRC = m_pRenderContext;
	ccvp.vdata = NULL;
	ccvp.rotation = &orientation; 
	static DWORD msgHash_SetActiveCameraOrientation = CHashString(_T("SetActiveCameraOrientation")).GetUniqueID();
	if (MSG_HANDLED != m_pToolBox->SendMessage(msgHash_SetActiveCameraOrientation, sizeof(CONTEXTCAMVECTORPARAMS), &ccvp))
	{
		m_pToolBox->Log(LOGWARNING, _T("%s(%i): SetCameraOrientation failed. \n"), __FILE__, __LINE__ );
	}
}

///	\brief	draw current render content
/// \param  orientation - new camera orientation
void CRenderContext::Draw()
{
	if (m_pRenderContext == NULL)
	{
		ASSERT(!"Render context should be created first");
		return;
	}
	static DWORD msgHash_DrawToRenderContext = CHashString(_T("DrawToRenderContext")).GetUniqueID();
	m_pToolBox->SendMessage(msgHash_DrawToRenderContext, sizeof(m_uiRenderContextID), &m_uiRenderContextID, NULL, NULL);
}

///	\brief	set new view port size
/// \param  cx - new view width
/// \param  cy - new view height
void CRenderContext::SetWindowSize(int cx, int cy)
{
	if (m_pRenderContext != NULL)
	{
		// TODO: AI. Check for memory leaks?
		//m_pRenderContext->CreateContext(*this, cx, cy, 24, 32);
	}

	VIEWPORTPARAMS gwrp;
	gwrp.offsetx = 0;
	gwrp.offsety = 0;
	gwrp.width = cx;
	gwrp.height = cy;
	static DWORD msgHash_SetWindowSize = CHashString(_T("SetWindowSize")).GetUniqueID();
	if (MSG_HANDLED != m_pToolBox->SendMessage(msgHash_SetWindowSize, sizeof(VIEWPORTPARAMS), &gwrp, NULL, NULL))
	{
		m_pToolBox->Log(LOGWARNING, _T("%s(%i): SetWindowSize failed. \n"), __FILE__, __LINE__ );
	}
}