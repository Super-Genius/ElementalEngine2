//-----------------------------------------------------------------------------
//
// File:          CDragListBoxEx.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CDragListBoxEx'
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

#ifndef _CDRAGLISTBOXEX_H
#define _CDRAGLISTBOXEX_H

//@START_USER1
//@END_USER1


/*@NOTE_4153
C:\Development\Game Dev System\Plugins\src\include\DragListBoxEx.h(13)

 DragListBoxEx.h : header file
C:\Development\Game Dev System\Plugins\src\include\DragListBoxEx.h(13)


C:\Development\Game Dev System\Plugins\src\include\DragListBoxEx.h(13)


C:\Development\Game Dev System\Plugins\src\include\DragListBoxEx.h(13)

 CDragListBoxEx window
*/

class CDragListBoxEx
    : public CDragListBox
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

//
// Non-Grouped Methods
//

protected:
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

public:
    CDragListBoxEx();
    virtual ~CDragListBoxEx();
    BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam,
                       LRESULT* pLResult);
};

#endif


#ifdef CB_INLINES
#ifndef _CDRAGLISTBOXEX_H_INLINES
#define _CDRAGLISTBOXEX_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
