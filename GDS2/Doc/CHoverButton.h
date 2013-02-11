//-----------------------------------------------------------------------------
//
// File:          CHoverButton.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CHoverButton'
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

#ifndef _CHOVERBUTTON_H
#define _CHOVERBUTTON_H

//@START_USER1
//@END_USER1


/*@NOTE_4197
C:\Development\Game Dev System\Plugins\src\include\HoverButton.h(15)

 HoverButton.h : header file
C:\Development\Game Dev System\Plugins\src\include\HoverButton.h(15)


C:\Development\Game Dev System\Plugins\src\include\HoverButton.h(15)


C:\Development\Game Dev System\Plugins\src\include\HoverButton.h(15)

 CHoverButton by Niek Albers
C:\Development\Game Dev System\Plugins\src\include\HoverButton.h(15)

 Thanks to some people for the tooltip.
C:\Development\Game Dev System\Plugins\src\include\HoverButton.h(15)

 A cool CBitmapButton derived class with 3 states,
C:\Development\Game Dev System\Plugins\src\include\HoverButton.h(15)

 Up/Down/Hover.
*/

class CHoverButton
    : public CBitmapButton
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
    BOOL m_bHover;
    CSize m_ButtonSize;
    CBitmap mybitmap;
    BOOL m_bTracking;
    CToolTipCtrl m_ToolTip;

//
// Non-Grouped Methods
//

protected:
    void ActivateTooltip(BOOL bActivate);
    void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    void InitToolTip();
    afx_msg void OnMouseHover(WPARAM wparam, LPARAM lparam);
    afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    BOOL PreTranslateMessage(MSG* pMsg);

public:
    CHoverButton();
    virtual ~CHoverButton();
    BOOL LoadBitmap(UINT bitmapid);
    void SetToolTipText(CString* spText, BOOL bActivate);
    void SetToolTipText(int nId, BOOL bActivate);
};

#endif


#ifdef CB_INLINES
#ifndef _CHOVERBUTTON_H_INLINES
#define _CHOVERBUTTON_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
