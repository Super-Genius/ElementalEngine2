//-----------------------------------------------------------------------------
//
// File:          CPushRoutingFrame.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CPushRoutingFrame'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '~CPushRoutingFrame'
*     Added method 'CPushRoutingFrame'
*     Added member 'pThreadState'
*     Added member 'pOldRoutingFrame'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _CPUSHROUTINGFRAME_H
#define _CPUSHROUTINGFRAME_H

//@START_USER1
//@END_USER1


/*@NOTE_3896
C:\Development\Game Dev System\StdAfx.h(29)

 so we don't have to include afximpl.h which is the source directory
*/

class CPushRoutingFrame
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
    CFrameWnd* pOldRoutingFrame;
    _AFX_THREAD_STATE* pThreadState;

//
// Non-Grouped Methods
//

public:
    CPushRoutingFrame(CFrameWnd* pNewRoutingFrame);
    virtual ~CPushRoutingFrame();
};

#endif


#ifdef CB_INLINES
#ifndef _CPUSHROUTINGFRAME_H_INLINES
#define _CPUSHROUTINGFRAME_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
