//-----------------------------------------------------------------------------
//
// File:          CPushRoutingFrame.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CPushRoutingFrame'
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

//@START_USER1
//@END_USER1


// Master include file
#include "StdAfx.h"


//@START_USER2
//@END_USER2




/*@NOTE_3905
Constructor method.
*/
CPushRoutingFrame::CPushRoutingFrame(CFrameWnd* pNewRoutingFrame) //@INIT_3905
{//@CODE_3905

		pThreadState = AfxGetThreadState();
		pOldRoutingFrame = pThreadState->m_pRoutingFrame;
		pThreadState->m_pRoutingFrame = pNewRoutingFrame;
}//@CODE_3905


/*@NOTE_3897
Destructor method.
*/
CPushRoutingFrame::~CPushRoutingFrame()
{//@CODE_3897
 pThreadState->m_pRoutingFrame = pOldRoutingFrame;
}//@CODE_3897


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3898
Method which must be called first in a constructor.
*/
void CPushRoutingFrame::ConstructorInclude()
{
}


/*@NOTE_3899
Method which must be called first in a destructor.
*/
void CPushRoutingFrame::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
