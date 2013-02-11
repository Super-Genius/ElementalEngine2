//=====================================================================
// File:		DX8RenderCore.cpp
// Description: Core routines for DX8 Rendering
// Started:		10/31/2001
// Original:	Kenneth Hurley
//=====================================================================

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------
// Function:    CDX8RenderCore
// Description: Constructor
// Parameters:  objMan = pointer to external object manager
// Returns:     .
//---------------------------------------------------------------------
CDX8RenderCore::CDX8RenderCore(CObjectManager *objMan) : 
    CRender(_T("DX8 Renderer"), objMan)
{
    m_pD3D = NULL;
    m_RenderContexts.clear();
}

//---------------------------------------------------------------------
// Function:    CDX8RenderCore
// Description: Destructor
// Parameters:  .
// Returns:     .
//---------------------------------------------------------------------
CDX8RenderCore::~CDX8RenderCore()
{
    if (!ShutDown())
    {
        if (gLogger != NULL)
        {
            gLogger->Print(_T("~CDX8RenderCore: Shutdown failed"));
        }
    }
}

//---------------------------------------------------------------------
// Function:    CDX8RenderCore
// Description: Destructor
// Parameters:  .
// Returns:     .
//---------------------------------------------------------------------
BOOL CDX8RenderCore::ShutDown()
{
    MAPHWNDRC::iterator itRC;
    
    for (itRC = m_RenderContexts.begin(); itRC != m_RenderContexts.end(); itRC++)
    {
        delete itRC->second;
    }

    m_RenderContexts.clear();

    if (m_pD3D != NULL)
    {
        m_pD3D->Release();
        m_pD3D = NULL;
    }

    return TRUE;
}

//---------------------------------------------------------------------
// Function:    CDX8RenderCore
// Description: Constructor
// Parameters:  objMan = pointer to external object manager
// Returns:     .
//---------------------------------------------------------------------
BOOL CDX8RenderCore::Initialize()
{
    // Create new Direct3D object
    m_pD3D = Direct3DCreate8( D3D_SDK_VERSION );
    if (m_pD3D == NULL)
    {
        m_LastError = D3DERR_NOTAVAILABLE;
        gLogger->Print(_T("Unable to Create DirectX 8 3D object"));
        return FALSE;
    }
    
    return TRUE;
}

//---------------------------------------------------------------------
// Function:    CreateRenderContext
// Description: Function to create a context in which to render to
// Parameters:  hWnd = windows handle for rendering context
//              name = name of the rendering context
//              left = left position inside window
//              top = top       "        "      "
//              width = width   "        "      "
//              height = height "        "      "
//              bpp = bits per pixel of rendering context
//              depth = Z-Buffer depth
//              stencil = Z-Stencil bits
//              fullscreen = whether window is full screen or not
//              parent = parents rendering context if available
// Returns:     TRUE, if successful
//---------------------------------------------------------------------
BOOL CDX8RenderCore::CreateRenderContext(HWND hWnd, LPCSTR name, DWORD left,
                              DWORD top, DWORD width, DWORD height, DWORD bpp, DWORD depth,
                              DWORD stencil, BOOL fullscreen, CRenderContext *parent)
{
    CDX8RenderContext *newCtx;

    // if parent is null then we have to have an hWnd
    if (parent == NULL)
    {
        // we have to have a valid window
        if (!IsWindow(hWnd))
        {
            m_LastError = ERROR_INVALID_WINDOW_HANDLE;
            gLogger->Print(_T("CreateRenderContext: Invalid Window handle specified"));
            return FALSE;
        }
    }
    else
    {
        // if we don't have a valid window, default to parents.
        if (!IsWindow(hWnd))
        {
            hWnd = parent->GetParentHwnd();
        }
    }

    // create a new DX8 Render context.
    newCtx = new CDX8RenderContext(m_pD3D, hWnd, name, left, top, width, height, bpp,
                                    depth, stencil, fullscreen, parent);

    if (newCtx == NULL)
    {
        m_LastError = ERROR_NOT_ENOUGH_MEMORY;
        gLogger->Print(_T("CreateRenderContext: Out of memory allocating CDX8RenderContext"));
        return FALSE;
    }

    if (newCtx->GetLastError() != D3D_OK)
    {
        gLogger->Print(_T("CreateRenderContext: Failed, see log file"));
        return FALSE;
    }

    // top level parents get saved here
    if (parent == NULL)
    {
        // what the hell?
        DWORD nContexts = m_RenderContexts.size();

        m_RenderContexts.insert(MAPHWNDRC::value_type(hWnd, newCtx));
        // and save into context list
//        m_RenderContexts[hWnd] = newCtx;
        if (nContexts != 0)
        {
            return FALSE;
        }
    }
    else
    {
        parent->AddChild(newCtx);
    }

    return TRUE;
}

