//-----------------------------------------------------------------------------
//
// File:          CDragListBoxEx.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CDragListBoxEx'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'OnLButtonDblClk'
*     Added method 'OnChildNotify'
*     Added method '~CDragListBoxEx'
*     Added method 'CDragListBoxEx'
*     Added inheritance 'CDragListBox'
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




/*@NOTE_4160
Constructor method.
*/
CDragListBoxEx::CDragListBoxEx() //@INIT_4160
{//@CODE_4160
    ConstructorInclude();

    // Put in your own code
}//@CODE_4160


/*@NOTE_4154
Destructor method.
*/
CDragListBoxEx::~CDragListBoxEx()
{//@CODE_4154
    DestructorInclude();

    // Put in your own code
}//@CODE_4154


BOOL CDragListBoxEx::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam,
                                   LRESULT* pLResult)
{//@CODE_4161
    BOOL value;

    return value;
}//@CODE_4161


afx_msg void CDragListBoxEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{//@CODE_4166
    afx_msg value;

    return value;
}//@CODE_4166


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4155
Method which must be called first in a constructor.
*/
void CDragListBoxEx::ConstructorInclude()
{
}


/*@NOTE_4156
Method which must be called first in a destructor.
*/
void CDragListBoxEx::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
