//-----------------------------------------------------------------------------
//
// File:          CRenderContext.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CRenderContext'
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

//@START_USER1
//@END_USER1


// Master include file
#include "StdAfx.h"


//@START_USER2
//@END_USER2




/*@NOTE_4595
Constructor method.
*/
CRenderContext::CRenderContext(HWND hWnd, LPCSTR name, DWORD left, DWORD top,
                               DWORD width, DWORD height, DWORD bpp,
                               DWORD depth, DWORD stencil, BOOL fullscreen,
                               CRenderContext* parent) //@INIT_4595
{//@CODE_4595

        // just copy everything
        m_hWnd = hWnd;
        m_Name = name;
        m_Bpp = bpp;
        m_Depth = depth;
        m_Stencil = stencil;
        m_FullScreen = fullscreen;
        m_Parent = parent;
        SetRect(left, top, width, height);
   
}//@CODE_4595


/*@NOTE_4577
Destructor method.
*/
CRenderContext::~CRenderContext()
{//@CODE_4577

        VECINDEXRC::iterator itRC;

        for (itRC = m_Children.begin(); itRC != m_Children.end(); itRC++)
        {
            delete (*itRC);
        }

        m_Children.empty();
   
}//@CODE_4577


void CRenderContext::AddChild(CRenderContext* child)
{//@CODE_4610

		m_Children.push_back(child);
   
}//@CODE_4610


BOOL CRenderContext::GetChildRenderContext(DWORD index, CRenderContext* *pRC)
{//@CODE_4607

        if (index < m_Children.size())
        {
            *pRC = m_Children[index];
            return TRUE;
        }

        return FALSE;
   
}//@CODE_4607


CConfig* CRenderContext::GetConfig()
{//@CODE_4612

		return &m_RCConfig;
}//@CODE_4612


LPCSTR CRenderContext::GetName()
{//@CODE_4624

        return m_Name;
   
}//@CODE_4624


HWND CRenderContext::GetParentHwnd()
{//@CODE_4621

        return m_hWnd;
   
}//@CODE_4621


void CRenderContext::GetRect(RECT* outRect)
{//@CODE_4618

        outRect->left = m_Left;
        outRect->top = m_Top;
        outRect->right = m_Left + m_Width;
        outRect->bottom = m_Top + m_Height;
   
}//@CODE_4618


DWORD CRenderContext::GetStencil()
{//@CODE_4622

        return m_Stencil;
   
}//@CODE_4622


DWORD CRenderContext::GetZDepth()
{//@CODE_4623

        return m_Depth;
   
}//@CODE_4623


void CRenderContext::SetRect(DWORD left, DWORD top, DWORD width, DWORD height)
{//@CODE_4613

        m_Left = left;
        m_Top = top;
        m_Width = width;
        m_Height = height;
   
}//@CODE_4613


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4578
Method which must be called first in a constructor.
*/
void CRenderContext::ConstructorInclude()
{
}


/*@NOTE_4579
Method which must be called first in a destructor.
*/
void CRenderContext::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
