//-----------------------------------------------------------------------------
//
// File:          CObjectOptions.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CObjectOptions'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'OnOK'
*     Added method 'OnCancel'
*     Added method 'OnApply'
*     Added method 'DoDataExchange'
*     Added method '~CObjectOptions'
*     Added method 'CObjectOptions'
*     Added inheritance 'CPropertyPage'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _COBJECTOPTIONS_H
#define _COBJECTOPTIONS_H

//@START_USER1
//@END_USER1


/*@NOTE_3801
C:\Development\Game Dev System\ObjectOptions.h(13)

 ObjectOptions.h : header file
C:\Development\Game Dev System\ObjectOptions.h(13)


C:\Development\Game Dev System\ObjectOptions.h(13)


C:\Development\Game Dev System\ObjectOptions.h(13)

 CObjectOptions dialog
*/

class CObjectOptions
    : public CPropertyPage
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

public:
    CObjectOptions();
    virtual ~CObjectOptions();
    BOOL OnApply();
    void OnCancel();
    void OnOK();
};

#endif


#ifdef CB_INLINES
#ifndef _COBJECTOPTIONS_H_INLINES
#define _COBJECTOPTIONS_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
