//-----------------------------------------------------------------------------
//
// File:          CObjectBar.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CObjectBar'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'OnSize'
*     Added method 'OnRemoveObject'
*     Added method 'OnCreate'
*     Added method 'OnClearObjects'
*     Added method 'OnAddObject'
*     Added method 'InitObjectPages'
*     Added method '~CObjectBar'
*     Added method 'CObjectBar'
*     Added inheritance 'CSizingControlBarG'
*     Added member 'm_Icons'
*     Added member 'm_PropSheet'
*     Added member 'obOptions'
*     Added member 'm_ObjectPages'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _COBJECTBAR_H
#define _COBJECTBAR_H

//@START_USER1
//@END_USER1


/*@NOTE_3745
C:\Development\Game Dev System\ObjectBar.h(14)

 ObjectBar.h : header file
C:\Development\Game Dev System\ObjectBar.h(14)


C:\Development\Game Dev System\ObjectBar.h(14)


C:\Development\Game Dev System\ObjectBar.h(14)

 CObjectBar window
*/

class CObjectBar
    : public CSizingControlBarG
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

private:
    CNotifierPropSheet m_PropSheet;
    CImageList m_Icons;

public:
    * m_ObjectPages;
    CObjectOptions* obOptions;

//
// Non-Grouped Methods
//

private:
    void InitObjectPages();

protected:
    afx_msg LRESULT OnAddObject(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnClearObjects(WPARAM wParam, LPARAM lParam);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT OnRemoveObject(WPARAM wParam, LPARAM lParam);
    afx_msg void OnSize(UINT nType, int cx, int cy);

public:
    CObjectBar();
    virtual ~CObjectBar();
};

#endif


#ifdef CB_INLINES
#ifndef _COBJECTBAR_H_INLINES
#define _COBJECTBAR_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
