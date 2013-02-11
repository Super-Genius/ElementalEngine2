//-----------------------------------------------------------------------------
//
// File:          CHardwareStream.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CHardwareStream'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '~CHardwareStream'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _CHARDWARESTREAM_H
#define _CHARDWARESTREAM_H

//@START_USER1
//@END_USER1



class CHardwareStream
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
    virtual ~CHardwareStream();
};

#endif


#ifdef CB_INLINES
#ifndef _CHARDWARESTREAM_H_INLINES
#define _CHARDWARESTREAM_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
