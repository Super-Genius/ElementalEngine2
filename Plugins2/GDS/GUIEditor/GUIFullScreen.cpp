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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CGUIFullScreen

IMPLEMENT_DYNAMIC(CGUIFullScreen, CWnd)
CGUIFullScreen::CGUIFullScreen()
{
	m_Doc = NULL;
	m_ToolBox = EngineGetToolBox();
	
	CREATERENDERCONTEXTPARAMS crcp;
	crcp.colorbits = 32;
	crcp.depthbits = 24;
	crcp.height = 0; // resize later
	crcp.width = 0; // resize later
	crcp.hwindow = *this;
	static DWORD msgHash_CreateRenderContext = CHashString(_T("CreateRenderContext")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_CreateRenderContext, sizeof(CREATERENDERCONTEXTPARAMS), &crcp);

	m_RC = crcp.context;
	assert(m_RC);

	m_iRenderContextID = crcp.uiRenderContextID;

	m_RC->SetBGColor(0, 0, 0);

	m_iXRes = 0;
	m_iYRes = 0;

	m_iLastMouseX = 0;
	m_iLastMouseY = 0;

	m_bLeftButtonDown = false;
}

CGUIFullScreen::~CGUIFullScreen()
{
	if (m_iRenderContextID != -1)
	{
		DWORD msgDeleteRenderContext = CHashString(_T("DeleteRenderContext")).GetUniqueID();
		m_ToolBox->SendMessage(msgDeleteRenderContext, sizeof(UINT), &m_iRenderContextID);		
		m_iRenderContextID = -1;
	}
}


BEGIN_MESSAGE_MAP(CGUIFullScreen, CWnd)
	ON_WM_SHOWWINDOW()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()



// CGUIFullScreen message handlers


void CGUIFullScreen::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		CWnd *desktop = GetDesktopWindow();
		assert(desktop);
		RECT rect;
		desktop->GetClientRect(&rect);

		SetWindowPos(NULL, 0, 0, rect.right, rect.bottom, 0);		

		static DWORD msgHash_RenderPageToContext = CHashString(_T("RenderPageToContext")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_RenderPageToContext, sizeof(IRenderContext), m_RC, NULL, NULL);

		ADDRENDERLISTPARAMS arlp;
		OBJECTLIST ol; // empty object list
		arlp.dwListPriorityKey = RENDERPRIORITY_FRONTBUFFER;
		arlp.dwSortMethod = SORT_FIFO;
		arlp.objList = &ol;
		arlp.renderContext = m_RC;
		static DWORD msgHash_AddRenderList = CHashString(_T("AddRenderList")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_AddRenderList, sizeof(ADDRENDERLISTPARAMS), &arlp);	

		m_RC->CreateContext(*this, rect.right, rect.bottom, 24, 32);

		VIEWPORTPARAMS svp;
		svp.height = m_iYRes;
		svp.width = m_iXRes;
		static DWORD msgHash_SetViewPort = CHashString(_T("SetViewPort")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetViewPort, sizeof(VIEWPORTPARAMS), &svp);		

//		FULLSCREENPARAMS fsp;
//		fsp.m_iWidth = m_iXRes;
//		fsp.m_iHeight = m_iYRes;
//		fsp.m_bFull = true;
static DWORD msgHash_SetFullScreen = CHashString(_T("SetFullScreen")).GetUniqueID();
//		m_ToolBox->SendMessage(msgHash_SetFullScreen, sizeof(FULLSCREENPARAMS), &fsp);

		VIEWPORTPARAMS gwrp;
		gwrp.offsetx = 0;
		gwrp.offsety = 0;
		gwrp.width = m_iXRes;
		gwrp.height = m_iYRes;		
		static DWORD msgHash_SetWindowSize = CHashString(_T("SetWindowSize")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetWindowSize, sizeof(VIEWPORTPARAMS), &gwrp, NULL, NULL);

		POSITIONDATA pd;
		pd.m_fXPos = 0.f;
		pd.m_fYPos = 0.f;
		static DWORD msgHash_GUIManagerSetPageOffset = CHashString(_T("GUIManagerSetPageOffset")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GUIManagerSetPageOffset, sizeof(POSITIONDATA), &pd);

		float zf = 1.f;
		static DWORD msgHash_SetZoomFactor = CHashString(_T("SetZoomFactor")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetZoomFactor, sizeof(float), &zf);	
		ShowCursor(false);
	}
	else
	{
		ShowCursor(true);
	}
}

void CGUIFullScreen::OnMouseMove(UINT nFlags, CPoint point)
{
	MOUSECOORDS mouse;
	mouse.m_LButtonDown = m_bLeftButtonDown;
	mouse.m_lXPos = point.x;
	mouse.m_lYPos = point.y;
	mouse.m_lXDelta = point.x - m_iLastMouseX;
	mouse.m_lYDelta = point.y - m_iLastMouseY;
	static DWORD msgHash_MouseMove = CHashString(_T("MouseMove")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_MouseMove, sizeof(MOUSECOORDS), &mouse);

	m_iLastMouseX = point.x;
	m_iLastMouseY = point.y;

	CWnd::OnMouseMove(nFlags, point);
}

void CGUIFullScreen::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLeftButtonDown = true;
	MOUSECOORDS mouse;
	mouse.m_LButtonDown = true;
	mouse.m_lXPos = m_iLastMouseX;
	mouse.m_lYPos = m_iLastMouseY;
	mouse.m_lXDelta = 0;
	mouse.m_lYDelta = 0;
	static DWORD msgHash_LeftMousePress = CHashString(_T("LeftMousePress")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_LeftMousePress, sizeof(MOUSECOORDS), &mouse);

	CWnd::OnLButtonDown(nFlags, point);
}

void CGUIFullScreen::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLeftButtonDown = false;
	MOUSECOORDS mouse;
	mouse.m_LButtonDown = false;
	mouse.m_lXPos = m_iLastMouseX;
	mouse.m_lYPos = m_iLastMouseY;
	mouse.m_lXDelta = 0;
	mouse.m_lYDelta = 0;
	static DWORD msgHash_LeftMouseRelease = CHashString(_T("LeftMouseRelease")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_LeftMouseRelease, sizeof(MOUSECOORDS), &mouse);

	CWnd::OnLButtonUp(nFlags, point);
}

void CGUIFullScreen::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE)
	{
		if (m_Doc)
		{
			ShowWindow(SW_HIDE);
			m_Doc->ReturnToWindow();			
		}
	}
	else
	{
		static DWORD msgHash_KeyPress = CHashString(_T("KeyPress")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_KeyPress, sizeof(char), &nChar);
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGUIFullScreen::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	static DWORD msgHash_KeyRelease = CHashString(_T("KeyRelease")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_KeyRelease, sizeof(char), &nChar);
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CGUIFullScreen::SetColor(UINT r, UINT g, UINT b)
{
	if (m_RC)
	{
		m_RC->SetBGColor(r, g, b);
	}
}