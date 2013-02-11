//-----------------------------------------------------------------------------
//
// File:          CObjectDLLInterface.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CObjectDLLInterface'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'Save'
*     Added method 'Load'
*     Added method '~CObjectDLLInterface'
*     Added inheritance 'CDLLInterface'
*     Added member 'm_ThumbNail'
*     Added member 'm_ObjInfo'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _COBJECTDLLINTERFACE_H
#define _COBJECTDLLINTERFACE_H

//@START_USER1
//@END_USER1



class CObjectDLLInterface
    : public CDLLInterface
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

public:
    OBJECTINFO m_ObjInfo;
    CBitmap* m_ThumbNail;

//
// Non-Grouped Methods
//

public:
    virtual ~CObjectDLLInterface();
    DWORD Load(LPTSTR filename);
    DWORD Save(LPTSTR filename);
};

#endif


#ifdef CB_INLINES
#ifndef _COBJECTDLLINTERFACE_H_INLINES
#define _COBJECTDLLINTERFACE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
