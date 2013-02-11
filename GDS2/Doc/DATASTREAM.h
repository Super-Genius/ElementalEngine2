//-----------------------------------------------------------------------------
//
// File:          DATASTREAM.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'DATASTREAM'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '~DATASTREAM'
*     Added member 'm_Description'
*     Added member 'm_Data'
*     Added member 'm_Count'
*     Added member 'm_type'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _DATASTREAM_H
#define _DATASTREAM_H

//@START_USER1
//@END_USER1



class DATASTREAM
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
    FORMATFLAGS* m_type;
    DWORD m_Count;
    void* m_Data;
    DESCRIPTIONS* m_Description;

//
// Non-Grouped Methods
//

public:
    virtual ~DATASTREAM();
};

#endif


#ifdef CB_INLINES
#ifndef _DATASTREAM_H_INLINES
#define _DATASTREAM_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
