//=====================================================================
// File:		DX8RenderCore.h
// Description: Header file for the DX8 Render Core
// Started:		09/21/2002
// Original:	Kenneth Hurley
//=====================================================================

#ifndef DX8RENDERCORE_H
#define DX8RENDERCORE_H

class CDX8RenderCore : public CRender
{
private:
    IDirect3D8 *m_pD3D;
    
public:
    CDX8RenderCore(CObjectManager *objMan);
    ~CDX8RenderCore();

    virtual BOOL Initialize();
    virtual BOOL CreateRenderContext(HWND hWnd, LPCSTR name, DWORD left,
                              DWORD top, DWORD width, DWORD height, DWORD bpp, DWORD depth,
                              DWORD stencil, BOOL fullscreen, CRenderContext *parent);
    virtual BOOL ShutDown();
    virtual BOOL BeginFrame(HWND hWnd, DWORD contextIndex);
    virtual BOOL EndFrame(HWND hWnd, DWORD contextIndex);
    virtual BOOL ShowFrame(HWND hWnd, DWORD contextIndex);
    virtual BOOL Render(CObject *object) { return TRUE; };
    virtual LPCSTR GetErrorDescription(DWORD errorNum) { return NULL; };

};

#endif      // #ifndef DX8RENDERCORE_H
