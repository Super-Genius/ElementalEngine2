//-----------------------------------------------------------------------------
//
// File:          CRender.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CRender'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'SetLastError'
*     Added method 'GetRenderContext'
*     Added method 'GetRenderContext'
*     Added method 'GetLastError'
*     Added method 'GetConfig'
*     Added method 'DestroyRenderContext'
*     Added method 'DestroyRenderContext'
*     Added method '~CRender'
*     Added method 'CRender'
*     Added inheritance 'CBaseObject'
*     Added member 'm_RenderContexts'
*     Added member 'm_RenderConfig'
*     Added member 'm_LastError'
*     Added member 'm_ObjManager'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _CRENDER_H
#define _CRENDER_H

//@START_USER1
//@END_USER1


/*@NOTE_4549
C:\Development\Game Dev System\Plugins\src\include\Render.h(39)

 {074AC60D-537C-4490-973C-AD56C3A870CD}
*/

class CRender
    : public CBaseObject
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
    CObjectManager* m_ObjManager;
    DWORD m_LastError;
    CConfig m_RenderConfig;
    MAPHWNDRC m_RenderContexts;

//
// Non-Grouped Methods
//

public:
    CRender(LPCSTR name, CObjectManager* objMan);
    virtual ~CRender();
    BOOL DestroyRenderContext(HWND wnd);
    BOOL DestroyRenderContext(LPCSTR name);
    CConfig* GetConfig();
    DWORD GetLastError();
    BOOL GetRenderContext(HWND wnd, CRenderContext* *outContext);
    BOOL GetRenderContext(LPCSTR name, CRenderContext* *outContext);
    void SetLastError(DWORD error);
};

#endif


#ifdef CB_INLINES
#ifndef _CRENDER_H_INLINES
#define _CRENDER_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
