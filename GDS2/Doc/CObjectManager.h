//-----------------------------------------------------------------------------
//
// File:          CObjectManager.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CObjectManager'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '~CObjectManager'
*     Added method 'VECHO::GetObjects'
*     Added method 'GetHeadObject'
*     Added method 'CObjectManager::CObjectManager'
*     Added method 'CObjectManager::CObjectManager'
*     Added method 'CObjectManager::CObjectManager'
*     Added method 'CObjectManager'
*     Added method '~CObjectManager'
*     Added member 'm_ProgramInfo'
*     Added member 'm_VecHead'
*     Added member 'm_HeadObject'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _COBJECTMANAGER_H
#define _COBJECTMANAGER_H

//@START_USER1
// testing, 1.1..2.
//@END_USER1


/*@NOTE_3775
C:\Development\Game Dev System\ObjectManager.h(25)

 {6D547917-153C-4255-96A7-7F64383D70CB}
*/

class CObjectManager
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
    CBaseObject* m_HeadObject;
    VECHO m_VecHead;
    CProgramInfo* m_ProgramInfo;

//
// Non-Grouped Methods
//

public:
    virtual ~CObjectManager();
    __inline CObjectManager(CProgramInfo* ProgramInfo);
    __inline BOOL CObjectManager::CObjectManager(CBaseObject* object,
                                                 CBaseObject* parent,
                                                 DWORD before);
    __inline void CObjectManager::CObjectManager(CBaseObject* srcObj);
    __inline void CObjectManager::CObjectManager();
    __inline CBaseObject* GetHeadObject();
    __inline VECHO::iterator VECHO::GetObjects();
    __inline ~CObjectManager();
};

#endif


#ifdef CB_INLINES
#ifndef _COBJECTMANAGER_H_INLINES
#define _COBJECTMANAGER_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
