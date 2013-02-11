//=====================================================================
// File:		DX8RenderContext.cpp
// Description: Implementation file for the DX8 Rendering contexts
//                  i.e. rendering pages inside windows
// Started:		09/29/2002
// Original:	Kenneth Hurley
//=====================================================================

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------
// Function:    CDX8RenderContext
// Description: Constructor for class
// Parameters:  pD3D = pointer to d3d created object (IDirect3D8 *)
//              hWnd = windows handle to receive rendering output
//              name = pointer to name for rendering context
//              left = left position inside window
//              top = top position inside of window
//              width = initial width of rendering buffer
//              height = initial height of rendering buffer
//              bpp = bits per pixel depth in which to render,
//                  ignored if !fullscreen
//              depth = z-Buffer depth
//              stencil = number of stencil bits requested
//              fullscreen = TRUE if fullscreen, FALSE = windowed
//              parent = pointer to parent context or NULL for top level
// Return:      .
//---------------------------------------------------------------------
CDX8RenderContext::CDX8RenderContext(IDirect3D8 *pD3D, HWND hWnd, LPCSTR name, 
            DWORD left, DWORD top, DWORD width, DWORD height, DWORD bpp, DWORD depth, 
            DWORD stencil, BOOL fullscreen, CRenderContext *parent) : 
                CRenderContext(hWnd, name, left, top, width, height, bpp, depth, 
                        stencil, fullscreen, parent)
{
    D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE mode;

    m_LastError = D3D_OK;
	m_pD3DDev = NULL;
	m_pSwapChain = NULL;

    if (!IsWindow(hWnd))
    {
        hWnd = ((CDX8RenderContext *)(parent))->m_hWnd;
    }

    ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.Windowed = !fullscreen;

    // Use the current display mode, for some parameters
    m_LastError = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT , &mode);
    if (FAILED(m_LastError))
    {
        gLogger->Print((LPTSTR)DXGetErrorDescription8(m_LastError));
        return;
    }

    // check for full screen or not
    if (fullscreen)
    {
        // get format based on request
        GetModeFormat(bpp, &d3dpp.BackBufferFormat);
        d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
    }
    else
    {
        // this had to be the screenformat
        d3dpp.BackBufferFormat = mode.Format;
        d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
    }

    d3dpp.BackBufferWidth = width;
    d3dpp.BackBufferHeight = height;
    // is any depth or stencil requested?
    if ((depth == 0) && (stencil == 0))
    {
        d3dpp.EnableAutoDepthStencil = FALSE;
    }
    else
    {
        // enable it and let D3D manage it
        d3dpp.EnableAutoDepthStencil = TRUE;
        if (!GetDepthStencilFormat(depth, stencil, &d3dpp.AutoDepthStencilFormat))
        {
            return;
        }
    }

    // create the d3d device if new one is needed
    if (parent == NULL)
    {
        m_LastError = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
                                  D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                  &d3dpp, &m_pD3DDev);
        if (FAILED(m_LastError))
        {
            gLogger->Print((LPTSTR)DXGetErrorDescription8(m_LastError));
            return;
        }   
    }
    else    // we just need to be part of our parent.
    {
        // m_pSwapChain is IDirect3DSwapChain * 
        m_LastError = ((CDX8RenderContext *)(parent))->m_pD3DDev->CreateAdditionalSwapChain(
                        &d3dpp, &m_pSwapChain);

        if (FAILED(m_LastError))
        {
            gLogger->Print((LPTSTR)DXGetErrorDescription8(m_LastError));
            return;
        }   
    }

}

//---------------------------------------------------------------------
// Function:    ~CDX8RenderContext
// Description: Standard Destructor 
// Parameters:  .
// Return:      .
//---------------------------------------------------------------------
CDX8RenderContext::~CDX8RenderContext()
{
	if (m_Parent == NULL)
	{
		if (m_pD3DDev != NULL)
		{
			m_pD3DDev->Release();
			m_pD3DDev = NULL;
		}
	}
	else
	{
		if (m_pSwapChain != NULL)
		{
			m_pSwapChain->Release();
			m_pSwapChain = NULL;
		}
	}

}

//---------------------------------------------------------------------
// Function:    GetModeFormat
// Description: Get a DX8 specific format for bits per pixel
// Parameters:  bpp = bits per pixel requested
//              fmt = output of D3DFMT_XXXX
// Return:      BOOL = true if success
//---------------------------------------------------------------------
BOOL CDX8RenderContext::GetModeFormat(DWORD bpp, D3DFORMAT *fmt)
{
    m_LastError = D3D_OK;

    // based on stencil #bits
    switch (bpp)
    {
        case 15:
            *fmt = D3DFMT_A1R5G5B5;
            break;
        case 16:
            // this should be more robust.
            *fmt = D3DFMT_X1R5G5B5;
            break;
        case 24:
            *fmt = D3DFMT_X8R8G8B8;
            break;
        case 32:
            *fmt = D3DFMT_A8R8G8B8;
            break;
        default:
            gLogger->Print(_T("CDX8RenderContext::GetModeFormat, unsupported format specified"));
            m_LastError = D3DERR_WRONGTEXTUREFORMAT;
            return FALSE;
    }

    return TRUE;
}

