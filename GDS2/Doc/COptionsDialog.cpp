//-----------------------------------------------------------------------------
//
// File:          COptionsDialog.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'COptionsDialog'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'OnInitDialog'
*     Added method 'OnClose'
*     Added method 'DoDataExchange'
*     Added method '~COptionsDialog'
*     Added method 'COptionsDialog'
*     Added inheritance 'CPropertySheet'
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




/*@NOTE_3861
Constructor method.
*/
COptionsDialog::COptionsDialog() //@INIT_3861
: 
	CPropertySheet()
{//@CODE_3861

	//{{AFX_DATA_INIT(COptionsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
}//@CODE_3861


/*@NOTE_3855
Destructor method.
*/
COptionsDialog::~COptionsDialog()
{//@CODE_3855
    DestructorInclude();

    // Put in your own code
}//@CODE_3855


void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{//@CODE_3862

	CPropertySheet::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}//@CODE_3862


afx_msg void COptionsDialog::OnClose()
{//@CODE_3864

	ShowWindow(SW_HIDE);	
}//@CODE_3864


BOOL COptionsDialog::OnInitDialog()
{//@CODE_3865

	m_bModeless = FALSE;   
	m_nFlags |= WF_CONTINUEMODAL;

	BOOL bResult = CPropertySheet::OnInitDialog();

	m_bModeless = TRUE;
	m_nFlags &= ~WF_CONTINUEMODAL;

	return bResult;  
}//@CODE_3865


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3856
Method which must be called first in a constructor.
*/
void COptionsDialog::ConstructorInclude()
{
}


/*@NOTE_3857
Method which must be called first in a destructor.
*/
void COptionsDialog::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
