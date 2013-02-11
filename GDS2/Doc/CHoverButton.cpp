//-----------------------------------------------------------------------------
//
// File:          CHoverButton.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CHoverButton'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'SetToolTipText'
*     Added method 'SetToolTipText'
*     Added method 'PreTranslateMessage'
*     Added method 'OnMouseMove'
*     Added method 'OnMouseLeave'
*     Added method 'OnMouseHover'
*     Added method 'LoadBitmap'
*     Added method 'InitToolTip'
*     Added method 'DrawItem'
*     Added method 'ActivateTooltip'
*     Added method '~CHoverButton'
*     Added method 'CHoverButton'
*     Added inheritance 'CBitmapButton'
*     Added member 'm_ToolTip'
*     Added member 'm_bTracking'
*     Added member 'mybitmap'
*     Added member 'm_ButtonSize'
*     Added member 'm_bHover'
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




/*@NOTE_4204
Constructor method.
*/
CHoverButton::CHoverButton() //@INIT_4204
{//@CODE_4204
    ConstructorInclude();

    // Put in your own code
}//@CODE_4204


/*@NOTE_4198
Destructor method.
*/
CHoverButton::~CHoverButton()
{//@CODE_4198
    DestructorInclude();

    // Put in your own code
}//@CODE_4198


void CHoverButton::ActivateTooltip(BOOL bActivate)
{//@CODE_4211
}//@CODE_4211


void CHoverButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{//@CODE_4221
}//@CODE_4221


void CHoverButton::InitToolTip()
{//@CODE_4228
}//@CODE_4228


BOOL CHoverButton::LoadBitmap(UINT bitmapid)
{//@CODE_4224
    BOOL value;

    return value;
}//@CODE_4224


afx_msg void CHoverButton::OnMouseHover(WPARAM wparam, LPARAM lparam)
{//@CODE_4235
    afx_msg value;

    return value;
}//@CODE_4235


afx_msg LRESULT CHoverButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{//@CODE_4232
    afx_msg value;

    return value;
}//@CODE_4232


afx_msg void CHoverButton::OnMouseMove(UINT nFlags, CPoint point)
{//@CODE_4229
    afx_msg value;

    return value;
}//@CODE_4229


BOOL CHoverButton::PreTranslateMessage(MSG* pMsg)
{//@CODE_4218
    BOOL value;

    return value;
}//@CODE_4218


void CHoverButton::SetToolTipText(CString* spText, BOOL bActivate)
{//@CODE_4205
}//@CODE_4205


void CHoverButton::SetToolTipText(int nId, BOOL bActivate)
{//@CODE_4208
}//@CODE_4208


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4199
Method which must be called first in a constructor.
*/
void CHoverButton::ConstructorInclude()
{
}


/*@NOTE_4200
Method which must be called first in a destructor.
*/
void CHoverButton::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
