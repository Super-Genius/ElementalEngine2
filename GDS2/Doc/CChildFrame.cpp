//-----------------------------------------------------------------------------
//
// File:          CChildFrame.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CChildFrame'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'PreCreateWindow'
*     Added method 'Dump'
*     Added method 'AssertValid'
*     Added method '~CChildFrame'
*     Added method 'CChildFrame'
*     Added inheritance 'CMDIChildWnd'
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




/*@NOTE_3603
Constructor method.
*/
CChildFrame::CChildFrame() //@INIT_3603
{//@CODE_3603

	// TODO: add member initialization code here
	
}//@CODE_3603


/*@NOTE_3597
Destructor method.
*/
CChildFrame::~CChildFrame()
{//@CODE_3597

}//@CODE_3597


void CChildFrame::AssertValid()
{//@CODE_3608

	CMDIChildWnd::AssertValid();
}//@CODE_3608


void CChildFrame::Dump(CDumpContext& dc)
{//@CODE_3609

	CMDIChildWnd::Dump(dc);
}//@CODE_3609


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{//@CODE_3605

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}//@CODE_3605


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3598
Method which must be called first in a constructor.
*/
void CChildFrame::ConstructorInclude()
{
}


/*@NOTE_3599
Method which must be called first in a destructor.
*/
void CChildFrame::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