//---------------------------------------------------------------------
// Function:    BeginFrame
// Description: Beginning of Frame before any rendering takes place
// Parameters:  HWND = window of rendering context
//              contextIndex = index of rendering context
// Returns:     TRUE, if successful.
//---------------------------------------------------------------------
BOOL CDX8RenderCore::BeginFrame(HWND hWnd, DWORD contextIndex)
{
    CDX8RenderContext *rc;
    MAPHWNDRC::iterator itMRC;
    IDirect3DDevice8 *pD3DDevice;
    DWORD bkColor;
    DWORD bColorClearEnable;
    float zClearValue;
    DWORD bZClearEnable;
    DWORD stencilValue;
    DWORD bStencilClearEnable;
    DWORD flags;
    CDX8RenderContext *backContext;
    LPDIRECT3DSURFACE8 pBack, pStencil;

    itMRC = m_RenderContexts.find(hWnd);
    if (itMRC == m_RenderContexts.end())
    {
        gLogger->Print(_T("Unable to find rendering context for hWnd %x"),
                        hWnd);
        return FALSE;
    }

    // grab the parent rendering context
    rc = (CDX8RenderContext *)(itMRC->second);

	backContext = rc;
	// if child context, grab child context
	if ((contextIndex > 0) && (!rc->GetChildRenderContext(contextIndex-1, 
			(CRenderContext **)&backContext)))
	{
        gLogger->Print(_T("Unable to find child rendering context %d for hWnd %x"),
                        contextIndex-1, hWnd);
        return FALSE;
	}
	
    // initialized default values
    bColorClearEnable = TRUE;
    bkColor = 0x000000;
    bZClearEnable = TRUE;
    zClearValue = 1.0f;
    bStencilClearEnable = TRUE;
    stencilValue = 0x00000000;

    // get the configurations, if applicable
    GetConfig()->GetValue("Color Clear Enable", &bColorClearEnable);
    backContext->GetConfig()->GetValue("Background Color", &bkColor);
    backContext->GetConfig()->GetValue("Z Clear Enable", &bZClearEnable);
    backContext->GetConfig()->GetValue("Z Clear Value", &zClearValue);
    backContext->GetConfig()->GetValue("Stencil Clear Enable", &bStencilClearEnable);
    backContext->GetConfig()->GetValue("Stencil Value", &stencilValue);

    // start off with no flags relevant
    flags = 0;
    // is color clear enabled?
    if (bColorClearEnable)
    {
        flags |= D3DCLEAR_TARGET;
    }

    // now about about clearing the z buffer?
    if (bZClearEnable && (rc->GetZDepth() != 0))
    {
        flags |= D3DCLEAR_ZBUFFER;
    }

    // finally what about the stencil?
    if (bStencilClearEnable && (rc->GetStencil() != 0))
    {
        flags |= D3DCLEAR_STENCIL;
    }


	if (!rc->GetD3DDevice(&pD3DDevice))
	{
        gLogger->Print(_T("Unable to get D3D Device from rendering context"));
        return FALSE;
	}

	// get the buffer for the backbuffer
    if (!backContext->GetBackBuffer(&pBack))
    {
        gLogger->Print(_T("Unable to get back buffer from rendering context"));
        return FALSE;
    }

    pStencil = NULL;
    if (bStencilClearEnable || bZClearEnable)
    {
        CHECKHRESULT(pD3DDevice->GetDepthStencilSurface(&pStencil), FALSE);
    }

    CHECKHRESULT(pD3DDevice->SetRenderTarget(pBack, pStencil), FALSE);
    pBack->Release();
    pStencil->Release();

    CHECKHRESULT(pD3DDevice->BeginScene(), FALSE);
    if (flags != 0)
    {
        CHECKHRESULT(pD3DDevice->Clear(0, NULL, flags, bkColor, zClearValue, stencilValue), FALSE);
    }

    return TRUE;
}

//---------------------------------------------------------------------
// Function:    EndFrame
// Description: End of Frame after rendering takes place
// Parameters:  HWND = window of rendering context
//              contextIndex = index of rendering context
// Returns:     TRUE, if successful.
//---------------------------------------------------------------------
BOOL CDX8RenderCore::EndFrame(HWND hWnd, DWORD contextIndex)
{
    CDX8RenderContext *rc;
    MAPHWNDRC::iterator itMRC;
    IDirect3DDevice8 *pD3DDevice;

    itMRC = m_RenderContexts.find(hWnd);
    if (itMRC == m_RenderContexts.end())
    {
        gLogger->Print(_T("Unable to find rendering context for hWnd %x"),
                        hWnd);
        return FALSE;
    }

    // grab the parent rendering context
    rc = (CDX8RenderContext *)(itMRC->second);
    if (!rc->GetD3DDevice(&pD3DDevice))
    {
        gLogger->Print(_T("Unable to get Direct3D Device from rendering context"));
        return FALSE;
    }

    CHECKHRESULT(pD3DDevice->EndScene(), FALSE);

    return TRUE;
}

//---------------------------------------------------------------------
// Function:    ShowFrame
// Description: Beginning of Frame before any rendering takes place
// Parameters:  HWND = window of rendering context
//              contextIndex = index of rendering context
// Returns:     TRUE, if successful.
//---------------------------------------------------------------------
BOOL CDX8RenderCore::ShowFrame(HWND hWnd, DWORD contextIndex)
{
    CDX8RenderContext *rc;
    MAPHWNDRC::iterator itMRC;
    RECT rect;
    CDX8RenderContext *backContext;

    itMRC = m_RenderContexts.find(hWnd);
    if (itMRC == m_RenderContexts.end())
    {
        gLogger->Print(_T("Unable to find rendering context for hWnd %x"),
                        hWnd);
        return FALSE;
    }

    // grab the parent rendering context
    rc = (CDX8RenderContext *)(itMRC->second);
	backContext = rc;

    // if not parent get the child index, else return same
    if ((contextIndex > 0) && !rc->GetChildRenderContext(contextIndex-1, (CRenderContext **)&backContext))
    {
        gLogger->Print(_T("Unable to get rendering context for index %d"), contextIndex-1);
        return FALSE;
    }
        
    // grab where we want destination to go.
    backContext->GetRect(&rect);

    // %%%%% here we should resize the destination, if user resized the window

    // see if they are trying to override the output window
    if (backContext->GetParentHwnd() != hWnd)
    {
        CHECKHRESULT(backContext->Present( NULL, &rect, hWnd, NULL), FALSE);
    }
    else
    {
        CHECKHRESULT(backContext->Present( NULL, &rect, NULL, NULL), FALSE);
    }

    return TRUE;
}

