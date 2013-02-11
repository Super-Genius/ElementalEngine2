//-----------------------------------------------------------------------------
//
// File:          CRenderContext.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CRenderContext'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'SetRect'
*     Added method 'GetZDepth'
*     Added method 'GetStencil'
*     Added method 'GetRect'
*     Added method 'GetParentHwnd'
*     Added method 'GetName'
*     Added method 'GetConfig'
*     Added method 'GetChildRenderContext'
*     Added method 'AddChild'
*     Added method '~CRenderContext'
*     Added method 'CRenderContext'
*     Added member 'm_RCConfig'
*     Added member 'm_Children'
*     Added member 'm_Parent'
*     Added member 'm_FullScreen'
*     Added member 'm_Stencil'
*     Added member 'm_Depth'
*     Added member 'm_Bpp'
*     Added member 'm_Height'
*     Added member 'm_Width'
*     Added member 'm_Top'
*     Added member 'm_Left'
*     Added member 'm_Name'
*     Added member 'm_hWnd'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _CRENDERCONTEXT_H
#define _CRENDERCONTEXT_H

//@START_USER1
//@END_USER1



class CRenderContext
{

//@START_USER2
//@END_USER2

//
// Group: ClassBuilder methods
//

private:
    void ConstructorInclude();
    void DestructorInclude();

//
// Non-Grouped Members
//

protected:
    HWND m_hWnd;
    LPCSTR m_Name;
    DWORD m_Left;
    DWORD m_Top;
    DWORD m_Width;
    DWORD m_Height;
    DWORD m_Bpp;
    DWORD m_Depth;
    DWORD m_Stencil;
    BOOL m_FullScreen;
    CRenderContext* m_Parent;
    VECINDEXRC m_Children;
    CConfig m_RCConfig;

//
// Non-Grouped Methods
//

public:
    CRenderContext(HWND hWnd, LPCSTR name, DWORD left, DWORD top, DWORD width,
                   DWORD height, DWORD bpp, DWORD depth, DWORD stencil,
                   BOOL fullscreen, CRenderContext* parent);
    virtual ~CRenderContext();
    void AddChild(CRenderContext* child);
    BOOL GetChildRenderContext(DWORD index, CRenderContext* *pRC);
    CConfig* GetConfig();
    LPCSTR GetName();
    HWND GetParentHwnd();
    void GetRect(RECT* outRect);
    DWORD GetStencil();
    DWORD GetZDepth();
    void SetRect(DWORD left, DWORD top, DWORD width, DWORD height);
};

#endif


#ifdef CB_INLINES
#ifndef _CRENDERCONTEXT_H_INLINES
#define _CRENDERCONTEXT_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
