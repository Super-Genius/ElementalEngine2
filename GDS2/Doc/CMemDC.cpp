//-----------------------------------------------------------------------------
//
// File:          CMemDC.cpp
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Method implementations of class 'CMemDC'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'operator-'
*     Added method 'CMemDC*'
*     Added method '~CMemDC'
*     Added method 'CMemDC'
*     Added inheritance 'CDC'
*     Added member 'm_bMemDC'
*     Added member 'm_rect'
*     Added member 'm_pDC'
*     Added member 'm_oldBitmap'
*     Added member 'm_bitmap'
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




/*@NOTE_4377
Constructor method.
*/
CMemDC::CMemDC(CDC* pDC, const CRect* pRect) //@INIT_4377
: CDC()
	
{//@CODE_4377

		ASSERT(pDC != NULL); 

		// Some initialization
		m_pDC = pDC;
		m_oldBitmap = NULL;
		m_bMemDC = !pDC->IsPrinting();

		// Get the rectangle to draw
		if (pRect == NULL) {
			pDC->GetClipBox(&m_rect);
		} else {
			m_rect = *pRect;
		}
		
		if (m_bMemDC) {
			// Create a Memory DC
			CreateCompatibleDC(pDC);
			pDC->LPtoDP(&m_rect);

			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_oldBitmap = SelectObject(&m_bitmap);
			
			SetMapMode(pDC->GetMapMode());
			pDC->DPtoLP(&m_rect);
			SetWindowOrg(m_rect.left, m_rect.top);
		} else {
			// Make a copy of the relevent parts of the current DC for printing
			m_bPrinting = pDC->m_bPrinting;
			m_hDC       = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}

		// Fill background 
		FillSolidRect(m_rect, pDC->GetBkColor());
}//@CODE_4377


/*@NOTE_4365
Destructor method.
*/
CMemDC::~CMemDC()
{//@CODE_4365
		
		if (m_bMemDC) {
			// Copy the offscreen bitmap onto the screen.
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
				this, m_rect.left, m_rect.top, SRCCOPY);			
			
			//Swap back the original bitmap.
			SelectObject(m_oldBitmap);		
		} else {
			// All we need to do is replace the DC with an illegal value,
			// this keeps us from accidently deleting the handles associated with
			// the CDC that was passed to the constructor.			
			m_hDC = m_hAttribDC = NULL;
		}	
}//@CODE_4365


operator CMemDC::CMemDC*()
{//@CODE_4382

		return this;
}//@CODE_4382


CMemDC* CMemDC::operator-()
{//@CODE_4380

		return this;
}//@CODE_4380


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4366
Method which must be called first in a constructor.
*/
void CMemDC::ConstructorInclude()
{
}


/*@NOTE_4367
Method which must be called first in a destructor.
*/
void CMemDC::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
