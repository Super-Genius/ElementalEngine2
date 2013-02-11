//-----------------------------------------------------------------------------
//
// File:          CAboutDlg.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CAboutDlg'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'DoDataExchange'
*     Added method '~CAboutDlg'
*     Added method 'CAboutDlg'
*     Added method 'CAboutDlg'
*     Added inheritance 'CDialog'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _CABOUTDLG_H
#define _CABOUTDLG_H

//@START_USER1
//@END_USER1


/*@NOTE_3917
C:\Development\Game Dev System\gds.cpp(241)

 First try the resources
C:\Development\Game Dev System\gds.cpp(241)

 possible message to windows
C:\Development\Game Dev System\gds.cpp(241)

 First try the resources
C:\Development\Game Dev System\gds.cpp(241)

 possible message to windows
C:\Development\Game Dev System\gds.cpp(241)


C:\Development\Game Dev System\gds.cpp(241)

 CAboutDlg dialog used for App About
*/

class CAboutDlg
    : public CDialog
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
    CAboutDlg();
    void DoDataExchange(CDataExchange* pDX);

public:
    CAboutDlg();
    virtual ~CAboutDlg();
};

#endif


#ifdef CB_INLINES
#ifndef _CABOUTDLG_H_INLINES
#define _CABOUTDLG_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
