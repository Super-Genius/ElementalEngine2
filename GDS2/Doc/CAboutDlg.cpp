//-----------------------------------------------------------------------------
//
// File:          CAboutDlg.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CAboutDlg'
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

//@START_USER1
//@END_USER1


// Master include file
#include "StdAfx.h"


//@START_USER2
//@END_USER2




/*@NOTE_3924
Constructor method.
*/
CAboutDlg::CAboutDlg() //@INIT_3924
{//@CODE_3924
    ConstructorInclude();

    // Put in your own code
}//@CODE_3924


/*@NOTE_3927
Constructor method.
*/
CAboutDlg::CAboutDlg() //@INIT_3927
: CDialog(CAboutDlg::IDD)
{//@CODE_3927

	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}//@CODE_3927


/*@NOTE_3918
Destructor method.
*/
CAboutDlg::~CAboutDlg()
{//@CODE_3918
    DestructorInclude();

    // Put in your own code
}//@CODE_3918


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{//@CODE_3925

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}//@CODE_3925


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3919
Method which must be called first in a constructor.
*/
void CAboutDlg::ConstructorInclude()
{
}


/*@NOTE_3920
Method which must be called first in a destructor.
*/
void CAboutDlg::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
