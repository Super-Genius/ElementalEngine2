//-----------------------------------------------------------------------------
//
// File:          CProgramInfo.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CProgramInfo'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'BroadcastMessage'
*     Added method '(BOOL (*srcBroadcastCBPtr)'
*     Added method '(*BroadcastCBPtr)'
*     Added method '~CProgramInfo'
*     Added member 'ObjectManager'
*     Added member 'AppDirectory'
*     Added member 'curAccel'
*     Added member 'curMenu'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _CPROGRAMINFO_H
#define _CPROGRAMINFO_H

//@START_USER1
//@END_USER1


/*@NOTE_4135
C:\Development\Game Dev System\Plugins\src\include\DLLInterface.h(112)

 pure virtual class for callback interface to program from DLLS
*/

class CProgramInfo
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

public:
    HMENU* curMenu;
    HACCEL* curAccel;
    CString AppDirectory;
    CObjectManager* ObjectManager;

//
// Non-Grouped Methods
//

private:
    BOOL (*BroadcastCBPtr)(UINT nID, void* pExtra);

public:
    virtual ~CProgramInfo();
    CProgramInfo (BOOL (*srcBroadcastCBPtr)(UINT nID, void* pExtra);
    BOOL BroadcastMessage(UINT nID, void* pExtra);
};

#endif


#ifdef CB_INLINES
#ifndef _CPROGRAMINFO_H_INLINES
#define _CPROGRAMINFO_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
