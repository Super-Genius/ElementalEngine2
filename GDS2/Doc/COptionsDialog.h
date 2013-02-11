//-----------------------------------------------------------------------------
//
// File:          COptionsDialog.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'COptionsDialog'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'OnInitDialog'
*     Added method 'OnClose'
*     Added method 'DoDataExchange'
*     Added method '~COptionsDialog'
*     Added method 'COptionsDialog'
*     Added inheritance 'CPropertySheet'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _COPTIONSDIALOG_H
#define _COPTIONSDIALOG_H

//@START_USER1
//@END_USER1


/*@NOTE_3854
C:\Development\Game Dev System\OptionsDialog.h(13)

 OptionsDialog.h : header file
C:\Development\Game Dev System\OptionsDialog.h(13)


C:\Development\Game Dev System\OptionsDialog.h(13)


C:\Development\Game Dev System\OptionsDialog.h(13)

 COptionsDialog dialog
*/

class COptionsDialog
    : public CPropertySheet
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
    void DoDataExchange(CDataExchange* pDX);
    afx_msg void OnClose();
    BOOL OnInitDialog();

public:
    COptionsDialog();
    virtual ~COptionsDialog();
};

#endif


#ifdef CB_INLINES
#ifndef _COPTIONSDIALOG_H_INLINES
#define _COPTIONSDIALOG_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
