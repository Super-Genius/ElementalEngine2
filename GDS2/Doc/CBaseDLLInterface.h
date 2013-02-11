//-----------------------------------------------------------------------------
//
// File:          CBaseDLLInterface.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CBaseDLLInterface'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '~CBaseDLLInterface'
*     Added inheritance 'CDLLInterface'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _CBASEDLLINTERFACE_H
#define _CBASEDLLINTERFACE_H

//@START_USER1
//@END_USER1



class CBaseDLLInterface
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

//
// Non-Grouped Methods
//

public:
    virtual ~CBaseDLLInterface();
};

#endif


#ifdef CB_INLINES
#ifndef _CBASEDLLINTERFACE_H_INLINES
#define _CBASEDLLINTERFACE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
