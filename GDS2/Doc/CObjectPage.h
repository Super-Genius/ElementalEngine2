//-----------------------------------------------------------------------------
//
// File:          CObjectPage.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CObjectPage'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'RemoveObject'
*     Added method 'OnSize'
*     Added method 'OnMouseMove'
*     Added method 'OnLButtonUp'
*     Added method 'OnInitDialog'
*     Added method 'OnBegindragObjectsList'
*     Added method 'DoDataExchange'
*     Added method 'ClearObjects'
*     Added method 'AddObject'
*     Added method '~CObjectPage'
*     Added method 'CObjectPage'
*     Added inheritance 'CPropertyPage'
*     Added member 'm_pDropWnd'
*     Added member 'm_nDropIndex'
*     Added member 'm_bDragging'
*     Added member 'm_pDragImage'
*     Added member 'm_ThumbNails'
*     Added member 'm_ObjectsList'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _COBJECTPAGE_H
#define _COBJECTPAGE_H

//@START_USER1
//@END_USER1


/*@NOTE_3815
C:\Development\Game Dev System\ObjectPage.h(13)

 ObjectPage.h : header file
C:\Development\Game Dev System\ObjectPage.h(13)


C:\Development\Game Dev System\ObjectPage.h(13)


C:\Development\Game Dev System\ObjectPage.h(13)

 CObjectPage dialog
*/

class CObjectPage
    : public CPropertyPage
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
    CImageList m_ThumbNails;
    CImageList* m_pDragImage;
    BOOL m_bDragging;
    m_nDragIndex m_nDropIndex;
    CWnd* m_pDropWnd;

public:
    CListCtrl m_ObjectsList;

//
// Non-Grouped Methods
//

protected:
    void DoDataExchange(CDataExchange* pDX);
    afx_msg void OnBegindragObjectsList(NMHDR* pNMHDR, LRESULT* pResult);
    BOOL OnInitDialog();
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnSize(UINT nType, int cx, int cy);

public:
    CObjectPage();
    virtual ~CObjectPage();
    void AddObject(OBJECTSTRUCT* rs);
    void ClearObjects();
    void RemoveObject(UINT index);
};

#endif


#ifdef CB_INLINES
#ifndef _COBJECTPAGE_H_INLINES
#define _COBJECTPAGE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
