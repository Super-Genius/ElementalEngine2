//-----------------------------------------------------------------------------
//
// File:          CHardwareIndexStream.h
// Creation date: November 26, 2003 14:11
// Author:        Kenneth L. Hurley
// Purpose:       Declaration of class 'CHardwareIndexStream'
//
// Modifications: @INSERT_MODIFICATIONS(* )
* November 26, 2003 14:11 Kenneth Hurley
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method 'UnlockHardwareStream'
*     Added method 'LockHardwareStream'
*     Added method '~CHardwareIndexStream'
*     Added method 'CHardwareIndexStream'
*     Added method 'CHardwareIndexStream'
*     Added inheritance 'CBaseObject'
*     Added inheritance 'CHardwareStream'
*     Added member 'm_hIndexStream'
//
// Copyright 2003, Signature Devices, Inc.
// All rights are reserved. Reproduction in whole or part is prohibited
// without the written consent of the copyright owner.
//
//-----------------------------------------------------------------------------

#ifndef _CHARDWAREINDEXSTREAM_H
#define _CHARDWAREINDEXSTREAM_H

//@START_USER1
//@END_USER1


/*@NOTE_4186
C:\Development\Game Dev System\Plugins\src\include\HardwareStream.h(52)

 {B247A198-D713-4186-AF55-710E79E8EC87}
*/

class CHardwareIndexStream
    : public CHardwareStream
    , public CBaseObject
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
    HANDLE m_hIndexStream;

//
// Non-Grouped Methods
//

public:
    CHardwareIndexStream();
    CHardwareIndexStream(LPCSTR name);
    virtual ~CHardwareIndexStream();
    void* LockHardwareStream();
    BOOL UnlockHardwareStream();
};

#endif


#ifdef CB_INLINES
#ifndef _CHARDWAREINDEXSTREAM_H_INLINES
#define _CHARDWAREINDEXSTREAM_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
