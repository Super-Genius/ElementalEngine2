//-----------------------------------------------------------------------------
//
// File:          CDLLInterface.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CDLLInterface'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'Save'
*     Added method 'OnIdle'
*     Added method 'OnCreate'
*     Added method 'OnCmdMsg'
*     Added method 'Load'
*     Added method 'BroadcastDLLMessage'
*     Added method '~CDLLInterface'
*     Added method 'CDLLInterface'
*     Added member 'm_Type'
*     Added member 'm_SaveStrings'
*     Added member 'm_LoadStrings'
*     Added member 'm_Priority'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _CDLLINTERFACE_H
#define _CDLLINTERFACE_H

//@START_USER1
//@END_USER1



class CDLLInterface
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
    DWORD m_Priority;
    LPTSTR m_LoadStrings;
    LPTSTR m_SaveStrings;
    DLLTYPE m_Type;

//
// Non-Grouped Methods
//

public:
    CDLLInterface();
    virtual ~CDLLInterface();
    BOOL BroadcastDLLMessage(UINT nID, void* pExtra);
    DWORD Load(LPTSTR filename);
    BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
                  AFX_CMDHANDLERINFO* pHandlerInfo);
    void OnCreate(CMDIFrameWnd* mainWnd);
    BOOL OnIdle(LONG lCount);
    DWORD Save(LPTSTR filename);
};

#endif


#ifdef CB_INLINES
#ifndef _CDLLINTERFACE_H_INLINES
#define _CDLLINTERFACE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
