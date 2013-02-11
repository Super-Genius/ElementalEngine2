//-----------------------------------------------------------------------------
//
// File:          CObjectOptions.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CObjectOptions'
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

//@START_USER1
//@END_USER1


// Master include file
#include "StdAfx.h"


//@START_USER2
//@END_USER2




/*@NOTE_3808
Constructor method.
*/
CObjectOptions::CObjectOptions() //@INIT_3808
: CPropertyPage(CObjectOptions::IDD)
{//@CODE_3808

	//{{AFX_DATA_INIT(CObjectOptions)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// we can do this because we are part of gds.  Other
	// options pages should use BroadCastMessages
	AfxGetMainWnd()->SendMessage(AddOptionPageMessage, 0, (LPARAM)this);
}//@CODE_3808


/*@NOTE_3802
Destructor method.
*/
CObjectOptions::~CObjectOptions()
{//@CODE_3802
    DestructorInclude();

    // Put in your own code
}//@CODE_3802


void CObjectOptions::DoDataExchange(CDataExchange* pDX)
{//@CODE_3812

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectOptions)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}//@CODE_3812


BOOL CObjectOptions::OnApply()
{//@CODE_3809

	// do validation here.
	return CPropertyPage::OnApply();
}//@CODE_3809


void CObjectOptions::OnCancel()
{//@CODE_3811

	GetParent()->ShowWindow(SW_HIDE);
}//@CODE_3811


void CObjectOptions::OnOK()
{//@CODE_3810

	GetParent()->ShowWindow(SW_HIDE);
}//@CODE_3810


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3803
Method which must be called first in a constructor.
*/
void CObjectOptions::ConstructorInclude()
{
}


/*@NOTE_3804
Method which must be called first in a destructor.
*/
void CObjectOptions::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
