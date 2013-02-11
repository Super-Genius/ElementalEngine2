//-----------------------------------------------------------------------------
//
// File:          CDLLInterface.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CDLLInterface'
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

//@START_USER1
//@END_USER1


// Master include file
#include "StdAfx.h"


//@START_USER2
//@END_USER2




/*@NOTE_4108
Constructor method.
*/
CDLLInterface::CDLLInterface() //@INIT_4108
{//@CODE_4108

		m_Priority = 0;
		m_LoadStrings = NULL;
		m_SaveStrings = NULL;
}//@CODE_4108


/*@NOTE_4100
Destructor method.
*/
CDLLInterface::~CDLLInterface()
{//@CODE_4100
    DestructorInclude();

    // Put in your own code
}//@CODE_4100


BOOL CDLLInterface::BroadcastDLLMessage(UINT nID, void* pExtra)
{//@CODE_4120

		return FALSE;
}//@CODE_4120


DWORD CDLLInterface::Load(LPTSTR filename)
{//@CODE_4109

		return 0;
}//@CODE_4109


BOOL CDLLInterface::OnCmdMsg(UINT nID, int nCode, void* pExtra,
                             AFX_CMDHANDLERINFO* pHandlerInfo)
{//@CODE_4115

		return FALSE;
}//@CODE_4115


void CDLLInterface::OnCreate(CMDIFrameWnd* mainWnd)
{//@CODE_4113

}//@CODE_4113


BOOL CDLLInterface::OnIdle(LONG lCount)
{//@CODE_4123

        return FALSE;
}//@CODE_4123


DWORD CDLLInterface::Save(LPTSTR filename)
{//@CODE_4111

		return 0;
}//@CODE_4111


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4101
Method which must be called first in a constructor.
*/
void CDLLInterface::ConstructorInclude()
{
}


/*@NOTE_4102
Method which must be called first in a destructor.
*/
void CDLLInterface::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