//---------------------------------------------------------------------
// Function:    GetDepthStencilFormat
// Description: Get a DX8 specific format for depth and stencil 
//                        specified
// Parameters:  depth = z-Buffer depth
//              stencil = #bits of stencil
//              fmt = output of D3DFMT_XXXX
// Return:      BOOL = true if success
//---------------------------------------------------------------------
BOOL CDX8RenderContext::GetDepthStencilFormat(DWORD depth, DWORD stencil, D3DFORMAT *fmt)
{
    m_LastError = D3D_OK;

    // based on stencil #bits
    switch (stencil)
    {
        case 0:
            switch (depth)
            {
                case 16:
                    *fmt = D3DFMT_D16;
                    break;
                case 24:
                    *fmt = D3DFMT_D24X8;
                    break;
                case 32:
                    *fmt = D3DFMT_D32;
                    break;
                default:
                    gLogger->Print(_T("CDX8RenderContext::GetDepthStencilFormat, unsupported depth/"
                            "stencil combination format specified"));
                    m_LastError = D3DERR_WRONGTEXTUREFORMAT;
                    return FALSE;
            }
            break;
        case 1:
            switch (depth)
            {
                case 15:
                    *fmt = D3DFMT_D15S1;
                    break;
                default:
                    gLogger->Print(_T("CDX8RenderContext::GetDepthStencilFormat, unsupported depth/"
                            "stencil combination format specified"));
                    m_LastError = D3DERR_WRONGTEXTUREFORMAT;
                    return FALSE;
            }
            break;
        case 8:
            switch (depth)
            {
                case 24:
                    *fmt = D3DFMT_D24S8;
                    break;
                default:
                    gLogger->Print(_T("CDX8RenderContext::GetDepthStencilFormat, unsupported depth/"
                            "stencil combination format specified"));
                    m_LastError = D3DERR_WRONGTEXTUREFORMAT;
                    return FALSE;
            }
    }

    // we should probably validate these formats against device
    return TRUE;
}

//---------------------------------------------------------------------
// Function:    GetD3DDevice
// Description: Get a DX8 specific deviceformat for depth and stencil 
// Parameters:	ppDevice = pointer to receive DX8 Device pointer
// Returns:		TRUE if success
//---------------------------------------------------------------------
BOOL CDX8RenderContext::GetD3DDevice(IDirect3DDevice8 **ppDevice)
{
    if (m_Parent != NULL)
    {
        return ((CDX8RenderContext *)m_Parent)->GetD3DDevice(ppDevice);
    }
    else
    {
        *ppDevice = m_pD3DDev;
        return TRUE;
    }
}
//---------------------------------------------------------------------
// Function:    Present
// Description: Presents the DX8 Back buffer
// Parameters:	.
// Returns:		TRUE if success
//---------------------------------------------------------------------
BOOL CDX8RenderContext::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect,
			HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	IDirect3DDevice8 *pD3DDev;
	// if this context is a child, grab swapchain back buffer
	if (m_Parent != NULL)
	{
		CHECKHRESULT(m_pSwapChain->Present(pSourceRect, pDestRect,
				hDestWindowOverride, pDirtyRegion), FALSE);
	}
	else	// is parent so grab back buffer
	{
		CHECKHRESULT(GetD3DDevice(&pD3DDev), FALSE);
		CHECKHRESULT(pD3DDev->Present(pSourceRect, pDestRect,
				hDestWindowOverride, pDirtyRegion), FALSE);
	}

	return TRUE;
}

//---------------------------------------------------------------------
// Function:    GetBackBuffer
// Description: Get a DX8 specific back buffer for a rendering context
// Parameters:	ppBack = pointer to receive DX8 back buffer pointer
// Returns:		TRUE if success
//---------------------------------------------------------------------
BOOL CDX8RenderContext::GetBackBuffer(IDirect3DSurface8** ppBack)
{
	IDirect3DDevice8 *pD3DDev;
	// if this context is a child, grab swapchain back buffer
	if (m_Parent != NULL)
	{
		CHECKHRESULT(m_pSwapChain->GetBackBuffer(0, 
				D3DBACKBUFFER_TYPE_MONO, ppBack), FALSE);
	}
	else	// is parent so grab back buffer
	{
		CHECKHRESULT(GetD3DDevice(&pD3DDev), FALSE);
		CHECKHRESULT(pD3DDev->GetBackBuffer(0, 
				D3DBACKBUFFER_TYPE_MONO, ppBack), FALSE);
	}

	return TRUE;
}