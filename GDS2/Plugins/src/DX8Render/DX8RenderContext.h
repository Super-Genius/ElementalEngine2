//=====================================================================
// File:		DX8RenderContext.h
// Description: Header file for the Rendering contexts
//                  i.e. rendering pages inside windows
// Started:		09/29/2002
// Original:	Kenneth Hurley
//=====================================================================

#include "RenderContext.h"

class CDX8RenderContext : public CRenderContext
{
protected:
    IDirect3DDevice8 *m_pD3DDev;        // only if parent == NULL, is this valid
    IDirect3DSwapChain8 *m_pSwapChain;   // swap chain if parent != NULL;
    DWORD m_LastError;                  // error if anything.

public:
    CDX8RenderContext(IDirect3D8 *pD3D, HWND hWnd, LPCSTR name, DWORD left, DWORD top, 
                DWORD width, DWORD height, DWORD bpp, DWORD depth, DWORD stencil,
                BOOL fullscreen, CRenderContext *parent);

	~CDX8RenderContext();

    BOOL GetModeFormat(DWORD bpp, D3DFORMAT *fmt);
    BOOL GetDepthStencilFormat(DWORD depth, DWORD stencil, D3DFORMAT *fmt);
    BOOL GetD3DDevice(IDirect3DDevice8 **ppDevice);
	BOOL GetBackBuffer(IDirect3DSurface8** ppBack);
	BOOL Present(CONST RECT* pSourceRect, CONST RECT* pDestRect,
			HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
 
    DWORD GetLastError()
    {
        return m_LastError;
    }

};

