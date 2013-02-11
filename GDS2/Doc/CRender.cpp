//-----------------------------------------------------------------------------
//
// File:          CRender.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CRender'
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

//@START_USER1
//@END_USER1


// Master include file
#include "StdAfx.h"


//@START_USER2
//@END_USER2




/*@NOTE_4559
Constructor method.
*/
CRender::CRender(LPCSTR name, CObjectManager* objMan) //@INIT_4559
: CBaseObject(&GUID_RENDER, name)
    
{//@CODE_4559

        m_ObjManager = objMan;
   
}//@CODE_4559


/*@NOTE_4550
Destructor method.
*/
CRender::~CRender()
{//@CODE_4550
    DestructorInclude();

    // Put in your own code
}//@CODE_4550


BOOL CRender::DestroyRenderContext(HWND wnd)
{//@CODE_4572

        CRenderContext *rc;
        if (!GetRenderContext(wnd, &rc))
        {
            return FALSE;
        }

        delete rc;
   
}//@CODE_4572


BOOL CRender::DestroyRenderContext(LPCSTR name)
{//@CODE_4574

        CRenderContext *rc;
        if (!GetRenderContext(name, &rc))
        {
            return FALSE;
        }

        delete rc;
   
}//@CODE_4574


CConfig* CRender::GetConfig()
{//@CODE_4565

        return &m_RenderConfig;
   
}//@CODE_4565


DWORD CRender::GetLastError()
{//@CODE_4562

        return m_LastError;
   
}//@CODE_4562


BOOL CRender::GetRenderContext(HWND wnd, CRenderContext* *outContext)
{//@CODE_4566

        MAPHWNDRC::iterator itMHRC;
        itMHRC = m_RenderContexts.find(wnd);
        if (itMHRC != m_RenderContexts.end())
        {
            *outContext = itMHRC->second;
            return TRUE;
        }

        *outContext = NULL;
        return FALSE;
   
}//@CODE_4566


BOOL CRender::GetRenderContext(LPCSTR name, CRenderContext* *outContext)
{//@CODE_4569

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
   
}//@CODE_4569


void CRender::SetLastError(DWORD error)
{//@CODE_4563

        m_LastError = error;
   
}//@CODE_4563


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4551
Method which must be called first in a constructor.
*/
void CRender::ConstructorInclude()
{
}


/*@NOTE_4552
Method which must be called first in a destructor.
*/
void CRender::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
