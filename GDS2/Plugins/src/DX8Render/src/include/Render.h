//---------------------------------------------------------------------
// File:		Render.h
// Description:	Base Render class
// Created:		6/27/02
// Author:		Kenneth L. Hurley
//---------------------------------------------------------------------

#ifndef RENDER_H
#define RENDER_H

#ifndef BASEOBJECT_H
#include "BaseObject.h"
#endif

#ifndef CONFIG_H
#include "Config.h"
#endif

#ifndef MESH_H
#include "Mesh.h"
#endif

class CRenderContext;

#pragma warning(disable : 4786)
#include <map>

using namespace std;

typedef map<HWND, CRenderContext *> MAPHWNDRC;
typedef vector<CRenderContext *> VECINDEXRC;

#include "renderContext.h"

// {074AC60D-537C-4490-973C-AD56C3A870CD}
static const GUID GUID_RENDER = 
{ 0x74ac60d, 0x537c, 0x4490, { 0x97, 0x3c, 0xad, 0x56, 0xc3, 0xa8, 0x70, 0xcd } };

class CRender : public CBaseObject
{
	// attributes
protected:
    CObjectManager *m_ObjManager;   // pointer to object manager
    DWORD m_LastError;
    CConfig m_RenderConfig;        // configuration class
    MAPHWNDRC m_RenderContexts;

	// operations
public:
    CRender(LPCSTR name, CObjectManager *objMan) : CBaseObject(&GUID_RENDER, name)
    {
        m_ObjManager = objMan;
    };

    virtual BOOL CreateRenderContext(HWND wnd, LPCSTR name, DWORD left, DWORD top, 
        DWORD width, DWORD height, DWORD bpp, DWORD depth, DWORD stencil,
        BOOL fullscreen, CRenderContext *parent) = 0;
    virtual BOOL Initialize() = 0;
    virtual BOOL ShutDown() = 0;
    virtual BOOL BeginFrame(HWND hWnd, DWORD contextIndex) = 0;
    virtual BOOL EndFrame(HWND hWnd, DWORD contextIndex) = 0;
    virtual BOOL ShowFrame(HWND hWnd, DWORD contextIndex) = 0;
    virtual BOOL Render(CObject *object) = 0;
    virtual LPCSTR GetErrorDescription(DWORD errorNum) = 0;

    DWORD GetLastError()
    {
        return m_LastError;
    }

    void SetLastError(DWORD error)
    {
        m_LastError = error;
    }

    CConfig *GetConfig()
    {
        return &m_RenderConfig;
    }

    BOOL GetRenderContext(HWND wnd, CRenderContext **outContext)
    {
        MAPHWNDRC::iterator itMHRC;
        itMHRC = m_RenderContexts.find(wnd);
        if (itMHRC != m_RenderContexts.end())
        {
            *outContext = itMHRC->second;
            return TRUE;
        }

        *outContext = NULL;
        return FALSE;
    }

    BOOL GetRenderContext(LPCSTR name, CRenderContext **outContext)
    {
        MAPHWNDRC::iterator itMHRC;
        CRenderContext *rc;

        *outContext = NULL;
        for (itMHRC = m_RenderContexts.begin(); itMHRC != m_RenderContexts.end(); itMHRC++)
        {
            rc = itMHRC->second;
            if (!_tcscmp(rc->GetName(), name))
            {
                *outContext = rc;
                return TRUE;
            }
        }

        return FALSE;
    }

    BOOL DestroyRenderContext(HWND wnd)
    {
        CRenderContext *rc;
        if (!GetRenderContext(wnd, &rc))
        {
            return FALSE;
        }

        delete rc;
    };

    BOOL DestroyRenderContext(LPCSTR name)
    {
        CRenderContext *rc;
        if (!GetRenderContext(name, &rc))
        {
            return FALSE;
        }

        delete rc;
    }
};


#endif	// #ifndef RENDER_H
