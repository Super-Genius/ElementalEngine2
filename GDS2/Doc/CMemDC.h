//-----------------------------------------------------------------------------
//
// File:          CMemDC.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CMemDC'
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

#ifndef _CMEMDC_H
#define _CMEMDC_H

//@START_USER1
//@END_USER1


/*@NOTE_4364
C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)


C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)

 CMemDC - memory DC
C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)


C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)

 Author: Keith Rule
C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)

 Email:  keithr@europa.com
C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)

 Copyright 1996-1999, Keith Rule
C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)


C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)

 You may freely use or modify this code provided this
C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)

 Copyright is included in all derived versions.
C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)


C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)

 History - 10/3/97 Fixed scrolling bug.
C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)

                   Added print support. - KR
C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)


C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)

           11/3/99 Fixed most common complaint. Added
C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)

                   background color fill. - KR
C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)


C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)

           11/3/99 Added support for mapping modes other than
C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)

                   MM_TEXT as suggested by Lee Sang Hun. - KR
C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)


C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)

 This class implements a memory Device Context which allows
C:\Development\Game Dev System\Plugins\src\include\MemDC.h(26)

 flicker free drawing.
*/

class CMemDC
    : public CDC
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

private:
    CBitmap m_bitmap;
    CBitmap* m_oldBitmap;
    CDC* m_pDC;
    CRect m_rect;
    BOOL m_bMemDC;

//
// Non-Grouped Methods
//

public:
    CMemDC(CDC* pDC, const CRect* pRect);
    virtual ~CMemDC();
    operator CMemDC*();
    CMemDC* operator-();
};

#endif


#ifdef CB_INLINES
#ifndef _CMEMDC_H_INLINES
#define _CMEMDC_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
