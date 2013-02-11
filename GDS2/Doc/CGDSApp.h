//-----------------------------------------------------------------------------
//
// File:          CGDSApp.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CGDSApp'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'StaticBroadcastMessage'
*     Added method 'SearchDLLS'
*     Added method 'OnOpenRecentFile'
*     Added method 'OnIdle'
*     Added method 'OnFileOpen'
*     Added method 'OnAppAbout'
*     Added method 'LoadDLLS'
*     Added method 'InitInstance'
*     Added method 'InitAllDLLS'
*     Added method 'GetRecentFileList'
*     Added method 'GetFilterStrLength'
*     Added method 'GetALLInterfaces'
*     Added method 'FindExtensionMatch'
*     Added method 'ExitInstance'
*     Added method 'BuildFilterLists'
*     Added method 'BroadcastMessage'
*     Added method '~CGDSApp'
*     Added method 'CGDSApp'
*     Added inheritance 'CWinApp'
*     Added member 'm_CurAccel'
*     Added member 'm_CurMenu'
*     Added member 'm_ProgramInfo'
*     Added member 'm_DLLHandles'
*     Added member 'm_DLLS'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _CGDSAPP_H
#define _CGDSAPP_H

//@START_USER1
//@END_USER1


/*@NOTE_3612
C:\Development\Game Dev System\gds.h(19)


C:\Development\Game Dev System\gds.h(19)

 CGDSApp:
C:\Development\Game Dev System\gds.h(19)

 See gds.cpp for the implementation of this class
C:\Development\Game Dev System\gds.h(19)


*/

class CGDSApp
    : public CWinApp
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
    STDDLLLIST m_DLLS;
    HANDLELIST m_DLLHandles;
    CProgramInfo* m_ProgramInfo;
    HMENU m_CurMenu;
    HACCEL m_CurAccel;

//
// Non-Grouped Methods
//

private:
    void GetALLInterfaces(const CString& pathName, DLLTYPE type);
    void InitAllDLLS();
    void LoadDLLS();
    void SearchDLLS(const CString& pathName, DLLTYPE type);

public:
    CGDSApp();
    virtual ~CGDSApp();
    BOOL BroadcastMessage(UINT nID, void* pExtra);
    void BuildFilterLists();
    int ExitInstance();
    CDLLInterface* FindExtensionMatch(LPTSTR ext, BOOL isLoad);
    DWORD GetFilterStrLength(LPTSTR filterStr);
    CRecentFileList* GetRecentFileList();
    BOOL InitInstance();
    afx_msg void OnAppAbout();
    afx_msg void OnFileOpen();
    BOOL OnIdle(LONG lCount);
    BOOL OnOpenRecentFile(UINT nID);
    BOOL StaticBroadcastMessage(UINT nID, void* pExtra);
};

#endif


#ifdef CB_INLINES
#ifndef _CGDSAPP_H_INLINES
#define _CGDSAPP_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
